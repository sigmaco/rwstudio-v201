/*****************************************************************************
 *
 * File :     cfxrefectenvironment.cpp
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

#include "precomp.h"
// Left to its own devices, the MW linker will dead-strip the registrar ctors, so...
#ifdef __MWERKS__
#pragma force_active on
#pragma exceptions off // Turn off exceptions due to bug in MW Dwarf Generator for PS2, present in CW 3.04
#endif

#include "rpmatfx.h"

#include "framework/toolkits/world/clevel.h"
#include "cfxreflectenvironment.h"
#include "framework/core/resourcemanager/cresourcemanager.h"

namespace RWS
{                    

   namespace FX
   {

      RWS_IMPLEMENT_CLASSID(CFXReflectEnvironment);
      RWS_REGISTERCLASS(CFXReflectEnvironment);

      /**
      *
      *  Construct a CFXReflectEnvironment object.
      *
      */
      CFXReflectEnvironment::CFXReflectEnvironment(const CAttributePacket& rAttr) : CFXEnvironmentMap(rAttr)
      {
         RWS_FUNCTION("RWS::FX::CFXReflectEnvironment::CFXReflectEnvironment");

         RegisterMsg(m_UpdateMessage, "iMsgDoRender", "RwCamera*");
         LinkMsg(m_UpdateMessage, "RwCamera*");

         RegisterMsg(m_ShowEnvMapMessage, "iMsgDoRender", "RwCamera*");
         LinkMsg(m_ShowEnvMapMessage, "RwCamera*");

         // setup the environment map
         // this needs to be called here so that m_MatList is built up.
         ProcessAttachments(rAttr, m_pEnvFrame);

         // create the off screen raster which will be used as an environment map
         m_Raster  = RwRasterCreate(static_cast<RwUInt32>(ENV_MAP_WIDTH), static_cast<RwUInt32>(ENV_MAP_HEIGHT), 32,
                                    rwRASTERTYPECAMERATEXTURE);
         m_Texture = RwTextureCreate( m_Raster );

#if defined(SKY)
         RpSkyTexCacheRasterLock( m_Raster, TRUE );

#endif
         // set the environment map textures pointer to by m_MatList to the sphere environment map we will
         // generate
         t_SET_TEXTURE_CALLBACK_INFO CallBackData;

         CallBackData.pTexture = m_Texture;
         m_MatList.ForAllMaterials(SetTexture, &CallBackData);         

         RWS_RETURNVOID();
      }

      /**
      *
      *  Destroy a CFXReflectEnvironment object.
      *
      */
      CFXReflectEnvironment::~CFXReflectEnvironment()
      {
         RWS_FUNCTION("RWS::FX::CFXReflectEnvironment::~CFXReflectEnvironment");

         UnLinkMsg(m_UpdateMessage);
         UnRegisterMsg(m_UpdateMessage);

         UnLinkMsg(m_ShowEnvMapMessage);
         UnRegisterMsg(m_ShowEnvMapMessage);

#if defined(SKY)
         RpSkyTexCacheRasterLock(m_Raster, FALSE);

#endif
         RwTextureDestroy ( m_Texture );

         RWS_RETURNVOID();
      }

      /**
      *
      * Responds to events sent to update the environment texture and display the environment map texture in the
      * preview window.
      *  
      * \param pMsg Standard Message Package
      *  
      */
      void CFXReflectEnvironment::HandleEvents (CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::FX::CFXReflectEnvironment::HandleEvents");

         if (pMsg.Id == m_ShowEnvMapMessage)
         {
            const RwV2d Vx[]  = { {0.83f, 0.02f}, {0.98f, 0.17f} };
            const RwV2d Uvs[] = { {0.0f,  0.0f }, {1.0f,  1.0f } };

            EnvMapRasterRender(m_Raster, Vx, Uvs, reinterpret_cast<RwCamera*>(pMsg.pData));
         }

         if (pMsg.Id == m_UpdateMessage)
         {
            // update the environment map texture
            UpdateTextureFromDisplay( m_TopLeft, m_BottomRight, m_Raster );
         }

         CFXEnvironmentMap::HandleEvents(pMsg);  

         RWS_RETURNVOID();
      }


      /**
      *
      * \ref HandleAttributes. Handle attribute updates.
      *
      * \param Attr reference to a CAttributePacket.
      *
      */   
      void CFXReflectEnvironment::HandleAttributes  (const CAttributePacket& attr)
      {
         RWS_FUNCTION("RWS::FX::CFXReflectEnvironment::HandleAttributes");

         CAttributeHandler::HandleAttributes(attr);

         // deal with the attributes related to the environment map behaviour
         CAttributeCommandIterator attrIt(attr, RWS_CLASSID_OF(CFXReflectEnvironment));
         
         while (!attrIt.IsFinished())
         {
            switch (attrIt->GetCommandId())
            {

               case CMD_UpdateMap:
               {
                  // If already linked, unlink and unregister as we are about to register with
                  // a new event.
                  UnLinkMsg(m_UpdateMessage);
                  UnRegisterMsg(m_UpdateMessage);
                  // register the new event
                  RegisterMsg(m_UpdateMessage, attrIt->GetAs_RwChar_ptr(), "RwCamera*");
                  LinkMsg(m_UpdateMessage, "RwCamera*");
                  break;
               }

               case CMD_ShowEnvMap:
               {
                  // If already linked, unlink and unregister as we are about to register with
                  // a new event.
                  UnLinkMsg(m_ShowEnvMapMessage);
                  UnRegisterMsg(m_ShowEnvMapMessage);
                  // register the new event
                  RegisterMsg(m_ShowEnvMapMessage, attrIt->GetAs_RwChar_ptr(), "RwCamera*");
                  LinkMsg(m_ShowEnvMapMessage, "RwCamera*");
                  break;
               }

               case CMD_Set_X1:
               {
                  m_TopLeft.x = attrIt->GetAs_RwReal();
                  break;
               }

               case CMD_Set_Y1:
               {
                  m_TopLeft.y = attrIt->GetAs_RwReal();
                  break;
               }

               case CMD_Set_X2:
               {
                  m_BottomRight.x = attrIt->GetAs_RwReal();
                  break;
               }

               case CMD_Set_Y2:
               {
                  m_BottomRight.y = attrIt->GetAs_RwReal();
                  break;
               }

            }
           ++attrIt;
         }

         CFXEnvironmentMap::HandleAttributes( attr);


         RWS_RETURNVOID();
      }

      /**
      *
      * Checks to see if pMaterial contains an environment map effect, if it does then the texture used for the
      * environment map effect is set to the pTexture member of the t_SET_TEXTURE_CALLBACK_INFO structure pointed to by
      * pCallBackData.
      *
      * \param pMaterial      Pointer to the material to set environment map.
      * \param pCallBackData  Pointer to a t_SET_TEXTURE_CALLBACK_INFO structure containing the callback data.
      *
      */   
      RpMaterial *CFXReflectEnvironment::SetTexture(RpMaterial *pMaterial, void *pCallBackData)
      {
         RWS_FUNCTION("RWS::FX::CFXReflectEnvironment::HandleAttributes");

         RpMatFXMaterialFlags Flags;

         Flags = RpMatFXMaterialGetEffects(pMaterial);

         if (Flags == rpMATFXEFFECTENVMAP || Flags == rpMATFXEFFECTBUMPENVMAP)
         {

            t_SET_TEXTURE_CALLBACK_INFO *pInfo = static_cast<t_SET_TEXTURE_CALLBACK_INFO*>(pCallBackData);

            // assign the new texture to the material
            RpMatFXMaterialSetEnvMapTexture (pMaterial, pInfo->pTexture);
         }

         RWS_RETURN(pMaterial);
      }


      /**
      *  Renders a 2D quad with the current render states to the back buffer
      * 
      *
      *  \param x1          Top left x coordinate of quad.
      *  \param y1          Top left y coordinate of quad.
      *  \param x2          Bottom right x coordinate of quad.
      *  \param y2          Bottom right y coordinate of quad.
      *  \param z           Quads z value.
      *  \param recipCamZ   Quads reciprocal z value.
      *  \param pUvs        Pointer to an array of two RwV2d vectors containing the uv coordinates of the quad.
      *
      */   
      void CFXReflectEnvironment::Im2DRenderQuad(const RwReal x1, const RwReal y1, const RwReal x2,
                                          const RwReal y2, const RwReal z,  const RwReal recipCamZ,
                                          const RwV2d *pUvs)
      {
         RWS_FUNCTION("RWS::FX::CFXReflectEnvironment::Im2DRenderQuad");

         RwIm2DVertex vx[4];

         //  Render an opaque white 2D quad at the given coordinates and
         //  spanning a whole texture.
         RwIm2DVertexSetScreenX(&vx[0], x1);
         RwIm2DVertexSetScreenY(&vx[0], y1);
         RwIm2DVertexSetScreenZ(&vx[0], z);
         RwIm2DVertexSetIntRGBA(&vx[0], 255, 255, 255, 255);
         RwIm2DVertexSetRecipCameraZ(&vx[0], recipCamZ);
         RwIm2DVertexSetU      (&vx[0], pUvs[0].x,  recipCamZ);
         RwIm2DVertexSetV      (&vx[0], pUvs[0].y,  recipCamZ);

         RwIm2DVertexSetScreenX(&vx[1], x1);
         RwIm2DVertexSetScreenY(&vx[1], y2);
         RwIm2DVertexSetScreenZ(&vx[1], z);
         RwIm2DVertexSetIntRGBA(&vx[1], 255, 255, 255, 255);
         RwIm2DVertexSetRecipCameraZ(&vx[1], recipCamZ);
         RwIm2DVertexSetU      (&vx[1], pUvs[0].x,  recipCamZ);
         RwIm2DVertexSetV      (&vx[1], pUvs[1].y,  recipCamZ);

         RwIm2DVertexSetScreenX(&vx[2], x2);
         RwIm2DVertexSetScreenY(&vx[2], y1);
         RwIm2DVertexSetScreenZ(&vx[2], z);
         RwIm2DVertexSetIntRGBA(&vx[2], 255, 255, 255, 255);
         RwIm2DVertexSetRecipCameraZ(&vx[2], recipCamZ);
         RwIm2DVertexSetU      (&vx[2], pUvs[1].x,  recipCamZ);
         RwIm2DVertexSetV      (&vx[2], pUvs[0].y,  recipCamZ);

         RwIm2DVertexSetScreenX(&vx[3], x2);
         RwIm2DVertexSetScreenY(&vx[3], y2);
         RwIm2DVertexSetScreenZ(&vx[3], z);
         RwIm2DVertexSetIntRGBA(&vx[3], 255, 255, 255, 255);
         RwIm2DVertexSetRecipCameraZ(&vx[3], recipCamZ);
         RwIm2DVertexSetU      (&vx[3], pUvs[1].x,  recipCamZ);
         RwIm2DVertexSetV      (&vx[3], pUvs[1].y,  recipCamZ);

         RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, vx, 4);

         RWS_RETURNVOID();
      }

      /**
      *  Renders a 2D quad with the current render states to pCamera.
      *
      *  \param pRaster    Pointer to the env map raster to render to pCamera.
      *  \param pVx        RwV2d array of vertices defining the shape of the quad.
      *  \param pUvs       RwV2d array of uvs defining the texture coordinates of the quad.
      *  \param pCamera    Pointer to the camera to use to render the quad.
      *
      */   
      void CFXReflectEnvironment::EnvMapRasterRender(const RwRaster *pRaster, const RwV2d *pVx,
                                                     const RwV2d    *pUvs,    const RwCamera *pCamera)

      {
         RWS_FUNCTION ("RWS:FX::CFXReflectEnvironment::EnvMapRasterRender");

         RWS_PRE (pRaster);
         RWS_PRE (pVx);
         RWS_PRE (pCamera);
  
         if (RwCameraBeginUpdate((RwCamera *)pCamera))
         {
            RwRaster           *pCamRas;
            RwReal              Crw, Crh;
            RwReal              RecipCamZ;
            RwBlendFunction     RENDERSTATESRCBLEND;
            RwBlendFunction     RENDERSTATEDESTBLEND;
            RwBlendFunction     RENDERSTATETEXTUREFILTER;
            RwBlendFunction     RENDERSTATETEXTURERASTER;

            //  Render a preview of the shadow raster using the given coordinates
            //  for the upper-left and lower-right corners. These should be 
            //  specified as a fraction of the display width and height.            
            pCamRas  = RwCameraGetRaster( pCamera );
            Crw      = static_cast<RwReal>( RwRasterGetWidth( pCamRas ) );
            Crh      = static_cast<RwReal>( RwRasterGetHeight( pCamRas ) );

            RecipCamZ = 1.0f / RwCameraGetNearClipPlane( pCamera );

            RwRenderStateGet( rwRENDERSTATESRCBLEND, &RENDERSTATESRCBLEND );
            RwRenderStateGet( rwRENDERSTATEDESTBLEND, &RENDERSTATEDESTBLEND );

            RwRenderStateGet( rwRENDERSTATETEXTUREFILTER, &RENDERSTATETEXTUREFILTER);
            RwRenderStateGet( rwRENDERSTATETEXTURERASTER, &RENDERSTATETEXTURERASTER);

            //  Set renderstate            
            RwRenderStateSet( rwRENDERSTATEDESTBLEND,     (void *) rwBLENDZERO );
            RwRenderStateSet( rwRENDERSTATESRCBLEND,      (void *) rwBLENDONE );

            RwRenderStateSet( rwRENDERSTATETEXTUREFILTER, (void *) rwFILTERLINEAR );
            RwRenderStateSet( rwRENDERSTATETEXTURERASTER, (void *) pRaster );


            Im2DRenderQuad  ( pVx[0].x * Crw, pVx[0].y * Crh, pVx[1].x * Crw, pVx[1].y * Crh, 
                              RwIm2DGetNearScreenZ(), RecipCamZ, pUvs);

            //  Restore renderstate             
            RwRenderStateSet( rwRENDERSTATETEXTUREFILTER, (void *) RENDERSTATETEXTUREFILTER);
            RwRenderStateSet( rwRENDERSTATETEXTURERASTER, (void *) RENDERSTATETEXTURERASTER);
            RwRenderStateSet( rwRENDERSTATEDESTBLEND,     (void *) RENDERSTATEDESTBLEND );
            RwRenderStateSet( rwRENDERSTATESRCBLEND,      (void *) RENDERSTATESRCBLEND );
            RwCameraEndUpdate((RwCamera *)pCamera);
         }

         RWS_RETURNVOID()
      }


   } // namespace FX

} //namespace RWS
