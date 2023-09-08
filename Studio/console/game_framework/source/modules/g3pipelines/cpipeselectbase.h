/*****************************************************************************
 *
 * File :     CPipeSelectBase.h
 *
 * Abstract:   Declaration of CG3PipeSelectBase.h
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


#ifndef __CPipeSelectBase_H__
#define __CPipeSelectBase_H__

#if defined (SKY)

//////////////////////////////////////////////////////////////////
//
// RenderWare Includes
//
#include "rppds.h"

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "framework/core/attributehandler/cattributehandler.h"
#include "framework/core/attributehandler/csystemcommands.h"

#include "framework/core/attributehandler/allocation policies/LinearAllocationPolicy.h"

namespace RWS
{
   namespace Pipelines
   {
     /**
      *
      *  \ingroup Mod_Pipelines
      *
      *  CPipeSelectBase provides functionality for attaching pipelines to various RenderWare assets, i.e.
      *  Atomics, Clumps and Worlds. 
      *
      */
      class CPipeSelectBase: public CAttributeHandler, public LinearAllocationPolicy
      {
         public:

            RWS_IMPORT_COMMANDS(CSystemCommands);

            virtual void HandleAttributes(const CAttributePacket& attr);

         protected:

            CPipeSelectBase(  const CAttributePacket& rAttr,
                              RpPDSPipeID MaterialPipeID,

                              RpPDSPipeID AtomicPipeID,
                              RpAtomicCallBackRender AtomicCallBackRender,

                              RpPDSPipeID SectorPipeID,
                              RpWorldSectorCallBackRender WorldSectorCallBackRender);

            void AttachMaterialPipeLine(RpAtomic *pAtomic);
            void AttachMaterialPipeLine(RpClump *pClump);
            void AttachMaterialPipeLine(RpWorld *pWorld);

            void AttachAtomicPipeLine(RpAtomic *pAtomic);
            void AttachAtomicPipeLine(RpClump *pClump);

            void AttachWorldPipeLine(RpWorld *pWorld);
 
            RxPipeline *m_MaterialRxPipeline;                        /**< Renderware Gfx RxPipeline for Material*/

            RxPipeline *m_AtomicsRxPipeline;                         /**< RenderWare Gfx RxPipeline for Atomic*/

            RxPipeline *m_SectorRxPipeline;                         /**< Renderware Gfx RxPipeline for Sectors*/

            RpAtomicCallBackRender m_AtomicCallBackRender;           /**< RenderWare Gfx RpAtomic render callback*/

            RpWorldSectorCallBackRender m_WorldSectorCallBackRender; /**< RenderWare Gfx RpWorldSector render callback*/
      };
   }
}
#endif
#endif
