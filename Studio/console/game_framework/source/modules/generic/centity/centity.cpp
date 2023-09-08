/*****************************************************************************
 *
 * File :     CEntity.cpp
 *
 * Abstract:   Implementation of CEntity class
 *
 *****************************************************************************
 *
 * This file is a product of Criterion Software Ltd.
 *
 * This file is provided as is with no warranties of any kind and is
 * provided without any obligation on Criterion Software Ltd. or
 * Canon Inc. to assist in its use or modification.
 *
 * Criterion Software Ltd. will not, under any
 * circumstances, be liable for any lost revenue or other damages arising
 * from the use of this file.
 *
 * Copyright (c) 2001 Criterion Software Ltd.
 * All Rights Reserved.
 *
 * RenderWare is a trademark of Canon Inc.
 *
 *****************************************************************************/
#include "precomp.h"
// Left to its own devices, the MW linker will dead strip the registrar ctors, so...
#ifdef __MWERKS__
#pragma force_active on
#endif

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "centity.h"

#include "framework/toolkits/world/clevel.h"
#include "framework/toolkits/world/helpers/worldhelper.h"
#include "framework/toolkits/world/helpers/skinhelper.h"
#include "framework/toolkits/world/factory.h"
#include "framework/toolkits/world/smartptrs/catomicptr.h"
#include "framework/toolkits/world/smartptrs/cclumpptr.h"
#include "framework/core/macros/debugmacros.h"
#include "framework/mainloop/render.h"
#include "framework/core/resourcemanager/cresourcemanager.h"
#include "framework/toolkits/animation/animhelper.h"
#include "framework/toolkits/animation/skinsafe.h"
#include "framework/toolkits/debugtools/debugtools.h"

namespace RWS
{
   namespace Generic
   {
      // If the resource attached to CEntity is an atomic an instance of 
      // CRpAtomicEntity is created to manage it.
      //
      class CRpAtomicEntity : public CAttributeHandler, public CSystemCommands, public LinearAllocationPolicy
      {
      public:
      
         CRpAtomicEntity(const CAttributePacket&attr)
         {
            RWS_FUNCTION("RWS::Generic::CRpAtomicEntity::CRpAtomicEntity");

            m_pAtomic = CreateAtomicInWorldFromResource(attr);

            RWS_ASSERT(m_pAtomic, "Failed to create atomic");

            RWS_RETURNVOID();
         }

         virtual void HandleAttributes(const CAttributePacket& attr)
         {
            RWS_FUNCTION("RWS::Generic::CRpAtomicEntity::HandleAttributes");

            m_pAtomic.HandleSystemCommands(attr);

            RWS_RETURNVOID();
         }

      private:

         CAtomicPtr m_pAtomic;
      };

      // If the resource attached to CEntity is a clump an instance of 
      // CRpClumpEntity is created to manage it.
      //
      class CRpClumpEntity: public CAttributeHandler, public CSystemCommands, public LinearAllocationPolicy
      {
      public:
      
         CRpClumpEntity(const CAttributePacket&attr)
         {
            RWS_FUNCTION("RWS::Generic::CRpClumpEntity::CRpClumpEntity");

            m_pClump = CreateClumpInWorldFromResource(attr);

            RWS_ASSERT(m_pClump, "Failed to create clump");

            // Make safe any skins in the clump, this fixes the problem of displaying
            // a clump which hasn't yet been paired with an animation.
            //
            RpHAnimHierarchy* pHierarchy = FindHAnimHierarchy(*m_pClump);
            if (pHierarchy)
            {
               RpSkin* pFirstSkin = SkinHelper::FindFirstSkin(*m_pClump);
               if (pFirstSkin) ApplyDefaultPoseToHierarchy(*pHierarchy, *pFirstSkin);
               AttachHAnimHierarchy(*m_pClump, *pHierarchy);
            }

            RWS_RETURNVOID();
         }

