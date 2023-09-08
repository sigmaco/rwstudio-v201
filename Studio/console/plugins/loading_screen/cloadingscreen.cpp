/*****************************************************************************
*
* File :     CLoadingScreen.cpp
*
* Notes :
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
#endif

#ifdef RWS_DESIGN

#include <vector>

//////////////////////////////////////////////////////////////////
//
// Renderware Studio Includes
//
#include <rwcore.h>
#include <rtcharse.h>
#include <rtworld.h>
#include <rpworld.h>
#include <rtpitexd.h>

#include "../../game_framework/source/framework/core/macros/debugmacros.h"

#include "cloadingscreen.h"
#include "splashtxd.h"

//#define CREATETXD

namespace RWS
{
   static RwRGBA ForegroundColor = { 255,  255,  255,  192};
   static RwRGBA BackgroundColor = { 0, 0, 0, 0};

   /**
   *
   *  Construct CLoadingScreen object
   *
   */
   CLoadingScreen::CLoadingScreen()
   {
      RWS_FUNCTION("CLoadingScreen::CLoadingScreen");

      m_splashTXD = 0;

      if (!m_splashTXD)
      {
         m_splashTXD = ReadSplashTXD();
      }

      m_Charset = RtCharsetCreate(&ForegroundColor, &BackgroundColor);

      RWS_RETURNVOID();
   }


    /**
    *
    *  Destroy CLoadingScreen object
    *
    */
   CLoadingScreen::~CLoadingScreen()
   {
      RWS_FUNCTION("CLoadingScreen::~CLoadingScreen");

      RtCharsetDestroy(m_Charset);

      if (m_splashTXD)
      {
         RwTexDictionaryDestroy(m_splashTXD);
         m_splashTXD = 0;
      }

      RWS_RETURNVOID();
   }

   
   void CLoadingScreen::RenderOverlay(RwRaster *texRaster, RwUInt8 alpha, RwCamera *camera,
                                      RwReal minU, RwReal minV, RwReal maxU, RwReal maxV)
   {
      RWS_FUNCTION("CLoadingScreen::RenderOverlay");

      RwIm2DVertex box[4];               

      RwIm2DVertexSetScreenX(&box[0], (RwReal)(0.0f));
      RwIm2DVertexSetScreenY(&box[0], (RwReal)(0.0f));
      RwIm2DVertexSetScreenZ(&box[0], RwIm2DGetNearScreenZ());
      RwIm2DVertexSetRecipCameraZ(&box[0], (RwReal)(1.0f)/RwCameraGetNearClipPlane(camera));
      RwIm2DVertexSetU(&box[0], (RwReal)(minU), (RwReal)(1.0f));
      RwIm2DVertexSetV(&box[0], (RwReal)(minV), (RwReal)(1.0f));
      RwIm2DVertexSetIntRGBA(&box[0], 255, 255, 255, alpha);

      RwIm2DVertexSetScreenX(&box[1], (RwReal)(0.0f));
      RwIm2DVertexSetScreenY(&box[1], (RwReal)RwRasterGetHeight(RwCameraGetRaster(camera)));
      RwIm2DVertexSetScreenZ(&box[1], RwIm2DGetNearScreenZ());
      RwIm2DVertexSetRecipCameraZ(&box[1], (RwReal)(1.0f)/RwCameraGetNearClipPlane(camera));
      RwIm2DVertexSetU(&box[1], (RwReal)(minU), (RwReal)(1.0f));
      RwIm2DVertexSetV(&box[1], (RwReal)(maxV), (RwReal)(1.0f));
      RwIm2DVertexSetIntRGBA(&box[1], 255, 255, 255, alpha);

      RwIm2DVertexSetScreenX(&box[2], (RwReal)RwRasterGetWidth(RwCameraGetRaster(camera)));
      RwIm2DVertexSetScreenY(&box[2], (RwReal)(0.0f));
      RwIm2DVertexSetScreenZ(&box[2], RwIm2DGetNearScreenZ());
      RwIm2DVertexSetRecipCameraZ(&box[2], (RwReal)(1.0f)/RwCameraGetNearClipPlane(camera));
      RwIm2DVertexSetU(&box[2], (RwReal)(maxU), (RwReal)(1.0f));
      RwIm2DVertexSetV(&box[2], (RwReal)(minV), (RwReal)(1.0f));
      RwIm2DVertexSetIntRGBA(&box[2], 255, 255, 255, alpha);

      RwIm2DVertexSetScreenX(&box[3], (RwReal)RwRasterGetWidth(RwCameraGetRaster(camera)));
      RwIm2DVertexSetScreenY(&box[3], (RwReal)RwRasterGetHeight(RwCameraGetRaster(camera)));
      RwIm2DVertexSetScreenZ(&box[3], RwIm2DGetNearScreenZ());
      RwIm2DVertexSetRecipCameraZ(&box[3], (RwReal)(1.0f)/RwCameraGetNearClipPlane(camera));
      RwIm2DVertexSetU(&box[3], (RwReal)(maxU), (RwReal)(1.0f));
      RwIm2DVertexSetV(&box[3], (RwReal)(maxV), (RwReal)(1.0f));
      RwIm2DVertexSetIntRGBA(&box[3], 255, 255, 255, alpha);

      RwTextureFilterMode tRwTextureFilterMode;
      RwBool trwRENDERSTATEFOGENABLE;
      RwBool trwRENDERSTATEZTESTENABLE;
      RwBool trwRENDERSTATEZWRITEENABLE;
      RwRaster *pRaster = NULL;
      RwBool trwRENDERSTATEVERTEXALPHAENABLE;
      RwBlendFunction trwRENDERSTATESRCBLEND;
      RwBlendFunction trwRENDERSTATEDESTBLEND;
      RwTextureAddressMode trwRENDERSTATETEXTUREADDRESS;

      // Save current Render State
      //
      RwRenderStateGet(rwRENDERSTATETEXTURERASTER, (void *)&pRaster);    
      RwRenderStateGet(rwRENDERSTATETEXTUREFILTER, (void *)&tRwTextureFilterMode);
      RwRenderStateGet(rwRENDERSTATEFOGENABLE, (void *)&trwRENDERSTATEFOGENABLE);
      RwRenderStateGet(rwRENDERSTATEZTESTENABLE, (void *)&trwRENDERSTATEZTESTENABLE);
      RwRenderStateGet(rwRENDERSTATEZWRITEENABLE, (void *)&trwRENDERSTATEZWRITEENABLE);
      RwRenderStateGet(rwRENDERSTATETEXTUREADDRESS , (void *)&trwRENDERSTATETEXTUREADDRESS);
      RwRenderStateGet(rwRENDERSTATEVERTEXALPHAENABLE, (void *)&trwRENDERSTATEVERTEXALPHAENABLE);
      RwRenderStateGet(rwRENDERSTATESRCBLEND, (void *)&trwRENDERSTATESRCBLEND);
      RwRenderStateGet(rwRENDERSTATEDESTBLEND, (void *)&trwRENDERSTATEDESTBLEND);

      // Select new Render State
      //
      RwRenderStateSet(rwRENDERSTATETEXTUREFILTER, (void *)rwFILTERLINEAR);
      RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void *)FALSE);
      RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void *)FALSE);
      RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void *)FALSE);
      RwRenderStateSet(rwRENDERSTATETEXTURERASTER, (void *)texRaster);   
      RwRenderStateSet(rwRENDERSTATETEXTUREADDRESS , (void *)rwTEXTUREADDRESSCLAMP);
      RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *)TRUE);
      RwRenderStateSet(rwRENDERSTATESRCBLEND, (void *)rwBLENDSRCALPHA);
      RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void *)rwBLENDINVSRCALPHA);

      RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, box, 4);            

      // Restore Render State
      //
      RwRenderStateSet(rwRENDERSTATETEXTUREFILTER, (void *)tRwTextureFilterMode);
      RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void *)trwRENDERSTATEFOGENABLE);
      RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void *)trwRENDERSTATEZTESTENABLE);
      RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void *)trwRENDERSTATEZWRITEENABLE);
      RwRenderStateSet(rwRENDERSTATETEXTURERASTER, (void *)pRaster);    
      RwRenderStateSet(rwRENDERSTATETEXTUREADDRESS , (void *)trwRENDERSTATETEXTUREADDRESS);
      RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *)trwRENDERSTATEVERTEXALPHAENABLE);
      RwRenderStateSet(rwRENDERSTATESRCBLEND, (void *)trwRENDERSTATESRCBLEND);
      RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void *)trwRENDERSTATEDESTBLEND);
 
      RWS_RETURNVOID();
   }


   RwTexDictionary *CLoadingScreen::ReadSplashTXD(void)
   {
      RWS_FUNCTION("CLoadingScreen::ReadSplashTXD");

      RwTexDictionary *m_splashTXD = (RwTexDictionary *)NULL;

#ifdef CREATETXD
      RwInt32 wasMip;
      //get old settings
      RwTexDictionary *oldTXD = RwTexDictionaryGetCurrent();
      wasMip = RwTextureGetMipmapping();
      //setup for new txd and loading
      m_splashTXD = RwTexDictionaryCreate();
      RwTexDictionarySetCurrent(m_splashTXD);
      RwTextureSetMipmapping(FALSE);
      //load the texture and name it
      RwTexture *splashTex = RwTextureRead("c:\\splash.png", NULL);
      RWS_ASSERT(splashTex, "Invalid Texture. c:\\splash.png not found");
      
      RwTextureSetName(splashTex, "SPLASH");

      //save the texdict to a memory stream
      RwStream *stream;
      RwMemory memblock;
      memblock.length = 0;
      memblock.start = NULL;
      stream = RwStreamOpen(rwSTREAMMEMORY, rwSTREAMWRITE, &memblock);
      RtPITexDictionaryStreamWrite(m_splashTXD, stream);
      RwStreamClose(stream, &memblock);
      //dump the memory to a header file
      FILE *fp = fopen("c:\\splashtxd.h", "w");
      unsigned int charcount = 0;

      fprintf(fp, "static RwUInt8 splashMemDump[] = {\n");
      while(charcount < memblock.length)
      {
         for (int i = 0; i < 16 && (charcount < memblock.length); i++)
         {
            fprintf(fp, "0x%x, ", memblock.start[charcount]);
            charcount++;
         }
         fprintf(fp, "\n");
      }
      fprintf(fp, "\n};");
      fclose(fp);
      //free the memory and restore settings
      RwFree(memblock.start);
      RwTextureSetMipmapping(wasMip);
      RwTexDictionarySetCurrent(oldTXD);
#else // CREATETXD
      // we need to "load" the txd
      RwStream *stream;
      RwMemory memblock;
      memblock.start = splashMemDump;
      memblock.length = sizeof(splashMemDump);

      stream = RwStreamOpen(rwSTREAMMEMORY, rwSTREAMREAD, &memblock);
      RwStreamFindChunk(stream, rwID_PITEXDICTIONARY, 0, 0);
      m_splashTXD = RtPITexDictionaryStreamRead(stream);
#endif // CREATETXD

      RWS_RETURN(m_splashTXD);
   }

   
   /**
   *
   *  CLoadingScreen::Render
   *
   */
   void CLoadingScreen::Render(RwCamera *pRwCamera, RwChar *message)
   {
      RWS_FUNCTION("CLoadingScreen::Render");

      if(pRwCamera)
      {
         // Begin camera update
         //
         if(RwCameraBeginUpdate(pRwCamera) != NULL)
         {
            RenderOverlay(RwTextureGetRaster(RwTexDictionaryFindNamedTexture(m_splashTXD, "SPLASH")),
                              255, pRwCamera, 0.0f, 0.0f, 1.0f, 1.0f);

            if (message)
            {
               RwRaster *pRaster = RwCameraGetRaster(pRwCamera);

               RtCharsetPrint(m_Charset, message, RwRasterGetWidth(pRaster)/8, RwRasterGetHeight(pRaster)/2);
            }

          // End camera update
          //
          RwCameraEndUpdate (pRwCamera);
         }
      }

      RWS_RETURNVOID();
   }

} // namespace RWS
#endif