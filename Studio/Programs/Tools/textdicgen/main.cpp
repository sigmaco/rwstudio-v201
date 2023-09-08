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
#ifdef _DEBUG
#pragma message ("WARNING : texdictgend.exe will never be called from the build rules")
#endif

#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <fstream>
#include <cassert>

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
//#include "rpptank.h"
//#include "rpprtstd.h"
//#include "rpprtadv.h"
#include "rtltmap.h"
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
//#pragma comment (lib, "rpptank.lib")
//#pragma comment (lib, "rpprtstd.lib")
//#pragma comment (lib, "rpprtadv.lib")
#pragma comment (lib, "rtbary.lib")
#pragma comment (lib, "rtintsec.lib")
#pragma comment (lib, "rpcollis.lib")
#pragma comment (lib, "rppvs.lib")
#pragma comment (lib, "rprandom.lib")
#pragma comment (lib, "rtltmap.lib")
#pragma comment(lib, "rtfsyst.lib")

using namespace std;

#ifndef _DEBUG
#define ASSERT(a) if (!a) {cout << "[Texture Dictionary] Fatal Error " << #a << endl;}
#define FATALERROR(a) cout << "[Texture Dictionary] Fatal Error " << a << endl
#else
#define ASSERT(a) assert(a)
#define FATALERROR(a) assert(!a)
#endif

namespace
{
   /////////////////////////////////////////////////////////////////////////////
   // Keep a vector of all loaded objects
   typedef std::pair <RwObject*, RwUInt8> ObjectPair;
   typedef std::vector <ObjectPair> ObjectVector;
   typedef std::list <RwChar*> TexturesList;

   ObjectVector g_ObjectVector;

#ifdef RWSPLATFORM_PS2
   TexturesList g_ProcessedBaseTextures;

   TexturesList g_NoneProcessedBaseTextures;
#endif

   bool g_GeneratingTextureDict = false;

   const RwChar *g_NameOfCurrentItem = 0;
   
   /////////////////////////////////////////////////////////////////////////////
   //
#if defined (RWSPLATFORM_XBOX) || defined (RWSPLATFORM_GAMECUBE)
   static RwTextureCallBackRead OldCB;
   static RwTexture * DDSLoader(const RwChar * szName, const RwChar * szMaskName)
   {    
      RwTexture * pTexture = NULL;    /* first try to load the dds texture */
      
#if defined (RWSPLATFORM_XBOX)
      if (pTexture = RwXboxDDSTextureRead(szName, szMaskName))
#elif defined (RWSPLATFORM_GAMECUBE)
      if (pTexture = RwGameCubeDDSTextureRead(szName, szMaskName))
#else
      if (0)
#endif
      {
         return pTexture;    
      }
      
      /* failed, so try to load using normal texture loader */   
      return OldCB(szName, szMaskName);
   }
   
   /* make RW try to load dds textures first of all */
   void HookDDSTextureRead(void)
   {    
      OldCB = RwTextureGetReadCallBack(); 
      RwTextureSetReadCallBack(DDSLoader);
   }
#endif
   
   /////////////////////////////////////////////////////////////////////////////
   //
   RwBool RegisterImageLoader (void)
   {
      struct ImageFormat
      {
         RwImage* (__cdecl *m_pFunc)(const RwChar *);
         RwChar *m_Imagename;
      };
      
      ImageFormat ImageFormats[]=
      {
         {RtPNGImageRead, RWSTRING("png")},
         {RtBMPImageRead, RWSTRING("bmp")},
         {RtRASImageRead, RWSTRING("ras")},
         {RtTIFFImageRead,RWSTRING("tif")},
      }; 
      
      RwBool bRetVal = TRUE;
      for(RwUInt32 nIndex = 0;
      nIndex < sizeof(ImageFormats) / sizeof(ImageFormat);
      nIndex++)
      {
         if (!RwImageRegisterImageFormat(ImageFormats[nIndex].m_Imagename,
            ImageFormats[nIndex].m_pFunc,
            (RwImageCallBackWrite)0))
         {
            FATALERROR ("Unable to register image loader\n");
            bRetVal = FALSE;
         }
      }

#if defined (RWSPLATFORM_XBOX) || defined (RWSPLATFORM_GAMECUBE)
      // Add the DDS texture read
      HookDDSTextureRead();
#endif
      
      return bRetVal;
   }

