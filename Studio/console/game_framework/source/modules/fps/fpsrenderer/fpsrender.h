/*****************************************************************************
*
* File :     FPSRender.h
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
* Copyright (c) 2002 Criterion Software Ltd.
* All Rights Reserved.
*
* RenderWare is a trademark of Canon Inc.
*
*****************************************************************************/

#ifndef __FPS_RENDER_H__
#define __FPS_RENDER_H__

//////////////////////////////////////////////////////////////////
//
// RenderWare Includes
//
#include <rwcore.h>
#include <rpworld.h>
#include <rppvs.h>
#include <rtworld.h>

#ifdef _MSC_VER
#pragma comment (lib, "rwcore.lib")
#pragma comment (lib, "rpworld.lib")
#pragma comment (lib, "rppvs.lib")
#pragma comment (lib, "rtworld.lib")
#endif


//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "framework/core/eventhandler/ceventhandler.h"
#include "framework/core/attributehandler/cattributehandler.h"
#include "framework/core/attributehandler/csystemcommands.h"
#include "framework/core/attributehandler/allocation policies/LinearAllocationPolicy.h"

namespace RWS
{
   namespace FPS
   {
      // Class used to depth sort atomics
      //
      class CAtomicSorter
      {
      public:
         
         CAtomicSorter(RpAtomic *pAtomic, RwReal rDepth) : m_rDepth(rDepth), m_pAtomic(pAtomic)
         {
            pNext = sm_pHead;
            sm_pHead = this;
            
            m_numItems++;
         }
         
         static void Init(void) {sm_pHead = 0;m_numItems = 0;}
         
         static int DepthSortFunc(const void *a, const void *b)
         {
            const CAtomicSorter **ptrA = (const CAtomicSorter**)(a);
            const CAtomicSorter **ptrB = (const CAtomicSorter**)(b);
            
            RwReal DepthA = (*ptrA)->m_rDepth;
            RwReal DepthB = (*ptrB)->m_rDepth;
            
            if (DepthA > DepthB) return -1;
            else if (DepthA == DepthB) return 0;
            else return 1;
         }
         
         static void DepthSort(void)
         {
            PreSort();
            
            qsort((void*)sm_pSortList, m_numItems, sizeof(CAtomicSorter**), DepthSortFunc);
         }
         
         RwReal m_rDepth;
         RpAtomic *m_pAtomic;
         
         static CAtomicSorter **sm_pSortList;
         static RwUInt32 m_SizeSortList;
         static RwUInt32 m_numItems;
         static RwUInt32 m_refcount;
         
         void *operator new(size_t size)
         {
            return RwFreeListAlloc(sm_pFreeList, rwMEMHINTDUR_GLOBAL);
         }
         
         void operator delete(void *pObj)
         {
            if (pObj)
            {
               RwFreeListFree(sm_pFreeList, pObj);
            }
         }
         
         static void Open(RwUInt32 blockSize)
         {
            m_refcount++;
            
            if (sm_pFreeList == 0)
            {
               sm_pFreeList = RwFreeListCreate(sizeof(CAtomicSorter), blockSize, 16, rwMEMHINTDUR_GLOBAL);
            }
            
            if (sm_pSortList == 0)
            {
               m_SizeSortList = blockSize;
               
               sm_pSortList = reinterpret_cast<CAtomicSorter**>(RwMalloc(sizeof(CAtomicSorter*) * m_SizeSortList, rwMEMHINTDUR_GLOBAL));
            }
         }
         
         static void Close(void)
         {
            m_refcount--;
            
            if (m_refcount == 0)
            {
               if (sm_pFreeList)
               {
                  RwFreeListDestroy(sm_pFreeList);
                  sm_pFreeList = 0;
               }
               
               if (sm_pSortList)
               {
                  RwFree(sm_pSortList);
                  sm_pSortList = 0;
               }
            }
         }
         
         static const RwUInt32 uiDefaultNumberOfAtomicsInSector; /**< Used to specify the initial size of the freelist used to sort atomics per sector */
         
      private:
         
         static void PreSort(void)
         {
            if (m_numItems>m_SizeSortList)
            {
               m_SizeSortList = m_numItems;
               
               sm_pSortList = reinterpret_cast<CAtomicSorter**>(RwRealloc(sm_pSortList, sizeof(CAtomicSorter*) * m_SizeSortList, rwMEMHINTDUR_GLOBAL));
            }
            
            CAtomicSorter *ptr = sm_pHead;
            
            for (RwUInt32 i = 0;i < m_numItems; i++)
            {
               sm_pSortList[i] = ptr;
               
               ptr = ptr->pNext;
            }
         }
         
