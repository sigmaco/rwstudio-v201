/*****************************************************************************
 *
 * File :     DebugMacros.h
 *
 * Abstract : RWS debug macros. All macros are implemented in terms of the cdbg
 *            ostream, and all macros compile to NOP in non-debug builds.
 *
 * RWS_DEBUGSTREAM - sends output to cdbg e.g RWS_DEBUGSTREAM( "Time: " << t )
 *
 * RWS_ASSERTFAIL - outputs a message to cdbg and breaks into the debugger
 *
 * RWS_ASSERT/RWS_ASSERTE - tests an expression and raises an RWS_ASSERTFAIL with a
 *                        message if the expression is false. RWS_ASSERT uses a
 *                        client-supplied message, whereas RWS_ASSERTE uses an
 *                        automatically-generated message that includes the failed
 *                        expression
 *
 * RWS_PRE/RWS_POST - similar to RWS_ASSERTE, but intended specifically for PBC pre-/post-conditions
 *
 * RWS_TRACE - sends output to cdbg. Output is indented to the current call depth (see below)
 *
 * RWS_TRACEIF - if ( condition is true) sends output to cdbg. Output is indented to the current
 *              call depth (see below)
 *
 * RWS_FUNCTION/RWS_RETURN - similar to RWS_TRACE, but intended for tracing function entry/exit
 *                         points. Increments the current call depth and prepends a function
 *                         entry/exit symbol to the output
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

#ifndef __DEBUGMACROS_H__
#define __DEBUGMACROS_H__

   // Release Build
   //
#ifdef NDEBUG

   // All NOP in release...
   //
   #define RWS_DEBUGSTREAM(a)
   #define RWS_DEBUGSTREAM_NETWORK_ENABLE()
   #define RWS_DEBUGSTREAM_NETWORK_DISABLE()
   #define RWS_ASSERTFAIL(a)
   #define RWS_ASSERT(a, b)
   #define RWS_ASSERTE(a)
   #define RWS_PRE(a)
   #define RWS_POST(a)
   #define RWS_TRACE(a)
   #define RWS_WARNING(condition, msg)
   #define RWS_TRACE_NO_FUNC(a)
   #define RWS_TRACEIF(a, b)
   #define RWS_FUNCTION(a)
   #define RWS_RETURN(_rwsreturnval) {return(_rwsreturnval);}
   #define RWS_RETURNVOID() {return;}
   #define RWS_DEBUGBREAK

   // Debug Build
   //
#else

#include "debugostream.h"


#ifdef DOXYGEN

   /**
   *
   * \ingroup DebugMacros
   *
   * Generate a trace of the following metrics data, trace's are displayed in the workspace.
   *
   *  \param rate Number of times this macro is executed before the trace
   *  data is sent,For example, if RWS_TRACE_METRIC is called
   *  at 60Hz, then setting the rate to 60 sends the data once a second.
   *
   *  \param name The name of the trace used to identify which trace the data is
   *  appended for, note the name cannot contain tabs or spaces.
   *
   *  \param var The value of the data.
   *
   */

   #define RWS_TRACE_METRIC(rate, name, var)

   /**
   *
   * \ingroup DebugMacros
   *
   * Force the buffered trace metrics data to be sent, note sending metrics data too quickly
   * can result in the network blocking.
   *
   *  \param rate Number of times this macro is executed before the trace data is sent. For
   *  example, if RWS_SEND_TRACE_METRICS is called at 60Hz, then setting the rate to 60 sends
   *  the data once a second.
   *
   *  \note The default implementation of the game framework core calls this from within
   *  mainloop::Poll() in file mainloop\mainloop.cpp
   */

   #define RWS_SEND_TRACE_METRICS(rate)

   /**
   *
   * \ingroup DebugMacros
   *
   * Enable call stack debugging, \see RWSFUNCTION, RWSRETURN, RWSRETURNVOID
   *
   */

   #define RWS_CALLSTACKENABLE

   /**
   *
   * \ingroup DebugMacros
   *
   * Enable trace debugging, \see RWSRACE, RWSTRACEIF
   *
   */

   #define RWS_TRACEENABLE

   /**
   *
   * \ingroup DebugMacros
   *
   * Platform independent 'break' instruction. Halts CPU execution.
   *
   */

   #define RWS_DEBUGBREAK

   /**
   *
   * \ingroup DebugMacros
   *
   * Pipe output to debug stream
   *
   */

   #define RWS_DEBUGSTREAM(a)

   /**
   *
   *  \ingroup DebugMacros
   *
   *  Enable echoing the output of the debug stream to the RenderWare Studio
   *  Workspace.
   *
   */

   #define RWS_DEBUGSTREAM_NETWORK_ENABLE()

   /**
   *
   *  \ingroup DebugMacros
   *
   *  Disable echoing the output of the debug stream to the RenderWare Studio
   *  Workspace.
   *
   */

   #define RWS_DEBUGSTREAM_NETWORK_DISABLE()

   /**
   *
   * \ingroup DebugMacros
   *
   * Generates an assert.
   *
   *  \verbatim

      RWS_ASSERTFAIL("Code Should Never Reach This Point");

      ------------------------------------------------------------------------
      RWS_ASSERTFAIL( Code Should Never Reach This Point )
      MyFile.cpp, line 381
      ------------------------------------------------------------------------

      \endverbatim
   *
   */

   #define RWS_ASSERTFAIL(msg)

   /**
   *
   * \ingroup DebugMacros
   *
   * Generates an assert if the condition is false.
   *
   *  \verbatim

      RWS_ASSERT(ptr!=0,"Illegal Pointer");

      ------------------------------------------------------------------------
      RWS_ASSERT( Illegal Pointer )
      myfile.cpp, line 325
      ------------------------------------------------------------------------

      \endverbatim
   *
   */

   #define RWS_ASSERT(condition, msg)

   /**
   *
   * \ingroup DebugMacros
   *
   * Generates an assert if the condition is false, the condition is then used to generate the 
   * message.
   *
   *  \verbatim

      RWS_ASSERTE(ptr!=0)

      ------------------------------------------------------------------------
      RWS_ASSERT( ptr!=0 Failed )
      myfile.cpp, line 381
      ------------------------------------------------------------------------

      \endverbatim
   *
   */

   #define RWS_ASSERTE(condition)

   /**
   *
   * \ingroup DebugMacros
   *
   * Generates an assert if the condition is false, the condition is then used to generate the 
   * message.
   *
   *  \verbatim

      RWS_PRE(ptr!=0)

      ------------------------------------------------------------------------
      RWS_ASSERT( Failed PRE-condition: ptr!=0 )
      myfile.cpp, line 381
      ------------------------------------------------------------------------

      \endverbatim
   *
   */

   #define RWS_PRE(condition)

   /**
   *
   * \ingroup DebugMacros
   *
   * Generates an assert if the condition is false, the condition is then used to generate the 
   * message.
   *
   *  \verbatim

      RWS_POST(ptr!=0)

      ------------------------------------------------------------------------
      RWS_ASSERT( Failed POST-condition: ptr!=0 )
      myfile.cpp, line 381
      ------------------------------------------------------------------------

      \endverbatim
   *
   */
   
   #define RWS_POST(condition)

   /**
   *
   * \ingroup DebugMacros
   *
   * Generates a trace message. Disabled unless RWS_TRACEENABLE is defined at the
   * top of the translation unit. Can only be used within a RWS_FUNCTION, RWS_RETURN
   * block.
   *
   */

   #define RWS_TRACE(a)

   /**
   *
   * \ingroup DebugMacros
   *
   * if the condition is false, generates a warning message.
   *
   */
   #define RWS_WARNING(condition, msg)

   /**
   *
   * \ingroup DebugMacros
   *
   * Generates a trace message. Disabled unless RWS_TRACEENABLE is defined at the
   * top of the translation unit. Can only be used within a RWS_FUNCTION, RWS_RETURN
   * block. The same as 'RWS_TRACE' except that the function name is not output.
   *
   * \see RWS_TRACE
   *
   */

   #define RWS_TRACE_NO_FUNC(a)

   /**
   *
   * \ingroup DebugMacros
   *
   * Generates a trace message if the condition is true. Disabled unless RWS_TRACEENABLE
   * is defined at the top of the translation unit. Can only be used within a RWS_FUNCTION,
   * RWS_RETURN block.
   *
   */

   #define RWS_TRACEIF(cond, a)

   /**
   *
   * \ingroup DebugMacros
   *
   * Display call depth and function name. Disabled unless RWS_CALLSTACKENABLE is defined
   * at the top of the translation unit.
   *
   */

   #define RWS_FUNCTION(a)

   /**
   *
   * \ingroup DebugMacros
   *
   * Display call depth and function name. Disabled unless RWS_CALLSTACKENABLE is defined
   * at the top of the translation unit, \see RWS_FUNCTION, RWS_RETURNVOID
   *
   */

   #define RWS_RETURN(_rwsreturnval)

   /**
   *
   * \ingroup DebugMacros
   *
   * Display call depth and function name. Disabled unless RWS_CALLSTACKENABLE is defined
   * at the top of the translation unit, \see RWS_FUNCTION, RWS_RETURN
   *
   */

   #define RWS_RETURNVOID()

