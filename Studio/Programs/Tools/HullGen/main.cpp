/*****************************************************************************
*
* File :     main.cpp
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
* Copyright © 1993 - 2003 Criterion Software Ltd. All rights reserved.
* All Rights Reserved.
*
* RenderWare is a trademark of Canon Inc.
*
*****************************************************************************/

#include <iostream>
#include <string>
#include <cassert>
#include <windows.h>
#include <atlbase.h>

////////////////////////////////////////////////////////////////////////////////
// Renderware includes
#include "rwcore.h"
#include "rpworld.h"
#include "rpmorph.h"
#include "rpdmorph.h"
#include "rpskin.h"
#include "rpmatfx.h"
#include "rpltmap.h"
#include "rppatch.h"
#include "rplodatm.h"
#include "rtbmp.h"
#include "rtpng.h"
#include "rtras.h"
#include "rttiff.h"
#include "rtpitexd.h"
#include "rphull.h"
#include "rtfsyst.h"

//////////////////////////////////////////////////////////////////////////////
// Renderware libs
#pragma comment(lib, "rwcore.lib")
#pragma comment(lib, "rtfsyst.lib")
#pragma comment(lib, "rpworld.lib")
#pragma comment(lib, "rpmorph.lib")
#pragma comment(lib, "rpdmorph.lib")
#pragma comment(lib, "rpskin.lib")
#pragma comment(lib, "rpltmap.lib")
#pragma comment(lib, "rpmatfx.lib")
#pragma comment(lib, "rppatch.lib")
#pragma comment(lib, "rtbezpat.lib")
#pragma comment(lib, "rplodatm.lib")
#pragma comment(lib, "rtbmp.lib")
#pragma comment(lib, "rtpng.lib")
#pragma comment(lib, "rtras.lib")
#pragma comment(lib, "rttiff.lib")
#pragma comment(lib, "rtpitexd.lib")
#pragma comment(lib, "rtfsyst.lib")

#import <msxml3.dll> raw_interfaces_only
using namespace MSXML2;
using namespace std;

#ifndef _DEBUG
#define ASSERT(a) if (!a) {cout << "[Hull Generator] Fatal Error " << #a << endl;}
#define FATALERROR(a) cout << "[Hull Generator] Fatal Error " << a << endl
#else
#define ASSERT(a) assert(a)
#define FATALERROR(a) assert(!a)
#endif

namespace
{
   RwMatrix *g_pMatrix = 0;

   /////////////////////////////////////////////////////////////////////////////
   //
   RwBool AttachPlugins ()
   {
      struct Plugin
      {
         RwBool (__cdecl *m_pFunc)();
      };

      Plugin Plugins[] = 
      {
         RpWorldPluginAttach, RpMorphPluginAttach, RpDMorphPluginAttach,
         RpSkinPluginAttach,  RpPatchPluginAttach, RpMatFXPluginAttach,
         RpLtMapPluginAttach, RpLODAtomicPluginAttach
      };

      // Attach the plugins
      RwBool bRetVal = TRUE;
      for (RwUInt32 i = 0; i < sizeof (Plugins) / sizeof (Plugin); i++)
         if (!Plugins[i].m_pFunc ())
            bRetVal = FALSE;
      
      return bRetVal;
   }

   /////////////////////////////////////////////////////////////////////////////
   // Simple debug message handler for RenderWare debug messages
#ifdef _DEBUG
   void DebugMessageHandler(RwDebugType type, const RwChar *pMessage)
   {
      OutputDebugString (pMessage);
      OutputDebugString ("\n");
   }
#endif // _DEBUG

   const RwUInt32 UNC_PATH_SIZE = 256;   /* should be sufficient */
   const RwUInt32 MAX_NB_FILES_PER_FS = 20;

   void RpHullOut(const RpHull *pHull)
   {
      cout << "Data: " << endl;

      cout << " center: " << pHull->center.x << ", " << pHull->center.y << ", " << pHull->center.z << endl;

      cout << " numPlanes: " << pHull->numPlanes << endl;

      cout << " planes: " << endl;      

      for (RwInt32 i = 0; i < pHull->numPlanes; i++)
      {
         cout << "  n: " 
                     << pHull->planes[i].normal.x << ", " 
                     << pHull->planes[i].normal.y << ", " 
                     << pHull->planes[i].normal.z 
              << " d: " 
                     << pHull->planes[i].d << endl;
      }
   }

