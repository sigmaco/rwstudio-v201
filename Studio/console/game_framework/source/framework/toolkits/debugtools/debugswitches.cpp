/*****************************************************************************
*
* File :     DebugSwitches.cpp
*
* Abstract : Implementation of DebugSwitches API.
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

#ifdef RWS_DEBUGSWITCHES

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "debugswitches.h"
#include "../macros/debugmacros.h"

namespace RWS
{
   namespace DebugSwitches
   {
      bool _flags[m_last];

      /**
      *  \ingroup DebugSwitches
      *
      *  Set the state of the specified debug switch.
      *
      *  \param var Id of the flag to modify.
      *  \param flag New value of the flag.
      */
      void SetSwitch(enum_flags var, bool flag)
      {
         RWS_FUNCTION("RWS::DebugSwitches::SetSwitch");

         _flags[var] = flag;

         RWS_RETURNVOID();
      }

      /**
      *  \ingroup DebugSwitches
      *
      *  Get debug switch status.
      *
      *  \param var Id of the flag.
      *
      *  Return the state of the specified debug switch.
      *
      */

      bool GetSwitch(enum_flags var)
      {
         RWS_FUNCTION("RWS::DebugSwitches::GetSwitch");
         
         RWS_RETURN(_flags[var]);
      }

      /**
      *  \ingroup DebugSwitches
      *
      *  Open DebugSwitches module, performs initialization of debug switches.
      *
      */
      void Open(void)
      {
         RWS_FUNCTION("RWS::DebugSwitches::Open");

         SetSwitch(m_enabled, true);
         SetSwitch(m_req_enabled, true);
         SetSwitch(m_enable_printf, false);
         SetSwitch(m_enable_metrics_printf, false);
         SetSwitch(m_enable_collision_debug_info, false);
         SetSwitch(m_enable_message_flow_debug_info, false);
         SetSwitch(m_enable_displaysectors, false);

         RWS_RETURNVOID();
      }
   }
}
#endif