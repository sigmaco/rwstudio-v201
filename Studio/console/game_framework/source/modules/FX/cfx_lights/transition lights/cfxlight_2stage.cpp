/*****************************************************************************
*
* File :     CFXLight_2Stage.cpp
*
* Abstract : Implementation of CFXLight_2Stage class. This type of light
*            can be triggered to turn on and off by triggering an event.  This
*            can be useful if you need to trigger a warning light within a 
*            game for example.  CFXLight_2Stage  fades between  source and destination
*            colors.
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
#include "cfxlight_2stage.h"
#include "framework/core/macros/debugmacros.h"
#include "framework/toolkits/math/maths.h"
#include "framework/toolkits/world/clevel.h"

namespace RWS
{
   
   namespace FX
   {
      /**
      *
      * Using this behavior you can make flickering and pulsing light effects. You do this be selecting
      * a start and end color and an interpolation type,  e.g. PLAY_ONCE, LOOP, and PING PONG.  The speed can be set 
      * using the step value.
      * 
      */
      RWS_IMPLEMENT_CLASSID(CFXLight_2Stage);
      RWS_REGISTERCLASS(CFXLight_2Stage);
      
      const RwReal Angle180Degrees = 180.0f;
   
      /**
      *
      *  Construct a CFXLight_2Stage object and initialize member data.
      *
      */
      CFXLight_2Stage::CFXLight_2Stage(const CAttributePacket& rAttr)
         : CFXBaseLight(rAttr),
         InitCEventHandler(&m_pLight),
         pingpong(false),
         step(0),
         running(false),
         LerpType(PLAY_ONCE),
         LerpFinishRed(false),
         LerpFinishGreen(false),
         LerpFinishBlue(false)
      {
         RWS_FUNCTION("RWS::FX::CFXLight_2Stage::CFXLight_2Stage");
         
         m_mat = RwMatrixCreate();
         RWS_POST(m_mat);
         
         m_rot[0]       = 0.0f;
         m_rot[1]       = 0.0f;  
         m_rot[2]       = 0.0f;
         
         color1.red     = 0.0f;
         color1.green   = 0.0f;
         color1.blue    = 0.0f;
         color1.alpha   = 0.0f;
         
         color2.red     = 0.0f;
         color2.green   = 0.0f;
         color2.blue    = 0.0f;
         color2.alpha   = 0.0f;
         
         temp.red       = 0.0f;
         temp.green     = 0.0f;
         temp.blue      = 0.0f;
         temp.alpha     = 0.0f;
         
         // Setup pointers for use in Ping-Pong state
         pColor1 = &color1;
         pColor2 = &color2;

         // Make sure light is not attached to the world, until an event has been received.
         RemoveLightFromWorld();
         
         RWS_RETURNVOID();
      }
   
      /**
      *
      *  Destroy a CFXLight_2Stage object.
      *
      */
      CFXLight_2Stage::~CFXLight_2Stage() 
      {
         RWS_FUNCTION("RWS::FX::CFXLight_2Stage::~CFXLight_2Stage");
         
         RWS_PRE(m_mat);
         RwMatrixDestroy(m_mat);
         m_mat = 0;
         
         UnLinkMsg(iMsgRunningTick);
         
         UnLinkMsg(m_trigger_enable);
         UnRegisterMsg(m_trigger_enable);
         
         UnLinkMsg(m_trigger_disable);
         UnRegisterMsg(m_trigger_disable);
         
         RWS_RETURNVOID();
      }
   
      /**
      *
      *  Set lights rotation matrix.
      *
      */
      void CFXLight_2Stage::RotateLight( void )
      {  
         RWS_FUNCTION("RWS::FX::CFXLight_2Stage::RotateLight");
         
         // Set lights rotation matrix
         RwMatrixRotate(m_mat, &XAxis,m_rot[0], rwCOMBINEREPLACE);
         RwMatrixRotate(m_mat, &YAxis,m_rot[1], rwCOMBINEPRECONCAT);
         RwMatrixRotate(m_mat, &ZAxis,m_rot[2], rwCOMBINEPRECONCAT);      

         RWS_RETURNVOID();
      }
   
      /**
      *
      *  Handle events.  Interpolate between source and destination RGB values.
      *  
      */
      void CFXLight_2Stage::HandleEvents(CMsg &pMsg)
      {    
         RWS_FUNCTION("RWS::FX::CFXLight_2Stage::HandleEvents");

         // If connected to the running tick.
         if (pMsg.Id == iMsgRunningTick)
         {
            UpdateLight();
         }
         // Handle both in case m_trigger_enable is same as m_trigger_disable event.
         //
         else if (pMsg.Id == m_trigger_enable && EnableLight(pMsg))
         {
         }
         else if (pMsg.Id == m_trigger_disable && DisableLight(pMsg))
         {
         }
         
         RWS_RETURNVOID();
      }
   
      /**
      *
      *  Transform and interpolate light source.
      *  
      */
      void CFXLight_2Stage::UpdateLight( void )
      {
         RWS_FUNCTION("RWS::FX::CFXLight_2Stage::UpdateLight");

         RwFrameTransform(RpLightGetFrame(m_pLight.ptr()), m_mat, rwCOMBINEPRECONCAT);
      
         // Interpolate the colors.
         LERP_Lights();
         
         RWS_RETURNVOID();
      }
   
      /**
      *
      *  If not already in the world add the light and start animating.
      *  
      *  \return True if the light has been added to the world, if the light was already in the world
      *  returns false.
      */
      bool CFXLight_2Stage::EnableLight(CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::FX::CFXLight_2Stage::EnableLight");
         
         bool ret = false;

         // If not linked to the world link it.
         //
         if (RpLightGetWorld(m_pLight) == 0)
         {
            AddLightToWorld();
            LinkMsg(iMsgRunningTick);
            
            ret = true;
         }

         // Update position of light.
         //
         if (pMsg.pData && m_reset_pos_on_trigger)
         {
               RwFrame* pSFrame = static_cast<RwFrame*>(pMsg.pData);

               RwMatrix *pSMatrix = RwFrameGetLTM(pSFrame);

               if (pSMatrix && m_pLight)
               {
                  RwFrameTransform(RpLightGetFrame(m_pLight.ptr()), pSMatrix, rwCOMBINEREPLACE);
               }
         }

         // Re-trigger effect.
         //
         if (m_reset_on_trigger)
         {
            LerpFinishRed  = false;
            LerpFinishGreen   = false;
            LerpFinishBlue = false;
            pingpong       = false;
         }
         
         RWS_RETURN(ret);
      }
      
      /**
      *
      *  If the light is attached to the world, remove it, and stop animating.
      *  
      *  \return True if the light has been removed from the world, if the light wasn't already in the world
      *  returns false.
      */
      bool CFXLight_2Stage::DisableLight( CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::FX::CFXLight_2Stage::DisableLight");
         
         bool ret = false;
         // If linked to the world
         //
         if (RpLightGetWorld(m_pLight) != 0)
         {
            // remove light from world and unlink from running tick
            RemoveLightFromWorld();
            UnLinkMsg(iMsgRunningTick);
            
            ret = true;
         }
         
         // Update position of light
         //
         if (pMsg.pData && m_reset_pos_on_trigger)
         {
               RwFrame* pSFrame = static_cast<RwFrame*>(pMsg.pData);

               RwMatrix *pSMatrix = RwFrameGetLTM(pSFrame);

               if (pSMatrix && m_pLight)
               {
                  RwFrameTransform(RpLightGetFrame(m_pLight.ptr()), pSMatrix, rwCOMBINEREPLACE);
               }
         }
         
         RWS_RETURN(ret);
      }
   
      /**
      *
      *  Handle attributes.
      *
      */
      void CFXLight_2Stage::HandleAttributes(const CAttributePacket& attr)
      {    
         RWS_FUNCTION("RWS::FX::CFXLight_2Stage::HandleAttributes");
         
         CFXBaseLight::HandleAttributes(attr);    
         CAttributeCommandIterator attrIt(attr, RWS_CLASSID_OF(CFXLight_2Stage));
      
         while (!attrIt.IsFinished())
         {
            switch (attrIt->GetCommandId())
            {            
            case CMD_Set_t_color1:
               {
                  RwRGBA Color = attrIt->GetAs_RwRGBA();
                  RwRGBARealFromRwRGBA  (&color1, &Color);
               }
               break;
               
            case CMD_Set_t_color2:
               {
                  RwRGBA Color = attrIt->GetAs_RwRGBA();
                  RwRGBARealFromRwRGBA  (&color2, &Color);
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
               
            case CMD_m_trigger_enable:
               // Re-register new trigger event
               ReplaceLinkedMsg (m_trigger_enable, attrIt->GetAs_RwChar_ptr(), "RwFrame*");
               break;
               
            case CMD_m_trigger_disable:
               // Re-register new trigger event
               ReplaceLinkedMsg (m_trigger_disable, attrIt->GetAs_RwChar_ptr(), "RwFrame*");
               break;
               
            case CMD_Playback_type:
               {
                  RwUInt32 type;
                  
                  attrIt->GetCommandData(type);

                  LerpFinishRed    = false;
                  LerpFinishGreen  = false;
                  LerpFinishBlue   = false;
                 
                  // LIST sends the element to select, need to convert this to the 
                  // define for the light type as they are not 0,1,2
                  switch (type)
                  {
                  case 0: 
                     LerpType = PLAY_ONCE; 
                     break;
                  case 1: 
                     LerpType = PING_PONG; 
                     break;
                  case 2: 
                     LerpType = LOOP; 
                     break;
                     
                  default:
                     RWS_ASSERTFAIL("CFXLight_2Stage::HandleAttributes illegal parameter value.");
                     break;
                  }
                  
                  // Reset pointers to correct addresses.
                  pColor1 = &color1;
                  pColor2 = &color2;
               }
               break;
               
            case CMD_step:
               // Get new step value
               attrIt->GetCommandData(step);
               break;
               
            case CMD_m_reset_on_trigger:
               attrIt->GetCommandData(m_reset_on_trigger);
               break;
               
            case CMD_m_reset_pos_on_trigger:
               attrIt->GetCommandData(m_reset_pos_on_trigger);
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
      void CFXLight_2Stage::LERP_Lights(void)
      {
         RWS_FUNCTION("RWS::FX::CFXLight_2Stage::LERP_Lights");

         switch (LerpType)
         {
            // Only interpolate once
            case PLAY_ONCE: 
               ColorCycle();
            break;

            // Interpolate backwards and forwards
            case PING_PONG: 
               ColorCycle();

               // If lerp finished
               if ((LerpFinishRed) && (LerpFinishGreen) && (LerpFinishBlue))
               {
                  // Toggle pingpong state
                  pingpong = !pingpong;

                  // Swap the pointers to the two colors
                  RwRGBAReal * pTmp;
                  pTmp = pColor1;
                  pColor1 = pColor2;
                  pColor2 = pTmp;

                  LerpFinishRed    = false;
                  LerpFinishGreen  = false;
                  LerpFinishBlue   = false;
               }

            break;

            // Interpolate looped
            case LOOP: 
               ColorCycle();

               // If lerp finished
               if ((LerpFinishRed) && (LerpFinishGreen) && (LerpFinishBlue))
               {
                  // Restore source color
                  temp = color1;

                  LerpFinishRed    = false;
                  LerpFinishGreen  = false;
                  LerpFinishBlue   = false;
               }

            break;
         }

         RpLightSetColor(m_pLight, &temp);
         RWS_RETURNVOID();
      }
    
      /**
      *
      *  cycle using a step values between a source and destination RGB value
      *
      */
      void CFXLight_2Stage::ColorCycle( void )
      {
         RWS_FUNCTION("RWS::FX::CFXLight_2Stage::ColorCycle");

         if (temp.red < pColor2->red)
         {
            temp.red += step;

            if (temp.red >= pColor2->red)
            {
               temp.red = pColor2->red;
               LerpFinishRed = true;
            }
         }
         else
         {
            temp.red -= step;

            if (temp.red <= pColor2->red)
            {
               temp.red = pColor2->red;
               LerpFinishRed = true;
            }
         }

         if (temp.green < pColor2->green)
         {
            temp.green += step;

            if (temp.green >= pColor2->green)
            {
               temp.green = pColor2->green;
               LerpFinishGreen = true;
            }
         }
         else
         {
            temp.green -= step;

            if (temp.green <= pColor2->green)
            {
               temp.green = pColor2->green;
               LerpFinishGreen = true;
            }
         }

         if (temp.blue < pColor2->blue)
         {
            temp.blue += step;

            if (temp.blue >= pColor2->blue)
            {
               temp.blue = pColor2->blue;
               LerpFinishBlue = true;
            }
         }
         else
         {
            temp.blue -= step;

            if (temp.blue <= pColor2->blue)
            {
               temp.blue = pColor2->blue;
               LerpFinishBlue = true;
            }
         }

         RWS_RETURNVOID();
      }
   } // namespace FX
} // namespace RWS
