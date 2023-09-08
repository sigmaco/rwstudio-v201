/*****************************************************************************
*
* File :     CDebugTools.cpp
*
* Abstract:   Implementation of CDebugTools class
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
* Copyright (c) 2001 Criterion Software Ltd.
* All Rights Reserved.
*
* RenderWare is a trademark of Canon Inc.
*
*****************************************************************************/
#include "precomp.h"
#ifdef RWS_DEBUGTOOLS

// Left to its own devices, the MW linker will dead-strip the registrar ctors, so...
#ifdef __MWERKS__
#pragma force_active on
#pragma exceptions off // Turn off exceptions due to bug in MW Dwarf Generator for PS2, present in CW 3.04
#endif

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "cdebugtools.h"

#include "framework/toolkits/debugtools/debugtools.h"
#include "framework/core/macros/debugmacros.h"

namespace RWS
{
   namespace Generic
   {
      RWS_IMPLEMENT_CLASSID(CDebugTools);
      RWS_REGISTERCLASS(CDebugTools);

      using namespace DebugTools;

      /**
      *
      *  This function maps the attribute data specified by 
      *  LIST("rwBLENDZERO|rwBLENDONE|rwBLENDSRCCOLOR|rwBLENDINVSRCCOLOR|...")
      *  to the RenderWare enums for the particular blend mode.
      */  
      RwBlendFunction CDebugTools::Map_AttributeListToBlendMode(RwUInt32 var) const
      {
         RWS_FUNCTION("RWS::Generic::CDebugTools::Map_AttributeListToBlendMode");
      
         RwBlendFunction temp;
      
         switch (var)
         {
            default:
            case 0: temp = rwBLENDZERO; break;
            case 1: temp = rwBLENDONE; break;
            case 2: temp = rwBLENDSRCCOLOR; break;
            case 3: temp = rwBLENDINVSRCCOLOR; break;
            case 4: temp = rwBLENDSRCALPHA; break;
            case 5: temp = rwBLENDINVSRCALPHA; break;
            case 6: temp = rwBLENDDESTALPHA; break;
            case 7: temp = rwBLENDINVDESTALPHA; break;
            case 8: temp = rwBLENDDESTCOLOR; break;
            case 9: temp = rwBLENDINVDESTCOLOR; break;
            case 10: temp = rwBLENDSRCALPHASAT;break;
         }
      
         RWS_RETURN(temp);
      }

