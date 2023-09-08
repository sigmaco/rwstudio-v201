/*****************************************************************************
*
* File :     FPSRender.cpp
*
* Abstract : High level custom rendering pipeline making use of PVS data.
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
* Copyright (c) 2002 Criterion Software Ltd.
* All Rights Reserved.
*
* RenderWare is a trademark of Canon Inc.
*
*****************************************************************************/
#include "precomp.h"
//#define RWS_CALLSTACKENABLE
//#define RWS_TRACEENABLE
//#define SHOW_ATOMIC_RENDER_ORDER

// Left to its own devices, the MW linker will dead-strip the registrar ctors, so...
#ifdef __MWERKS__
#pragma force_active on
#pragma exceptions off // Turn off exceptions due to bug in MW Dwarf Generator for PS2, present in CW 3.04
#endif

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "fpsrender.h"
#include "framework/core/macros/debugmacros.h"
#include "framework/mainloop/render.h"
#include "framework/toolkits/debugtools/debugtools.h"
#include "framework/toolkits/world/clevel.h"
#include "framework/toolkits/world/helpers/worldhelper.h"

//////////////////////////////////////////////////////////////////
//
// RenderWare Includes
//
#include <rpltmap.h>
#include <rplodatm.h>
#include <rtltmap.h>

#ifdef _MSC_VER
//#pragma comment (lib, "rpltmap.lib")
#pragma comment (lib, "rplodatm.lib")
#pragma comment (lib, "rtltmap.lib")
#endif

namespace RWS
{
   namespace FPS
   {
      RWS_REGISTERCLASS(FPSRender);
      RWS_IMPLEMENT_CLASSID(FPSRender);

      namespace
      {
   #ifdef RWS_DESIGN
         RwUInt32 g_debug;

         // Temporary store of RenderState
         //
         RwCullMode _rwRENDERSTATECULLMODE;
         RwBlendFunction _rwRENDERSTATESRCBLEND;
         RwBlendFunction _rwRENDERSTATEDESTBLEND;
         RwUInt32 _rwRENDERSTATEVERTEXALPHAENABLE;
         RwUInt32 _rwRENDERSTATEZWRITEENABLE;
         RwUInt32 _rwRENDERSTATEZTESTENABLE;
         RwBool _rwRENDERSTATEFOGENABLE;
         RwUInt32 _AtomicCounter;

         /**
         *
         *  Store render state.
         *
         */
         void StoreRenderState(void)
         {
            RWS_FUNCTION("RWS::FPS::NULL::StoreRenderState");

            RwRenderStateGet(rwRENDERSTATESRCBLEND, &_rwRENDERSTATESRCBLEND);
            RwRenderStateGet(rwRENDERSTATEDESTBLEND, &_rwRENDERSTATEDESTBLEND);
            RwRenderStateGet(rwRENDERSTATEVERTEXALPHAENABLE, &_rwRENDERSTATEVERTEXALPHAENABLE);
      
            RwRenderStateGet(rwRENDERSTATEZWRITEENABLE, &_rwRENDERSTATEZWRITEENABLE);
            RwRenderStateGet(rwRENDERSTATEZTESTENABLE, &_rwRENDERSTATEZTESTENABLE);
      
            RwRenderStateGet(rwRENDERSTATECULLMODE, &_rwRENDERSTATECULLMODE);
            RwRenderStateGet(rwRENDERSTATEFOGENABLE, &_rwRENDERSTATEFOGENABLE);

            RWS_RETURNVOID();
         }

