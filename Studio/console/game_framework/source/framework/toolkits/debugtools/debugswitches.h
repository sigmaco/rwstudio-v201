/*****************************************************************************
*
* File :     DebugSwitches.h
*
* Abstract : Simple true or false flags used to enable and disable sections of code during debugging. New flags
*            can be added by extending enum_fags. Default values can be set by modifying DebugSwitches::Open.
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

#ifndef __DEBUGSWITCHES_H__
#define __DEBUGSWITCHES_H__

namespace RWS
{
   /**
   *  \ingroup DebugSwitches
   *
   *  Debug switch API, simple true or false flags used to enable and disable sections of code during debugging. New flags
   *  can be added by extending enum_fags. Default values can be set by modifying DebugSwitches::Open.
   */
   namespace DebugSwitches
   {
      /**
      *  \ingroup DebugSwitches
      */
      enum enum_flags
      {
         m_enabled,                        /**< Enable/disable debug Info updates, 
                                                while in pause mode debug info updates 
                                                are ignored  */
         m_req_enabled,                    /**< Request new state of m_enabled (Updated 
                                                at end of frame) */
         m_enable_printf,                  /**< Enable/Disable Printf function   */
         m_enable_metrics_printf,          /**< Enable/Disable Printf_Metrics function */
         m_enable_collision_debug_info,    /**< Enable/Disable Collision debug info */
         m_enable_message_flow_debug_info, /**< Enable/Disable Message flow debug info */
         m_enable_displaysectors,          /**< Enable/Disable drawing of sectors */
         m_enable_DirectorsCamera,         /**< Enable/Disable directors camera */

         m_enable_Metric_RwResourcesGetArenaUsage, /**< Enable/Disable tracing of RwResourcesGetArenaUsage */
         m_enable_Metric_FramesPerSecond, /**< Enable/Disable tracing of FramesPerSecond */
         m_enable_Metric_Triangles, /**< Enable/Disable tracing of Triangles */
         m_enable_Metric_Processed_Triangles, /**< Enable/Disable tracing of Processed_Triangles */
         m_enable_Metric_Vertices, /**< Enable/Disable tracing of Vertices */
         m_enable_Metric_Size_Texture_Uploads, /**< Enable/Disable tracing of Size_Texture_Uploads */
         m_enable_Metric_Num_Texture_Uploads, /**< Enable/Disable tracing of Num_Texture_Uploads */
         m_enable_Metric_CPUTime, /**< Enable/Disable tracing of CPU Time */
         m_enable_Metric_PS2, /**< Enable/Disable tracing of PS2 Specific Metrics */

         m_last
      };

#ifdef RWS_DEBUGSWITCHES   // Include debug tools code
      
      extern void SetSwitch(enum_flags var, bool flag);
      extern bool GetSwitch(enum_flags var);

      extern void Open(void);

#else          // Exclude debug tools code 

      __inline void SetSwitch(enum_flags var, bool flag) {}
      __inline bool GetSwitch(enum_flags var) {return false;}

      __inline void Open(void) {}
#endif

      /**
      *  \ingroup DebugSwitches
      *
      *  Close DebugSwitches module.
      *
      */
      __inline void Close(void) {}
   }
}

#endif