   /*
   *  
   *  platform specific method to install the file system
   *
   */
   RwBool psInstallFileSystem(void)
   {
       RwChar      curDirBuffer[_MAX_PATH];
       RwUInt32    retValue;
       RtFileSystem *wfs;

       // Get the current directory.
       retValue = GetCurrentDirectory(_MAX_PATH, curDirBuffer);
       if (0 == retValue || retValue > _MAX_PATH)
       {
           return FALSE;
       }

       // This is a unc path.
       if (curDirBuffer[1] != ':')
       {
           // tag a \ to the end of the current directory 
           // only fails if the buffer size is exhausted 
           rwstrcat( curDirBuffer, "\\" );

           // Setup the file system manager
           if ((wfs = RtWinFSystemInit(MAX_NB_FILES_PER_FS, curDirBuffer, "unc")) != 0)
           {
               // Now actually register the file system
               if (RtFSManagerRegister(wfs) == FALSE)
               {
                   return (FALSE);
               }
               else
               {
                   RtFSManagerSetDefaultFileSystem(wfs);
               }
           }
           else
           {
               return (FALSE);
           }
       }
       else
       {
           RwUInt32 drivesMask;
           RwInt32 drive;
           RwChar  fsName[2];

           CharUpper(curDirBuffer);

            
           // loop through all logical drives and register a file system for each
           // valid one.
           // Start at 2: this removes a:                      
           drivesMask = GetLogicalDrives();

           for( drive = 2, drivesMask >>= 1; drivesMask != 0; drive++, drivesMask >>= 1)
           {
               if (drivesMask & 0x01)
               {
                   RwInt32 ret;
                   RwChar  deviceName[4];

                   deviceName[0] = drive + 'A' - 1;
                   deviceName[1] = ':';
                   deviceName[2] = '\\';   // Needed for Win98
                   deviceName[3] = '\0';

                   ret = GetDriveType(deviceName);

                   if ( ret != DRIVE_UNKNOWN && ret != DRIVE_NO_ROOT_DIR )
                   {
                       // Fix device name
                       deviceName[2] = '\0';

                       fsName[0] = deviceName[0];
                       fsName[1] = '\0';

                       // Setup the file system manager
                       if ((wfs = RtWinFSystemInit(MAX_NB_FILES_PER_FS, deviceName, fsName)) != 0)
                       {
                           // Now actually register the file system
                           if (RtFSManagerRegister(wfs) == FALSE)
                           {
                               return (FALSE);
                           }
                           else
                           {
                               if (deviceName[0] == curDirBuffer[0])
                               {
                                   RtFSManagerSetDefaultFileSystem(wfs);
                               }
                           }
                       }
                       else
                       {
                           return (FALSE);
                       }
                   }
               }
           }
       }

       return (TRUE);
   }   

   /////////////////////////////////////////////////////////////////////////////
   //
   RwBool Initialise (void)
   {
      // Initialise RenderWare. The exceptions are used to protect against
      // any access violations inside RenderWare
      try
      {
         if (!RwEngineInit (0, 0, 0))
         {
            FATALERROR ("RwEngineInit failed");
            goto cleanup_init;
         }
      }
      catch (...)
      {
         FATALERROR ("An exception occurred initialising RenderWare");
         goto cleanup_init;
      }

#ifdef _DEBUG
      RwDebugSetHandler (DebugMessageHandler);
#endif // _DEBUG

      try
      {
            if (!RtFSManagerOpen(RTFSMAN_UNLIMITED_NUM_FS))
            {
               FATALERROR ("RtFSManagerOpen failed");
               goto cleanup_init;
            }
      }
      catch (...)
      {
         FATALERROR ("An exception occurred initialising RenderWare file system.\n");
         goto cleanup_init;
      }

      try
      {
            if (!psInstallFileSystem())
            {
               FATALERROR ("RWS::psInstallFileSystem() failed");
               goto cleanup_init;
            }
      }
      catch (...)
      {
         FATALERROR ("An exception occurred registering RenderWare file system.\n");
         goto cleanup_init;
      }

      // displayID should be 0 for all null libraries
      RwEngineOpenParams openParams;

      openParams.displayID = 0;

      // Attach plugins to RenderWare
      if (!AttachPlugins ())
         FATALERROR ("AttachPlugins failed\n");

      // Open RenderWare
      try
      {
         if (!RwEngineOpen (&openParams))
         {
            FATALERROR ("RwEngineOpen failed\n");
            goto cleanup_open;
         }
      }
      catch (...)
      {
         FATALERROR ("An exception occurred opening RenderWare\n");
         goto cleanup_open;
      }

      if (!RwEngineSetSubSystem(0))
         FATALERROR ("RwEngineSetSubSystem failed\n");

      // Start RenderWare
      try
      {
         if(!RwEngineStart ())
         {
            FATALERROR ("RwEngineStart failed");
            goto cleanup_start;
         }
      }
      catch (...)
      {
         FATALERROR ("An exception occurred starting RenderWare");
         goto cleanup_start;
      }

      CoInitialize (0);
      return TRUE;

      // An error occurred. Try our best to clean up RenderWare.
cleanup_start:
      try {RwEngineClose ();}
      catch (...) {}
cleanup_open:
      try {RwEngineTerm ();}
      catch (...) {}
cleanup_init:
      return FALSE;
   }