   /////////////////////////////////////////////////////////////////////////////
   //
   // Sets the current iamge path for RenderWare Graphics, also makes sure the
   // image path is in the right format.
   //
   void SetImagePath( const char * const szImagePath )
   {
      if (*szImagePath)
      {
         char *temp = (char*) malloc (strlen(szImagePath) + 2 );

         strcpy(temp, szImagePath);

         char *ptemp = temp;

         // Replace bk slash with fwd slash
         //
         while (*ptemp != 0)
         {
            if (*ptemp == '\\') 
               *ptemp = '/';

            ptemp++;
         }

         // Remove the trailing dbl quote
         //
         if (ptemp[-1] == '"')
         {
            ptemp[-1] = '/';
         }

         // Ensure there is a trailing fwd slash, otherwise the image path
         // will not be valid
         //
         if (ptemp[-1] != '/')
         {
            ptemp[0] = '/';
            ptemp[1] = '\0';
         }

         RwImageSetPath (temp);

         free(temp);
      }
   }
   
   /////////////////////////////////////////////////////////////////////////////
   //
   RwBool AttachPlugins()
   {
      struct Plugin
      {
         RwBool (__cdecl *m_pFunc)();
      };
      
      // Note if you add new plugins here then you also need to add them to
      // AttachPlugins in the texdepend project main.cpp
      //
      Plugin Plugins[] = 
      {  RpWorldPluginAttach, RpMorphPluginAttach, RpDMorphPluginAttach,
         RpSkinPluginAttach,  RpPatchPluginAttach, RpMatFXPluginAttach,
         RpLtMapPluginAttach, RpLODAtomicPluginAttach
//         , RpPTankPluginAttach, RpPrtStdPluginAttach, RpPrtAdvPluginAttach
      };
      
      // Attach the plugins
      RwBool bRetVal = TRUE;
      for (RwUInt32 nIndex = 0;
      nIndex < sizeof (Plugins)/sizeof(Plugin);
      nIndex++)
      {
         if (!Plugins[nIndex].m_pFunc())
         {
            bRetVal = FALSE;
         }
      }
      
      return bRetVal;
   }
   
