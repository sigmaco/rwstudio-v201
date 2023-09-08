/*****************************************************************************
*
* File :     ATBase.cpp
*
* Abstract : Base class for an area trigger.
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

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "framework/core/macros/debugmacros.h"
#include "framework/mainloop/logic.h"
#include "framework/mainloop/render.h"

#include "atbase.h"


namespace RWS
{
   namespace AreaTrigger
   {
      RWS_IMPLEMENT_CLASSID(ATBase);

      /**
      *
      *  Constructor
      *
      */
      ATBase::ATBase( const CAttributePacket& attr )
         : InitCEventHandler(0),
           m_pLogic(0)
      {
         RwMatrixSetIdentity(&m_Matrix);
         RwMatrixSetIdentity(&m_InvMatrix);
         m_pLogic = new ATLogic(this);
      }

      /**
      *
      *  Destructor
      *
      */
      ATBase::~ATBase()
      {
         if ( m_pLogic )
         {
            delete m_pLogic;
            m_pLogic = 0;
         }

         UnLinkMsg(m_QueryEvent);
         UnRegisterMsg(m_QueryEvent);
         UnRegisterMsg(m_EnterEvent);
         UnRegisterMsg(m_ExitEvent);
         UnLinkMsg(iMsgRunningTick);
      }


      /**
      *
      *  Handle events
      *
      *  \param Msg Message to react to.
      *
      */
      void ATBase::HandleEvents( CMsg& Msg )
      {
         RWS_FUNCTION("ATBase::HandleEvents");

         if ( Msg.Id == m_QueryEvent )
         {
            // test for collision here
            ATCollisionQuery * pData = reinterpret_cast<ATCollisionQuery *>(Msg.pData);

            if (m_pLogic && pData)
            {
               RwBool bIntersecting;

               switch (pData->Data.type)
               {
               case rpINTERSECTPOINT:

                  bIntersecting = PointInsideGeometry(&pData->Data.t.point);
                  break;

               case rpINTERSECTLINE:

                  bIntersecting = LineIntersectGeometry(&pData->Data.t.line);
                  break;
                  
               default:
                  // Unhandled intersection types
                  bIntersecting = false;
                  break;
               }
               
               m_pLogic->ProcessLogic(pData->pEventHandler, bIntersecting);
            }
         }

#if defined(RWS_DEBUGTOOLS)
         if ( Msg.Id == iMsgRunningTick )
         {
            DebugToolsRender();
         }
#endif

         RWS_RETURNVOID();
      }

      /**
      *
      *  Handle attributes
      *
      *  \param attr Attribute packet to respond to.
      *
      */
      void ATBase::HandleAttributes( const CAttributePacket& attr )
      {
         RWS_FUNCTION("ATBase::HandleAttributes");

         CAttributeHandler::HandleAttributes(attr);

         // Parse attributes that belong to CSystemCommands. (Only need the matrix though).
         CAttributeCommandIterator attrItSysCommands(attr, RWS_CLASSID_OF(CSystemCommands));

         while (!attrItSysCommands.IsFinished())
         {
            switch (attrItSysCommands->GetCommandId())
            {
            case CSystemCommands::CMD_LoadMatrix:
               {
                  // Workaround in case the RwMatrix in the attribute packet is not
                  // properly aligned for the current platform
                  const RwMatrix* pUnalignedMat = static_cast<const RwMatrix*>(attrItSysCommands->GetAs_void_ptr());
                  m_Matrix = *pUnalignedMat;
                  RwMatrixOptimize(&m_Matrix, 0);
                  RwMatrixInvert(&m_InvMatrix, &m_Matrix);
                  RwMatrixOptimize(&m_InvMatrix, 0);
               }
               break;

            default:
   
               break;
            }

            ++attrItSysCommands;
         }

         // Parse attributes for this class
         CAttributeCommandIterator attrIt(attr, RWS_CLASSID_OF(ATBase));

         while (!attrIt.IsFinished())
         {
            switch (attrIt->GetCommandId())
            {
            case CMD_QueryEvent:
            
               ReplaceLinkedMsg( m_QueryEvent, attrIt->GetAs_RwChar_ptr(), "ATCollisionQuery*" );
               break;

            case CMD_Logic:
               {
                  // create new logic object
                  RwUInt32 LogicType = attrIt->GetAs_RwUInt32();
                  ATLogic * pNewLogic = 0;
                  switch ( LogicType )
                  {
                  case 0:
                     pNewLogic = new ATLogic(this);
                     break;

                  case 1:
                     pNewLogic = new ATFireOnceOnEntry(this);                  
                     break;
                     
                  case 2:
                     pNewLogic = new ATFireOnceOnEntryAndExit(this);
                     break;

                  default:
                     break;
                  }

                  // destroy current logic object
                  if ( m_pLogic )
                  {
                     delete m_pLogic;
                  }

                  m_pLogic = pNewLogic;
               }
               break;

            case CMD_EnterEvent:
            
               ReplaceRegisteredMsg( m_EnterEvent, attrIt->GetAs_RwChar_ptr(), 0 );
               break;

            case CMD_ExitEvent:
            
               ReplaceRegisteredMsg( m_ExitEvent, attrIt->GetAs_RwChar_ptr(), 0 );
               break;

            case CMD_DebugRender:
            
#if defined(RWS_DEBUGTOOLS)
               if ( attrIt->GetAs_RwUInt32() )
               {
                  LinkMsg( iMsgRunningTick );
               }
               else
               {
                  UnLinkMsg( iMsgRunningTick );
               }
#else
               // ignore this command in non-RWS_DEBUGTOOLS builds
#endif
               break;

            default:
   
               break;
            }

            ++attrIt;
         }

         RWS_RETURNVOID();
      }

   } // namespace AreaTrigger

} // namespace RWS