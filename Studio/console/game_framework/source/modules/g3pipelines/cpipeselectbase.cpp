/*****************************************************************************
 *
 * File :     CPipeSelectBase.cpp
 *
 * Abstract:   Implementation of CG3PipeSelectBase.cpp
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
 * Copyright (c) 2001 Criterion Software Ltd.
 * All Rights Reserved.
 *
 * RenderWare is a trademark of Canon Inc.
 *
 *****************************************************************************/

#include "precomp.h"

#if defined (SKY)

// Left to its own devices, the MW linker will dead-strip the registrar ctors, so...
#ifdef __MWERKS__
#pragma force_active on
#pragma exceptions off // Turn off exceptions due to bug in MW Dwarf Generator for PS2, present in CW 3.04
#endif

//#define RWS_CALLSTACKENABLE
//#define RWS_TRACEENABLE

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "framework/core/macros/debugmacros.h"
#include "framework/core/resourcemanager/cresourcemanager.h"
#include "cpipeselectbase.h"

namespace RWS
{
   namespace Pipelines
   {
      /**
      *
      *  Construct CPipeSelectBase object, the parameters of the constructor are used to select the pipelines
      *  and render callback which is applied to each of the assets in HandleAttributes.
      *
      *  \param rAttr Reference to a CAttributePacket
      *
      *  \param MaterialPipeID Id of RenderWare Gfx Material Pipeline \see RpPDSPipeID
      *
      *  \param AtomicPipeID Id of RenderWare Gfx Atomic Pipeline \see RpPDSPipeID
      *
      *  \param AtomicCallBackRender The Atomic Render Callback used to setup the lighting for the pipeline.
      */
      CPipeSelectBase::CPipeSelectBase(const CAttributePacket& rAttr,
         RpPDSPipeID MaterialPipeID,
         
         RpPDSPipeID AtomicPipeID,
         RpAtomicCallBackRender AtomicCallBackRender,

         RpPDSPipeID SectorPipeID,
         RpWorldSectorCallBackRender WorldSectorCallBackRender):

         m_MaterialRxPipeline(0), m_AtomicsRxPipeline(0), m_SectorRxPipeline(0)
      {
         RWS_FUNCTION("RWS::Pipelines::CPipeSelectBase::CPipeSelectBase");

         m_AtomicCallBackRender = AtomicCallBackRender;
         m_WorldSectorCallBackRender = WorldSectorCallBackRender;

         if (MaterialPipeID)
         {
            m_MaterialRxPipeline = RpPDSGetPipe (MaterialPipeID);

            RWS_ASSERT(m_MaterialRxPipeline,
               "RpPDSGetPipe Failed to locate the material pipeline, please check it has been registered.");
         }

         if (AtomicPipeID)
         {
            m_AtomicsRxPipeline = RpPDSGetPipe (AtomicPipeID);

            RWS_ASSERT(m_AtomicsRxPipeline,
               "RpPDSGetPipe Failed to locate the atomic pipeline, please check it has been registered.");
         }

         if (SectorPipeID)
         {
            m_SectorRxPipeline = RpPDSGetPipe( SectorPipeID);

            RWS_ASSERT(m_SectorRxPipeline,
               "RpPDSGetPipe Failed to locate the WorldSector pipeline, please check it has been registered.");
         }

         RWS_TRACE("m_AtomicCallBackRender "<<m_AtomicCallBackRender);

         RWS_RETURNVOID();
      }