   /////////////////////////////////////////////////////////////////////////////
   // Callback fn used to destroy all texture dictionaries
   RwTexDictionary * DestroyTexDictCB(RwTexDictionary * pDict, void *)
   {
      RwTexDictionaryDestroy(pDict);
      
      // Invalid now yes but we don't want to early-out
      return pDict;
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
   
   /////////////////////////////////////////////////////////////////////////////
   // Reads an rwID_WORLD object from a stream
   RwObject* ReadWorld (RwStream * pStream)
   {
      try
      {
         return reinterpret_cast<RwObject*>(RpWorldStreamRead (pStream));
      }
      catch (...)
      {
         FATALERROR ("An error occurred saving a world");
         return 0;
      }
   }
   
   /////////////////////////////////////////////////////////////////////////////
   // Reads an rwID_ATOMIC object from a stream
   RwObject* ReadAtomic (RwStream * pStream)
   {
      try
      {
         return reinterpret_cast<RwObject*>(RpAtomicStreamRead (pStream));
      }
      catch (...)
      {
         FATALERROR ("An error occurred saving an atomic");
         return 0;
      }
   }
   
   /////////////////////////////////////////////////////////////////////////////
   // Reads an rwID_CLUMP object from a stream
   RwObject* ReadClump (RwStream * pStream)
   {
      try
      {
         return reinterpret_cast<RwObject*>(RpClumpStreamRead (pStream));
      }
      catch (...)
      {
         FATALERROR ("An error occurred saving a clump");
         return 0;
      }
   }

   /////////////////////////////////////////////////////////////////////////////
   //
   void Clear ()
   {
      ObjectVector::iterator Iter = g_ObjectVector.begin ();
      while (Iter != g_ObjectVector.end ())
      {
         RwObject * pObject = Iter->first;
         RwUInt8    nType   = Iter->second;
         ASSERT (pObject);
         
         try
         {
            switch (nType)
            {
            case rwID_WORLD:
               RpWorldDestroy (reinterpret_cast<RpWorld*>(pObject)); break;
            case rwID_ATOMIC:
               RpAtomicDestroy (reinterpret_cast<RpAtomic*>(pObject)); break;
            case rwID_CLUMP:
               RpClumpDestroy (reinterpret_cast<RpClump*>(pObject)); break;
            case rwID_TEXTURE:
               RwTextureDestroy(reinterpret_cast<RwTexture*>(pObject)); break;
            default:
               FATALERROR ("Unhandled type in vector");
            }
         }
         catch (...)
         {
            FATALERROR ("An exception occurred destroying an object");
         }
         
         ++Iter;
      }
      
      // Clear the vector
      g_ObjectVector.clear ();
   }

   const RwUInt32 UNC_PATH_SIZE = 256;   /* should be sufficient */
   const RwUInt32 MAX_NB_FILES_PER_FS = 20;

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
      if (!AttachPlugins ()) FATALERROR ("AttachPlugins failed\n");

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
      
      // Register image plugins
      RegisterImageLoader ();


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
      // Unload any remaining RenderWare objects
      if (!g_ObjectVector.empty ())
         Clear ();
      
      // Destroy the texture dictionary
      RwTexDictionaryForAllTexDictionaries(DestroyTexDictCB, NULL);
      
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
   }

   
#ifdef RWSPLATFORM_PS2
   /**
   *
   *  Process base texture, add to list of processed base textures.
   *
   *  
   *
   */
   RpMesh *InitLMapMesh(RpMesh *mesh, RpMeshHeader __RWUNUSED__ *header, void *data)
   {
      RpMaterial *mat = mesh->material;

      RwTexture *pTexture = RpMaterialGetTexture(mat);

      if (pTexture)
      {
         RwChar *pName = RwTextureGetName(pTexture);

         RwUInt32 flags = RtLtMapMaterialGetFlags(mat);

         RtLtMapSkyBaseTextureProcess(pTexture);

         bool found = false;

         TexturesList::iterator i = g_ProcessedBaseTextures.begin();

         while (i != g_ProcessedBaseTextures.end())
         {
            if (rwstrcmp(*i, pName) == 0)
            {
               found = true;
               break;
            }
            i++;
         }

         if (!found) 
         {
            g_ProcessedBaseTextures.push_front(pName);

#ifdef RWSPLATFORM_PS2
            if (g_NameOfCurrentItem)
            {
#ifdef RWSPLATFORM_PS2
               TexturesList::iterator i1 = g_NoneProcessedBaseTextures.begin();

               while (i1 != g_NoneProcessedBaseTextures.end())
               {
                  if (rwstrcmp(*i1, pName) == 0)
                  {
                     cout << " WARNING Object " << g_NameOfCurrentItem << " shares a light mapped texture " << pName << endl;
                     break;
                  }
                  i1++;
               }
#endif
            }
#endif
         }
      }
      
      return(mesh);
   }

   /**
   *
   *  None processed base texture, add to list of none processed textures.
   *
   *  
   *
   */
   RpMesh *ProcessNoneLtMapMeshes(RpMesh *mesh, RpMeshHeader __RWUNUSED__ *header, void *data)
   {
      RpMaterial *mat = mesh->material;

      RwTexture *pTexture = RpMaterialGetTexture(mat);

      if (pTexture)
      {
         RwChar *pName = RwTextureGetName(pTexture);


         bool found = false;

         TexturesList::iterator i = g_NoneProcessedBaseTextures.begin();

         while (i != g_NoneProcessedBaseTextures.end())
         {
            if (rwstrcmp(*i, pName) == 0)
            {
               found = true;
               break;
            }
            i++;
         }

         if (!found) 
         {
            g_NoneProcessedBaseTextures.push_front(pName);

#ifdef RWSPLATFORM_PS2
               TexturesList::iterator i1 = g_ProcessedBaseTextures.begin();

               while (i1 != g_ProcessedBaseTextures.end())
               {
                  if (rwstrcmp(*i1, pName) == 0)
                  {
                     cout << " WARNING Object " << g_NameOfCurrentItem << " shares a light mapped texture " << pName << endl;
                     break;
                  }
                  i1++;
               }
#endif
         }
      }
      
      return(mesh);
   }

