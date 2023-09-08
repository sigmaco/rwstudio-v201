/*****************************************************************************
 *
 * File :     CFXBaseEnvironmentMap.cpp
 *
 * Abstract : A behavior that allows control over RenderWare materials
 *            that contain environment maps
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
 * Copyright (c) 2000 Criterion Software Ltd.
 * All Rights Reserved.
 *
 * RenderWare is a trademark of Canon Inc.
 *
 *****************************************************************************/
#include "precomp.h"

// Left to its own devices, the MW linker will dead-strip the registrar ctors, so...
#ifdef __MWERKS__
#pragma force_active on
#endif

#include "rpmatfx.h"

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "cfxbaseenvironmentmap.h"

#include "framework/core/macros/debugmacros.h"
#include "framework/toolkits/world/clevel.h"
#include "framework/toolkits/math/maths.h"
#include "framework/mainloop/render.h"
#include "framework/core/resourcemanager/cresourcemanager.h"

namespace RWS
{

   namespace FX
   {

      RwFrame *CFXBaseEnvironmentMap::m_DefaultEnvFrame = 0;


      /**
      *  \ingroup Mod_FX
      *  This structure is used to store information required by the CFXBaseEnvironmentMap::MaterialCallback function.
      */
      typedef struct
      {
         RwFrame                 *EnvMapFrame;  /**< Pointer to frame used by the materials environment map */
         CFXBaseEnvironmentMap   *EnvMapObject; /**< Pointer to CFXBaseEnvironmentMap class  */
      }t_MaterialCallBackData;


      /**
      *
      * Checks to see if the material has either rpMATFXEFFECTENVMAP or rpMATFXEFFECTBUMPENVMAP set,
      * if the material contains a environment map the materials frame is set to EnvMapFrame.
      * A pointer to the environment map object is stored in the material and the material is added
      * to a list of materials using the environment map behavior.
      *
      * \param pMaterial     Pointer to an RpMaterial object.
      * \param pCallBackData Pointer to a t_MaterialCallBackData structure containing a pointer to a frame
      *                      and a pointer to a CFXBaseEnvironmentMap object.
      * \return              Returns pointer to an RpMaterial object. 
      *
      */
      RpMaterial *CFXBaseEnvironmentMap::MaterialCallback(RpMaterial *pMaterial, void *pCallBackData)
      {
         RWS_FUNCTION("RWS::FX::CFXBaseEnvironmentMap::MaterialCallback")

         RpMatFXMaterialFlags  Flags;
         RwFrame              *pMatFrame;

         pMatFrame = reinterpret_cast<t_MaterialCallBackData*>(pCallBackData)->EnvMapFrame;

         Flags = RpMatFXMaterialGetEffects(pMaterial);

         if ( Flags == rpMATFXEFFECTENVMAP || Flags == rpMATFXEFFECTBUMPENVMAP )
         {
            RpMaterial            *Res;         
            CFXBaseEnvironmentMap *pEnvMapObject;

            pEnvMapObject = reinterpret_cast<t_MaterialCallBackData*>(pCallBackData)->EnvMapObject;

            Res = RpMatFXMaterialSetEnvMapFrame(pMaterial, pMatFrame);

            RWS_ASSERT(Res, "RpMatFXMaterialSetEnvMapFrame failed");

            // tell the material event plug-in that this behaviour would
            // like to be informed when the material is deleted
            CMaterialEvent::SetObjectToReceiveEvent(pMaterial, pEnvMapObject);
            // if this is not the default environment map frame then EnvMapObject pointer will be valid
            // so we need to add the current material to the list of materials that contain environment maps
            if (pEnvMapObject)
            {
               pEnvMapObject->m_MatList.AddToList(pMaterial);
            }
            
         }

         RWS_RETURN( pMaterial );
      }

      /**
      *
      * Gets the atomics geometry and calls the Geometry callback for every material
      * \param pAtomic      Pointer to an RpAtomic object.
      * \param pEnvMapFrame Pointer to an environment map frame
      * \return             Returns pointer to an RpAtomic object. 
      *  
      *
      */
      RpAtomic *CFXBaseEnvironmentMap::AtomicCallback(RpAtomic *pAtomic, void *pCallBackData)
      {
         RWS_FUNCTION("RWS::FX::CFXBaseEnvironmentMap::AtomicCallback")

         RpGeometry *pGeom;
         pGeom = RpAtomicGetGeometry( pAtomic );

         if (pGeom)
         { 
            RpGeometry *pRes;
            pRes = RpGeometryForAllMaterials (pGeom, MaterialCallback, pCallBackData); 
            RWS_ASSERT(pRes, "RpGeometryForAllMaterials failed");
         }

         RWS_RETURN( pAtomic );
      }

