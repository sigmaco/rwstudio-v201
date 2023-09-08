/*****************************************************************************
*
* File :     attributehandlerstrfunc.cpp
*
* Abstract : Implements chunk handlers provided by this module.
*
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

#include "../macros/debugmacros.h"
#include "../streamhandler/streamhandler.h"
#include "../streamhandler/strfunc.h"
#include "cattributehandler.h"
#include "../eventhandler/ceventhandler.h"
#include "../functionprofiler/profile.h"


namespace RWS
{
   NewAttributeHandlerCallBack CAttributeHandler::sm_pNewAttributeHandlerCallBack = 0;

   namespace 
   {

      /**
      *
      *  Delete a specific entity.
      *
      *  \param rChunkHeader A reference to a RenderWare Gfx RwChunkHeaderInfo object.
      *
      *  \param pStream A pointer to the RenderWare Gfx Stream object.
      */
      void DeleteEntity (RwChunkHeaderInfo &rChunkHeader, RwStream *pStream)
      {
         RWS_FUNCTION("RWS::NULL::DeleteEntity");
         
         RwChar *buffer = static_cast<char*>(::RWS_OP_NEW(rChunkHeader.length) );
         
         RwStreamRead(pStream, buffer, rChunkHeader.length);
         
         // Note, pmsg->pData points to a list of commands built up using
         // CLASSID, INSTANCEID, CREATECLASSID see enum ChunkType in cattributehandler.cpp
         //
         const CAttributePacket* pAttr = reinterpret_cast<CAttributePacket*>(buffer);
         
         RWSGUID Id = pAttr->GetInstanceId();
         
         CAttributeHandler* pCAttributeHandler = CAttributeHandler::Find(Id);
         
         delete pCAttributeHandler;
         
         operator delete (buffer);
         
         RWS_RETURNVOID();
      }
      
      /**
      *
      *  Delete all entities, event handlers and attribute handlers.
      *
      *  \param rChunkHeader A reference to a RenderWare Gfx RwChunkHeaderInfo object.
      *
      *  \param pStream A pointer to the RenderWare Gfx Stream object.
      */
      void DeleteAllEntities (RwChunkHeaderInfo &rChunkHeader, RwStream *pStream)
      {
         RWS_FUNCTION("RWS::NULL::DeleteAllEntities");
         
         // Delete all event handlers.
         //
         CEventHandler::DeleteEventHandlers(iMsgDeleteEventHandler); // Delete all entities
         
         // Deletes all instances of CAttributeHandler that have not been locked.
         //
         CAttributeHandler::Purge();
         
         RWS_RETURNVOID();
      }
      
      /**
      *
      *  Update an attribute of a specified behavior class.
      *
      *  \param rChunkHeader A reference to a RenderWare Gfx RwChunkHeaderInfo object.
      *
      *  \param pStream A pointer to the RenderWare Gfx Stream object.
      */
      void UpdateEntityAttributes (RwChunkHeaderInfo &rChunkHeader, RwStream *pStream)
      {
         RWS_FUNCTION("RWS::NULL::UpdateEntityAttributes");
         
         RwChar *buffer = static_cast<char*>(::RWS_OP_NEW(rChunkHeader.length) );
         
         RwStreamRead(pStream, buffer, rChunkHeader.length);
         
         // Note, pmsg->pData points to a list of commands built up using
         // CLASSID, INSTANCEID,CREATECLASSID see enum ChunkType in cattributehandler.cpp
         //
         const CAttributePacket* pAttr = reinterpret_cast<CAttributePacket*>(buffer);
         
         RWSGUID InstanceId = pAttr->GetInstanceId();
         
         CAttributeHandler *pCAttributeHandler = CAttributeHandler::Find(InstanceId);
         
         if (pCAttributeHandler)
         {
            pCAttributeHandler->HandleAttributes(*pAttr);
         }
         else
         {
            RWS_TRACE("UpdateEntityParams Unable to locate class "<<RWS_HEX(InstanceId));
         }
         
         operator delete (buffer);
         
         RWS_RETURNVOID();
      }      
      
      /**
      *
      *  Create a new instance of a behavior class.
      *
      *  \param rChunkHeader A reference to a RenderWare Gfx RwChunkHeaderInfo object.
      *
      *  \param pStream A pointer to the RenderWare Gfx Stream object.
      */
      void CreateEntity (RwChunkHeaderInfo &rChunkHeader, RwStream *pStream)
      {
         RWS_FUNCTION("RWS::NULL::CreateEntity");
         
         RwChar *buffer = static_cast<char*>(::RWS_OP_NEW(rChunkHeader.length) );
         
         RwStreamRead(pStream, buffer, rChunkHeader.length);
         
         RwBool * pbGlobal = reinterpret_cast<RwBool *>(buffer);
         
         // Note, pmsg->pData points to a list of commands built up using 
         // CLASSID, INSTANCEID, CREATECLASSID see enum ChunkType in cattributehandler.cpp
         //
         const CAttributePacket* pAttr = reinterpret_cast<CAttributePacket*>(pbGlobal + 1);
         
         CAttributeHandler *pNewAttrHand = pAttr->CreateEntity();

         if (pNewAttrHand && (*pbGlobal))
         {
            pNewAttrHand->Lock();
         }
         
         operator delete (buffer);
         
         RWS_RETURNVOID();
      }
   }     

   /**
   *
   *  Handle Custom data
   *
   *  \param rChunkHeader A reference to a RenderWare Gfx RwChunkHeaderInfo object.
   *
   *  \param pStream A pointer to the RenderWare Gfx Stream object.
   */
   void HandleProfilerMessages(RwChunkHeaderInfo &rChunkHeader, RwStream *pStream)
   {
      RWS_FUNCTION("RWS::NULL::HandleProfilerMessages");

      int Command, Child = 0;
         
      RwStreamRead(pStream, &Command, sizeof(int));
      RwStreamRead(pStream, &Child, sizeof(int));

#ifdef RWS_FUNCTIONPROFILE
      switch (Command)
      {
         case RWS::FunctionProfile::EnterChild_Command:
            RWS::FunctionProfile::CProfileManager::pIttr->EnterChild(Child);
         break;

         case RWS::FunctionProfile::EnterParent_Command:
            RWS::FunctionProfile::CProfileManager::pIttr->EnterParent();
         break;

         case RWS::FunctionProfile::ProfilerReset_Command:
            RWS::FunctionProfile::CProfileManager::Reset();
         break;
      }
#endif
      
      RWS_RETURNVOID();
   }

   /**
   *
   *  Register the chunk handlers implemented by CResourceManager, \see strfunc_func \see CStreamHandler.
   *
   *  CResourceManager implements the following chunk handlers, \ref strfunc_UnLoadAsset,
   *  \ref strfunc_LoadAsset, \ref strfunc_LoadEmbeddedAsset.
   *
   */
   void CAttributeHandler::RegisterStreamChunkHandlers(void)
   {
      RWS_FUNCTION("RWS::CAttributeHandler::RegisterStreamChunkHandlers");

      CStreamHandler::RegisterChunkHandler(MAKECHUNKID (rwVENDORID_CRITERIONRM,
         strfunc_CreateEntity), CreateEntity);
      
      CStreamHandler::RegisterChunkHandler(MAKECHUNKID (rwVENDORID_CRITERIONRM,
         strfunc_UpdateEntityAttributes), UpdateEntityAttributes);
      
      CStreamHandler::RegisterChunkHandler(MAKECHUNKID (rwVENDORID_CRITERIONRM,
         strfunc_DeleteEntity), DeleteEntity);
      
      CStreamHandler::RegisterChunkHandler(MAKECHUNKID (rwVENDORID_CRITERIONRM,
         strfunc_DeleteAllEntities), DeleteAllEntities);

      CStreamHandler::RegisterChunkHandler(MAKECHUNKID (rwVENDORID_CRITERIONRM,
         strfunc_FunctionProfiler), HandleProfilerMessages);

      // The default implementation of reset simply destroys the entity, then
      // recreates it. This is the same as CreateEntity, but we use a
      // separate strfunc to allow the reset function to be customized.
      CStreamHandler::RegisterChunkHandler(MAKECHUNKID (rwVENDORID_CRITERIONRM,
         strfunc_ResetEntity), CreateEntity);

      RWS_RETURNVOID();
   }
   
   /**
   *
   *  Unregister the chunk handlers implemented by CResourceManager, \see strfunc_func \see CStreamHandler.
   *
   */
   void CAttributeHandler::UnRegisterStreamChunkHandlers(void)
   {
      RWS_FUNCTION("RWS::CAttributeHandler::UnRegisterStreamChunkHandlers");

      CStreamHandler::UnRegisterChunkHandler(MAKECHUNKID (rwVENDORID_CRITERIONRM, strfunc_UpdateEntityAttributes));
      CStreamHandler::UnRegisterChunkHandler(MAKECHUNKID (rwVENDORID_CRITERIONRM, strfunc_CreateEntity));
      CStreamHandler::UnRegisterChunkHandler(MAKECHUNKID (rwVENDORID_CRITERIONRM, strfunc_DeleteEntity));
      CStreamHandler::UnRegisterChunkHandler(MAKECHUNKID (rwVENDORID_CRITERIONRM, strfunc_DeleteAllEntities));
      CStreamHandler::UnRegisterChunkHandler(MAKECHUNKID (rwVENDORID_CRITERIONRM, strfunc_FunctionProfiler));
      CStreamHandler::UnRegisterChunkHandler(MAKECHUNKID (rwVENDORID_CRITERIONRM, strfunc_ResetEntity));

      RWS_RETURNVOID();
   }
}
