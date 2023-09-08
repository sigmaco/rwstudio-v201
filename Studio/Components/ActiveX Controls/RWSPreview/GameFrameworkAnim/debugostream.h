/*****************************************************************************
 *
 * File :     DebugOstream.h
 *
 * Abstract : An ostream implementation for debug output (e.g. under Win32
 *            output goes to OutputDebugString)
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

#ifndef __DEBUGOSTREAM_H__
#define __DEBUGOSTREAM_H__

#ifndef NDEBUG

#if defined (SKY)

// Force data within sdata,sbss to be in data,bss section.
//
#pragma push
#pragma define_section sdata ".data" far_absolute RW
#pragma define_section sbss ".bss" far_absolute RW
#include <iostream>
#pragma pop

#elif defined _MSC_VER | defined (DOLPHIN)

#include <iostream>

#endif

namespace RWS
{
   /**
   *
   *  \ingroup DebugMacros
   *
   *  Custom implementation of the stl std::streambuf used by RenderWare Studio
   *  for outputting debug messages. Has two main features:
   *  \li Uses a static buffer to store the characters. If the end of the 
   *      buffer is reached, it is flushed and data is put into the buffer 
   *      from the start.
   *  \li Can echo the output to the RenderWare Studio Workspace, but this
   *      depends on the RWS::NetStream module being intialised. Hence it can
   *      enabled / disabled as needed (default is disabled). Use the macros
   *      DEBUGSTREAM_NETWORK_ENABLE / DEBUGSTREAM_NETWORK_DISABLE to control
   *      this feature.
   *
   */
   class debug_buf : public std::streambuf
   {
   public:
      
      typedef unsigned int OutputFlags;
      static const OutputFlags standard_debug_output;
      static const OutputFlags rws_workspace_output;

   private:

      char m_buffer[1024];
      OutputFlags m_OutputFlags;

   public:
   
      debug_buf();
      virtual ~debug_buf();

      void setOutputFlags( OutputFlags Flags );

   protected:
   
      void outputDebugMessage ( char * message );

      int sync();

      int overflow( int c = EOF );

      // Pure virtual function on PC - include a default implementation that does nothing.
      //    It's only used for input streams anyway.
      int underflow();
   };


   /**
   *
   *  \ingroup DebugMacros
   *
   *  Implementation of a std::ostream that uses the custom character buffer RWS::debug_buf.
   *  Used for the output of RenderWare Studio debug messages.
   *
   *  \see RWS_DEBUGSTREAM RWS_DEBUGSTREAM_NETWORK_ENABLE 
   *       RWS_DEBUGSTREAM_NETWORK_DISABLE RWS_TRACE
   *
   */
   class debug_ostream : public std::ostream
   {
   private:
      
      debug_buf * m_pBuf;

   public:

      debug_ostream( debug_buf * _buf );

      virtual ~debug_ostream();
      
      debug_buf* rdbuf() const;

/*
      void Test();
*/
   };

   /*
   * Object acessors - it'd be nicer if these could be externed as globals, cout-style, but
   * this leads to problems with initialization order on startup
   */
   debug_ostream& cdbg();

} // namespace RWS


/*
*  Replacement iostream formatting manipulators not defined in the PS2 C++ header files. (Assuming that the
*  iostream implementation in C:\usr\local\sce\ee\gcc\include\g++-2 or ...\g++-3 is being used).
*/
#if defined(SN_TARGET_PS2)

namespace std
{

   inline ios& fixed(ios& i)
   { i.setf(ios::fixed, ios::floatfield); return i; }

   inline ios& scientific(ios& i)
   { i.setf(ios::scientific, ios::floatfield); return i; }

   // Note: boolalpha flag doesn't seem to have been implemented on this platform.
   inline ios& boolalpha(ios& i)
   { return i; }

   inline ios& noboolalpha(ios& i)
   { return i; }

}

#endif // SN_TARGET_PS2


/*
*  operator << functions for some core RenderWare types
*/

// Vector / Matrix types
inline std::ostream& operator << ( std::ostream& s, const RwV2d& v )
{
   return s << '{' << v.x << ',' << v.y << '}';
}

inline std::ostream& operator << ( std::ostream& s, const RwV3d& v )
{
   return s << '{' << v.x << ',' << v.y << ',' << v.z << '}';
}

inline std::ostream& operator << ( std::ostream& s, const RwV4d& v )
{
   return s << '{' << v.x << ',' << v.y << ',' << v.z << ',' << v.w << '}';
}

inline std::ostream& operator << ( std::ostream& s, const RwMatrix& m )
{
   s << "R: " << m.right << ", ";
   s << "U: " << m.up << ", ";
   s << "A: " << m.at << ", ";
   s << "P: " << m.pos;

   return s;
}

// Color types
inline std::ostream& operator << ( std::ostream& s, const RwRGBAReal& c )
{
   return s << '{' << c.red << ',' << c.green << ',' << c.blue << ',' << c.alpha << '}';
}

inline std::ostream& operator << ( std::ostream& s, const RwRGBA& c )
{
   return s << '{' << c.red << ',' << c.green << ',' << c.blue << ',' << c.alpha << '}';
}

// Geometrical types
inline std::ostream& operator << ( std::ostream& s, const RwLine& l )
{
   s << "{S: " << l.start << ", E: " << l.end << "}";

   return s;
}

inline std::ostream& operator << ( std::ostream& s, const RwRect& r )
{
   s << "{" << r.x << ", " << r.y << ", " << r.w << ", " << r.h << "}";

   return s;
}

inline std::ostream& operator << ( std::ostream& s, const RwSphere& sp )
{
   s << "{C: " << sp.center << ", R: " << sp.radius << "}";

   return s;
}

inline std::ostream& operator << ( std::ostream& s, const RwBBox& bb )
{
   s << "{" << bb.sup << ", " << bb.inf << "}";

   return s;
}

// Misc types
inline std::ostream& operator << ( std::ostream& s, const RwSurfaceProperties& sp )
{
   s << "{A: " << sp.ambient << ", D: " << sp.diffuse << ", S: " << sp.specular << "}";

   return s;
}

inline std::ostream& operator << ( std::ostream& s, const RwTexCoords& tc )
{
   s << "{" << tc.u << ", " << tc.v << "}";

   return s;
}

#endif   // NDEBUG

#endif   // __DEBUGOSTREAM_H__


