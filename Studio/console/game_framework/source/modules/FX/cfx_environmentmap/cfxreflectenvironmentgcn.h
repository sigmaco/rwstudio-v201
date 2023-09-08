/*****************************************************************************
 *
 * File :     cfxreflectenvironmentgcn.h
 *
 * Abstract : CFXReflectEnvironmentGCN contains the GCN implementation of the code
 *          : that copies the back buffer to the environment map texture
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


#ifndef _CFXREFLECTENVIRONMENTGCN_H_
#define _CFXREFLECTENVIRONMENTGCN_H_

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

      const RwReal ENV_MAP_WIDTH  = 256.0f;
      const RwReal ENV_MAP_HEIGHT = 256.0f;

      /**
      *
      *  \ingroup Mod_FX
      *  This class is used by CFXReflectEnvironment to copy from the back buffer to a raster that is used
      *  as an environment map. The area of the back buffer copied can be specified as it is useful to be able
      *  to restrict environment map to a small part of the back buffer. 
      *
      */
      class CFXReflectEnvironmentGCN
      {
         public:

         CFXReflectEnvironmentGCN();
         ~CFXReflectEnvironmentGCN();

         protected:

            void  UpdateTextureFromDisplay(const RwV2d &TopLeftUV, const RwV2d &BottomRightUV, const RwRaster *pRaster);

         private:
            void  CameraTextureFlush(const RwRaster *raster, RwBool boxFilter, RwUInt32 X, RwUInt32 Y, RwUInt32 Width, RwUInt32 Height);
      
      };

   } // namespace FX

} //namespace RWS


#endif