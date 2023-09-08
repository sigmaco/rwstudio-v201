/*****************************************************************************
 *
 * File :     CDebugTools.h
 *
 * Abstract:   Declaration of CDebugTools class
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

#ifdef RWS_DEBUGTOOLS

#ifndef __CDEBUGTOOLS_H__
#define __CDEBUGTOOLS_H__

//////////////////////////////////////////////////////////////////
//
// RenderWare Includes
//
#include <rwcore.h>
#include <rpworld.h>
#include <rtcharse.h>

#ifdef _MSC_VER
#pragma comment (lib, "rwcore.lib")
#pragma comment (lib, "rpworld.lib")
#pragma comment (lib, "rtcharse.lib")
#endif

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "framework/core/eventhandler/ceventhandler.h"
#include "framework/core/attributehandler/cattributehandler.h"
#include "framework/core/streamhandler/strfunc.h"
#include "framework/core/attributehandler/allocation policies/LinearAllocationPolicy.h"

namespace RWS
{
   namespace Generic
   {
      /**
      *
      *  \ingroup Mod_Generic
      *
      *  This class exposes an interface to the DebugTools toolkit.
      *
      */
      class CDebugTools : public CAttributeHandler, public LinearAllocationPolicy
      {
         public:
            RWS_MAKENEWCLASS(CDebugTools);
            RWS_DECLARE_CLASSID(CDebugTools);
            RWS_CATEGORY("Generic");
            RWS_DESCRIPTION("Debug Tools.", "Provides an interface to the DebugTools toolkit.");

            CDebugTools(const CAttributePacket&) {}

            virtual void HandleAttributes(const CAttributePacket& attr);

            RWS_BEGIN_COMMANDS

               RWS_ATTRIBUTE(CMD_DebugPrintf,         "Printf",             "Display output directed to DebugTools::Printf",   BOOLEAN,RwUInt32,DEFAULT(0))

               RWS_ATTRIBUTE(CMD_MetricsPrintf,       "Printf_Metrics",     "Display RenderWare metrics info",   BOOLEAN,RwUInt32,DEFAULT(0))

               RWS_ATTRIBUTE(CMD_MessageFlows,        "Visualise Events",   "Visualise game events",   BOOLEAN,RwUInt32,DEFAULT(0))

               RWS_ATTRIBUTE(CMD_CollisionInfo,       "Visualise Collision","Visualise collision info",   BOOLEAN,RwUInt32,DEFAULT(0))

               RWS_ATTRIBUTE(CMD_DisplaySectors,      "Visualise Sectors",  "Visualise sectors as bounding boxes, lights linked to sectors are shown as lines connecting the light to the center of the sector",   BOOLEAN,RwUInt32,DEFAULT(0))

            RWS_SEPARATOR("Metrics Data", 0)

               RWS_ATTRIBUTE(CMD_Metric_RwResourcesGetArenaUsage,      "Resources Arena Usage",  "Workspace trace resource arena usage",   BOOLEAN,RwUInt32,DEFAULT(0))
               RWS_ATTRIBUTE(CMD_Metric_FramesPerSecond,      "Frames Per Second",  "Workspace trace frames per second",   BOOLEAN,RwUInt32,DEFAULT(0))
               RWS_ATTRIBUTE(CMD_Metric_Triangles,      "Triangles",  "Workspace trace num triangles drawn",   BOOLEAN,RwUInt32,DEFAULT(0))
               
               RWS_ATTRIBUTE(CMD_Metric_Processed_Triangles,      "Processed Triangles", "Workspace trace num triangles processed",   BOOLEAN,RwUInt32,DEFAULT(0))

               RWS_ATTRIBUTE(CMD_Metric_Vertices,      "Vertices",  "Workspace trace number of vertices",   BOOLEAN,RwUInt32,DEFAULT(0))
               RWS_ATTRIBUTE(CMD_Metric_Size_Texture_Uploads,      "Size Of Texture Uploads",  "Workspace trace size of texture uploads",   BOOLEAN,RwUInt32,DEFAULT(0))
               RWS_ATTRIBUTE(CMD_Metric_Num_Texture_Uploads,      "Number Of Texture Uploads",  "Workspace trace number of texture uploads",   BOOLEAN,RwUInt32,DEFAULT(0))

               RWS_ATTRIBUTE(CMD_Metric_CPUTime,      "CPU Time",  "Workspace trace CPU Time, start of pre tick to end of post tick",   BOOLEAN,RwUInt32,DEFAULT(0))

               RWS_ATTRIBUTE(CMD_Metric_PS2,      "PS2 Specific Metrics",  "Workspace trace PS2 Specific Metrics data",   BOOLEAN,RwUInt32,DEFAULT(0))

            RWS_SEPARATOR("Rendering", 0)

               RWS_MESSAGE  (CMD_Start_Render, "Start Render Event", "Begin rendering specified camera.", RECEIVE , RwCamera*, "iMsgDoRender")

               RWS_ATTRIBUTE(CMD_priority,  "Render Priority",   "Specify the render priority",   SLIDER,  RwUInt32,   RANGE(0,0,65535))

            RWS_SEPARATOR("Render State", 0)

               RWS_ATTRIBUTE(CMD_rwRENDERSTATECULLMODE,             "rwRENDERSTATECULLMODE"            ,"Set back face culling mode",LIST,RwUInt32,LIST("@rwCULLMODECULLNONE|rwCULLMODECULLBACK|rwCULLMODECULLFRONT"))
               RWS_ATTRIBUTE(CMD_rwRENDERSTATESRCBLEND,             "rwRENDERSTATESRCBLEND"            ,"Set source blending mode",LIST,RwUInt32,LIST("rwBLENDZERO|rwBLENDONE|rwBLENDSRCCOLOR|rwBLENDINVSRCCOLOR|@rwBLENDSRCALPHA|rwBLENDINVSRCALPHA|rwBLENDDESTALPHA|rwBLENDINVDESTALPHA|rwBLENDDESTCOLOR|rwBLENDINVDESTCOLOR|rwBLENDSRCALPHASAT"))
               RWS_ATTRIBUTE(CMD_rwRENDERSTATEDESTBLEND,            "rwRENDERSTATEDESTBLEND"           ,"Set destination blending mode",LIST,RwUInt32,LIST("rwBLENDZERO|rwBLENDONE|rwBLENDSRCCOLOR|rwBLENDINVSRCCOLOR|rwBLENDSRCALPHA|@rwBLENDINVSRCALPHA|rwBLENDDESTALPHA|rwBLENDINVDESTALPHA|rwBLENDDESTCOLOR|rwBLENDINVDESTCOLOR|rwBLENDSRCALPHASAT"))
               RWS_ATTRIBUTE(CMD_rwRENDERSTATEVERTEXALPHAENABLE,    "rwRENDERSTATEVERTEXALPHAENABLE"   ,"Set vertex alpha enable mode",BOOLEAN,RwUInt32,DEFAULT(1))
               RWS_ATTRIBUTE(CMD_rwRENDERSTATEZWRITEENABLE,         "rwRENDERSTATEZWRITEENABLE"        ,"Set Z write enable mode",BOOLEAN,RwUInt32,DEFAULT(0))
               RWS_ATTRIBUTE(CMD_rwRENDERSTATEZTESTENABLE,          "rwRENDERSTATEZTESTENABLE"         ,"Set Z test enable mode",BOOLEAN,RwUInt32,DEFAULT(1))
  
            RWS_END_COMMANDS;

         private:

            RwBlendFunction Map_AttributeListToBlendMode(RwUInt32 var) const;
      };
   }//namespace Generic
}//namespace RWS

#endif//__CDEBUGTOOLS_H__
#endif//RWS_DEBUGTOOLS