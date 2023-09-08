#include "stdafx.h"
#include "rpmorph.h"
#include "rpmatfx.h"
#include "rpltmap.h"
#include "rtpng.h"
#include "rtbmp.h"
#include "rttiff.h"
#include "rtras.h"
#include "rpcollis.h"
#include "rpusrdat.h"
#include "RenderWareHelpers.h"
#include "RenderWareEngine.h"
#include "Components/Common/psFileSysInstall.h"

namespace RW
{
   namespace
   {
      /////////////////////////////////////////////////////////////////////////////
      // RenderWare debug handler
      void DebugMessageHandler(RwDebugType type, const RwChar *str)
      {
         switch (type)
         {
         case rwDEBUGASSERT:  OutputDebugStringA ("RW ASSERT: "); break;
         case rwDEBUGERROR:   OutputDebugStringA ("RW ERROR: "); break;
         case rwDEBUGMESSAGE: OutputDebugStringA ("RW MESSAGE: "); break;
         case rwDEBUGTRACE:   OutputDebugStringA ("RW TRACE: "); break;
         }

         OutputDebugStringA (str);
         OutputDebugStringA ("\n");
      }



#if defined (D3D8_DRVMODEL_H) || defined (D3D9_DRVMODEL_H)

      /////////////////////////////////////////////////////////////////////////////
      // Image Handler for loading DDS textures under D3D8 & D3D9

      RwTextureCallBackRead standardCB;

      RwTexture * DDSTextureLoader(const RwChar * szName, const RwChar * szMaskName)
      {    
         // first try to load the dds texture.
         RwTexture * pTexture = NULL;    

#if defined (D3D8_DRVMODEL_H) 
         if (pTexture =  RwD3D8DDSTextureRead(szName, szMaskName))
#elif defined (D3D9_DRVMODEL_H)
         if (pTexture =  RwD3D9DDSTextureRead(szName, szMaskName))
#endif
         {
            return pTexture;    
         }
   
         //Failed, so try to load using normal texture loader.
         return standardCB(szName, szMaskName);
      }

      // Make RW try to load dds textures first of all.
      void HookDDSTextureRead(void)
      {    
         standardCB = RwTextureGetReadCallBack(); 
         RwTextureSetReadCallBack(DDSTextureLoader);
      }
#endif

   }

   ///////////////////////////////////////////////////////////////////////////////
   //
   RenderWareEngine::RenderWareEngine () : m_EngineStatus (0)
   {
   }

   ///////////////////////////////////////////////////////////////////////////////
   //
   RwBool RenderWareEngine::Initialise (HWND hWnd)
   {
      if (RwEngineInit (0, 0, DEFAULT_ARENASIZE))
      {
         m_EngineStatus |= ENGINE_INIT;

         // Attach plugins
         if (AttachPlugins ())
         {
            RwEngineOpenParams openParams = {hWnd};
            if (RwEngineOpen (&openParams))
            {
               m_EngineStatus |= ENGINE_OPEN;

               if (RwEngineStart ())
               {
                  m_EngineStatus |= ENGINE_START;

                  if (RtFSManagerOpen(RTFSMAN_UNLIMITED_NUM_FS))
                  {
                     m_EngineStatus |= FILESYS_INSTALL;

                     psInstallFileSystem();

                     RwEngineSetSubSystem (0);

                     RwImageRegisterImageFormat("bmp", RtBMPImageRead, RtBMPImageWrite);
                     RwImageRegisterImageFormat("png", RtPNGImageRead, RtPNGImageWrite);
                     RwImageRegisterImageFormat("tif", RtTIFFImageRead, 0);
                     RwImageRegisterImageFormat("ras", RtRASImageRead, RtRASImageWrite);

                     RwDebugSetHandler (DebugMessageHandler);

#if defined (D3D8_DRVMODEL_H) || defined (D3D9_DRVMODEL_H)
                     HookDDSTextureRead();
#endif

                     InitTextPrint();

                     // Create the world
                     m_World.Initialise (hWnd);
                  
                     RtCompressedKeyFrameRegister();
                  }
               }
            }
         }
      }

      return IsInitialised ();
   }

   ///////////////////////////////////////////////////////////////////////////////
   //
   RwBool RenderWareEngine::IsInitialised ()
   {
      return ((m_EngineStatus == SUCCESSSTATUS) && m_World.IsInitialised ());
   }

   ///////////////////////////////////////////////////////////////////////////////
   //
   RwBool RenderWareEngine::Shutdown ()
   {
      DestroyTextPrint();
      
      // Shutdown the world
      m_World.Shutdown ();

      // Shutdown the engine
      if ((m_EngineStatus & ENGINE_START) && RwEngineStop ())
      {
         m_EngineStatus &= ~ENGINE_START;

         if ((m_EngineStatus & ENGINE_OPEN) && RwEngineClose ())
         {
            m_EngineStatus &= ~ENGINE_OPEN;
            m_EngineStatus &= ~PLUGIN_WORLD;

            if ((m_EngineStatus & ENGINE_INIT) && RwEngineTerm ())
            {
               m_EngineStatus &= ~ENGINE_INIT;

               if (m_EngineStatus & FILESYS_INSTALL) 
               {
                  RtFSManagerClose();
                  m_EngineStatus &= ~FILESYS_INSTALL;
               }
            }
         }
      }

      return (m_EngineStatus == 0);
   }

   ///////////////////////////////////////////////////////////////////////////////
   //
   RwBool RenderWareEngine::AttachPlugins ()
   {
      if (RpWorldPluginAttach ())
         m_EngineStatus |= PLUGIN_WORLD;

      // Attach animation toolkit
      if (RtAnimInitialize ())
         m_EngineStatus |= PLUGIN_ANIM;

      // Attach hierarchical animation plug-in
      if (RpHAnimPluginAttach ())
         m_EngineStatus |= PLUGIN_HANIM;

      // Attach skin plug-in.
      if (RpSkinPluginAttach ())
         m_EngineStatus |= PLUGIN_SKIN;

      if (RpMorphPluginAttach ())
         m_EngineStatus |= PLUGIN_MORPH;

      // Attach matFX & lightMap plugins
      if (RpMatFXPluginAttach ())
         m_EngineStatus |= PLUGIN_MATFX;

      if (RpLtMapPluginAttach ())
         m_EngineStatus |= PLUGIN_LIGHTMAP;

      if (RpCollisionPluginAttach())
         m_EngineStatus |= PLUGIN_COLLIS;

      if (RpUserDataPluginAttach())
         m_EngineStatus |= PLUGIN_USERDATA;

      return (m_EngineStatus & PLUGIN_WORLD);
   }

   ///////////////////////////////////////////////////////////////////////////////
   //
   RenderWareWorld& RenderWareEngine::World ()
   {
      return m_World;
   }

} // namespace RW
