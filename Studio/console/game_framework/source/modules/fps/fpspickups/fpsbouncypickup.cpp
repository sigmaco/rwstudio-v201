/*****************************************************************************
*
* File :     FPSBouncyPickup.cpp
*
* Abstract : Implementation of FPSBouncyPickup class
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
// RenderWare Includes
//
#include "rprandom.h"
#ifdef _MSC_VER
#pragma comment (lib, "rprandom.lib")
#endif


//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "fpsbouncypickup.h"
#include "framework/mainloop/logic.h"
#include "framework/toolkits/math/maths.h"

namespace RWS
{
   namespace FPS
   {

      RWS_IMPLEMENT_CLASSID(FPSBouncyPickup);
      RWS_REGISTERCLASS(FPSBouncyPickup);
      
      /**
      *
      * \ref FPSBouncyPickup.  Constructor for pickup
      *
      * \see ~FPSBouncyPickup
      *
      */
      FPSBouncyPickup::FPSBouncyPickup(const CAttributePacket& rAttr) : FPSPickup(rAttr),
                                                                        m_rTheta(0),
                                                                        m_rActualHeight(0),
                                                                        m_RotRate(8.0f),
                                                                        m_BounceHeight(3.0f),
                                                                        m_Count(RpRandom() & 0x7f)
      {
         RWS_FUNCTION("RWS::FPS::FPSBouncyPickup::FPSBouncyPickup");

         Set_m_msg_response_tx("FPS_Example_Pickup");
                      
         RwMatrixRotate(&m_pRotation, &YAxis, m_RotRate, rwCOMBINEREPLACE);      
                 
         LinkMsg(iMsgRunningTick, 0);

         m_BounceRate = RWDEG2RAD(8.0f);

         RWS_RETURNVOID();
      }
      

      /**
      *
      * \ref ~FPSBouncyPickup.  Destructor for behavior
      *
      * \see FPSBouncyPickup
      *
      */
      FPSBouncyPickup::~FPSBouncyPickup(void)
      {
         RWS_FUNCTION("RWS::FPS::FPSBouncyPickup::~FPSBouncyPickup");

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
      void FPSBouncyPickup::HandleEvents(CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::FPS::FPSBouncyPickup::HandleEvents");

         if (pMsg.Id == iMsgRunningTick)
         {
            RwFrame* pRwFrame = RpAtomicGetFrame(m_pAtomic.ptr());

            RwV3d *pPos = RwMatrixGetPos(RwFrameGetMatrix(pRwFrame));

            pPos->y = m_rActualHeight + m_BounceHeight * static_cast<RwReal>(RwSin( m_rTheta ));

            m_rTheta += m_BounceRate;
 
            RwFrameTransform(pRwFrame, &m_pRotation, rwCOMBINEPRECONCAT);

            // The RenderWare API documentation recommends this happen at least once every 128 concatenations
            //
            m_Count++;
            
            if (m_Count>128)
            {
               RwFrameOrthoNormalize(pRwFrame);
               m_Count = 0;
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
      void FPSBouncyPickup::HandleAttributes(const CAttributePacket& rAttr)
      {
         RWS_FUNCTION("RWS::FPS::FPSBouncyPickup::HandleAttributes");

         CAttributeHandler::HandleAttributes(rAttr);

         FPSPickup::HandleAttributes(rAttr);

         CAttributeCommandIterator attrIt_sys(rAttr, RWS_CLASSID_OF(CSystemCommands));

         while (!attrIt_sys.IsFinished())
         {
            switch (attrIt_sys->GetCommandId())
            {
            case CSystemCommands::CMD_LoadMatrix:
               {
                  RwFrame* pRwFrame = RpAtomicGetFrame(m_pAtomic.ptr());
                  if (pRwFrame)
                  {
                     // We need to setup the initial height of the frame
                     //
                     RwV3d *pPos = RwMatrixGetPos(RwFrameGetMatrix(pRwFrame));
                     
                     m_rTheta = static_cast<RwReal>(RwFmod (pPos->x + pPos->z, 360.0f));
                     m_rActualHeight = pPos->y;
                  }
               }
               break;
            }
            
            ++attrIt_sys;
         } 


         CAttributeCommandIterator attrIt(rAttr, RWS_CLASSID_OF(FPSBouncyPickup));
         
         while (!attrIt.IsFinished())
         {
            switch (attrIt->GetCommandId())
            {
            case CMD_Set_Spin_Rate:
               {
                  m_RotRate = attrIt->GetAs_RwReal();
                  
                  RwMatrixRotate(&m_pRotation, &YAxis, m_RotRate, rwCOMBINEREPLACE);
               }
               break;
               
            case CMD_Set_Bounce_Height:
               {
                  m_BounceHeight = attrIt->GetAs_RwReal();
               }
               break;
               
            case CMD_Set_Bounce_Rate:
               {
                  m_BounceRate = RWDEG2RAD(attrIt->GetAs_RwReal());
               }
               break;
            }
            ++attrIt;
         }
         
         RWS_RETURNVOID();
      }
   }
}