         virtual void HandleAttributes(const CAttributePacket& attr)
         {
            RWS_FUNCTION("RWS::Generic::CRpClumpEntity::HandleAttributes");

            m_pClump.HandleSystemCommands(attr);

            RWS_RETURNVOID();
         }

      private:

         CClumpPtr m_pClump;
      };

      // If the resource attached to CEntity is a world an instance of 
      // CRpWorldEntity is created to manage it.
      //
      class CRpWorldEntity: public CEventHandler, public CAttributeHandler, public CSystemCommands, public LinearAllocationPolicy
      {
      public:


         CRpWorldEntity(const CAttributePacket &attr) : InitCEventHandler(0), m_RpWorld(0)
         {
            RWS_FUNCTION("RWS::Generic::CRpWorldEntity:: CRpWorldEntity");

            m_RpWorld = 
               static_cast<RpWorld*>(
               const_cast<void*>(
               CSystemCommands::FindFirstResourceOfType(attr, "rwID_WORLD")));

            if (!m_RpWorld)
            {
               RWS_TRACE("Unable to find world resource");

               // Safe way to destroy a behavior if the resource is unavailable
               //
               Delete();
            }
            else
            {
                // Assume that this is the default world
                //
                CLevel::SetOpaqueWorld(m_RpWorld);

                LinkMsg(iMsgDoRender, "RwCamera*");
            }

            RWS_RETURNVOID();
         }

         ~CRpWorldEntity()
         {  
            RWS_FUNCTION("RWS::Generic::CRpWorldEntity::~CRpWorldEntity");

            // If this is the main world, set main world back to previous world.
            //
            if (CLevel::GetOpaqueWorld() == m_RpWorld)
            {
               CLevel::SetOpaqueWorld(0);
            }

            UnLinkMsg(iMsgDoRender);

            RWS_RETURNVOID();
         }

         void HandleEvents(CMsg &pMsg)
         {
            RWS_FUNCTION("RWS::Generic::CRpWorldEntity::HandleEvents");

            if (pMsg.Id == iMsgDoRender)
            {
               RwCamera *pRwCamera = reinterpret_cast<RwCamera*>(pMsg.pData);

               if (pRwCamera && m_RpWorld)
               {
                  // Calculate a new distance for the camera far plane to ensure
                  // that the entire world object is visible. This distance is
                  // calculated by calculating the distance between the bounding
                  // volumes min and max, the result is then multiplied by 1.5
                  // to retain visibility when the user moves the camera outside
                  // of the world.
                  //
                  RwV3d DiagonalVect;

                  RwV3dSub(&DiagonalVect, &m_RpWorld->boundingBox.sup, &m_RpWorld->boundingBox.inf);

                  RwCameraSetFarClipPlane( pRwCamera, RwV3dLength(&DiagonalVect)*1.5f); 

                  // Add camera to world
                  //
                  RpWorldAddCamera(m_RpWorld, pRwCamera);

                  // Clear background and z buffer, must be done before RwCameraBeginUpdate
                  //
                  const RwRGBA backgroundColor = {0, 0, 128, 255};

                  RwCameraClear(pRwCamera,
                     const_cast<RwRGBA*>(&backgroundColor),
                     rwCAMERACLEARIMAGE | rwCAMERACLEARZ);
                  
 
                  // Begin camera update
                  //
                  if (RwCameraBeginUpdate(pRwCamera))
                  {
                     // Render the world
                     //
                     RpWorldRender(m_RpWorld);

                     // End camera update
                     //                                   
                     RwCameraEndUpdate (pRwCamera);
                  }

                  // Remove Camera from world
                  //
                  RpWorldRemoveCamera(m_RpWorld, pRwCamera);
               }
            }

            RWS_RETURNVOID();
         }

      private:

            RpWorld *m_RpWorld;           /**< Pointer to world, owned by this behavior */
      };

      RWS_IMPLEMENT_CLASSID(CEntity);

