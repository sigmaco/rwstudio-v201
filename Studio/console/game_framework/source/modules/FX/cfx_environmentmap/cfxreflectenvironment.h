/*****************************************************************************
 *
 * File :     CFXReflectEnvironment.h
 *
 * Abstract : A behaviour to map the back buffer to an environment map texture
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

#ifndef _CFXREFLECTENVIRONMENT_H_
#define _CFXREFLECTENVIRONMENT_H_
          
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
#include "cfxenvironmentmap.h"

namespace RWS
{

   namespace FX
   {
#if (defined (DOLPHIN))
      class CFXReflectEnvironmentGCN;
#endif
#if (defined (_XBOX))
      class CFXReflectEnvironmentXBox;
#endif
#if (defined (SKY))
      class CFXReflectEnvironmentSky;
#endif
   }
}

#if (defined (DOLPHIN))
   #include "cfxreflectenvironmentgcn.h"
#endif
#if (defined (_XBOX))
   #include "cfxreflectenvironmentxbox.h"
#endif
#if (defined (SKY))
   #include "cfxreflectenvironmentsky.h"
#endif

namespace RWS
{

   namespace FX
   {
      typedef struct
      {
         RwTexture   *pTexture;
      }t_SET_TEXTURE_CALLBACK_INFO;


#if (defined (DOLPHIN))
      #define CFXEnvironmentMapPlatformImplementation CFXReflectEnvironmentGCN
#endif
#if (defined (_XBOX))
      #define CFXEnvironmentMapPlatformImplementation CFXReflectEnvironmentXBox
#endif
#if (defined (SKY))
      #define CFXEnvironmentMapPlatformImplementation CFXReflectEnvironmentSky
#endif

      /**
      *
      *  \ingroup Mod_FX
      *  This behavior is used to map a section of the back buffer to an environment map texture. This provides a way
      *  for objects to give the impression they are reflecting their environment even though a true reflection is not
      *  taking place.
      *  To use the behavior drop the CFXReflectEnvironment behavior into the game database, then drag the assets you
      *  want to make use of the back buffer environment map onto the behavior. The assets must contain materials that
      *  have an environment map material in order to see any effect.
      *
      */
      class CFXReflectEnvironment : public CFXEnvironmentMapPlatformImplementation, public CFXEnvironmentMap
      {
         public:

         RWS_MAKENEWCLASS(CFXReflectEnvironment);
         RWS_DECLARE_CLASSID(CFXReflectEnvironment);
         RWS_CATEGORY("FX");
         RWS_DESCRIPTION("Reflect Environment", "Environment map effect behavior");

         RWS_BEGIN_COMMANDS
            RWS_MESSAGE  (CMD_UpdateMap,  "Update map event",   "Copys the front buffer to the environment map texture",RECEIVE , RwCamera*,"iMsgDoPostRender")
            RWS_MESSAGE  (CMD_ShowEnvMap, "Show env map event", "Displays the environment map in a window",RECEIVE , RwCamera*,"iMsgDoRender")

            RWS_SEPARATOR ("Back Buffer Area", 1)
            RWS_ATTRIBUTE(CMD_Set_X1,     "X1 ", "Defines the first X coordinate of back buffer area to grab",   SLIDER,  RwReal, RANGE(0.0, 0.0, 1.0))
            RWS_ATTRIBUTE(CMD_Set_Y1,     "Y1 ", "Defines the first Y coordinate of back buffer area to grab",   SLIDER,  RwReal, RANGE(0.0, 0.0, 1.0))
            RWS_ATTRIBUTE(CMD_Set_X2,     "X2 ", "Defines the second X coordinate of back buffer area to grab",  SLIDER,  RwReal, RANGE(0.0, 1.0, 1.0))
            RWS_ATTRIBUTE(CMD_Set_Y2,     "Y2 ", "Defines the second Y coordinate of back buffer area to grab",  SLIDER,  RwReal, RANGE(0.0, 1.0, 1.0))
         RWS_END_COMMANDS;


         CFXReflectEnvironment(const CAttributePacket& rAttr);
         ~CFXReflectEnvironment();
         virtual void         HandleEvents      (CMsg &pMsg);
         virtual void         HandleAttributes  (const CAttributePacket& attr);
         static RpMaterial   *SetTexture        (RpMaterial *pMaterial, void *pCallBackData);
         static void          Im2DRenderQuad    (const RwReal x1, const RwReal y1, const RwReal x2, const RwReal y2,
                                                 const RwReal z,  const RwReal recipCamZ, const RwV2d *pUvs);
         static void          EnvMapRasterRender(const RwRaster *pRaster, const RwV2d *pVx,
                                                 const RwV2d    *pUvs,    const RwCamera *pCamera);

         protected:

         private:
         CEventId             m_UpdateMessage;     /**< Event ID that will trigger the copying of the back buffer to
                                                        the environment map texture. */
         CEventId             m_ShowEnvMapMessage; /**< Event ID used to preview the environment map texture in the
                                                        corner of the screen */
         RwRaster            *m_Raster;            /**< Raster used to store the environment map texture */
         RwTexture           *m_Texture;           /**< Texture used to store the environment map texture */

         RwV2d                m_TopLeft;           /**< Defines the top left of the frame raster area to use as environment map. */
         RwV2d                m_BottomRight;       /**< Defines the bottom right of the frame raster area to use as environment map. */
  
      };

   };

};

#endif