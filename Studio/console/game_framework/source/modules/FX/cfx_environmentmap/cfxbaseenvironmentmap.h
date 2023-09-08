/*****************************************************************************
 *
 * File :     CFXBaseEnvironmentMap.h
 *
 * Abstract : A behavior that allows control over RenderWare materials
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

#ifndef __CFXBASEENVIRONMENTMAP_H__
#define __CFXBASEENVIRONMENTMAP_H__

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
#include "framework/toolkits/world/cmaterialevent.h"

#include "framework/core/attributehandler/allocation policies/LinearAllocationPolicy.h"

namespace RWS
{

   namespace FX
   {
      /**
      *
      *  \ingroup Mod_FX
      *
      *  CFXBaseEnvironmentMap is a class that can be used as a base class for behaviors wishing to make use of
      *  environment mapped materials.
      *  CreateMatFXDefaultEnvFrame() must be called during the initialization of RenderWare to ensure a default frame
      *  can be attached to materials using environment maps.
      */
      class CFXBaseEnvironmentMap : public MaterialHelper::CMaterialEvent, public LinearAllocationPolicy
      {
      public:

         CFXBaseEnvironmentMap(void);
         ~CFXBaseEnvironmentMap(void);

         static void    SetMatFXDefaultEnvFrameForAtomic (CFXBaseEnvironmentMap *pObject, const RpAtomic *pAtomic);
         static void    SetMatFXDefaultEnvFrameForClump  (CFXBaseEnvironmentMap *pObject, const RpClump *pClump);
         static void    SetMatFXDefaultEnvFrameForWorld  (CFXBaseEnvironmentMap *pObject, const RpWorld *pWorld);
         static void    DeleteMatFXDefaultEnvFrame       (void);
         static RwFrame *GetDefaultEnvFrame              ();

         virtual void   MaterialConstructEvent           (const RpMaterial *pMaterial);
         virtual void   MaterialCopyEvent                (const RpMaterial *pSrcMaterial, const RpMaterial *pDstMaterial);
         virtual void   MaterialDestroyEvent             (const RpMaterial *pMaterial);
      protected:                                         
                                                         
         void                 ProcessAttachments         (const CAttributePacket& attr, RwFrame *EnvFrame);
         static RpMaterial    *MaterialCallback          (RpMaterial *pMaterial, void *pCallBackData);
         static RpAtomic      *AtomicCallback            (RpAtomic   *pAtomic, void *pCallBackData);
         static RpClump       *ClumpCallback             (RpClump *pClump, void *pCallBackData);
         static RpWorldSector *WorldSectorCallback       (RpWorldSector *pWorldSector, void *pCallBackData);
         static void          SetMatFXEnvFrameForAtomic  (CFXBaseEnvironmentMap *pObject, const RpAtomic *pAtomic, RwFrame  *pEnvMapFrame);
         static void          SetMatFXEnvFrameForClump   (CFXBaseEnvironmentMap *pObject, const RpClump *pClump, RwFrame  *pEnvMapFrame);
         static void          SetMatFXEnvFrameForWorld   (CFXBaseEnvironmentMap *pObject, const RpWorld *pWorld, RwFrame  *pEnvMapFrame);

         static RwFrame       *m_DefaultEnvFrame;        /**< Default environment map frame */
      };

   } // namespace FX
      
} // namespace RWS

#endif