   /////////////////////////////////////////////////////////////////////////////
   //
   void Shutdown (void)
   {
      // Shutdown RenderWare
      try
      {
         if (!RwEngineStop ()) FATALERROR ("RwEngineStop failed\n");
      }
      catch (...)
      {
         FATALERROR ("An exception occurred stopping RenderWare\n");
      }

      try
      {
         if (!RwEngineClose ()) FATALERROR ("RwEngineClose failed\n");
      }
      catch (...)
      {
         FATALERROR ("An exception occurred closing RenderWare\n");
      }

      try
      {
         if (!RwEngineTerm ()) FATALERROR ("RwEngineTerm failed\n");
      }
      catch (...)
      {
         FATALERROR ("An exception occurred terminating RenderWare\n");
      }

      CoUninitialize ();
   }

   /////////////////////////////////////////////////////////////////////////////
   //
   bool ConvertBSTRToMatrix (BSTR bstrText, RwMatrix *pMatrix)
   {
      ATLASSERT (bstrText);
      ATLASSERT (pMatrix);

      // Read the tokens from the string
      const wchar_t *szSeparators = L" \t\r\n";
      wchar_t *szToken = wcstok (bstrText, szSeparators);
      RwReal f[16];
      ATLASSERT (sizeof (f) == sizeof (RwMatrix));
      int n = 0;
      while (szToken)
      {
         swscanf (szToken, L"%x", &f[n]);
         n++;

         szToken = wcstok (0, szSeparators);
         if (!szToken || n >= 16)
            break;
      }

      // Copy the data
      if (16 == n)
      {
         memcpy (pMatrix, f, sizeof (RwMatrix));
         return true;
      }
      return false;
   }

   /////////////////////////////////////////////////////////////////////////////
   //
   int ProcessHullXML (BSTR szEntityXML,
                       RwMatrix *pMatrix, string &strType)
   {
      ASSERT (szEntityXML);

      // Open XML doc
      IXMLDOMDocument2 *pXMLDoc = 0;
      HRESULT hr = CoCreateInstance (__uuidof (MSXML2::DOMDocument), 0,
                                     CLSCTX_INPROC_SERVER,
                                     __uuidof (IXMLDOMDocument2),
                                     reinterpret_cast <void**>(&pXMLDoc));
      if (FAILED (hr))
      {
         FATALERROR ("Couldn't create XML document\n");
         return -10;
      }

      pXMLDoc->put_async (VARIANT_FALSE);

      VARIANT_BOOL bSuccessful = VARIANT_FALSE;
      hr = pXMLDoc->load (CComVariant(szEntityXML), &bSuccessful);
      if (hr != S_OK || VARIANT_FALSE == bSuccessful)
      {
         pXMLDoc->Release ();
         FATALERROR ("Couldn't load entity XML file\n");
         return -11;
      }

      int nRetVal = 0;

      // Find the hull node and extract the data
      MSXML2::IXMLDOMNode *pXMLHullNode = 0;
      if (S_OK == pXMLDoc->selectSingleNode (L"/hulldata/hull", &pXMLHullNode))
      {
         // Get the shape
         MSXML2::IXMLDOMNode *pXMLNode = 0;
         if (S_OK == pXMLHullNode->selectSingleNode (L"./shape", &pXMLNode))
         {
            MSXML2::IXMLDOMElement *pElement = 0;
            if (SUCCEEDED (pXMLNode->QueryInterface (&pElement)))
            {
               VARIANT vType = {VT_EMPTY};
               if (S_OK == pElement->getAttribute (CComBSTR (L"type"), &vType))
               {
                  if (VT_BSTR == vType.vt)
                  {
                     USES_CONVERSION;
                     strType = W2A (vType.bstrVal);
                  }
                  VariantClear (&vType);
               }
               pElement->Release ();
            }
            pXMLNode->Release ();
         }

         // Get the transform
         pXMLNode = 0;
         if (S_OK == pXMLHullNode->selectSingleNode (L"./xform", &pXMLNode))
         {
            BSTR bstrText = 0;
            if (S_OK == pXMLNode->get_text (&bstrText))
            {
               ATLASSERT (pMatrix);
               if (!ConvertBSTRToMatrix (bstrText, pMatrix))
                  nRetVal = -12;

               SysFreeString (bstrText);
            }
            pXMLNode->Release ();
         }

         pXMLHullNode->Release ();
      }

      // Clean up
      pXMLDoc->Release ();

      return nRetVal;
   }