      // Another special case, CEntity is used to create an instance of CRpWorldEntity, CRpClumpEntity
      // or CRpAtomicEntity depending on the asset type, we still want to use the LinearAllocationPolicy
      // so we need RWS_REGISTERCLASS to register the largest class. In practice they are all the same size
      // as they each contain one pointer but for safety we check see GetMaxClassSize.
      //
      //
#undef RWS_REGISTERCLASS
#define RWS_REGISTERCLASS(className) RWS::CClassFactory::CRegistrar\
      className##Registrar(#className,\
      className::MakeNew,\
      CEntity::GetMaxClassSize(),\
      className::UseLinearMemory())

      RWS_REGISTERCLASS(CEntity);

      // See comment RWS_REGISTERCLASS above
      //
      RwUInt32 CEntity::GetMaxClassSize(void)
      {
         RWS_FUNCTION("RWS::Generic::CEntity::MaxClassSize");
         
         // Is CRpWorldEntity the biggest
         //
         if (  sizeof(CRpWorldEntity) >= sizeof(CRpClumpEntity)
            && sizeof(CRpClumpEntity) >= sizeof(CRpAtomicEntity))
         {
            RWS_RETURN( sizeof(CRpWorldEntity) );
         }
         
         // Is CRpClumpEntity the biggest
         //
         if (  sizeof(CRpClumpEntity) >= sizeof(CRpWorldEntity)
            && sizeof(CRpWorldEntity) >= sizeof(CRpAtomicEntity))
         {
            RWS_RETURN( sizeof(CRpClumpEntity) );
         }
         
         // Is CRpAtomicEntity the biggest
         //
         if (  sizeof(CRpAtomicEntity) >= sizeof(CRpClumpEntity)
            && sizeof(CRpClumpEntity) >= sizeof(CRpWorldEntity))
         {
            RWS_RETURN( sizeof(CRpAtomicEntity) );
         }
         
         RWS_ASSERTFAIL("This should never happen, check the logic of the size tests above.");
         
         RWS_RETURN(0);
      }

      /**
      *
      *  Make new function for CEntity, usually the make new function of a 
      *  behaviour is used to create an instance of the behaviour. CEntity
      *  is the default behaviour applied to an asset when it is dragged 
      *  into the workspace. This means that CEntity is required to handle any
      *  asset that is loaded into the workspace. It does this by checking the
      *  resource type and creating an instance of CRpAtomicEntity, CRpClumpEntity
      *  or CRpWorldEntity to handle it.
      *
      *  \param attr Reference to a CAttributePacket packet.
      *
      *  \return Returns either a pointer to a CAttributeHandler if the asset type is handled, otherwise 0.
      */
      CAttributeHandler* CEntity::MakeNew(const CAttributePacket& attr)
      {
         RWS_FUNCTION("RWS::Generic::CEntity::MakeNew");

         CAttributeHandler* pResult = 0;

         // Look in the attribute packet, work out whether the first resource
         // is an atomic or a clump and create an appropriate behavior
         //
         const RWSGUID * pFirstResource = CSystemCommands::FindFirstResource(attr);

         if (pFirstResource)
         {
            const RwChar* pResourceType = 0;
            const void* pObject;
            pObject = CResourceManager::FindById(pFirstResource, &pResourceType);

            if (pResourceType)
            {
               if (!rwstrcmp("rwID_ATOMIC", pResourceType))
               {
                  pResult = ::RWS_NEW CRpAtomicEntity(attr);
               }
               else if (!rwstrcmp("rwID_CLUMP", pResourceType))
               {
                  pResult = ::RWS_NEW CRpClumpEntity(attr);
               }
               else if (!rwstrcmp("rwID_WORLD", pResourceType))
               {
                  pResult = ::RWS_NEW CRpWorldEntity(attr);
               }
            }
         }
         RWS_RETURN(pResult);
      }
   }//namespace Generic
}//namespace RWS