      /**
      *
      * For every atomic in the world sector a call is made to CFXBaseEnvironmentMap::AtomicCallback
      * \param pWorldSector  Pointer to an RpWorldSector object.
      * \param pCallBackData Pointer to an environment map frame
      * \return              Returns pointer to an RpWorldSector object. 
      *  
      *
      */
      RpWorldSector *CFXBaseEnvironmentMap::WorldSectorCallback(RpWorldSector *pWorldSector, void *pCallBackData)
      {
         RWS_FUNCTION("RWS::FX::CFXBaseEnvironmentMap::WorldSectorCallback")

         RpWorldSectorForAllAtomics(pWorldSector, CFXBaseEnvironmentMap::AtomicCallback, pCallBackData);

         RWS_RETURN( pWorldSector );

      }

      /**
      *
      *  Calls AtomicCallBack for every atomic in the clump
      * \param pClump         Pointer to an RpClump object.
      * \param pCallBackData  Pointer to an environment map frame.
      * \return               Returns pointer to an RpClump object. 
      *  
      *
      */
      RpClump * CFXBaseEnvironmentMap::ClumpCallback(RpClump *pClump, void *pCallBackData)
      {
         RWS_FUNCTION("RWS::FX::CFXBaseEnvironmentMap::ClumpCallback")

         RpClump *pRes;
         pRes = RpClumpForAllAtomics (pClump, AtomicCallback, pCallBackData); 

         RWS_ASSERT(pRes, "RpClumpForAllAtomics failed");

         RWS_RETURN( pClump );
      }

      /**
      *
      *  Checks each material associated with "atomic" to see if the material contains an environment map
      *  If a environment map is found the materials environment map frame is set to "EnvMapFrame"
      * \param pObject        Pointer to CFXBaseEnvironmentMap object;
      * \param pAtomic        Pointer to an RpAtomic object.
      * \param pEnvMapFrame   Pointer to an environment map frame.
      *
      */
      void CFXBaseEnvironmentMap::SetMatFXEnvFrameForAtomic(CFXBaseEnvironmentMap *pObject, const RpAtomic *pAtomic,
                                                            RwFrame *pEnvMapFrame)
      {
         RWS_FUNCTION("RWS::FX::CFXBaseEnvironmentMap::SetMatFXEnvFrameForAtomic")

         RWS_ASSERT(pAtomic, "NULL atomic ptr");

         RWS_ASSERT(pEnvMapFrame, "environment map frame has not been setup");

         RpGeometry *pGeom;
         pGeom = RpAtomicGetGeometry( pAtomic );

         if ( pGeom )
         { 
            // fill in the callback data
            RpGeometry              *pRes;
            t_MaterialCallBackData  CallBackData;
            CallBackData.EnvMapFrame  = pEnvMapFrame;// environment map frame
            CallBackData.EnvMapObject = pObject;     // environment map object
            pRes = RpGeometryForAllMaterials (pGeom, MaterialCallback, &CallBackData); 
            RWS_ASSERT(pRes, "RpGeometryForAllMaterials failed");
         }

         RWS_RETURNVOID();
      }

      /**
      *
      *  Sets every materials environment map frame that is associated with "clump" and contains an environment map
      *  to "EnvMapFrame"
      * \param pObject        Pointer to CFXBaseEnvironmentMap object;
      * \param pClump         Pointer to an RpClump object.
      * \param pEnvMapFrame   Pointer to an environment map frame.
      *
      */
      void CFXBaseEnvironmentMap::SetMatFXEnvFrameForClump(CFXBaseEnvironmentMap *pObject, const RpClump *pClump,
                                                           RwFrame  *pEnvMapFrame)
      {
         RWS_FUNCTION("RWS::FX::CFXBaseEnvironmentMap::SetMatFXEnvFrameForClump")

         RWS_ASSERT(pEnvMapFrame, "environment map frame has not been setup");


         RpClump *pRes;
         t_MaterialCallBackData  CallBackData;
         CallBackData.EnvMapFrame  = pEnvMapFrame;
         CallBackData.EnvMapObject = pObject;
         pRes = RpClumpForAllAtomics (const_cast<RpClump*>(pClump), AtomicCallback, &CallBackData); 
         RWS_ASSERT(pRes, "RpClumpForAllAtomics failed");


         RWS_RETURNVOID();
      }

