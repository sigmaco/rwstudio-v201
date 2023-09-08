/****************************************************************************
 *
 * This file is a product of Criterion Software Ltd.
 *
 * This file is provided as is with no warranties of any kind and is
 * provided without any obligation on Criterion Software Ltd.
 * or Canon Inc. to assist in its use or modification.
 *
 * Criterion Software Ltd. and Canon Inc. will not, under any
 * circumstances, be liable for any lost revenue or other damages
 * arising from the use of this file.
 *
 * Copyright (c) 2002 Criterion Software Ltd.
 * All Rights Reserved.
 *
 */

/****************************************************************************
 *
 * win_main.c
 *
 ****************************************************************************/

#include "windows.h"

// Renderware Includes
#include "rwcore.h"
#include "rpworld.h"
#include "rt2d.h"
#include "rt2danim.h"

#include "maestro.h"
#include "startup.h"

LRESULT CALLBACK MainWndProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{   
   switch (message)
   {
   case WM_CLOSE:
      DestroyWindow(window);
      
      return 0L;
   case WM_DESTROY:
      PostQuitMessage(0);
      
      return 0L;
      
   default:
      return( DefWindowProc(window, message, wParam, lParam) );
   }
   
   return -1L;
}

int RegisterMainWindowClass(HANDLE processInstance)
{
   WNDCLASS windowClass;
   
   windowClass.style = CS_BYTEALIGNWINDOW;

   windowClass.lpfnWndProc = (WNDPROC)MainWndProc;
   windowClass.cbClsExtra = 0;
   windowClass.cbWndExtra = 0;
   windowClass.hInstance = (HINSTANCE)processInstance;
   windowClass.hIcon = 0;
   windowClass.hCursor = LoadCursor(0, IDC_ARROW);
   windowClass.hbrBackground = 0;
   windowClass.lpszMenuName = 0;
   windowClass.lpszClassName = "Maestro Exporter";
   
   return (RegisterClass(&windowClass) != 0);
}

HWND CreateMainWindow(HANDLE processInstance, int posX, int posY, int sizeX, int sizeY, RwVideoModeFlag kFlags)
{   
   RECT rect;
   DWORD wndStyle;
   UINT uFlags;
   HWND hwnd;

   rect.left = rect.top = 0;
   rect.right = sizeX;
   rect.bottom = sizeY;   
   
   wndStyle = WS_OVERLAPPEDWINDOW | WS_THICKFRAME;

   uFlags = SWP_NOMOVE | SWP_NOSIZE;

   hwnd = CreateWindow(  "Maestro Exporter", "Maestro Exporter",
      wndStyle,
      posX, posY,
      rect.right - rect.left, rect.bottom - rect.top,
      (HWND)0, (HMENU)0, (HINSTANCE)processInstance, 0);
   
   return hwnd;
}

static RwChar **CommandLineToArgv(RwChar *cmdLine, RwInt32 *argCount)
{
    RwInt32 numArgs = 0;
    RwBool inArg, inString;
    RwInt32 i, len;
    RwChar *res, *str, **aptr;

    len = strlen(cmdLine);

    /* 
     * Count the number of arguments...
     */
    inString = FALSE;
    inArg = FALSE;

    for(i=0; i<=len; i++)
    {
        if( cmdLine[i] == '"' )
        {
            inString = !inString;
        }

        if( (cmdLine[i] <= ' ' && !inString) || i == len )
        {
            if( inArg ) 
            {
                inArg = FALSE;
                
                numArgs++;
            }
        } 
        else if( !inArg )
        {
            inArg = TRUE;
        }
    }

    /* 
     * Allocate memory for result...
     */
    res = (RwChar *)malloc(sizeof(RwChar *) * numArgs + len + 1);
    str = res + sizeof(RwChar *) * numArgs;
    aptr = (RwChar **)res;

    strcpy(str, cmdLine);

    /*
     * Walk through cmdLine again this time setting pointer to each arg...
     */
    inArg = FALSE;
    inString = FALSE;

    for(i=0; i<=len; i++)
    {
        if( cmdLine[i] == '"' )
        {
            inString = !inString;
        }

        if( (cmdLine[i] <= ' ' && !inString) || i == len )
        {
            if( inArg ) 
            {
                if( str[i-1] == '"' )
                {
                    str[i-1] = '\0';
                }
                else
                {
                    str[i] = '\0';
                }
                
                inArg = FALSE;
            }
        } 
        else if( !inArg && cmdLine[i] != '"' )
        {
            inArg = TRUE; 
            
            *aptr++ = &str[i];
        }
    }

    *argCount = numArgs;

    return (RwChar **)res;
}


// ******************************************************************************
int PASCAL WinMain(  HINSTANCE instance, 
   HINSTANCE prevInstance , 
   LPSTR cmdLine, 
   int cmdShow)
{
    RwInt32 i;
    HWND theMainWindow = 0;
    const RwVideoModeFlag kFlags = 0;
    RECT clientRect;
    int clientWidth;
    int clientHeight;
    RwVideoMode videoModeInfo;
    const RwUInt32 kBitDepth = 32;
    RwInt32 argc;
    RwChar **argv;

    if (!RegisterMainWindowClass(instance))
    {
        return -1;
    }

    theMainWindow = CreateMainWindow(instance, 0, 0, 640, 480, kFlags);
      
    if (theMainWindow)
    {
       GetClientRect(theMainWindow, &clientRect);       
       clientWidth = clientRect.right - clientRect.left;
       clientHeight = clientRect.bottom - clientRect.top;

       videoModeInfo.width = clientWidth;
       videoModeInfo.height = clientHeight;
       videoModeInfo.depth = kBitDepth;
       videoModeInfo.flags = kFlags;
    }

    cmdLine = GetCommandLine();

    argv = CommandLineToArgv(cmdLine, &argc);

    if (Startup(theMainWindow) != 0)
    {
       // Error occurred
       return 0;
    }

    /* Do stream dump for each file specified on command line */
    if (argc > 1)
    {
        for (i=1; i < argc; i++)
        {
           if (!ExportMaestroData(argv[i]))
           {
              break;
           }
        }

        printf("\n");
    }
    else
    {
        printf("Usage: maestroexp file1 file2 ...\n\n");
    }

    Close();

    return 0;
}

