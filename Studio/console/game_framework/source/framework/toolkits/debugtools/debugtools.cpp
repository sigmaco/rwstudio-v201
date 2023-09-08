/*****************************************************************************
*
* File :     DebugTools.cpp
*
* Notes : 
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

//#define RWS_CALLSTACKENABLE
//#define RWS_TRACEENABLE

#ifdef RWS_DEBUGTOOLS

//////////////////////////////////////////////////////////////////
//
// System Includes
//
#ifdef DOLPHIN
#include <string.h>
#endif

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "debugtools.h"
#include "framework/core/macros/debugmacros.h"
#include "framework/mainloop/mainloop.h"
#include "framework/mainloop/logic.h"
#include "framework/mainloop/render.h"
#include "framework/toolkits/world/clevel.h"
#include "framework/toolkits/time/gftime.h"

namespace RWS
{
   namespace DebugTools
   {
      RwReal m_fcounter;

      /**
      *
      *  \ingroup DebugTools
      *  \page Overview Overview
      *
      *  DebugTools provides simple immediate mode graphics functionality that
      *  is buffered. Lines, polygons, etc. can be requested at any point during
      *  the execution of the application, but are drawn by the DisplayDebugTools
      *  in response to a render event. Note that debug text is only displayed if 
      *  DebugSwitches::m_enable_printf is enabled. CDebugTools exposes this
      *  switch.
      *
      */
      namespace 
      {
         // RenderWare simple font/character printing lib
         //
         RtCharset *charset;
         RtCharsetDesc charsetDesc;

         // Max size of debug buffer per frame
         //
         const unsigned int MAX_DEBUG_TOOLS_BUFFER_SIZE = 2048;

         RwChar debugtools_buffer[MAX_DEBUG_TOOLS_BUFFER_SIZE];
         
         RwUInt32 PreTick_Time;
         RwUInt32 PostTick_Time;

         const unsigned int MAXLINES = 8192;

         RwV3d          m_LineStart[MAXLINES], m_LineEnd[MAXLINES];
         RwRGBA         m_LineCol[MAXLINES];
         RwUInt32       m_LineCount;

         const unsigned int MAXTEXTS = 128;   /* Max number of text strings to draw */

         const unsigned int MAXTEXTLEN = 128;  /* Max length of a text string */

         RwChar         m_TextString[MAXTEXTS][MAXTEXTLEN];/* String to draw for each text line */
         RwV3d          m_TextPos[MAXTEXTS];               /* World coordinates for each text line */
         RwV3d          m_tTextPos[MAXTEXTS];              /* Transformed world coordinates for each text line */
         RwUInt32       m_TextCount;                       /* Number of text lines to draw */

         const unsigned int MAXELLIPSOIDS = 128;

         RwV3d          m_EllipseCenter[MAXELLIPSOIDS];
         RwV3d          m_EllipseRadii[MAXELLIPSOIDS];
         RwRGBA         m_EllipseCol[MAXELLIPSOIDS];
         RwUInt32       m_EllipseCount;   

         const unsigned int MAXPOLYS = 1024;

         RwV3d          m_PolyV1[MAXPOLYS], m_PolyV2[MAXPOLYS], m_PolyV3[MAXPOLYS];
         RwRGBA         m_PolyCol[MAXPOLYS];
         RwUInt32       m_PolyCount;

         RwIm3DVertex  m_tLineVerts[MAXLINES * 2];

         const unsigned int DISPATCHSIZE = 4096;

      }

      /**
      *
      *  \ingroup DebugTools
      *
      *  Open debugtools toolkit, creates the font used to display debug text.
      *
      */
      void Open(void)
      {
         RWS_FUNCTION("RWS::DebugTools::Open");

         DebugSwitches::Open();

         static RwRGBA ForegroundColor = {255, 255, 255, 128};
         static RwRGBA BackgroundColor = {0, 0,0, 128};
      
         RtCharsetOpen ();
         charset = RtCharsetCreate (&ForegroundColor, &BackgroundColor);
         RtCharsetGetDesc(charset, &charsetDesc);
      
         // zero terminate start of string buffers
         //
         debugtools_buffer[0] = 0;  

         RWS_RETURNVOID();
      }

      /**
      *
      *  \ingroup DebugTools
      *
      *  Close debugtools toolkit, destroys the font used to display debug text.
      *
      */
      void Close(void)
      {
         RWS_FUNCTION("RWS::DebugTools::Close");

         DebugSwitches::Close();

         RtCharsetDestroy (charset);
         RtCharsetClose ();

         RWS_RETURNVOID();
      }

      /*
      *
      *  \ingroup DebugTools
      *
      *  Dumps a string specified via RtCharsetPrintBuffered to the
      *  screen and the specified x, y co-ordinates. RtCharsetPrintBuffered doesn't
      *  handle \n, so this function breaks the string into smaller strings
      *  so that they are displayed correctly.
      *
      *  Note: The text is rendered into the current camera, and this function
      *  can only be called within RwCameraBeginUpdate and RwCameraEndUpdate.
      *
      *  \param pchar A pointer to the string.
      *  \param x The horizontal position of text on screen.
      *  \param y The vertical position of text on screen.
      *
      */
      void DumpPrintfWithLineFeed (const RwChar *pchar, RwInt32 x, RwInt32 *y)
      {
         RWS_FUNCTION("RWS::DebugTools::DumpPrintfWithLineFeed");

         RwChar buffer[MAX_DEBUG_TOOLS_BUFFER_SIZE];
         const RwChar *ps = pchar;
         const RwChar *pe = pchar;
      
         while (*ps != 0)
         {
            while (!(*pe == 0 || *pe == '\n')) pe++;
         
            rwstrncpy (buffer, ps, (pe - ps));
         
            buffer[pe - ps] = 0;  // Terminate
         
            RwChar lbuffer[MAX_DEBUG_TOOLS_BUFFER_SIZE];
         
            for (RwUInt32 i = 0, j = 0;i < rwstrlen(buffer) + 1;i++)
            {
               lbuffer[j] = buffer[i];
            
               if (j >= 70 || lbuffer[j] == 0) 
               {
                  lbuffer[++j] = 0;
               
                  RtCharsetPrintBuffered (charset, (const RwChar *)lbuffer, x, *y, false) ;
               
                  j = 0;
               
                  *y += 14;
               }
               else
               {
                  j++;
               }
            }
         
            if (*pe != 0) pe++;   // Next String
         
            ps = pe;
         }

         RWS_RETURNVOID();
      }

      /*
      *
      *  \ingroup DebugTools
      *
      *  Dumps text accumulated via Printf to screen using DumpPrintfWithLineFeed.
      *
      *  Note: The text is rendered into the current camera, and this function
      *  can only be called within RwCameraBeginUpdate and RwCameraEndUpdate.
      *
      */
      void DumpPrintf(void)
      {
         RWS_FUNCTION("RWS::DebugTools::DumpPrintf");

         RwInt32 x, y;
         
         x = 32;
         y = 32;
        
         DumpPrintfWithLineFeed (debugtools_buffer, x, &y);
      
         debugtools_buffer[0] = 0; // Reset Buffer Always

         RWS_RETURNVOID();
      }

      /*
      *
      *  \ingroup DebugTools
      *
      *  A standard printf function, except that output is directed to the screen.
      *  The buffer is cleared with each call to DumpPrintf.
      *
      */
      void _Printf (const char *fmt, ...)
      {
         RWS_FUNCTION("RWS::DebugTools::_Printf");

         const unsigned int temp_buffer_size = MAX_DEBUG_TOOLS_BUFFER_SIZE;

         char buffer[temp_buffer_size];

         va_list argptr;
      
         va_start (argptr, fmt);

// _vsnprintf would be the preferred approach, but it is unsupported on PS2 therefore
// for consistency vsprintf is used even thought this is potentially unsafe as it may
// overrun the buffer.
//
//       _vsnprintf (buffer, temp_buffer_size, fmt, argptr);
         vsprintf (buffer, fmt, argptr); 

         RWS_ASSERT(strlen(buffer) < temp_buffer_size,
               "DebugTools::Printf vsprintf has exceeded temporary buffer length."
               "Expect stack to be corrupt. Max size = " << temp_buffer_size);

         va_end (argptr);
      
         // Check if its safe to append this string to the previous strings
         //
         if ( (rwstrlen (buffer) + rwstrlen (debugtools_buffer) ) >= MAX_DEBUG_TOOLS_BUFFER_SIZE)
            RWS_RETURNVOID();
      
         rwstrcat(debugtools_buffer, buffer);

         RWS_RETURNVOID();
      }

      /**
      *
      *  \ingroup DebugTools
      *
      *  A standard printf function, except that output is directed to screen.
      *  The buffer is cleared with each call to DumpPrintf.
      *
      */
      void Printf (const char *fmt, ...)
      {
         RWS_FUNCTION("RWS::DebugTools::Printf");

         if (DebugSwitches::GetSwitch(DebugSwitches::m_enable_printf))
         {
            const unsigned int temp_buffer_size = MAX_DEBUG_TOOLS_BUFFER_SIZE;

            char buffer[temp_buffer_size];

            va_list argptr;
      
            va_start (argptr, fmt);

   // _vsnprintf would be the preferred approach, but it is unsupported on PS2 therefore
   // for consistency vsprintf is used even thought this is potentially unsafe as it may
   // overrun the buffer.
   //
   //       _vsnprintf (buffer, temp_buffer_size, fmt, argptr);
            vsprintf (buffer, fmt, argptr); 

            RWS_ASSERT(strlen(buffer) < temp_buffer_size,
                  "DebugTools::Printf vsprintf has exceeded temporary buffer length."
                  "Expect stack to be corrupt. Max size = " << temp_buffer_size);

            va_end (argptr);
      
            // Check if its safe to append this string to the previous strings
            //
            if ( (rwstrlen (buffer) + rwstrlen (debugtools_buffer) ) >= MAX_DEBUG_TOOLS_BUFFER_SIZE)
               RWS_RETURNVOID();
      
            rwstrcat(debugtools_buffer, buffer);
         }

         RWS_RETURNVOID();
      }

      /**
      *
      *  \ingroup DebugTools
      *
      *  A standard printf function, except that output is directed to screen at the specified x, y coordinate.
      *
      *  Note: The text is rendered into the current camera, and this function can only be
      *  called within RwCameraBeginUpdate and RwCameraEndUpdate.
      *
      *  \param x The screen X position to draw the text.
      *  \param y The Screen Y position to draw the text.
      *  \param fmt The format string of the text.
      *
      */
      void PrintfXY (const RwInt32 x, const RwInt32 y, const char *fmt, ...)
      {
         RWS_FUNCTION("RWS::DebugTools::PrintfXY");

         const unsigned int temp_buffer_size = MAX_DEBUG_TOOLS_BUFFER_SIZE;

         char buffer[temp_buffer_size];
         va_list argptr;
         //////////////////////////////////////////////////////////////////
         // Process the varargs part and create a string
         //
         va_start (argptr, fmt);
// _vsnprintf would be the preferred approach, but it is unsupported on PS2 therefore
// for consistency vsprintf is used even thought this is potentially unsafe as it may
// overrun the buffer.
//
//         _vsnprintf (buffer, temp_buffer_size, fmt, argptr);
         vsprintf (buffer, fmt, argptr);

         RWS_ASSERT(strlen(buffer) < temp_buffer_size,
               "DebugTools::Printf vsprintf has exceeded temporary buffer length."
               "Expect stack to be corrupt. Max size = " << temp_buffer_size);

         va_end (argptr);
      
         //////////////////////////////////////////////////////////////////
         // Display the string at the given screen coordinates
         //
         //RtCharsetPrintBuffered (charset, (const RwChar *)buffer, x, y, false) ;
      
         RwInt32 ty = y;
      
         DumpPrintfWithLineFeed(buffer, x, &ty);

         RWS_RETURNVOID();
      }

      /*
      *
      *  \ingroup DebugTools
      *
      *  \return The current calculated frame rate - that is, the number of frames rendered.
      *  during the last 1 second.
      *
      */
      RwUInt32 GetFrameRate (void)
      {
         RWS_FUNCTION("RWS::DebugTools::GetFrameRate");

         static RwUInt32 LastTime = Time::GetTime();
         static RwUInt32 FrameRate = 0;
         static RwUInt32 LastFramesRendered = MainLoop::Render::GetFrameCount ();
         
         static const RwUInt32 DeltaTimeLimit = 1000; // Delta time before framerate is recalculated.
      
         RwUInt32 ThisTime = Time::GetTime();
         RwUInt32 DeltaTime = ThisTime - LastTime;
      
         // We'll update the frame rate once every second
         if (DeltaTime > DeltaTimeLimit)
         {
            const RwUInt32 FrameCount = MainLoop::Render::GetFrameCount () - LastFramesRendered;
         
            FrameRate = (FrameCount * DeltaTimeLimit) / DeltaTime;
         
            LastTime = ThisTime;

            LastFramesRendered = MainLoop::Render::GetFrameCount ();
         }
      
         RWS_RETURN( FrameRate );
      }

      /*
      *
      *  \ingroup DebugTools
      *
      *  Reset debug tools buffers.
      *
      */
      void Reset(void)
      {
         RWS_FUNCTION("RWS::DebugTools::Reset");

         m_LineCount = 0;
         m_EllipseCount = 0;
         m_TextCount = 0;
         m_PolyCount = 0;

         RWS_RETURNVOID();
      }
   
      /**
      *
      *  \ingroup DebugTools
      *
      *  Display a matrix on screen using Printf.
      *
      *  \param pMatrix A pointer to an RwMatrix object that will be displayed using Printf.
      *
      */
      void PrintfMatrix(RwMatrix *pMatrix)
      {
         RWS_FUNCTION("RWS::DebugTools::PrintfMatrix");

         Printf("%f %f %f\n", pMatrix->right.x, pMatrix->right.y, pMatrix->right.z);
         Printf("%f %f %f\n", pMatrix->up.x, pMatrix->up.y, pMatrix->up.z);
         Printf("%f %f %f\n", pMatrix->at.x, pMatrix->at.y, pMatrix->at.z);
         Printf("%f %f %f\n\n", pMatrix->pos.x, pMatrix->pos.y, pMatrix->pos.z);

         RWS_RETURNVOID();
      }

      /*
      *
      *  \ingroup DebugTools
      *
      *  Trace RenderWare metrics information. Sends metrics information back to the Workspace.
      *
      */
      void TraceMetrics(RwMetrics *pRwMetrics)
      {
         RWS_FUNCTION("RWS::DebugTools::TraceMetrics");

         //
         // Get RenderWare Metrics And Display
         // 
         RwUInt32 framerate = GetFrameRate();

         RwUInt32 Time = PostTick_Time - PreTick_Time;

         // This section handles sending metrics data back to the workspace
         //
         if (DebugSwitches::GetSwitch(DebugSwitches::m_enable_Metric_CPUTime))
         {
            RWS_TRACE_METRIC(6, "CPU_Time", Time);
         }

         if (DebugSwitches::GetSwitch(DebugSwitches::m_enable_Metric_RwResourcesGetArenaUsage))
         {
            RWS_TRACE_METRIC(6, "RwResourcesGetArenaUsage", RwResourcesGetArenaUsage());
         }

         if (DebugSwitches::GetSwitch(DebugSwitches::m_enable_Metric_FramesPerSecond))
         {
            RWS_TRACE_METRIC(6, "FramesPerSecond", framerate);
         }

         if (DebugSwitches::GetSwitch(DebugSwitches::m_enable_Metric_Triangles))
         {
            RWS_TRACE_METRIC(6, "Triangles", pRwMetrics->numTriangles);
         }
         
         if (DebugSwitches::GetSwitch(DebugSwitches::m_enable_Metric_Processed_Triangles))
         {
            RWS_TRACE_METRIC(6, "Processed_Triangles", pRwMetrics->numProcTriangles);
         }

         if (DebugSwitches::GetSwitch(DebugSwitches::m_enable_Metric_Vertices))
         {
            RWS_TRACE_METRIC(6, "Vertices", pRwMetrics->numVertices);
         }

         if (DebugSwitches::GetSwitch(DebugSwitches::m_enable_Metric_Size_Texture_Uploads))
         {
            RWS_TRACE_METRIC(6, "Size_Texture_Uploads", pRwMetrics->sizeTextureUploads);
         }

         if (DebugSwitches::GetSwitch(DebugSwitches::m_enable_Metric_Num_Texture_Uploads))
         {
            RWS_TRACE_METRIC(6, "Num_Texture_Uploads", pRwMetrics->numTextureUploads);
         }

#if defined(SKY)
         if (DebugSwitches::GetSwitch(DebugSwitches::m_enable_Metric_PS2))
         {
           // This section 
            //
            RwSkyMetrics *skyMetrics = (RwSkyMetrics *)pRwMetrics->devSpecificMetrics;
            
            if ((skyMetrics) && (skyMetrics->profTotal))
            {
               RwInt32             val;
               
               val = (100 * skyMetrics->vu1Running) / skyMetrics->profTotal;
               RWS_TRACE_METRIC(6, "vu1", val);
               
               val = (100 * skyMetrics->dma1Running) / skyMetrics->profTotal;
               RWS_TRACE_METRIC(6, "dma", val);

               val = (100 * skyMetrics->dma2Running) / skyMetrics->profTotal;
               RWS_TRACE_METRIC(6, "dma2", val);
               
               // RenderWare Version Precedes 3.4
               //      
#if (rwLIBRARYCURRENTVERSION < 0x34002)
               val = (RwInt32) ((float) skyMetrics->flipPktToFlipTime / 14745.60f);
               RWS_TRACE_METRIC(6, "Flip", val);
#endif
               
               // RenderWare Version Exceeds 3.4
               //      
#if (rwLIBRARYCURRENTVERSION >= 0x34002)
               val = (100 * skyMetrics->cpuRunning) / skyMetrics->profTotal;
               RWS_TRACE_METRIC(6, "CPU_Utilization", val);
#endif
               
               RWS_TRACE_METRIC(6, "VSyncs", skyMetrics->vSyncsSinceLastFlip);
            }
         }
#endif

         RWS_RETURNVOID();
      }

      /*
      *
      *  \ingroup DebugTools
      *
      *  Display RenderWare metrics information.
      *
      *  \param pRwMetrics A pointer to an RwMetrics object used to display the metrics data using _Printf.
      *
      */
      void PrintfMetrics(RwMetrics *pRwMetrics)
      {
         RWS_FUNCTION("RWS::DebugTools::PrintfMetrics");

         RwUInt32 framerate = GetFrameRate();
         
         RwUInt32 Time = PostTick_Time - PreTick_Time;
         
         // This section displays metrics info on screen. 
         //
         if (DebugSwitches::GetSwitch(DebugSwitches::m_enable_metrics_printf))
         {
            // Arena Size/Usage
            //
            _Printf("RwResourcesGetArenaSize %d\n", RwResourcesGetArenaSize());
            _Printf("RwResourcesGetArenaUsage %d\n", RwResourcesGetArenaUsage());
            
            // Display frame rate
            //
            _Printf ("\nCurrent Frame Number %u\nFrames Per Second %u\n",
               MainLoop::Render::GetFrameCount (),
               framerate);

            // Scene metrics
            //
            _Printf ("Triangles per second %d\nTriangles drawn per second %d\n",
               pRwMetrics->numProcTriangles * framerate,
               pRwMetrics->numTriangles * framerate);

            _Printf ("numTriangles %d\nnumProcTriangles %d\n",
               pRwMetrics->numTriangles,
               pRwMetrics->numProcTriangles);

            _Printf("numVertices %d\n",
               pRwMetrics->numVertices);

            _Printf("numTextureUploads %d\nsizeTextureUploads %d\nnumResourceAllocs %d\n",
               pRwMetrics->numTextureUploads,
               pRwMetrics->sizeTextureUploads,
               pRwMetrics->numResourceAllocs);
            
            _Printf ("numTriangles:numProcTriangles 1:%f\n",
               (RwReal)pRwMetrics->numProcTriangles / (RwReal)pRwMetrics->numTriangles);
            
            _Printf ("CPU Time PreTick %d PostTick %d -> %d ms\n",
               PreTick_Time,
               PostTick_Time,
               Time);
         }

         RWS_RETURNVOID();
      }  

      /*
      *
      *  \ingroup DebugTools
      *
      *  Displays PS2-specific metrics info if available.
      *
      *  \param pRwMetrics A pointer to an RwMetrics object, used to display the metrics data using _Printf.
      */
      void PrintfSkyMetrics(RwMetrics *pRwMetrics)
      {
         RWS_FUNCTION("RWS::DebugTools::PrintfSkyMetrics");

#if defined (SKY)
         if (DebugSwitches::GetSwitch(DebugSwitches::m_enable_metrics_printf))
         {
            // This section 
            //
            RwSkyMetrics *skyMetrics = (RwSkyMetrics *)pRwMetrics->devSpecificMetrics;
            
            if ((skyMetrics) && (skyMetrics->profTotal))
            {
               RwInt32             val, integer, fraction;
               
               val = (10000 * skyMetrics->vu1Running) / skyMetrics->profTotal;
               integer = val / 100;
               fraction = val % 100;
               _Printf("vu1 utilization = %02d.%02d%%\n", integer, fraction);
               
               val = (10000 * skyMetrics->dma1Running) / skyMetrics->profTotal;
               integer = val / 100;
               fraction = val % 100;
               _Printf("dma1 utilization = %02d.%02d%%\n", integer, fraction);
               
               val = (10000 * skyMetrics->dma2Running) / skyMetrics->profTotal;
               integer = val / 100;
               fraction = val % 100;
               _Printf("dma2 utilization = %02d.%02d%%\n", integer, fraction);
               
               // RenderWare Version Precedes 3.4
               //      
#if (rwLIBRARYCURRENTVERSION < 0x34002)
               val = (RwInt32) ((float) skyMetrics->flipPktToFlipTime / 14745.60f);
               integer = val / 10;
               fraction = val % 10;
               _Printf ("Flip pkt to flip = %02d.%01d\n", integer,  fraction);
#endif
               
               // RenderWare Version Exceeds 3.4
               //      
#if (rwLIBRARYCURRENTVERSION >= 0x34002)
               val = (10000 * skyMetrics->cpuRunning) / skyMetrics->profTotal;
               integer = val / 100;
               fraction = val % 100;
               _Printf("cpu utilization = %02d.%02d%%\n", integer, fraction);
#endif
               
               _Printf("VSyncs between flips = %d\n", skyMetrics->vSyncsSinceLastFlip);
            }
         }
#endif
         RWS_RETURNVOID();
      }

      /**
      *
      *  \ingroup DebugTools
      *
      *  Draws text at a given 3D position.
      *
      *  \param pPos A pointer to an RwV3d object: the position where the text 
      *  should be displayed in world coordinates.
      *
      *  \param pText A pointer to a string containing the text to display.
      */
      void  PrintfXYZ(const RwV3d *pPos, const RwChar *pText)
      {
         RWS_FUNCTION("RWS::DebugTools::PrintfXYZ");

         if (!DebugSwitches::GetSwitch(DebugSwitches::m_enabled)) {RWS_RETURNVOID();}
         else
         {
            RwUInt32 len = rwstrlen (pText) + 1;
         
            if (len > MAXTEXTLEN) {RWS_RETURNVOID();} // Too long to display
            else if (m_TextCount < MAXTEXTS)
            {
               sprintf(m_TextString[m_TextCount], "%s", pText);
            
               m_TextPos[m_TextCount] = *pPos;
               m_TextCount++;
            }
         }

         RWS_RETURNVOID();
      }

      /**
      *
      *  \ingroup DebugTools
      *
      *  Draws a line between the two (3D) points specified.
      *  The color of the line is specified by the red, green, blue values.
      *
      *  \param p1 A pointer to an RwV3d object: the start position of the line in world coordinates.
      *  \param p2 A pointer to an RwV3d object, the end position of the line in world coordinates.
      *  \param red The red component of the color.
      *  \param green The green component of the color.
      *  \param blue The blue component of the color.
      *
      */
      void Line(const RwV3d * p1, const RwV3d * p2, RwUInt8 red, RwUInt8 green, RwUInt8 blue)
      {
         RWS_FUNCTION("RWS::DebugTools::Line");

         if (!DebugSwitches::GetSwitch(DebugSwitches::m_enabled)) {RWS_RETURNVOID();}
         else if (m_LineCount < MAXLINES)
         {
            m_LineCol[m_LineCount].red = red;
            m_LineCol[m_LineCount].green = green;
            m_LineCol[m_LineCount].blue = blue;

            m_LineStart[m_LineCount] = *p1;
            m_LineEnd[m_LineCount] = *p2;
            m_LineCount++;
         }

         RWS_RETURNVOID();
      }

      /**
      *
      *  \ingroup DebugTools
      *
      *  Draws an arrow between the two (3D) points specified.
      *  The color of the line is specified by the red, green, blue values.
      *
      *  \param p1 A pointer to an RwV3d object, the start position of the arrow in world coordinates.
      *  \param p2 A pointer to an RwV3d object, the end position of the arrow in world coordinates.
      *  \param red The red component of the color.
      *  \param green The green component of the color.
      *  \param blue The blue component of the color.
      *
      */
      void Arrow(const RwV3d * p1, const RwV3d * p2, RwUInt8 red, RwUInt8 green, RwUInt8 blue)
      {
         RWS_FUNCTION("RWS::DebugTools::Line");

         if (!DebugSwitches::GetSwitch(DebugSwitches::m_enabled)) {RWS_RETURNVOID();}
         else if (m_LineCount < MAXLINES)
         {
            RwV3d X;
            RwV3d Y;
            RwV3d Z;

            RwV3d dotvector;

            RwV3dSub(&X, p2, p1);
            
            RwReal length = RwV3dLength(&X);

            if (length != 0.0f)
            {
               RwV3dScale(&X, &X, 1.0f / length);
            }

            if (fabs(X.x) < fabs(X.y) && fabs(X.x) < fabs(X.z))
            {
               dotvector.x = 1.0f;
               dotvector.y = 0.0f;
               dotvector.z = 0.0f;
            }
            else
            if (fabs(X.y) < fabs(X.x) && fabs(X.y) < fabs(X.z))
            {
               dotvector.x = 0.0f;
               dotvector.y = 1.0f;
               dotvector.z = 0.0f;
            }
            else
            {
               dotvector.x = 0.0f;
               dotvector.y = 0.0f;
               dotvector.z = 1.0f;
            }

            RwV3dCrossProduct(&Y, &X, &dotvector);
            RwV3dCrossProduct(&Z, &X, &Y);

            RwV3dScale(&X, &X, length * 0.9f);
            RwV3dAdd(&X, &X, p1);

            RwV3dScale(&Y, &Y, length * 0.1f);
            RwV3dScale(&Z, &Z, length * 0.1f);

            RwV3d tp1, tp2, tp3, tp4;

            RwV3dSub(&tp1, &X, &Y);
            RwV3dAdd(&tp2, &X, &Y);

            RwV3dSub(&tp3, &X, &Z);
            RwV3dAdd(&tp4, &X, &Z);

            Triangle(&tp1, p2, &tp2, red, green, blue);
            Triangle(&tp3, p2, &tp4, red, green, blue);

            Line(p1, p2, red, green, blue);
         }

         RWS_RETURNVOID();
      }
      /**
      *
      *  \ingroup DebugTools
      *
      *  Draws a triangle specified by the three (3D) points.
      *  The color of the triangle is specified by the red, green, blue values.
      *
      *  \param p1 A pointer to an RwV3d object, the 1st vertex of the triangle in world coordinates.
      *  \param p2 A pointer to an RwV3d object, the 2nd vertex of the triangle in world coordinates.
      *  \param p3 A pointer to an RwV3d object, the 3rd vertex of the triangle in world coordinates.
      *  \param red The red component of the color.
      *  \param green The green component of the color.
      *  \param blue The blue component of the color.
      */
      void Triangle(const RwV3d * p1, const RwV3d * p2, const RwV3d *p3, RwUInt8 red, RwUInt8 green, RwUInt8 blue)
      {
         RWS_FUNCTION("RWS::DebugTools::Triangle");

         if (!DebugSwitches::GetSwitch(DebugSwitches::m_enabled)) {RWS_RETURNVOID();}
         else if (m_PolyCount < MAXPOLYS)
         {
            m_PolyCol[m_PolyCount].red = red;
            m_PolyCol[m_PolyCount].green = green;
            m_PolyCol[m_PolyCount].blue = blue;
         
            m_PolyV1[m_PolyCount] = *p1;
            m_PolyV2[m_PolyCount] = *p2;
            m_PolyV3[m_PolyCount] = *p3;
         
            m_PolyCount++;
         }
         RWS_RETURNVOID();
      }
   
      /**
      *
      *  \ingroup DebugTools
      *
      *  Draws an ellipsoid specified by the (3D) center point, the
      *  radii of the three axis xr, yr, zr, and the color specified by the red,
      *  green, blue values.
      *
      *  \param center A pointer to an RwV3d object that is the center of the ellipse.
      *  \param xr The size of the ellipse along the X axis.
      *  \param yr The size of the ellipse along the Y axis.
      *  \param zr The size of the ellipse along the Z axis.
      *  \param red The red component of the color.
      *  \param green The green component of the color.
      *  \param blue The blue component of the color.
      *
      */
      void  Ellipsoid(const RwV3d *center, RwReal xr,RwReal yr,RwReal zr,RwUInt8 red, RwUInt8 green, RwUInt8 blue)
      {
         RWS_FUNCTION("RWS::DebugTools::Ellipsoid");

         if (!DebugSwitches::GetSwitch(DebugSwitches::m_enabled)) {RWS_RETURNVOID();}
         else if (m_EllipseCount < MAXELLIPSOIDS)
         {
            m_EllipseCol[m_EllipseCount].red = red;
            m_EllipseCol[m_EllipseCount].green = green;
            m_EllipseCol[m_EllipseCount].blue = blue;

            m_EllipseCenter[m_EllipseCount] = *center;

            m_EllipseRadii[m_EllipseCount].x = xr;
            m_EllipseRadii[m_EllipseCount].y = yr;
            m_EllipseRadii[m_EllipseCount].z = zr;

            m_EllipseCount++;
         }

         RWS_RETURNVOID();
      }

      /**
      *
      *  \ingroup DebugTools
      *
      *  Printf the current RenderWare Graphics render state to the screen.
      *
      */
      void PrintfRenderState(void)
      {
         RWS_FUNCTION("RWS::DebugTools::PrintfRenderState");

         RwTexture *pTexture;

         RwRenderStateGet(rwRENDERSTATETEXTURERASTER, (void*)&pTexture);

         Printf("rwRENDERSTATETEXTURERASTER 0x%x\n", pTexture);

         RwBool flag;

         RwRenderStateGet(rwRENDERSTATETEXTUREPERSPECTIVE, (void*)&flag);

         Printf("rwRENDERSTATETEXTUREPERSPECTIVE %s\n", flag?"On":"Off");

         RwRenderStateGet(rwRENDERSTATEZTESTENABLE, (void*)&flag);

         Printf("rwRENDERSTATEZTESTENABLE %s\n", flag?"On":"Off");

         RwShadeMode mode;

         RwRenderStateGet(rwRENDERSTATESHADEMODE, (void*)&mode);

         Printf("rwRENDERSTATESHADEMODE 0x%x %s\n",
            mode, (mode == rwSHADEMODEFLAT)?"rwSHADEMODEFLAT":"rwSHADEMODEGOURAUD");

         RwRenderStateGet(rwRENDERSTATEZWRITEENABLE, (void*)&flag);

         Printf("rwRENDERSTATEZWRITEENABLE %s\n", flag?"On":"Off");
         
         RwBlendFunction blend;

         RwRenderStateGet(rwRENDERSTATESRCBLEND, (void*)&blend);

         Printf("rwRENDERSTATESRCBLEND %x\n", blend);

         RwRenderStateGet(rwRENDERSTATEDESTBLEND, (void*)&blend);

         Printf("rwRENDERSTATEDESTBLEND %x\n", blend);

         RwRenderStateGet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)&flag);

         Printf("rwRENDERSTATEVERTEXALPHAENABLE %s\n", flag?"On":"Off");

         RwRenderStateGet(rwRENDERSTATEFOGENABLE, (void*)&flag);

         Printf("rwRENDERSTATEFOGENABLE %s\n", flag?"On":"Off");

         RwRenderStateGet(rwRENDERSTATESTENCILENABLE, (void*)&flag);

         Printf("rwRENDERSTATESTENCILENABLE %s\n", flag?"On":"Off");

         RWS_RETURNVOID();
      }


     namespace {
        
         /*
         *
         *  \ingroup DebugTools
         *
         *  Based on the macros defined in rwcore.h and bamath.h,
         *  this function calculates sin and cos of x where -PI <= x <= PI. It has a performance gain over
         *  using sinf, cosf, RwCosMinusPiToPi, and RwSinMinusPiToPi. It also produces smaller code as it's
         *  a function not a macro.
         *
         */
         void RwSinCosMinusPiToPi(const RwReal &x, RwReal &s, RwReal &c)
         {                                                                 
            const RwReal z = x * x;                                        
            const RwReal v = z * x;   
            RwReal r;
         
            r = ( z * (_RW_C1  +                              
               z * (_RW_C2  +                         
               z * (_RW_C3  +                    
               z * (_RW_C4  +               
               z * (_RW_C5  +          
               z * _RW_C6))))));  

            c = (_RW_one - (0.5f * z - (z * r )));            
         
            r = ( _RW_S2 +                                  
               z * (_RW_S3 +                               
               z * (_RW_S4 +                          
               z * (_RW_S5 +                     
               z * _RW_S6))) );             
         
            s = x + v * (_RW_S1 + z * r);                            
         }                                                                 
      }


      /**
      *
      *  \ingroup DebugTools
      *
      *  Callback that draws a line from a light to the center of the bounding
      *  box of the sector it is linked to. The line has the same color as the light. 
      *
      *  \param light A pointer to an RpLight object, providing the position and color of the light.
      *  \param data A pointer to an RwBBox object, providing the dimensions of the sector containing the light.
      *
      */
      RpLight *ShowLightCB (RpLight * light, void *data)
      {
         RWS_FUNCTION("RWS::DebugTools::ShowLightCB");

         RwBBox *pRwBBox = static_cast<RwBBox*>(data);
      
         RwV3d pcenter;
      
         pcenter.x = (pRwBBox->sup.x + pRwBBox->inf.x) * 0.5f;
         pcenter.y = (pRwBBox->sup.y + pRwBBox->inf.y) * 0.5f;
         pcenter.z = (pRwBBox->sup.z + pRwBBox->inf.z) * 0.5f;
      
         RwFrame *frame = RpLightGetFrame(light);
      
         if (frame)
         {
            RwMatrix *matrix = RwFrameGetLTM(frame);
         
            if (matrix)
            {
               const RwRGBAReal *pcolor = RpLightGetColor(light);
               RwRGBA color;         

               RwRGBAFromRwRGBAReal(&color, pcolor);
            
               RwIm3DVertexSetPos(&(m_tLineVerts[0]), pcenter.x, pcenter.y, pcenter.z);
               RwIm3DVertexSetRGBA(&(m_tLineVerts[0]), color.red, color.green, color.blue, color.alpha);
               RwIm3DVertexSetPos(&(m_tLineVerts[1]), matrix->pos.x, matrix->pos.y, matrix->pos.z);
               RwIm3DVertexSetRGBA(&(m_tLineVerts[1]), color.red, color.green, color.blue, color.alpha);

               if (RwIm3DTransform (&m_tLineVerts[0], 2, 0, 0))
               {
                  RwIm3DRenderPrimitive (rwPRIMTYPELINELIST);
                  RwIm3DEnd ();
               }
            }
         }
      
         RWS_RETURN(light);
      }

      /**
      *
      *  \ingroup DebugTools
      *
      *  Callback that visualizes a world sector and draws a wire-frame box around the sector using RwIm3DVertex.
      *  Note that this can only be called from within a camera begin/end update.
      *
      *  \param worldSector A pointer to a world sector
      *
      *  \param data A pointer to an RwRGBA object. This is used to select the color to draw the bounding
      *  box of the sector.
      */
      RpWorldSector *ShowSectorCB(RpWorldSector *worldSector, void *data)
      {
         RWS_FUNCTION("RWS::DebugTools::ShowSectorCB");

         RWS_PRE(data);
         
         RwRGBA *color = static_cast<RwRGBA*>(data);

         const RwBBox *pRwBBox = RpWorldSectorGetBBox(worldSector);
      
         RwV3d p1, p2;

         RwUInt32 index = 0;

         // 
         //
         //
         p1.x = pRwBBox->sup.x;         p1.y = pRwBBox->sup.y;         p1.z = pRwBBox->sup.z;
         p2.x = pRwBBox->inf.x;         p2.y = pRwBBox->sup.y;         p2.z = pRwBBox->sup.z;
      
         RwIm3DVertexSetPos(&(m_tLineVerts[index]), p1.x, p1.y, p1.z);
         RwIm3DVertexSetRGBA(&(m_tLineVerts[index]), color->red, color->green, color->blue, color->alpha);
         index++;
         RwIm3DVertexSetPos(&(m_tLineVerts[index]), p2.x, p2.y, p2.z);
         RwIm3DVertexSetRGBA(&(m_tLineVerts[index]), color->red, color->green, color->blue, color->alpha);
         index++;
      
         p2.x = pRwBBox->sup.x;         p2.y = pRwBBox->inf.y;         p2.z = pRwBBox->sup.z;
      
         RwIm3DVertexSetPos(&(m_tLineVerts[index]), p1.x, p1.y, p1.z);
         RwIm3DVertexSetRGBA(&(m_tLineVerts[index]), color->red, color->green, color->blue, color->alpha);
         index++;
         RwIm3DVertexSetPos(&(m_tLineVerts[index]), p2.x, p2.y, p2.z);
         RwIm3DVertexSetRGBA(&(m_tLineVerts[index]), color->red, color->green, color->blue, color->alpha);
         index++;
      
         p2.x = pRwBBox->sup.x;         p2.y = pRwBBox->sup.y;         p2.z = pRwBBox->inf.z;
      
         RwIm3DVertexSetPos(&(m_tLineVerts[index]), p1.x, p1.y, p1.z);
         RwIm3DVertexSetRGBA(&(m_tLineVerts[index]), color->red, color->green, color->blue, color->alpha);
         index++;
         RwIm3DVertexSetPos(&(m_tLineVerts[index]), p2.x, p2.y, p2.z);
         RwIm3DVertexSetRGBA(&(m_tLineVerts[index]), color->red, color->green, color->blue, color->alpha);
         index++;
      
         //
         //
         //
         p1.x = pRwBBox->inf.x;         p1.y = pRwBBox->inf.y;         p1.z = pRwBBox->inf.z;
         p2.x = pRwBBox->sup.x;         p2.y = pRwBBox->inf.y;         p2.z = pRwBBox->inf.z;
      
         RwIm3DVertexSetPos(&(m_tLineVerts[index]), p1.x, p1.y, p1.z);
         RwIm3DVertexSetRGBA(&(m_tLineVerts[index]), color->red, color->green, color->blue, color->alpha);
         index++;
         RwIm3DVertexSetPos(&(m_tLineVerts[index]), p2.x, p2.y, p2.z);
         RwIm3DVertexSetRGBA(&(m_tLineVerts[index]), color->red, color->green, color->blue, color->alpha);
         index++;
      
         p2.x = pRwBBox->inf.x;         p2.y = pRwBBox->sup.y;         p2.z = pRwBBox->inf.z;
      
         RwIm3DVertexSetPos(&(m_tLineVerts[index]), p1.x, p1.y, p1.z);
         RwIm3DVertexSetRGBA(&(m_tLineVerts[index]), color->red, color->green, color->blue, color->alpha);
         index++;
         RwIm3DVertexSetPos(&(m_tLineVerts[index]), p2.x, p2.y, p2.z);
         RwIm3DVertexSetRGBA(&(m_tLineVerts[index]), color->red, color->green, color->blue, color->alpha);
         index++;
      
         p2.x = pRwBBox->inf.x;         p2.y = pRwBBox->inf.y;         p2.z = pRwBBox->sup.z;
      
         RwIm3DVertexSetPos(&(m_tLineVerts[index]), p1.x, p1.y, p1.z);
         RwIm3DVertexSetRGBA(&(m_tLineVerts[index]), color->red, color->green, color->blue, color->alpha);
         index++;
         RwIm3DVertexSetPos(&(m_tLineVerts[index]), p2.x, p2.y, p2.z);
         RwIm3DVertexSetRGBA(&(m_tLineVerts[index]), color->red, color->green, color->blue, color->alpha);
         index++;
      
         //
         //
         //
         p1.x = pRwBBox->sup.x;         p1.y = pRwBBox->inf.y;         p1.z = pRwBBox->inf.z;
         p2.x = pRwBBox->sup.x;         p2.y = pRwBBox->inf.y;         p2.z = pRwBBox->inf.z;
      
         RwIm3DVertexSetPos(&(m_tLineVerts[index]), p1.x, p1.y, p1.z);
         RwIm3DVertexSetRGBA(&(m_tLineVerts[index]), color->red, color->green, color->blue, color->alpha);
         index++;
         RwIm3DVertexSetPos(&(m_tLineVerts[index]), p2.x, p2.y, p2.z);
         RwIm3DVertexSetRGBA(&(m_tLineVerts[index]), color->red, color->green, color->blue, color->alpha);
         index++;
      
         p2.x = pRwBBox->sup.x;         p2.y = pRwBBox->sup.y;         p2.z = pRwBBox->inf.z;
      
         RwIm3DVertexSetPos(&(m_tLineVerts[index]), p1.x, p1.y, p1.z);
         RwIm3DVertexSetRGBA(&(m_tLineVerts[index]), color->red, color->green, color->blue, color->alpha);
         index++;
         RwIm3DVertexSetPos(&(m_tLineVerts[index]), p2.x, p2.y, p2.z);
         RwIm3DVertexSetRGBA(&(m_tLineVerts[index]), color->red, color->green, color->blue, color->alpha);
         index++;
      
         //
         //
         //
      
         p1.x = pRwBBox->inf.x;         p1.y = pRwBBox->inf.y;         p1.z = pRwBBox->sup.z;
         p2.x = pRwBBox->inf.x;         p2.y = pRwBBox->sup.y;         p2.z = pRwBBox->sup.z;
      
         RwIm3DVertexSetPos(&(m_tLineVerts[index]), p1.x, p1.y, p1.z);
         RwIm3DVertexSetRGBA(&(m_tLineVerts[index]), color->red, color->green, color->blue, color->alpha);
         index++;
         RwIm3DVertexSetPos(&(m_tLineVerts[index]), p2.x, p2.y, p2.z);
         RwIm3DVertexSetRGBA(&(m_tLineVerts[index]), color->red, color->green, color->blue, color->alpha);
         index++;
      
         p2.x = pRwBBox->sup.x;         p2.y = pRwBBox->inf.y;         p2.z = pRwBBox->sup.z;
      
         RwIm3DVertexSetPos(&(m_tLineVerts[index]), p1.x, p1.y, p1.z);
         RwIm3DVertexSetRGBA(&(m_tLineVerts[index]), color->red, color->green, color->blue, color->alpha);
         index++;
         RwIm3DVertexSetPos(&(m_tLineVerts[index]), p2.x, p2.y, p2.z);
         RwIm3DVertexSetRGBA(&(m_tLineVerts[index]), color->red, color->green, color->blue, color->alpha);
         index++;
      
         //
         //
         //
      
         p1.x = pRwBBox->inf.x;         p1.y = pRwBBox->sup.y;         p1.z = pRwBBox->inf.z;
         p2.x = pRwBBox->inf.x;         p2.y = pRwBBox->sup.y;         p2.z = pRwBBox->sup.z;
      
         RwIm3DVertexSetPos(&(m_tLineVerts[index]), p1.x, p1.y, p1.z);
         RwIm3DVertexSetRGBA(&(m_tLineVerts[index]), color->red, color->green, color->blue, color->alpha);
         index++;
         RwIm3DVertexSetPos(&(m_tLineVerts[index]), p2.x, p2.y, p2.z);
         RwIm3DVertexSetRGBA(&(m_tLineVerts[index]), color->red, color->green, color->blue, color->alpha);
         index++;
      
         p2.x = pRwBBox->sup.x;         p2.y = pRwBBox->sup.y;         p2.z = pRwBBox->inf.z;
      
         RwIm3DVertexSetPos(&(m_tLineVerts[index]), p1.x, p1.y, p1.z);
         RwIm3DVertexSetRGBA(&(m_tLineVerts[index]), color->red, color->green, color->blue, color->alpha);
         index++;
         RwIm3DVertexSetPos(&(m_tLineVerts[index]), p2.x, p2.y, p2.z);
         RwIm3DVertexSetRGBA(&(m_tLineVerts[index]), color->red, color->green, color->blue, color->alpha);
         index++;
      
         if (RwIm3DTransform (&m_tLineVerts[0], index, 0, 0))
         {
            RwIm3DRenderPrimitive (rwPRIMTYPELINELIST);
            RwIm3DEnd ();
         }

         RWS_RETURN(worldSector);
      }

      /*
      *
      *  \ingroup DebugTools
      *
      *  Runs through all the visible sectors (RwCameraForAllSectorsInFrustum), and calls ShowSector.
      *
      *  \param pCamera A pointer to an RwCamera object.
      *
      */
      void ShowAllSectors(RwCamera *pCamera)
      {
         RWS_FUNCTION("RWS::DebugTools::ShowAllSectors");

         if (DebugSwitches::GetSwitch(DebugSwitches::m_enable_displaysectors))
         {
            if (pCamera && RwCameraGetWorld(pCamera))
            {
               static RwRGBA color = {255, 255, 255,0};

               RwCameraForAllSectorsInFrustum(pCamera, ShowSectorCB, &color);
            }
         }

         RWS_RETURNVOID();
      }

      /**
      *
      *  \ingroup DebugTools
      *
      *  A callback used to run through all the lights in a sector, and call Show_Light.
      *
      *  \param worldSector A pointer to an RpWorldSector sector.
      *  \param data A pointer to additional data, unused.
      *
      */
      RpWorldSector *ShowLightsInSectorCB(RpWorldSector *worldSector, void *data)
      {
         RWS_FUNCTION("RWS::DebugTools::ShowLightsInSectorCB");

         const RwBBox *pRwBBox = RpWorldSectorGetBBox(worldSector);
      
         RpWorldSectorForAllLights(worldSector, ShowLightCB,(void*)(pRwBBox));

         RWS_RETURN(worldSector);
      }

      /*
      *
      *  \ingroup DebugTools
      *
      *  Runs through all the visible sectors, and calls Show_LightsInSector to display the lights attached
      *  to a specific sector.
      *
      *  \param pCamera A pointer to an RwCamera object.
      *
      */
      void ShowAllLights(RwCamera *pCamera)
      {
         RWS_FUNCTION("RWS::DebugTools::ShowAllLights");

         if (DebugSwitches::GetSwitch(DebugSwitches::m_enable_displaysectors))
         {
            if (pCamera && RwCameraGetWorld(pCamera))
            {
               RwCameraForAllSectorsInFrustum(pCamera, ShowLightsInSectorCB, 0);
            }
         }

         RWS_RETURNVOID();
      }

      /*
      *
      *  Renders the buffered lines. Note that this function can only be called between RwCameraBeginUpdate and RwCameraEndUpdate.
      *
      */
      void DisplayDebugTools::DrawLines(void)
      {
         RWS_FUNCTION("RWS::DebugTools::DrawLines");

         // Draw Lines
         //
         if (m_LineCount)
         {
            for (unsigned int nI = 0; nI < m_LineCount; nI++)
            {
               RwIm3DVertexSetPos (&(m_tLineVerts[nI * 2]),
                  m_LineStart[nI].x,
                  m_LineStart[nI].y,
                  m_LineStart[nI].z);
               RwIm3DVertexSetRGBA (&(m_tLineVerts[nI * 2]),
                  m_LineCol[nI].red,
                  m_LineCol[nI].green,
                  m_LineCol[nI].blue,
                  128);

               RwIm3DVertexSetPos (&(m_tLineVerts[nI * 2 + 1]),
                  m_LineEnd[nI].x,
                  m_LineEnd[nI].y,
                  m_LineEnd[nI].z);
               RwIm3DVertexSetRGBA (&(m_tLineVerts[nI * 2 + 1]),
                  m_LineCol[nI].red,
                  m_LineCol[nI].green,
                  m_LineCol[nI].blue,
                  128);
            }

            if (RwIm3DTransform (&m_tLineVerts[0], m_LineCount*2, 0, 0))
            {
               RwIm3DRenderPrimitive (rwPRIMTYPELINELIST);
               RwIm3DEnd ();
            }
         }

         RWS_RETURNVOID();
      }

      /*
      *
      *  Renders the buffered ellipses. Note that this function can only be called between RwCameraBeginUpdate and RwCameraEndUpdate.
      *
      */
      void DisplayDebugTools::DrawEllipses(void)
      {
         RWS_FUNCTION("RWS::DebugTools::DrawEllipses");

         const RwUInt32 num_ELLIPSE_NUM_SEGMENTS = 16;
         const RwReal ELLIPSE_NUM_SEGMENTS = (RwReal)num_ELLIPSE_NUM_SEGMENTS;
         
         // Draw Ellipse
         //
         static RwImVertexIndex indicesT[num_ELLIPSE_NUM_SEGMENTS*num_ELLIPSE_NUM_SEGMENTS*3*2];
         
         for (unsigned int nE = 0; nE < m_EllipseCount;nE++)
         {
            RwReal alpha, beta, alpha2, beta2;
            RwUInt32 i, j;
            unsigned int nI = 0;
            
            // Generate Indices
            //
            for (i = 0;i < num_ELLIPSE_NUM_SEGMENTS * num_ELLIPSE_NUM_SEGMENTS * 2;i++)
            {
               indicesT[i * 3] = (RwImVertexIndex)(i * 3);
               indicesT[i * 3 + 1] = (RwImVertexIndex)(i * 3 + 1);
               indicesT[i * 3 + 2] = (RwImVertexIndex)(i * 3 + 2);
            }
            
            // Calculate vertices
            //
            for (i = 0, alpha = 0, beta = (2.0f * rwPI / ELLIPSE_NUM_SEGMENTS);
                 i < ELLIPSE_NUM_SEGMENTS;
                 i++,
                 alpha += (2.0f * rwPI / ELLIPSE_NUM_SEGMENTS),
                 beta += (2.0f * rwPI / ELLIPSE_NUM_SEGMENTS))
            {
               RwReal x1, y1, z1;
               RwReal x2, y2, z2;
               RwReal x3, y3, z3;
               RwReal x4, y4, z4;
               
               RwReal sinalpha, cosalpha;
               RwReal sinbeta, cosbeta;
               
               RwSinCosMinusPiToPi(alpha, sinalpha, cosalpha);
               RwSinCosMinusPiToPi(beta, sinbeta, cosbeta);
               
               for ( j = 0,
                     alpha2 = 0,
                     beta2 = (2.0f * rwPI / ELLIPSE_NUM_SEGMENTS);
                     j < ELLIPSE_NUM_SEGMENTS;
                     j++,
                     alpha2 += (2.0f * rwPI / ELLIPSE_NUM_SEGMENTS),
                     beta2 += (2.0f * rwPI / ELLIPSE_NUM_SEGMENTS))
               {
                  RwReal sinalpha2, cosalpha2;
                  RwReal sinbeta2, cosbeta2;
                  
                  RwSinCosMinusPiToPi(alpha2, sinalpha2, cosalpha2);
                  RwSinCosMinusPiToPi(beta2, sinbeta2, cosbeta2);
                  
                  x1 = m_EllipseCenter[nE].x + m_EllipseRadii[nE].x * (sinalpha * sinalpha2);
                  y1 = m_EllipseCenter[nE].y + m_EllipseRadii[nE].y * (cosalpha * sinalpha2);
                  z1 = m_EllipseCenter[nE].z + m_EllipseRadii[nE].z * (cosalpha2);
                  
                  x2 = m_EllipseCenter[nE].x + m_EllipseRadii[nE].x * (sinbeta * sinalpha2);
                  y2 = m_EllipseCenter[nE].y + m_EllipseRadii[nE].y * (cosbeta * sinalpha2);
                  z2 = m_EllipseCenter[nE].z + m_EllipseRadii[nE].z * (cosalpha2);
                  
                  x3 = m_EllipseCenter[nE].x + m_EllipseRadii[nE].x * (sinalpha * sinbeta2);
                  y3 = m_EllipseCenter[nE].y + m_EllipseRadii[nE].y * (cosalpha * sinbeta2);
                  z3 = m_EllipseCenter[nE].z + m_EllipseRadii[nE].z * (cosbeta2);
                  
                  x4 = m_EllipseCenter[nE].x + m_EllipseRadii[nE].x * (sinbeta * sinbeta2);
                  y4 = m_EllipseCenter[nE].y + m_EllipseRadii[nE].y * (cosbeta * sinbeta2);
                  z4 = m_EllipseCenter[nE].z + m_EllipseRadii[nE].z * (cosbeta2);
                  // 1 2
                  // 3            
                  RwIm3DVertexSetPos(&(m_tLineVerts[nI]), x1, y1, z1);
                  RwIm3DVertexSetRGBA(&(m_tLineVerts[nI]),
                     m_EllipseCol[nE].red,
                     m_EllipseCol[nE].green,
                     m_EllipseCol[nE].blue, 128);
                  
                  nI += 1;
                  
                  RwIm3DVertexSetPos(&(m_tLineVerts[nI]), x2, y2, z2);
                  RwIm3DVertexSetRGBA(&(m_tLineVerts[nI]),
                     m_EllipseCol[nE].red,
                     m_EllipseCol[nE].green,
                     m_EllipseCol[nE].blue, 128);
                  
                  nI += 1;
                  
                  RwIm3DVertexSetPos(&(m_tLineVerts[nI]), x3, y3, z3);
                  RwIm3DVertexSetRGBA(&(m_tLineVerts[nI]),
                     m_EllipseCol[nE].red,
                     m_EllipseCol[nE].green,
                     m_EllipseCol[nE].blue, 128);
                  
                  nI += 1;
                  
                  //   2
                  // 3 4          
                  RwIm3DVertexSetPos(&(m_tLineVerts[nI]), x2, y2, z2);
                  RwIm3DVertexSetRGBA(&(m_tLineVerts[nI]),
                     m_EllipseCol[nE].red,
                     m_EllipseCol[nE].green,
                     m_EllipseCol[nE].blue, 128);
                  
                  nI += 1;
                  
                  RwIm3DVertexSetPos(&(m_tLineVerts[nI]), x4, y4, z4);
                  RwIm3DVertexSetRGBA(&(m_tLineVerts[nI]),
                     m_EllipseCol[nE].red,
                     m_EllipseCol[nE].green,
                     m_EllipseCol[nE].blue, 128);
                  
                  nI += 1;
                  
                  RwIm3DVertexSetPos (&(m_tLineVerts[nI]), x3, y3, z3);
                  RwIm3DVertexSetRGBA (&(m_tLineVerts[nI]),
                     m_EllipseCol[nE].red,
                     m_EllipseCol[nE].green,
                     m_EllipseCol[nE].blue, 128);
                  
                  nI += 1;
                  
               }
            }
            
            if (RwIm3DTransform(&m_tLineVerts[0], nI, 0, 0))
            {
               RwIm3DRenderIndexedPrimitive (rwPRIMTYPETRILIST, 
                  indicesT,
                  (num_ELLIPSE_NUM_SEGMENTS * num_ELLIPSE_NUM_SEGMENTS) * 3 * 2);
               
               RwIm3DEnd ();
            }
         }

         RWS_RETURNVOID();
      }

      /*
      *
      *  Renders the buffered polygons. Note that this function can only be called between RwCameraBeginUpdate and RwCameraEndUpdate.
      *
      */    
      void DisplayDebugTools::DrawPolys(void)
      {
         RWS_FUNCTION("RWS::DisplayDebugTools::DrawPolys");

         // Draw Polys
         //
         for (unsigned int nP = 0;nP < m_PolyCount;nP++)
         {
            static RwImVertexIndex indicesT[] = 
            {
               0, 1, 2
            };
            
            unsigned int nI = 0;
            
            RwIm3DVertexSetPos(&(m_tLineVerts[nI]), m_PolyV1[nP].x, m_PolyV1[nP].y, m_PolyV1[nP].z);
            RwIm3DVertexSetRGBA(&(m_tLineVerts[nI]), m_PolyCol[nP].red, m_PolyCol[nP].green, m_PolyCol[nP].blue, 128);
            
            nI += 1;
            
            RwIm3DVertexSetPos(&(m_tLineVerts[nI]), m_PolyV2[nP].x, m_PolyV2[nP].y, m_PolyV2[nP].z);
            RwIm3DVertexSetRGBA(&(m_tLineVerts[nI]), m_PolyCol[nP].red, m_PolyCol[nP].green, m_PolyCol[nP].blue, 128);
            
            nI += 1;
            
            RwIm3DVertexSetPos(&(m_tLineVerts[nI]), m_PolyV3[nP].x, m_PolyV3[nP].y, m_PolyV3[nP].z);
            RwIm3DVertexSetRGBA (&(m_tLineVerts[nI]), m_PolyCol[nP].red, m_PolyCol[nP].green, m_PolyCol[nP].blue, 128);
            
            nI += 1;
            
            if (RwIm3DTransform (&m_tLineVerts[0], nI, 0, 0))
            {
               RwIm3DRenderIndexedPrimitive (rwPRIMTYPETRILIST, indicesT, 3);
               RwIm3DEnd();
            }
         }
         RWS_RETURNVOID();
      }

      /*
      *
      *  Renders the buffered 3D text. Note that this function can only be called between RwCameraBeginUpdate and RwCameraEndUpdate.
      *
      *  \param pCamera A pointer to an RwCamera object used to transform the position of the 3D text.
      *
      */    
      void DisplayDebugTools::Draw3DText(RwCamera *pCamera)
      {
         RWS_FUNCTION("RWS::DisplayDebugTools::Draw3DText");

         /* Loop over any text strings to draw. Work out screen space from world coordinates and draw it. */
         if (m_TextCount)
         {
            RwUInt32 i;
            
            if (pCamera)
            {
               /* Get camera's projection matrix */
               RwMatrix *camMatrix = 0;
               camMatrix = RwCameraGetViewMatrix (pCamera);
               
               /* Get near and far clip planes */
               RwReal nearClip, farClip;
               nearClip = RwCameraGetNearClipPlane (pCamera);
               farClip = RwCameraGetFarClipPlane (pCamera);
               
               /* Get screen raster size */
               RwRaster *frameBuffer = RwCameraGetRaster (pCamera);
               RwInt32 width, height;
               width = RwRasterGetWidth (frameBuffer);
               height = RwRasterGetHeight (frameBuffer);
               
               /* Apply projection matrix to array of points */
               RwV3dTransformPoints (m_tTextPos, m_TextPos, m_TextCount, camMatrix);
               
               for (i = 0; i < m_TextCount; i++)
               {
                  if ( (m_tTextPos[i].z > nearClip) && (m_tTextPos[i].z < farClip) )
                  {
                     RwReal recipZ;
                     
                     /* Put into screen space */
                     recipZ = (RwReal)(1.0) / m_tTextPos[i].z;
                     m_tTextPos[i].x *= (recipZ * width);
                     m_tTextPos[i].y *= (recipZ * height);
                     m_tTextPos[i].x += (RwReal)(frameBuffer->nOffsetX);
                     m_tTextPos[i].y += (RwReal)(frameBuffer->nOffsetY);
                     
                     if(m_tTextPos[i].x > 0 && m_tTextPos[i].x < width)
                     {
                        if(m_tTextPos[i].y > 0 && m_tTextPos[i].y < height)
                        {
                           PrintfXY((RwUInt32)m_tTextPos[i].x, (RwUInt32)m_tTextPos[i].y, m_TextString[i]);
                        }
                     }
                  }
               }
            }
         }
         RWS_RETURNVOID();
      }

      /**
      *
      *  Saves the current render state.
      *
      */
      void DisplayDebugTools::SaveRenderState()
      {
         RWS_FUNCTION("RWS::DisplayDebugTools::SaveRenderState");

         RwRenderStateGet(rwRENDERSTATESRCBLEND, &_rwRENDERSTATESRCBLEND);
         RwRenderStateGet(rwRENDERSTATEDESTBLEND, &_rwRENDERSTATEDESTBLEND);
         RwRenderStateGet(rwRENDERSTATEVERTEXALPHAENABLE, &_rwRENDERSTATEVERTEXALPHAENABLE);
         RwRenderStateGet(rwRENDERSTATEZWRITEENABLE, &_rwRENDERSTATEZWRITEENABLE);
         RwRenderStateGet(rwRENDERSTATEZTESTENABLE, &_rwRENDERSTATEZTESTENABLE);
         RwRenderStateGet(rwRENDERSTATECULLMODE, &_rwRENDERSTATECULLMODE);
         RwRenderStateGet(rwRENDERSTATEFOGENABLE, &_rwRENDERSTATEFOGENABLE);
         RwRenderStateGet(rwRENDERSTATETEXTURERASTER, &_rwRENDERSTATETEXTURERASTER);
         
         RWS_RETURNVOID();
      }
      
      /**
      *
      *  Selects the render state for displaying debug information.
      *
      */
      void DisplayDebugTools::SetRenderState()
      {
         RWS_FUNCTION("RWS::DisplayDebugTools::SetRenderState");

         RwRenderStateSet(rwRENDERSTATESRCBLEND, (void *)m_rwRENDERSTATESRCBLEND);
         RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void *)m_rwRENDERSTATEDESTBLEND);
         RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *)m_rwRENDERSTATEVERTEXALPHAENABLE);
         RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)m_rwRENDERSTATEZWRITEENABLE);
         RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)m_rwRENDERSTATEZTESTENABLE);
         RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)m_rwRENDERSTATECULLMODE);
         RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void *)FALSE);
         RwRenderStateSet(rwRENDERSTATETEXTURERASTER, (void *)0);    

         RWS_RETURNVOID();
      }
      
      /**
      *
      *  Restores the last saved render state.
      *
      */
      void DisplayDebugTools::RestoreRenderState()
      {
         RWS_FUNCTION("RWS::DisplayDebugTools::RestoreRenderState");

         RwRenderStateSet(rwRENDERSTATESRCBLEND, (void *)_rwRENDERSTATESRCBLEND);
         RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void *)_rwRENDERSTATEDESTBLEND);
         RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *)_rwRENDERSTATEVERTEXALPHAENABLE);
         RwRenderStateSet(rwRENDERSTATEZWRITEENABLE,(void*)_rwRENDERSTATEZWRITEENABLE);
         RwRenderStateSet(rwRENDERSTATEZTESTENABLE,(void*)_rwRENDERSTATEZTESTENABLE);
         RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)_rwRENDERSTATECULLMODE);
         RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*)_rwRENDERSTATEFOGENABLE);
         RwRenderStateSet(rwRENDERSTATETEXTURERASTER, (void *)_rwRENDERSTATETEXTURERASTER);    

         RWS_RETURNVOID();
      }


      /**
      *
      *  Renders the buffered text, lines, and triangles.
      *
      *  \param pCamera A pointer to an RwCamera object.
      */
      void DisplayDebugTools::Poll(RwCamera *pCamera)
      {
         RwMetrics *pRwMetrics = RwEngineGetMetrics ();

         if (pRwMetrics)
         {
            PrintfMetrics(pRwMetrics);
            PrintfSkyMetrics(pRwMetrics);
            TraceMetrics(pRwMetrics);
         }

         if (pCamera)
         {
            // Matrix flags are sometimes discarded, RwCameraBeginUpdate will assert if these are not set
            //
            rwMatrixSetFlags(RwFrameGetLTM(RwCameraGetFrame(pCamera)), rwMATRIXTYPENORMAL | rwMATRIXTYPEORTHOGONAL);

            if (RwCameraBeginUpdate (pCamera))
            {
               SaveRenderState();
               SetRenderState();

               ShowAllSectors(pCamera);
               ShowAllLights(pCamera);
               DrawLines();
               DrawEllipses();
               DrawPolys();    
               Draw3DText(pCamera);

               DumpPrintf();
               RtCharsetBufferFlush();

               RestoreRenderState();

               RwCameraEndUpdate (pCamera);
            }
         }
      }

      DisplayDebugTools *DisplayDebugTools::pDisplayDebugToolsInstance = 0;

      /**
      *
      *  \ingroup DebugTools
      *
      *  Constructs a DisplayDebugTools object.
      *
      */
      DisplayDebugTools::DisplayDebugTools(void) : InitCEventHandler(0),
         m_link_priority(0),
         m_rwRENDERSTATECULLMODE(rwCULLMODECULLNONE),
         m_rwRENDERSTATESRCBLEND(rwBLENDSRCALPHA),
         m_rwRENDERSTATEDESTBLEND(rwBLENDINVSRCALPHA),
         m_rwRENDERSTATEVERTEXALPHAENABLE(TRUE),
         m_rwRENDERSTATEZWRITEENABLE(FALSE),
         m_rwRENDERSTATEZTESTENABLE(TRUE)
      {
         RWS_FUNCTION("RWS::DisplayDebugTools::DisplayDebugTools");

         RWS_ASSERT(pDisplayDebugToolsInstance == 0, "Can only create 1 instance of DisplayDebugTools at a time");

         pDisplayDebugToolsInstance = this;

         DebugTools::Open();
         DebugTools::Reset();

         LinkMsg(iMsgRunningPreTick, 0, 0xffff); // Highest Priority
         LinkMsg(iMsgRunningPostTick, 0, 0);     // Lowest Priority

         m_StartRender = iMsgDoRender;

         LinkMsg(m_StartRender, "RwCamera*", m_link_priority);  // Lowest Priority

         LinkMsg(iMsgSetRunningMode, 0);
         LinkMsg(iMsgSetFrozenMode, 0);

         RWS_RETURNVOID();
      }

      /**
      *
      *  Destroys a DisplayDebugTools object.
      *
      */
      DisplayDebugTools::~DisplayDebugTools(void)
      {
         RWS_FUNCTION("RWS::DisplayDebugTools::~DisplayDebugTools");

         pDisplayDebugToolsInstance = 0;

         UnLinkMsg(iMsgRunningPreTick);
         UnLinkMsg(iMsgRunningPostTick);
         UnLinkMsg(iMsgSetRunningMode);
         UnLinkMsg(iMsgSetFrozenMode);
         UnLinkMsg(m_StartRender);

         UnRegisterMsg(m_StartRender);
         DebugTools::Close();

         RWS_RETURNVOID();
      }

      /**
      *
      *  Sets Render event. Specifies the event DisplayDebugTools is linked to, to start
      *  rendering the buffered debug data.
      *
      *  \param _StartRender A string containing the name of the event, \see CEventHandler::RegisterMsg
      *
      */
      void DisplayDebugTools::SetStartRenderEvent(const RwChar *_StartRender)
      {
         RWS_FUNCTION("RWS::DisplayDebugTools::SetStartRenderEvent");

         UnLinkMsg(m_StartRender);
         UnRegisterMsg(m_StartRender);
         RegisterMsg(m_StartRender, _StartRender, "RwCamera*");
         LinkMsg(m_StartRender, "RwCamera*", m_link_priority);

         RWS_RETURNVOID();
      }

      /**
      *
      *  Sets Render priority. Specifies the priority with which DisplayDebugTools is linked to the 
      *  render event.
      *
      *  \param _link_priority The link priority, \see CEventHandler::LinkMsg
      *
      */
      void DisplayDebugTools::SetRenderPriority(RwUInt16 _link_priority)
      {
         RWS_FUNCTION("RWS::DisplayDebugTools::SetRenderPriority");

         UnLinkMsg(m_StartRender);
         LinkMsg(m_StartRender, "RwCamera*", m_link_priority);

         RWS_RETURNVOID();
      }

      /**
      *
      *  Handles events, waits for iMsgDoRender event, and calls DebugTools::Poll
      *  to render the accumulated debug information.
      *
      */
      void DisplayDebugTools::HandleEvents(CMsg &pMsg)
      {
         // Set paused mode event
         //
         if (pMsg.Id == iMsgSetFrozenMode) 
         {
            DebugSwitches::SetSwitch(DebugSwitches::m_req_enabled, false);   // Disable updates
         }
         // Set running mode event
         //
         else if (pMsg.Id == iMsgSetRunningMode)  
         {
            DebugSwitches::SetSwitch(DebugSwitches::m_req_enabled, true);   // Enable updates
         }
         // Pre running event,
         //
         else if (pMsg.Id == iMsgRunningPreTick) 
         {
            Reset();
      
            m_fcounter += 1.0f / 120.0f;
      
            if (m_fcounter > 1.0f) m_fcounter -= 1.0f;
         
            PreTick_Time = Time::GetTime();
         }
         // Post running event,
         //
         else if (pMsg.Id == iMsgRunningPostTick)
         {
            PostTick_Time = Time::GetTime();
         }
         // Post render event,
         //
         else if (pMsg.Id == m_StartRender) 
         {
            RwCamera *pRwCamera = reinterpret_cast<RwCamera*>(pMsg.pData);
      
            if (pRwCamera)
            {
               Poll(pRwCamera);
            }

            // Pause/Unpause has been selected enable/disable updates to debug info
            //
            DebugSwitches::SetSwitch(DebugSwitches::m_enabled, DebugSwitches::GetSwitch(DebugSwitches::m_req_enabled));
         }
      }
      
      /**
      *
      *  \ingroup DebugTools
      *
      *  Draws a wireframe representation of a sphere. Uses DebugTools::Line for the actual drawing.
      *  The sphere has a radius of 0.5, with its centre at (0,0,0). pWorldMatrix is used to position,
      *  orientate and scale it in world space.
      *
      *  \param pWorldMatrix Matrix to transform the sphere into world space.
      *  \param Red Red component of the color.
      *  \param Green Green component of the color.
      *  \param Blue Blue component of the color.
      *
      */
      void WireSphere( const RwMatrix * pWorldMatrix, RwUInt8 Red, RwUInt8 Green, RwUInt8 Blue )
      {
         RWS_FUNCTION("DebugTools::WireSphere");

         static const RwUInt32 STACK_NUM = 6;
         static const RwUInt32 SLICE_NUM = 12;
         static const RwUInt32 HORIZONTAL_LINE_NUM = ( STACK_NUM * SLICE_NUM );
         static const RwUInt32 VERTICAL_LINE_NUM = ( ( STACK_NUM + 1 ) * SLICE_NUM );
         static const RwUInt32 TOTAL_LINE_NUM = ( HORIZONTAL_LINE_NUM + VERTICAL_LINE_NUM );

         static RwBool bSphereGenerated = FALSE;
         static RwLine UnitSphere[TOTAL_LINE_NUM];
         RwLine Sphere[TOTAL_LINE_NUM];

         // the first time around, generate a unit sphere
         if ( !bSphereGenerated )
         {
            RwUInt32 Index = 0;
            RwUInt32 Stack = 0;
            RwUInt32 Slice = 0;

            // Horizontal lines (depends on number of stacks)
            RwReal AzimuthDelta = ( ( 2.0f * rwPI ) / SLICE_NUM );
            RwReal ElevationDelta = ( rwPI / ( STACK_NUM + 1 ) );
   
            RwReal Elevation = ( - rwPI / 2.0f ) + ElevationDelta;   // don't need to draw a horizontal line for the pole
            RwReal Azimuth;
            RwReal Radius = 0.5f; // unit sphere, diameter = 1.0f, radius = 0.5f

            for (Stack = 0; Stack < STACK_NUM; Stack++)
            {
               Azimuth = 0.0f;
            
               for (Slice = 0; Slice < SLICE_NUM; Slice++)
               {
                  UnitSphere[Index].start.x = Radius * RwCos(Elevation) * RwSin(Azimuth);
                  UnitSphere[Index].start.y = Radius * RwSin(Elevation);
                  UnitSphere[Index].start.z = Radius * RwCos(Elevation) * RwCos(Azimuth);
                  UnitSphere[Index].end.x = Radius * RwCos(Elevation) * RwSin(Azimuth + AzimuthDelta);
                  UnitSphere[Index].end.y = Radius * RwSin(Elevation);
                  UnitSphere[Index].end.z = Radius * RwCos(Elevation) * RwCos(Azimuth + AzimuthDelta);

                  ++Index;
                  Azimuth += AzimuthDelta;
               }

               Elevation += ElevationDelta;
            }

            // Vertical lines
            Azimuth = 0.0f;
            for ( Slice = 0; Slice < SLICE_NUM; Slice++ )
            {
               Elevation = ( -rwPI / 2.0f );

               for ( Stack = 0; Stack < (STACK_NUM + 1); Stack++ )
               {
                  UnitSphere[Index].start.x = Radius * RwCos(Elevation) * RwSin( Azimuth );
                  UnitSphere[Index].start.y = Radius * RwSin(Elevation);
                  UnitSphere[Index].start.z = Radius * RwCos(Elevation) * RwCos( Azimuth );
                  UnitSphere[Index].end.x = Radius * RwCos(Elevation + ElevationDelta) * RwSin(Azimuth);
                  UnitSphere[Index].end.y = Radius * RwSin(Elevation + ElevationDelta);
                  UnitSphere[Index].end.z = Radius * RwCos(Elevation + ElevationDelta) * RwCos(Azimuth);

                  ++Index;
                  Elevation += ElevationDelta;
               }

               Azimuth += AzimuthDelta;
            }

            bSphereGenerated = TRUE;
         }

         // transform into world space
         RwV3dTransformPoints(reinterpret_cast<RwV3d*>(&Sphere), 
                              reinterpret_cast<RwV3d*>(&UnitSphere), 
                              ( 2 * TOTAL_LINE_NUM ), 
                              pWorldMatrix );

         // draw the lines
         for (RwUInt32 Index = 0; Index < TOTAL_LINE_NUM; Index++)
         {
            DebugTools::Line( &Sphere[Index].start, &Sphere[Index].end, Red, Green, Blue );
         }

         RWS_RETURNVOID();
      }

      /**
      *
      *  \ingroup DebugTools
      *
      *  Draws a wireframe representation of a box. Uses DebugTools::Line for the actual drawing.
      *  The box has sides of length 1.0, with its centre at (0,0,0). pWorldMatrix is used to position,
      *  orientate and scale it in world space.
      *
      *  \param pWorldMatrix Matrix to transform the box into world space.
      *  \param Red Red component of the color.
      *  \param Green Green component of the color.
      *  \param Blue Blue component of the color.
      *
      */
      void WireBox( const RwMatrix * pWorldMatrix, RwUInt8 Red, RwUInt8 Green, RwUInt8 Blue )
      {
         RWS_FUNCTION("DebugTools::WireBox");

         static const RwUInt32 LINE_NUM = 12;
         static RwLine UnitBox[LINE_NUM] = 
         {
            { {0.5f, 0.5f, 0.5f},      {0.5f, 0.5f, -0.5f} },
            { {0.5f, 0.5f, -0.5f},     {-0.5f, 0.5f, -0.5f} },
            { {-0.5f, 0.5f, -0.5f},    {-0.5f, 0.5f, 0.5f} },
            { {-0.5f, 0.5f, 0.5f},     {0.5f, 0.5f, 0.5f} },
            { {0.5f, -0.5f, 0.5f},     {0.5f, -0.5f, -0.5f} },
            { {0.5f, -0.5f, -0.5f},    {-0.5f, -0.5f, -0.5f} },
            { {-0.5f, -0.5f, -0.5f},   {-0.5f, -0.5f, 0.5f} },
            { {-0.5f, -0.5f, 0.5f},    {0.5f, -0.5f, 0.5f} },
            { {0.5f, 0.5f, 0.5f},      {0.5f, -0.5f, 0.5f} },
            { {0.5f, 0.5f, -0.5f},     {0.5f, -0.5f, -0.5f} },
            { {-0.5f, 0.5f, -0.5f},    {-0.5f, -0.5f, -0.5f} },
            { {-0.5f, 0.5f, 0.5f},     {-0.5f, -0.5f, 0.5f} }
         };
         RwLine Box[LINE_NUM];

         // transform into world space
         RwV3dTransformPoints( reinterpret_cast<RwV3d*>(&Box), 
                               reinterpret_cast<RwV3d*>(&UnitBox), 
                               ( 2 * LINE_NUM ), 
                               pWorldMatrix );

         for (RwUInt32 Index = 0; Index < LINE_NUM; ++Index)
         {
            DebugTools::Line(&Box[Index].start, &Box[Index].end, Red, Green, Blue);
         }

         RWS_RETURNVOID();
      }

      /**
      *
      *  \ingroup DebugTools
      *
      *  Draws a wireframe representation of a cylinder. Uses DebugTools::Line for the actual drawing.
      *  The cylinder has a radius of 0.5, a height of 1.0 and has its centre at (0,0,0). 
      *  pWorldMatrix is used to position, orientate and scale it in world space.
      *
      *  \param pWorldMatrix Matrix to transform the cylinder into world space.
      *  \param Red Red component of the color.
      *  \param Green Green component of the color.
      *  \param Blue Blue component of the color.
      *
      */
      void WireCylinder( const RwMatrix * pWorldMatrix, RwUInt8 Red, RwUInt8 Green, RwUInt8 Blue )
      {
         RWS_FUNCTION("DebugTools::WireCylinder");

         static const RwUInt32 STACK_NUM = 5;
         static const RwUInt32 SLICE_NUM = 16;
         static const RwUInt32 HORIZONTAL_LINE_NUM = ( ( STACK_NUM + 1 ) * SLICE_NUM );
         static const RwUInt32 VERTICAL_LINE_NUM = ( STACK_NUM * SLICE_NUM );
         static const RwUInt32 TOTAL_LINE_NUM = ( HORIZONTAL_LINE_NUM + VERTICAL_LINE_NUM );

         static RwBool bCylinderGenerated = FALSE;
         static RwLine UnitCylinder[TOTAL_LINE_NUM];
         RwLine Cylinder[TOTAL_LINE_NUM];

         // the first time around, generate a unit sphere
         if (!bCylinderGenerated)
         {
            RwUInt32 Index = 0;
            RwUInt32 Stack = 0;
            RwUInt32 Slice = 0;

            // Horizontal lines (depends on number of stacks)
            RwReal AzimuthDelta = ( ( 2.0f * rwPI ) / SLICE_NUM );
            RwReal Azimuth;
            RwReal HeightDelta = 1.0f / ( STACK_NUM );
            RwReal Height = -0.5f;
            RwReal Radius = 0.5f; // unit cylinder, radius = 0.5f, height = 1.0f

            for (Stack = 0; Stack < ( STACK_NUM + 1 ); Stack++)
            {
               Azimuth = 0.0f;
            
               for (Slice = 0; Slice < SLICE_NUM; Slice++)
               {
                  UnitCylinder[Index].start.x = Radius * RwSin(Azimuth);
                  UnitCylinder[Index].start.y = Height;
                  UnitCylinder[Index].start.z = Radius * RwCos(Azimuth);
                  UnitCylinder[Index].end.x = Radius * RwSin(Azimuth + AzimuthDelta);
                  UnitCylinder[Index].end.y = Height;
                  UnitCylinder[Index].end.z = Radius * RwCos(Azimuth + AzimuthDelta);

                  ++Index;
                  Azimuth += AzimuthDelta;
               }

               Height += HeightDelta;
            }

            // Vertical lines
            Azimuth = 0.0f;
            for (Slice = 0; Slice < SLICE_NUM; Slice++)
            {
               Height = -0.5f;

               for (Stack = 0; Stack < STACK_NUM; Stack++)
               {
                  UnitCylinder[Index].start.x = Radius * RwSin(Azimuth);
                  UnitCylinder[Index].start.y = Height;
                  UnitCylinder[Index].start.z = Radius * RwCos(Azimuth);
                  UnitCylinder[Index].end.x = Radius * RwSin(Azimuth);
                  UnitCylinder[Index].end.y = Height + HeightDelta;
                  UnitCylinder[Index].end.z = Radius * RwCos(Azimuth);

                  ++Index;
                  Height += HeightDelta;
               }

               Azimuth += AzimuthDelta;
            }

            bCylinderGenerated = TRUE;
         }

         // transform into world space
         RwV3dTransformPoints(reinterpret_cast<RwV3d*>(&Cylinder), 
                              reinterpret_cast<RwV3d*>(&UnitCylinder), 
                              ( 2 * TOTAL_LINE_NUM ), 
                              pWorldMatrix);

         // draw the lines
         for (RwUInt32 Index = 0; Index < TOTAL_LINE_NUM; Index++)
         {
            DebugTools::Line(&Cylinder[Index].start, &Cylinder[Index].end, Red, Green, Blue);
         }

         RWS_RETURNVOID();
      }
      
   }// namespace DebugTools 
}//namespace RWS

#endif