         /**
         *
         *  Set render state to display debug info.
         *
         */
         void SetRenderState(void)
         {
            RWS_FUNCTION("RWS::FPS::NULL::SetRenderState");

            RwRenderStateSet(rwRENDERSTATESRCBLEND, (void *)rwBLENDSRCALPHA);
            RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void *)rwBLENDINVSRCALPHA);
            RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *)1);
      
            RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)0);
            RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)0);
      
            RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)rwCULLMODECULLNONE);
            RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void *)FALSE);

            RWS_RETURNVOID();
         }

         /**
         *
         *  Restore render state last stored by, StoreRenderState.
         *
         */
         void RestoreRenderState(void)
         {
            RWS_FUNCTION("RWS::FPS::NULL::StoreRenderState");

            RwRenderStateSet(rwRENDERSTATESRCBLEND, (void *)_rwRENDERSTATESRCBLEND);
            RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void *)_rwRENDERSTATEDESTBLEND);
            RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *)_rwRENDERSTATEVERTEXALPHAENABLE);
            RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)_rwRENDERSTATEZWRITEENABLE);
            RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)_rwRENDERSTATEZTESTENABLE);
            RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)_rwRENDERSTATECULLMODE);
            RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*)_rwRENDERSTATEFOGENABLE);

            RWS_RETURNVOID();
         }
   #endif
      }

      CAtomicSorter *CAtomicSorter::sm_pHead = 0;
      CAtomicSorter **CAtomicSorter::sm_pSortList = 0;
      RwUInt32 CAtomicSorter::m_numItems = 0;
      RwFreeList *CAtomicSorter::sm_pFreeList = 0;
      RwUInt32 CAtomicSorter::m_SizeSortList = 0;
      RwUInt32 CAtomicSorter::m_refcount = 0;
      const RwUInt32 CAtomicSorter::uiDefaultNumberOfAtomicsInSector = 64;

      /**
      *
      * Construct a FPSRender object.
      *
      * \see ~FPSRender
      *
      */    
      FPSRender::FPSRender(const CAttributePacket&attr) : InitCEventHandler(0) ,
         m_rwRENDERSTATECULLMODE(rwCULLMODECULLNONE),
         m_rwRENDERSTATESRCBLEND(rwBLENDZERO),
         m_rwRENDERSTATEDESTBLEND(rwBLENDZERO),
         m_rwRENDERSTATEVERTEXALPHAENABLE(FALSE),
         m_rwRENDERSTATEZWRITEENABLE(TRUE),
         m_rwRENDERSTATEZTESTENABLE(TRUE),
      
         // Init all the vars for the fog
         m_FogType(rwFOGTYPELINEAR),
         m_FogDensity(0.0f),
         m_FogDistance(0.0f),
      
         m_clearMode(0),

         // Initialize clipping planes
         m_near_clip_factor(0.005f),
         m_far_clip_factor(1.0f),
      
         m_RpWorld(0),
         m_DrawWorldSectors(TRUE),
         m_DrawWorldAtomics(TRUE),
         m_rpWORLDRENDER(rpWORLDRENDERFRONT2BACK)
      {
         RWS_FUNCTION("RWS::FPS::FPSRender::FPSRender");
      
         // Init background color.
         m_BackgroundColor.red = 0;
         m_BackgroundColor.green = 0;
         m_BackgroundColor.blue = 0;
         m_BackgroundColor.alpha = 0;

         // Init fog color.
         m_FogColor.red = 0;
         m_FogColor.green = 0;
         m_FogColor.blue = 0;
         m_FogColor.alpha = 0;         
      
         m_RpWorld = 
            static_cast<RpWorld*>(
            const_cast<void*>(
            CSystemCommands::FindFirstResourceOfType(attr, "rwID_WORLD")));
      
         RWS_ASSERT(m_RpWorld, "Unable to find world resource.");
      
         // Check if world contains PVS data, if it does hook in render callback.
         //
         if (RpPVSQuery(m_RpWorld))
         {
            RWS_TRACE("World contains PVS data, adding hook.");
         
            // Normally we would install the PVS sector callback by calling RpPVSHook
            // but as we are doing the visibility test ourselves, we don't need to do 
            // this as it would test everything twice.
            //
            // Note: A side effect of this is that RpPVSStatisticsGet always returns 0.
            //
            RpPVSHook(m_RpWorld);
         
            m_WorldRenderFunctionPtr = &FPS::FPSRender::WorldRenderPVS;
         }
         else
         {
            RWS_TRACE("World Contains No PVS Data.");
         
            m_WorldRenderFunctionPtr = &FPS::FPSRender::WorldRender;
         }

         m_AtomicRenderFunctionPtr = &FPS::FPSRender::WorldSectorRenderAtomics;

         CAtomicSorter::Open(CAtomicSorter::uiDefaultNumberOfAtomicsInSector);
      
         RWS_RETURNVOID();
      }
   
   
      /**
      *
      * Destroy a FPSRender object.
      *
      *
      * \see FPSRender.
      *
      */
      FPSRender::~FPSRender(void)
      {
         RWS_FUNCTION("RWS::FPS::FPSRender::~FPSRender");
      
         CAtomicSorter::Close();

         // Check if world contains PVS data, if it does unhook the PVS renderer
         //
         // Removed..see RpPVSHook
         //
         if (RpPVSQuery(m_RpWorld))
         {
            RpPVSUnhook(m_RpWorld);
         }

         if (CLevel::GetTranslucentWorld() == m_RpWorld)
         {
            CLevel::SetTranslucentWorld(0);
         }

         if (CLevel::GetOpaqueWorld() == m_RpWorld)
         {
            CLevel::SetOpaqueWorld(0);
         }

         UnLinkMsg(m_StartRender);
         UnRegisterMsg(m_StartRender);
      
         UnRegisterMsg(m_EndRender);

         RWS_RETURNVOID();
      }
   

      /**
      *
      * \ref WorldSectorRenderAtomics. WorldSectorRender callback used for rendering atomics
      * 
      * \param worldSector  Sector to be rendered.
      *
      * \see PVS_WorldSectorRender
      *
      */
      void FPSRender::WorldSectorRenderAtomics(RpWorldSector * worldSector)
      {
         RwLLLink           *cur, *end;
         RpTie              *tie;
         RpAtomic           *atomic;
      
         RWS_FUNCTION("RWS::FPS::FPSRender::WorldSectorRenderAtomics");
      
         RWS_PRE(worldSector);
      
         /* Coll objects first */
         cur = rwLinkListGetFirstLLLink(&worldSector->collAtomicsInWorldSector);
         end = rwLinkListGetTerminator(&worldSector->collAtomicsInWorldSector);
      
         while (cur != end)
         {
            /* Render the sucker */
            tie = rwLLLinkGetData(cur, RpTie, lAtomicInWorldSector);
            atomic = tie->apAtom;
         
            if (rwObjectTestFlags(atomic, rpATOMICRENDER))
            {
               /* Don't render objects multiple times within one frame - there's no point */
               if (atomic->renderFrame != RWSRCGLOBAL(renderFrame))
               {
                  const RwSphere     *atomicBoundingSphere;
               
                  /* Frustum test it */
                  atomicBoundingSphere = RpAtomicGetWorldBoundingSphere(atomic);
               
                  RWS_PRE(atomicBoundingSphere);
               
                  if (RwCameraFrustumTestSphere((RwCamera *) RWSRCGLOBAL(curCamera),
                     atomicBoundingSphere) != rwSPHEREOUTSIDE)
                  {
                     atomic->renderCallBack(atomic);

#ifdef RWS_DEBUGTOOLS
#ifdef SHOW_ATOMIC_RENDER_ORDER
#ifdef RWS_DESIGN
                     if (g_debug)
                     {
                        char buffer[255];

                        sprintf(buffer, "R %d", _AtomicCounter++);

                        DebugTools::PrintfXYZ(&RpAtomicGetWorldBoundingSphere(atomic)->center, buffer);
                     }
#endif
#endif
#endif
                  }
               
                  /* We don't really want to do the frustrum test for
                  * the next sector too - it'll give the same answer
                  */
                  atomic->renderFrame = RWSRCGLOBAL(renderFrame);
               }
            }
         
            /* Next */
            cur = rwLLLinkGetNext(cur);
         }
      
         /* All done */
         RWS_RETURNVOID();
      }

      /**
      *
      * \ref AtomicGetCameraDistance. Calculates the distance of the center of the atomic from the camera.
      *
      * \param atomic Pointer to an RpAtomic object.
      *
      * \return Returns the distance from the atomic to the camera.
      *
      */
      RwReal FPSRender::AtomicGetCameraDistance(RpAtomic *atomic)
      {
          RWS_FUNCTION("RWS::FPS::FPSRender::AtomicGetCameraDistance");

          RwFrame *frame;
          RwV3d *camPos, atomicPos, temp;
          RwSphere *atomicBSphere;
          RwReal distance2;

          /*
           * Atomic's bounding-sphere world-space position...
           */
          atomicBSphere = RpAtomicGetBoundingSphere(atomic);

          RwV3dTransformPoints(&atomicPos, &atomicBSphere->center, 
              1, RwFrameGetLTM(RpAtomicGetFrame(atomic)));

          /*
           * ...camera position...
           */
          frame = RwCameraGetFrame(RwCameraGetCurrentCamera());
          camPos = RwMatrixGetPos(RwFrameGetLTM(frame));

          /*
           * ...vector from camera to atomic...
           */
          RwV3dSub(&temp, &atomicPos, camPos);

          /*
           * Squared distance...
           */
          distance2 = RwV3dDotProduct(&temp, &temp);

          RWS_RETURN(distance2);
      }

      /**
      *
      * \ref WorldSectorRenderAtomics. WorldSectorRender callback used for rendering atomics
      *      in depth sorted order
      * 
      * \param worldSector  Sector to be rendered.
      *
      * \see PVS_WorldSectorRender
      *
      */
      void FPSRender::WorldSectorRenderDepthSortedAtomics(RpWorldSector * worldSector)
      {
         RwLLLink           *cur, *end;
         RpTie              *tie;
         RpAtomic           *atomic;
      
         RWS_FUNCTION("RWS:FPS::FPSRender::WorldSectorRenderDepthSortedAtomics");
      
         RWS_PRE(worldSector);
      
         CAtomicSorter::Init();
      
         /* Coll objects first */
         cur = rwLinkListGetFirstLLLink(&worldSector->collAtomicsInWorldSector);
         end = rwLinkListGetTerminator(&worldSector->collAtomicsInWorldSector);
      
         while (cur != end)
         {
            /* Render the sucker */
            tie = rwLLLinkGetData(cur, RpTie, lAtomicInWorldSector);
            atomic = tie->apAtom;
         
            if (rwObjectTestFlags(atomic, rpATOMICRENDER))
            {
               /* Don't render objects multiple times within one frame - there's no point */
               if (atomic->renderFrame != RWSRCGLOBAL(renderFrame))
               {
                  const RwSphere     *atomicBoundingSphere;
               
                  /* Frustum test it */
                  atomicBoundingSphere = RpAtomicGetWorldBoundingSphere(atomic);
               
                  RWS_PRE(atomicBoundingSphere);
               
                  if (RwCameraFrustumTestSphere((RwCamera *) RWSRCGLOBAL(curCamera),
                     atomicBoundingSphere) != rwSPHEREOUTSIDE)
                  {
                     new CAtomicSorter(atomic, AtomicGetCameraDistance(atomic));
                  }
               
                  /* We don't really want to do the frustrum test for
                  * the next sector too - it'll give the same answer
                  */
                  atomic->renderFrame = RWSRCGLOBAL(renderFrame);
               }
            }
         
            /* Next */
            cur = rwLLLinkGetNext(cur);
         }
      
         if (CAtomicSorter::m_numItems)
         {
            CAtomicSorter::DepthSort();

            for (RwUInt32 i = 0; i < CAtomicSorter::m_numItems; i++)
            {
               CAtomicSorter *ptr = CAtomicSorter::sm_pSortList[i];

               RpAtomic *atomic = ptr->m_pAtomic;
         
               atomic->renderCallBack(atomic);

#ifdef RWS_DEBUGTOOLS
#ifdef SHOW_ATOMIC_RENDER_ORDER
#ifdef RWS_DESIGN
               if (g_debug)
               {
                  char buffer[255];

                  sprintf(buffer, "D %d", _AtomicCounter++);

                  DebugTools::PrintfXYZ(&RpAtomicGetWorldBoundingSphere(atomic)->center, buffer);
               }
#endif
#endif
#endif
               delete ptr;
            }
         }
      
         /* All done */
         RWS_RETURNVOID();
      }
   

      /**
      *
      * \ref PVS_WorldSectorRender. WorldSectorRender callback used for PVS sector rendering.
      *
      * \param RpWorldSector  Sector to be rendered.
      * \param pData          Data to be passed into callback.
      *
      * \return Return visible sector.
      *
      * \see WorldSectorRenderAtomics
      *
      */
      RpWorldSector *FPSRender::PVS_WorldSectorRender(RpWorldSector * sector, void *pData)
      {
         RWS_FUNCTION("RWS::FPS::FPSRender::PVS_WorldSectorRender");
      
         RWS_PRE(sector);

         if (RpPVSWorldSectorVisible(sector))
         {
            ((RpWorld *)RWSRCGLOBAL(curWorld))->renderCallBack(sector);
         }
      
         RWS_RETURN(sector);
      }


   #ifdef RWS_DESIGN
      /**
      *
      * \ref PVS_WorldSectorRender_ShowSectors. WorldSectorRender callback used for PVS sector debugging
      *
      * \param RpWorldSector  Sector to be rendered.
      * \param pData          Data to be passed into callback.
      *
      * \return Return visible sector.
      *
      */
      RpWorldSector *FPSRender::PVS_WorldSectorRender_ShowSectors(RpWorldSector * sector, void *pData)
      {
         RWS_FUNCTION("RWS::FPS::FPSRender::PVS_WorldSectorRender");
      
         RWS_PRE(sector);

   #ifdef RWS_DEBUGTOOLS

         StoreRenderState();
         SetRenderState();

         if (RpPVSWorldSectorVisible(sector))
         {
            static RwRGBA color = {0, 255, 0, 255};

            DebugTools::ShowSectorCB(sector, &color);
         }
         else
         {
            static RwRGBA color = {255, 0, 0, 255};

            DebugTools::ShowSectorCB(sector, &color);
         }

         RestoreRenderState();

   #endif
      
         RWS_RETURN(sector);
      }
   #endif

       
      /**
      *
      * \ref PVS_WorldAtomicsRender. WorldAtomicsRender callback used for PVS sector/atomic culling.
      *
      * \param RpWorldSector  Sector to be rendered.
      * \param pData          Data to be passed into callback.
      *
      * \return Return visible sector.
      *
      */
      RpWorldSector *FPSRender::PVS_WorldAtomicsRender(RpWorldSector * sector, void *pData)
      {
         RWS_FUNCTION("RWS::FPS::FPSRender::PVS_WorldAtomicsRender");
      
         RWS_PRE(sector);

         if (RpPVSWorldSectorVisible(sector))
         {
            // C Style cast as static_cast/reinterpret_cast of void* to function pointer is not allowed. 
            //
            AtomicRenderFunctionPtr _AtomicRenderFunctionPtr = (AtomicRenderFunctionPtr)(pData);

            (_AtomicRenderFunctionPtr)(sector);
         }
      
         RWS_RETURN(sector);
      }


      /**
      *
      * \ref WorldRenderPVS Render world using PVS_WorldSectorRender, and PVS_WorldAtomicsRender.
      *
      * \param pCamera Camera to render with.
      *
      */
      void FPSRender::WorldRenderPVS(RwCamera *pCamera)
      {
         RWS_FUNCTION("RWS::FPS::FPSRender::WorldRenderPVS");
      
         RWS_PRE( pCamera );
      
         RpPVSSetViewPosition(m_RpWorld, RwMatrixGetPos(RwFrameGetLTM(RwCameraGetFrame(pCamera))));
      
         // Replacement for RpWorldRender
         //
         //
         // Normally atomics are rendered as each sector is rendered, to improve
         // texture re-use I'm rendering all the sectors and then all the atomics.
         //
   #ifdef RWS_DESIGN
         if (m_DrawWorldSectors)
         {
            RwCameraForAllSectorsInFrustum(pCamera, PVS_WorldSectorRender, m_RpWorld);
         }
         else
         {
            RwCameraForAllSectorsInFrustum(pCamera, PVS_WorldSectorRender_ShowSectors, m_RpWorld);
         }
   #else
         if (m_DrawWorldSectors)
         {
            RwCameraForAllSectorsInFrustum(pCamera, PVS_WorldSectorRender, m_RpWorld);
         }
   #endif
         // 
         // 
         //
         if (m_DrawWorldAtomics)
         {
            RwCameraForAllSectorsInFrustum(pCamera, PVS_WorldAtomicsRender, m_AtomicRenderFunctionPtr);
         }
      
         RWS_RETURNVOID();
      }

      /**
      *
      * \ref WorldSectorRender. WorldSectorRender callback used for sector rendering.
      *
      * \param RpWorldSector  Sector to be rendered.
      * \param pData          Data to be passed into callback.
      *
      * \return Return visible sector.
      *
      * \see WorldSectorRenderAtomics
      *
      */
      RpWorldSector *FPSRender::WorldSectorRender(RpWorldSector * sector, void *pData)
      {
         RWS_FUNCTION("RWS::FPS::FPSRender::PVS_WorldSectorRender");
      
         RWS_PRE(sector);

         ((RpWorld *)RWSRCGLOBAL(curWorld))->renderCallBack(sector);

         RWS_RETURN(sector);
      }

   #ifdef RWS_DESIGN
      /**
      *
      * \ref WorldSectorRender_ShowSectors. WorldSectorRender callback used for sector debugging
      *
      * \param RpWorldSector  Sector to be rendered.
      * \param pData          Data to be passed into callback.
      *
      * \return Return visible sector.
      *
      */
      RpWorldSector *FPSRender::WorldSectorRender_ShowSectors(RpWorldSector * sector, void *pData)
      {
         RWS_FUNCTION("RWS::FPS::FPSRender::PVS_WorldSectorRender");
      
         RWS_PRE(sector);

   #ifdef RWS_DEBUGTOOLS

         StoreRenderState();
         SetRenderState();

         static RwRGBA color = {0, 255, 0, 255};

         DebugTools::ShowSectorCB(sector, &color);

         RestoreRenderState();

   #endif
      
         RWS_RETURN(sector);
      }
   #endif

      /**
      *
      * \ref WorldAtomicsRender. WorldAtomicsRender callback used for render atomics.
      *
      * \param RpWorldSector  Sector to be rendered.
      * \param pData          Data to be passed into callback.
      *
      * \return Return visible sector.
      *
      */
      RpWorldSector *FPSRender::WorldAtomicsRender(RpWorldSector * sector, void *pData)
      {
         RWS_FUNCTION("RWS::FPS::FPSRender::WorldAtomicsRender");
      
         RWS_PRE(sector);

         // C Style cast as static_cast/reinterpret_cast of void* to function pointer is not allowed. 
         //
         AtomicRenderFunctionPtr _AtomicRenderFunctionPtr = (AtomicRenderFunctionPtr)(pData);

         (_AtomicRenderFunctionPtr)(sector);
      
         RWS_RETURN(sector);
      }

      /**
      *
      * \ref WorldRender. Render world using WorldSectorRender, and WorldAtomicsRender.
      *
      * \param pCamera Camera to render with.
      *
      */
      void FPSRender::WorldRender(RwCamera *pCamera)
      {
         RWS_FUNCTION("RWS::FPS::FPSRender::WorldRender");
         
   #ifdef RWS_DESIGN
         if (m_DrawWorldSectors)
         {
            RwCameraForAllSectorsInFrustum(pCamera, WorldSectorRender, m_RpWorld);
         }
         else
         {
            RwCameraForAllSectorsInFrustum(pCamera, WorldSectorRender_ShowSectors, m_RpWorld);
         }
   #else
         if (m_DrawWorldSectors)
         {
            RwCameraForAllSectorsInFrustum(pCamera, WorldSectorRender, m_RpWorld);
         }
   #endif
         
         // 
         // 
         //
         if (m_DrawWorldAtomics)
         {
            RwCameraForAllSectorsInFrustum(pCamera, WorldAtomicsRender, m_AtomicRenderFunctionPtr);
         }

         RWS_RETURNVOID();
      }
 
      /**
      *
      *  \ref Map_AttributeListToBlendMode. This function maps the attribute data specified by 
      *  LIST("rwBLENDZERO|rwBLENDONE|rwBLENDSRCCOLOR|rwBLENDINVSRCCOLOR|rwBLENDSRCALPHA|
      *  rwBLENDINVSRCALPHA|rwBLENDDESTALPHA|rwBLENDINVDESTALPHA|rwBLENDDESTCOLOR|
      *  rwBLENDINVDESTCOLOR|rwBLENDSRCALPHASAT")
      *
      *  to the RenderWare enums for the particular blend mode.
      *
      * \param var  Integer number to be mapped to blend mode
      *
      * \return Blending mode. 0  rwBLENDZERO
      *                        1  rwBLENDONE
      *                        2  rwBLENDSRCCOLOR
      *                        3  rwBLENDINVSRCCOLOR
      *                        4  rwBLENDSRCALPHA
      *                        5  rwBLENDINVSRCALPHA
      *                        6  rwBLENDDESTALPHA
      *                        7  rwBLENDINVDESTALPHA
      *                        8  rwBLENDDESTCOLOR
      *                        9  rwBLENDINVDESTCOLOR
      *                        10 rwBLENDSRCALPHASAT
      *
      */  
      RwBlendFunction FPSRender::Map_AttributeListToBlendMode(RwUInt32 var) const
      {
         RWS_FUNCTION("RWS::FPS::FPSRender::Map_AttributeListToBlendMode");
      
         RwBlendFunction temp;
      
         switch (var)
         {
            default:
            case 0: temp = rwBLENDZERO; break;
            case 1: temp = rwBLENDONE; break;
            case 2: temp = rwBLENDSRCCOLOR; break;
            case 3: temp = rwBLENDINVSRCCOLOR; break;
            case 4: temp = rwBLENDSRCALPHA; break;
            case 5: temp = rwBLENDINVSRCALPHA; break;
            case 6: temp = rwBLENDDESTALPHA; break;
            case 7: temp = rwBLENDINVDESTALPHA; break;
            case 8: temp = rwBLENDDESTCOLOR; break;
            case 9: temp = rwBLENDINVDESTCOLOR; break;
            case 10: temp = rwBLENDSRCALPHASAT;break;
         }
      
         RWS_RETURN(temp);
      }
   

      /**
      *
      * \ref SetClippingMode. Enable/Disable true near clipping mode.
      *
      * \param trueclip  set true near clipping TRUE or FALSE.
      *
      */  
   #ifdef SKY
      void FPSRender::SetClippingMode(RwBool trueclip)
      {
         RWS_FUNCTION("RWS::FPS::FPSRender::SetClippingMode");

         RpSkySelectTrueTSClipper(trueclip); //  select true clipping - for tristrips
         RpSkySelectTrueTLClipper(trueclip); //  select true clipping - for trilists

         RWS_RETURNVOID();
      }
   #endif

     /**
      *
      * \ref HandleEvents(). Handle events.    
      *
      * \param pMsg Standard Message Package.
      *  
      * \ref iMsgRunningTick
      */
      void FPSRender::HandleEvents(CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::FPS::FPSRender::HandleEvents");
      
         if (pMsg.Id == m_StartRender)
         {
            RwCamera *pRwCamera = reinterpret_cast<RwCamera*>(pMsg.pData);

            DoRender(pRwCamera);

            // End Camera Event
            //
            CMsg Msg(m_EndRender, pMsg.pData);

            SendMsg(Msg);
         }
      
         RWS_RETURNVOID();
      }
 

      /**
      *
      * \ref DoRender. Render world.  The world is rendered from the /ref iMsgDoRender event.
      *
      * \param pMsg Standard Message Package.
      *
      */
      void FPSRender::DoRender( RwCamera *pRwCamera )
      {
         RWS_FUNCTION("RWS::FPS::FPSRender::DoRender");

         RwBool UpdateFogDistance = false;

#ifdef RWS_DESIGN
         if (m_Flags & uATTRIBUTEHANDLER_FLAG_DEBUG)
         {
            g_debug = true;
         }
         else
         {
            g_debug = false;
         }
#endif

         if ( pRwCamera)
         {
            // Add camera to world
            RpWorldAddCamera(m_RpWorld, pRwCamera);

            if (m_clearMode) RwCameraClear (pRwCamera, &m_BackgroundColor, m_clearMode);
         
            // Update near/far clip values, note based on CLevel::GetOpaqueWorld() not m_RpWorld so 
            // that all of the worlds use the same clipping range (Z-buffer range is fitted to near
            // far planes)
            const RwBBox& worldBox = *RpWorldGetBBox(CLevel::GetOpaqueWorld());
            RwV3d diff;
            RwV3dSub( &diff, &worldBox.sup, &worldBox.inf );
            RwReal worldDiagonal = RwV3dLength(&diff);
         
            RwReal m_near_clipping_plane = m_near_clip_factor * worldDiagonal;
            RwReal m_far_clipping_plane = m_far_clip_factor * worldDiagonal;

            RwCameraSetNearClipPlane(pRwCamera, m_near_clipping_plane);
            RwCameraSetFarClipPlane(pRwCamera, m_far_clipping_plane);
         
   #ifdef SKY
            SetClippingMode(m_trueclip);
   #endif

            (void)RpWorldSetRenderOrder(m_RpWorld, m_rpWORLDRENDER);

            // Matrix flags are sometimes discarded, RwCameraBeginUpdate will
            // assert if these are not set
            //
            rwMatrixSetFlags(RwFrameGetLTM(RwCameraGetFrame(pRwCamera)),
               rwMATRIXTYPENORMAL | rwMATRIXTYPEORTHOGONAL);

            // Begin camera update
            if (RwCameraBeginUpdate(pRwCamera))
            {
               // Select correct LOD levels
               //
               RpLODAtomicSetCamera(pRwCamera);

               // Temporary store of RenderState
               //
               RwCullMode _rwRENDERSTATECULLMODE;
               RwBlendFunction _rwRENDERSTATESRCBLEND;
               RwBlendFunction _rwRENDERSTATEDESTBLEND;
               RwUInt32 _rwRENDERSTATEVERTEXALPHAENABLE;
               RwUInt32 _rwRENDERSTATEZWRITEENABLE;
               RwUInt32 _rwRENDERSTATEZTESTENABLE;
               RwBool _rwRENDERSTATEFOGENABLE;

               // Store render state.
               //
               RwRenderStateGet(rwRENDERSTATESRCBLEND, &_rwRENDERSTATESRCBLEND);
               RwRenderStateGet(rwRENDERSTATEDESTBLEND, &_rwRENDERSTATEDESTBLEND);
               RwRenderStateGet(rwRENDERSTATEVERTEXALPHAENABLE, &_rwRENDERSTATEVERTEXALPHAENABLE);
               RwRenderStateGet(rwRENDERSTATEZWRITEENABLE, &_rwRENDERSTATEZWRITEENABLE);
               RwRenderStateGet(rwRENDERSTATEZTESTENABLE, &_rwRENDERSTATEZTESTENABLE);
               RwRenderStateGet(rwRENDERSTATECULLMODE, &_rwRENDERSTATECULLMODE);
               RwRenderStateGet(rwRENDERSTATEFOGENABLE, &_rwRENDERSTATEFOGENABLE);

               // Set Render State
               //
               RwRenderStateSet(rwRENDERSTATESRCBLEND, (void *)m_rwRENDERSTATESRCBLEND);
               RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void *)m_rwRENDERSTATEDESTBLEND);
               RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *)m_rwRENDERSTATEVERTEXALPHAENABLE);
               RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)m_rwRENDERSTATEZWRITEENABLE);
               RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)m_rwRENDERSTATEZTESTENABLE);
               RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)m_rwRENDERSTATECULLMODE);
         
               // Enable fog etc
               if (m_FogType == rwFOGTYPENAFOGTYPE)
               {
                  RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void *)FALSE);
               }
               else
               {
                  RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void *)TRUE);
            
                  if ( !RwRenderStateSet(rwRENDERSTATEFOGTYPE, (void *)m_FogType) )
                  {
                     // Unable to use this fog mode with this hardware
                     //
                     RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void *)FALSE);
                  }
                  else
                  {
                     // Setup the fog color and density etc
                     RwRenderStateSet(rwRENDERSTATEFOGCOLOR, (void *)RWRGBALONG(m_FogColor.red, m_FogColor.green, m_FogColor.blue, m_FogColor.alpha));                     
                     RwRenderStateSet(rwRENDERSTATEFOGDENSITY, (void *)&m_FogDensity);
                     UpdateFogDistance = true;
                  }
               }

   #ifdef RWS_DESIGN
               _AtomicCounter = 0;
   #endif

               // Render the world, either using default renderer or PVS Renderer
               (this->*m_WorldRenderFunctionPtr)(pRwCamera);

               RwRenderStateSet(rwRENDERSTATESRCBLEND, (void *)_rwRENDERSTATESRCBLEND);
               RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void *)_rwRENDERSTATEDESTBLEND);
               RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *)_rwRENDERSTATEVERTEXALPHAENABLE);
               RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)_rwRENDERSTATEZWRITEENABLE);
               RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)_rwRENDERSTATEZTESTENABLE);
               RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)_rwRENDERSTATECULLMODE);
               RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*)_rwRENDERSTATEFOGENABLE);

               // End camera update
               RwCameraEndUpdate (pRwCamera);


            } // RwCameraBeginUpdate
         
            if (UpdateFogDistance)
            {
               RwCameraSetFogDistance(pRwCamera, m_FogDistance);
            }

            // Remove Camera from world
            RpWorldRemoveCamera(m_RpWorld, pRwCamera);
         }