      /**
      *
      *  Handle attribute updates associated with the CDebugTools class
      *
      */
      void CDebugTools::HandleAttributes(const CAttributePacket& attr)
      {
         RWS_FUNCTION("RWS::Generic::CDebugTools::HandleAttributes");

         CAttributeHandler::HandleAttributes(attr);

         CAttributeCommandIterator attrIt(attr, RWS_CLASSID_OF(CDebugTools));

         DisplayDebugTools *pInstance = DisplayDebugTools::GetInstance();
         
         while (!attrIt.IsFinished())
         {
            RwUInt32 flag;

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning( disable: 4800 )
#endif   // _MSC_VER
         
            switch (attrIt->GetCommandId())
            {
               //////////////////////////////////////////////////////////////////
               case CMD_rwRENDERSTATECULLMODE:
               //////////////////////////////////////////////////////////////////
               {
                  RwCullMode _rwRENDERSTATECULLMODE;

                  switch (attrIt->GetAs_RwUInt32())
                  {
                     default:
                     case 0: _rwRENDERSTATECULLMODE = rwCULLMODECULLNONE;
                        break;
                     case 1: _rwRENDERSTATECULLMODE = rwCULLMODECULLBACK;
                        break;
                     case 2: _rwRENDERSTATECULLMODE = rwCULLMODECULLFRONT;
                        break;
                  }

                  pInstance->SetRENDERSTATECULLMODE(_rwRENDERSTATECULLMODE);
               }
               break;
               //////////////////////////////////////////////////////////////////
               case CMD_rwRENDERSTATESRCBLEND:
                  //////////////////////////////////////////////////////////////////
                  pInstance->SetRENDERSTATESRCBLEND(Map_AttributeListToBlendMode(attrIt->GetAs_RwInt32()));
                  break;
                  //////////////////////////////////////////////////////////////////
               case CMD_rwRENDERSTATEDESTBLEND:
                  //////////////////////////////////////////////////////////////////
                  pInstance->SetRENDERSTATEDESTBLEND(Map_AttributeListToBlendMode(attrIt->GetAs_RwInt32()));
                  break;
                  //////////////////////////////////////////////////////////////////
               case CMD_rwRENDERSTATEVERTEXALPHAENABLE:
                  //////////////////////////////////////////////////////////////////
                  pInstance->SetRENDERSTATEVERTEXALPHAENABLE(attrIt->GetAs_RwInt32());
                  break;
                  //////////////////////////////////////////////////////////////////
               case CMD_rwRENDERSTATEZWRITEENABLE:
                  //////////////////////////////////////////////////////////////////
                  pInstance->SetRENDERSTATEZWRITEENABLE(attrIt->GetAs_RwInt32());
                  break;
                  //////////////////////////////////////////////////////////////////
               case CMD_rwRENDERSTATEZTESTENABLE:
                  //////////////////////////////////////////////////////////////////
                  pInstance->SetRENDERSTATEZTESTENABLE(attrIt->GetAs_RwInt32());
                  break;

                  //////////////////////////////////////////////////////////////////
               case CMD_Start_Render:
                  //////////////////////////////////////////////////////////////////
                  pInstance->SetStartRenderEvent(attrIt->GetAs_RwChar_ptr());
                  break;

               case CMD_priority:
                  //////////////////////////////////////////////////////////////////
                  pInstance->SetRenderPriority(attrIt->GetAs_RwUInt32());
                  break;

                  //////////////////////////////////////////////////////////////////
               case CMD_DebugPrintf:
                  //////////////////////////////////////////////////////////////////
                  attrIt->GetCommandData(flag);
                  DebugSwitches::SetSwitch(DebugSwitches::m_enable_printf, flag);
                  break;

                  //////////////////////////////////////////////////////////////////
               case CMD_MetricsPrintf:
                  //////////////////////////////////////////////////////////////////
                  attrIt->GetCommandData(flag);
                  DebugSwitches::SetSwitch(DebugSwitches::m_enable_metrics_printf, flag);
                  break;
            
                  //////////////////////////////////////////////////////////////////
               case CMD_MessageFlows:
                  //////////////////////////////////////////////////////////////////
                  attrIt->GetCommandData(flag);
                  DebugSwitches::SetSwitch(DebugSwitches::m_enable_message_flow_debug_info, flag);
                  break;
            
                  //////////////////////////////////////////////////////////////////
               case CMD_CollisionInfo:
                  //////////////////////////////////////////////////////////////////
                  attrIt->GetCommandData(flag);
                  DebugSwitches::SetSwitch(DebugSwitches::m_enable_collision_debug_info, flag);
                  break;
                  //////////////////////////////////////////////////////////////////
               case CMD_DisplaySectors:
                  //////////////////////////////////////////////////////////////////
                  attrIt->GetCommandData(flag);
                  DebugSwitches::SetSwitch(DebugSwitches::m_enable_displaysectors, flag);
                  break;

               case CMD_Metric_RwResourcesGetArenaUsage:
                  //////////////////////////////////////////////////////////////////
                  attrIt->GetCommandData(flag);
                  DebugSwitches::SetSwitch(DebugSwitches::m_enable_Metric_RwResourcesGetArenaUsage, flag);
                  break;

               case CMD_Metric_FramesPerSecond:
                  //////////////////////////////////////////////////////////////////
                  attrIt->GetCommandData(flag);
                  DebugSwitches::SetSwitch(DebugSwitches::m_enable_Metric_FramesPerSecond, flag);
                  break;

               case CMD_Metric_Triangles:
                  //////////////////////////////////////////////////////////////////
                  attrIt->GetCommandData(flag);
                  DebugSwitches::SetSwitch(DebugSwitches::m_enable_Metric_Triangles, flag);
                  break;

               case CMD_Metric_Processed_Triangles:
                  //////////////////////////////////////////////////////////////////
                  attrIt->GetCommandData(flag);
                  DebugSwitches::SetSwitch(DebugSwitches::m_enable_Metric_Processed_Triangles, flag);
                  break;

               case CMD_Metric_Vertices:
                  //////////////////////////////////////////////////////////////////
                  attrIt->GetCommandData(flag);
                  DebugSwitches::SetSwitch(DebugSwitches::m_enable_Metric_Vertices, flag);
                  break;

               case CMD_Metric_Size_Texture_Uploads:
                  //////////////////////////////////////////////////////////////////
                  attrIt->GetCommandData(flag);
                  DebugSwitches::SetSwitch(DebugSwitches::m_enable_Metric_Size_Texture_Uploads, flag);
                  break;

               case CMD_Metric_Num_Texture_Uploads:
                  //////////////////////////////////////////////////////////////////
                  attrIt->GetCommandData(flag);
                  DebugSwitches::SetSwitch(DebugSwitches::m_enable_Metric_Num_Texture_Uploads, flag);
                  break;

               case CMD_Metric_CPUTime:
                  //////////////////////////////////////////////////////////////////
                  attrIt->GetCommandData(flag);
                  DebugSwitches::SetSwitch(DebugSwitches::m_enable_Metric_CPUTime, flag);
                  break;

               case CMD_Metric_PS2:
                  //////////////////////////////////////////////////////////////////
                  attrIt->GetCommandData(flag);
                  DebugSwitches::SetSwitch(DebugSwitches::m_enable_Metric_PS2, flag);
                  break;

#ifdef _MSC_VER
#pragma warning(pop)
#endif // _MSC_VER
            }
            ++attrIt;
         }

         RWS_RETURNVOID();
      }
   }
}
#endif

