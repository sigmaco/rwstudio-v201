/*****************************************************************************
 *
 * File :     CFXBaseEnvironmentMap.h
 *
 * Abstract : A behaviour that allows control over RenderWare materials
 *            that contain environment maps
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
#if (defined (_XBOX))

#ifndef __CFXXBOXBASEVERTEXSHADER_H__
#define __CFXXBOXBASEVERTEXSHADER_H__

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

#include "framework/core/attributehandler/allocation policies/linearallocationpolicy.h"

namespace RWS
{

   namespace FX
   {
      struct CVertexShaderPlugin;

      /**
      *
      *  \ingroup Mod_FX
      *
      *  CFXXBoxBaseVertexShader can be used as a base class to form behaviors that render atomics with a vertex shader.
      *  When creating a vertex shader for use in rendering atomics it's important to realize that you will need to take
      *  over a large part of the RenderWare pipeline. You will need to provide transformation and lighting code in your
      *  vertex shader as well as deal with render state changes in your render callback.
      *  
      *  In order to create a vertex shader behavior you will need to create a new RenderWare pipeline. You can do this
      *  very simply by calling the CreatePipeLine() member function of the CFXXBoxBaseVertexShader class. You are required
      *  to pass in a render callback to CreatePipeLine() which will be called whenever RenderWare wishes to render an
      *  atomic that is associated with the vertex shader behavior, it is best to set this render callback to point at
      *  the ProcessVertexShader() member function of CFXXBoxBaseVertexShader as this will allow you to call a non static
      *  render callback in your vertex shader class. This process is detailed further down this text.
      *  
      *  The next step is to create the vertex shaders. This is done by calling D3DDevice_CreateVertexShader() for each
      *  vertex shader you wish to use with the current behavior. Typically you will need to create a vertex shader for
      *  each of the possible types of vertex formats used by RenderWare. When rendering atomics RenderWare normally uses
      *  one of three different vertex formats listed below. 
      *  
      *  Position, Normal, Texcoords      (Textured).
      *  Position, Normal, D3Dcolor    (Not texture but vertex colored).
      *  Position, Normal, D3Dcolor, Texcoords  (Textured with vertex color).
      *  
      *  Once the vertex shader has been constructed we need to deal with the problem of the RenderWare render callback
      *  being a static function. Because RenderWare callbacks are static functions it is not possible to pass in a pointer
      *  to the vertex shader object in a nice manner (this is need to access information in the vertex shader class that
      *  is used to setup the vertex shader constant registers). However a geometry plugin has been created which gets around
      *  this problem.  Setting your render callback to the ProcessVertexShader() member of CFXXBoxBaseVertexShader gets around
      *  this problem by retrieving a pointer to your vertex shader object that has been stored in the geometry plugin data
      *  for the geometry to be rendered. This pointer was stored when the CreatePipeLine() function was called during
      *  construction. Once this pointer has been extracted a call is made to the VShaderRenderCallBack() member of your
      *  vertex shader class. Therefore classes deriving from CFXXBoxBaseVertexShader must implement VshaderRenderCallBack()
      *  as one if it's member functions. The implementation of VShaderRenderCallBack() will typically consist of the
      *  following stages.
      *  
      *  1. Getting pointers to the geometry to render.
      *  2. Setting up vertex shader constants to reflect the current state of the camera and lights (SetVertexShaderBaseDefs()
      *     is provided to assist in passing the camera state to the constant registers).
      *  3. Setting up any render states the geometry requires.
      *  4. Setting the current vertex shader.
      *  5. Drawing the vertex buffers.
      *  6. Restoring the render states to be the same states as they were on entry.
      *  
      *  When the vertex shader object is destroyed it's important to delete the vertex shaders that were created during
      *  construction. This must be done at a time when RenderWare is not using the vertex shader to render the atomic, so
      *  it's typically best to destroy the atomic before destroying the vertex shader object.
      *  
      *
      */
      class CFXXBoxBaseVertexShader : public LinearAllocationPolicy
      {

      public:

         static RxXboxAllInOneRenderCallBack             DefaultRenderCallback;
         static DWORD                                    TexturedDeclaration[];
         static DWORD                                    VertexColorDeclaration[];
         static DWORD                                    TexturedVertexColorDeclaration[];


         CFXXBoxBaseVertexShader                             ();
         ~CFXXBoxBaseVertexShader                            (void);
                                                         
         RwBool               CreatePipeLine             (RxXboxAllInOneRenderCallBack RenderCallBack);
         virtual void         VShaderRenderCallBack      (RxXboxResEntryHeader *resEntryHeader, void *object, RwUInt8 type, RwUInt32 flags) = 0;
         static void          ProcessVertexShader        (RxXboxResEntryHeader *resEntryHeader, void *object, RwUInt8 type, RwUInt32 flags);
         static RpAtomic     *SetAtomicVShaderPipeline   (RpAtomic *pAtomic, void *data);
         static void          SetVertexShaderBaseDefs    (RwMatrix *ltm);

         static CVertexShaderPlugin* GetPluginData       (RpGeometry &geometry );
         static void         *Plugin_Constructor         (void *pObject, RwInt32 offsetInObject, RwInt32 sizeInObject);
         static void         *Plugin_Copy                (void *pDstObject, const void *pSrcObject, RwInt32 offset, RwInt32 size);
         static RwBool        PluginAttach               (void);
         static CFXXBoxBaseVertexShader *GetVShader      (RpAtomic *pAtomic);
         static void          SetVShader                 (RpAtomic *pAtomic, CFXXBoxBaseVertexShader *pVertexShader);


      protected:
         RxPipeline          *m_AtomicPipe;
      };

   } // namespace FX
      
} // namespace RWS

#endif
#endif