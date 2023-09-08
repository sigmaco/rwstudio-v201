/*****************************************************************************
*
* File :     CFXWaveLight.cpp
*
* Abstract : Creates light that uses a text string to animate color intensities
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

//#define RWS_TRACEENABLE
//#define RWS_CALLSTACKENABLE

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "cfx_wavelight.h"
#include "framework/core/macros/debugmacros.h"
#include "framework/toolkits/math/maths.h"
#include "framework/toolkits/world/clevel.h"

namespace RWS
{
   
   namespace FX
   {
    
      // FPS_WaveGenerator
      CFXWaveGenerator::string_wave_map CFXWaveGenerator::_singleton_map;

      RWS_IMPLEMENT_CLASSID(CFXWaveLight);
      RWS_REGISTERCLASS(CFXWaveLight);

      // lights may need to be scaled to look similar in RenderWare
      const RwReal LIGHT_2_RWS_LIGHT_FACTOR = 1.0f;

      const RwChar  SparkToggleMessage[] = RWSTRING ("SparkToggleMessage");
    
      const RwChar *SparkStyle[] = 
      {
         { RWSTRING ("m") },                                                     // 0 normal
         { RWSTRING ("mmnmmommommnonmmonqnmmo") },                               // 1 FLICKER (first variety)
         { RWSTRING ("abcdefghijklmnopqrstuvwxyzyxwvutsrqponmlkjihgfedcba") },   // 2 SLOW STRONG PULSE
         { RWSTRING ("mmmmmaaaaammmmmaaaaaabcdefgabcdefg") },                    // 3 CANDLE (first variety)
         { RWSTRING ("mamamamamama") },                                          // 4 FAST STROBE
         { RWSTRING ("jklmnopqrstuvwxyzyxwvutsrqponmlkj") },                     // 5 GENTLE PULSE 1
         { RWSTRING ("nmonqnmomnmomomno") },                                     // 6 FLICKER (second variety)
         { RWSTRING ("mmmaaaabcdefgmmmmaaaammmaamm") },                          // 7 CANDLE (second variety)
         { RWSTRING ("mmmaaammmaaammmabcdefaaaammmmabcdefmmmaaaa") },            // 8 CANDLE (third variety)
         { RWSTRING ("aaaaaaaazzzzzzzz") },                                      // 9 SLOW STROBE (fourth variety)
         { RWSTRING ("mmamammmmammamamaaamammma") },                             // 10 FLUORESCENT FLICKER
         { RWSTRING ("abcdefghijklmnopqrrqponmlkjihgfedcba") }                   // 11 SLOW PULSE NOT FADE TO BLACK
      };

      /**
      *
      *  Returns an instance to the per-wave-type singleton class
      *
      */
      CFXWaveGenerator* CFXWaveGenerator::QueryInstance (const RwChar* WaveType, RwUInt32 TicksPerEvent)
      {
         RWS_FUNCTION("RWS::FX::CFXWaveGenerator::QueryInstance");

         std::string msg = WaveType;
         CFXWaveGenerator* WaveGenerator = 0;
        
         // Search for a singleton class with the same message name
         string_wave_map::iterator i = _singleton_map.find(msg);
        
         if (i != _singleton_map.end())
         {
             WaveGenerator = reinterpret_cast<CFXWaveGenerator*>((*i).second);
             WaveGenerator->AddRef ();
         }
         else
         {
            WaveGenerator = ::RWS_NEW CFXWaveGenerator(WaveType, TicksPerEvent);
             _singleton_map.insert(string_wave_map::value_type(msg, WaveGenerator));
         }
        
         RWS_RETURN(WaveGenerator);
      }
    
      /**
      *
      * Increments the reference count of a wave generator instance
      *
      */
      void CFXWaveGenerator::AddRef (void)
      {
         RWS_FUNCTION("RWS::FX::CFXWaveGenerator::AddRef");

         ++m_RefCount;

         RWS_RETURNVOID();
      }
    
      /**
      *
      * Releases an instance of a wave generator
      *
      */
      void CFXWaveGenerator::Release ()
      {
         RWS_FUNCTION("RWS::FX::CFXWaveGenerator::Release");

         --m_RefCount;
        
         // If no more references exist then delete the instance.
         if (m_RefCount == 0)
         {
             delete this;
         }

         RWS_RETURNVOID();
      }

      /**
      *
      * Constructor for CFXWaveGenerator
      *
      */
      CFXWaveGenerator::CFXWaveGenerator(const RwChar* WaveType, RwUInt32 TicksPerEvent) : 
            InitCEventHandler(0),
            m_WaveType(WaveType),
            m_TickCount(0),
            m_TicksPerEvent(TicksPerEvent),
            m_RefCount(1)
      {
         RWS_FUNCTION("RWS::FX::CFXWaveGenerator::CFXWaveGenerator");

         RegisterMsg (m_WaveEventId, WaveType, "RwChar*");
         LinkMsg (iMsgRunningTick, 0);

         RWS_RETURNVOID();
      }

      /**
      *
      * Destructor for CFXWaveGenerator
      *
      */
      CFXWaveGenerator::~CFXWaveGenerator()
      {
         RWS_FUNCTION("RWS::FX::CFXWaveGenerator::~CFXWaveGenerator");

         // Remove reference from the map
         //
         string_wave_map::iterator i = _singleton_map.find (m_WaveType);

         if (i != _singleton_map.end() )
         {
            _singleton_map.erase (i);
         }

         UnLinkMsg (iMsgRunningTick);
         UnRegisterMsg (m_WaveEventId);

         RWS_RETURNVOID();
      }

      /**
      *
      * Handle events for CFXWaveGenerator
      *
      */
      void CFXWaveGenerator::HandleEvents (CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::FX::CFXWaveGenerator::HandleEvents");

         if (pMsg.Id == iMsgRunningTick)
         {
            GetSparkValue();
         }

         RWS_RETURNVOID();
      }

      /**
      *
      * Get value of spark from string
      *
      */
      void CFXWaveGenerator::GetSparkValue( void )
      {
         RWS_FUNCTION("RWS::FX::CFXWaveGenerator::GetSparkValue");

         if (++m_TickCount > (m_WaveType.length() - 1) * m_TicksPerEvent)
         {
            m_TickCount = 0;
         }

         // Determine the current state of the light
         RwChar WaveValue = m_WaveType[m_TickCount / m_TicksPerEvent];

         if (m_WaveValue != WaveValue)
         {
            m_WaveValue = WaveValue;

            // Send a spark toggle message
            CMsg iMsg(m_WaveEventId, &m_WaveValue);
            SendMsg (iMsg);
         }

         RWS_RETURNVOID();
      }
    
      /**
      *
      * Handle events for CFXWaveLight
      *
      */    
      void CFXWaveLight::HandleEvents(CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::FX::CFXWaveLight::HandleEvents");

         if (pMsg.Id == m_rx_toggle && m_pLight)
         {
            // if in the world remove the light, otherwise add it to the world
            RpWorld *world = RpLightGetWorld(m_pLight);

            if (world)
            {
               RemoveLightFromWorld();
            }
            else
            {
               AddLightToWorld();
            }
         }
         else  
         {
            // Check for light effect
            if (pMsg.Id == m_SparkEventId)
            {
               RwChar SparkValue = *reinterpret_cast<RwChar*>(pMsg.pData);
               m_ColorValue = ( ( static_cast<RwReal>(SparkValue) - 'a') + 1.0f ) / static_cast<RwReal> (26.0f);

               // Update the actual light color from the new value for m_ColorValue
               SetCurrentLightColor();
            }
         }

         RWS_RETURNVOID();
      }

      /**
      *
      * Handle attributes for CFXWaveLight
      *
      */    
      void CFXWaveLight::HandleAttributes(const CAttributePacket& attr)
      {
         RWS_FUNCTION("RWS::FX::CFXWaveLight::HandleAttributes");

         // Initialize base class first
         CFXBaseLight::HandleAttributes(attr);

         CAttributeCommandIterator attrIt(attr, RWS_CLASSID_OF(CFXWaveLight));

         while (!attrIt.IsFinished())
         {
            switch (attrIt->GetCommandId())
            {
               case CMD_targetname:
               {
                  const RwChar *name;

                  attrIt->GetCommandData(&name);

                  UnLinkMsg(m_rx_toggle);
                  UnRegisterMsg(m_rx_toggle);
                  RegisterMsg(m_rx_toggle, name,0);
                  LinkMsg(m_rx_toggle, 0);
               }
               break; 

               case CMD_Set_t_color:
               {
                  RwRGBA Color = attrIt->GetAs_RwRGBA();
                  RwRGBARealFromRwRGBA  (&t_color, &Color);

                  // Set the current color from the new base value
                  SetCurrentLightColor();
               }
               break;

               case CMD_spawnflags:
                  RwUInt32 spawnflags;

                  attrIt->GetCommandData(spawnflags);

                  if (m_pLight)
                  {
                     if (spawnflags == 0) // Light Starts On
                     {
                        AddLightToWorld();
                     }
                     else  // Starts off
                     {
                        RemoveLightFromWorld();
                     }
                  }
                  else
                  {
                     RWS_ASSERT (false, "Unable to set light on/off as m_pLight == 0");
                  }

               break;

               case CMD_style:
                  attrIt->GetCommandData(m_Style);

                  if (m_Style < (sizeof(SparkStyle) / sizeof(RwChar*)) )
                  {
                     CFXWaveGen->Release ();
                     CFXWaveGen = CFXWaveGenerator::QueryInstance (SparkStyle[m_Style], 4);

                     UnLinkMsg (m_SparkEventId);
                     UnRegisterMsg (m_SparkEventId);
                     RegisterMsg (m_SparkEventId, SparkStyle[m_Style], "RwChar*");
                     LinkMsg (m_SparkEventId, "RwChar*");
                  }
                  else
                  {
                     RWS_TRACE("m_Style "<<m_Style<<" Out Of Range");
                  }
                break;
            }

            ++attrIt;
         }
         
         RWS_RETURNVOID();
      }
   
      /**
      *
      * \ref CFXWaveLight::SetCurrentColor
      *
      * Sets the current brightness of the light based on the current value from the wave generator
      *
      */
      void CFXWaveLight::SetCurrentLightColor( void )
      {
         RWS_FUNCTION("RWS::FX::CFXWaveLight::SetCurrentLightColor");
         
         RwRGBAReal Color;
         
         Color.red   = t_color.red   * m_ColorValue;
         Color.green = t_color.green * m_ColorValue;
         Color.blue  = t_color.blue  * m_ColorValue;
         Color.alpha = t_color.alpha;
         
         RpLightSetColor (m_pLight, &Color);
         
         RWS_RETURNVOID();
      }

      /**
      *
      * Initialize CFXWaveLight
      *
      */  
      void CFXWaveLight::CFXlight_init(void)
      {
         RWS_FUNCTION("RWS::FX::CFXWaveLight::CFXlight_init");

         // Get an instance to the spark toggle class.
         CFXWaveGen = CFXWaveGenerator::QueryInstance (SparkStyle[m_Style], 4);

         // The SparkToggleMessage will be used to toggle the enabled
         // state of the light.
         RegisterMsg (m_SparkEventId, SparkStyle[m_Style], "RwChar*");
         LinkMsg (m_SparkEventId, "RwChar*");

         // If CMD_light is not sent, default light value is 300.0f
         if (m_pLight)
         {
            RpLightSetRadius(m_pLight, 300.0f * LIGHT_2_RWS_LIGHT_FACTOR);
         }

         RWS_RETURNVOID();
      }
      
      CFXWaveLight::CFXWaveLight(const CAttributePacket& rAttr) : 
         CFXBaseLight(rAttr), 
         InitCEventHandler(&m_pLight), 
         m_Style(0), 
         m_ColorValue(1.0f)
      {
         RWS_FUNCTION("RWS::FX::CFXWaveLight::CFXWaveLight");
         CFXlight_init();
         RWS_RETURNVOID();
      }
      
      CFXWaveLight::~CFXWaveLight(void)
      {
         RWS_FUNCTION("RWS::FX::CFXWaveLight::~CFXWaveLight");
         CFXWaveGen->Release ();
         
         UnLinkMsg(m_SparkEventId);
         UnRegisterMsg(m_SparkEventId);
         
         UnLinkMsg(m_rx_toggle);
         UnRegisterMsg(m_rx_toggle);
         
         RWS_RETURNVOID();
      }
      
   } // namespace FX
   
} // namespace RWS
