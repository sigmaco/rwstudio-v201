/*****************************************************************************
 *
 * File :     CFXFlagShader.cpp
 *
 * Abstract : An XBox behavior for rendering animated flags using a vertex shader.
 *            Sine waves are applied to each vertex based on the attributes setup
 *            in workspace. Each vertex has a weight value that is specified by
 *            changing each of the vertices alpha value. Setting the alpha to zero
 *            for example allows no movement, while setting a value of 255 allows
 *            the sine waves to have maximum effect. In the example the flagpole
 *            has an alpha value of 0 and so is not animated by the vertex shader.
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
#include "framework/core/macros/debugmacros.h"
#include "framework/toolkits/world/clevel.h"
#include "framework/toolkits/math/maths.h"
#include "framework/mainloop/render.h"
#include "framework/core/resourcemanager/cresourcemanager.h"
#include "framework/core/attributehandler/cattributehandler.h"

#include "framework/core/attributehandler/allocation policies/LinearAllocationPolicy.h"

#include "../cfxxboxvertexshaderbase.h"
#include "cfx_xbox_flagshader.h"

#include "flagdefs.h"
#include "flagtextured.h"
#include "flagtexturedvertexcolor.h"
#include "flagvertexcolor.h"

namespace RWS
{

   namespace FX
   {

      RWS_IMPLEMENT_CLASSID(CFXFlagShader);
      RWS_REGISTERCLASS(CFXFlagShader);


      // structure that is filled out during the light callback
      typedef struct
      {
         RwRGBAReal  Color;
         RwBool      Found;   
         RwV3d       Pos;
      }t_LIGHTCALLBACKRESULT;

      /**
      *
      *  Construct a CFXFlagShader object.
      *
      */
      CFXFlagShader::CFXFlagShader(const CAttributePacket& rAttr) : InitCEventHandler(0),
      m_VertexShaderTextured(0),
      m_VertexShaderVertexColor(0),
      m_VertexShaderTexturedVertexColor(0)
      {
         RWS_FUNCTION("RWS::FX::CFXFlagShader::CFXFlagShader");

         LinkMsg(iMsgDoRender, "RwCamera*");

         m_HfreqA       = 0;
         m_HspeedA      = 0;
         m_HamplitudeA  = 0;
         m_HfreqB       = 0;
         m_HspeedB      = 0;
         m_HamplitudeB  = 0;
         m_VfreqA       = 0;
         m_VspeedA      = 0;
         m_VamplitudeA  = 0;

         m_WaveOffset.x = 0;
         m_WaveOffset.y = 0;
         m_WaveOffset.z = 0;
         m_WaveOffset.w = 0;

         CreatePipeLine(ProcessVertexShader);
         CreateVertexShaders();

         RWS_RETURNVOID();
      }

      /**
      *
      *  Destroy a CFXFlagShader object.
      *
      */
      CFXFlagShader::~CFXFlagShader(void) 
      {
         RWS_FUNCTION("RWS::FX::CFXFlagShader::~CFXFlagShader");

         UnLinkMsg(iMsgDoRender);

         DestroyVertexShaders();

         RWS_RETURNVOID();
      }


      /**
      * Creates the vertex shaders from their declarations.
      * \param void
      * \return               Returns void. 
      *
      */
      RwBool CFXFlagShader::CreateVertexShaders(void)
      {
         RWS_FUNCTION("RWS::FX::CFXFlagShader::CreateVertexShaders");

         /*
          * Create the vertex shader
          */
         if (D3D_OK != D3DDevice_CreateVertexShader(TexturedDeclaration, dwFlagTexturedVertexShader,
                                                    &m_VertexShaderTextured, 0))
         {
             RWS_RETURN(FALSE);
         }

         /*
          * Create the vertex shader
          */
         if (D3D_OK != D3DDevice_CreateVertexShader(VertexColorDeclaration, dwFlagVertexColorVertexShader,
                                                    &m_VertexShaderVertexColor, 0))
         {
             RWS_RETURN(FALSE);
         }

         /*
          * Create the vertex shader
          */
         if (D3D_OK != D3DDevice_CreateVertexShader(TexturedVertexColorDeclaration,
                                                    dwFlagTexturedVertexColorVertexShader,
                                                    &m_VertexShaderTexturedVertexColor, 0))
         {
             RWS_RETURN(FALSE);
         }


         RWS_RETURN(TRUE);
      }

      /**
      * Destroys the vertex shaders
      * \param void
      * \return      Returns void. 
      *
      */
      void CFXFlagShader::DestroyVertexShaders(void)
      {
         RWS_FUNCTION("RWS::FX::CFXFlagShader::DestroyVertexShaders");


         RwXboxSetCurrentVertexShader(0);
         if (m_VertexShaderTextured)
         {
            D3DDevice_DeleteVertexShader(m_VertexShaderTextured);
         }

         if (m_VertexShaderVertexColor)
         {
            D3DDevice_DeleteVertexShader(m_VertexShaderVertexColor);
         }

         if (m_VertexShaderTexturedVertexColor)
         {
            D3DDevice_DeleteVertexShader(m_VertexShaderTexturedVertexColor);
         }

         RWS_RETURNVOID();
      }


      /**
      * The light call back that is used to fill in a t_LIGHTCALLBACKRESULT structure with details of the
      * first ambient light found in the world.
      * \param *pLight  Pointer to an RpLight.
      * \param *pData   Pointer to an t_LIGHTCALLBACKRESULT structure that is filled out with a 
      *                 description of the light.
      * \return         Pointer to pLight or NULL if the light is an ambient light (terminate the callback).
      *
      */
      RpLight *CFXFlagShader::AmbientLightCallBack(RpLight *pLight, void *pData)
      {
         RWS_FUNCTION("RWS::FX::CFXFlagShader::AmbientLightCallBack");

         if (RpLightGetType(pLight) == rpLIGHTAMBIENT)
         {
            t_LIGHTCALLBACKRESULT *Result = reinterpret_cast<t_LIGHTCALLBACKRESULT*>(pData);
            RwFrame               *LightFrame;
            RwMatrix              *LightMatrix;

            LightFrame    = RpLightGetFrame(pLight);
            LightMatrix   = RwFrameGetLTM(LightFrame);


            Result->Color = *RpLightGetColor(pLight);
            Result->Pos   = *RwMatrixGetPos(LightMatrix);
            Result->Found = TRUE;

            RWS_RETURN(0);
         }


         RWS_RETURN(pLight);
      }

      /**
      * The light call back that is used to fill in a t_LIGHTCALLBACKRESULT structure with details of the
      * first point light found in the world.
      * \param *pLight  Pointer to an RpLight.
      * \param *pData   Pointer to an t_LIGHTCALLBACKRESULT structure that is filled out with a 
      *                 description of the light.
      * \return         Pointer to pLight or NULL if the light is a point light (terminate the callback).
      *
      */
      RpLight *CFXFlagShader::PointLightCallBack(RpLight *pLight, void *pData)
      {
         RWS_FUNCTION("RWS::FX::CFXFlagShader::PointLightCallBack");

         if (RpLightGetType(pLight) == rpLIGHTPOINT)
         {
            reinterpret_cast<t_LIGHTCALLBACKRESULT*>(pData)->Color = *RpLightGetColor(pLight);
            reinterpret_cast<t_LIGHTCALLBACKRESULT*>(pData)->Found = TRUE;
            RWS_RETURN(0);
         }


         RWS_RETURN(pLight);
      }

      /**
      *  Sets up the constant registers for use in the vertex shader, the constants needed to transform
      *  and light each vertex are setup along with the constants need to apply the wave effect.
      * \param *ltm     Pointer to a local transform matrix for the atomic to be rendered.
      * \param *pWorld  Pointer to the world which contains the lights. 
      * \return         Returns void.
      *
      */
      void CFXFlagShader::SetFlagConstantRegisters(RwMatrix *ltm, RpWorld *pWorld)
      {
         RWS_FUNCTION("RWS::FX::CFXFlagShader::SetFlagConstantRegisters");

         // setup common vertex shader constants. i.e.. Camera and local transform matrix
         SetVertexShaderBaseDefs(ltm);

         const RwReal PI = 3.14f;

         RwMatrix invLtm;
         RwV3d    LightPos             =  {1.0f, 0.0f, 0.0f};
         RwV4d    TransformedLightPos  =  {0.0f, 0.0f, 0.0f, 0.0f};
         RwV4d    One                  =  {1.0f, 1.0f, 1.0f, 1.0f};
         RwV4d    Zero                 =  {0.0f, 0.0f, 0.0f, 0.0f};
         RwV4d    Half                 =  {0.5f, 0.5f, 0.5f, 0.5f};
         RwV4d    TSeries[3]           =  {{PI,    1.f / 2.f, 2.f * PI,   1.f / (2.f * PI)},
                                          {1.0f, -1.f / 2.f, 1.f / 24.f,  -1.f / 720.f},
                                          {1.0f, -1.f / 6.f, 1.f / 120.f, -1.f / 5040.f}
                                          };

         t_LIGHTCALLBACKRESULT   LightRes;

         // get the ambient and set the light position
         LightRes.Found = FALSE;
         RpWorldForAllLights(pWorld, AmbientLightCallBack, &LightRes);
         if (LightRes.Found)
         {
            D3DDevice_SetVertexShaderConstant(VSCONST_REG_MAT_AMB_OFFSET, (void *)&LightRes.Color,
                                              VSCONST_REG_MAT_AMB_SIZE);
         }
            else
         {
            D3DDevice_SetVertexShaderConstant(VSCONST_REG_MAT_AMB_OFFSET, (void *)&Zero, VSCONST_REG_MAT_AMB_SIZE);
         }

         // get the first point light and set it's position and colour
         LightRes.Found = FALSE;
         RpWorldForAllLights(pWorld, PointLightCallBack, &LightRes);
         if (LightRes.Found)
         {
            // transform light position into object space
            RwMatrixInvert(&invLtm, ltm); 

            TransformedLightPos.x = (LightRes.Pos.x * invLtm.right.x) + (LightRes.Pos.y * invLtm.up.x) + (LightRes.Pos.z * invLtm.at.x) + invLtm.pos.x;
            TransformedLightPos.y = (LightRes.Pos.x * invLtm.right.y) + (LightRes.Pos.y * invLtm.up.y) + (LightRes.Pos.z * invLtm.at.y) + invLtm.pos.y;
            TransformedLightPos.z = (LightRes.Pos.x * invLtm.right.z) + (LightRes.Pos.y * invLtm.up.z) + (LightRes.Pos.z * invLtm.at.z) + invLtm.pos.z;

            D3DDevice_SetVertexShaderConstant(VSCONST_REG_LIGHT_POS_OFFSET, (void *)&TransformedLightPos,
                                              VSCONST_REG_LIGHT_POS_SIZE);
         }
            else
         {
            D3DDevice_SetVertexShaderConstant(VSCONST_REG_LIGHT_POS_OFFSET, (void *)&LightPos,
                                              VSCONST_REG_LIGHT_POS_SIZE);
         }

         D3DDevice_SetVertexShaderConstant(VSCONST_REG_ONE_OFFSET,     (void *)&One,          VSCONST_REG_ONE_SIZE);
         D3DDevice_SetVertexShaderConstant(VSCONST_REG_ZERO_OFFSET,    (void *)&Zero,         VSCONST_REG_ZERO_SIZE);
         D3DDevice_SetVertexShaderConstant(VSCONST_REG_TSERIES_OFFSET, (void *)&TSeries,      VSCONST_REG_TSERIES_SIZE);
         D3DDevice_SetVertexShaderConstant(VSCONST_REG_TIME_OFFSET,    (void *)&m_WaveOffset, VSCONST_REG_TIME_SIZE);


         RwV4d ConstVal;

         ConstVal.x = ConstVal.y = ConstVal.z = m_HfreqA; 
         D3DDevice_SetVertexShaderConstant(VSCONST_REG_HFREQA_OFFSET,      (void *)&ConstVal,
                                           VSCONST_REG_HFREQA_SIZE);
                                                                          
         ConstVal.x = ConstVal.y = ConstVal.z = m_HspeedA; 
         D3DDevice_SetVertexShaderConstant(VSCONST_REG_HSPEEDA_OFFSET,     (void *)&ConstVal,
                                           VSCONST_REG_HSPEEDA_SIZE);

         ConstVal.x = ConstVal.y = ConstVal.z = m_HamplitudeA; 
         D3DDevice_SetVertexShaderConstant(VSCONST_REG_HAMPLITUDEA_OFFSET, (void *)&ConstVal,
                                           VSCONST_REG_HAMPLITUDEA_SIZE);

         ConstVal.x = ConstVal.y = ConstVal.z = m_HfreqB; 
         D3DDevice_SetVertexShaderConstant(VSCONST_REG_HFREQB_OFFSET,      (void *)&ConstVal,
                                           VSCONST_REG_HFREQB_SIZE);

         ConstVal.x = ConstVal.y = ConstVal.z = m_HspeedB; 
         D3DDevice_SetVertexShaderConstant(VSCONST_REG_HSPEEDB_OFFSET,     (void *)&ConstVal,
                                           VSCONST_REG_HSPEEDB_SIZE);

         ConstVal.x = ConstVal.y = ConstVal.z = m_HamplitudeB; 
         D3DDevice_SetVertexShaderConstant(VSCONST_REG_HAMPLITUDEB_OFFSET, (void *)&ConstVal,
                                           VSCONST_REG_HAMPLITUDEB_SIZE);

         ConstVal.x = ConstVal.y = ConstVal.z = m_VfreqA; 
         D3DDevice_SetVertexShaderConstant(VSCONST_REG_VFREQA_OFFSET,      (void *)&ConstVal,
                                           VSCONST_REG_VFREQA_SIZE);

         ConstVal.x = ConstVal.y = ConstVal.z = m_VspeedA; 
         D3DDevice_SetVertexShaderConstant(VSCONST_REG_VSPEEDA_OFFSET,     (void *)&ConstVal,
                                           VSCONST_REG_VSPEEDA_SIZE);

         ConstVal.x = ConstVal.y = ConstVal.z = m_VamplitudeA; 
         D3DDevice_SetVertexShaderConstant(VSCONST_REG_VAMPLITUDEA_OFFSET, (void *)&ConstVal,
                                           VSCONST_REG_VAMPLITUDEA_SIZE);

         RWS_RETURNVOID();
      }

      /**
      * Called by RenderWare to render the Atomic pointed to by pObject.
      *
      * \param *resEntryHeader   Pointer to a resource entry.
      * \param *object           Pointer to a object that the resource entry belongs to.
      * \param type              rpATOMIC or rwSECTORATOMIC.
      * \param flags             RpGeometryFlag's or RpWorldFlag's.
      * \return                  Returns void.
      *
      */
      void CFXFlagShader::VShaderRenderCallBack(RxXboxResEntryHeader *resEntryHeader, void *pObject, RwUInt8 Type,
                                                RwUInt32 Flags)
      {
         RWS_FUNCTION("RWS::FX::CFXFlagShader::VShaderRenderCallBack");
         RpWorld              *World;
         RpGeometry           *Geom;
         RwUInt32             GeomFlags;
         RxXboxInstanceData   *instancedMesh;
         DWORD                oldAddressU,   oldAddressV,
                              oldMinFilter,  oldMagFilter, oldMipFilter,
                              oldColorArg1,  oldColorOp,   oldColorArg2,
                              oldAlphaEnable, oldAlphaTest, oldSrcBlend, oldDestBlend, oldCullMode;

         Geom      = RpAtomicGetGeometry(reinterpret_cast<RpAtomic*>(pObject));  
         GeomFlags = RpGeometryGetFlags(Geom);
         World     = RpAtomicGetWorld(reinterpret_cast<RpAtomic*>(pObject));

          /*
          * Set up vertex shader with required constants
          */
         SetFlagConstantRegisters(RwFrameGetLTM(RpAtomicGetFrame((RpAtomic *)pObject)), World);

         /*
          * Note previous renderstates on texture stage 0 so we can restore them later.
          * RenderWare caches renderstates for best performance and expects the hardware
          * to be in the same state as we found it.
          */
         
         RwXboxGetCachedTextureStageState(0, D3DTSS_ADDRESSU,  &oldAddressU );
         RwXboxGetCachedTextureStageState(0, D3DTSS_ADDRESSV,  &oldAddressV );
         RwXboxGetCachedTextureStageState(0, D3DTSS_MINFILTER, &oldMinFilter);
         RwXboxGetCachedTextureStageState(0, D3DTSS_MAGFILTER, &oldMagFilter);
         RwXboxGetCachedTextureStageState(0, D3DTSS_MIPFILTER, &oldMipFilter);
         RwXboxGetCachedTextureStageState(0, D3DTSS_COLORARG1, &oldColorArg1);
         RwXboxGetCachedTextureStageState(0, D3DTSS_COLOROP,   &oldColorOp);
         RwXboxGetCachedTextureStageState(0, D3DTSS_COLORARG2, &oldColorArg2);

         RwXboxGetCachedRenderState(D3DRS_ALPHABLENDENABLE,    &oldAlphaEnable);
         RwXboxGetCachedRenderState(D3DRS_ALPHATESTENABLE,     &oldAlphaTest);
         RwXboxGetCachedRenderState(D3DRS_SRCBLEND,            &oldSrcBlend);
         RwXboxGetCachedRenderState(D3DRS_DESTBLEND,           &oldDestBlend);
         RwXboxGetCachedRenderState(D3DRS_CULLMODE,            &oldCullMode);

         /*
          * Set up texture stages                      
          */

         RwXboxSetCachedTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
         RwXboxSetCachedTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
         RwXboxSetCachedTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
         
         /* turn off other texture stages */
         RwXboxSetCachedTextureStageState(1, D3DTSS_COLOROP,   D3DTOP_DISABLE);

         /* turn off alpha blending */
         RwXboxSetCachedRenderState(D3DRS_ALPHABLENDENABLE,    FALSE);
         RwXboxSetCachedRenderState(D3DRS_SRCBLEND,            D3DBLEND_ONE );
         RwXboxSetCachedRenderState(D3DRS_DESTBLEND,           D3DBLEND_ZERO );
         RwXboxSetCachedRenderState(D3DRS_CULLMODE,            D3DCULL_NONE );


         /*
          * Set the stream source - shared by all meshes in geometry
          */
         D3DDevice_SetStreamSource(0, (D3DVertexBuffer *)resEntryHeader->vertexBuffer, resEntryHeader->stride);


         /* for each mesh in geometry */
         for (instancedMesh = resEntryHeader->begin;
              instancedMesh != resEntryHeader->end;
              ++instancedMesh)
         {

            /*
              * Draw the indexed primitive - note primitive type is same for all meshes
              * in geometry
              */
            D3DCOLOR MatColor;

            MatColor = D3DCOLOR_ARGB(  instancedMesh->material->color.alpha,
                                       instancedMesh->material->color.red,
                                       instancedMesh->material->color.green,
                                       instancedMesh->material->color.blue);
            {
               RwV4d MatColour;
               MatColour.x = instancedMesh->material->color.red / 255.0f;
               MatColour.y = instancedMesh->material->color.green / 255.0f;
               MatColour.z = instancedMesh->material->color.blue / 255.0f;
               MatColour.w = instancedMesh->material->color.alpha / 255.0f;
               D3DDevice_SetVertexShaderConstant(VSCONST_REG_MAT_COLOR_OFFSET, (void *)&MatColour,
                                                 VSCONST_REG_MAT_COLOR_SIZE);
            }


            if (instancedMesh->material->texture)
            {
               RwXboxRenderStateSetTexture(instancedMesh->material->texture, 0);
            }
              else
            {
              RwXboxRenderStateSetTexture(0, 0);
            }

            if (GeomFlags&rpGEOMETRYTEXTURED)
            {
               // textured
               if (GeomFlags&rpGEOMETRYPRELIT)
               {
                  RwXboxSetCurrentVertexShader(m_VertexShaderTexturedVertexColor);
               }
               else
               {
                  RwXboxSetCurrentVertexShader(m_VertexShaderTextured);
               }
            }
               else
            {
               // non textured
               // textured
               if (GeomFlags&rpGEOMETRYPRELIT)
               {
                  RwXboxSetCurrentVertexShader(m_VertexShaderVertexColor);
               }
                  else
               {
                  RwXboxSetCurrentVertexShader(m_VertexShaderTextured);
               }
            }

            RwXboxDrawIndexedVertices((D3DPRIMITIVETYPE)resEntryHeader->primType,
                                        instancedMesh->numIndices,
                                        instancedMesh->indexBuffer);
         }

         /*
          * Restore renderstates
          */
         RwXboxRenderStateSetTexture(0, 0);
         RwXboxRenderStateSetTexture(0, 1);
         RwXboxSetCachedTextureStageState(0, D3DTSS_ADDRESSU,  oldAddressU);
         RwXboxSetCachedTextureStageState(0, D3DTSS_ADDRESSV,  oldAddressV);
         RwXboxSetCachedTextureStageState(0, D3DTSS_MINFILTER, oldMinFilter);
         RwXboxSetCachedTextureStageState(0, D3DTSS_MAGFILTER, oldMagFilter);
         RwXboxSetCachedTextureStageState(0, D3DTSS_MIPFILTER, oldMipFilter);
         RwXboxSetCachedTextureStageState(0, D3DTSS_COLORARG1, oldColorArg1);
         RwXboxSetCachedTextureStageState(0, D3DTSS_COLOROP,   oldColorOp);
         RwXboxSetCachedTextureStageState(0, D3DTSS_COLORARG2, oldColorArg2);
         RwXboxSetCachedTextureStageState(1, D3DTSS_COLOROP,   D3DTOP_DISABLE);
         RwXboxSetCachedRenderState(D3DRS_ALPHABLENDENABLE,    oldAlphaEnable);
         RwXboxSetCachedRenderState(D3DRS_SRCBLEND,            oldSrcBlend);
         RwXboxSetCachedRenderState(D3DRS_DESTBLEND,           oldDestBlend);
         RwXboxSetCachedRenderState(D3DRS_CULLMODE,            oldCullMode);

         RWS_RETURNVOID();
      }

      /**
      *
      * \ref HandleEvents(). Handle events.  Upon receiving a iMsgDoRender event the offset in the
      *                      sine wave is increased 
      *  
      *
      * \param pMsg Standard Message Package
      *  
      * \ref iMsgRunningTick
      */
      void CFXFlagShader::HandleEvents(CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::FX::CFXFlagShader::HandleEvents");

         if (pMsg.Id == iMsgDoRender)
         {
            m_WaveOffset.x += m_HspeedA;
            if (m_WaveOffset.x * m_HfreqA > 6.28f)  
            {
               m_WaveOffset.x = 0.0f;  
            }               
 
            m_WaveOffset.y += m_HspeedB;
            if (m_WaveOffset.y * m_HfreqB > 6.28f)  
            {
               m_WaveOffset.y = 0.0f;  
            }               

            m_WaveOffset.z += m_VspeedA;
            if (m_WaveOffset.z * m_VfreqA > 6.28f)  
            {
               m_WaveOffset.z = 0.0f;  
            }               
         }

         RWS_RETURNVOID();
      }

      /**
      *
      *  Update attributes.
      *
      */
      void CFXFlagShader::HandleAttributes(const CAttributePacket& attr)
      {
         RWS_FUNCTION("RWS::FX::CFXFlagShader::HandleAttributes");
         
         CAttributeHandler::HandleAttributes(attr);
         
         CAttributeCommandIterator sysattrIt(attr, RWS_CLASSID_OF(CSystemCommands));
         while (!sysattrIt.IsFinished())
         {
            switch (sysattrIt->GetCommandId())
            {
               case CSystemCommands::CMD_AttachResource:
                  {
                     const RWSGUID * pResourceId = 0;
                     sysattrIt->GetCommandData(&pResourceId);

                     const RwChar *resourceType;
                     const void *pObject = 0;
                     if (pResourceId)
                     {
                        pObject = CResourceManager::FindById(pResourceId, &resourceType);
                     }

                     RWS_ASSERT(pObject, "Resource::Find failed for id = " << pResourceId);

                     if (resourceType)
                     {
                        if (!rwstrcmp("rwID_ATOMIC", resourceType))
                        {
                           SetAtomicVShaderPipeline(const_cast<RpAtomic*>(reinterpret_cast<const RpAtomic *>(pObject)),
                                                    reinterpret_cast<void *>(this));
                        }
                           else if (!rwstrcmp("rwID_CLUMP", resourceType))
                        {
                           RpClumpForAllAtomics(const_cast<RpClump*>(reinterpret_cast<const RpClump *>(pObject)),
                                                SetAtomicVShaderPipeline, reinterpret_cast<void *>(this));
                        }
                     }
                  }
                  break;
            }; 
            ++sysattrIt;
         }

         // deal with the attributes related to the environment map behaviour
         CAttributeCommandIterator attrIt(attr, RWS_CLASSID_OF(CFXFlagShader));
         
         while (!attrIt.IsFinished())
         {
            switch (attrIt->GetCommandId())
            {

               case CMD_Set_m_HfreqA:
                  attrIt->GetCommandData(m_HfreqA);
                  RWS_TRACE("CMD_Set_m_HfreqA "<<m_HfreqA);
                  break;
               case CMD_Set_m_HamplitudeA:
                  attrIt->GetCommandData(m_HamplitudeA);
                  RWS_TRACE("CMD_Set_m_HamplitudeA "<<m_HamplitudeA);
                  break;
               case CMD_Set_m_HspeedA:
                  attrIt->GetCommandData(m_HspeedA);
                  RWS_TRACE("CMD_Set_m_HspeedA "<<m_HspeedA);
                  break;


               case CMD_Set_m_HfreqB:
                  attrIt->GetCommandData(m_HfreqB);
                  RWS_TRACE("CMD_Set_m_HfreqB "<<m_HfreqB);
                  break;
               case CMD_Set_m_HamplitudeB:
                  attrIt->GetCommandData(m_HamplitudeB);
                  RWS_TRACE("CMD_Set_m_HamplitudeB "<<m_HamplitudeB);
                  break;
               case CMD_Set_m_HspeedB:
                  attrIt->GetCommandData(m_HspeedB);
                  RWS_TRACE("CMD_Set_m_HspeedB "<<m_HspeedB);
                  break;


               case CMD_Set_m_VfreqA:
                  attrIt->GetCommandData(m_VfreqA);
                  RWS_TRACE("CMD_Set_m_VfreqA "<<m_VfreqA);
                  break;
               case CMD_Set_m_VamplitudeA:
                  attrIt->GetCommandData(m_VamplitudeA);
                  RWS_TRACE("CMD_Set_m_VamplitudeA "<<m_VamplitudeA);
                  break;
               case CMD_Set_m_VspeedA:
                  attrIt->GetCommandData(m_VspeedA);
                  RWS_TRACE("CMD_Set_m_VspeedA "<<m_VspeedA);
                  break;
            }
            ++attrIt;
         }
         RWS_RETURNVOID();
      }
   } // namespace FX
} //namespace RWS