   /**
   *
   *  Process base textures for atomic
   *
   *  
   *
   */
   RpAtomic *InitAtomLMap(RpAtomic *atom, void *data)
   {
      if (RtLtMapAtomicGetFlags(atom) & rtLTMAPOBJECTLIGHTMAP)
      {
         RpGeometryForAllMeshes(RpAtomicGetGeometry(atom), InitLMapMesh, 0);
      }
      else
      {
         RpGeometryForAllMeshes(RpAtomicGetGeometry(atom), ProcessNoneLtMapMeshes, 0);
      }
   
      return(atom);
   }

   /**
   *
   *  Process base textures for world sector
   *
   *  
   *
   */
   RpWorldSector *InitSectorLMap(RpWorldSector *sector, void *data)
   {
      if (RtLtMapWorldSectorGetFlags(sector) & rtLTMAPOBJECTLIGHTMAP)
      {
         RpWorldSectorForAllMeshes(sector, InitLMapMesh, 0);
      }
      else
      {
         RpWorldSectorForAllMeshes(sector, ProcessNoneLtMapMeshes, 0);
      }
   
      return(sector);
   }

   /**
   *
   *  Convert lightmaps to darkmaps
   *
   *
   */
   RwTexture *InitLMap(RwTexture *pTexture, void *data)
   {
      RwChar *pName = RwTextureGetName(pTexture);

      if (pName[0] == RWSTRING('l')
      &&  pName[1] == RWSTRING('t')
      &&  pName[2] == RWSTRING('m')
      &&  pName[3] == RWSTRING('p'))
      {
         cout << "Found LightMapped geometry, converting texture to darkmap [" << RwTextureGetName(pTexture) << "] setting rwFILTERLINEAR" << endl;

         RtLtMapSkyLightMapMakeDarkMap(pTexture);

         RwTextureSetFilterMode(pTexture, rwFILTERLINEAR); // Optimal mode for rendering darkmaps on PS2
      }

      if (pName[0] == RWSTRING('d')
      &&  pName[1] == RWSTRING('k')
      &&  pName[2] == RWSTRING('m')
      &&  pName[3] == RWSTRING('p'))
      {
         cout << "Found DarkMapped geometry, converting texture to darkmap [" << RwTextureGetName(pTexture) << "] setting rwFILTERLINEAR" << endl;

         RtLtMapSkyLightMapMakeDarkMap(pTexture);

         RwTextureSetFilterMode(pTexture, rwFILTERLINEAR); // Optimal mode for rendering darkmaps on PS2 
      }
      
      return(pTexture);
   }

   RwTexture *LimitFilterMode(RwTexture *pTexture, void *data)
   {
      RwTextureFilterMode mode = RwTextureGetFilterMode(pTexture);

      if (mode == rwFILTERNEAREST)
      {
      }
      else if (mode == rwFILTERLINEAR)
      {
      }
      else if (mode == rwFILTERMIPNEAREST)
      {
      }
      else if (mode == rwFILTERMIPLINEAR)
      {
      }
      else if (mode == rwFILTERLINEARMIPNEAREST)
      {
         RwTextureSetFilterMode(pTexture, rwFILTERMIPNEAREST);
      }
      else if (mode == rwFILTERLINEARMIPLINEAR)
      {
         RwTextureSetFilterMode(pTexture, rwFILTERMIPLINEAR);
      }

      return (pTexture);
   }
#endif

