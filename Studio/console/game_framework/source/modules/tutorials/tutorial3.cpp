/*****************************************************************************
*
* File :      Tutorial3.cpp
*
* Abstract :  Editing behavior attributes.
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
#include "tutorial3.h"
#include "framework/core/macros/debugmacros.h"
#include "framework/toolkits/math/maths.h"
#include "framework/toolkits/world/factory.h"

namespace RWS
{
   namespace Tutorial
   {
      RWS_IMPLEMENT_CLASSID(CTutorial3);
      RWS_REGISTERCLASS(CTutorial3);
      
      /**
      *
      *  Create CTutorial3 object. Gets behaviors atomic by extracting information the behavior needs from the 
      *  attributes used to construct the behavior.  This constructor also sets up the matrix used for rotating the 
      *  object and links to the running tick.
      *
      *  \param attr the standard attribute data used for initialization.
      *
      */
      CTutorial3::CTutorial3(const CAttributePacket& attr)
         : InitCEventHandler(&m_pAtomic)
      {
         RWS_FUNCTION("RWS::Tutorial::CTutorial3::CTutorial3");
         
         m_pAtomic = CreateAtomicInWorldFromResource(attr, this);
         
         RWS_ASSERT(m_pAtomic, "Failed to create atomic");
         m_mat = RwMatrixCreate();
         
         RWS_POST(m_mat);
         
         LinkMsg(iMsgRunningTick, 0);
         
         RWS_RETURNVOID();
      }
      
      /**
      *
      *  Destroy CTutorial3 object.  Destroy matrix and unlink from running tick.
      *
      */
      CTutorial3::~CTutorial3(void)
      {
         RWS_FUNCTION("RWS::Tutorial::CTutorial3::~CTutorial3");
         
         RWS_PRE(m_mat);
         
         RwMatrixDestroy(m_mat);
         m_mat = 0;
         
         UnLinkMsg(iMsgRunningTick);
         
         RWS_RETURNVOID();
      }
      
      /**
      *
      *   Handle events.  Rotate object based on values within m_rot. This happens on the system running tick.
      *  
      *   \param pMsg the standard RenderWare Studio message packets.
      *
      */
      void CTutorial3::HandleEvents(CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::Tutorial::CTutorial3::HandleEvents");
         
         RwFrame* pFrame = RpAtomicGetFrame(m_pAtomic.ptr());
         
         if (pMsg.Id == iMsgRunningTick)
         {
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
      *  Handle attributes.  This function receives the attribute values from RenderWare Studio.
      *  The x, y, and z values are stored in the m_rot array.
      *
      *  \param attr The standard attribute packets, some of which contain changes to the rotation.
      *
      */
      void CTutorial3::HandleAttributes(const CAttributePacket& attr)
      {
         RWS_FUNCTION("RWS::Tutorial::CTutorial3::HandleAttributes");
         
         CAttributeHandler::HandleAttributes(attr);
         
         m_pAtomic.HandleSystemCommands(attr);
         
         CAttributeCommandIterator attrIt(attr, RWS_CLASSID_OF(CTutorial3));
         
         while (!attrIt.IsFinished())
         {
            switch (attrIt->GetCommandId())
            {
            case CMD_rot_x:
               attrIt->GetCommandData(m_rot[0]);
               m_rot[0] = RWDEG2RAD(m_rot[0]);
               break;
               
            case CMD_rot_y:
               attrIt->GetCommandData(m_rot[1]);
               m_rot[1] = RWDEG2RAD(m_rot[1]);
               break;
               
            case CMD_rot_z:
               attrIt->GetCommandData(m_rot[2]);
               m_rot[2] = RWDEG2RAD(m_rot[2]);
               break;
               
            }
            ++attrIt;
         }
         
         RWS_RETURNVOID();
      }
      
   }//namespace Tutorial
}//namespace RWS