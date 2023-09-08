/*****************************************************************************
*
* File :     CSystemCommands.cpp
*
* Abstract : ABC for classes that responds to RWS system commands
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
#include "csystemcommands.h"
#include "../macros/debugmacros.h"
#include "../resourcemanager/cresourcemanager.h"


namespace RWS
{
   RWS_IMPLEMENT_CLASSID(CSystemCommands);

   /**
   *
   * Finds the first 'attach resource' command and returns the ID of
   * the corresponding resource, or NULL if no such command is found.
   * Note that no checks are made with the resource manager, so there
   * is no guarantee that the returned ID corresponds to a retrievable
   * resource.
   *
   * \param rAttr Reference to a CAttributePacket.
   *
   * \return Returns a pointer to a string containing the ID of the resource.
   *
   */
   const RWSGUID * CSystemCommands::FindFirstResource( const CAttributePacket& rAttr )
   {
      RWS_FUNCTION("RWS::CSystemCommands::FindFirstResource");

      const RWSGUID * pResult = 0;

      CAttributeCommandIterator attrIt(rAttr, RWS_CLASSID_OF(CSystemCommands));
      while (!attrIt.IsFinished() && 0 == pResult)
      {
         if (CSystemCommands::CMD_AttachResource == attrIt->GetCommandId()) 
         {
            attrIt->GetCommandData(&pResult);
         }
         ++attrIt;
      }
      
      RWS_RETURN(pResult);
   }

   /**
   *
   * Finds the first 'attach resource' command to specify a resource of
   * a given type and retrieves the resource from the resource manager.
   * Returns NULL if no such command is found or if the resource cannot
   * be retrieved.
   *
   * \param rAttr Reference to a CAttributePacket.
   * \param pType Pointer to a string containing the resource type.
   *
   * \return Returns a pointer to the resource's data.
   */
   const void* CSystemCommands::FindFirstResourceOfType( const CAttributePacket& rAttr, const RwChar* pType )
   {
      RWS_FUNCTION("RWS::CSystemCommands::FindFirstResourceOfType");

      RWS_PRE(pType)

      const void* pResult = 0;

      CAttributeCommandIterator attrIt(rAttr, RWS_CLASSID_OF(CSystemCommands));

      while (!attrIt.IsFinished() && 0 == pResult)
      {
         if (CSystemCommands::CMD_AttachResource == attrIt->GetCommandId())
         {
            const RWSGUID * pResourceId = 0;
            attrIt->GetCommandData(&pResourceId);

            const RwChar *pObjectType = 0;
            const void* pObject = CResourceManager::FindById (pResourceId, &pObjectType);
            if (pObject)
            {
               RWS_ASSERT(pObjectType, "Object type of found resource (id " << pResourceId << ") is invalid");
               if (0 == rwstrcmp(pType, pObjectType)) pResult = pObject;
            }
         }

         ++attrIt;
      }
      
      RWS_RETURN(pResult);
   }

   /**
   *
   *  Update a RenderWare Gfx RwFrame object with the data stored in the CAttributeCommand.
   *
   *  \param rFrame A reference to a RenderWare Gfx RwFrame.
   *  \param rAttrCmd A reference to a CAttributeCommand containing the raw matrix.
   *
   */
   void CSystemCommands::UpdateFrame(RwFrame &rFrame, const CAttributeCommand &rAttrCmd)
   {
      RWS_FUNCTION("RWS::CSystemCommands::UpdateFrame (CAttributeCommand &)");

      RWS_PRE( CMD_LoadMatrix == rAttrCmd.GetCommandId() );
      const RwMatrix* pUnalignedMat = static_cast<const RwMatrix*>(rAttrCmd.GetAs_void_ptr());

      UpdateFrame(rFrame, pUnalignedMat);

      RWS_RETURNVOID();
   }

   /**
   *
   *  Update a RenderWare Gfx RwFrame object with the data stored in the CAttributeCommand.
   *
   *  \param frame A reference to a RenderWare Gfx RwFrame.
   *  \param pUnalignedMat A pointer to the (unaligned) new matrix to use.
   *
   */
   void CSystemCommands::UpdateFrame( RwFrame& frame, const RwMatrix *pUnalignedMat)
   {
      RWS_FUNCTION("RWS::CSystemCommands::UpdateFrame (RwMatrix *)");

      // Objects may be a child in a hierarchy we need to handle this...
      //

      RwMatrix *pMatrix = RwFrameGetMatrix(&frame);
      *pMatrix = *pUnalignedMat;
      RwMatrixOptimize(pMatrix, 0);
      RwFrameUpdateObjects(&frame);

#ifdef _DEBUG
      // Check if the Matrix is invertible, if its not or asserts due to precision
      // problems, then look at changing RwEngineSetMatrixTolerances in startup.cpp
      //
      RwMatrix *dst = RwMatrixCreate();
      RwMatrixInvert(dst, pMatrix);
      RwMatrixDestroy(dst);
#endif

      RWS_RETURNVOID();
   }
}

