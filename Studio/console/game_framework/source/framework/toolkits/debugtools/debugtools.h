/*****************************************************************************
*
* File :     DebugTools.h
*
* Abstract : Provides real-time debugging tools, lines, polygons, box's
* ellipses and text on screen, this code is only available if RWS_DEBUGTOOLS is
* defined.
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

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#ifndef __DEBUGTOOLS_H__
#define __DEBUGTOOLS_H__

#include "debugswitches.h"

#ifdef RWS_DEBUGTOOLS

//////////////////////////////////////////////////////////////////
//
// RenderWare Includes
//
#include <rwcore.h>
#include <rpworld.h>
#include <rtcharse.h>

#ifdef _MSC_VER
#pragma comment (lib, "rwcore.lib")
#pragma comment (lib, "rpworld.lib")
#pragma comment (lib, "rtcharse.lib")
#endif

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "framework/core/eventhandler/ceventhandler.h"

namespace RWS
{
   /**
   *
   *  \ingroup DebugTools
   *
   *  Debugging tools, immediate mode drawing functions for displaying simple primitives such as lines and polygons.
   *
   */
   namespace DebugTools
   {
      extern RwReal m_fcounter;

      extern void Open(void);

      extern void Close(void);

      extern void _Printf (const char *fmt, ...);

      extern void Printf (const char *fmt, ...);

      extern void PrintfXY (const RwInt32 x, const RwInt32 y, const char *fmt, ...);

      extern void PrintfMatrix(RwMatrix *matrix);

      extern void PrintfXYZ(const RwV3d *pos, const RwChar *text);

      extern void Line(const RwV3d * p1, const RwV3d * p2, RwUInt8 red, RwUInt8 green, RwUInt8 blue);

      extern void Arrow(const RwV3d * p1, const RwV3d * p2, RwUInt8 red, RwUInt8 green, RwUInt8 blue);

      extern void Triangle(const RwV3d * p1,
         const RwV3d * p2,
         const RwV3d *p3,
         RwUInt8 red,
         RwUInt8 green,
         RwUInt8 blue);

      extern void Ellipsoid(const RwV3d *center,
         RwReal xr,
         RwReal yr,
         RwReal zr,
         RwUInt8 red,
         RwUInt8 green,
         RwUInt8 blue);

      extern RpLight *ShowLightCB(RpLight * light, void *data);

      extern RpWorldSector *ShowSectorCB(RpWorldSector *worldSector, void *data);

      extern void PrintfRenderState(void);

      extern void WireSphere( const RwMatrix * pWorldMatrix, RwUInt8 Red, RwUInt8 Green, RwUInt8 Blue );

      extern void WireBox( const RwMatrix * pWorldMatrix, RwUInt8 Red, RwUInt8 Green, RwUInt8 Blue );

      extern void WireCylinder( const RwMatrix * pWorldMatrix, RwUInt8 Red, RwUInt8 Green, RwUInt8 Blue );
      
      /**
      *
      *  \ingroup DebugTools
      *
      *  The \ref DisplayDebugTools class defines a behavior that handles displaying the information
      *  collected by the debugtools toolkit. 
      *  
      */
      class DisplayDebugTools : public CEventHandler
      {
         public:

           virtual void HandleEvents(CMsg &pMsg);

           DisplayDebugTools(void);

           ~DisplayDebugTools(void);

           static DisplayDebugTools* GetInstance(void) {return pDisplayDebugToolsInstance;}

           void SetStartRenderEvent(const RwChar *m_StartRender);
           void SetRenderPriority(RwUInt16 _link_priority);

           void SetRENDERSTATECULLMODE(RwCullMode _rwRENDERSTATECULLMODE)
               { m_rwRENDERSTATECULLMODE = _rwRENDERSTATECULLMODE;}

           void SetRENDERSTATESRCBLEND(RwBlendFunction _rwRENDERSTATESRCBLEND)
               { m_rwRENDERSTATESRCBLEND = _rwRENDERSTATESRCBLEND;}

           void SetRENDERSTATEDESTBLEND(RwBlendFunction _rwRENDERSTATEDESTBLEND)
               { m_rwRENDERSTATEDESTBLEND = _rwRENDERSTATEDESTBLEND;}

           void SetRENDERSTATEVERTEXALPHAENABLE(RwUInt32 _rwRENDERSTATEVERTEXALPHAENABLE)
               { m_rwRENDERSTATEVERTEXALPHAENABLE = _rwRENDERSTATEVERTEXALPHAENABLE;}

           void SetRENDERSTATEZWRITEENABLE(RwUInt32 _rwRENDERSTATEZWRITEENABLE)
               { m_rwRENDERSTATEZWRITEENABLE = _rwRENDERSTATEZWRITEENABLE;}

           void SetRENDERSTATEZTESTENABLE(RwUInt32 _rwRENDERSTATEZTESTENABLE)
               { m_rwRENDERSTATEZTESTENABLE = _rwRENDERSTATEZTESTENABLE;}

         protected:

           CEventId m_StartRender;                          /**< Event used to specify when
                                                                 rendering should begin with
                                                                 the specified RwCamera */

           RwUInt32 m_link_priority;                        /**< Current Link Priority, Used
                                                                 to determine rendering order */

           static DisplayDebugTools *pDisplayDebugToolsInstance;  /**< Pointer to instance of 
                                                                       DisplayDebugTools object */

           RwCullMode m_rwRENDERSTATECULLMODE;              /**< see RenderWare Gfx Help,
                                                                  rwRENDERSTATECULLMODE */

           RwBlendFunction m_rwRENDERSTATESRCBLEND;         /**< see RenderWare Gfx Help,
                                                                  rwRENDERSTATESRCBLEND */

           RwBlendFunction m_rwRENDERSTATEDESTBLEND;        /**< see RenderWare Gfx Help,
                                                                  rwRENDERSTATEDESTBLEND */

           RwUInt32 m_rwRENDERSTATEVERTEXALPHAENABLE;       /**< see RenderWare Gfx Help,
                                                                  rwRENDERSTATEVERTEXALPHAENABLE */
           
           RwUInt32 m_rwRENDERSTATEZWRITEENABLE;            /**< see RenderWare Gfx Help,
                                                                  rwRENDERSTATEZWRITEENABLE */

           RwUInt32 m_rwRENDERSTATEZTESTENABLE;             /**< see RenderWare Gfx Help,
                                                                  rwRENDERSTATEZTESTENABLE */

           void Poll(RwCamera *pCamera);

           void DrawLines(void);
           void DrawEllipses(void);
           void DrawPolys(void);
           void Draw3DText(RwCamera *pCamera);

           void SaveRenderState();
           void SetRenderState();
           void RestoreRenderState();

           // Temporary store of RenderState
           //
           RwCullMode _rwRENDERSTATECULLMODE;
           RwBlendFunction _rwRENDERSTATESRCBLEND;
           RwBlendFunction _rwRENDERSTATEDESTBLEND;
           RwUInt32 _rwRENDERSTATEVERTEXALPHAENABLE;
           RwUInt32 _rwRENDERSTATEZWRITEENABLE;
           RwUInt32 _rwRENDERSTATEZTESTENABLE;
           RwBool _rwRENDERSTATEFOGENABLE;
           RwTexture *_rwRENDERSTATETEXTURERASTER;
      };
   }
}
#endif   // RWS_DEBUGTOOLS
#endif   // __DEBUGTOOLS_H__
