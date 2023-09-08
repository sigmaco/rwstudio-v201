/*****************************************************************************
*
* File :     FPSTeleporter.cpp
*
* Abstract : Teleport the player or monster to an FPSTeleportDestination. 
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
#pragma exceptions off // Turn off exceptions due to bug in MW Dwarf Generator for PS2, present in CW 3.04
#endif

//////////////////////////////////////////////////////////////////
//
// RenderWare Includes
//
#include <rwcore.h>
#include <rpworld.h>

#ifdef _MSC_VER
#pragma comment (lib, "rwcore.lib")
#pragma comment (lib, "rpworld.lib")
#endif

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "framework/toolkits/world/factory.h"

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "fpsteleporter.h"

namespace RWS
{
   namespace FPS
   {
      
      RWS_IMPLEMENT_CLASSID(FPSTeleporter);
      RWS_REGISTERCLASS(FPSTeleporter);
      
      
      /**
      *
      * \ref FPSTeleporter. FPSTeleporter constructor.
      *
      * \see ~FPSTeleporter.
      *
      */
      FPSTeleporter::FPSTeleporter(const CAttributePacket& attr) : InitCEventHandler(&m_pAtomic)
      {
         RWS_FUNCTION("RWS::FPS::FPSTeleporter::FPSTeleporter");
         
         // Can't use initializer ('this' isn't valid) so...
         m_pAtomic = CreateAtomicInWorldFromResource(attr, this);
         
         RWS_ASSERT(m_pAtomic, "Failed to create atomic");
         
         // if no m_targetname is specified then use ACTN_PLAYERTOUCH
         //
         RegisterMsg(m_targetname, "ACTN_PLAYERTOUCH",0);
         
         LinkMsg(m_targetname, 0);
         
         // Send this event to the object being teleported
         //
         RegisterMsg(m_ACTN_TELEPORT, "ACTN_TELEPORT", "RwFrame*");
         
         RWS_RETURNVOID();
      }
      
      
      /**
      *        
      * \ref ~FPSTeleporter. FPSTeleporter destructor.
      *
      * \see FPSTeleporter.
      *
      */
      FPSTeleporter::~FPSTeleporter(void)
      {
         RWS_FUNCTION("RWS::FPS::FPSTeleporter::~FPSTeleporter");
         
         UnLinkMsg(m_targetname);
         UnRegisterMsg(m_targetname);
         UnRegisterMsg(m_target);
         UnRegisterMsg(m_ACTN_TELEPORT);
         
         RWS_RETURNVOID();
      }
      
      
      /**
      *
      * \ref HandleEvents(). Handle events.    
      *
      * \param pMsg Standard Message Package.
      *  
      * \ref iMsgRunningTick
      */
      void FPSTeleporter::HandleEvents(CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::FPS::FPSTeleporter::HandleEvents");
         
         // Teleport Request Has Been Generated
         if (pMsg.Id == m_targetname)
         {
            RWS_TRACE("FPSTeleporter::HandleEvents");
            
            CEventHandler *pCEventHandler = static_cast<CEventHandler*>(pMsg.pData);
            
            // Sender should have provided a pointer to its RpAtomic so that
            // we can move it.
            if (pCEventHandler)
            {
               // m_target is sent passing the address of a RwFrame pointer
               // this is set by the receiver of the message, FPS_info_teleport_destination                   
               RwFrame *pFrame = 0;
               
               // Build up an event, to request the frame to be modified
               CMsg tMsg(m_target, &pFrame);
               
               if (SendMsg(tMsg) && pFrame)
               {
                  // Need to send this frame to the object being teleported
                  CMsg tMsg(m_ACTN_TELEPORT, pFrame);
                  
                  SendMsgToEventHandler(tMsg, pCEventHandler);
               }
               else
               {
                  RWS_TRACE("FPSTeleporter::HandleEvents no response to event "<<tMsg.Id->p_msgname);
               }
            }
         }
         RWS_RETURNVOID();
      }
      
      
      /**
      *
      * \ref HandleAttributes. Handle attribute updates.
      *
      * \param attr reference to a CAttributePacket.
      *
      */ 
      void FPSTeleporter::HandleAttributes(const CAttributePacket& attr)
      {
         RWS_FUNCTION("RWS::FPS::FPSTeleporter::HandleAttributes");
         
         CAttributeHandler::HandleAttributes(attr);
         
         m_pAtomic.HandleSystemCommands(attr);
         
         CAttributeCommandIterator attrIt(attr, RWS_CLASSID_OF(FPSTeleporter));
         
         while (!attrIt.IsFinished())
         {
            switch (attrIt->GetCommandId())
            {
            case CMD_targetname:
               {
                  const RwChar *name;
                  
                  attrIt->GetCommandData(&name);
                  UnLinkMsg (m_targetname);
                  UnRegisterMsg(m_targetname);
                  RegisterMsg(m_targetname, name, "CEventHandler*");
                  LinkMsg(m_targetname, "CEventHandler*");
               }
               break;
               
            case CMD_target:
               {
                  const RwChar *name;
                  
                  attrIt->GetCommandData(&name);
                  UnRegisterMsg(m_target);
                  RegisterMsg(m_target, name, "RwFrame**");
               }
               break;
            }
            ++attrIt;
         }
         
         RWS_RETURNVOID();
      }
   }// namespace FPS
}// namespace RWS