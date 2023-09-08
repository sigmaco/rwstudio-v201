/*****************************************************************************
*
* File :      Tutorial8.cpp
*
* Abstract :  Inheritance.
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
#include "tutorial8.h"

#include "framework/core/macros/debugmacros.h"
#include "framework/toolkits/math/maths.h"

namespace RWS
{
   namespace Tutorial
   {
      RWS_IMPLEMENT_CLASSID(CTutorial8);
      RWS_REGISTERCLASS(CTutorial8);
      
      /**
      *
      *  Create CTutorial8 object. Sets up the default values (not set by attributes) for this inherited class.
      *  Note that this behavior does not need to link to the running tick, as tutorial 2's constructor does
      *  this.
      *
      *  \param attr standard attribute packets used for initializing.
      *
      */
      CTutorial8::CTutorial8(const CAttributePacket& attr) :
         CTutorial2(attr),
         m_counter(0),
         m_rotate_flag(FALSE),
         m_scale(1.0f),
         m_scale_flag(TRUE)
      {
         RWS_FUNCTION("RWS::Tutorial::CTutorial8::CTutorial8");

         RWS_RETURNVOID();
      }
            
      /**
      *
      *  Destroy CTutorial7 object. Note that this behavior does not need to unlink from the running tick as
      *  tutorial 2's destructor does this.
      *
      */
      CTutorial8::~CTutorial8(void)
      {
         RWS_FUNCTION("RWS::Tutorial::CTutorial8::~CTutorial8");
         
         RWS_RETURNVOID();
      }
            
      /**
      *
      *  Handle Events, calls CTutorial2's handle events function (if necessary). Handles update of the counter
      *  for this inherited class and adjusts the scale.
      *
      *  \param pMsg the standard message packets, including the running tick.
      *  \see CTutorial2::HandleEvents
      *  \see Scale_Object
      *
      */
      void CTutorial8::HandleEvents(CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::Tutorial::CTutorial8::HandleEvents");
         
         if(pMsg.Id == iMsgRunningTick)
         {
            m_counter += 1;
            
            if(m_counter>=m_counter_max)
            {
               m_counter = 0;
               
               // switch state to control if the object rotates

               m_rotate_flag = !m_rotate_flag;
            }

            if(m_rotate_flag)
            {
               // Call CTutorial2's HandleEvents function, which will rotate the object

               CTutorial2::HandleEvents(pMsg);    
            }

            Scale_Object();
         }
            
         
         RWS_RETURNVOID();
      }
            
      /**
      *
      *  Handle Attributes, call CTutorial2's handle attributes function and handle attributes for this
      *  inherited class.
      *
      *  \param attr standard attribute packets, including one for tutorial 2 and this tutorials attribute controls.
      *  \see CTutorial2::HandleAttributes
      *
      */
      void CTutorial8::HandleAttributes(const CAttributePacket& attr)
      {
         RWS_FUNCTION("RWS::Tutorial::CTutorial8::HandleAttributes");
         
         // Call CTutorial2's HandleAttributes function which gives us access
         // to CTutorial2's atomic so that it can handle commands from the system.
         CTutorial2::HandleAttributes(attr);
         
         CAttributeCommandIterator attrIt(attr,RWS_CLASSID_OF(CTutorial8));
         
         while(!attrIt.IsFinished())
         {
            switch(attrIt->GetCommandId())
            {
            case CMD_counter_max:

               attrIt->GetCommandData(m_counter_max);
               break;
            }
            ++attrIt;
         }
         
         RWS_RETURNVOID();
      }

      /**
      *
      *  Scale_Object, use Atomic to scale the object by the internally held current scaling value. Also
      *  updates the scale value and flips the scale direction when the minimum / maximum values are met.
      *
      */
      void CTutorial8::Scale_Object(void)
      {
         RWS_FUNCTION("RWS::Tutorial::CTutorial8::Scale_Object");

         RwMatrix scale_Mat;
         RwV3d scale_vec;

         RwFrame* pFrame = RpAtomicGetFrame(m_pAtomic.ptr());

         if (m_scale_flag)
         {
            m_scale += 0.005f; // increase size
         }
         else
         {
            m_scale -= 0.005f; // reduce size
         }

         if (m_scale < 0.9f || m_scale > 1.1f)
         {
            if (!m_scale_flag)
            {
               // Make sure frames matrix remains orthonormal
               RwFrameOrthoNormalize(pFrame);
            }

            // reset counter
            m_scale = 1.0f;

            // switch state
            m_scale_flag = !m_scale_flag;
         }

         // Scale x, y, z
         scale_vec.x = m_scale;
         scale_vec.y = m_scale;
         scale_vec.z = m_scale;

         RwMatrixSetIdentity(&scale_Mat);

         RwMatrixScale(&scale_Mat, &scale_vec, rwCOMBINEPRECONCAT);

         RwFrameTransform(pFrame, &scale_Mat, rwCOMBINEPRECONCAT);

         RWS_RETURNVOID();
      }
      
    }//namespace Tutorial
}//namespace RWS