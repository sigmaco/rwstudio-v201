/*****************************************************************************
*
* File :	  CFXWaveLight.h
*
* Abstract : An entity wrapper for the RpLight type
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

#ifndef __CFXLIGHTSPLINE_H__
#define __CFXLIGHTSPLINE_H__

#if defined (_MSC_VER) // C4786 : identifier was truncated to '255' characters in the browser information 
                       //(Doesn't work for the map)
#pragma warning ( push )
#pragma warning ( disable : 4786 )
#endif // defined (_MSC_VER)

//////////////////////////////////////////////////////////////////
//
// STL Includes
//
#include <map>
#include <string>

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "../cfxbaselight.h"
#include "framework/toolkits/world/smartptrs/catomicptr.h"
#include "framework/mainloop/logic.h"

#include "framework/core/attributehandler/allocation policies/linearallocationpolicy.h"

namespace RWS
{

   namespace FX
   {
      ///////////////////////////////////////////////////////////////////////////
      //
      // Singleton event class. Creates a single instance for each wave type
      //
      class CFXWaveGenerator : public CEventHandler, public LinearAllocationPolicy
      {
      public:
         static CFXWaveGenerator* QueryInstance (const RwChar* WaveType, RwUInt32 TicksPerEvent);
         
         void Release (void);
         void AddRef (void);
         
         virtual void HandleEvents(CMsg &pMsg);
         
      protected:
         CFXWaveGenerator(const RwChar* WaveType, RwUInt32 TicksPerEvent);
         ~CFXWaveGenerator();
         void GetSparkValue( void );
         
         std::string m_WaveType;
         
         CEventId m_WaveEventId;
         RwUInt32 m_TickCount;
         RwUInt32 m_TicksPerEvent;
         RwChar   m_WaveValue;
         RwUInt32 m_RefCount;
         
      private:
         typedef std::map<std::string, CFXWaveGenerator*> string_wave_map;
         static string_wave_map _singleton_map;
      };
      
      /**
      *
      *  \ingroup Mod_FX
      *
      */
      class CFXWaveLight : public CFXBaseLight, public CEventHandler
      {
      public:
         RWS_MAKENEWCLASS(CFXWaveLight);
         RWS_DECLARE_CLASSID(CFXWaveLight);
         RWS_CATEGORY("FX");
         RWS_DESCRIPTION("Wave Light", "");
         
         virtual void HandleAttributes(const CAttributePacket& attr);
         virtual void HandleEvents(CMsg &pMsg);
         
         RWS_BEGIN_COMMANDS
            RWS_ATTRIBUTE(CMD_Set_t_color,       
            "Color",       
            "Set light color, see RpLightSetColor",   
            COLOR,  
            RwRGBA,   
            DEFAULT(0xffffffff))
            
            RWS_MESSAGE(   CMD_targetname,       
            "Target name",  
            "If targeted, it will toggle between on or off",
            RECEIVE,
            0,
            0)
            
            RWS_ATTRIBUTE( CMD_spawnflags,       
            "Flags",        
            "0 - Light Starts On :: 1 - Light Starts Off",
            LIST,
            RwUInt32,
            LIST("Starts Enabled|Starts Disabled"))
            
            RWS_ATTRIBUTE( CMD_style,            
            "Style",        
            "Lighting Style", 
            RADIO_BUTTON, 
            RwUInt32, 
            LIST("Normal|Flicker|Slow Strong Pulse|Candle|Fast Strobe|Gentle Pulse|Flicker 2| "
            "Candle 2|Candle 3|Slow Strobe|Fluorescent Flicker|Slow Pulse"))
            RWS_END_COMMANDS;
         
         CFXWaveLight(const CAttributePacket& attr);
         ~CFXWaveLight(void);
      protected:
         
         CEventId m_rx_toggle;
         CEventId m_SparkEventId;
         CFXWaveGenerator* CFXWaveGen;
         RwUInt32 m_Style;
         RwRGBAReal t_color;
         RwReal m_ColorValue;
      private:
         void CFXlight_init(void);  // Initialization of the light
         void SetCurrentLightColor(void);
      };
      
   } // namespace FX
   
} // namespace RWS

#endif

