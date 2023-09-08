/*****************************************************************************
 *
 * File :     CFXFlagShader.h
 *
 * Abstract : An XBox behavior for rendering animated flags using a vertex shader.
 *            Sine waves are applied to each vertex based on the attributes setup
 *            in workspace. Each vertex has a weight value that is specified by
 *            changing each of the vertices alpha value. Setting the alpha to zero
 *            for example allows no movement, while setting a value of 255 allows
 *            the sine waves to have maximum effect. In the example the flagpole
 *            has an alpha value of 0 and so is not animated by the vertex shader.
 *            
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

#ifndef __CFXFLAGSHADER_H__
#define __CFXFLAGSHADER_H__

//////////////////////////////////////////////////////////////////
//
// RenderWare Includes
//
#include "rwcore.h"
#include "rpworld.h"

#ifdef _MSC_VER
#pragma comment (lib, "rwcore.lib")
#pragma comment (lib, "rpworld.lib")
#endif

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "framework/core/eventhandler/ceventhandler.h"
#include "framework/core/attributehandler/cattributehandler.h"
#include "framework/core/attributehandler/csystemcommands.h"
#include "framework/toolkits/world/smartptrs/catomicptr.h"

namespace RWS
{

   namespace FX
   {
      /**
      *
      *  \ingroup Mod_FX
      *  
      *  CFXFlagShader is an example of how to use the CFXVertexShaderBase base class in order to create a
      *  vertex shader that can be applied to atomics. Atomics are associated with the shader by dragging their asset
      *  onto an instance of CFXFlagShader in studio. The shader will animate the geometry by applying a sine wave to
      *  each vertex. Vertex alpha is used as a weight value so that if for example vertex alpha is set to zero on
      *  the flagpole then the flagpole will not be affected by the sine wave. However setting a range of vertex alpha
      *  values on the vertices in the flag cloth will vary the amount the sine waves will animate the cloth vertices.
      *
      */
      class CFXFlagShader: public CFXXBoxBaseVertexShader, public CSystemCommands, public CAttributeHandler,
                           public CEventHandler
      {

      public:

         RWS_MAKENEWCLASS(CFXFlagShader);
         RWS_DECLARE_CLASSID(CFXFlagShader);
         RWS_CATEGORY("FX_XBOX");
         RWS_DESCRIPTION("Flag Vertex Shader", "A simple vertex shader that moves the vertices in a asset to " 
                                               "approximate the motion of a flag waving in the wind");

         RWS_BEGIN_COMMANDS

         RWS_ATTRIBUTE(CMD_Set_m_HfreqA,      "Freq A",      "Frequency",     SLIDER,  RwReal,RANGE(0, 0, 0.5))
         RWS_ATTRIBUTE(CMD_Set_m_HspeedA,     "Speed A",     "Speed",         SLIDER,  RwReal,RANGE(0, 0, 2.0))
         RWS_ATTRIBUTE(CMD_Set_m_HamplitudeA, "Amplitude A", "Frequency",     SLIDER,  RwReal,RANGE(0, 0, 20.0))
         RWS_ATTRIBUTE(CMD_Set_m_HfreqB,      "Freq B",      "Frequency",     SLIDER,  RwReal,RANGE(0, 0, 0.5))
         RWS_ATTRIBUTE(CMD_Set_m_HspeedB,     "Speed B",     "Speed",         SLIDER,  RwReal,RANGE(0, 0, 2.0))
         RWS_ATTRIBUTE(CMD_Set_m_HamplitudeB, "Amplitude B", "Frequency",     SLIDER,  RwReal,RANGE(0, 0, 20.0))
         RWS_ATTRIBUTE(CMD_Set_m_VfreqA,      "Freq A",      "Frequency",     SLIDER,  RwReal,RANGE(0, 0, 0.5))
         RWS_ATTRIBUTE(CMD_Set_m_VspeedA,     "Speed A",     "Speed",         SLIDER,  RwReal,RANGE(0, 0, 2.0))
         RWS_ATTRIBUTE(CMD_Set_m_VamplitudeA, "Amplitude A", "Frequency",     SLIDER,  RwReal,RANGE(0, 0, 20.0))

         RWS_END_COMMANDS;

         CFXFlagShader                                   (const CAttributePacket& rAttr);
         ~CFXFlagShader                                  (void);
         virtual void         HandleEvents               (CMsg &pMsg);
         virtual void         HandleAttributes           (const CAttributePacket& attr);
         RwBool               CreateVertexShaders        (void);
         void                 DestroyVertexShaders       (void);
         void                 SetFlagConstantRegisters   (RwMatrix *ltm, RpWorld *pWorld);
         void                 VShaderRenderCallBack      (RxXboxResEntryHeader *resEntryHeader, void *pObject,
                                                          RwUInt8 Type, RwUInt32 Flags);
         static RpLight       *AmbientLightCallBack      (RpLight *pLight, void *pData);
         static RpLight       *PointLightCallBack        (RpLight *pLight, void *pData);

         private:
         // attributes
         RwReal               m_HfreqA;
         RwReal               m_HspeedA;
         RwReal               m_HamplitudeA;

         RwReal               m_HfreqB;
         RwReal               m_HspeedB;
         RwReal               m_HamplitudeB;
         RwReal               m_VfreqA;
         RwReal               m_VspeedA;
         RwReal               m_VamplitudeA;


         DWORD                m_VertexShaderTextured;
         DWORD                m_VertexShaderVertexColor;
         DWORD                m_VertexShaderTexturedVertexColor;
         RwV4d                m_WaveOffset;

      };


   } // namespace FX
      
} // namespace RWS

#endif