      /**
      *
      *  Handle attributes of CPipeSelectBase, the attributes of the CPipeSelectBase class will be the
      *  assets associated with the derived class, the pipelines specified in the constructor are
      *  attached to each of these assets.
      *
      *  \param rAttr Reference to a CAttributePacket object, \see CAttributeHandler
      */
      void CPipeSelectBase::HandleAttributes(const CAttributePacket& rAttr)
      {
         RWS_FUNCTION("RWS::Pipelines::CPipeSelectBase::HandleAttributes");

         // Find and process each of the attached assets.
         //
         CAttributeCommandIterator sysattrIt(rAttr, RWS_CLASSID_OF(CSystemCommands));

         while (!sysattrIt.IsFinished())
         {
            switch (sysattrIt->GetCommandId())
            {
               case CSystemCommands::CMD_AttachResource:
                  {
                     const RwChar *resourceType;
      
                     // Const cast is requires as we are going to modify the actual resource.
                     //
                     void *pObject = 0;
                     const RWSGUID * pResourceId = 0;
                     sysattrIt->GetCommandData(&pResourceId);
                     if (pResourceId)
                     {
                        pObject = const_cast<void*>(CResourceManager::FindById(pResourceId, &resourceType));
                     }
      
                     RWS_ASSERT(pObject, "Resource::Find failed id = " << pResourceId);

                     if (resourceType)
                     {
                        if (!rwstrcmp("rwID_ATOMIC", resourceType))
                        {
                           if (m_AtomicsRxPipeline)  AttachAtomicPipeLine(reinterpret_cast<RpAtomic *>(pObject));
                           if (m_MaterialRxPipeline)   AttachMaterialPipeLine(reinterpret_cast<RpAtomic *>(pObject));
                        }
                        else if (!rwstrcmp("rwID_CLUMP", resourceType))
                        {
                           if (m_AtomicsRxPipeline)  AttachAtomicPipeLine(reinterpret_cast<RpClump *>(pObject));
                           if (m_MaterialRxPipeline)   AttachMaterialPipeLine(reinterpret_cast<RpClump *>(pObject));
                        }
                        else if (!rwstrcmp("rwID_WORLD", resourceType))
                        {
                           if (m_AtomicsRxPipeline)  AttachWorldPipeLine(reinterpret_cast<RpWorld *>(pObject));
                           if (m_MaterialRxPipeline)   AttachMaterialPipeLine(reinterpret_cast<RpWorld *>(pObject));                         }
                        else
                        {
                           RWS_ASSERTFAIL("CPipeSelectBase can only process atomics and clumps and worlds.");
                        }
                     }
                  }
                  break;
            }; 
            ++sysattrIt;
         }

         RWS_RETURNVOID();
      }

      /**
      *
      *  Attach an atomic pipeline to the specified atomic.
      *
      *  \param pAtomic Pointer to a RenderWare Gfx RpAtomic object.
      */
      void CPipeSelectBase::AttachAtomicPipeLine(RpAtomic *pAtomic)
      {
         RWS_FUNCTION("RWS::Pipelines::CPipeSelectBase::AttachAtomicPipeLine(RpAtomic *pAtomic)");

         RpAtomicSetPipeline(pAtomic, m_AtomicsRxPipeline);

         if (m_AtomicCallBackRender) RpAtomicSetRenderCallBack (pAtomic, m_AtomicCallBackRender);
 
         RWS_RETURNVOID();
      }

      namespace
      {
         struct AttachAtomicPipeLineCB_data
         {
            RxPipeline *pRxPipeline;
            RpAtomicCallBackRender AtomicCallBackRender;
         };

         /*
         *
         *  Callback to set a pipeline for the specified atomic, used by CPipeSelectBase::AttachAtomicPipeLine
         *
         *  \param pAtomic Pointer to RenderWare Gfx RpAtomic object.
         *
         *  \param pData Pointer to additional data, in this case a RxPipeline.
         */
         RpAtomic *AttachAtomicPipeLineCB(RpAtomic *pAtomic, void *pData)
         {
            RWS_FUNCTION("RWS::Pipelines::NULL::AttachAtomicPipeLineCB");

            AttachAtomicPipeLineCB_data *pAttachAtomicPipeLineCB_data =
               reinterpret_cast<AttachAtomicPipeLineCB_data*>(pData);

            RpAtomicSetPipeline(pAtomic, pAttachAtomicPipeLineCB_data->pRxPipeline);

            if (pAttachAtomicPipeLineCB_data->AtomicCallBackRender)
               RpAtomicSetRenderCallBack (pAtomic, pAttachAtomicPipeLineCB_data->AtomicCallBackRender);
 
            RWS_RETURN(pAtomic);
         }
      }

