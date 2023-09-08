/*****************************************************************************
*
* File :     CFXReplicator.h
*
* Abstract : General purpose particle entity
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

#ifndef __CFXReplicator_H__
#define __CFXReplicator_H__

#include "rwcore.h"
#include "framework/core/attributehandler/cattributehandler.h"
#include "framework/core/eventhandler/ceventhandler.h"
#include "framework/toolkits/world/smartptrs/catomicptr.h"
#include "framework/toolkits/world/smartptrs/cclumpptr.h"
#include "framework/mainloop/logic.h"
#include "framework/core/attributehandler/allocation policies/LinearAllocationPolicy.h"

namespace RWS
{
   namespace FX
   {
   /**
   *
   *  Container for CFXReplicator velocity, acceleration etc
   *
      */
      class CFXReplicatorDynamics
      {
      public:
         CFXReplicatorDynamics() : Time (0)
         {
         }

         // Per Particle
         //
         RwV3d vXYZ;                 /**< Velocity */
         RwV3d aXYZ;                 /**< Acceleration */
         RwV3d fXYZ;                 /**< Friction */

         RwV3d avXYZ;                   /**< Angular Velocity */
         RwV3d aaXYZ;                   /**< Angular Acceleration */
         RwV3d afXYZ;                   /**< Angular Friction */

         RwV3d iScale;               /**< Initial Scale factor */
         RwV3d vScale;               /**< Each Frame Scale factor */
         RwInt32 TimeOut;            /**< Expirary Time In Frames */

         RwReal TimeStep;            /**< Time is incremented by this value each logic tick */
         RwReal Time;                /**< Current time, 0..1 over lifetime of CReplicator */

         // All Particles
         //
         RwRGBAReal c0;              /**< color control point 1*/
         RwRGBAReal c1;              /**< color control point 2*/
         RwRGBAReal c2;              /**< color control point 3*/
         RwRGBAReal c3;              /**< color control point 4*/
         RwRGBAReal c4;              /**< color control point 5*/

         // These messages can be used to create more interesting particle effects
         //
         CEventId Msg_Particle_Expired; /**< Message sent when CReplicator expires, usefull for joining CReplicatorGenerator. */
         CEventId Msg_Particle_Active;  /**< Message sent while the CReplicator is active */
      };

      class CFXReplicator : public CEventHandler
      {
      public:
         virtual void HandleEvents(CMsg &pMsg);

         CFXReplicator(const CFXReplicatorDynamics &ParticleDynamics);
         ~CFXReplicator();

         void UpdateParticleAtomic( void );

         CFXReplicatorDynamics m_ParticleDynamics;      /**< \ref CReplicatorDynamics */

         CAtomicPtr m_pAtomic;                          /**< \ref CAtomicPtr */

         void SetRenderCallBack( void );

         static RpAtomic *AtomicRenderCallBack(RpAtomic *pAtomic);

         static CEventId sm_GetCFXReplicatorPtr;

      protected:
      };
   }
}

#endif
