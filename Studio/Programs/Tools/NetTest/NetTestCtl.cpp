/*****************************************************************************
 *
 * File :     NetTestCtl.cpp
 *
 * Abstract:  Implementation of Net Test Control tool
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
 * Copyright (c) 2004 Criterion Software Ltd.
 * All Rights Reserved.
 *
 * RenderWare is a trademark of Canon Inc.
 *
 *****************************************************************************/

// NetTestCtl.cpp : Implementation of CNetTestCtl
#include "precomp.h"
#include "NetTest.h"
#include "NetTestCtl.h"
#include <process.h> // For _beginthreadex
#include <sstream>
#include "rtfsyst.h"

// Game framework includes
#include "framework/core/netstream/netstrm.h"
#include "framework/core/streamhandler/strfunc.h"

// RenderWare3 Libraries
#pragma comment(lib, "rtfsyst.lib")

using namespace std;

namespace
{
   bool g_bStopThread = false;

   const RwUInt32 MAX_NB_FILES_PER_FS = 20;

   // ----------------------------------------------------------------------- //
   RwBool psInstallFileSystem ()
   {
       RwChar      curDirBuffer[_MAX_PATH];
       RwUInt32    retValue;
       RtFileSystem *wfs;

       // Get the current directory.
       retValue = GetCurrentDirectoryA (_MAX_PATH, curDirBuffer);
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

           CharUpperA (curDirBuffer);

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

                   ret = GetDriveTypeA (deviceName);

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

   // ----------------------------------------------------------------------- //
   bool InitRenderWare ()
   {
      if (!RwEngineInit (0, false, 0))
         return false;

      // Install file system
      if (!RtFSManagerOpen (RTFSMAN_UNLIMITED_NUM_FS))
      {
         return false;
      }
      if (!psInstallFileSystem ())
      {
         RtFSManagerClose ();
         return false;
      }

      RwEngineOpenParams OpenParams = {0};
      if (!RwEngineOpen (&OpenParams))
      {
         RwEngineTerm ();
         return false;
      }

      if (!RwEngineStart ())
      {
         RwEngineClose ();
         RtFSManagerClose ();
         RwEngineTerm ();
         return false;
      }

      return true;
   }

   // ----------------------------------------------------------------------- //
   void ShutdownRenderWare ()
   {
      RwEngineStop ();
      RwEngineClose ();
      RtFSManagerClose ();
      RwEngineTerm ();
   }

   // ----------------------------------------------------------------------- //
   template<typename T> T EndianConvert32 (const T t, bool bBigEndian)
   {
      ATLASSERT (4 == sizeof (T));
      if (bBigEndian)
      {
         u_long tmp = ntohl (*reinterpret_cast<const u_long *>(&t));
         return *reinterpret_cast<T*>(&tmp);
      }
      return t;
   }

   // ----------------------------------------------------------------------- //
   template<typename T> T EndianConvert16 (const T t, bool bBigEndian)
   {
      ATLASSERT (2 == sizeof (T));
      if (bBigEndian)
      {
         u_short tmp = ntohs (*reinterpret_cast<const u_short *>(&t));
         return *reinterpret_cast<T*>(&tmp);
      }
      return t;
   }

   // ----------------------------------------------------------------------- //
   void OutputMessage (const wchar_t * const szMessage, DWORD dwCookie)
   {
      BSTR bstrMessage = SysAllocString (szMessage);
      CNetTestCtl::SendMessageEvent (dwCookie, bstrMessage);
      SysFreeString (bstrMessage);
   }

   // ----------------------------------------------------------------------- //
   void OutputMessage (const char * const szNarrowMessage, DWORD dwCookie)
   {
      // Widen string and output it
      USES_CONVERSION;
      OutputMessage (A2W (szNarrowMessage), dwCookie);
   }

   // ----------------------------------------------------------------------- //
   void PrintCamera (RwStream *pStream, DWORD dwCookie,
                     RwChunkHeaderInfo &chi, bool bBigEndian)
   {
      OutputMessage (L"strfunc_SetDirectorsCameraMatrix:", dwCookie);

      char *buffer = new char [chi.length];
      RwStreamRead (pStream, buffer, chi.length);

      struct CameraMsg
      {
         RwMatrix m;
         RwReal fov;
      };

      CameraMsg *pCameraMsg = reinterpret_cast<CameraMsg *>(buffer);
      RwMatrix *pMatrix = RwMatrixCreate ();
      memcpy (pMatrix, &pCameraMsg->m, sizeof (RwMatrix));

      wstringstream OutputStream;
      OutputStream << L"\tPosition :" << L"<"
                   << EndianConvert32 (pMatrix->pos.x, bBigEndian) << L"\t"
                   << EndianConvert32 (pMatrix->pos.y, bBigEndian) << L"\t"
                   << EndianConvert32 (pMatrix->pos.z, bBigEndian) << L">"
                   << L"\r\n";
      OutputStream << L"\tRight    :" << L"<"
                   << EndianConvert32 (pMatrix->right.x, bBigEndian) << L"\t"
                   << EndianConvert32 (pMatrix->right.y, bBigEndian) << L"\t"
                   << EndianConvert32 (pMatrix->right.z, bBigEndian) << L">"
                   << L"\r\n";
      OutputStream << L"\tUp       :" << L"<"
                   << EndianConvert32 (pMatrix->up.x, bBigEndian) << L"\t"
                   << EndianConvert32 (pMatrix->up.y, bBigEndian) << L"\t"
                   << EndianConvert32 (pMatrix->up.z, bBigEndian) << L">"
                   << L"\r\n";
      OutputStream << L"\tAt       :" << L"<"
                   << EndianConvert32 (pMatrix->at.x, bBigEndian) << L"\t"
                   << EndianConvert32 (pMatrix->at.y, bBigEndian) << L"\t"
                   << EndianConvert32 (pMatrix->at.z, bBigEndian) << L">"
                   << L"\r\n";
      OutputStream << L"\tField of view : "
                   << pCameraMsg->fov
                   << L"\r\n";

      OutputMessage (OutputStream.str ().c_str (), dwCookie);

      RwMatrixDestroy (pMatrix);
      delete [] buffer;
   }

   // ----------------------------------------------------------------------- //
   const wstring FormatString (const char * const szText,
                               bool bUnicode, bool bBigEndian)
   {
      if (bUnicode)
      {
         const wchar_t *szString = reinterpret_cast<const wchar_t *>(szText);
         wchar_t *sz = reinterpret_cast<wchar_t *>(_alloca (
                                   sizeof (wchar_t) * (wcslen (szString) + 1)));
         wchar_t *psz = sz;
         while (*szString)
         {
            *psz = EndianConvert16 (*szString, bBigEndian);
            psz++;
            szString++;
         }
         *psz = 0;

         return sz;
      }
      else
      {
         USES_CONVERSION;
         return wstring (A2W (szText));
      }
   }

   // ----------------------------------------------------------------------- //
   const wstring FormatGUID (const GUID * const pGUID, bool bBigEndian)
   {
      wstring wsGUID;

      // Read entity uid
      GUID UID;
      ATLASSERT (sizeof (GUID) == 4 * sizeof (RwUInt32));
      memcpy (&UID, pGUID, sizeof (GUID));

      // Endian convert the data
      for (int i = 0; i < 4; i++)
      {
         reinterpret_cast<RwUInt32 *>(&UID)[i] =
             EndianConvert32 (reinterpret_cast<RwUInt32 *>(&UID)[i],
                                                           bBigEndian);
      }

      wchar_t *szGUID = 0;
      if (S_OK == StringFromIID (UID, &szGUID) && szGUID)
      {
         wsGUID = szGUID;
         CoTaskMemFree (szGUID);
      }

      return wsGUID;
   }

   // ----------------------------------------------------------------------- //
   void HandleAttribute (wstringstream &OutputStream, RwUInt32 nCommand,
                         const char *pPacket, DWORD dwCookie,
                         bool bUnicode, bool bBigEndian)
   {
      // Attribute number is padded to use 3 characters
      OutputStream << L"\t\tAttribute ";
      OutputStream.fill (' ');
      OutputStream.width (3);
      OutputStream << nCommand;

      // Display memory dump of data
      RwUInt32 nSize = *reinterpret_cast<const RwUInt32 *>(pPacket);
      nSize -= 2 * sizeof (RwUInt32);
      const RwUInt32 nMax = 16;

      char szText[nMax] = {0};
      if (int nCopied = min (nSize, nMax-1))
      {
         memcpy (szText, pPacket + 2 * sizeof (RwUInt32), nCopied);
         szText[nCopied] = 0;

         OutputStream << L": [";

         // The text view of the data is padded to use 15 chars
         // Must strip any unprintable characters or get script errors
         OutputStream.width (0);
         int i;
         wchar_t sz[2] = {0};
         for (i = 0; i < nCopied; i++)
         {
            if (szText[i] >= 0 && isalpha (szText[i]))
               sz[0] = wchar_t (szText[i]);
            else
               sz[0] = L' ';
            OutputStream << sz;
         }
         for (; i < nMax - 1; i++)
            OutputStream << L' ';

         OutputStream << L"][";
         OutputStream.setf (ios_base::right, ios_base::adjustfield);

         OutputStream.setf (ios_base::hex, ios_base::basefield);
         OutputStream.setf (ios_base::uppercase);
         if (nCopied)
         {
            OutputStream.width (2);
            OutputStream.fill ('0');
            OutputStream << (RwUInt32 (szText[0]) & 0x000000ff);

            for (i = 1; i < nCopied; i++)
            {
               OutputStream << L" ";
               OutputStream.width (2);
               OutputStream.fill ('0');
               OutputStream << (RwUInt32 (szText[i]) & 0x000000ff);
            }
         }
         OutputStream.unsetf (ios_base::uppercase);
         OutputStream.setf (ios_base::dec, ios_base::basefield);
         OutputStream << L"]";
      }
   }

   // ----------------------------------------------------------------------- //
   void HandleAttributes (const char * const buffer, DWORD dwCookie,
                          bool bUnicode, bool bBigEndian)
   {
      const RwUInt32 RWSPH_CLASSID       = 0x80000000;
      const RwUInt32 RWSPH_INSTANCEID    = 0x40000000;
      const RwUInt32 RWSPH_CREATECLASSID = 0x20000000;

      const char *pPacket = buffer;
      wstring strCurrentClass;
      while (*reinterpret_cast<const RwUInt32 *>(pPacket))
      {
         // Special commands
         RwUInt32 nCommand = *reinterpret_cast<const RwUInt32 *>(pPacket +
                                                           sizeof (RwUInt32));
         nCommand = EndianConvert32 (nCommand, bBigEndian);
         wstringstream OutputStream;
         if (RWSPH_CLASSID == nCommand)
         {
            OutputStream << L"\tClass:\t";
            strCurrentClass = FormatString (pPacket + 2 * sizeof (RwUInt32),
                                            bUnicode, bBigEndian);
            OutputStream << strCurrentClass;
         }
         else if (RWSPH_INSTANCEID == nCommand)
         {
            const GUID * pGUID = reinterpret_cast<const GUID *>(pPacket + (2 * sizeof(RwUInt32)));
            OutputStream << L"\tEntity ID:\t";
            OutputStream << FormatGUID (pGUID, bBigEndian);
         }
         else if (RWSPH_CREATECLASSID == nCommand)
         {
            const char *szBehaviour = pPacket + 2 * sizeof (RwUInt32);
            OutputStream << L"\tBehaviour:\t";
            OutputStream << FormatString (szBehaviour, bUnicode, bBigEndian);
         }
         else // It's an attribute ID
         {
            if (0 == nCommand &&
                0 == strCurrentClass.compare (L"CSystemCommands"))
            {
               OutputStream << L"\t\tAttach asset, ID:\t";
               const GUID *pGUID = reinterpret_cast<const GUID *>(pPacket + (2 * sizeof(RwUInt32)));
               OutputStream << FormatGUID (pGUID, bBigEndian);
            }
            else
            {
               HandleAttribute (OutputStream, nCommand, pPacket, dwCookie,
                                bUnicode, bBigEndian);
            }
         }
         OutputMessage (OutputStream.str ().c_str (), dwCookie);

         // Next packet
         RwUInt32 nSize = *reinterpret_cast<const RwUInt32 *>(pPacket);
         nSize = EndianConvert32 (nSize, bBigEndian);
         pPacket += nSize;
      }
      OutputMessage (L"", dwCookie);
   }

   // ----------------------------------------------------------------------- //
   void PrintCreateEntity (RwStream *pStream, DWORD dwCookie,
                           RwChunkHeaderInfo &chi,
                           bool bUnicode, bool bBigEndian)
   {
      wstringstream OutputStream;
      OutputMessage (L"strfunc_CreateEntity:", dwCookie);

      char *buffer = new char [chi.length];

      RwStreamRead (pStream, buffer, chi.length);

      RwBool * pbGlobal = reinterpret_cast<RwBool *>(buffer);
      char * pAttributePacket = reinterpret_cast<char *>(pbGlobal + 1);
      HandleAttributes (pAttributePacket, dwCookie, bUnicode, bBigEndian);

      OutputStream << L"\tGlobal Flag:\t" << ((*pbGlobal) ? L"TRUE" : L"FALSE");
      OutputStream << L"\r\n";
      OutputMessage (OutputStream.str ().c_str (), dwCookie);

      delete [] buffer;
   }

   // ----------------------------------------------------------------------- //
   void PrintLoadResources (RwStream *pStream, DWORD dwCookie,
                            RwChunkHeaderInfo &chi,
                            bool bUnicode, bool bBigEndian)
   {
      wstringstream OutputStream;
      OutputStream << L"strfunc_LoadAsset:";

      char *buffer = new char [chi.length];
      RwStreamRead (pStream, buffer, chi.length);
      if (bUnicode)
      {
         unsigned int * puLength = reinterpret_cast<unsigned int *>(buffer);
         unsigned int uLength = EndianConvert32((*puLength), bBigEndian);
         wchar_t *szName = reinterpret_cast<wchar_t *>(puLength + 1);
         GUID * pGUID   = reinterpret_cast<GUID *>(szName + uLength);
         puLength = reinterpret_cast<unsigned int *>(pGUID + 1);
         uLength = EndianConvert32((*puLength), bBigEndian);
         wchar_t *szType = reinterpret_cast<wchar_t *>(puLength + 1);
         puLength = reinterpret_cast<unsigned int *>(szType + uLength);
         uLength = EndianConvert32((*puLength), bBigEndian);
         wchar_t *szFile = reinterpret_cast<wchar_t *>(puLength + 1);
         puLength = reinterpret_cast<unsigned int *>(szFile + uLength);
         wchar_t *szDeps = reinterpret_cast<wchar_t *>(puLength + 1);

         OutputStream << L"\tName:\t";
         OutputStream << FormatString (reinterpret_cast<char *>(szName), 
                                       bUnicode, bBigEndian);
         OutputStream << L"\r\n";
         OutputStream << L"\tID:\t";
         OutputStream << FormatGUID(pGUID, bBigEndian);

         OutputStream << L"\r\n";
         OutputStream << L"\tType:\t";
         OutputStream << FormatString (reinterpret_cast<char *>(szType),
                                       bUnicode, bBigEndian);
         OutputStream << L"\r\n";
         OutputStream << L"\tFile:\t";
         OutputStream << FormatString (reinterpret_cast<char *>(szFile),
                                       bUnicode, bBigEndian);
         OutputStream << L"\r\n";

         OutputStream << L"\tDependencies:\t";
         OutputStream << FormatString (reinterpret_cast<char *>(szDeps),
                                       bUnicode, bBigEndian);
         OutputStream << L"\r\n";
         OutputStream << L"\r\n";
      }
      else
      {
         unsigned int * puLength = reinterpret_cast<unsigned int *>(buffer);
         unsigned int uLength = EndianConvert32((*puLength), bBigEndian);
         char *szName = reinterpret_cast<char *>(puLength + 1);
         GUID * pGUID   = reinterpret_cast<GUID *>(szName + uLength);
         puLength = reinterpret_cast<unsigned int *>(pGUID + 1);
         uLength = EndianConvert32((*puLength), bBigEndian);
         char *szType = reinterpret_cast<char *>(puLength + 1);
         puLength = reinterpret_cast<unsigned int *>(szType + uLength);
         uLength = EndianConvert32((*puLength), bBigEndian);
         char *szFile = reinterpret_cast<char *>(puLength + 1);
         puLength = reinterpret_cast<unsigned int *>(szFile + uLength);
         char *szDeps = reinterpret_cast<char *>(puLength + 1);

         OutputStream << L"\tName:\t";
         OutputStream << FormatString (szName, bUnicode, bBigEndian);
         OutputStream << L"\r\n";
         OutputStream << L"\tID:\t";
         OutputStream << FormatGUID (pGUID, bBigEndian);
         OutputStream << L"\r\n";
         OutputStream << L"\tType:\t";
         OutputStream << FormatString (szType, bUnicode, bBigEndian);
         OutputStream << L"\r\n";
         OutputStream << L"\tFile:\t";
         OutputStream << FormatString (szFile, bUnicode, bBigEndian);
         OutputStream << L"\r\n";

         OutputStream << L"\tDependencies:\t";
         OutputStream << FormatString (szDeps, bUnicode, bBigEndian);
         OutputStream << L"\r\n";
         OutputStream << L"\r\n";
      }
      OutputMessage (OutputStream.str ().c_str (), dwCookie);

      delete [] buffer;
   }

   // ----------------------------------------------------------------------- //
   void PrintUploadResources (RwStream *pStream, DWORD dwCookie,
                              RwChunkHeaderInfo &chi,
                              bool bUnicode, bool bBigEndian)
   {
      OutputMessage (L"strfunc_LoadEmbeddedAsset:", dwCookie);

      RwUInt32 nHeaderSize = 0;
      RwStreamRead (pStream, &nHeaderSize, sizeof (RwUInt32));
      nHeaderSize = EndianConvert32 (nHeaderSize, bBigEndian);

      wstringstream OutputStream;
      OutputStream << L"\tHeader size:\t" << nHeaderSize << L"\r\n";

      char *buffer = new char [nHeaderSize];
      RwStreamRead (pStream, buffer, nHeaderSize);

      RwUInt32 nDataSize = 0;
      RwStreamRead (pStream, &nDataSize, sizeof (RwUInt32));
      nDataSize = EndianConvert32 (nDataSize, bBigEndian);
      OutputStream << L"\tData size:\t" << nDataSize << L"\r\n";

      if (bUnicode)
      {
         unsigned int * puLength = reinterpret_cast<unsigned int *>(buffer);
         unsigned int uLength = EndianConvert32((*puLength), bBigEndian);
         wchar_t *szName = reinterpret_cast<wchar_t *>(puLength + 1);
         GUID * pGUID   = reinterpret_cast<GUID *>(szName + uLength);
         puLength = reinterpret_cast<unsigned int *>(pGUID + 1);
         uLength = EndianConvert32((*puLength), bBigEndian);
         wchar_t *szType = reinterpret_cast<wchar_t *>(puLength + 1);
         puLength = reinterpret_cast<unsigned int *>(szType + uLength);
         uLength = EndianConvert32((*puLength), bBigEndian);
         wchar_t *szFile = reinterpret_cast<wchar_t *>(puLength + 1);
         puLength = reinterpret_cast<unsigned int *>(szFile + uLength);
         wchar_t *szDeps = reinterpret_cast<wchar_t *>(puLength + 1);

         OutputStream << L"\tName:\t";
         OutputStream << FormatString (reinterpret_cast<char *>(szName),
                                       bUnicode, bBigEndian);
         OutputStream << L"\r\n";
         OutputStream << L"\tID:\t";
         OutputStream << FormatGUID (pGUID, bBigEndian);

         OutputStream << L"\r\n";
         OutputStream << L"\tType:\t";
         OutputStream << FormatString (reinterpret_cast<char *>(szType),
                                       bUnicode, bBigEndian);
         OutputStream << L"\r\n";
         OutputStream << L"\tFile:\t";
         OutputStream << FormatString (reinterpret_cast<char *>(szFile),
                                       bUnicode, bBigEndian);
         OutputStream << L"\r\n";

         OutputStream << L"\tDependencies:\t";
         OutputStream << FormatString (reinterpret_cast<char *>(szDeps),
                                       bUnicode, bBigEndian);
         OutputStream << L"\r\n\r\n";
      }
      else
      {
         unsigned int * puLength = reinterpret_cast<unsigned int *>(buffer);
         unsigned int uLength = EndianConvert32((*puLength), bBigEndian);
         char *szName = reinterpret_cast<char *>(puLength + 1);
         GUID * pGUID   = reinterpret_cast<GUID *>(szName + uLength);
         puLength = reinterpret_cast<unsigned int *>(pGUID + 1);
         uLength = EndianConvert32((*puLength), bBigEndian);
         char *szType = reinterpret_cast<char *>(puLength + 1);
         puLength = reinterpret_cast<unsigned int *>(szType + uLength);
         uLength = EndianConvert32((*puLength), bBigEndian);
         char *szFile = reinterpret_cast<char *>(puLength + 1);
         puLength = reinterpret_cast<unsigned int *>(szFile + uLength);
         char *szDeps = reinterpret_cast<char *>(puLength + 1);

         OutputStream << L"\tName:\t";
         OutputStream << FormatString (szName, bUnicode, bBigEndian);
         OutputStream << L"\r\n";
         OutputStream << L"\tID:\t";
         OutputStream << FormatGUID (pGUID, bBigEndian);
         OutputStream << L"\r\n";
         OutputStream << L"\tType:\t";
         OutputStream << FormatString (szType, bUnicode, bBigEndian);
         OutputStream << L"\r\n";
         OutputStream << L"\tFile:\t";
         OutputStream << FormatString (szFile, bUnicode, bBigEndian);
         OutputStream << L"\r\n";

         OutputStream << L"\tDependencies:\t";
         OutputStream << FormatString (szDeps, bUnicode, bBigEndian);
         OutputStream << L"\r\n\r\n";
      }
      OutputMessage (OutputStream.str ().c_str (), dwCookie);

      delete [] buffer;

      // Skip the file contents
      RwStreamSkip (pStream, nDataSize);

      // Skip any extra padding
      int nLen = chi.length - nDataSize - nHeaderSize - 2 * sizeof (RwUInt32);
      if (nLen > 0)
         RwStreamSkip (pStream, nLen);
   }

   // ----------------------------------------------------------------------- //
   void PrintTextComment (RwStream *pStream, DWORD dwCookie,
                          RwChunkHeaderInfo &chi,
                          bool bUnicode, bool bBigEndian)
   {
      wstringstream OutputStream;
      OutputStream << L"strfunc_TextComment: ";
      if (char *buffer = reinterpret_cast<char *>(_alloca (chi.length)))
      {
         RwStreamRead (pStream, buffer, chi.length);

         OutputStream << FormatString (buffer, bUnicode, bBigEndian);
      }
      OutputStream << L"\r\n";
      OutputMessage (OutputStream.str ().c_str (), dwCookie);
   }

   // ----------------------------------------------------------------------- //
   void PrintUpdateEntityParams (RwStream *pStream, DWORD dwCookie,
                                 RwChunkHeaderInfo &chi,
                                 bool bUnicode, bool bBigEndian)
   {
      OutputMessage (L"strfunc_UpdateEntityAttributes:", dwCookie);

      char *buffer = new char [chi.length];
      RwStreamRead (pStream, buffer, chi.length);

      HandleAttributes (buffer, dwCookie, bUnicode, bBigEndian);

      delete [] buffer;
   }

   // ----------------------------------------------------------------------- //
   void PrintDeleteEntity (RwStream *pStream, DWORD dwCookie,
                           RwChunkHeaderInfo &chi,
                           bool bUnicode, bool bBigEndian)
   {
      OutputMessage (L"strfunc_DeleteEntity:", dwCookie);

      char *buffer = new char [chi.length];
      RwStreamRead (pStream, buffer, chi.length);

      // Attribute packet contains ID to delete
      HandleAttributes (buffer, dwCookie, bUnicode, bBigEndian);

      delete [] buffer;
   }

   // ----------------------------------------------------------------------- //
   void PrintDeleteAsset (RwStream *pStream, DWORD dwCookie,
                          RwChunkHeaderInfo &chi,
                          bool bUnicode, bool bBigEndian)
   {
      OutputMessage (L"strfunc_UnLoadAsset:", dwCookie);

      char *buffer = new char [chi.length];
      RwStreamRead (pStream, buffer, chi.length);

      wstringstream OutputStream;
      OutputStream << L"Asset ID: ";
      GUID * pGUID   = reinterpret_cast<GUID *>(buffer);
      OutputStream << FormatGUID (pGUID, bBigEndian);
      OutputStream << L"\r\n";
      OutputMessage (OutputStream.str ().c_str (), dwCookie);

      delete [] buffer;
   }

   // ----------------------------------------------------------------------- //
   void PrintVersionNumber (RwStream *pStream, DWORD dwCookie,
                            RwChunkHeaderInfo & chi, bool bBigEndian)
   {
      wstringstream OutputStream;
      OutputStream << L"strfunc_VersionNumber:\t0x";

      char *buffer = new char [chi.length];
      RwStreamRead (pStream, buffer, chi.length);

      OutputStream.setf (ios_base::hex, ios_base::basefield);
      OutputStream << *reinterpret_cast<RwUInt32*>(buffer) << L"\r\n";
      OutputMessage (OutputStream.str ().c_str (), dwCookie);

      delete [] buffer;
   }

   // ----------------------------------------------------------------------- //
   void PrintPlacementNewParams (RwStream * pStream, RwChunkHeaderInfo & chi, 
                                 DWORD dwCookie, bool bUnicode, bool bBigEndian )
   {
      RwUInt32 uElement;
      RwUInt32 uElementCount;
      char * pCurrent;
      wstringstream OutputStream;
      OutputStream << L"strFunc_PlacementNew\r\n";

      char * pBuffer = new char[chi.length];
      RwStreamRead(pStream, pBuffer, chi.length);
      pCurrent = pBuffer;

      uElementCount = EndianConvert32(*reinterpret_cast<RwUInt32 *>(pCurrent), bBigEndian);
      pCurrent += sizeof(RwUInt32);

      // read off each element of the buffer
      for (uElement = 0; uElement < uElementCount; ++uElement)
      {
         char * pString = reinterpret_cast<char *>(pCurrent);
         RwUInt32 uStringLength = strlen(pString) + 1;
         pCurrent += uStringLength;

         // strings are padded for the PS2...
         uStringLength = uStringLength % 4;
         if (uStringLength)
         {
            pCurrent += 4 - uStringLength;
         }
         
         RwUInt32 uEntityCount = *reinterpret_cast<RwInt32 *>(pCurrent);
         uEntityCount = EndianConvert32(uEntityCount, bBigEndian);
         pCurrent += sizeof(RwInt32);

         OutputStream << L"\r\n\t";
         OutputStream << FormatString(pString, bUnicode, bBigEndian);
         OutputStream << L"\tCount: " << uEntityCount;
      }

      OutputStream << L"\r\n";
      OutputMessage (OutputStream.str ().c_str (), dwCookie);
   }

   // ---------------------------------------------------------------------- //
   void PrintUnknownHeader (RwStream * pStream, RwChunkHeaderInfo & chi, DWORD dwCookie)
   {
      wstringstream OutputStream;
      OutputStream << "Unknown header";
      OutputStream << "\r\n\tObject ID: " << GETOBJECTID (chi.type);
      OutputStream << "\r\n\tVendor ID: " << GETVENDORID (chi.type);
      OutputStream << "\r\n\tSize: " << chi.length << " bytes\r\n";
      OutputMessage (OutputStream.str ().c_str (), dwCookie);
   }

   // ----------------------------------------------------------------------- //
   // Given a stream of data, read each individual chunk header to determine 
   // the data contained in the chunk
   bool ReadStreamContents (RwStream *pStream, DWORD dwCookie,
                            bool bUnicode, bool bBigEndian)
   {
      // Returns true unless a disconnection packet is received

      using namespace RWS;

      ATLASSERT (pStream);
      // Assuming network stream or file stream
      ATLASSERT (rwSTREAMCUSTOM == pStream->type ||
                 rwSTREAMFILENAME == pStream->type);

      RwChunkHeaderInfo chi;
      while (1)
      {
         if (rwSTREAMCUSTOM == pStream->type)
         {
            if (!pStream->Type.custom.sfnread (pStream->Type.custom.data, 0, 0))
               break;
         }
         else
         {
            if (RwFeof (pStream->Type.file.fpFile))
               break;
         }

         if (!RwStreamReadChunkHeaderInfo (pStream, &chi))
         {
            //OutputMessage (L"Error reading stream!", dwCookie);
            OutputMessage (L"End of stream encountered\r\n", dwCookie);
            break;
         }

         switch (chi.type)
         {
         case MAKECHUNKID (rwVENDORID_CRITERIONRM, strfunc_VersionNumber):
            PrintVersionNumber (pStream, dwCookie, chi, bBigEndian);
            break;

         case MAKECHUNKID (rwVENDORID_CRITERIONRM, strfunc_Reset):
            OutputMessage (L"strfunc_Reset", dwCookie);
            RwStreamSkip (pStream, chi.length);
            break;

         case MAKECHUNKID (rwVENDORID_CRITERIONRM, strfunc_Reserved1):
            OutputMessage (L"strfunc_Reserved1 - ignored", dwCookie);
            RwStreamSkip (pStream, chi.length);
            break;

         case MAKECHUNKID (rwVENDORID_CRITERIONRM, strfunc_Reserved2):
            OutputMessage (L"strfunc_Reserved2 - ignored", dwCookie);
            RwStreamSkip (pStream, chi.length);
            break;
            
         case MAKECHUNKID (rwVENDORID_CRITERIONRM,
                           strfunc_SetDirectorsCameraMatrix):
            PrintCamera (pStream, dwCookie, chi, bBigEndian);
            break;

         case MAKECHUNKID (rwVENDORID_CRITERIONRM, strfunc_CreateEntity):
            PrintCreateEntity (pStream, dwCookie, chi, bUnicode, bBigEndian);
            break;

         case MAKECHUNKID (rwVENDORID_CRITERIONRM,
                           strfunc_UpdateEntityAttributes):
            PrintUpdateEntityParams (pStream, dwCookie, chi,
                                     bUnicode, bBigEndian);
            break;

         case MAKECHUNKID (rwVENDORID_CRITERIONRM, strfunc_SetFrozenMode):
            OutputMessage (L"strfunc_SetFrozenMode", dwCookie);
            RwStreamSkip (pStream, chi.length);
            break;

         case MAKECHUNKID (rwVENDORID_CRITERIONRM, strfunc_SetRunningMode):
            OutputMessage (L"strfunc_SetRunningMode", dwCookie);
            RwStreamSkip (pStream, chi.length);
            break;

         case MAKECHUNKID (rwVENDORID_CRITERIONRM,
                           strfunc_EnableDirectorsCamera):
            OutputMessage (L"strfunc_EnableDirectorsCamera", dwCookie);
            RwStreamSkip (pStream, chi.length);
            break;

         case MAKECHUNKID (rwVENDORID_CRITERIONRM,
                           strfunc_DisableDirectorsCamera):
            OutputMessage (L"strfunc_DisableDirectorsCamera", dwCookie);
            RwStreamSkip (pStream, chi.length);
            break;

         case MAKECHUNKID (rwVENDORID_CRITERIONRM, strfunc_TextComment):
            PrintTextComment (pStream, dwCookie, chi, bUnicode, bBigEndian);
            break;

         case MAKECHUNKID (rwVENDORID_CRITERIONRM, strfunc_StartSystem):
            OutputMessage (L"strfunc_StartSystem", dwCookie);
            RwStreamSkip (pStream, chi.length);
            break;

         case MAKECHUNKID (rwVENDORID_CRITERIONRM, strfunc_StopSystem):
            OutputMessage (L"strfunc_StopSystem", dwCookie);
            RwStreamSkip (pStream, chi.length);
            break;

         case MAKECHUNKID (rwVENDORID_CRITERIONRM, strfunc_DeleteEntity):
            PrintDeleteEntity (pStream, dwCookie, chi, bUnicode, bBigEndian);
            break;

         case MAKECHUNKID (rwVENDORID_CRITERIONRM, strfunc_DeleteAllEntities):
            OutputMessage (L"strfunc_DeleteAllEntities", dwCookie);
            RwStreamSkip (pStream, chi.length);
            break;

         case MAKECHUNKID (rwVENDORID_CRITERIONRM, strfunc_UnLoadAsset):
            PrintDeleteAsset (pStream, dwCookie, chi, bUnicode, bBigEndian);
            break;

         case MAKECHUNKID (rwVENDORID_CRITERIONRM, strfunc_Shutdown):
            OutputMessage (L"strfunc_Shutdown", dwCookie);
            RwStreamSkip (pStream, chi.length);
            break;

         case MAKECHUNKID (rwVENDORID_CRITERIONRM, strfunc_CloseConnection):
            OutputMessage (L"strfunc_CloseConnection", dwCookie);
            RwStreamSkip (pStream, chi.length);
            return false;
            break;

         case MAKECHUNKID (rwVENDORID_CRITERIONRM, strfunc_SendTestEvent):
            OutputMessage (L"strfunc_SendTestEvent", dwCookie);
            RwStreamSkip (pStream, chi.length);
            break;

         case MAKECHUNKID (rwVENDORID_CRITERIONRM, strfunc_Reserved3):
            OutputMessage (L"strfunc_Reserved3 - ignored", dwCookie);
            RwStreamSkip (pStream, chi.length);
            break;

         case MAKECHUNKID (rwVENDORID_CRITERIONRM, strfunc_Reserved3b):
            OutputMessage (L"strfunc_Reserved3b - ignored", dwCookie);
            RwStreamSkip (pStream, chi.length);
            break;

         case MAKECHUNKID (rwVENDORID_CRITERIONRM, strfunc_LoadAsset):
            PrintLoadResources (pStream, dwCookie, chi, bUnicode, bBigEndian);
            break;

         case MAKECHUNKID (rwVENDORID_CRITERIONRM, strfunc_LoadEmbeddedAsset):
            PrintUploadResources (pStream, dwCookie, chi, bUnicode, bBigEndian);
            break;

         case MAKECHUNKID (rwVENDORID_CRITERIONRM, strfunc_Reserved4):
            OutputMessage (L"strfunc_Reserved4 - ignored", dwCookie);
            RwStreamSkip (pStream, chi.length);
            break;

         case MAKECHUNKID (rwVENDORID_CRITERIONRM, strfunc_GetEntityMatrix):
            OutputMessage (L"strfunc_GetEntityMatrix", dwCookie);
            RwStreamSkip (pStream, chi.length);
            break;

         case MAKECHUNKID (rwVENDORID_CRITERIONRM, strfunc_CustomData):
            OutputMessage (L"strfunc_CustomData", dwCookie);
            RwStreamSkip (pStream, chi.length);
            break;

         case MAKECHUNKID (rwVENDORID_CRITERIONRM, strfunc_FunctionProfiler):
            OutputMessage (L"strfunc_Profiler", dwCookie);
            RwStreamSkip (pStream, chi.length);
            break;

         case MAKECHUNKID (rwVENDORID_CRITERIONRM, strFunc_PlacementNew):
            PrintPlacementNewParams(pStream, chi, dwCookie, bUnicode, bBigEndian);
            break;

         case MAKECHUNKID (rwVENDORID_CRITERIONRM, strfunc_ResetEntity):
            OutputMessage (L"strfunc_ResetEntity", dwCookie);
            RwStreamSkip (pStream, chi.length);
            break;

         case MAKECHUNKID (rwVENDORID_CRITERIONRM, strfunc_Initialize):
            OutputMessage (L"strfunc_Initialize", dwCookie);
            RwStreamSkip (pStream, chi.length);
            break;

         case MAKECHUNKID (rwVENDORID_CRITERIONRM, strfunc_UpdateAsset):
            OutputMessage (L"strfunc_UpdateAsset", dwCookie);
            RwStreamSkip (pStream, chi.length);
            break;

         default:
            PrintUnknownHeader (pStream, chi, dwCookie);
            RwStreamSkip (pStream, chi.length);
            break;
         }
      }

      return true;
   }

   // ----------------------------------------------------------------------- //
   long NetListen (DWORD dwCookie, bool bUnicode, bool bBigEndian)
   {
      long nErrorCode = 0;
      while (RwStream *pNetStream = RWS::NetStream::OpenForRead ())
      {
         OutputMessage (L"Read a stream", dwCookie);

         // If it's a disconnection message, disconnect
         if (!ReadStreamContents (pNetStream, dwCookie, bUnicode, bBigEndian))
         {
            nErrorCode = -1;
         }

         RwStreamClose (pNetStream, 0);
      }

      return nErrorCode;
   }

   // ----------------------------------------------------------------------- //
   RwUInt32 GetTime ()
   {
      return timeGetTime ();
   }

   // ----------------------------------------------------------------------- //
   // Starts a new thread in which to listen to requests.
   UINT __stdcall NetThreadFunc (void *pParam)
   {
      CoInitialize (0);
      DWORD dwCookie = reinterpret_cast<DWORD>(pParam);

      if (!RWS::NetStream::Init (CNetTestCtl::GetPort (dwCookie), GetTime))
      {
         OutputMessage (L"Network failed to initialise! - Aborting.", dwCookie);
         return 0;
      }

      bool bUnicode   = CNetTestCtl::IsUnicode (dwCookie);
      bool bBigEndian = CNetTestCtl::IsBigEndian (dwCookie);

      long nErrorCode = 0;
      while (!g_bStopThread)
      {
         if (nErrorCode = NetListen (dwCookie, bUnicode, bBigEndian))//=Intended
            break;

         Sleep (50);
      }

      RWS::NetStream::Shutdown ();

      {
         // This wstringstream exists in its own scope and will
         // be destroyed immediately after it is finished with.
         wstringstream OutputStream;
         OutputStream << L"Network shutdown with code " << nErrorCode;
         OutputMessage (OutputStream.str ().c_str (), dwCookie);
      }

      CNetTestCtl::SendDisconnectEvent (dwCookie, nErrorCode);
      CNetTestCtl::Revoke (dwCookie);

      CoUninitialize ();
      return 0;
   }
}

// -------------------------------------------------------------------------- //
HRESULT CNetTestCtl::FinalConstruct ()
{
   if (!InitRenderWare ())
      return E_FAIL;

   return S_OK;
}

// -------------------------------------------------------------------------- //
void CNetTestCtl::FinalRelease ()
{
   Stop ();
   ShutdownRenderWare ();
}

// -------------------------------------------------------------------------- //
void CNetTestCtl::SendDisconnectEvent (DWORD dwCookie, UINT nErrorCode)
{
   // Fire event from correct thread
   if (dwCookie)
   {
      IGlobalInterfaceTable *pIGIT = 0;
      if (SUCCEEDED (CoCreateInstance (CLSID_StdGlobalInterfaceTable, 
                                       0, CLSCTX_INPROC_SERVER, 
                                       IID_IGlobalInterfaceTable, 
                                       reinterpret_cast<void **>(&pIGIT))))
      {
         INetTestCtlUtils * pIUtils = 0;
         if (SUCCEEDED (pIGIT->GetInterfaceFromGlobal (
                                          dwCookie, __uuidof (INetTestCtlUtils),
                                          reinterpret_cast<void **>(&pIUtils))))
         {
            pIUtils->FireDisconnect (nErrorCode);
            pIUtils->Release ();
         }

         pIGIT->Release ();
      }
   }
}

// -------------------------------------------------------------------------- //
// Fires a message which can be handled by any connected objects
void CNetTestCtl::SendMessageEvent (DWORD dwCookie, BSTR strMessage)
{
   // Fire event from correct thread
   if (dwCookie)
   {
      IGlobalInterfaceTable *pIGIT = 0;
      if (SUCCEEDED (CoCreateInstance (CLSID_StdGlobalInterfaceTable, 
                                       0, CLSCTX_INPROC_SERVER, 
                                       IID_IGlobalInterfaceTable, 
                                       reinterpret_cast<void **>(&pIGIT))))
      {
         INetTestCtlUtils * pIUtils = 0;
         if (SUCCEEDED (pIGIT->GetInterfaceFromGlobal (
                                          dwCookie, __uuidof (INetTestCtlUtils),
                                          reinterpret_cast<void **>(&pIUtils))))
         {
            pIUtils->FireMessage (strMessage);
            pIUtils->Release ();
         }

         pIGIT->Release ();
      }
   }
}

// -------------------------------------------------------------------------- //
// Get the port this thread was initialised with
long CNetTestCtl::GetPort (DWORD dwCookie)
{
   long nRetVal = 0;

   if (dwCookie)
   {
      IGlobalInterfaceTable *pIGIT = 0;
      if (SUCCEEDED (CoCreateInstance (CLSID_StdGlobalInterfaceTable, 
                                       0, CLSCTX_INPROC_SERVER, 
                                       IID_IGlobalInterfaceTable, 
                                       reinterpret_cast<void **>(&pIGIT))))
      {
         INetTestCtlUtils * pIUtils = 0;
         if (SUCCEEDED (pIGIT->GetInterfaceFromGlobal (
                                          dwCookie, __uuidof (INetTestCtlUtils),
                                          reinterpret_cast<void **>(&pIUtils))))
         {
            pIUtils->GetPort (&nRetVal);
            pIUtils->Release ();
         }

         pIGIT->Release ();
      }
   }

   return nRetVal;
}

// -------------------------------------------------------------------------- //
bool CNetTestCtl::IsUnicode (DWORD dwCookie)
{
   VARIANT_BOOL bUnicode = VARIANT_FALSE;
   if (dwCookie)
   {
      IGlobalInterfaceTable *pIGIT = 0;
      if (SUCCEEDED (CoCreateInstance (CLSID_StdGlobalInterfaceTable, 
                                       0, CLSCTX_INPROC_SERVER, 
                                       IID_IGlobalInterfaceTable, 
                                       reinterpret_cast<void **>(&pIGIT))))
      {
         INetTestCtlUtils * pIUtils = 0;
         if (SUCCEEDED (pIGIT->GetInterfaceFromGlobal (
                                          dwCookie, __uuidof (INetTestCtlUtils),
                                          reinterpret_cast<void **>(&pIUtils))))
         {
            pIUtils->IsUnicode (&bUnicode);
            pIUtils->Release ();
         }

         pIGIT->Release ();
      }
   }

   return (bUnicode != VARIANT_FALSE);
}

// -------------------------------------------------------------------------- //
bool CNetTestCtl::IsBigEndian (DWORD dwCookie)
{
   VARIANT_BOOL bBigEndian = VARIANT_FALSE;
   if (dwCookie)
   {
      IGlobalInterfaceTable *pIGIT = 0;
      if (SUCCEEDED (CoCreateInstance (CLSID_StdGlobalInterfaceTable, 
                                       0, CLSCTX_INPROC_SERVER, 
                                       IID_IGlobalInterfaceTable, 
                                       reinterpret_cast<void **>(&pIGIT))))
      {
         INetTestCtlUtils * pIUtils = 0;
         if (SUCCEEDED (pIGIT->GetInterfaceFromGlobal (
                                          dwCookie, __uuidof (INetTestCtlUtils),
                                          reinterpret_cast<void **>(&pIUtils))))
         {
            pIUtils->IsBigEndian (&bBigEndian);
            pIUtils->Release ();
         }

         pIGIT->Release ();
      }
   }

   return (bBigEndian != VARIANT_FALSE);
}

// -------------------------------------------------------------------------- //
void CNetTestCtl::Revoke (DWORD dwCookie)
{
   if (dwCookie)
   {
      IGlobalInterfaceTable *pIGIT = 0;
      if (SUCCEEDED (CoCreateInstance (CLSID_StdGlobalInterfaceTable, 
                                       0, CLSCTX_INPROC_SERVER, 
                                       IID_IGlobalInterfaceTable, 
                                       reinterpret_cast<void **>(&pIGIT))))
      {
         HRESULT hr = pIGIT->RevokeInterfaceFromGlobal(dwCookie);
         pIGIT->Release ();
      }
   }
}

// -------------------------------------------------------------------------- //
// ------------------------------- COM Interface ---------------------------- //
// -------------------------------------------------------------------------- //
// Starts a new thread in which to listen to requests. If a thread is already running
// it will be killed off first.
STDMETHODIMP CNetTestCtl::Start (long nPort,
                                 VARIANT_BOOL bUnicode, VARIANT_BOOL bBigEndian)
{
   // Kill current thread if we're already running
   if (m_hThread)
      Stop ();

   // Initialise data
   m_nPort         = nPort;
   m_bUnicode      = (bUnicode   != VARIANT_FALSE);
   m_bBigEndian    = (bBigEndian != VARIANT_FALSE);

   // Register the utils object as process-wide, to access it from worker thread
   ATLASSERT (0 == m_dwCookie);
   IGlobalInterfaceTable *pIGIT = 0;
   if (SUCCEEDED (CoCreateInstance (CLSID_StdGlobalInterfaceTable, 
                                    0, CLSCTX_INPROC_SERVER, 
                                    IID_IGlobalInterfaceTable, 
                                    reinterpret_cast<void **>(&pIGIT))))
   {
      pIGIT->RegisterInterfaceInGlobal (GetUnknown (),
                                        __uuidof (INetTestCtlUtils),
                                        &m_dwCookie);
      pIGIT->Release ();
   }

   // Start the listener thread
   UINT nThreadID = 0;
   m_hThread = reinterpret_cast<HANDLE>(_beginthreadex (0, 0, NetThreadFunc,
                                        reinterpret_cast<void*>(m_dwCookie),
                                        0, &nThreadID));
   if (m_hThread)
   {
      return S_OK;
   }
   else
   {
      Revoke (m_dwCookie);
      m_dwCookie = 0;
      return S_FALSE;
   }
}

// -------------------------------------------------------------------------- //
// Destroys the current thread.
STDMETHODIMP CNetTestCtl::Stop ()
{
   // Kill the thread
   if (m_hThread)
   {
      g_bStopThread = true;

      while (WAIT_TIMEOUT == WaitForSingleObject (m_hThread, 100))
      {
         MSG msg = {0};
         if (GetMessage (&msg, 0, 0, 0))
            DispatchMessage (&msg);
      }

      CloseHandle (m_hThread);
      m_hThread = 0;
      m_dwCookie = 0;

      g_bStopThread = false;
   }

   return S_OK;
}

// -------------------------------------------------------------------------- //
// Given a file read in a stream of data, then call ReadStreamContents to
// unpack the stream
STDMETHODIMP CNetTestCtl::ShowStreamContents (BSTR strFile)
{
   if (!m_dwCookie || !strFile)
      return S_FALSE;

   USES_CONVERSION;
   char *szFile = W2A (strFile);
   if (RwStream *pStream = RwStreamOpen (rwSTREAMFILENAME, rwSTREAMREAD,
                                         reinterpret_cast<void *>(szFile)))
   {
      OutputMessage (L"Read a file stream", m_dwCookie);

      ReadStreamContents (pStream, m_dwCookie,
                          CNetTestCtl::IsUnicode (m_dwCookie),
                          CNetTestCtl::IsBigEndian (m_dwCookie));
      RwStreamClose (pStream, 0);
   }
   else
   {
      wstringstream OutputStream;
      OutputStream << L"Failed to open stream: " << strFile;
      OutputMessage (OutputStream.str ().c_str (), m_dwCookie);
   }

   return S_OK;
}

// -------------------------------------------------------------------------- //