      /**
      *
      *  Sets every materials environment map frame that is associated with "world" and contains an environment map
      *  to "EnvMapFrame"
      * \param pObject        Pointer to CFXBaseEnvironmentMap object;
      * \param pWorld         Pointer to an RpWorld object.
      * \param pEnvMapFrame   Pointer to an environment map frame.
      *
      */
      void CFXBaseEnvironmentMap::SetMatFXEnvFrameForWorld(CFXBaseEnvironmentMap *pObject, const RpWorld *pWorld,
                                                           RwFrame  *pEnvMapFrame)
      {
         RWS_FUNCTION("RWS::FX::CFXBaseEnvironmentMap::SetMatFXEnvFrameForWorld")

         RWS_ASSERT(pEnvMapFrame, "environment map frame has not been setup");

         RpWorld *pRes;

         t_MaterialCallBackData  CallBackData;
         CallBackData.EnvMapFrame  = pEnvMapFrame;// environment map frame 
         CallBackData.EnvMapObject = pObject;    // environment map object
         pRes = RpWorldForAllMaterials(const_cast<RpWorld*>(pWorld), MaterialCallback, &CallBackData);
         RWS_ASSERT(pRes, "RpWorldForAllMaterials failed");

         RWS_RETURNVOID();
      }


      /**
      *
      *  Sets the atomics associated environment map materials to the default environment map frame
      * \param pObject        Pointer to CFXBaseEnvironmentMap object;
      * \param pAtomic        Pointer to an RpAtomic object.
      *
      */
      void CFXBaseEnvironmentMap::SetMatFXDefaultEnvFrameForAtomic(CFXBaseEnvironmentMap *pObject,
                                                                   const RpAtomic *pAtomic)
      {
         RWS_FUNCTION("RWS::FX::CFXBaseEnvironmentMap::SetMatFXDefaultEnvFrameForAtomic")

         RWS_ASSERT( GetDefaultEnvFrame(), "Default environment map frame has not been created");
         SetMatFXEnvFrameForAtomic(pObject, pAtomic,  GetDefaultEnvFrame());

         RWS_RETURNVOID();
      }

      /**
      *
      *  Sets the clumps associated environment map materials to the default environment map frame
      * \param pObject        Pointer to CFXBaseEnvironmentMap object;
      * \param pClump         Pointer to an RpClump object.
      *
      */
      void CFXBaseEnvironmentMap::SetMatFXDefaultEnvFrameForClump(CFXBaseEnvironmentMap *pObject, const RpClump *pClump)
      {
         RWS_FUNCTION("RWS::FX::CFXBaseEnvironmentMap:SetMatFXDefaultEnvFrameForClump:")

         RWS_ASSERT( GetDefaultEnvFrame(), "Default environment map frame has not been created");
         SetMatFXEnvFrameForClump(pObject, pClump,  GetDefaultEnvFrame());

         RWS_RETURNVOID();
      }

      /**
      *
      *  Sets the worlds associated environment map materials to the default environment map frame
      * \param pObject       Pointer to CFXBaseEnvironmentMap object;
      * \param pWorld        Pointer to an RpWorld object.
      *
      */
      void CFXBaseEnvironmentMap::SetMatFXDefaultEnvFrameForWorld(CFXBaseEnvironmentMap *pObject, const RpWorld *pWorld)
      {
         RWS_FUNCTION("RWS::FX::CFXBaseEnvironmentMap::SetMatFXDefaultEnvFrameForWorld")

         RWS_ASSERT( GetDefaultEnvFrame(), "Default environment map frame has not been created");
         SetMatFXEnvFrameForWorld(pObject, pWorld, GetDefaultEnvFrame());

         RWS_RETURNVOID();
      }

      /**
      *
      *  Returns the default environment map frame.  If the frame does not exist then a frame will be created.
      *  \return     Returns a pointer to the default environment map frame.
      *
      */
      RwFrame *CFXBaseEnvironmentMap::GetDefaultEnvFrame()
      {
         RWS_FUNCTION("RWS::FX::CFXBaseEnvironmentMap::GetDefaultEnvFrame")

         if (!m_DefaultEnvFrame)
         {
            m_DefaultEnvFrame = RwFrameCreate();
         }

         RWS_RETURN(m_DefaultEnvFrame);
      }

      /**
      *
      *  Deletes the environment map frame that is used by default on all materials containing environment maps
      *
      */
      void CFXBaseEnvironmentMap::DeleteMatFXDefaultEnvFrame(void)
      {
         RWS_FUNCTION("RWS::FX::CFXBaseEnvironmentMap::DeleteMatFXDefaultEnvFrame")

         if (m_DefaultEnvFrame)
         {
            RwFrameDestroy (m_DefaultEnvFrame);
            m_DefaultEnvFrame = 0;
         }

         RWS_RETURNVOID();
      }

