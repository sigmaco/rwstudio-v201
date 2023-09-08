/*****************************************************************************
 *
 * File :     DebugMacros.cpp
 *
 * Abstract : Support for RWS debug macros.
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
#include "stdafx.h"
#include "debugmacros.h"

#ifndef NDEBUG

namespace RWS 
{
   namespace 
   {
      int CallDepth = 0;
   }

   void _IncCallDepth() { ++CallDepth; }
   void _DecCallDepth() { --CallDepth; }
   int _GetCallDepth() { return CallDepth; }

   void _IndentToCallDepth()
   {
      for ( int i = 0; i < CallDepth; ++i )
      {
         RWS_DEBUGSTREAM(".");
      }
   }
}

#endif // #ifndef NDEBUG

#ifdef RWS_TRACE_METRICS
namespace RWS 
{
   namespace 
   {
      const unsigned int OUTPUT_BUFFER_SIZE = (64 * 1024);
      
      RwChar outputbuffer[OUTPUT_BUFFER_SIZE + 1];  // 1 extra element for termination

      unsigned int uiIndex = 0;
   }

   /*
   *
   *  Function used by RWS_SEND_TRACE_METRICS macro, sends metrics data buffer back to workspace.
   *
   */
   void _SendMetricsDataBuffer(void)
   {
      RWS_FUNCTION("RWS::_SendMetricsDataBuffer");

      if (uiIndex > 0)
      {
         const RwChar MetricsData[] = "RWS::MetricsData";

         outputbuffer[uiIndex] = 0;   // Terminate packet with 0,0
         uiIndex++;

         if (RwStream *pStream = NetStream::OpenForWrite (MetricsData, uiIndex))
         {
            RwStreamWrite (pStream, outputbuffer, uiIndex);
            RwStreamClose (pStream, 0);
         }

         uiIndex = 0;
      }

      RWS_RETURNVOID();
   }

   /*
   *
   *  Function used by RWS_TRACE_METRIC macro, collects metrics data buffer.
   *
   *  \param pName A pointer to a string containing the name of the metric, this name is used to
   *  identify the metric in the workspace.
   *
   *  \param uiTime Time stamp of metrics data, X axis of Metrics view in Workspace.
   *
   *  \param rVar Value of metric, Y axis of Metrics view in Workspace.
   *
   */
   void _TransmitMetricsData(const RwChar *const pName, RwUInt32 uiTime, RwReal rVar)
   {
      RWS_FUNCTION("RWS::_TransmitMetricsData");

      const unsigned int TEMP_BUFFER_SIZE = 255;

      RwChar buffer[TEMP_BUFFER_SIZE];

      const RwChar *ptr = pName;

      while (*ptr != 0)
      {
         if (*ptr == ' ' || *ptr == '\t') 
         {
            RWS_ASSERTFAIL("RWS_TRACE_METRIC, name cannot contain tabs or spaces.");
         }

         ptr++;
      }

      sprintf(buffer, "%s %d %f", pName, uiTime, rVar);

      RWS_ASSERT((rwstrlen(buffer) < TEMP_BUFFER_SIZE),
         "RWS_TRACE_METRIC, TEMP_BUFFER_SIZE overrun, increase the size of TEMP_BUFFER_SIZE");

      if ( (uiIndex + (rwstrlen(buffer) + 1)) >= OUTPUT_BUFFER_SIZE)
      {
         _SendMetricsDataBuffer();
      }

      rwstrcpy(&outputbuffer[uiIndex], buffer);

      uiIndex += rwstrlen(buffer) + 1;

      RWS_RETURNVOID();
   }
}
#endif


