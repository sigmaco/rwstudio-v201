/*****************************************************************************
*
* File :     FPSSpinningPickup.cpp
*
* Abstract : Implementation of FPSSpinningPickup class
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
* Copyright (c) 2002 Criterion Software Ltd.
* All Rights Reserved.
*
* RenderWare is a trademark of Canon Inc.
*
*****************************************************************************/
#include "precomp.h"
#ifdef __MWERKS__
#pragma force_active on // Do not deadstrip this class
#pragma exceptions off // Turn off exceptions due to bug in MW Dwarf Generator for PS2, present in CW 3.04
#endif

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "fpsspinningpickup.h"
#include "framework/mainloop/logic.h"
#include "framework/toolkits/math/maths.h"

namespace RWS
{
   namespace FPS
   {
      RWS_IMPLEMENT_CLASSID(FPSSpinningPickup);
      RWS_REGISTERCLASS(FPSSpinningPickup);
      

      /**
      *
      * \ref FPSSpinningPickup.  Constructor for behavior.
      *
      * \see ~FPSSpinningPickup
      */
      FPSSpinningPickup::FPSSpinningPickup(const CAttributePacket& rAttr) : FPSPickup(rAttr), m_RotRate(8.0f),
            m_count(0)
      {
         RWS_FUNCTION("RWS::FPS::FPSSpinningPickup::FPSSpinningPickup");

         Set_m_msg_response_tx("FPS_Example_Pickup");

         RwMatrixRotate(&m_pRotation, &YAxis, m_RotRate, rwCOMBINEREPLACE);
         LinkMsg(iMsgRunningTick, 0);

         RWS_RETURNVOID();
      }

      
      /**
      *
      * \ref ~FPSSpinningPickup.  Destructor for behavior
      *
      * \see FPSSpinningPickup
      *
      */
      FPSSpinningPickup::~FPSSpinningPickup(void)
      {
         RWS_FUNCTION("RWS::FPS::FPSSpinningPickup::~FPSSpinningPickup");

         UnLinkMsg(iMsgRunningTick);

         RWS_RETURNVOID();
      }


      /**
      *
      * \ref HandleEvents(). Handle events       
      *
      * \param pMsg Standard Message Package
      *  
      * \ref iMsgRunningTick
      */
      void FPSSpinningPickup::HandleEvents(CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::FPS::FPSSpinningPickup::HandleEvents");

         if (pMsg.Id == iMsgRunningTick)
         {
            RwFrame* pFrame = RpAtomicGetFrame(m_pAtomic.ptr());
            RwFrameTransform(pFrame, &m_pRotation,rwCOMBINEPRECONCAT);
            if (++m_count > 128)
            {
                RwFrameOrthoNormalize(pFrame);
                m_count = 0;
            }
         }
         else
         {
            FPSPickup::HandleEvents(pMsg);
         }

         RWS_RETURNVOID();
      }


      /**
      *
      * \ref HandleAttributes. Handle attribute updates.
      *
      * \param rAttr reference to a CAttributePacket.
      *
      */
      void FPSSpinningPickup::HandleAttributes(const CAttributePacket &rAttr)
      {
         RWS_FUNCTION("RWS::FPS::FPSSpinningPickup::HandleAttributes");
         
         FPSPickup::HandleAttributes(rAttr);
         
         CAttributeCommandIterator attrIt(rAttr, RWS_CLASSID_OF(FPSSpinningPickup));
         
         while (!attrIt.IsFinished())
         {
            switch (attrIt->GetCommandId())
            {
               case CMD_Set_Spin_Rate:
                  m_RotRate = attrIt->GetAs_RwReal();
                  // alter spin rate.
                  RwMatrixRotate(&m_pRotation, &YAxis, m_RotRate, rwCOMBINEREPLACE);
               break;
            }
            ++attrIt;
         }
         RWS_RETURNVOID();
      }
   }
}