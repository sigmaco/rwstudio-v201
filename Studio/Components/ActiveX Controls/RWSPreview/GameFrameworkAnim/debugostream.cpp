/*****************************************************************************
 *
 * File :     DebugOstream.cpp
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
#include "stdafx.h"


#ifndef NDEBUG

#include "debugmacros.h"
#include "debugostream.h"

namespace RWS
{
   namespace
   {
      /*
      *
      *  Outputs a null-terminated string via the standard debug function for a given platform.
      *
      */
      void OutputPlatformDebugMessage( const char * message )
      {
#if defined(_MSC_VER)

         OutputDebugStringA(message);

#elif defined (SKY)

         printf("%s", message);

#elif defined (DOLPHIN)

         OSReport("%s", message);

#else
#error "You need to put your platforms debug output function here."
#endif
      }
   }
   

   const debug_buf::OutputFlags debug_buf::standard_debug_output = 0x1;
   const debug_buf::OutputFlags debug_buf::rws_workspace_output = 0x2;


   debug_buf::debug_buf()
      : m_OutputFlags(standard_debug_output)
   {
      // setup the buffer area (note end is one less, to allow for the null terminating char)
      setp(&m_buffer[0], (&m_buffer[0] + (sizeof(m_buffer) - 1)));
   }


   debug_buf::~debug_buf()
   {
      sync();
   }

   
   void debug_buf::setOutputFlags( OutputFlags Flags )
   {
      m_OutputFlags |= Flags;
   }


   void debug_buf::outputDebugMessage( char * message )
   {
      if ( (m_OutputFlags & standard_debug_output) == standard_debug_output )
      {
         // output the message to the standard debug destination
         OutputPlatformDebugMessage(message);
      }

#if defined(RWS_DESIGN)

      if ((m_OutputFlags & rws_workspace_output) == rws_workspace_output)
      {
         // send the message back to RenderWare Studio workspace. 
         //  (Disable sending any more network messages to avoid recursion.)
         m_OutputFlags |= ~rws_workspace_output;
         NetTools::SendDebugMessage(message);
         m_OutputFlags |= rws_workspace_output;
      }
      
#endif

   }


   int debug_buf::sync()
   {
      // Can't use an RWS_ASSERT here, or it will recursively call this trace class
      if ( pptr() > epptr() )
      {
         OutputPlatformDebugMessage("Warning: pptr() > epptr(), debug_buf buffer has been overrun.\n");
         RWS_DEBUGBREAK;
      }

#if defined(DOLPHIN)
      // Buffer the text on Gamecube, the buffering is a speed optimization due to the slow
      // USB connection to the devkit.
      //
      // Only flush the buffer if it is full - to keep the number of network packets down
      if ( pptr() == epptr() )
      {
#endif
         // null-terminate the string
         *pptr() = '\0';

         outputDebugMessage(pbase());

         // reset the buffer pointers (note end is one less, to allow for the null terminating char)
         setp(&m_buffer[0], (&m_buffer[0] + (sizeof(m_buffer) - 1)));

#if defined(DOLPHIN)
      }
#endif

      return 0; // return -1 for failure, anything else for success
   }


   int debug_buf::overflow( int c )
   {
      // flush the current contents of the buffer
      if ( sync() != -1 )
      {
         // flush went ok - add the given character to the buffer
         return sputc(c);
      }
      else
      {
         // flush failed - return error
         return EOF;
      }
   }


   // Pure virtual function on PC - include a default implementation that does nothing.
   //    It's only used for input streams anyway.
   int debug_buf::underflow()
   {
      return EOF;
   }


   /*
   *
   *  Constructor for a debug_ostream. Requires a debug_buf that has already been fully constructed.
   *     Note: the PS2 version of debug_ostream uses old-style iostreams, other platforms use the newer,
   *           standards-compliant versions hence the slightly different base constructor calls.
   *
   */
   debug_ostream::debug_ostream( debug_buf * pBuf )
#if defined(SKY)
      : std::ostream ( pBuf ),
#else
      : std::basic_ostream<char, std::char_traits<char> > ( pBuf ),
#endif
        m_pBuf(pBuf)
   {
   }


   debug_ostream::~debug_ostream()
   {
   }
   

   debug_buf * debug_ostream::rdbuf() const
   {
      return m_pBuf;
   }

   
   /*
   *  Global accessor function for getting at the debug ostream
   */
   debug_ostream& cdbg() 
   {
      static debug_buf buf;
      static debug_ostream str(&buf);

      return str;
   }

/*   
   // simple unit test for the debug stream class
   void debug_ostream::Test()
   {
      debug_buf buf;
      debug_ostream dbgout(&buf);

      short ashort = -45;
      int aint = -34445;
      long along = -345346521;
      unsigned short aushort = 250;
      unsigned int auint = 35444;
      unsigned long aulong = 241432342;
      float afloat = 3.56423f;
      double adouble = 24234.242423675734;
      long double alongdouble = 3421346787.2624586824;
      bool abool = true;
      void * apointer = (void*)1024;

      // test output of inbuilt types
      dbgout << 'a' << 'b' << 'c' << 'd' << 'e' << '\n' << 'f' << '\n' << std::endl;

      dbgout << "a long sentence to test the overun checking done by the output stream class" << '\n' << std::endl;

      dbgout << ashort << std::endl;
      dbgout << aint << std::endl;  
      dbgout << along << std::endl;
      dbgout << aushort << std::endl; 
      dbgout << auint << std::endl;
      dbgout << aulong << std::endl;
      dbgout << afloat << '\n' << adouble << '\n' << alongdouble << std::endl;
      dbgout << abool << '\n' << apointer << std::endl;

      // test manipulator functions
      dbgout << "Default : " << afloat << std::endl;
      dbgout << "Scientific : " << std::scientific << afloat << std::endl;
      dbgout << "Fixed : " << std::fixed << afloat << std::scientific << std::endl;
      dbgout << "Default (again): " << afloat << std::endl;

      dbgout << "The bool is : " << std::boolalpha << abool << std::noboolalpha << std::endl;
      

      // test output of RenderWare types
      RwReal aRwReal = 134.02445f;
      const RwV2d aRwV2d = {1.2232f, 345.2423f};
      const RwV3d aRwV3d = {0.5f, 2.0f, 4.0f};
      RwMatrix aRwMatrix = { {1.0f, 0.0f, 0.0f}, 0,
                             0.0f, 1.5f, 0.0f, 0, 
                             0.0f, 0.0f, 1.0f, 0, 
                             242343.05543223f, 3.362f, -4.1555553323f, 1 };
      RwV3d YAxis = {0.0f, 1.0f, 0.0f};
      RwMatrix * pMatrix = RwMatrixCreate();

      RwMatrixRotate (pMatrix, &YAxis, 45.0f, rwCOMBINEREPLACE);
      RwMatrixTranslate(pMatrix, &aRwV3d, rwCOMBINEPRECONCAT);

      dbgout << aRwReal << std::endl;
      dbgout << aRwV2d << std::endl;
      dbgout << aRwV3d << std::endl;
      dbgout << aRwMatrix << std::endl;
      dbgout << *pMatrix << std::endl;

      RwMatrixDestroy(pMatrix);
   }
*/
   
}

#endif // NDEBUG