      /**
      *
      *  Attach an atomic pipeline to each of the atomics in the specified clump. 
      *
      *  \param pClump Pointer to a RenderWare Gfx RpClump object.
      */
      void CPipeSelectBase::AttachAtomicPipeLine(RpClump *pClump)
      {
         RWS_FUNCTION("RWS::Pipelines::CPipeSelectBase::AttachAtomicPipeLine(RpClump *pClump)");

         AttachAtomicPipeLineCB_data tAttachAtomicPipeLineCB_data;

         tAttachAtomicPipeLineCB_data.pRxPipeline = m_AtomicsRxPipeline;
         tAttachAtomicPipeLineCB_data.AtomicCallBackRender = m_AtomicCallBackRender;

         RpClumpForAllAtomics(pClump, AttachAtomicPipeLineCB, &tAttachAtomicPipeLineCB_data);

         RWS_RETURNVOID();
      }

      namespace
      {
         /*
         *
         *  Callback to set a material pipeline for the specified material.
         *
         *  \param pMaterial Pointer to RenderWare Gfx RpMaterial object.
         *
         *  \param pData Pointer to additional data, in this case a RxPipeline.
         */
         RpMaterial *AttachMaterialPipeLineCB(RpMaterial *pMaterial, void *pData)
         {
            RWS_FUNCTION("RWS::Pipelines::NULL::AttachMaterialPipeLineCB");

            RxPipeline *pRxPipeline = reinterpret_cast<RxPipeline*>(pData);

            RpMaterialSetPipeline(pMaterial, pRxPipeline);

            RWS_RETURN(pMaterial);
         }
      }

      /**
      *
      *  Attach a material pipeline to each of the materials in the specified atomic.
      *
      *  \param pAtomic Pointer to a RenderWare Gfx RpAtomic object.
      */
      void CPipeSelectBase::AttachMaterialPipeLine(RpAtomic *pAtomic)
      {
         RWS_FUNCTION("RWS::Pipelines::CPipeSelectBase::AttachMaterialPipeLine(RpAtomic *pAtomic)");

         RpGeometry *pRpGeometry = RpAtomicGetGeometry(pAtomic);

         RpGeometryForAllMaterials(pRpGeometry, AttachMaterialPipeLineCB, m_MaterialRxPipeline);

         if (m_AtomicCallBackRender) RpAtomicSetRenderCallBack (pAtomic, m_AtomicCallBackRender);

         RWS_RETURNVOID();
      }

      namespace
      {
         struct AttachMaterialToAtomicPipeLineCB_data
         {
            RxPipeline *pRxPipeline;
            RpAtomicCallBackRender AtomicCallBackRender;
         };

         /*
         *
         *  Callback to set a material pipeline for each of the materials of the specified atomic,
         *  used by CPipeSelectBase::AttachMaterialPipeLine
         *
         *  \param pAtomic Pointer to RenderWare Gfx RpAtomic object.
         *
         *  \param pData Pointer to additional data, in this case a RxPipeline.
         */
         RpAtomic *AttachMaterialToAtomicPipeLineCB(RpAtomic *pAtomic, void *pData)
         {
            RWS_FUNCTION("RWS::Pipelines::NULL::AttachAtomicPipeLineCB");

            AttachMaterialToAtomicPipeLineCB_data *pAttachMaterialToAtomicPipeLineCB_data =
               reinterpret_cast<AttachMaterialToAtomicPipeLineCB_data*>(pData);

            RpGeometry *pRpGeometry = RpAtomicGetGeometry(pAtomic);

            RpGeometryForAllMaterials(pRpGeometry, AttachMaterialPipeLineCB,
               pAttachMaterialToAtomicPipeLineCB_data->pRxPipeline);
            
            if (pAttachMaterialToAtomicPipeLineCB_data->AtomicCallBackRender)
               RpAtomicSetRenderCallBack (pAtomic, pAttachMaterialToAtomicPipeLineCB_data->AtomicCallBackRender);

            RWS_RETURN(pAtomic);
         }
      }