namespace RWS
{
   namespace RpAtomicCollisionProperties
   {
      /**
      *
      *  \ingroup RpAtomicCollisionPropertiesPlugin
      *
      *  \page RpAtomicCollisionPropertiesPluginOverview Overview
      *
      *  RpAtomicCollisionProperties is a RenderWare Graphics plug-in that
      *  adds extra collision property data to a RpAtomic. The extra data
      *  is a flag which is used to tell whether a RpAtomic should be
      *  considered as solid or not.
      *
      *  \note The RpAtomic and RpClump helper functions set the solid flag
      *        to the value of the CMD_SetSolidFlag attribute of 
      *        RWS::CSystemCommands.
      *  
      *        \see RWS::AtomicHelper::HandleSystemCommands
      *        \see RWS::ClumpHelper::HandleSystemCommands
      *
      */
      namespace
      {

         /**
         *  \ingroup RpAtomicCollisionPropertiesPlugin
         *
         *  The extra data to be attached and managed by the plug-in
         */
         struct PluginData
         {
            RwUInt32 m_isSolid:1;
         };

         /**
         *  \ingroup RpAtomicCollisionPropertiesPlugin
         *
         *  Offset of extra data within RpAtomic
         */
         unsigned int iPlugin_Offset = 0;

         /**
         *  \ingroup RpAtomicCollisionPropertiesPlugin
         *
         *  Retrieves Atomic Plug-in data.
         *
         *  \param atomic Reference to an RpAtomic object.
         *
         *  \return A pointer to the RpAtomicCollisionPropertiesPluginData data.
         */
         inline PluginData* GetPluginData( RpAtomic& atomic )
         {
            RWS_FUNCTION("RWS::RpAtomicCollisionProperties::GetPluginData");

            RWS_PRE(iPlugin_Offset);

            const RwUInt32 baseAddr = reinterpret_cast<RwUInt32>(&atomic);
            const RwUInt32 pluginAddr = baseAddr + iPlugin_Offset;

            RWS_RETURN( reinterpret_cast<PluginData*>(pluginAddr) );
         }

         /**
         *  \ingroup RpAtomicCollisionPropertiesPlugin
         *
         *  Retrieves Atomic Plug-in data, const version.
         *
         *  \param atomic Reference to an RpAtomic object.
         *
         *  \return A pointer to the RpAtomicCollisionPropertiesPluginData data.
         */
         inline const PluginData *GetPluginData( const RpAtomic& atomic )
         {
            RWS_FUNCTION("RWS::RpAtomicCollisionProperties::GetPluginData");

            RWS_PRE(iPlugin_Offset);

            const RwUInt32 baseAddr = reinterpret_cast<RwUInt32>(&atomic);
            const RwUInt32 pluginAddr = baseAddr + iPlugin_Offset;

            RWS_RETURN( reinterpret_cast<const PluginData*>(pluginAddr) );
         }

         /**
         *  \ingroup RpAtomicCollisionPropertiesPlugin
         *
         *  Constructor for RpAtomicCollisionPropertiesPlugin data.
         *
         *  \param pObject A pointer to the object to be constructed, the RpAtomic.
         *  \param offsetInObject A byte offset from object pointer to plug-in data.
         *  \param sizeInObject The size of the plug-in data.
         *
         *  \returns Returns pointer to the constructed object.
         */
         void* Plugin_Constructor(void* pObject, RwInt32 offsetInObject, RwInt32 sizeInObject)
         {
            RWS_FUNCTION("RWS::RpAtomicCollisionProperties::Plugin_Constructor");

            RWS_PRE(pObject);

            PluginData& pluginData = *GetPluginData(*(RpAtomic*)pObject);
            pluginData.m_isSolid = 1;      // Solid by default

            RWS_RETURN(pObject);
         }

         /**
         *  \ingroup RpAtomicCollisionPropertiesPlugin
         *
         *  Copy operator for RpAtomicCollisionPropertiesPlugin.
         *
         *  \param pDstObject A void pointer to the destination object, the source RpAtomic.
         *  \param pSrcObject A void pointer to the destination object, the destination RpAtomic.
         *  \param offset A byte offset from object pointers to plug-in data.
         *  \param size The size of the plug-in data to copy.
         */
         void* Plugin_Copy(void* pDstObject, const void* pSrcObject, RwInt32 offset, RwInt32 size)
         {
            RWS_FUNCTION("RWS::RpAtomicCollisionProperties::Plugin_Copy");

            RWS_PRE(pDstObject);
            RWS_PRE(pSrcObject);

            PluginData& srcData = *GetPluginData(*(RpAtomic*)pSrcObject);
            PluginData& dstData = *GetPluginData(*(RpAtomic*)pDstObject);

            dstData = srcData;

            RWS_RETURN(pDstObject);
         }

      } // namespace <anonymous>