   /////////////////////////////////////////////////////////////////////////////
   //
   RwBool AddAsset (const RwChar * const szFileName,
                    const RwChar * const szImagePath)
   {
      ASSERT (szFileName);
      ASSERT (szImagePath);

      RwBool bRetVal = FALSE;
      
#ifdef _DEBUG
      RwDebugSetTraceState (TRUE);
#endif // _DEBUG
      
      // First, try to load the asset assuming it is in a RenderWare stream.
      if (RwStream *pStream = RwStreamOpen (rwSTREAMFILENAME, rwSTREAMREAD,
                                            szFileName))
      {
         bRetVal = TRUE;

         g_NameOfCurrentItem = szFileName;

         // Setup the resource path
         SetImagePath(szImagePath);
         
         RwChunkHeaderInfo HeaderInfo;
         RwStreamReadChunkHeaderInfo (pStream, &HeaderInfo);
         
         RwObject * pObject = 0;
         switch (HeaderInfo.type)
         {
         case rwID_WORLD:  pObject = ReadWorld (pStream); 
            if (pObject && g_GeneratingTextureDict)
            {
               RpWorld *pWorld = reinterpret_cast<RpWorld*>(pObject);

               if (RpLtMapWorldLightMapsQuery(pWorld))
               {
                  cout << "Processing " << szFileName << "\n";

#ifdef RWSPLATFORM_PS2
                  RpWorldForAllWorldSectors(pWorld, InitSectorLMap, 0);
#endif
               }
            }
            break;
         case rwID_ATOMIC: pObject = ReadAtomic (pStream); 
            if (pObject && g_GeneratingTextureDict)
            {
               RpAtomic *pAtomic = reinterpret_cast<RpAtomic*>(pObject);

               cout << "Processing " << szFileName << "\n";

#ifdef RWSPLATFORM_PS2
               InitAtomLMap(pAtomic, 0);
#endif
            }
            break;
         case rwID_CLUMP:  pObject = ReadClump (pStream); 
            if (pObject && g_GeneratingTextureDict)
            {
               RpClump *pClump = reinterpret_cast<RpClump*>(pObject);

               cout << "Processing " << szFileName << "\n";

#ifdef RWSPLATFORM_PS2
               RpClumpForAllAtomics(pClump, InitAtomLMap, 0);
#endif
            }
            break;
         }

         // Store the object so we can free it later
         if (pObject)
            g_ObjectVector.push_back (ObjectPair (pObject, HeaderInfo.type));

         RwStreamClose (pStream, 0);

         // If loading the asset as a RenderWare asset failed, try loading it as a texture
         // (e.g. assume it is a .png, .bmp, etc.).
         if (!pObject)
         {
            RwTexture * pTexture = 0;
            RwChar szFile[MAX_PATH];
            RwChar szImagePath[MAX_PATH];
            const RwChar * pChar;
            RwUInt32 uStart = 0;
            RwUInt32 uEnd = 0;
            RwUInt32 uCurr = 0;            

            // Only need the raw filename, so strip off any path and extension
            pChar = szFileName;
            while (*pChar != '\0')
            {
               if (*pChar == '\\' || *pChar == '/')
               {
                  uStart = static_cast<RwUInt32>(pChar - szFileName) + 1;
               }

               if (*pChar == '.')
               {
                  uEnd = static_cast<RwUInt32>(pChar - szFileName);
               }

               pChar++;
            }

            uCurr = uStart;
            while (uCurr < uEnd)
            {
               szFile[uCurr - uStart] = szFileName[uCurr];
               uCurr++;
            }
            szFile[uCurr - uStart] = '\0';

            // Set the image path for the file
            rwstrncpy(szImagePath, szFileName, uStart);
            szImagePath[uStart] = '\0';
            SetImagePath(szImagePath);

            pTexture = RwTextureRead(szFile, 0);
            if (pTexture)
            {
               g_ObjectVector.push_back (ObjectPair (reinterpret_cast<RwObject*>(pTexture), rwID_TEXTURE));

               // Force textures loaded direct from files to be bilinera no mip map
               //
               RwTextureSetFilterMode(pTexture, rwFILTERLINEAR);
            }
         }
      }
      
#ifdef _DEBUG
      RwDebugSetTraceState (FALSE);
#endif // _DEBUG
      
      return bRetVal;
   }

