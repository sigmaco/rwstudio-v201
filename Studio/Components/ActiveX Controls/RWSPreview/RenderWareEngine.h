#ifndef __RENDERWAREENGINE_H__
#define __RENDERWAREENGINE_H__

#include "RenderWareWorld.h"

namespace RW
{
   class RenderWareEngine
   {
   public:
      RenderWareEngine ();
      RwBool Initialise (HWND hWnd);
      RwBool IsInitialised ();
      RwBool Shutdown ();

      RenderWareWorld& World ();

   protected:
      RwBool AttachPlugins ();

      enum {DEFAULT_ARENASIZE = 0x400000 /* 4Mb */};

      // Stores the initialisation status of renderware.
      enum {ENGINE_INIT     = 0x1 << 0,
            ENGINE_OPEN     = 0x1 << 1,
            ENGINE_START    = 0x1 << 2,
            FILESYS_INSTALL = 0x1 << 3,
            PLUGIN_WORLD    = 0x1 << 4,
            PLUGIN_ANIM     = 0x1 << 5,
            PLUGIN_HANIM    = 0x1 << 6,
            PLUGIN_SKIN     = 0x1 << 7,
            PLUGIN_MORPH    = 0x1 << 8,
            PLUGIN_MATFX    = 0x1 << 9,
            PLUGIN_LIGHTMAP = 0x1 << 10,
            PLUGIN_COLLIS   = 0x1 << 11,
            PLUGIN_USERDATA = 0x1 << 12,
            SUCCESSSTATUS = (0x1 << 13) - 1};

      RwUInt32 m_EngineStatus;
      RenderWareWorld m_World;
   };
}

#endif // __RENDERWAREENGINE_H__