#else // DOXYGEN


   #include "iocoercion.h"

   namespace RWS 
   {
      // Calldepth-based indenting implementation
      void _IndentToCallDepth();
      void _IncCallDepth();
      void _DecCallDepth();
      int _GetCallDepth();
   }

   #ifdef _WIN32
      #ifdef _MSC_VER
         #define RWS_DEBUGBREAK __asm { int 3 }
      #elif
         #error "Your platform specific break code here"
      #endif
   #elif defined(SKY)
      #ifdef __MWERKS__
         #define RWS_DEBUGBREAK asm volatile ("breakc 0x0")
      #elif SN_TARGET_PS2
         #define RWS_DEBUGBREAK asm ("break 0x0")
      #elif
         #error "Your platform specific break code here"
      #endif
   #elif defined(DOLPHIN)
        #define RWS_DEBUGBREAK OSHalt("RWS_DEBUGBREAK");
   #elif
      #error RWS_DEBUGBREAK is not defined for this platform.
   #endif

   /*
   *  Use these defines to enable / disable sending the debug messages back to the RenderWare Studio
   *  workspace. Make sure that the network module has been initialized before sending any messages though!
   */

   #define OUTPUTSTREAM() RWS::cdbg()

   #define RWS_DEBUGSTREAM(a) ( OUTPUTSTREAM() << a )

   #define RWS_DEBUGSTREAM_NETWORK_ENABLE()  \
   {  \
      OUTPUTSTREAM().rdbuf()->setOutputFlags(debug_buf::rws_workspace_output);   \
   }

   #define RWS_DEBUGSTREAM_NETWORK_DISABLE() \
   {  \
      OUTPUTSTREAM().rdbuf()->setOutputFlags(~debug_buf::rws_workspace_output);  \
   }
   
   #define RWS_ASSERTFAIL(msg) \
   { \
      RWS_DEBUGSTREAM( __FILE__ << "(" << RWS_DEC(__LINE__) << "): " << "ASSERT: " << msg << std::endl ); \
      RWS_DEBUGBREAK; \
   }

   #define RWS_ASSERT(condition, msg) \
      { if (!(condition)) RWS_ASSERTFAIL(msg) }

   #define RWS_ASSERTE(condition) \
      RWS_ASSERT( (condition), #condition " Failed" )

   #define RWS_PRE(condition) \
      RWS_ASSERT( (condition), "Failed PRE-condition: " #condition )

   #define RWS_POST(condition) \
      RWS_ASSERT( (condition), "Failed POST-condition: " #condition )

   #ifdef RWS_TRACEENABLE
      #define RWS_TRACE(a) \
      {  \
         RWS_DEBUGSTREAM(" "); \
         RWS::_IndentToCallDepth(); \
         RWS_DEBUGSTREAM( _rwsfunctionstr << " " << a << std::endl ); \
      }
   #else
      #define RWS_TRACE(a)
   #endif

   #define RWS_WARNING(condition, msg) \
      {  \
         if (!(condition))  \
         {  \
            RWS_DEBUGSTREAM( __FILE__ << "(" << RWS_DEC(__LINE__) << "): " << "WARNING: " << msg << std::endl ); \
         }  \
      }

   #ifdef RWS_TRACEENABLE
      #define RWS_TRACE_NO_FUNC(a) \
      {  \
         RWS_DEBUGSTREAM(" "); \
         RWS_DEBUGSTREAM( a << std::endl ); \
      }
   #else
      #define RWS_TRACE_NO_FUNC(a)
   #endif

   #ifdef RWS_TRACEENABLE
      #define RWS_TRACEIF(cond, a) \
      { \
         if ((cond))  \
         {  \
            RWS_DEBUGSTREAM(" "); \
            RWS::_IndentToCallDepth(); \
            RWS_DEBUGSTREAM( a << std::endl ); \
         }  \
      }
   #else
      #define RWS_TRACEIF(cond, a)
   #endif

   #ifdef RWS_CALLSTACKENABLE
      #define RWS_FUNCTION(a) \
         const char _rwsfunctionstr[] = a; \
         int _rwscalldepth = RWS::_GetCallDepth(); \
         RWS::_IncCallDepth(); \
         RWS_DEBUGSTREAM(">"); \
         RWS::_IndentToCallDepth(); \
         RWS_DEBUGSTREAM(_rwsfunctionstr << std::endl ); 
   #else
      #define RWS_FUNCTION(a) \
         const char _rwsfunctionstr[] = a; \
         int _rwscalldepth = RWS::_GetCallDepth(); \
         RWS::_IncCallDepth();
   #endif

   #ifdef RWS_CALLSTACKENABLE
      #define RWS_RETURN(_rwsreturnval) \
      {  \
         RWS_DEBUGSTREAM("<"); \
         RWS::_IndentToCallDepth(); \
         RWS_DEBUGSTREAM( _rwsfunctionstr << " " << std::endl); \
         RWS::_DecCallDepth(); \
         if (RWS::_GetCallDepth() != _rwscalldepth){   \
            RWS_DEBUGSTREAM( __FILE__ << "(" << RWS_DEC(__LINE__) << "): " << "ASSERT: "); \
            RWS_DEBUGSTREAM("Missing RWS_FUNCTION, RWS_RETURN or RWS_RETURNVOID" << std::endl);RWS_DEBUGBREAK;  \
         } \
         return (_rwsreturnval);    \
      }
   #else
      #define RWS_RETURN(_rwsreturnval) \
      {  \
         RWS::_DecCallDepth(); \
         if (RWS::_GetCallDepth() != _rwscalldepth){   \
            RWS_DEBUGSTREAM( __FILE__ << "(" << RWS_DEC(__LINE__) << "): " << "ASSERT: "); \
            RWS_DEBUGSTREAM("Missing RWS_FUNCTION, RWS_RETURN or RWS_RETURNVOID" << std::endl);RWS_DEBUGBREAK;  \
         } \
         return (_rwsreturnval);    \
      }
   #endif

   #ifdef RWS_CALLSTACKENABLE
      #define RWS_RETURNVOID() \
      {  \
         RWS_DEBUGSTREAM("<"); \
         RWS::_IndentToCallDepth(); \
         RWS_DEBUGSTREAM( _rwsfunctionstr << " " << std::endl); \
         RWS::_DecCallDepth(); \
         if (RWS::_GetCallDepth() != _rwscalldepth){  \
            RWS_DEBUGSTREAM( __FILE__ << "(" << RWS_DEC(__LINE__) << "): " << "ASSERT: "); \
            RWS_DEBUGSTREAM("Missing RWS_FUNCTION, RWS_RETURN or RWS_RETURNVOID" << std::endl);RWS_DEBUGBREAK;  \
         } \
         return;     \
      }
   #else
      #define RWS_RETURNVOID() \
      {  \
         RWS::_DecCallDepth(); \
         if (RWS::_GetCallDepth() != _rwscalldepth){  \
            RWS_DEBUGSTREAM( __FILE__ << "(" << RWS_DEC(__LINE__) << "): " << "ASSERT: "); \
            RWS_DEBUGSTREAM("Missing RWS_FUNCTION, RWS_RETURN or RWS_RETURNVOID" << std::endl);RWS_DEBUGBREAK;  \
         } \
         return;     \
      }
   #endif

#endif

#endif // NDEBUG

// Macros which are always enabled...

#ifdef DOXYGEN

   /**
   *
   * \ingroup DebugMacros
   *
   * Displays a message on the compilers output window during compilation. Must be used with
   * the #pragma directive. Adds in the file and line number where the message is placed.
   *
   * For example...
   *
   * \verbatim
    
        #pragma RWS_COMP_MSG(display manager included)
    
   * \endverbatim
   *
   * ... will display something similar to (depending on platform)...
   *
   * \verbatim
    
        c:\project\gfxsys\gfx.cpp (124) - MESSAGE - display manager included
    
   * \endverbatim
   *
   */

   #define RWS_COMP_MSG(uq_str)

   /**
   *
   * \ingroup DebugMacros
   *
   * Displays a warning on the compilers output window during compilation. Must be used with
   * the #pragma directive. Adds in the file and line number where the warning is placed.
   *
   * For example...
   *
   * \verbatim

        #pragma RWS_COMP_WARN(display manager may not support mode)
    
   * \endverbatim
   *
   * ... will display something similar to (depending on platform)...
   *
   * \verbatim
   
        c:\project\gfxsys\gfx.cpp (208) + WARNING + display manager may not support mode
    
   * \endverbatim
   *
   */

   #define RWS_COMP_WARN(uq_str)

   /**
   *
   * \ingroup DebugMacros
   *
   * Displays an error on the compilers output window during compilation. Must be used with
   * the #pragma directive. Adds in the file and line number where the error is placed.
   *
   * For example...
   *
   * \verbatim

        #pragma RWS_COMP_ERR(display manager must have mode specified)
    
   * \endverbatim
   *
   * ... will display something similar to (depending on platform)...
   *
   * \verbatim
   
        c:\project\gfxsys\gfx.cpp (208) ## ERROR ## display manager must have mode specified
   
   * \endverbatim
   *
   */

   #define RWS_COMP_ERR(uq_str)

#else

   #define STR(str)  #str
   #define STR2(str) STR(str)
   #define RWS_COMP_MSG(uq_str) message (__FILE__ "(" STR2(__LINE__) ") : - MESSAGE - " #uq_str)
   #define RWS_COMP_WARN(uq_str) message (__FILE__ "(" STR2(__LINE__) ") : + WARNING + " #uq_str)
   #define RWS_COMP_ERR(uq_str) message (__FILE__ "(" STR2(__LINE__) ") : ## ERROR ## " #uq_str)

#endif      // Always enabled & !DOXYGEN

#endif // __DEBUGMACROS_H__

#ifdef RWS_TRACE_METRICS

#include "..\..\mainloop\render.h"

   namespace RWS
   {
      void _TransmitMetricsData(const RwChar * const name, RwUInt32 time, RwReal var);
      void _SendMetricsDataBuffer(void);
   }

   #define RWS_TRACE_METRIC(rate, name, var) {\
      static RwUInt32 count = 0;\
      count++;if (rate && count >= rate)\
      {\
         count = 0;\
         RWS::_TransmitMetricsData((name), \
               static_cast<RwUInt32>((RWS::MainLoop::Render::GetFrameCount())), \
               static_cast<RwReal>((var)));\
      }\
   }

   #define RWS_SEND_TRACE_METRICS(rate) {\
      static RwUInt32 count = 0;\
      count++;if (rate && count >= rate)\
      {\
         count = 0;\
         RWS::_SendMetricsDataBuffer();\
      }\
   }

#else
   #define RWS_TRACE_METRIC(rate, name, var)
   #define RWS_SEND_TRACE_METRICS(rate)
#endif
