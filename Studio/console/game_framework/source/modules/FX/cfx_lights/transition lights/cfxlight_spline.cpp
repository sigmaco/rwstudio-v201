/*****************************************************************************
*
* File :     CFXLight_Spline.cpp
*
* Abstract : Implementation of CFXLight_Spline class. This type of light
*            can be triggered to turn on and off by triggering an event.  This
*            can be useful if you need to trigger a warning light within a 
*            game for example. the colors CFXLight_Spline are set by using
*            a spline.
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
// Left to its own devices, the MW linker will dead-strip the registrar ctors, so...
#ifdef __MWERKS__
#pragma force_active on
#pragma exceptions off // Turn off exceptions due to bug in MW Dwarf Generator for PS2, present in CW 3.04
#endif

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "cfxlight_spline.h"
#include "framework/core/macros/debugmacros.h"
#include "framework/toolkits/math/maths.h"
#include "framework/toolkits/world/clevel.h"


namespace RWS
{
   
   namespace FX
   {

      /**
      *
      * Using this behavior you can make flickering light effects and pulsing effects.  you do this be selecting
      * a start and end color and a interpolation type,  e.g. PLAY_ONCE, LOOP, and PING PONG.  The speed can 
      * be set using the Step value.
      * 
      */
      RWS_IMPLEMENT_CLASSID(CFXLight_Spline);
      RWS_REGISTERCLASS(CFXLight_Spline);
          
      const RwReal Angle180Degrees = 180.0f;
    
    
      /**
      *
      *  Construct a CFXLight_Spline object and initialize member data.
      *
      */
      CFXLight_Spline::CFXLight_Spline(const CAttributePacket& rAttr): CFXBaseLight(rAttr),
          InitCEventHandler(&m_pLight)
      {
         RWS_FUNCTION("RWS::FX::CFXLight_Spline::CFXLight_Spline");

         m_mat = RwMatrixCreate();
         RWS_POST(m_mat);
        
         m_rot[0]        = 0.0f;
         m_rot[1]        = 0.0f;  
         m_rot[2]        = 0.0f;

         for (int numCols = 0; numCols < 5; numCols++)
         {
             color[0].red      = 0;
             color[0].green    = 0;
             color[0].blue     = 0;
             color[0].alpha    = 0;
         }

         pingpong        = false;
         step            = 0;
         tempstep        = 0;

         // Make sure light is not attached to the world,  until an event has been received.
         RemoveLightFromWorld();
         RWS_RETURNVOID();
      }

      /**
      *
      *  Destroy a CFXLight_Spline object.
      *
      */
      CFXLight_Spline::~CFXLight_Spline() 
      {
          RWS_FUNCTION("RWS::FX::CFXLight_Spline::~CFXLight_Spline");

          RWS_PRE(m_mat);
          RwMatrixDestroy(m_mat);
          m_mat = 0;
          UnLinkMsg(iMsgRunningTick);
          UnLinkMsg(m_trigger);
          UnRegisterMsg(m_trigger);

          RWS_RETURNVOID();
      } 
    
      /**
      *
      *  Set lights rotation matrix
      *
      */
      void CFXLight_Spline::RotateLight( void )
      {
          RWS_FUNCTION("RWS::FX::CFXLight_Spline::RotateLight");

          // Set lights rotation matrix
          RwMatrixRotate(m_mat, &XAxis, m_rot[0],rwCOMBINEREPLACE);
          RwMatrixRotate(m_mat, &YAxis, m_rot[1],rwCOMBINEPRECONCAT);
          RwMatrixRotate(m_mat, &ZAxis, m_rot[2],rwCOMBINEPRECONCAT);            

          RWS_RETURNVOID();
      }
       
      /**
      *
      *  Handle events.  Interpolate between source and destination RGB values
      *  
      */
      void CFXLight_Spline::HandleEvents(CMsg &pMsg)
      {      
         RWS_FUNCTION("RWS::FX::CFXLight_Spline::HandleEvents");

         // If connected to the running tick
         if (pMsg.Id == iMsgRunningTick)
         {   
             UpdateLight();
         }
         // if event has been received to start this behaviour running
         else if (pMsg.Id == m_trigger)
         {
             EnableLight();
         }

         RWS_RETURNVOID();
      }
    
      /**
      *
      *  Transform and interpolate light source
      *  
      */
      void CFXLight_Spline::UpdateLight( void )
      {
          RWS_FUNCTION("RWS::FX::CFXLight_Spline::UpdateLight");

          RwFrameTransform(RpLightGetFrame(m_pLight.ptr()), m_mat,rwCOMBINEPRECONCAT);

          // Interpolate the colors
          LERP_Lights();

          RWS_RETURNVOID();
      }

      /**
      *
      *  Set and add light to world,  link to running tick
      *  
      */
      void CFXLight_Spline::EnableLight( void)
      {
         RWS_FUNCTION("RWS::FX::CFXLight_Spline::EnableLight");

         // ifnot linked to running tick link and add light to the world
         if (IsLinked(iMsgRunningTick) == false)
         {
             AddLightToWorld();
             LinkMsg(iMsgRunningTick);
         }
         else
         {
            // remove light from world and unlink from running tick
            RemoveLightFromWorld();
            UnLinkMsg(iMsgRunningTick);
         }

          RWS_RETURNVOID();
      }

      /**
      *
      *  Handle attributes.
      *
      */
      void CFXLight_Spline::HandleAttributes(const CAttributePacket& attr)
      {  
         RWS_FUNCTION("RWS::FX::CFXLight_Spline::HandleAttributes");

         CFXBaseLight::HandleAttributes(attr);  
         CAttributeCommandIterator attrIt(attr, RWS_CLASSID_OF(CFXLight_Spline));

         while (!attrIt.IsFinished())
         {
            switch (attrIt->GetCommandId())
            {
               case CMD_Set_c0:
               {
                  RwRGBA Color = attrIt->GetAs_RwRGBA();
                  RwRGBARealFromRwRGBA  (&color[0], &Color);
               }
               break;

               case CMD_Set_c1:
               {
                  RwRGBA Color = attrIt->GetAs_RwRGBA();
                  RwRGBARealFromRwRGBA  (&color[1], &Color);
               }
               break;

               case CMD_Set_c2:
               {
                  RwRGBA Color = attrIt->GetAs_RwRGBA();
                  RwRGBARealFromRwRGBA  (&color[2], &Color);                  
               }
               break;

               case CMD_Set_c3:
               {
                  RwRGBA Color = attrIt->GetAs_RwRGBA();
                  RwRGBARealFromRwRGBA  (&color[3], &Color);
               }
               break;

               case CMD_Set_c4:
               {
                  RwRGBA Color = attrIt->GetAs_RwRGBA();
                  RwRGBARealFromRwRGBA  (&color[4], &Color);
               }
               break;

               case CMD_rot_x:
                  // Get X rotation value
                  attrIt->GetCommandData(m_rot[0]);
                  m_rot[0] = (m_rot[0] / Angle180Degrees) * rwPI;
                  RotateLight();
               break;

               case CMD_rot_y:
                  // Get Y rotation value
                  attrIt->GetCommandData(m_rot[1]);
                  m_rot[1] = (m_rot[1] / Angle180Degrees) * rwPI;
                  RotateLight();
               break;

               case CMD_rot_z:
                  // Get Z rotation value
                  attrIt->GetCommandData(m_rot[2]);
                  m_rot[2] = (m_rot[2] / Angle180Degrees) * rwPI;
                  RotateLight();
               break;

               case CMD_m_trigger:
                  // Re-register new trigger event
                  ReplaceLinkedMsg ( m_trigger,  attrIt->GetAs_RwChar_ptr(), 0);
               break;

               case CMD_step:
                  // Get new step value
                  step = attrIt->GetAs_RwReal();
            break;
            }

            ++attrIt;
         }

      RWS_RETURNVOID();
      }

      /**
      *
      *  Interpolate color values based on playback type
      *
      */
      void CFXLight_Spline::LERP_Lights(void)
      {
          RWS_FUNCTION("RWS::FX::CFXLight_Spline::LERP_Lights");

          RwRGBAReal temp;

          tempstep = tempstep + step;

          if (tempstep >= 1.0f) tempstep = 0.0f;

          EvaluateColor(color[0], color[1], color[2], color[3],color[4], tempstep, temp);

          // Set light
          RpLightSetColor(m_pLight, &temp);

          RWS_RETURNVOID();
      }

      //////////////////////////////////////////////////////////////////
      //
      // Evaluate a Catmull-Rom / Overhauser spline at a specific position along it
      //
      // Note: This type of spline passes through all control points.
      //
      // Given control points in {v0, v1, v2, v3} and a value 0<=t<=1.0f this function returns points on the spline 
      // between v1 and v2
      //
      // To do more than two points just step through the array of points using the previous point, 
      // the current point and the next two points as the four points for the spline. For each of these segments 
      // draw a curve for 0<t<1. 
      // This curve will be between the current point and the next point. 
      //
      // In : v0, v1, v2, v3 = 4 vectors representing the spline control points.
      //                  t = parametric step 0.0 > 1.0
      //                  vr = pointer to vector to receive point
      //
      // EvaluateColor extends this to evaluate a color along 5 control points, note 5 points gives you 4 sections
      //
      // c0 -> c1 
      // c1 -> c2
      // c2 -> c3
      // c3 -> c4
      //
      void CFXLight_Spline::EvaluateColor(const RwRGBAReal &c0, const RwRGBAReal &c1, const RwRGBAReal &c2, 
                                          const RwRGBAReal &c3, const RwRGBAReal &c4, const RwReal t, RwRGBAReal &cr)
      {
         RWS_FUNCTION("RWS::FX::CFXLight_Spline::EvaluateColor");
         
         RwReal      s = t * 4.0f;  // 4 sections
         
         RwReal      spow2;
         RwReal      spow3;
         
         const RwRGBAReal *tcr0;
         const RwRGBAReal *tcr1;
         const RwRGBAReal *tcr2;
         const RwRGBAReal *tcr3;

         if (s<1.0f)
         {
            tcr0 = &c0;         tcr1 = &c0;         tcr2 = &c1;         tcr3 = &c2;
         }
         else
         if (s<2.0f)
         {
            tcr0 = &c0;         tcr1 = &c1;         tcr2 = &c2;         tcr3 = &c3;

            s = s - 1.0f;  // 0..1
         }
         else if (s<3.0f)
         {
            tcr0 = &c0;         tcr1 = &c2;         tcr2 = &c3;         tcr3 = &c3;

            s = s - 2.0f;  // 0..1
         }
         else
         {
            tcr0 = &c2;         tcr1 = &c3;         tcr2 = &c4;         tcr3 = &c4;

            s = s - 3.0f;  // 0..1
         }

         // Generate point on spline
         //
         spow2 = s * s;
         spow3 = s * spow2;

         cr.red = 0.5f * (2.0f * tcr1->red + s * (tcr2->red - tcr0->red) 
                + spow2 * (2.0f * tcr0->red - 5.0f * tcr1->red + 4.0f * tcr2->red - tcr3->red) 
                + spow3 * (3.0f * tcr1->red - 3.0f * tcr2->red + tcr3->red - tcr0->red));

         cr.green = 0.5f * (2.0f * tcr1->green + s * (tcr2->green - tcr0->green) 
                 + spow2 * (2.0f * tcr0->green - 5.0f * tcr1->green + 4.0f * tcr2->green - tcr3->green) 
                  + spow3 * (3.0f * tcr1->green - 3.0f * tcr2->green + tcr3->green - tcr0->green));

         cr.blue = 0.5f * (2.0f * tcr1->blue + s * (tcr2->blue - tcr0->blue) 
                 + spow2 * (2.0f * tcr0->blue - 5.0f * tcr1->blue + 4.0f * tcr2->blue - tcr3->blue) 
                 + spow3 * (3.0f * tcr1->blue - 3.0f * tcr2->blue + tcr3->blue - tcr0->blue));

         cr.alpha = 0.5f * (2.0f * tcr1->alpha + s * (tcr2->alpha - tcr0->alpha)
                  + spow2 * (2.0f * tcr0->alpha - 5.0f * tcr1->alpha + 4.0f * tcr2->alpha - tcr3->alpha) 
                  + spow3 * (3.0f * tcr1->alpha - 3.0f * tcr2->alpha + tcr3->alpha - tcr0->alpha));

         // Clamp rgba values
         //
         if (cr.red < 0) cr.red = 0.0f;
         if (cr.red > 1.0f) cr.red = 1.0f;

         if (cr.green < 0) cr.green = 0.0f;
         if (cr.green > 1.0f) cr.green = 1.0f;

         if (cr.blue < 0) cr.blue = 0.0f;
         if (cr.blue > 1.0f) cr.blue = 1.0f;

         if (cr.alpha < 0) cr.alpha = 0.0f;
         if (cr.alpha > 1.0f) cr.alpha = 1.0f;

         RWS_RETURNVOID();
      }
   } // namespace FX
} // namespace RWS