      /**
      *
      *  Construct a CFXBaseEnvironmentMap object.
      *
      */
      CFXBaseEnvironmentMap::CFXBaseEnvironmentMap(void)
      {
         RWS_FUNCTION("RWS::FX::CFXBaseEnvironmentMap::CFXBaseEnvironmentMap");

         RWS_RETURNVOID();
      }

      /**
      *
      *  Destroy a CFXBaseEnvironmentMap object.
      *
      */
      CFXBaseEnvironmentMap::~CFXBaseEnvironmentMap(void) 
      {
         RWS_FUNCTION("RWS::FX::CFXBaseEnvironmentMap::~CFXBaseEnvironmentMap")

         t_MaterialCallBackData  CallBackData;

         // set the default environment map frame
         CallBackData.EnvMapFrame  = GetDefaultEnvFrame();
         // as this is the default environment map frame we are setting then we don't have a owner
         // environment map behaviour, so we set EnvMapObject to be null 
         CallBackData.EnvMapObject = 0;

         // set the default environment map frame for every material
         m_MatList.ForAllMaterials(MaterialCallback, &CallBackData);

         // delete the list of materials attached to this behaviour
         m_MatList.DeleteList();

         RWS_RETURNVOID();

      }

      /**
      *
      *  Attachs the environment map behaviors frame to the frame of any Assets referenced by the attribute packet attr.
      * \param attr        Attribute packet.
      * \param pEnvFrame   Pointer to an environment map frame.
      *
      */
      void CFXBaseEnvironmentMap::ProcessAttachments(const CAttributePacket& attr, RwFrame *pEnvFrame)
      {
         RWS_FUNCTION("RWS::FX::CFXBaseEnvironmentMap::ProcessAttachments")

         // check the system commands to see if a new atomic, clump or world asset has been dropped onto the
         // the current entity. The asset will then be linked to the current environment map entity so that
         // attributes changed in the current entity will effect the environment maps on the assets that have been
         // linked
         CAttributeCommandIterator sysattrIt (attr, RWS_CLASSID_OF (CSystemCommands));
         while (!sysattrIt.IsFinished())
         {
            switch (sysattrIt->GetCommandId())
            {
               case CSystemCommands::CMD_AttachResource:
                  {
                     const RWSGUID * pResourceId;
                     sysattrIt->GetCommandData(&pResourceId);

                     const RwChar *resourceType;
      
                     const void *pObject = CResourceManager::FindById( pResourceId, &resourceType);
      
                     RWS_ASSERT (pObject, "Resource::Find failed for id = " << pResourceId);

                     if (resourceType)
                     {
                        if (!rwstrcmp("rwID_ATOMIC", resourceType))
                        {
                           SetMatFXEnvFrameForAtomic(this, reinterpret_cast<const RpAtomic *>(pObject), pEnvFrame);
                        }
                        else if (!rwstrcmp("rwID_CLUMP", resourceType))
                        {
                           SetMatFXEnvFrameForClump(this, reinterpret_cast<const RpClump *>(pObject),   pEnvFrame);
                        }
                        else if (!rwstrcmp("rwID_WORLD", resourceType))
                        {
                           SetMatFXEnvFrameForWorld(this, reinterpret_cast<const RpWorld *>(pObject),   pEnvFrame);
                        }
                     }
                  }
                  break;
            }; 
            ++sysattrIt;
         }

         RWS_RETURNVOID();
      }

      /**
      *
      *  Called when a material is constructed
      * \param pMaterial   Pointer to an pMaterial object.
      *
      */
      void CFXBaseEnvironmentMap::MaterialConstructEvent(const RpMaterial *pMaterial)
      {
         RWS_FUNCTION("RWS::FX::CFXBaseEnvironmentMap::MaterialConstructEvent")

         RWS_RETURNVOID();
      }

      /**
      *
      *  Called when a material is copied
      * \param pSrcMaterial   Pointer to the source material.
      * \param pDstMaterial   Pointer to the destination material.
      *
      */
      void CFXBaseEnvironmentMap::MaterialCopyEvent(const RpMaterial *pSrcMaterial, const RpMaterial *pDstMaterial)
      {
         RWS_FUNCTION("RWS::FX::CFXBaseEnvironmentMap::MaterialCopyEvent")

         RWS_RETURNVOID();
      }

      /**
      *
      *  Called when a material is destroyed
      * \param pMaterial        Pointer to a pMaterial object.
      *
      */
      void CFXBaseEnvironmentMap::MaterialDestroyEvent(const RpMaterial *pMaterial)
      {
         RWS_FUNCTION("RWS::FX::CFXBaseEnvironmentMap::MaterialDestroyEvent")

         m_MatList.DeleteFromMaterialList(pMaterial);

         RWS_RETURNVOID();
      }

   } // namespace FX
} //namespace RWS