#ifdef RWS_DESIGN
         g_debug = false;
#endif

         RWS_RETURNVOID();
      }
   
   
      /**
      *
      * \ref HandleAttributes. Handle attribute updates.
      *
      * \param Attr reference to a CAttributePacket.
      *
      */     
      void FPSRender::HandleAttributes(const CAttributePacket& attr)
      {
         RWS_FUNCTION("RWS::FPS::FPSRender::HandleAttributes");
      
         CAttributeHandler::HandleAttributes(attr);
      
         CAttributeCommandIterator attrIt(attr, RWS_CLASSID_OF(FPSRender));
      
         while (!attrIt.IsFinished())
         {
            switch (attrIt->GetCommandId())
            {
               case CMD_rwRENDERSTATECULLMODE:
                  switch (attrIt->GetAs_RwUInt32())
                  {
                  case 0: m_rwRENDERSTATECULLMODE = rwCULLMODECULLNONE;
                     break;
                  case 1: m_rwRENDERSTATECULLMODE = rwCULLMODECULLBACK;
                     break;
                  case 2: m_rwRENDERSTATECULLMODE = rwCULLMODECULLFRONT;
                     break;
                  }
               break;
               case CMD_rwRENDERSTATESRCBLEND:
                  m_rwRENDERSTATESRCBLEND = Map_AttributeListToBlendMode(attrIt->GetAs_RwInt32());
                  break;
               case CMD_rwRENDERSTATEDESTBLEND:
                  m_rwRENDERSTATEDESTBLEND = Map_AttributeListToBlendMode(attrIt->GetAs_RwInt32());
                  break;
               case CMD_rwRENDERSTATEVERTEXALPHAENABLE:
                  m_rwRENDERSTATEVERTEXALPHAENABLE = attrIt->GetAs_RwInt32();
                  break;
               case CMD_rwRENDERSTATEZWRITEENABLE:
                  m_rwRENDERSTATEZWRITEENABLE = attrIt->GetAs_RwInt32();
                  break;
               case CMD_rwRENDERSTATEZTESTENABLE:
                  m_rwRENDERSTATEZTESTENABLE = attrIt->GetAs_RwInt32();
                  break;
               case CMD_GlobalOpaqueWorld:
                  if (attrIt->GetAs_RwUInt32() != 0)
                  {
                     CLevel::SetOpaqueWorld(m_RpWorld);
                  }
                  break;
               case CMD_GlobalTranslucentWorld:
                  if (attrIt->GetAs_RwUInt32() != 0)
                  {
                     CLevel::SetTranslucentWorld(m_RpWorld);
                  }
                  break;
               case CMD_Set_m_FogType:
                  //////////////////////////////////////////////////////////////////
                  // Check the list position to determine the value to apply,
                  // note need to do the conversion as rwFOGTYPENAFOGTYPE etc
                  // may not be defined 0, 1, 2, 3
                  //
                  switch (attrIt->GetAs_RwInt32())
                  {
                     case 0: m_FogType = rwFOGTYPENAFOGTYPE; break;
                     case 1: m_FogType = rwFOGTYPELINEAR; break;
                     case 2: m_FogType = rwFOGTYPEEXPONENTIAL; break;
                     case 3: m_FogType = rwFOGTYPEEXPONENTIAL2; break;
                  }
                  break;
               case CMD_Set_m_FogColor:
                  m_FogColor = attrIt->GetAs_RwRGBA();                  
                  break;
               case CMD_Set_m_FogDensity:
                  attrIt->GetCommandData(m_FogDensity);
                  break;
               case CMD_Set_m_FogDistance:
                  attrIt->GetCommandData(m_FogDistance);
                  break;
               case CMD_Set_m_BackgroundColor:
                     m_BackgroundColor =  attrIt->GetAs_RwRGBA();
                  break;
                  //
                  //
               case CMD_Set_m_ClearScreen:
                  if (attrIt->GetAs_RwUInt32()) m_clearMode |= rwCAMERACLEARIMAGE;
                  else                         m_clearMode &= ~rwCAMERACLEARIMAGE;
                  break;
               case CMD_Set_m_ClearZ:
                  if (attrIt->GetAs_RwUInt32()) m_clearMode |= rwCAMERACLEARZ;
                  else                         m_clearMode &= ~rwCAMERACLEARZ;
                  break;
               case CMD_Set_m_ClearStencil:
                  if (attrIt->GetAs_RwUInt32()) m_clearMode |= rwCAMERACLEARSTENCIL;
                  else                         m_clearMode &= ~rwCAMERACLEARSTENCIL;
                  break;
                  //
                  //
               case CMD_Set_m_near_clip_factor:
                  attrIt->GetCommandData(m_near_clip_factor);
                  m_near_clip_factor *= 0.01f;   // Convert from % to fraction

                  if (m_near_clip_factor > m_far_clip_factor)
                  {
                     m_near_clip_factor = m_far_clip_factor * 0.99f;
                  }
                  break;
               case CMD_Set_m_far_clip_factor:
                  attrIt->GetCommandData(m_far_clip_factor);
                  m_far_clip_factor *= 0.01f;   // Convert from % to fraction

                  if (m_near_clip_factor > m_far_clip_factor)
                  {
                     m_near_clip_factor = m_far_clip_factor * 0.99f;
                  }

                  break;
   #ifdef SKY
               case CMD_trueclip:
                  attrIt->GetCommandData(m_trueclip);
                  break;
   #else
               case CMD_trueclip:
                  RWS_TRACE("CMD_trueclip Ignored on platforms other than PS2");
                  break;
   #endif
               case CMD_Start_Render:
                  {
                     int _link_priority = GetLinkedPriority(m_StartRender);

                     if (_link_priority<0) _link_priority = 0xffff;

                     // If already linked, unlink and unregister as were about to register with
                     // a new event.
                     //
                     UnLinkMsg(m_StartRender);
                     UnRegisterMsg(m_StartRender);

                     //
                     //
                     RegisterMsg(m_StartRender, attrIt->GetAs_RwChar_ptr(), "RwCamera*");
                     LinkMsg(m_StartRender, "RwCamera*", _link_priority);
                  }
                  break;
               case CMD_End_Render:
                  ReplaceRegisteredMsg(m_EndRender, attrIt->GetAs_RwChar_ptr(), "RwCamera*");
                  break;
               case CMD_priority:
                  // If already linked, unlink as were about to relink at a different priority
                  //
                  UnLinkMsg(m_StartRender);
                  LinkMsg(m_StartRender, "RwCamera*", attrIt->GetAs_RwUInt32());
                  break;
               case CMD_DEBUG_WORLD_SECTORS:
                  attrIt->GetCommandData(m_DrawWorldSectors);
                  break;
               case CMD_DEBUG_WORLD_ATOMICS:
                  attrIt->GetCommandData(m_DrawWorldAtomics);
                  break;
               case CMD_lightmapflags:
                  if (RpLtMapWorldLightMapsQuery(m_RpWorld))
                  {
                     RwUInt32 _uiflags;
                  
                     attrIt->GetCommandData(_uiflags);
                  
                     RwUInt32 _iolightflags = 0;
                  
                     if ( (_uiflags&1) ) _iolightflags |= rpLTMAPSTYLERENDERBASE;
                     if ( (_uiflags&2) ) _iolightflags |= rpLTMAPSTYLERENDERLIGHTMAP;
                     if ( (_uiflags&4) ) _iolightflags |= rpLTMAPSTYLEPOINTSAMPLE;
                  
                     RWS_TRACE("CMD_lightmapflags "<<_uiflags<<" "<<_iolightflags);
                  
                     RpLtMapSetRenderStyle( static_cast<RpLtMapStyle>(_iolightflags), m_RpWorld);
                  }
                  else
                  {
                     RWS_TRACE("World contains no Lightmap data");
                  }
                  break;

               case CMD_rpWORLDRENDER:
                  {
                     RwUInt32 index;

                     attrIt->GetCommandData(index);

                     switch (index)
                     {
                        default:
                        case 0:
                           m_rpWORLDRENDER = rpWORLDRENDERFRONT2BACK;
                           break;
                        case 1:
                           m_rpWORLDRENDER = rpWORLDRENDERBACK2FRONT;
                           break;
                     }
                  }
                  break;

               case CMD_rpATOMICRENDER:
                  switch (attrIt->GetAs_RwUInt32())
                  {
                     case 0:
                        m_AtomicRenderFunctionPtr = &FPS::FPSRender::WorldSectorRenderAtomics;
                        break;
                     case 1:
                        m_AtomicRenderFunctionPtr = &FPS::FPSRender::WorldSectorRenderDepthSortedAtomics;
                        break;
                  }
            }
            ++attrIt;
         }
         RWS_RETURNVOID();
      }
   }
}

