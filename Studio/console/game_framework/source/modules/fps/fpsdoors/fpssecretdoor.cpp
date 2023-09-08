/*****************************************************************************
*
* File :     FPSSecretDoor.cpp
*
* Abstract : Secret doors await a trigger event, and then use two transforms 
*            to move themselves. i.e. back followed by to the side.
*
*            This is implemented as a Finite State Machine
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
// Left to its own devices, the MW linker will dead-strip the registrar ctors, so...
#ifdef __MWERKS__
#pragma force_active on
#pragma exceptions off // Turn off exceptions due to bug in MW Dwarf Generator for PS2, present in CW 3.04
#endif

#include "fpssecretdoor.h"
#include "framework/toolkits/world/factory.h"
#include "framework/toolkits/math/maths.h"
#include <iomanip>

namespace RWS
{
   namespace FPS
   {
      RWS_IMPLEMENT_CLASSID(FPSSecretDoor);
      RWS_REGISTERCLASS(FPSSecretDoor);
      
      /**
      *
      * \ref FPSSecretDoor(). Constructor for FPSSecretDoor
      *
      * \see ~FPSSecretDoor
      */
      FPSSecretDoor::FPSSecretDoor (const CAttributePacket& attr) :
      InitCEventHandler(&m_pAtomic),
         m_FrameCount (0), 
         m_speed(150),
         m_Angle (0.0f), 
         m_spawnflags(0), 
         m_bAnimating (false),
         m_DoorState (DoorState0)
      {
         RWS_FUNCTION("RWS::FPS::FPSSecretDoor::FPSSecretDoor");
         
         // Can't use initializer ('this' isn't valid) so...
         m_pAtomic = CreateAtomicInWorldFromResource(attr, this);
         RWS_ASSERT(m_pAtomic, "Failed to create atomic");
         
         RegisterMsg(m_MsgTargetName, "ACTN_PLAYERTOUCH",0);
         LinkMsg (m_MsgTargetName, 0);
         
         m_Slide1 = RwMatrixCreate ();
         m_Slide2 = RwMatrixCreate ();
         m_Shift1 = RwMatrixCreate ();
         m_Shift2 = RwMatrixCreate ();
         
         RWS_RETURNVOID();
      }
      
      
      /*
      *
      * \ref ~FPSSecretDoor(). Destructor for FPSSecretDoor
      *
      * \see ~FPSSecretDoor
      */
      FPSSecretDoor::~FPSSecretDoor(void)
      {
         RWS_FUNCTION("RWS::FPS::FPSSecretDoor::~FPSSecretDoor");
         
         UnLinkMsg (iMsgRunningTick);
         UnLinkMsg(m_MsgTargetName);
         UnRegisterMsg(m_MsgTargetName);
         
         RWS_RETURNVOID();
      }
      
      
      /**
      *     
      * \ref HandleAttributes().  Handle Attributes for this behavior
      *
      * \param attr Standard Attribute Package       
      *
      */
      void FPSSecretDoor::HandleAttributes (const CAttributePacket& attr)
      {
         RWS_FUNCTION("RWS::FPS::FPSSecretDoor::HandleAttributes");
         
         CAttributeHandler::HandleAttributes(attr);
         
         // Initialize contained class first
         m_pAtomic.HandleSystemCommands(attr);
         
         CAttributeCommandIterator attrIt(attr, RWS_CLASSID_OF(FPSSecretDoor));
         
         while (!attrIt.IsFinished())
         {
            switch (attrIt->GetCommandId())
            {
            case CMD_targetname:
               {
                  if (m_MsgTargetName.Get_linked_count() > 0)
                  {
                     CEventHandler::ReplaceLinkedMsg(m_MsgTargetName, attrIt->GetAs_char_ptr(), 0);
                  }
                  else
                  {
                     CEventHandler::ReplaceRegisteredMsg(m_MsgTargetName, attrIt->GetAs_char_ptr(), 0);
                  }
               }
               break;
               
            case CMD_angle:
               {
                  RwReal angle;
                  
                  attrIt->GetCommandData(angle);
                  
                  m_Angle = RWDEG2RAD (angle);
               }
               break;
               
            case CMD_spawnflags:
               attrIt->GetCommandData(m_spawnflags);
               break;
               
            case CMD_speed:
               attrIt->GetCommandData(m_speed);
               break;
            }
            ++attrIt;
         }
         
         RWS_RETURNVOID();
      }
      
      
      /**
      *
      * \ref TriggerDoor().  When a message is triggered to activate the secret door,  
      * set the door state to 'DoorState1' which shifts the door backwards. The actual 
      * code to move the door is contained in \ref HandleEvents.  A secret door can be activated
      * by either shooting it,  or being triggered by an event,  which is by default set to 
      * ACTN_PLAYERTOUCH.
      *
      */
      void FPSSecretDoor::TriggerDoor(void)
      {
         RWS_FUNCTION("RWS::FPS::FPSSecretDoor::TriggerDoor");
         
         UnLinkMsg(m_MsgTargetName);
         
         // Open secret door
         GenerateTransformationMatrices ();
         LinkMsg (iMsgRunningTick, 0);
         
         m_DoorState = DoorState1;
         m_FrameCount = 0;
         m_bAnimating = true;
         
         RWS_RETURNVOID();
      }
      
      
      /**
      *
      * \ref ShiftBack(). Door state 1,  Shift the door backwards
      *
      * \see  Door_Wait, Door_Slide, Door_Wait_Return, Door_Return_Slide, Door_Wait_Slide, Door_Shift_Return
      *
      */
      void FPSSecretDoor::Door_ShiftBack()
      {
         RWS_FUNCTION("RWS::FPS::FPSSecretDoor::Dorr_ShiftBack");
         
         if (++m_FrameCount < m_NoShiftFrames)
         {
            RwFrameTransform (RpAtomicGetFrame(m_pAtomic.ptr()), m_Shift1, rwCOMBINEPRECONCAT);
         }
         else
         {
            m_DoorState = DoorState2;
            m_FrameCount = 0;
         }
         
         RWS_RETURNVOID();
      }
      
      
      /**
      *
      * \ref Door_Wait(). Door state 2,  Make the door wait before proceeding to next state.
      *
      *
      * \see  Door_ShiftBack, Door_Slide, Door_Wait_Return, Door_Return_Slide, Door_Wait_Slide, Door_Shift_Return
      *       
      */
      void FPSSecretDoor::Door_Wait( void )
      {
         RWS_FUNCTION("RWS::FPS::FPSSecretDoor::Door_Wait");
         
         // wait 1/3 second
         if (!(++m_FrameCount < 20)) 
         {
            m_DoorState = DoorState3;
            m_FrameCount = 0;
         }
         
         RWS_RETURNVOID();
      }
      
      
      /**
      *
      * \ref Door_Slide(). Door state 3,  Make the door slide sideways.
      *
      * \see  Door_ShiftBack, Door_Wait, Door_Wait_Return, Door_Return_Slide, Door_Wait_Slide, Door_Shift_Return
      *
      */
      void FPSSecretDoor::Door_Slide( void )
      {
         RWS_FUNCTION("RWS::FPS::FPSSecretDoor::Door_Slide");
         
         if (++m_FrameCount < m_NoSlideFrames)
         {
            RwFrameTransform (RpAtomicGetFrame(m_pAtomic.ptr()), m_Slide1, rwCOMBINEPRECONCAT);
         }
         else
         {
            if ( (m_spawnflags&SECRET_OPEN_ONCE) == SECRET_OPEN_ONCE)
            {
               // Unlink from running tick
               UnLinkMsg(iMsgRunningTick);
               
               m_DoorState = DoorState_Complete;
            }
            else
            {
               m_DoorState = DoorState4;
               m_FrameCount = 0;
            }
         }
         
         RWS_RETURNVOID();
      }
      
      
      /**
      *
      * \ref Door_Wait_Return(). Door state 4,  Make the door wait until it is ready to return.
      *
      * \see  Door_ShiftBack, Door_Wait, Door_Slide,  Door_Return_Slide, Door_Wait_Slide, Door_Shift_Return
      *
      */
      void FPSSecretDoor::Door_Wait_Return( void )
      {
         RWS_FUNCTION("RWS::FPS::FPSSecretDoor::Door_Wait_Return");
         
         // wait 5 secs
         if (!(++m_FrameCount < 300))  
         {
            m_DoorState = DoorState5;
            m_FrameCount = 0;
         }
         
         RWS_RETURNVOID();
      }
      
      
      /**
      *
      * \ref Door_Return_Slide(). Door state 5,  Make the door Door slide back to original 
      * position before moving forward to close door.
      *
      * \see  Door_ShiftBack, Door_Wait, Door_Slide, Door_Wait_Return,  Door_Wait_Slide, Door_Shift_Return        
      *
      */
      void FPSSecretDoor::Door_Return_Slide( void )
      {
         RWS_FUNCTION("RWS::FPS::FPSSecretDoor::Door_Return_Slide");
         
         if (++m_FrameCount < m_NoSlideFrames)
         {
            RwFrameTransform (RpAtomicGetFrame(m_pAtomic.ptr()), m_Slide2, rwCOMBINEPRECONCAT);
         }
         else
         {
            m_DoorState = DoorState6;
            m_FrameCount = 0;
         }
         
         RWS_RETURNVOID();
      }
      
      
      /**
      *
      * \ref Door_Wait_Slide(). Door state 6, Wait before moving to original position.
      *
      * \see  Door_ShiftBack, Door_Wait, Door_Slide, Door_Wait_Return, Door_Return_Slide,  Door_Shift_Return
      *
      */
      void FPSSecretDoor::Door_Wait_Slide( void )
      {
         RWS_FUNCTION("RWS::FPS::FPSSecretDoor::Door_Wait_Slide");
         
         // Wait 1/3 second
         if (!(++m_FrameCount < 20)) 
         {
            m_DoorState = DoorState7;
            m_FrameCount = 0;
         }
         
         RWS_RETURNVOID();
      }
      
      
      /**
      *
      * \ref Door_Shift_Return(). Door state 6, Return to original position.
      *
      * \see  Door_ShiftBack, Door_Wait, Door_Slide, Door_Wait_Return, Door_Return_Slide, Door_Wait_Slide
      *
      */
      void FPSSecretDoor::Door_Shift_Return( void )
      {
         RWS_FUNCTION("RWS::FPS::FPSSecretDoor::Door_Shift_Return");
         
         if (++m_FrameCount < m_NoShiftFrames)
         {
            RwFrameTransform (RpAtomicGetFrame(m_pAtomic.ptr()), m_Shift2, rwCOMBINEPRECONCAT);
         }
         else
         {
            UnLinkMsg (iMsgRunningTick);
            m_DoorState = DoorState0;
            m_FrameCount = 0;
            m_bAnimating = false;
            
            LinkMsg(m_MsgTargetName, 0);
         }
         
         RWS_RETURNVOID();
      }
      
      
      /**
      *
      * \ref HandleEvents(). Handle events.  HandleEvents controls the state of the secret 
      * door once it has been triggered.      
      *
      * \param msg Standard Message Package
      *  
      * \ref iMsgRunningTick
      */
      void FPSSecretDoor::HandleEvents (CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::FPS::FPSSecretDoor::HandleEvents");
         
         // Check for trigger event
         if ((m_DoorState == DoorState0) && !m_bAnimating)
         {
            if (pMsg.Id == m_MsgTargetName)
            {
               TriggerDoor();
            }
         }
         else if (pMsg.Id == iMsgRunningTick)
         {
            switch (m_DoorState)
            {
               // Shift back
            case DoorState1:  
               {
                  Door_ShiftBack();
               }
               break;
               
               // Wait after first movement
            case DoorState2: 
               {
                  Door_Wait();
               }
               break;
               
               // Sliding sideways
            case DoorState3: 
               {
                  Door_Slide();
               }
               break;
               
               // Wait here until ready to go back
            case DoorState4: 
               {
                  Door_Wait_Return();
               }
               break;
               
               // Return sliding sideways
            case DoorState5: 
               {
                  Door_Return_Slide( );
               }
               break;
               
               // Wait for final slide
            case DoorState6: 
               {
                  Door_Wait_Slide();
               }
               break;
               
               // Return shifting forwards
            case DoorState7: 
               {
                  Door_Shift_Return();
               }
               break;
               
               // Do nothing added here for completeness
            case DoorState_Complete:   
            case DoorState0:
               break;
            }
         }
         
         RWS_RETURNVOID();
      }
      
      
      /**
      * 
      *
      * Calculate the total bounding box of the atomics
      *  
      *
      */     
      RpAtomic *FPSSecretDoor::CalculateBoundingBox (RpAtomic *atomic, void *data)
      {
         RWS_FUNCTION("RWS::FPS::FPSSecretDoor::CalculateBoundingBox");
         
         RWS_PRE( atomic )
            RWS_PRE( data )
            
            RwBBox *bbox = reinterpret_cast<RwBBox*>(data);
         
         RpGeometry *geom = RpAtomicGetGeometry(atomic);
         
         if (geom)
         {
            RpMorphTarget *morph = RpGeometryGetMorphTarget(geom, 0);
            
            if (morph)
            {
               RwBBox tbbox;
               
               RwBBoxCalculate(&tbbox, RpMorphTargetGetVertices(morph), RpGeometryGetNumVertices(geom));
               
               // Need to extend the bounding box by this bounding box
               if (tbbox.sup.x > bbox->sup.x) bbox->sup.x = tbbox.sup.x;
               if (tbbox.sup.y > bbox->sup.y) bbox->sup.y = tbbox.sup.y;
               if (tbbox.sup.z > bbox->sup.z) bbox->sup.z = tbbox.sup.z;
               
               if (tbbox.inf.x < bbox->inf.x) bbox->inf.x = tbbox.inf.x;
               if (tbbox.inf.y < bbox->inf.y) bbox->inf.y = tbbox.inf.y;
               if (tbbox.inf.z < bbox->inf.z) bbox->inf.z = tbbox.inf.z;
               
               RWS_TRACE("bbox->sup " << bbox->sup.x << " " << bbox->sup.y << " " << bbox->sup.z)
                  RWS_TRACE("bbox->inf " << bbox->inf.x << " " << bbox->inf.y << " " << bbox->inf.z)
            }
         }
         
         RWS_RETURN( atomic );
      }
      
      
      /*
      *
      *
      * Generate the required transforms
      *
      *
      */
      void FPSSecretDoor::GenerateTransformationMatrices (void)
      {
         RWS_FUNCTION("RWS::FPS::FPSSecretDoor::GenerateTransformationMatrices");
         
         RwBBox bbox = {{-9999.0f, -9999.0f, -9999.0f}, {9999.0f, 9999.0f, 9999.0}};
         CalculateBoundingBox(m_pAtomic, &bbox);
         
         // Calculate the dimensions of the door
         RwV3d size, shift, slide;
         RwV3dSub(&size, &bbox.sup, &bbox.inf);
         RWS_TRACE("BBox dims (" << RWS_FIX(size.x, 2) << " " << RWS_FIX(size.y, 2) << " " << RWS_FIX(size.z, 2) << ")");
         
         // slide is a vector pointing in the direction of the sliding movement
         slide.x = RwSin (m_Angle);
         slide.y = 0.0f;
         slide.z = RwCos (m_Angle);
         
         // shift points perpendicular left or right of the slide vector
         RwReal tscale;
         
         if ( (m_spawnflags&SECRET_1ST_LEFT) == SECRET_1ST_LEFT ) tscale = 0.5f;
         else                                                    tscale = 0.5f;
         
         RwReal ShiftAngle = m_Angle + rwPI * tscale;//(m_FirstLeft ? 0.5f : +0.5f); 
         shift.x = RwSin (ShiftAngle);
         shift.y = 0.0f;
         shift.z = RwCos (ShiftAngle);
         
         // The direction vectors need to be scaled according to speed and dimensions
         // ShiftScale is set to the positive size of the dimension we are shifting in
         RwReal ShiftScale = RwV3dDotProduct (&shift, &size);
         
         if (ShiftScale < 0.0f) ShiftScale *= -1.0f;
         
         // Scale this so that the door is travelling at a rate of m_speed units per 60 frames
         m_NoShiftFrames = static_cast<RwUInt32>(ShiftScale * RWS::MainLoop::Logic::Rate / m_speed);
         
         RWS_TRACE("FPSSecretDoor::GenerateTransformationMatrices");
         RWS_TRACE("m_NoShiftFrames = "<<ShiftScale<<" * RWS::MainLoop::Logic::Rate / "<<m_speed);
         
         RwV3dScale (&shift, &shift, ShiftScale / static_cast<RwReal>(m_NoShiftFrames));
         
         // Create a matrix to move by the required amount, and it's inverse to move back.
         RwMatrixTranslate (m_Shift1, &shift, rwCOMBINEREPLACE);
         RwMatrixInvert (m_Shift2, m_Shift1);
         
         // Same as above but for the door slide matrix
         RwReal SlideScale = RwV3dDotProduct (&slide, &size);
         if (SlideScale < 0.0f) SlideScale *= -1.0f;
         m_NoSlideFrames = static_cast<RwUInt32>(RWS::MainLoop::Logic::Rate * SlideScale / m_speed);
         
         RWS_TRACE("m_NoSlideFrames = "<<SlideScale<<" * RWS::MainLoop::Logic::Rate / "<<m_speed);
         
         RwV3dScale (&slide, &slide, SlideScale / static_cast<RwReal>(m_NoSlideFrames));
         
         RwMatrixTranslate (m_Slide1, &slide, rwCOMBINEREPLACE);
         RwMatrixInvert (m_Slide2, m_Slide1);
         
         RWS_RETURNVOID();
      }
   } // namespace FPS
} // namespace RWS
