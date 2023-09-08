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

#if defined(__MWERKS__)
// Force data within sdata,sbss to be in data,bss section, which helps
// prevent the small data section beign filled up.
//
#pragma push
#pragma define_section sdata ".data" far_absolute RW
#pragma define_section sbss ".bss" far_absolute RW
#include <iostream>
#pragma pop
#endif

#if defined(SN_TARGET_PS2)
#include <iostream>
#endif

#if defined(SN_TARGET_PS2) && defined(_STLPORT_VERSION) && !defined(_SN_STL_IOSTREAMS)
#error "There must be a valid iostream implementation for debugostream.h to compile."
/*
 *  If you are using ProDG for Playstation2 with the STLPort STL
 *  implementation you will need to enable iostream support for this
 *  file to compile. To do this, define _SN_STL_IOSTREAMS for the project
 *  and link against libstl.a
 *
 *  See usr\README_STLPORT4.5.3.txt for details.
 */
#endif

#elif defined _MSC_VER | defined (DOLPHIN)

#include <iostream>

#endif


namespace RWS
{
   /**
   *
   *  \ingroup DebugMacros
   *
   *  Definition of a function used to output text from a debug_buf.
   *
   *  \param szMessage The null terminated string to output.
   *
   *  \see debug_buf
   *  \see debug_buf::AddOutputCallback
   *  \see debug_buf::RemoveOutputCallback
   *
   */
   typedef void (*OutputMessageCB)( const char * szMessage );

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
   
      debug_buf();
      virtual ~debug_buf();
      
      void AddOutputCallback( OutputMessageCB OutputCB );
      void RemoveOutputCallback( OutputMessageCB OutputCB );

   protected:
   
      void OutputDebugMessage ( char * szMessage );

      int sync();
      int overflow( int c = EOF );
      int underflow();

   protected:

      char m_buffer[1024];

      #define MAXOUTPUTCALLBACKS 8
      OutputMessageCB m_OutputCallbacks[MAXOUTPUTCALLBACKS];
      RwUInt32 m_OutputCBCount;
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
   };

   /*
   * Object acessors - it'd be nicer if these could be externed as globals, cout-style, but
   * this leads to problems with initialization order on startup
   */
   debug_ostream& cdbg();

} // namespace RWS


/*
*  Replacement iostream formatting manipulators not defined in the PS2 C++ header files. 
*  (Assuming that the iostream implementation in C:\usr\local\sce\ee\gcc\include\g++-2 
*  or ...\g++-3 is being used).
*/
#if defined(SN_TARGET_PS2) && !defined(_STLPORT_VERSION)

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

#endif // SN_TARGET_PS2 && !_STLPORT_VERSION


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