         CAtomicSorter *pNext;
         
         static RwFreeList *sm_pFreeList;
         static CAtomicSorter *sm_pHead;
      };
       
      /**
      *
      * \ingroup Mod_FPS
      *
      * High level customized rendering behavior, supports worlds with or without PVS data and
      * with or without Lightmap data. Also supports rendering atomics in depth sorted order, which
      * is used to render atomics with translucent materials etc
      *
      */
      class FPSRender : public CEventHandler, public CAttributeHandler, public CSystemCommands, public LinearAllocationPolicy
      {
      public:
         RWS_MAKENEWCLASS(FPSRender);
         RWS_DECLARE_CLASSID(FPSRender);
         RWS_CATEGORY("FPS");
         RWS_DESCRIPTION("Rendering Behavior", "Rendering behavior that supports PVS and light mapping.");
         
         RWS_BEGIN_COMMANDS
            
            RWS_SEPARATOR("Global Worlds CLevel", 0)
            
            // Global world pointers
            //
            RWS_ATTRIBUTE(CMD_GlobalOpaqueWorld, "Global Opaque World","Set this world as the global opaque world, if this is true RpAtomics, RpClumps, and RpLights will be added to this world, see CLevel",BOOLEAN,RwUInt32,DEFAULT(1))
            RWS_ATTRIBUTE(CMD_GlobalTranslucentWorld, "Global Translucent World","Set this world as the global translucent world, if this is true RpAtomics, RpClumps, and RpLights will be added to this world, see CLevel",BOOLEAN,RwUInt32,DEFAULT(0))
            
            RWS_SEPARATOR("Rendering Events", 0)
            
            RWS_MESSAGE  (CMD_Start_Render, "Start Render Event", "Begin rendering specified camera.", RECEIVE , RwCamera*,"iMsgDoRender")
            RWS_MESSAGE  (CMD_End_Render, "End Render Event", "Pass Camera on so that any other rendering can be done.", TRANSMIT , RwCamera*, 0)
            
            RWS_ATTRIBUTE(CMD_priority,  "Render Priority",   "Specify the render priority",   SLIDER,  RwUInt32,   RANGE(0, 65535, 65535))
            
            // Debugging flags
            RWS_SEPARATOR("Enable/Disable Drawing", 0)
            
            RWS_ATTRIBUTE(CMD_DEBUG_WORLD_SECTORS, "Render World Sectors", "Use this flag to enable/disable rendering of sectors, if the world contains PVS data use this flag to enable/disable rendering of World Sectors, Culled sectors are outlined in red drawn sectors in green.", BOOLEAN, RwUInt32, DEFAULT(1))
            RWS_ATTRIBUTE(CMD_DEBUG_WORLD_ATOMICS, "Render World Atomics", "Use this flag to enable/disable rendering of atomics", BOOLEAN, RwUInt32, DEFAULT(1))
            
            // Render state flags
            RWS_SEPARATOR("Render state flags", 0)
            
            RWS_ATTRIBUTE(CMD_rwRENDERSTATECULLMODE, "rwRENDERSTATECULLMODE", "",LIST, RwUInt32, LIST("rwCULLMODECULLNONE|@rwCULLMODECULLBACK|rwCULLMODECULLFRONT"))
            RWS_ATTRIBUTE(CMD_rwRENDERSTATESRCBLEND,  "rwRENDERSTATESRCBLEND", "",LIST, RwUInt32, LIST("rwBLENDZERO|rwBLENDONE|rwBLENDSRCCOLOR|rwBLENDINVSRCCOLOR|@rwBLENDSRCALPHA|rwBLENDINVSRCALPHA|rwBLENDDESTALPHA|rwBLENDINVDESTALPHA|rwBLENDDESTCOLOR|rwBLENDINVDESTCOLOR|rwBLENDSRCALPHASAT"))
            RWS_ATTRIBUTE(CMD_rwRENDERSTATEDESTBLEND, "rwRENDERSTATEDESTBLEND", "",LIST, RwUInt32, LIST("rwBLENDZERO|rwBLENDONE|rwBLENDSRCCOLOR|rwBLENDINVSRCCOLOR|rwBLENDSRCALPHA|@rwBLENDINVSRCALPHA|rwBLENDDESTALPHA|rwBLENDINVDESTALPHA|rwBLENDDESTCOLOR|rwBLENDINVDESTCOLOR|rwBLENDSRCALPHASAT"))
            RWS_ATTRIBUTE(CMD_rwRENDERSTATEVERTEXALPHAENABLE, "rwRENDERSTATEVERTEXALPHAENABLE", "", BOOLEAN, RwUInt32, DEFAULT(0))
            RWS_ATTRIBUTE(CMD_rwRENDERSTATEZWRITEENABLE, "rwRENDERSTATEZWRITEENABLE", "", BOOLEAN, RwUInt32, DEFAULT(1))
            RWS_ATTRIBUTE(CMD_rwRENDERSTATEZTESTENABLE,  "rwRENDERSTATEZTESTENABLE", "", BOOLEAN, RwUInt32, DEFAULT(1))
            
