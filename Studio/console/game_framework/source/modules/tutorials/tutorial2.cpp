/*****************************************************************************
*
* File :      Tutorial2.cpp
*
* Abstract :  Adding an action.
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
// RenderWare Studio Includes
//
#include "tutorial2.h"
#include "framework/core/macros/debugmacros.h"
#include "framework/toolkits/math/maths.h"
#include "framework/toolkits/world/factory.h"

namespace RWS
{
   namespace Tutorial
   {
      RWS_IMPLEMENT_CLASSID(CTutorial2);
      RWS_REGISTERCLASS(CTutorial2);
      
      /**
      *
      *  Create CTutorial2 object. Gets behaviors atomic by extracting information the behavior needs from the 
      *  attributes used to construct the behavior.  This constructor also sets up the matrix used for rotating the 
      *  object and links to the running tick.
      *
      *  \param attr the attribute packets used to initialize.
      *
      */
      CTutorial2::CTutorial2(const CAttributePacket& attr)
         : InitCEventHandler(&m_pAtomic)
      {
         RWS_FUNCTION("RWS::Tutorial::Ctutorial2::CTutorial2");
         
         m_pAtomic = CreateAtomicInWorldFromResource(attr, this);
         
         RWS_ASSERT(m_pAtomic, "Failed to create atomic");
         
         m_mat = RwMatrixCreate();
         RWS_POST(m_mat);
         LinkMsg(iMsgRunningTick, 0);
         
         m_rot[0] = 0.0f;
         m_rot[1] = rwPI;  // Rotation on the Y axis.
         m_rot[2] = 0.0f;
         
         RWS_RETURNVOID();
      }
        
      /**
      *
      *  Destroy CTutorial2 object, destroy matrix and unlink messages.
      *
      */
      CTutorial2::~CTutorial2(void)
      {
         RWS_FUNCTION("RWS::Tutorial::CTutorial2::~CTutorial2");
         
         RWS_PRE(m_mat);
         RwMatrixDestroy(m_mat);
         m_mat = 0;
         UnLinkMsg(iMsgRunningTick);
         
         RWS_RETURNVOID();
      }
      
      /**
      *
      *  Handle events.  Rotate object based on values within m_rot. This happens on the system running tick.
      *
      *  \param pMsg the standard message structure
      *  
      */
      void CTutorial2::HandleEvents(CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::Tutorial::CTutorial2::HandleEvents");
         
         if (pMsg.Id == iMsgRunningTick)
         {
            RwFrame* pFrame = RpAtomicGetFrame(m_pAtomic.ptr());
            
            RwMatrixSetIdentity(m_mat);
            RwMatrixRotate(m_mat, &XAxis, m_rot[0], rwCOMBINEPRECONCAT);
            RwMatrixRotate(m_mat, &YAxis, m_rot[1], rwCOMBINEPRECONCAT);
            RwMatrixRotate(m_mat, &ZAxis, m_rot[2], rwCOMBINEPRECONCAT);
            
            RwFrameTransform(pFrame, m_mat, rwCOMBINEPRECONCAT);
         }
         
         RWS_RETURNVOID();
      }
      
      /**
      *
      *  Handle attributes - calls the base class handling code.
      *
      *  \param attr the standard attribute packets.
      *  
      */
      void CTutorial2::HandleAttributes(const CAttributePacket& attr)
      {
         RWS_FUNCTION("RWS::Tutorial::CTutorial2::HandleAttributes");
         
         CAttributeHandler::HandleAttributes(attr);
         
         m_pAtomic.HandleSystemCommands(attr);
         
         RWS_RETURNVOID();
      }

   }//namespace Tutorial
}//namespace RWS