   RwTexture *TextureCB(RwTexture *texture, void *pData) 
   {
      char *strfiltermode = "";
      char *strname = RwTextureGetName (texture);
      char *strmask = RwTextureGetMaskName (texture);

      RwTextureFilterMode filtermode = RwTextureGetFilterMode  (texture);

      if (filtermode == rwFILTERNEAREST)          
         strfiltermode = "NEAREST - Point sampled";

      if (filtermode == rwFILTERLINEAR)           
         strfiltermode = "LINEAR - Bilinear interpolation";

      if (filtermode == rwFILTERMIPNEAREST)       
         strfiltermode = "MIPNEAREST - Point sampled per pixel mipmap";

      if (filtermode == rwFILTERMIPLINEAR)        
         strfiltermode = "MIPLINEAR - Bilinear interpolation per pixel mipmap";

      if (filtermode == rwFILTERLINEARMIPNEAREST) 
         strfiltermode = "LINEARMIPNEAREST - Mipmap interpolated, point sampled";

      if (filtermode == rwFILTERLINEARMIPLINEAR)  
         strfiltermode = "LINEARMIPLINEAR - Trilinear interpolation";

      cout << "Name [";

      if (strname)
      {
         cout << strname;
      }

      cout << "]";

      RwRaster *raster = RwTextureGetRaster (texture);

      if (raster) 
      {
         cout << " Num MipMap Levels [" << RwRasterGetNumLevels(raster) << "] [" << RwRasterGetWidth(raster) << " x " << RwRasterGetHeight(raster) << " x " << RwRasterGetDepth(raster) << "]";
      }

      cout << " Mask Name [";

      if (strmask)
      {
         cout << strmask;
      }

      cout << "]";

      cout << " Filter Mode [" << strfiltermode << "]";

      cout << endl;

      return texture;
   }
   
   /////////////////////////////////////////////////////////////////////////////
   //
   RwBool SaveTexDictionary (const RwChar * const szFileName)
   {
      RwTexDictionary *pTexDict = RwTexDictionaryGetCurrent ();
      RwBool bRetVal = FALSE;
      
      if (pTexDict)
      {
         if (RwStream *pStream = RwStreamOpen (rwSTREAMFILENAME,
            rwSTREAMWRITE, szFileName))
         {
            try
            {
#if defined (RWSPLATFORM_OPENGL) || defined (RWSPLATFORM_DIRECTX)
               // write platform independent texture dictionaries under windows
               bRetVal = (RtPITexDictionaryStreamWrite (pTexDict,pStream) != 0);

               cout << "Creating PI texture dictionary, containing the following textures;" << endl;

#pragma message ("Configured to build PI dictionary")

#else
               bRetVal = (RwTexDictionaryStreamWrite (pTexDict, pStream) != 0);

               cout << "Creating PD texture dictionary, containing the following textures;" << endl;

#pragma message ("Configured to build PD dictionary")

#endif

#ifdef RWSPLATFORM_PS2
               // This code displays a list of the textures added to the dictionary
               //
               RwTexDictionaryForAllTextures (pTexDict, TextureCB, 0);

               // And this those texture which have been processed.
               //
               TexturesList::iterator i2 = g_ProcessedBaseTextures.begin();

               while(i2 != g_ProcessedBaseTextures.end())
               {
                  cout << "Texture (" << *i2 << ") has been processed for light mapping." << endl;
                  i2++;
               }

               TexturesList::iterator i1 = g_NoneProcessedBaseTextures.begin();

               while (i1 != g_NoneProcessedBaseTextures.end())
               {
                  TexturesList::iterator i2 = g_ProcessedBaseTextures.begin();

                  while(i2 != g_ProcessedBaseTextures.end())
                  {
                     if (rwstrcmp(*i1, *i2) == 0)
                     {
                        cout << " WARNING Texture (" << *i1 << ") used in none lightmapped context is being processed for lightmaps - it will look transparent." << endl;
                     }
                     i2++;
                  }

                  i1++;
               }
#endif
            }
            catch (...)
            {
               FATALERROR ("An exception occurred saving the texture dictionary");
            }
            
            RwStreamClose (pStream, 0);
         }
      }
      
      return bRetVal;
   }
   