      /**
      *
      *  Attach a material pipeline to each of the materials in each of the atomics of the specified clump
      *
      *  \param pAtomic Pointer to a RenderWare Gfx RpAtomic object.
      */
      void CPipeSelectBase::AttachMaterialPipeLine(RpClump *pClump)
      {
         RWS_FUNCTION("RWS::Pipelines::CPipeSelectBase::AttachMaterialPipeLine(RpClump *pClump)");

         AttachMaterialToAtomicPipeLineCB_data tAttachMaterialToAtomicPipeLineCB_data;

         tAttachMaterialToAtomicPipeLineCB_data.pRxPipeline = m_MaterialRxPipeline;
         tAttachMaterialToAtomicPipeLineCB_data.AtomicCallBackRender = m_AtomicCallBackRender;

         RpClumpForAllAtomics(pClump, AttachMaterialToAtomicPipeLineCB, &tAttachMaterialToAtomicPipeLineCB_data);

         RWS_RETURNVOID();
      }

      namespace
      {
         struct AttachWorldSectorPipeLineCB_data
         {
            RxPipeline *pRxPipeline;
         };

         /*
         *
         *  Callback to set a pipeline for the specified atomic, used by CPipeSelectBase::AttachAtomicPipeLine
         *
         *  \param pAtomic Pointer to RenderWare Gfx RpAtomic object.
         *
         *  \param pData Pointer to additional data, in this case a RxPipeline.
         */
         RpWorldSector *AttachSectorPipeLineCB(RpWorldSector *pWorldSector, void *pData)
         {
            RWS_FUNCTION("RWS::Pipelines::NULL::AttachAtomicPipeLineCB");

            AttachWorldSectorPipeLineCB_data *pAttachWorldSectorPipeLineCB_data =
               reinterpret_cast<AttachWorldSectorPipeLineCB_data*>(pData);

            RpWorldSectorSetPipeline(pWorldSector, pAttachWorldSectorPipeLineCB_data->pRxPipeline);

            RWS_RETURN(pWorldSector);
         }
      }

      /**
      *
      *  Attach a world pipeline to the specified world. 
      *
      *  \param pWorld Pointer to a RenderWare Gfx RpWorld object.
      */
      void CPipeSelectBase::AttachWorldPipeLine(RpWorld *pWorld)
      {
         AttachWorldSectorPipeLineCB_data tAttachWorldSectorPipeLineCB_data;

         tAttachWorldSectorPipeLineCB_data.pRxPipeline = m_SectorRxPipeline;

         RpWorldForAllWorldSectors(pWorld, AttachSectorPipeLineCB, &tAttachWorldSectorPipeLineCB_data);

         if (m_WorldSectorCallBackRender)
         {
            RpWorldSetSectorRenderCallBack (pWorld, m_WorldSectorCallBackRender);
         }
      }

      /**
      *
      *  Attach a material pipeline to each of the materials in the specified atomic.
      *
      *  \param pAtomic Pointer to a RenderWare Gfx RpAtomic object.
      */
      void CPipeSelectBase::AttachMaterialPipeLine(RpWorld *pWorld)
      {
         RWS_FUNCTION("RWS::Pipelines::CPipeSelectBase::AttachMaterialPipeLine(RpWorld *pWorld)");

         RpWorldForAllMaterials (pWorld, AttachMaterialPipeLineCB, m_MaterialRxPipeline);

         RWS_RETURNVOID();
      }
   }
}

#endif