            // Render control flags
            RWS_SEPARATOR("Render order flags", 0)
            
            RWS_ATTRIBUTE(CMD_rpWORLDRENDER, "Order sectors are rendered.","Options available for the rendering order of world sectors in the camera's view frustum.", LIST, RwUInt32, LIST("@rpWORLDRENDERFRONT2BACK|rpWORLDRENDERBACK2FRONT"))
            
            RWS_ATTRIBUTE(CMD_rpATOMICRENDER, "Sorting of atomics", "Options available for the rendering order of atomics, if Depth Sorted is selected then typically sectors should be rendered BACK to FRONT see rpWORLDRENDERBACK2FRONT.", LIST, RwUInt32, LIST("@None|Depth Sorted"))
            
            // PS2 Specific
            RWS_SEPARATOR("PS2 Specific", 0)
            RWS_ATTRIBUTE(CMD_trueclip,                      "PS2 - True Near Clipping",        "PS2 - Only enable/disable true near clipping, see RpSkySelectTrueTSClipper, RpSkySelectTrueTLClipper",BOOLEAN,RwUInt32,DEFAULT(1))
            
            // Light mapping flags
            RWS_SEPARATOR("Light Mapping flags", 0)
            RWS_ATTRIBUTE( CMD_lightmapflags, "Lightmap Flags", "See RenderWare Help, RpLtMapSetRenderStyle",BITFIELD, RwUInt32, LIST("@rpLTMAPSTYLERENDERBASE|@rpLTMAPSTYLERENDERLIGHTMAP|rpLTMAPSTYLEPOINTSAMPLE"))
            
            // Fog settings
            RWS_SEPARATOR("Fog settings", 0)            
            RWS_ATTRIBUTE(CMD_Set_m_FogType,                 "Fog Type",            "Set Fog Type see RwFogType",                       LIST,    RwUInt32,   LIST("None|Linear|Exponential|Exponential 2"))
            RWS_ATTRIBUTE(CMD_Set_m_FogColor,                "Fog Color",          "Set Fog Color, see rwRENDERSTATEFOGCOLOR",        COLOR,  RwRGBA,   DEFAULT(46761))
            RWS_ATTRIBUTE(CMD_Set_m_FogDensity,              "Fog Density",         "Set Fog Density, see rwRENDERSTATEFOGDENSITY",     SLIDER,  RwReal,     RANGE(0, 0, 0.01))
            RWS_ATTRIBUTE(CMD_Set_m_FogDistance,             "Fog Distance",        "Set Fog Distance, see RwCameraSetFogDistance",     SLIDER,  RwReal,     RANGE(-4000, 0, 4000))
            
            // Screen Clearing
            RWS_SEPARATOR("Screen Clear", 0)            
            RWS_ATTRIBUTE(CMD_Set_m_BackgroundColor,         "Bgnd color",         "Set Background clear color, see RwCameraClear",   COLOR,  RwRGBA,   DEFAULT(0))
            
            RWS_ATTRIBUTE(CMD_Set_m_ClearScreen,             "Clear RGBA buffer",   "Enable/Disable raster color buffer clearing",              BOOLEAN, RwUInt32,   DEFAULT(1))
            RWS_ATTRIBUTE(CMD_Set_m_ClearZ,                  "Clear Z buffer",      "Enable/Disable raster Z buffer clearing",                   BOOLEAN, RwUInt32,   DEFAULT(1))
            RWS_ATTRIBUTE(CMD_Set_m_ClearStencil,            "Clear Stencil buffer","Enable/Disable raster stencil buffer clearing",             BOOLEAN, RwUInt32,   DEFAULT(1))
            
            // Near/Far clipping planes
            RWS_SEPARATOR("Near/Far clipping planes", 0)                        
            RWS_ATTRIBUTE(CMD_Set_m_near_clip_factor,        "Near clip %",         "Near clipping plane distance (as percentage of world diagonal)",   SLIDER,  RwReal,     RANGE(0.05, 0.5, 500))
            RWS_ATTRIBUTE(CMD_Set_m_far_clip_factor,         "Far clip %",          "Far clipping plane distance (as percentage of world diagonal)",   SLIDER,  RwReal,     RANGE(5, 100, 500))
            