      /**
      *  \ingroup RpAtomicCollisionPropertiesPlugin
      *
      *  Unique ID of the RpAtomicCollisionPropertiesPlugin used by RenderWare to identify the plug-in
      *
      */
      const RwUInt32 PluginID = 0xFE;

      /**
      *  \ingroup RpAtomicCollisionPropertiesPlugin
      *
      *  Attaches the RenderWare Studio Framework collision data 
      *  - RpAtomic plug-in - this must happen AFTER the RenderWare engine is
      *  initialized but BEFORE it is opened. Returns false if the plug-in fails to
      *  attach.
      *
      *  \returns TRUE if successful otherwise FALSE.
      */
      RwBool PluginAttach(void)
      {
         RWS_FUNCTION("RWS::RpAtomicCollisionProperties::PluginAttach");

         iPlugin_Offset = RpAtomicRegisterPlugin(
            sizeof(PluginData),
            MAKECHUNKID(rwVENDORID_CRITERIONRM, PluginID),
            Plugin_Constructor,  
            0, // NOT USED
            Plugin_Copy
            );

         RWS_POST(iPlugin_Offset);

         RWS_RETURN (0 != iPlugin_Offset);
      }

      /**
      *  \ingroup RpAtomicCollisionPropertiesPlugin
      *
      *  Sets the solidity of the atomic.
      *
      *  \param atomic A reference to an RpAtomic object.
      *  \param isSolid Flag that specifies the new value of the solid flag.
      */
      void SetIsSolid( RpAtomic& atomic, bool isSolid )
      {
         RWS_FUNCTION("RWS::RpAtomicCollisionProperties::SetIsSolid");

         RWS_PRE( iPlugin_Offset );

         GetPluginData(atomic)->m_isSolid = isSolid?1:0;

         RWS_RETURNVOID();
      }

      /**
      *  \ingroup RpAtomicCollisionPropertiesPlugin
      *
      *  Retrieves the solidity of the atomic
      *
      *  \param atomic A reference to an RpAtomic object.
      *
      *  \returns Returns contents of solid flag.
      */
      bool GetIsSolid( const RpAtomic& atomic )
      {
         RWS_FUNCTION("RWS::RpAtomicCollisionProperties::GetIsSolid");

         RWS_PRE( iPlugin_Offset );
      
         RWS_RETURN(GetPluginData(atomic)->m_isSolid?true:false);
      }


      /*
      *
      *  \ingroup RpAtomicCollisionPropertiesPlugin
      *
      *  Sets the solidity of the atomic
      *
      */
      RpAtomic* SetIsSolidCallback(RpAtomic* pAtomic, void* pData)
      {
         RWS_FUNCTION("RWS::RpAtomicCollisionProperties::SetIsSolidCallback");
         RWS_PRE(pAtomic);

         RpAtomicCollisionProperties::SetIsSolid(*pAtomic, (0 != pData));

         RWS_RETURN(pAtomic);
      }

      /**
      *
      *  \ingroup RpAtomicCollisionPropertiesPlugin
      *
      *  Sets the solidity of all the atomics in the clump
      *
      *  \param clump A reference to an RpClump object.
      *  \param isSolid Flag if true implies object is solid during collisions.
      */
      void SetIsSolid( RpClump& clump, bool isSolid )
      {
         RWS_FUNCTION("RWS::RpAtomicCollisionProperties::SetIsSolid");

         RpClumpForAllAtomics(&clump, SetIsSolidCallback, isSolid ? reinterpret_cast<void*>(1) : 0);

         RWS_RETURNVOID();
      }

     /*
      *
      *  \ingroup RpAtomicCollisionPropertiesPlugin
      *
      *  Gets the solidity of the atomic
      *
      */
      RpAtomic* GetIsSolidCallback(RpAtomic* pAtomic, void* pData)
      {
         RWS_FUNCTION("RWS::RpAtomicCollisionProperties::SetIsSolidCallback");

         RWS_PRE(pAtomic);

         bool *ret = reinterpret_cast<bool*>(pData);

         if (GetIsSolid(*pAtomic))
         {
            *ret = true;
         }

         RWS_RETURN(pAtomic);
      }

      /**
      *  \ingroup RpAtomicCollisionPropertiesPlugin
      *
      *  Retrieves the solidity of the atomic
      *
      *  \param clump A reference to an RpClump object.
      *
      *  \returns Returns true if any of the atomics within the clump are solid
      */
      bool GetIsSolid( const RpClump& clump )
      {
         RWS_FUNCTION("RWS::RpAtomicCollisionProperties::GetIsSolid");

         bool ret = false;

         RpClumpForAllAtomics(const_cast<RpClump*>(&clump), GetIsSolidCallback, &ret);

         RWS_RETURN(ret);
      }
   } // namespace RpAtomicCollisionProperties
}

