/*****************************************************************************
*
* File :     CLevel.h
*
* Abstract : Implementation of CLevel
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

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "clevel.h"
#include "framework/core/macros/debugmacros.h"

namespace RWS
{
   RpWorld  *CLevel::sm_pOpaqueWorld = 0;
   RpWorld  *CLevel::sm_pTranslucentWorld = 0;
   RpWorld  *CLevel::sm_pActiveWorld = 0;

   namespace 
   {
      const RwUInt32 CLevelGeometryPluginID = 0xFD;

      RwUInt32 iCLevelGeometryPlugin_Offset = 0;

      struct CLevelGeometryPlugin
      {
         // Hint is a pointer to the pointer to the preferred world, this is
         // done for efficiency when adding objects to the world. The 
         // alternative would be to store CLevel::CLevelHint but this 
         // would need to be resolved when objects are added.
         //
         RpWorld **m_PreferredWorld; 
      };

      /**
      *  Retrieves Geometry Plug-in data which is used to store the level hint.
      *
      *  \param geometry Reference to an RpGeometry object.
      *
      *  \return A pointer to the CLevelGeometryPlugin data.
      */
      CLevelGeometryPlugin* GetPluginData( RpGeometry& geometry )
      {
         RWS_PRE(iCLevelGeometryPlugin_Offset);

         const RwUInt32 baseAddr = reinterpret_cast<RwUInt32>(&geometry);
         const RwUInt32 pluginAddr = baseAddr + iCLevelGeometryPlugin_Offset;

         return reinterpret_cast<CLevelGeometryPlugin*>(pluginAddr);
      }
   }

   /**
   *  Constructor for CLevelAtomicPlugin data.
   *
   *  \param pObject A pointer to the object to be constructed, the RpAtomic.
   *  \param offsetInObject A byte offset from object pointer to plug-in data.
   *  \param sizeInObject The size of plug-in data.
   *
   *  \returns Returns pointer to the constructed object.
   */
   void* CLevel::Plugin_Constructor(void* pObject, 
                                    RwInt32 iOffsetInObject,
                                    RwInt32 iSizeInObject)
   {
      RWS_FUNCTION("RWS::CLevel::RpAtomicToEventHandler::Plugin_Constructor");

      RWS_PRE(pObject);

      CLevelGeometryPlugin& pluginData = *GetPluginData(*(RpGeometry*)pObject);
      pluginData.m_PreferredWorld = 0;

      RWS_RETURN(pObject);
   }

   /**
   *  Copy operator for CLevelAtomicPlugin.
   *
   *  \param pDstObject A void pointer to the destination object, the source 
   *  RpAtomic.
   *
   *  \param pSrcObject A void pointer to the destination object, the 
   *  destination RpAtomic.
   *
   *  \param offset A byte offset from object pointers to plug-in data.
   *  \param size The size of plug-in data to copy.
   *
   *  \return Returns pDstObject.
   */
   void* CLevel::Plugin_Copy( void* pDstObject,
                              const void* pSrcObject,
                              RwInt32 iOffset, RwInt32 iSize)
   {
      RWS_FUNCTION("RWS::CLevel::RpAtomicToEventHandler::Plugin_Copy");

      RWS_PRE(pDstObject);
      RWS_PRE(pSrcObject);

      CLevelGeometryPlugin& srcData = *GetPluginData(*(RpGeometry*)pSrcObject);
      CLevelGeometryPlugin& dstData = *GetPluginData(*(RpGeometry*)pDstObject);

      dstData = srcData;

      RWS_RETURN(pDstObject);
   }

   /**
   *  Attaches the CLevel RpAtomic plug-in - this must 
   *  happen AFTER the RenderWare engine is initialized but BEFORE it is 
   *  opened. 
   *
   *  Returns FALSE if the plug-in fails to attach.
   *
   *  \returns TRUE if successful otherwise FALSE.
   */
   RwBool CLevel::PluginAttach(void)
   {
      RWS_FUNCTION("RWS::CLevel::CLevel::PluginAttach");

      iCLevelGeometryPlugin_Offset = RpGeometryRegisterPlugin (
            sizeof(CLevelGeometryPlugin),
            MAKECHUNKID(rwVENDORID_CRITERIONRM, CLevelGeometryPluginID),
            Plugin_Constructor,  
            0, // NOT USED
            Plugin_Copy
            );

      RWS_POST(iCLevelGeometryPlugin_Offset);

      RWS_RETURN(0 != iCLevelGeometryPlugin_Offset);
   }

   namespace
   {
      /*
      *
      *  Internal function used to obtain pluginData.m_PreferredWorld for an
      *  atomic which is a pointer to a pointer to the preferred world.
      *
      */
      RpWorld **PreferredWorldPtr(const RpAtomic * const pAtomic)
      {
         RWS_FUNCTION("RWS::NULL::PreferredWorldPtr(RpAtomic *pAtomic)");

         RpGeometry *pGeometry = RpAtomicGetGeometry(pAtomic);

         CLevelGeometryPlugin& pluginData = *GetPluginData(*pGeometry);

         if (pluginData.m_PreferredWorld) 
         {
            // Return a pointer to a pointer to the world
            //
            RWS_RETURN(pluginData.m_PreferredWorld);   
         }

         RWS_RETURN(0);
      }
   }

   /**
   *
   *  Determine the preferred world of an atomic.
   *
   *  \param pAtomic Pointer to a RenderWare Gfx RpAtomic object.
   *
   *  \returns Returns a pointer to the preferred world.
   *
   *  \see CLevel::AddAtomic
   */
   RpWorld *CLevel::GetPreferredWorldPtr(const RpAtomic * const pAtomic)
   {
      RWS_FUNCTION("RWS::CLevel::GetPreferredWorldPtr(RpAtomic *pAtomic)");

      RpWorld **ppWorld = PreferredWorldPtr(pAtomic);

      if (ppWorld) 
      {
         RWS_RETURN(*ppWorld);   // Return a pointer to the world
      }

      RWS_RETURN(0);
   }

   /**
   *  Return the preferred world hint of an atomic.
   *
   *  \param pAtomic Pointer to a RenderWare Gfx RpAtomic object.
   *
   *  \returns Returns a \ref CLevel::CLevelHint hint.
   */
   CLevel::CLevelHint CLevel::GetPreferredWorldHint(const RpAtomic * const pAtomic)
   {
      RWS_FUNCTION("RWS::CLevel::GetPreferredWorldHint(const RpAtomic * const pAtomic);");

      RpWorld **pWorld = PreferredWorldPtr(pAtomic);

      if (pWorld == &sm_pOpaqueWorld) 
      {
         RWS_RETURN(HINT_OPAQUE_WORLD);
      }
      else  if (pWorld == &sm_pTranslucentWorld)
      {
         RWS_RETURN(HINT_TRANSLUCENT_WORLD);
      }

      RWS_RETURN(HINT_NO_HINT);
   }

   /**
   *
   *  Set the preferred world hint of an atomic.
   *
   *  \param pAtomic Pointer to an RpAtomic object.
   *  \param Hint See CLevelHint.
   *
   *  \see CLevel::AddAtomic
   */
   void CLevel::SetPreferredWorldHint(RpAtomic * const pAtomic, CLevelHint Hint)
   {
      RWS_FUNCTION("RWS::CLevel::SetPreferredWorldHint(RpAtomic * const pAtomic,CLevelHint Hint)¬");

      RpGeometry *pGeometry = RpAtomicGetGeometry(pAtomic);

      CLevelGeometryPlugin& pluginData = *GetPluginData(*pGeometry);
      
      switch (Hint)
      {
         default:
            RWS_ASSERTFAIL("Unknown HINT type.")
            break;
         case HINT_OPAQUE_WORLD:
            pluginData.m_PreferredWorld = &sm_pOpaqueWorld;
            break;
         case HINT_TRANSLUCENT_WORLD:
            pluginData.m_PreferredWorld = &sm_pTranslucentWorld;
            break;
         case HINT_NO_HINT:
            pluginData.m_PreferredWorld = 0;
            break;
      }

      RWS_RETURNVOID();
   }

   namespace
   {
      /*
      *
      *  Internal callback used by CLevel::SetPreferredWorldHint
      *  
      *  \param pAtomic Pointer to an RpAtomic object.
      *  \param data Pointer to a CLevelHint.
      *  
      *  \return Returns pAtomic.
      */
      RpAtomic *SetPreferredWorldHintCB(RpAtomic *pAtomic, void *data)
      {
         RWS_FUNCTION("RWS::NULL::SetPreferredWorldHintCB");

         RWS::CLevel::CLevelHint *pHint = reinterpret_cast<RWS::CLevel::CLevelHint*>(data);

         RWS::CLevel::SetPreferredWorldHint(pAtomic, *pHint);

         RWS_RETURN(pAtomic);
      }
   }

   /**
   *
   *  Sets the Preferred world hint for a clump, by setting the preferred world
   *  of each of the atomics within the clump to the Preferred world.
   *
   *  \param pClump Pointer to the RpClump object.
   *  \param Hint See CLevelHint.
   *
   *  \see CLevel::AddClump
   */
   void CLevel::SetPreferredWorldHint(RpClump * const pClump, CLevelHint Hint)
   {
      RWS_FUNCTION("RWS::CLevel::SetPreferredWorldHint(RpClump * const pClump,CLevelHint Hint)");

      RpClumpForAllAtomics(pClump, SetPreferredWorldHintCB, &Hint);

      RWS_RETURNVOID();
   }

   namespace
   {
      /*
      *
      *  Internal callback used by CLevel::GetPreferredWorldPtr
      *
      *  \param pAtomic Pointer to an RpAtomic object.
      *  \param data Pointer to, a pointer to a pointer to an RpWorld object, the pointer
      *  to the hint as it is stored in the plug-in.
      *
      *  \return Returns pAtomic or 0 if preferred world has been modified.
      *
      */
      RpAtomic *WorldCheckClumpAtomic(RpAtomic * pAtomic, void *data)
      {
         RWS_FUNCTION("RWS::NULL::WorldCheckClumpAtomic");

         RpWorld ***pppWorld = static_cast<RpWorld***>(data);

         RpWorld **ppPrefWorld = PreferredWorldPtr(pAtomic);

         *pppWorld = ppPrefWorld;

         RWS_RETURN(0);
      }

      /*
      *
      *  Internal function used to obtain pluginData.m_PreferredWorld for a clump
      *  which is a pointer to a pointer to the preferred world.
      *
      */
      RpWorld **PreferredWorldPtr(const RpClump * const pClump)
      {
         RWS_FUNCTION("RWS::NULL::PreferredWorldPtr(RpClump *pClump)");

         RpWorld **ppWorld = 0;

         RpClumpForAllAtomics(const_cast<RpClump*>(pClump), WorldCheckClumpAtomic, &ppWorld);

         RWS_RETURN(ppWorld);
      }
   }

   /**
   *
   *  Determines the preferred world for a clump, uses the first atomic in the
   *  clump to determine the preferred world.
   *
   *  \param RpClump Pointer to a RpClump object.
   *
   *  \return Returns a pointer to the preferred world, this is either the global
   *  opaque world or the global translucent world
   *
   *  \ref CLevel::SetOpaqueWorld \ref CLevel::SetTranslucentWorld
   *
   *  \see CLevel::AddClump
   */
   RpWorld *CLevel::GetPreferredWorldPtr(const RpClump * const pClump)
   {
      RWS_FUNCTION("RWS::CLevel::GetPreferredWorldPtr(RpClump *pClump)");

      RpWorld **ppWorld = PreferredWorldPtr(pClump);

      if (ppWorld) RWS_RETURN(*ppWorld);   // Return a pointer to the world

      RWS_RETURN(0);
   }

   /**
   *  Return the preferred world hint of a clump.
   *
   *  \param pClump Pointer to a RenderWare Gfx RpClump object.
   *
   *  \returns Returns a \ref CLevel::CLevelHint hint.
   */
   CLevel::CLevelHint CLevel::GetPreferredWorldHint(const RpClump * const pClump)
   {
      RWS_FUNCTION("RWS::CLevel::GetPreferredWorldHint(const RpAtomic * const pClump);");

      RpWorld **ppWorld = PreferredWorldPtr(pClump);

      if (ppWorld == &sm_pOpaqueWorld) 
      {
         RWS_RETURN(HINT_OPAQUE_WORLD);
      }
      else  if (ppWorld == &sm_pTranslucentWorld)
      {
         RWS_RETURN(HINT_TRANSLUCENT_WORLD);
      }

      RWS_RETURN(HINT_NO_HINT);
   }

   /**
   *  Add atomic to a world within level, using the hint \ref CLevel::SetPreferredWorld to
   *  decide which world to add the atomic to, that is either the translucent or 
   *  the opaque world if available. If the hint world is undefined then the active
   *  world is used which is the last opaque or translucent world set \ref CLevel::SetOpaqueWorld
   *  or \ref CLevel::SetTranslucentWorld.
   *  i.e. If the hint is set to opaque world and \ref CLevel::SetOpaqueWorld
   *  has been called with a valid world then the atomic is added to the opaque
   *  world.
   *  Similarly if the hint is set to translucent world and 
   *  \ref CLevel::SetTranslucentWorld has been called with a valid world then
   *  the atomic is added to the translucent world.
   *  However if the hint is set to translucent world and no translucent world has
   *  been specified then the atomic is added to the current world where the current
   *  world is the last valid world that either \ref CLevel::SetOpaqueWorld
   *  or CLevel::SetTranslucentWorld was called with. If neither an opaque world or translucent
   *  world has been specified then an assert is generated.
   *
   *  \param light Pointer to an RpAtomic object.
   *
   *  \return Returns result of RpWorldAddAtomic.
   */
   RpWorld* CLevel::AddAtomic(RpAtomic *atomic)
   {
      RWS_FUNCTION("RWS::CLevel::AddAtomic");

      RpWorld *pWorld = GetPreferredWorldPtr(atomic);

      if (pWorld == 0)
      {
         pWorld = sm_pActiveWorld;
      }

      RWS_PRE(pWorld);

      RWS_RETURN( RpWorldAddAtomic(pWorld, atomic) );
   }

   /**
   *  Add light to active world within level.
   *
   *  \param light Pointer to an RpLight object.
   *
   *  \return Returns result of RpWorldAddLight.
   */
   RpWorld* CLevel::AddLight(RpLight *light)
   {
      RWS_FUNCTION("RWS::CLevel::AddLight");

      RWS_ASSERT(sm_pActiveWorld, "Cannot add a clump as there is no world. Ensure the object is being"
            " added after the first world object and not before it.");

      RWS_RETURN( RpWorldAddLight(sm_pActiveWorld, light) );
   }

   /**
   *  Add clump to a world within level \see CLevel::AddAtomic.
   *
   *  \param clump Pointer to an RpClump object.
   *
   *  \return Returns result of RpWorldAddClump.
   */
   RpWorld *CLevel::AddClump(RpClump *clump)
   {
      RWS_FUNCTION("RWS::CLevel::AddClump");

      RpWorld *pWorld = GetPreferredWorldPtr(clump);

      if (pWorld == 0)
      {
         pWorld = sm_pActiveWorld;
      }

      RWS_ASSERT(pWorld, "Cannot add a clump as there is no world. Ensure the object is being"
            " added after the first world object and not before it.");

      RWS_RETURN( RpWorldAddClump(pWorld, clump) );
   }
}
