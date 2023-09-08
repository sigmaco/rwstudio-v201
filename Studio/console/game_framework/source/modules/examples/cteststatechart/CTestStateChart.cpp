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
#include "CTestStateChart.h"
#include "framework/core/macros/debugmacros.h"
#include "framework/toolkits/math/maths.h"
#include "framework/toolkits/world/factory.h"

namespace RWS
{
   namespace Examples
   {
      RWS_IMPLEMENT_CLASSID(CTestStateChart);
      RWS_REGISTERCLASS(CTestStateChart);
      
      /**
      *
      *  Create CTestStateChart object. Gets behaviors atomic by extracting information the behavior needs from the 
      *  attributes used to construct the behavior.  This constructor also sets up the matrix used for rotating the 
      *  object and links to the running tick.
      *
      *  \param attr the standard attribute data used for initialization.
      *
      */
      CTestStateChart::CTestStateChart(const CAttributePacket& attr)
         : InitCEventHandler(&m_pAtomic), CStateChartHandler(attr)
      {
         RWS_FUNCTION("RWS::Examples::CTestStateChart::CTestStateChart");
         
         m_pAtomic = CreateAtomicInWorldFromResource(attr, this);
         
         RWS_ASSERT(m_pAtomic, "Failed to create atomic");
         m_mat = RwMatrixCreate();
         
         RWS_POST(m_mat);
         
         LinkMsg(iMsgRunningTick, 0);

         // Create the statechart instance
         CreateInstance(this);
         
         RWS_RETURNVOID();
      }
      
      /**
      *
      *  Destroy CTestStateChart object.  Destroy matrix and unlink from running tick.
      *
      */
      CTestStateChart::~CTestStateChart(void)
      {
         RWS_FUNCTION("RWS::Examples::CTestStateChart::~CTestStateChart");
         
         RWS_PRE(m_mat);
         
         RwMatrixDestroy(m_mat);
         m_mat = 0;
         
         UnLinkMsg(iMsgRunningTick);

         // Destroy the statechart instance
         DestroyInstance();
         
         RWS_RETURNVOID();
      }
      
      /**
      *
      *   Handle events.  Rotate object based on values within m_rot. This happens on the system running tick.
      *  
      *   \param pMsg the standard RenderWare Studio message packets.
      *
      */
      void CTestStateChart::HandleEvents(CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::Examples::CTestStateChart::HandleEvents");
         
         RwFrame* pFrame = RpAtomicGetFrame(m_pAtomic.ptr());
         
         // Execute the statechart every tick
         if (pMsg.Id == iMsgRunningTick)
         {
            if (GetExecState() == CStateMachine::RUNNING)
            {
                Tick(this);
            }
         }
         
         HandleStateChartEvents(pMsg);

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
      void CTestStateChart::HandleAttributes(const CAttributePacket& attr)
      {
         RWS_FUNCTION("RWS::Examples::CTestStateChart::HandleAttributes");
         
         CAttributeHandler::HandleAttributes(attr);
         
         m_pAtomic.HandleSystemCommands(attr);

         HandleStateChartAttributes(attr);
         
         
         RWS_RETURNVOID();
      }
      
   }//namespace Examples
}//namespace RWS