   /////////////////////////////////////////////////////////////////////////////
   //
   int WriteHullStream (const RwMatrix *pMatrix, const char *szType,
                        const char *szOutputFilename)
   {
      ATLASSERT (pMatrix);
      ATLASSERT (szType);
      ATLASSERT (szOutputFilename);

      if (stricmp (szType, "smBox")) // Unrecognised hull type
         return -20;

      // Set up a hull with 6-planes (unit box with base on x-z plane)
      RpHull Hull = {0};

      RwV3d pos = *RwMatrixGetPos(const_cast<RwMatrix *>(pMatrix));

      Hull.center.x = pos.x;
      Hull.center.y = pos.y;
      Hull.center.z = pos.z;

      // Left
      Hull.planes[Hull.numPlanes].d = .5f;
      Hull.planes[Hull.numPlanes].normal.x = -1.f;
      Hull.numPlanes++;

      // Right
      Hull.planes[Hull.numPlanes].d = .5f;
      Hull.planes[Hull.numPlanes].normal.x =  1.f;
      Hull.numPlanes++;

      // Top
      Hull.planes[Hull.numPlanes].d = 1.f;
      Hull.planes[Hull.numPlanes].normal.y =  1.f;
      Hull.numPlanes++;

      // Bottom
      Hull.planes[Hull.numPlanes].d = 0.f;
      Hull.planes[Hull.numPlanes].normal.y = -1.f;
      Hull.numPlanes++;

      // Front
      Hull.planes[Hull.numPlanes].d = .5f;
      Hull.planes[Hull.numPlanes].normal.z = -1.f;
      Hull.numPlanes++;

      // Back
      Hull.planes[Hull.numPlanes].d = .5f;
      Hull.planes[Hull.numPlanes].normal.z =  1.f;
      Hull.numPlanes++;

      // Transform it
      RpHullTransform (&Hull, pMatrix);

      // Write out hull data
      // Only do this to for debugging purposes
      // If this is called the data will be written while moving a hull in the
      // Workspace since the VolumeEntity rule generates new data continuously
      // RpHullOut(&Hull);

      // Write it out
      if (RwStream *pStream = RwStreamOpen (rwSTREAMFILENAME, rwSTREAMWRITE,
                                            szOutputFilename))
      {
         WriteHullChunkHeader (pStream, &Hull);
         RpHullStreamWrite (pStream, &Hull);
         return 0;
      }

      FATALERROR ("Failed to open stream for writing\n");
      return -21;
   }

   /////////////////////////////////////////////////////////////////////////////

} // Anon. namespace


////////////////////////////////////////////////////////////////////////////////
//
int main (int argc, char *argv[])
{
   if (!Initialise ())
      return -1;

   int nRetVal = 0;

   // Process the command line args - each arg is a texture list file
   if (argc >= 2) // Need at least an xml file to read
   {
      // Extract the entity transform
      g_pMatrix = RwMatrixCreate ();
      if (!g_pMatrix)
      {
         FATALERROR ("Couldn't create hull matrix\n");
         return -2;
      }

      // Read data from XML
      string strType;
      
      USES_CONVERSION;
      BSTR xmlFile = SysAllocString(T2COLE(argv[1]));

      nRetVal = ProcessHullXML (xmlFile, g_pMatrix, strType);      

      // Write the data to disk
      if (0 == nRetVal)
      {
         nRetVal = WriteHullStream (g_pMatrix, strType.c_str (),
                                    2 == argc ? "out.hull" : argv[2]);
      }

      SysFreeString(xmlFile);

      // Clean up
      RwMatrixDestroy (g_pMatrix);
   }
   else
   {
      cerr << "Usage: " << endl << "\t" << argv[0]
           << " entity_xml [output_filename]" << endl;
   }

   Shutdown ();

   return nRetVal;
}

////////////////////////////////////////////////////////////////////////////////