            RWS_END_COMMANDS;
         
         FPSRender(const CAttributePacket&);
         ~FPSRender(void);
         
         virtual void HandleEvents(CMsg &pMsg);
         virtual void HandleAttributes(const CAttributePacket& attr);
         
      protected:
         void DoRender( RwCamera *pRwCamera );
         
         RwBlendFunction Map_AttributeListToBlendMode(RwUInt32 var) const;
         
         // Render worlds with PVS data
         //
         static RpWorldSector *PVS_WorldSectorRender(RpWorldSector * sector, void *pData);
#ifdef RWS_DESIGN
         static RpWorldSector *PVS_WorldSectorRender_ShowSectors(RpWorldSector * sector, void *pData);
#endif
         static RpWorldSector *PVS_WorldAtomicsRender(RpWorldSector * sector, void *pData);
         
         void WorldRenderPVS(RwCamera *pCamera);
         
         // Render worlds without PVS data
         //
         static RpWorldSector *WorldSectorRender(RpWorldSector * sector, void *pData);
         
#ifdef RWS_DESIGN
         static RpWorldSector *WorldSectorRender_ShowSectors(RpWorldSector * sector, void *pData);
#endif
         static RpWorldSector *WorldAtomicsRender(RpWorldSector * sector, void *pData);
         
         void WorldRender(RwCamera *pCamera);
         
         // Render atomics
         //
         static void WorldSectorRenderAtomics(RpWorldSector * worldSector);

         // Render depth sorted atomics
         //
         static RwReal AtomicGetCameraDistance(RpAtomic *atomic);
         
         static void WorldSectorRenderDepthSortedAtomics(RpWorldSector * worldSector);
         
         // Select rendering method.
         //
         typedef void (FPSRender::*WorldRenderFunctionPtr)(RwCamera *pCamera);
         
         WorldRenderFunctionPtr m_WorldRenderFunctionPtr;
         
         typedef void (*AtomicRenderFunctionPtr)(RpWorldSector * worldSector);
         
         AtomicRenderFunctionPtr m_AtomicRenderFunctionPtr;
         
#ifdef SKY
         void SetClippingMode(RwBool trueclip);
#endif
         
         void SetWorldRenderMode(void);
         
         void UpdateClipValues();
         
         RwCullMode m_rwRENDERSTATECULLMODE;              /**< see Renderware help, rwRENDERSTATECULLMODE */
         RwBlendFunction m_rwRENDERSTATESRCBLEND;         /**< see RenderWare help, rwRENDERSTATESRCBLEND */
         RwBlendFunction m_rwRENDERSTATEDESTBLEND;        /**< see Renderware help, rwRENDERSTATEDESTBLEND */
         RwUInt32 m_rwRENDERSTATEVERTEXALPHAENABLE;       /**< see RenderWare help, rwRENDERSTATEVERTEXALPHAENABLE */
         RwUInt32 m_rwRENDERSTATEZWRITEENABLE;            /**< see Renderware help, rwRENDERSTATEZWRITEENABLE */
         RwUInt32 m_rwRENDERSTATEZTESTENABLE;             /**< see RenderWare help, rwRENDERSTATEZTESTENABLE */       
         
         RwFogType m_FogType;                             /**< Fog Type  */
         RwRGBA m_FogColor;                             /**< Fog Color RGB  */
         RwReal m_FogDensity;                             /**< Fog Density   */
         RwReal m_FogDistance;                            /**< Fog Distance  */
         RwRGBA m_BackgroundColor;                        /**< Background clear color RGB  */
         
         RwInt32 m_clearMode;                             /**< Flags for clearing rasters */
         
         RwReal m_near_clip_factor;                       /**< Near clipping plane factor  */
         RwReal m_far_clip_factor;                        /**< Far clipping plane factor  */        
         
         RpWorld *m_RpWorld;                              /**< Pointer to world, owned by this behavior */
         
#ifdef SKY
         RwUInt32 m_trueclip;                             /**< Enable/Disable true clipping note PS2 only */
#endif
         
         CEventId m_StartRender;                          /**< Event used to specify when rendering should begin with the specified RwCamera */
         CEventId m_EndRender;                            /**< Event used to specify when rendering should begin with the specified RwCamera */
         
         RwBool m_DrawWorldSectors;     
         RwBool m_DrawWorldAtomics;
         
         RpWorldRenderOrder m_rpWORLDRENDER;             /**< see RenderWare GFX RpWorldRenderOrder */
      };
   }
}

#endif