   /////////////////////////////////////////////////////////////////////////////
   //
   const char * GetName ()
   {
#if defined (RWSPLATFORM_OPENGL)
      return ("OpenGL");
#elif defined (RWSPLATFORM_DIRECTX)
      return ("DirectX");
#elif defined (RWSPLATFORM_XBOX)
      return ("Xbox");
#elif defined (RWSPLATFORM_PS2)
      return ("PS2");
#elif defined (RWSPLATFORM_GAMECUBE)
      return ("GameCube");
#else
#error   A platform must be specified.
#endif
   }

   /////////////////////////////////////////////////////////////////////////////
   //
   RwBool Remove1stQuotedWord (string &strLine, string &strWord)
   {
      int nPos = strLine.find ("\"");
      if (nPos != string::npos)
      {
         strLine.erase (0, nPos+1);
         nPos = strLine.find ("\"");
         if (nPos != string::npos)
         {
            strWord = strLine.substr (0, nPos);
            strLine.erase (0, nPos+1);
            return TRUE;
         }
      }
      return FALSE;
   }

   /////////////////////////////////////////////////////////////////////////////
   //
   RwBool LoadAssets (const RwChar * const szFilename)
   {
      ASSERT (szFilename);

      // Open the file
      ifstream InFile (szFilename);
      if (!InFile)
         return FALSE;

      RwBool bOK = TRUE;

      // Parse the contents
      string strLine, strAssetFile, strImagePath;
      while (getline (InFile, strLine))
      {
         // Get first quoted word - asset file
         if (!Remove1stQuotedWord (strLine, strAssetFile))
         {
            bOK = FALSE;
            break;
         }

         // Get next quoted word - image path
         if (!Remove1stQuotedWord (strLine, strImagePath))
         {
            bOK = FALSE;
            break;
         }

         // Load the asset
         if (!AddAsset (strAssetFile.c_str (), strImagePath.c_str ()))
         {
            bOK = FALSE;
            break;
         }
      }

      // Clean up
      InFile.close ();

      return bOK;
   }

   RwTexture *TextureDependecyCB(RwTexture *texture, void *pData) 
   {
      char *strname = RwTextureGetName (texture);
      char *strmask = RwTextureGetMaskName (texture);

      cout << "\"";

      if (strname)
          cout << strname;

      cout << "\" \"";
         
      if (strmask)
         cout << strmask;

      cout  << "\"" << endl;

      return texture;
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
   if (argc > 1)
   {
      if (strcmp(argv[1], "-gen") == 0)
      {
         g_GeneratingTextureDict = true;
         // Load contents of texture list and add to texture dictionary
         if (!LoadAssets (argv[2]))
         {
            nRetVal = -2;
         }
         else
         {
#ifdef RWSPLATFORM_PS2
            RwTexDictionaryForAllTextures(RwTexDictionaryGetCurrent(), InitLMap, 0);

            RwTexDictionaryForAllTextures(RwTexDictionaryGetCurrent(), LimitFilterMode, 0);
#endif

            // Generate texture dictionary using the name provided
            if (!SaveTexDictionary (argc > 3 ? argv[3] : "out.txd"))
               nRetVal = -3;
         }
      }
      else
      {
         if (strcmp(argv[1], "-dep") == 0 && argc == 4)
         {
            g_GeneratingTextureDict = false;

            if (!AddAsset(argv[2], argv[3]))
            {
               nRetVal = -2;
            }
            else
            {
               RwTexDictionary *pTexDict = RwTexDictionaryGetCurrent ();
               RwTexDictionaryForAllTextures (pTexDict, TextureDependecyCB, 0);
            }            
         }
      }
   }
   else
   {
      cerr << argv[0] << ": " << GetName() << " texture dictionary building tool" << endl;
      cerr << "Options:" << endl;
      cerr << "-dep: " << "Generate a list of textures that an asset depends on." << endl;
      cerr << "  Usage: " << argv[0] << " -dep AssetFile ImagePath" << endl;
      cerr << "-gen: " << "Generate a texture dictionary for a given list of assets" << endl;
      cerr << "  Usage: " << argv[0] << " -gen AssetListFile OutputFile" << endl;
      cerr << "  (Each line of an asset list text file contains "
           << "\"asset path\" \"image path\")" << endl;
   }

   Shutdown ();

   return nRetVal;
}

////////////////////////////////////////////////////////////////////////////////

