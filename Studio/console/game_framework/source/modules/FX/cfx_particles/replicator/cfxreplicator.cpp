/*****************************************************************************
*
* File :     CReplicator.h
*
* Abstract : General purpose particle generator CReplicatorGenerator, this particle
* generator is not based on the RenderWare Particle Effects system rpprtsys.h. But creates
* individually controlled particles each with its own atomic, each particle is an instance 
* of the class CReplicator.
*
* The particle generator can be used to create dynamic effects such as trails, explosions,
* smoke, dust. Environment effects such as fountains, snow, rain etc would be more suited 
* to rpprtsys.h.
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
//////////////////////////////////////////////////////////////////
//
// Renderwarestudio Includes
//
#include "cfxreplicator.h"
#include "framework/toolkits/math/maths.h"
#include "framework/mainloop/render.h"
#include "framework/toolkits/world/clevel.h"


namespace RWS
{
   namespace FX
   {
      CEventId CFXReplicator::sm_GetCFXReplicatorPtr;
   
      /**
      *
      * CFXReplicator Constructor
      *
      */
      CFXReplicator::CFXReplicator(const CFXReplicatorDynamics &ParticleDynamics) : InitCEventHandler(&m_pAtomic)
      {
         RWS_FUNCTION("RWS::FX::CFXReplicator::CFXReplicator");

         // This event is Registered unregistered every time CFXReplicator is constructed, this
         // will be relatively slow as we only want to keep track of how many times its registered
         // i.e. we don't need to keep doing the registration checks.
         //
         // So in this case the first time the constructor is called RegisterMsg is called, from then on
         // only Inc_registered_count is called.
         //
         // Similarly in the destructor.
         //
         if (sm_GetCFXReplicatorPtr.Get_registered_count() == 0)
         {
            RegisterMsg(sm_GetCFXReplicatorPtr, "GetCFXReplicatorPtr", "return CFXReplicator*");
         }
         else
         {
            sm_GetCFXReplicatorPtr.Inc_registered_count();
         }
      
         LinkMsg (iMsgRunningTick, 0);
         
         // To make sure this object is destroyed at the end of a level,
         // register with the CEventHandler auto-delete mechanism.
         RegisterForAutoDelete();
      
         m_ParticleDynamics = ParticleDynamics;

         RWS_RETURNVOID();
      }
   
      /**
      *
      * CFXReplicator Constructor
      *
      */
      CFXReplicator::~CFXReplicator()
      {
         RWS_FUNCTION("RWS::FX::CFXReplicator::~CFXReplicator");

         if (sm_GetCFXReplicatorPtr.Get_registered_count() == 1)
         {
            UnRegisterMsg(sm_GetCFXReplicatorPtr);
         }
         else
         {
            sm_GetCFXReplicatorPtr.Dec_registered_count();
         }

         UnLinkMsg (iMsgRunningTick);

         UnRegisterMsg(m_ParticleDynamics.Msg_Particle_Active);
         UnRegisterMsg(m_ParticleDynamics.Msg_Particle_Expired);

         RWS_RETURNVOID();
      }
   
      //////////////////////////////////////////////////////////////////
      //
      // Evaluate a Catmull-Rom / Overhauser spline at a specific position along it
      //
      // Note: This type of spline passes through all control points.
      //
      // Given control points in {v0, v1, v2, v3} and a value 0<=t<=1.0f this function returns points on the
      // spline between v1 and v2
      //
      // To do more than two points just step through the array of points using the previous point, 
      // the current point and the next two points as the four points for the spline. For each of these segments draw a curve for 0<t<1. 
      // This curve will be between the current point and the next point. 
      //
      // In : v0, v1, v2, v3 = 4 vectors representing the spline control points.
      //                    t = parametric step 0.0 > 1.0
      //                  vr = pointer to vector to receive point
      //
      // EvaluateColor extends this to evaluate a color along 5 control points, note 5 points gives you 4 sections
      //
      // c0 -> c1 
      // c1 -> c2
      // c2 -> c3
      // c3 -> c4
      //
      void EvaluateColor(  const RwRGBAReal &c0,
                           const RwRGBAReal &c1,
                           const RwRGBAReal &c2,
                           const RwRGBAReal &c3,
                           const RwRGBAReal &c4,
                           const RwReal t,
                           RwRGBAReal &cr)
      {
         RWS_FUNCTION("RWS::FX::EvaluateColor");

         RwReal      s = t * 4.0f;  // 4 sections
      
         RwReal    spow2;
         RwReal    spow3;
      
         const RwRGBAReal *tcr0;
         const RwRGBAReal *tcr1;
         const RwRGBAReal *tcr2;
         const RwRGBAReal *tcr3;
      
         if (s<1.0f)
         {
            tcr0 = &c0;         tcr1 = &c0;         tcr2 = &c1;         tcr3 = &c2;
         }
         else
            if (s<2.0f)
            {
               tcr0 = &c0;         tcr1 = &c1;         tcr2 = &c2;         tcr3 = &c3;
            
               s = s - 1.0f;  // 0..1
            }
            else if (s<3.0f)
            {
               tcr0 = &c0;         tcr1 = &c2;         tcr2 = &c3;         tcr3 = &c3;
            
               s = s - 2.0f;  // 0..1
            }
            else
            {
               tcr0 = &c2;         tcr1 = &c3;         tcr2 = &c4;         tcr3 = &c4;
            
               s = s - 3.0f;  // 0..1
            }
         
            // Generate point on spline
            //
            spow2 = s * s;
            spow3 = s * spow2;
         
            cr.red = 0.5f * (2.0f * tcr1->red + s * (tcr2->red - tcr0->red) 
               + spow2 * (2.0f * tcr0->red - 5.0f * tcr1->red + 4.0f * tcr2->red - tcr3->red) 
               + spow3 * (3.0f * tcr1->red - 3.0f * tcr2->red + tcr3->red - tcr0->red));
         
            cr.green = 0.5f * (2.0f * tcr1->green + s * (tcr2->green - tcr0->green) 
               + spow2 * (2.0f * tcr0->green - 5.0f * tcr1->green + 4.0f * tcr2->green - tcr3->green) 
               + spow3 * (3.0f * tcr1->green - 3.0f * tcr2->green + tcr3->green - tcr0->green));
         
            cr.blue = 0.5f * (2.0f * tcr1->blue + s * (tcr2->blue - tcr0->blue) 
               + spow2 * (2.0f * tcr0->blue - 5.0f * tcr1->blue + 4.0f * tcr2->blue - tcr3->blue) 
               + spow3 * (3.0f * tcr1->blue - 3.0f * tcr2->blue + tcr3->blue - tcr0->blue));
         
            cr.alpha = 0.5f * (2.0f * tcr1->alpha + s * (tcr2->alpha - tcr0->alpha)
               + spow2 * (2.0f * tcr0->alpha - 5.0f * tcr1->alpha + 4.0f * tcr2->alpha - tcr3->alpha) 
               + spow3 * (3.0f * tcr1->alpha - 3.0f * tcr2->alpha + tcr3->alpha - tcr0->alpha));

            // Clamp rgba values
            //
            if (cr.red < 0) cr.red = 0.0f;
            if (cr.red > 1.0f) cr.red = 1.0f;
         
            if (cr.green < 0) cr.green = 0.0f;
            if (cr.green > 1.0f) cr.green = 1.0f;
         
            if (cr.blue < 0) cr.blue = 0.0f;
            if (cr.blue > 1.0f) cr.blue = 1.0f;
         
            if (cr.alpha < 0) cr.alpha = 0.0f;
            if (cr.alpha > 1.0f) cr.alpha = 1.0f;

            RWS_RETURNVOID();
      }
   
      /**
      *
      * Update Position of Particle atomic
      *
      */
      void CFXReplicator::UpdateParticleAtomic(void)
      {
         RWS_FUNCTION("RWS::FX::CFXReplicator::UpdateParticleAtomic");

         // Send Active Message each tick...
         //
         if (m_ParticleDynamics.Msg_Particle_Active.Get_registered_count()>0)
         {
            CMsg tMsg(m_ParticleDynamics.Msg_Particle_Active, RpAtomicGetFrame(m_pAtomic.ptr()));

            SendMsg(tMsg);
         }
      
         // Increment Spline Time by TimeStep
         m_ParticleDynamics.Time += m_ParticleDynamics.TimeStep;
      
         // Decrement expirary time delete instance if <= 0
         if (--m_ParticleDynamics.TimeOut <= 0)
         {
            // Send expire event
            //
            if (m_ParticleDynamics.Msg_Particle_Expired.Get_registered_count()>0)
            {
               CMsg tMsg(m_ParticleDynamics.Msg_Particle_Expired, RpAtomicGetFrame(m_pAtomic.ptr()));

               SendMsg(tMsg);
            }

            Delete();
         }
         else
         {
            // The atomic rendering callback posts a message to tell the event handler to render the atomic.  
            // this is so we can make sure the particle atomic is the last thing rendered.
            RwFrame *frame = RpAtomicGetFrame(m_pAtomic.ptr());
         
            if (frame)
            {             
               // Apply rotations
               RwFrameRotate (frame, &XAxis, m_ParticleDynamics.avXYZ.x, rwCOMBINEPRECONCAT);
               RwFrameRotate (frame, &YAxis, m_ParticleDynamics.avXYZ.y, rwCOMBINEPRECONCAT);
               RwFrameRotate (frame, &ZAxis, m_ParticleDynamics.avXYZ.z, rwCOMBINEPRECONCAT);
            
               // Add acceleration
               RwV3dAdd (&m_ParticleDynamics.avXYZ, &m_ParticleDynamics.avXYZ, &m_ParticleDynamics.aaXYZ);  
            
               // Apply friction
               m_ParticleDynamics.avXYZ.x *= m_ParticleDynamics.afXYZ.x;
               m_ParticleDynamics.avXYZ.y *= m_ParticleDynamics.afXYZ.y;
               m_ParticleDynamics.avXYZ.z *= m_ParticleDynamics.afXYZ.z;
            
               // Apply Translation
               RwFrameTranslate (frame, &m_ParticleDynamics.vXYZ, rwCOMBINEPOSTCONCAT);
            
               // Apply acceleration
               RwV3dAdd (&m_ParticleDynamics.vXYZ, &m_ParticleDynamics.vXYZ, &m_ParticleDynamics.aXYZ);  
            
               // Apply scale
               RwFrameScale (frame, &m_ParticleDynamics.vScale, rwCOMBINEPRECONCAT);
            
               // Apply friction
               m_ParticleDynamics.vXYZ.x *= m_ParticleDynamics.fXYZ.x;
               m_ParticleDynamics.vXYZ.y *= m_ParticleDynamics.fXYZ.y;
               m_ParticleDynamics.vXYZ.z *= m_ParticleDynamics.fXYZ.z;
            }
         }

         RWS_RETURNVOID();
      }
   
      /**
      *
      */
      RpAtomic *CFXReplicator::AtomicRenderCallBack(RpAtomic *pAtomic)
      {
         RWS_FUNCTION("RWS::FX::CFXReplicator::AtomicRenderCallBack");

         // Need to access the data stored in the CFXReplicator associated with this atomic
         //
         CEventHandler *pEventHandler = RpAtomicToEventHandler::GetEventHandler(*pAtomic);  
      
         CMsg Msg(sm_GetCFXReplicatorPtr);
      
         pEventHandler->HandleEvents(Msg);
      
         // This is normally illegal but we know for sure that this atomic is owned
         // by the CFXReplicator class so we can do the class.
         //
         CFXReplicator *pCFXReplicator = reinterpret_cast<CFXReplicator*>(Msg.pData);
      
         RwRGBA RGBA;         
         RwRGBAReal RGBAReal;
      
         // Evaluate color at point Time along spline
         EvaluateColor(pCFXReplicator->m_ParticleDynamics.c0,
            pCFXReplicator->m_ParticleDynamics.c1,
            pCFXReplicator->m_ParticleDynamics.c2,
            pCFXReplicator->m_ParticleDynamics.c3,
            pCFXReplicator->m_ParticleDynamics.c4,
            pCFXReplicator->m_ParticleDynamics.Time,
            RGBAReal);
      
         RwRGBAFromRwRGBAReal(&RGBA, &RGBAReal);
      
         RpGeometry *pGeometry = RpAtomicGetGeometry(pAtomic);
      
         if (pGeometry)
         {
            for (int i = 0; i < RpGeometryGetNumMaterials(pGeometry) ; i++)
            {     
               if (pGeometry)
               {
                  RpGeometrySetFlags  (  pGeometry, RpGeometryGetFlags(pGeometry) | rpGEOMETRYMODULATEMATERIALCOLOR);
               
                  (void) RpMaterialSetColor  (  RpGeometryGetMaterial (pGeometry, i),   &RGBA); 
               }
            }
         }
      
         RWS_RETURN(AtomicDefaultRenderCallBack(pAtomic));
      }
   
      /**
      *
      * Handle Events
      *  
      */
      void CFXReplicator::HandleEvents(CMsg &pMsg)   
      {
         RWS_FUNCTION("RWS::FX::CFXReplicator::HandleEvents");

         // Process running tick message
         if (pMsg.Id == iMsgRunningTick)
         {
            UpdateParticleAtomic();
         }
         else if (pMsg.Id == sm_GetCFXReplicatorPtr)
         {
            pMsg.pData = reinterpret_cast<void*>(this);
         }

         RWS_RETURNVOID();
      }
   
      /**
      *
      */
      void CFXReplicator::SetRenderCallBack(void)
      {
         RWS_FUNCTION("RWS::FX::CFXReplicator::SetRenderCallBack");
         RpAtomicSetRenderCallBack(m_pAtomic, AtomicRenderCallBack);
         RWS_RETURNVOID();
      }
   
   }//namespace
}