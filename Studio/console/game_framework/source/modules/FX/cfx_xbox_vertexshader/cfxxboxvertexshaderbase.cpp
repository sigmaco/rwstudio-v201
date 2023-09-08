/*****************************************************************************
 *
 * File :     CFXXBoxBaseVertexShader.cpp
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


#include "precomp.h"
#include "rwcore.h"

// Left to its own devices, the MW linker will dead-strip the registrar ctors, so...
#ifdef __MWERKS__
#pragma force_active on
#endif


//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "cfxxboxvertexshaderbase.h"

#include "framework/core/macros/debugmacros.h"
#include "framework/toolkits/world/clevel.h"
#include "framework/toolkits/math/maths.h"
#include "framework/mainloop/render.h"



#include "xboxvertexshaderbasedefs.h"

namespace RWS
{

   namespace FX
   {


      // textured
      DWORD CFXXBoxBaseVertexShader::TexturedDeclaration[] =
      {
          D3DVSD_STREAM(0),
          D3DVSD_REG(VSD_REG_POS,         D3DVSDT_FLOAT3),        /* POSITION  - register v0 */
          D3DVSD_REG(VSD_REG_NORMAL,      D3DVSDT_NORMPACKED3),   /* NORMAL    - register v1 */
          D3DVSD_REG(VSD_REG_TEXCOORDS,   D3DVSDT_FLOAT2),        /* TEXCOORDS - register v2 */
          D3DVSD_END()
      };

      // No texture, vertex color
      DWORD CFXXBoxBaseVertexShader::VertexColorDeclaration[] =
      {
          D3DVSD_STREAM(0),
          D3DVSD_REG(VSD_REG_POS,         D3DVSDT_FLOAT3),        /* POSITION  - register v0 */
          D3DVSD_REG(VSD_REG_NORMAL,      D3DVSDT_NORMPACKED3),   /* NORMAL    - register v1 */
          D3DVSD_REG(VSD_REG_COLOR,       D3DVSDT_D3DCOLOR),      /* D3DCOLOR  - register v3 */
          D3DVSD_END()
      };

      // texture and vertex color
      DWORD CFXXBoxBaseVertexShader::TexturedVertexColorDeclaration[] =
      {
          D3DVSD_STREAM(0),
          D3DVSD_REG(VSD_REG_POS,         D3DVSDT_FLOAT3),        /* POSITION  - register v0 */
          D3DVSD_REG(VSD_REG_NORMAL,      D3DVSDT_NORMPACKED3),   /* NORMAL    - register v1 */
          D3DVSD_REG(VSD_REG_COLOR,       D3DVSDT_D3DCOLOR),      /* D3DCOLOR  - register v3 */
          D3DVSD_REG(VSD_REG_TEXCOORDS,   D3DVSDT_FLOAT2),        /* TEXCOORDS - register v2 */
          D3DVSD_END()
      };


      // the default render callback is stored here
      RxXboxAllInOneRenderCallBack CFXXBoxBaseVertexShader::DefaultRenderCallback = 0;


      // geometry plugin
      const RwUInt32 CFXXBoxBaseVertexShaderGeometryPluginID = 0x1CD;
      RwUInt32       iCFXXBoxBaseVertexShaderGeometryPlugin_Offset = 0;

      struct CVertexShaderPlugin
      {
         CFXXBoxBaseVertexShader  *m_VShader;
      };            


      /**
      *  Retrieves Geometry Plugin data.
      *
      *  \param geometry Reference to an RpGeometry object.
      *
      *  \return A pointer to the CVertexShaderPlugin data.
      */
      CVertexShaderPlugin* CFXXBoxBaseVertexShader::GetPluginData( RpGeometry& geometry )
      {
         RWS_FUNCTION("RWS::FX::CFXXBoxBaseVertexShader::GetPluginData");

         RWS_PRE(iCFXXBoxBaseVertexShaderGeometryPlugin_Offset);

         const RwUInt32 baseAddr = reinterpret_cast<RwUInt32>(&geometry);
         const RwUInt32 pluginAddr = baseAddr + iCFXXBoxBaseVertexShaderGeometryPlugin_Offset;

         RWS_RETURN( reinterpret_cast<CVertexShaderPlugin*>(pluginAddr) );
      }

      /**
      *  Constructor for CLevelAtomicPlugin data.
      *
      *  \param pObject pointer to the object to be constructed, the RpAtomic.
      *  \param offsetInObject byte offset from object pointer to plugin data.
      *  \param sizeInObject size of plugin data.
      *
      *  \returns Returns pointer to the constructed object.
      */
      void* CFXXBoxBaseVertexShader::Plugin_Constructor(void* pObject, RwInt32 offsetInObject, RwInt32 sizeInObject)
      {
         RWS_FUNCTION("RWS::FX::CFXXBoxBaseVertexShader::Plugin_Constructor");

         RWS_PRE(pObject);

         CVertexShaderPlugin& pluginData = *GetPluginData(*(RpGeometry*)pObject);
         pluginData.m_VShader       = 0;

         RWS_RETURN(pObject);
      }

      /**
      *  Copy operator for CLevelAtomicPlugin.
      *
      *  \param pDstObject void pointer to the destination object, the source RpAtomic.
      *  \param pSrcObject void pointer to the destination object, the destination RpAtomic.
      *  \param offset byte offset from object pointers to plugin data.
      *  \param size of plugin data to copy.
      *
      *  \return Returns pDstObject.
      */
      void *CFXXBoxBaseVertexShader::Plugin_Copy(void* pDstObject, const void* pSrcObject, RwInt32 offset,
                                                 RwInt32 size)
      {
         RWS_FUNCTION("RWS::FX::CFXXBoxBaseVertexShader::Plugin_Copy");

         RWS_PRE(pDstObject);
         RWS_PRE(pSrcObject);

         CVertexShaderPlugin& srcData = *GetPluginData(*(RpGeometry*)pSrcObject);
         CVertexShaderPlugin& dstData = *GetPluginData(*(RpGeometry*)pDstObject);

         dstData = srcData;

         RWS_RETURN(pDstObject);
      }

      /**
      *  Attaches the CEventHandler-RpAtomic association plugin - this must happen AFTER the RW engine is
      *  initialized but BEFORE it is opened. Returns FALSE if the plugin fails to
      *  attach.
      *
      *  \returns TRUE if successful otherwise FALSE.
      */
      RwBool CFXXBoxBaseVertexShader::PluginAttach(void)
      {
          RWS_FUNCTION("RWS::FX::CFXXBoxBaseVertexShader::PluginAttach");

         iCFXXBoxBaseVertexShaderGeometryPlugin_Offset = RpGeometryRegisterPlugin (
               sizeof(CVertexShaderPlugin),
               MAKECHUNKID(rwVENDORID_CRITERIONRM, CFXXBoxBaseVertexShaderGeometryPluginID),
               Plugin_Constructor,  
               0, // NOT USED
               Plugin_Copy
               );

         RWS_POST(CFXXBoxBaseVertexShaderGeometryPluginID);

         RWS_RETURN(0 != CFXXBoxBaseVertexShaderGeometryPluginID);
      }

      /**
      *  Gets the vertex shader object from the geometry plugin data for pAtomic.
      *
      *  \param *pAtomic Pointer to an atomic.
      *
      *  \return Returns is pointer to an vertex shader object.
      */
      CFXXBoxBaseVertexShader *CFXXBoxBaseVertexShader::GetVShader(RpAtomic *pAtomic)
      {
         RWS_FUNCTION("RWS::FX::CFXXBoxBaseVertexShader::GetVShader");

         RpGeometry *pGeometry = RpAtomicGetGeometry(pAtomic);

         CVertexShaderPlugin& pluginData = *GetPluginData(*pGeometry);

         if (pluginData.m_VShader) RWS_RETURN(pluginData.m_VShader);

         RWS_RETURN(0);
      }

      /**
      *  Stores the vertex shader object in the geometry plugin data for pAtomic.
      *
      *  \param *pAtomic      Pointer to an atomic.
      *  \param *VertexShader Pointer to a vertex shader object.
      *
      *  \return Returns void.
      */
      void CFXXBoxBaseVertexShader::SetVShader(RpAtomic *pAtomic, CFXXBoxBaseVertexShader *pVertexShader)
      {
         RWS_FUNCTION("RWS::FX::CFXXBoxBaseVertexShader::SetVShader(RpAtomic *pAtomic, \
                       CFXXBoxBaseVertexShader *VertexShader)");

         RpGeometry *pGeometry = RpAtomicGetGeometry(pAtomic);

         CVertexShaderPlugin& pluginData = *GetPluginData(*pGeometry);

         pluginData.m_VShader = pVertexShader;
      
         RWS_RETURNVOID();
      }

      /**
      *
      *  Construct a CFXXBoxBaseVertexShader object.
      *
      */
      CFXXBoxBaseVertexShader::CFXXBoxBaseVertexShader()
      {
         RWS_FUNCTION("RWS::FX::CFXXBoxBaseVertexShader::CFXXBoxBaseVertexShader");

         m_AtomicPipe = 0;

         RWS_RETURNVOID();
      }

      /**
      *
      *  Destroy a CFXXBoxBaseVertexShader object.
      *
      */
      CFXXBoxBaseVertexShader::~CFXXBoxBaseVertexShader(void) 
      {
         RWS_FUNCTION("RWS::FX::CFXXBoxBaseVertexShader::~CFXXBoxBaseVertexShader");

         // if we have a pipeline the destory it
         if (m_AtomicPipe)
         {
            RxPipelineDestroy(m_AtomicPipe);
            m_AtomicPipe = 0;
         }

         RWS_RETURNVOID();

      }

      /**
      *  Creates the pipeline used to render the vertex shaded atomic.
      *
      *  \param *RxXboxAllInOneRenderCallBack A function to be called when the atomic is to be rendered.
      *
      *  \return Returns true if pipeline was successfully created or false if creation failed.
      */
      RwBool CFXXBoxBaseVertexShader::CreatePipeLine(RxXboxAllInOneRenderCallBack RenderCallBack)
      {
         RWS_FUNCTION("RWS::FX::CFXXBoxBaseVertexShader::CreatePipeLine");

         m_AtomicPipe = RxPipelineCreate();
         if (m_AtomicPipe)
         {
            RxLockedPipe    *lpipe;

            lpipe = RxPipelineLock(m_AtomicPipe);
            if (lpipe != 0)
            {
               RxNodeDefinition    *instanceNode;
               RxPipelineNode      *node;

               /*
                * Get the instance node definition
                */
               instanceNode = RxNodeDefinitionGetXboxAtomicAllInOne();

               /*
                * Add the node to the pipeline
                */
               lpipe = RxLockedPipeAddFragment(lpipe, 0, instanceNode, 0);

               /*
                * Unlock the pipeline
                */
               lpipe = RxLockedPipeUnlock(lpipe);

               /*
                * Set the pipeline specific data
                */
               node = RxPipelineFindNodeByName(m_AtomicPipe, instanceNode->name, 0, 0);

               /*
                * Cache the default render callback
                */
               DefaultRenderCallback = RxXboxAllInOneGetRenderCallBack(node);

               /*
                * Set the VShader render callback
                */
               RxXboxAllInOneSetRenderCallBack(node, RenderCallBack);


            }
         }

         RWS_RETURN(TRUE);
      }

 
      /**
      *  Creates the pipeline used to render the vertex shaded atomic, and associates the atomic with the
      *  vertex shader object.
      *
      *  \param *pAtomic Pointer to an atomic 
      *  \param *data    Pointer to the CFXXBoxBaseVertexShader object.
      *
      *  \return Returns pAtomic.
      */
      RpAtomic *CFXXBoxBaseVertexShader::SetAtomicVShaderPipeline(RpAtomic *pAtomic, void *data)
      {
         RWS_FUNCTION("RWS::FX::CFXXBoxBaseVertexShader::SetAtomicVShaderPipeline");

         // set the atomics pipeline to the new pipeline
         RpAtomicSetPipeline(pAtomic, reinterpret_cast<CFXXBoxBaseVertexShader *>(data)->m_AtomicPipe);

         // store a pointer to owner shader class, in the plugin area.
         SetVShader(pAtomic, static_cast<CFXXBoxBaseVertexShader*>(data));

         RWS_RETURN(pAtomic);
      }

      /**
      *  Sets up the vertex shader constants with all the matrices required during the transform and lighting of
      *  an atomic vertex shader.
      *
      *  \param *ltm pointer to a local transform matrix.
      *
      *  \return Returns void
      */
      void CFXXBoxBaseVertexShader::SetVertexShaderBaseDefs(RwMatrix *ltm)
      {
         RWS_FUNCTION("RWS::FX::CFXXBoxBaseVertexShader::SetVertexShaderBaseDefs");
         RwCamera    *camera;
         RwMatrix    *camLTM;
         RwMatrix    invLtm;
         RwMatrix    invCamMtx;

         D3DMATRIX   viewMatrix;        
         D3DMATRIX   worldMatrix;
         D3DMATRIX   projMatrix;
         D3DMATRIX   destMatrix;
         D3DMATRIX   tmpMatrix;
         D3DMATRIX   worldITMat;

         /*
          * View matrix - (camera matrix)
          */
         camera = RwCameraGetCurrentCamera();
         camLTM = RwFrameGetLTM(RwCameraGetFrame(camera));

         RwMatrixSetIdentity(&invCamMtx);
         RwMatrixInvert(&invCamMtx, camLTM);

         viewMatrix.m[0][0] = - invCamMtx.right.x;
         viewMatrix.m[0][1] = - invCamMtx.up.x;
         viewMatrix.m[0][2] = - invCamMtx.at.x;
         viewMatrix.m[0][3] = - invCamMtx.pos.x;

         viewMatrix.m[1][0] = invCamMtx.right.y;
         viewMatrix.m[1][1] = invCamMtx.up.y;
         viewMatrix.m[1][2] = invCamMtx.at.y;
         viewMatrix.m[1][3] = invCamMtx.pos.y;


         viewMatrix.m[2][0] = invCamMtx.right.z;
         viewMatrix.m[2][1] = invCamMtx.up.z;
         viewMatrix.m[2][2] = invCamMtx.at.z;
         viewMatrix.m[2][3] = invCamMtx.pos.z;

         viewMatrix.m[3][0] = 0.0f;
         viewMatrix.m[3][1] = 0.0f;
         viewMatrix.m[3][2] = 0.0f;
         viewMatrix.m[3][3] = 1.0f;

         /* 
          * World matrix
          */
         worldMatrix.m[0][0] = ltm->right.x;
         worldMatrix.m[0][1] = ltm->up.x;
         worldMatrix.m[0][2] = ltm->at.x;
         worldMatrix.m[0][3] = ltm->pos.x;

         worldMatrix.m[1][0] = ltm->right.y;
         worldMatrix.m[1][1] = ltm->up.y;
         worldMatrix.m[1][2] = ltm->at.y;
         worldMatrix.m[1][3] = ltm->pos.y;

         worldMatrix.m[2][0] = ltm->right.z;
         worldMatrix.m[2][1] = ltm->up.z;
         worldMatrix.m[2][2] = ltm->at.z;
         worldMatrix.m[2][3] = ltm->pos.z;

         worldMatrix.m[3][0] = 0.0f;
         worldMatrix.m[3][1] = 0.0f;
         worldMatrix.m[3][2] = 0.0f;
         worldMatrix.m[3][3] = 1.0f;

         /*
          * Projection matrix
          */
         projMatrix.m[0][0] = camera->recipViewWindow.x;
         projMatrix.m[0][1] = 0.0f;
         projMatrix.m[0][2] = 0.0f;
         projMatrix.m[0][3] = 0.0f;

         projMatrix.m[1][0] = 0.0f;
         projMatrix.m[1][1] = camera->recipViewWindow.y;
         projMatrix.m[1][2] = 0.0f;
         projMatrix.m[1][3] = 0.0f;

         projMatrix.m[2][0] = 0.0f;
         projMatrix.m[2][1] = 0.0f;
         projMatrix.m[2][2] = camera->farPlane / (camera->farPlane - camera->nearPlane);
         projMatrix.m[2][3] = - projMatrix.m[2][2] * camera->nearPlane;

         projMatrix.m[3][0] = 0.0f;
         projMatrix.m[3][1] = 0.0f;
         projMatrix.m[3][2] = 1.0f;
         projMatrix.m[3][3] = 0.0f;

         D3DXMatrixMultiply(reinterpret_cast<D3DXMATRIX*>(&tmpMatrix), reinterpret_cast<D3DXMATRIX*>(&viewMatrix),
                            reinterpret_cast<D3DXMATRIX*>(&worldMatrix));
         D3DXMatrixMultiply(reinterpret_cast<D3DXMATRIX*>(&destMatrix), reinterpret_cast<D3DXMATRIX*>(&projMatrix),
                            reinterpret_cast<D3DXMATRIX*>(&tmpMatrix));

         /*
          * Set the constant registers c0-c3 with the transformation matrix
          */
         D3DDevice_SetVertexShaderConstant(VSCONST_REG_TRANSFORM_OFFSET, (void *)&destMatrix,
                                           VSCONST_REG_TRANSFORM_SIZE);
         D3DDevice_SetVertexShaderConstant(VSCONST_REG_WORLD_TRANSFORM_OFFSET, (void *)&worldMatrix,
                                           VSCONST_REG_WORLD_TRANSFORM_SIZE);

         RwMatrixInvert(&invLtm, ltm);

         worldITMat.m[0][0] = invLtm.right.x;
         worldITMat.m[0][1] = invLtm.right.y;
         worldITMat.m[0][2] = invLtm.right.z;
         worldITMat.m[0][3] = 0.0f;

         worldITMat.m[1][0] = invLtm.up.x;
         worldITMat.m[1][1] = invLtm.up.y;
         worldITMat.m[1][2] = invLtm.up.z;
         worldITMat.m[1][3] = 0.0f;

         worldITMat.m[2][0] = invLtm.at.x;
         worldITMat.m[2][1] = invLtm.at.y;
         worldITMat.m[2][2] = invLtm.at.z;
         worldITMat.m[2][3] = 0.0f;

         worldITMat.m[3][0] = invLtm.pos.x;
         worldITMat.m[3][1] = invLtm.pos.y;
         worldITMat.m[3][2] = invLtm.pos.z;
         worldITMat.m[3][3] = 1.0f;

         D3DDevice_SetVertexShaderConstant(VSCONST_REG_WORLD_INVERSE_OFFSET, (void *)&worldITMat,
                                           VSCONST_REG_WORLD_INVERSE_SIZE);

         RWS_RETURNVOID();
      }
      

      /**
      *  Used as a render call back during rendering of atomics with vertex shaders. From the geometry plugin
      *  data that is attached to the atomic a pointer to the vertex shader object is extracted, this is then
      *  used to call the vertex shader objects VShaderRenderCallBack.
      *  \param *resEntryHeader  Pointer to a resource entry.                           
      *  \param *object          Pointer to a object that the resource entry belongs to.
      *  \param type             rpATOMIC or rwSECTORATOMIC.                            
      *  \param flags            RpGeometryFlag's or RpWorldFlag's.                     
      *
      *  \return Returns void.
      */
      void CFXXBoxBaseVertexShader::ProcessVertexShader(RxXboxResEntryHeader *resEntryHeader, void *object,
                                                        RwUInt8 type, RwUInt32 flags)
      {
         RWS_FUNCTION("RWS::FX::CFXXBoxBaseVertexShader::ProcessVertexShader");
         
         // get a pointer to the class which owns the vertex shader
         CFXXBoxBaseVertexShader *ShaderClass = GetVShader(static_cast<RpAtomic *>(object));

         // call the render call back belonging to ShaderClass to render the geometry
         ShaderClass->VShaderRenderCallBack(resEntryHeader, object, type, flags);

         RWS_RETURNVOID();
      }
   } // namespace FX
} //namespace RWS
