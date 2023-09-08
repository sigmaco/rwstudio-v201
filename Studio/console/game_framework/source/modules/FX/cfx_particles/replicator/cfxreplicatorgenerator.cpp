/*****************************************************************************
*
* File :     CFXReplicatorGenerator.h
*
* Abstract : General purpose particle generator CFXReplicatorGenerator, this particle
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
// Left to its own devices, the MW linker will dead strip the registrar ctors, so...
#ifdef __MWERKS__
#pragma force_active on
#pragma exceptions off // Turn off exceptions due to bug in MW Dwarf Generator for PS2, present in CW 3.04
#endif

//////////////////////////////////////////////////////////////////
//
// RenderWare Includes
//
#include "rprandom.h"
#ifdef _MSC_VER
#pragma comment (lib, "rprandom.lib")
#endif

//////////////////////////////////////////////////////////////////
//
// Renderwarestudio Includes
//
#include "cfxreplicatorgenerator.h"
#include "framework/toolkits/math/maths.h"
#include "framework/toolkits/world/factory.h"
#include "framework/toolkits/world/clevel.h" 

namespace RWS
{
   namespace FX
   {
      RWS_IMPLEMENT_CLASSID(CFXReplicatorGenerator);
      RWS_REGISTERCLASS(CFXReplicatorGenerator);

      /**
      *
      * Adds a constant value to each of the elements of an RwV3d
      *
      */
      void AddConstToRwV3d(RwV3d &V3d, const RwReal r)
      {
         RWS_FUNCTION("RWS::FX::AddConstToRwV3d");

         V3d.x += r; 
         V3d.y += r;
         V3d.z += r;

         RWS_RETURNVOID();
      }


      /**
      *
      * Sets each element of a vector to the specified value, if the value is omitted then it is assumed to be zero
      *
      */
      void SetRwV3D(RwV3d &V3d, const RwReal r = 0.0f)
      {
         RWS_FUNCTION("RWS::FX::SetRwV3D");

         V3d.x = 
         V3d.y = 
         V3d.z = r;

         RWS_RETURNVOID();
      }


      /**
      *
      *  Sets each element of a RwRGBAReal to the specified value, if the value is omitted then it is assumed to be one
      *
      */
      void SetRwRGBAReal(RwRGBAReal &RGBAReal, const RwReal r = 1.0f)
      {
         RWS_FUNCTION("RWS::FX::SetRwRGBAReal");

         RGBAReal.red = 
         RGBAReal.green =
         RGBAReal.blue = 
         RGBAReal.alpha = r;

         RWS_RETURNVOID();
      }


      /**
      *
      * Sets a RwRGBAReal from an 8 bit ARGB Unsigned Int
      * RWS_ATTRIBUTE(CMD_Set_Color, "Color",    "",   COLOR,  RwUInt32,   DEFAULT(65535))
      *
      */
      void SetAttribute_RwRGBAReal (RwRGBAReal &dest, const CAttributeCommandIterator& attr)
      {
         RWS_FUNCTION("RWS::FX::SetAttribute_RwRGBAReal");

         RwRGBA Color = attr->GetAs_RwRGBA();
         RwRGBARealFromRwRGBA  (&dest, &Color);

         RWS_RETURNVOID();
      }


      /**
      *
      * CFXReplicatorGenerator constructor
      *
      */
      CFXReplicatorGenerator::CFXReplicatorGenerator(const CAttributePacket& attr) : 
      InitCEventHandler(&m_pAtomic),
         m_total_number_of_particles(0),
         m_modify_paths(false),
         m_link_priority(32768)
      {
         RWS_FUNCTION("RWS::FX::CFXReplicatorGenerator::CFXReplicatorGenerator");

         m_pClump = RpClumpCreate();

         // Can't use initializer ('this' isn't valid) so...
         m_pAtomic = CreateAtomicInWorldFromResource(attr, this);

         RWS_ASSERT(m_pAtomic, "Failed to create atomic");

         // Acceleration
         m_ParticleDynamics.aXYZ.x = 0.0f;
         m_ParticleDynamics.aXYZ.y = -0.01f;
         m_ParticleDynamics.aXYZ.z = 0.0f;

         // Friction
         SetRwV3D(m_ParticleDynamics.fXYZ, 0.98f);

         // Angular acceleration
         SetRwV3D(m_ParticleDynamics.aaXYZ);

         // Angular friction
         SetRwV3D(m_ParticleDynamics.afXYZ, 1.0f);

         // Initial Scale
         SetRwV3D(m_ParticleDynamics.iScale, 1.0f);

         // Scale
         SetRwV3D(m_ParticleDynamics.vScale, 1.0f);

         // Expire Times
         m_ExpireTime = RWS::MainLoop::Logic::Rate * 2;
         m_ExpireTime_Bias = RWS::MainLoop::Logic::Rate;

         // Initial velocity of particles (Random)
         SetRwV3D(m_initial_velocity, 4.0f);

         // Initial angular velocity of particles (Random)
         SetRwV3D(m_initial_angvelocity, 3.0f);

         // Initial velocity bias i.e. velocity = random velocity + bias velocity
         SetRwV3D(m_initial_velocity_bias);

         // Initial angular velocity bias i.e. velocity = random angular velocity + bias angular velocity
         SetRwV3D(m_initial_angvelocity_bias);

         m_number_of_message_to_trigger  = 1;
         m_number_of_particles_to_create = 10;

         // Limit the total number of particles
         m_total_number_of_particles_to_create = 0;

         // Count number of trigger messages received
         m_message_count = 0;

         // Colors
         SetRwRGBAReal(m_ParticleDynamics.c0);
         SetRwRGBAReal(m_ParticleDynamics.c1);
         SetRwRGBAReal(m_ParticleDynamics.c2);
         SetRwRGBAReal(m_ParticleDynamics.c3);
         SetRwRGBAReal(m_ParticleDynamics.c4);

         RWS_RETURNVOID();
      }

      /**
      *
      *  Callback used to remove any remaining atomics from the clump before its is deleted.
      *
      */
      RpAtomic* CFXReplicatorGenerator::RemoveAtomic(RpAtomic* pAtomic, void *data)
      {
         RWS_FUNCTION("RWS::FX::CFXReplicatorGenerator::RemoveAtomic");

         RpClump *_pClump = RpAtomicGetClump(pAtomic);

         RpClumpRemoveAtomic( _pClump, pAtomic);

         RWS_RETURN(pAtomic);
      }

      /**
      *
      * CFXReplicatorGenerator Destructor
      *
      */
      CFXReplicatorGenerator::~CFXReplicatorGenerator(void)
      {
         RWS_FUNCTION("RWS::FX::CFXReplicatorGenerator::~CFXReplicatorGenerator");

         UnLinkMsg(m_StartRender);
         UnRegisterMsg(m_StartRender);

         UnLinkMsg(m_Msg_Rx_Trigger_Particles);
         UnLinkMsg(m_Msg_Rx_Alt_Trigger_Particles);

         UnRegisterMsg(m_Msg_Rx_Trigger_Particles);
         UnRegisterMsg(m_Msg_Rx_Alt_Trigger_Particles);

         UnRegisterMsg(m_ParticleDynamics.Msg_Particle_Expired);
         UnRegisterMsg(m_ParticleDynamics.Msg_Particle_Active);

         // Just in case the CFXReplicatorGenerator is deleted before all the CFXReplicator
         // which it generates are destroyed.
         //
         RpClumpForAllAtomics(m_pClump, RemoveAtomic, 0);

         RpClumpDestroy(m_pClump);

         RWS_RETURNVOID();
      }


      /**
      *
      * Handle events
      *
      */
      void CFXReplicatorGenerator::HandleEvents(CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::FX::CFXReplicatorGenerator::HandleEvents");

         if (pMsg.Id == m_StartRender)
         {
            RwCamera *pRwCamera = reinterpret_cast<RwCamera*>(pMsg.pData);

            if ( pRwCamera)
            {
               RpWorldAddCamera(CLevel::GetOpaqueWorld(), pRwCamera);

 
               if (RwCameraBeginUpdate(pRwCamera))
               {
                  // Temporary store of RenderState
                  //
                  RwBlendFunction _rwRENDERSTATESRCBLEND;
                  RwBlendFunction _rwRENDERSTATEDESTBLEND;
                  RwUInt32 _rwRENDERSTATEVERTEXALPHAENABLE;
                  RwUInt32 _rwRENDERSTATEZWRITEENABLE;
                  RwUInt32 _rwRENDERSTATEZTESTENABLE;
                  RwCullMode _rwRENDERSTATECULLMODE;

                  // Store render state.
                  //
                  RwRenderStateGet(rwRENDERSTATESRCBLEND, &_rwRENDERSTATESRCBLEND);
                  RwRenderStateGet(rwRENDERSTATEDESTBLEND, &_rwRENDERSTATEDESTBLEND);
                  RwRenderStateGet(rwRENDERSTATEVERTEXALPHAENABLE, &_rwRENDERSTATEVERTEXALPHAENABLE);
                  RwRenderStateGet(rwRENDERSTATEZWRITEENABLE, &_rwRENDERSTATEZWRITEENABLE);
                  RwRenderStateGet(rwRENDERSTATEZTESTENABLE, &_rwRENDERSTATEZTESTENABLE);
                  RwRenderStateGet(rwRENDERSTATECULLMODE, &_rwRENDERSTATECULLMODE);

                  // Set Render State
                  //
                  RwRenderStateSet(rwRENDERSTATESRCBLEND, (void *)m_rwRENDERSTATESRCBLEND);
                  RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void *)m_rwRENDERSTATEDESTBLEND);
                  RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *)m_rwRENDERSTATEVERTEXALPHAENABLE);
                  RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)m_rwRENDERSTATEZWRITEENABLE);
                  RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)m_rwRENDERSTATEZTESTENABLE);
                  RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)m_rwRENDERSTATECULLMODE);

                  ClumpHelper::RenderAllVisibleAtomics(m_pClump);

                  // Restore rendering states
                  //
                  RwRenderStateSet(rwRENDERSTATESRCBLEND, (void *)_rwRENDERSTATESRCBLEND);
                  RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void *)_rwRENDERSTATEDESTBLEND);
                  RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *)_rwRENDERSTATEVERTEXALPHAENABLE);
                  RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)_rwRENDERSTATEZWRITEENABLE);
                  RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)_rwRENDERSTATEZTESTENABLE);
                  RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)_rwRENDERSTATECULLMODE);

                  RwCameraEndUpdate (pRwCamera);

               } // RwCameraBeginUpdate

               RpWorldRemoveCamera(CLevel::GetOpaqueWorld(), pRwCamera);
            }
         }
         // Trigger generation of particles
         if (pMsg.Id == m_Msg_Rx_Trigger_Particles || pMsg.Id == m_Msg_Rx_Alt_Trigger_Particles)
         {
            TriggerParticles(pMsg);
         }

         RWS_RETURNVOID();
      }

      /**
      *
      *
      *
      */
      void CFXReplicatorGenerator::TriggerParticles ( CMsg &pMsg )
      {
         RWS_FUNCTION("RWS::FX::CFXReplicatorGenerator::TriggerParticles");

         m_message_count++;

         // Check if number of trigger messages received exceeds the trigger value
         if (m_message_count >= m_number_of_message_to_trigger)
         {
            m_message_count = 0;

            // Check if the message contained a RpAtomic* if so then 
            // that is where the particles should be generated.
            const RwMatrix* pSMatrix = 0;

            // Check if the message contains a valid RpAtomic *
            if (pMsg.Id == m_Msg_Rx_Trigger_Particles && pMsg.pData)
            {
               RwFrame* pFrame = static_cast<RwFrame*>(pMsg.pData);

               if ( pFrame ) pSMatrix = RwFrameGetLTM(pFrame);
            }

            // Otherwise use the frame of the particle generator
            else
            {
               RwFrame* pFrame = RpAtomicGetFrame(m_pAtomic.ptr());
               if ( pFrame ) pSMatrix = RwFrameGetLTM(pFrame);
            }
         
            // Create m_number_of_particles_to_create particles
            for (RwUInt32 i = 0; i < m_number_of_particles_to_create; i++)
            {
               CFXReplicator *pParticle = ::RWS_NEW CFXReplicator(m_ParticleDynamics);
            
               // Velocity = bias +- random/2
               pParticle->m_ParticleDynamics.vXYZ.x = m_initial_velocity_bias.x 
                  + m_initial_velocity.x * ((RwReal)RpRandom() / (RwUInt32MAXVAL>>1)) - m_initial_velocity.x/2;

               pParticle->m_ParticleDynamics.vXYZ.y = m_initial_velocity_bias.y 
                  + m_initial_velocity.y * ((RwReal)RpRandom() / (RwUInt32MAXVAL>>1)) - m_initial_velocity.y/2;

               pParticle->m_ParticleDynamics.vXYZ.z = m_initial_velocity_bias.z 
                  + m_initial_velocity.z * ((RwReal)RpRandom() / (RwUInt32MAXVAL>>1)) - m_initial_velocity.z/2;
            
               // Angular velocity = bias +- random / 2
               pParticle->m_ParticleDynamics.avXYZ.x = m_initial_angvelocity_bias.x 
                  + m_initial_angvelocity.x * ((RwReal)RpRandom() / (RwUInt32MAXVAL>>1)) - m_initial_angvelocity.x/2;

               pParticle->m_ParticleDynamics.avXYZ.y = m_initial_angvelocity_bias.y 
                  + m_initial_angvelocity.y * ((RwReal)RpRandom() / (RwUInt32MAXVAL>>1)) - m_initial_angvelocity.y/2;

               pParticle->m_ParticleDynamics.avXYZ.z = m_initial_angvelocity_bias.z 
                  + m_initial_angvelocity.z * ((RwReal)RpRandom() / (RwUInt32MAXVAL>>1)) - m_initial_angvelocity.z/2;
            
               // Expire Time = bias + random
               pParticle->m_ParticleDynamics.TimeOut = RpRandom() % m_ExpireTime 
                  + m_ExpireTime_Bias;
            
               // Calc Spline Step Rate
               pParticle->m_ParticleDynamics.TimeStep = 
                  1.0f / (RwReal) pParticle->m_ParticleDynamics.TimeOut;
            
               // Copy the properties
               //
               //               pParticle->m_CAtomic.m_properties = m_CAtomic.m_properties;   // Copy entity parameters
            
               // Clone the entity of the particle generator and attach the clone to the particle
               pParticle->m_pAtomic = RpAtomicClone(m_pAtomic);

               RpAtomicToEventHandler::SetEventHandler(*pParticle->m_pAtomic, pParticle);

               pParticle->SetRenderCallBack();

               RWS_ASSERT(pParticle->m_pAtomic, "RpAtomicClone failed");

               RwFrame* pFrame = RwFrameCreate();

               RWS_ASSERT(pFrame, "RwFrameCreate failed");

               RpAtomicSetFrame( pParticle->m_pAtomic, pFrame );

               // Don't add the atomic to the world otherwise it will be rendered inside RpWorldRender
               // which would be wrong, as we want to render the atomics using RpClumpRender 
               // so that we can control the order of rendering.
               //
               RpClumpAddAtomic(m_pClump, pParticle->m_pAtomic);
            
               // And set its position to that of the particle generator
               if (pSMatrix)
               {
                  if (m_modify_paths)
                  {
                     // Apply Local Transform to Particle System Parameters...
                     RwV3dTransformVectors(&pParticle->m_ParticleDynamics.vXYZ,
                        &pParticle->m_ParticleDynamics.vXYZ, 1, pSMatrix);

                     RwV3dTransformVectors(&pParticle->m_ParticleDynamics.aXYZ,
                        &pParticle->m_ParticleDynamics.aXYZ, 1, pSMatrix);
                  
                     // Because of the way the friction parameters are "centered" 
                     // around 1, 1, 1 need to move them to the origin 
                     // before applying the transform and then put them back.
                     //
                     AddConstToRwV3d(pParticle->m_ParticleDynamics.fXYZ, -1.0f);
                  
                     RwV3dTransformVectors(&pParticle->m_ParticleDynamics.fXYZ,
                        &pParticle->m_ParticleDynamics.fXYZ, 1, pSMatrix);
                  
                     AddConstToRwV3d(pParticle->m_ParticleDynamics.fXYZ, 1.0f);
                  }
               
                  // Position the particle, FE Add offset and area                  
                  RwFrame *frame = RpAtomicGetFrame(pParticle->m_pAtomic.ptr());
               
                  RwFrameTransform(frame, pSMatrix, rwCOMBINEREPLACE);

                  // Apply scale
                  RwFrameScale (frame, &m_ParticleDynamics.iScale, rwCOMBINEPRECONCAT);
               }
            }
         
            if (m_total_number_of_particles_to_create != 0)
            {
               m_total_number_of_particles += m_number_of_particles_to_create;
            
               if (m_total_number_of_particles > m_total_number_of_particles_to_create)
               {
                  Delete();
               }
            }
         }
         RWS_RETURNVOID();
      }


      /**
      *
      *  This function maps the attribute data specified by 
      *` LIST("rwBLENDZERO|rwBLENDONE|rwBLENDSRCCOLOR|...
      *  to the RenderWare enums for the particular blend mode.
      */
      RwBlendFunction CFXReplicatorGenerator::Map_AttributeListToBlendMode(RwUInt32 var) const
      {
         RWS_FUNCTION("RWS::FX::CFXParticle::Map_AttributeListToBlendMode");
         
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
      * Handle Attributes
      *
      */
      void CFXReplicatorGenerator::HandleAttributes(const CAttributePacket& attr)
      {
         RWS_FUNCTION("RWS::FX::CFXReplicatorGenerator::HandleAttributes");
         
         CAttributeHandler::HandleAttributes(attr);
         
         m_pAtomic.HandleSystemCommands(attr);
         
         CAttributeCommandIterator attrIt(attr, RWS_CLASSID_OF(CFXReplicatorGenerator));
         
         while (!attrIt.IsFinished())
         {
            switch (attrIt->GetCommandId())
            {
               //////////////////////////////////////////////////////////////////
            case CMD_Start_Render:
               //////////////////////////////////////////////////////////////////
               // If already linked, unlink and unregister as were about to register with
               // a new event.
               //
               UnLinkMsg(m_StartRender);
               UnRegisterMsg(m_StartRender);
               
               //
               //
               RegisterMsg(m_StartRender, attrIt->GetAs_RwChar_ptr(), "RwCamera*");
               LinkMsg(m_StartRender, "RwCamera*", m_link_priority);
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_priority:
               //////////////////////////////////////////////////////////////////
               m_link_priority = attrIt->GetAs_RwUInt32();
               
               // If already linked, unlink as were about to relink at a different priority
               //
               UnLinkMsg(m_StartRender);
               LinkMsg(m_StartRender, "RwCamera*", m_link_priority);
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_Set_m_Msg_Rx_Trigger_Particles:
               //////////////////////////////////////////////////////////////////
               ReplaceLinkedMsg(m_Msg_Rx_Trigger_Particles, attrIt->GetAs_RwChar_ptr(), "RwFrame*");
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_Set_m_Msg_Rx_Alt_Trigger_Particles:
               //////////////////////////////////////////////////////////////////
               ReplaceLinkedMsg(m_Msg_Rx_Alt_Trigger_Particles, attrIt->GetAs_RwChar_ptr(), 0);
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_Set_m_number_of_particles_to_create:
               //////////////////////////////////////////////////////////////////
               attrIt->GetCommandData(m_number_of_particles_to_create);
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_Set_m_total_number_of_particles_to_create:
               //////////////////////////////////////////////////////////////////
               attrIt->GetCommandData(m_total_number_of_particles_to_create);
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_Set_m_number_of_message_to_trigger:
               //////////////////////////////////////////////////////////////////
               attrIt->GetCommandData(m_number_of_message_to_trigger);
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_Set_m_ExpireTime:
               //////////////////////////////////////////////////////////////////
               attrIt->GetCommandData(m_ExpireTime);
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_Set_m_ExpireTime_Bias:
               //////////////////////////////////////////////////////////////////
               attrIt->GetCommandData(m_ExpireTime_Bias);
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_Set_m_initial_velocity:
               //////////////////////////////////////////////////////////////////
               attrIt->GetCommandData(m_initial_velocity);
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_Set_m_initial_angvelocity:
               //////////////////////////////////////////////////////////////////
               attrIt->GetCommandData(m_initial_angvelocity);
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_Set_m_initial_velocity_bias:
               //////////////////////////////////////////////////////////////////
               attrIt->GetCommandData(m_initial_velocity_bias);
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_Set_m_initial_angvelocity_bias:
               //////////////////////////////////////////////////////////////////
               attrIt->GetCommandData(m_initial_angvelocity_bias);
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_Set_m_initial_acceleration:
               //////////////////////////////////////////////////////////////////
               attrIt->GetCommandData(m_ParticleDynamics.aXYZ);
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_Set_m_initial_friction:
               //////////////////////////////////////////////////////////////////
               attrIt->GetCommandData(m_ParticleDynamics.fXYZ);
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_Set_m_initial_angacceleration:
               //////////////////////////////////////////////////////////////////
               attrIt->GetCommandData(m_ParticleDynamics.aaXYZ);
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_Set_m_initial_angfriction:
               //////////////////////////////////////////////////////////////////
               attrIt->GetCommandData(m_ParticleDynamics.afXYZ);
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_Set_iScale:
               //////////////////////////////////////////////////////////////////
               attrIt->GetCommandData(m_ParticleDynamics.iScale);
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_Set_vScale:
               //////////////////////////////////////////////////////////////////
               attrIt->GetCommandData(m_ParticleDynamics.vScale);
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_Set_m_Msg_Particle_Expired:
               //////////////////////////////////////////////////////////////////
               ReplaceRegisteredMsg(m_ParticleDynamics.Msg_Particle_Expired, attrIt->GetAs_RwChar_ptr(), "RwFrame*");
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_Set_m_Msg_Particle_Active:
               //////////////////////////////////////////////////////////////////
               ReplaceRegisteredMsg(m_ParticleDynamics.Msg_Particle_Active, attrIt->GetAs_RwChar_ptr(), "RwFrame*");
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_m_modify_paths:
               //////////////////////////////////////////////////////////////////
               m_modify_paths = attrIt->GetAs_RwUInt32();
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_Set_c0:
               //////////////////////////////////////////////////////////////////
               SetAttribute_RwRGBAReal(m_ParticleDynamics.c0, attrIt);
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_Set_c1:
               //////////////////////////////////////////////////////////////////
               SetAttribute_RwRGBAReal(m_ParticleDynamics.c1, attrIt);
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_Set_c2:
               //////////////////////////////////////////////////////////////////
               SetAttribute_RwRGBAReal(m_ParticleDynamics.c2, attrIt);
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_Set_c3:
               //////////////////////////////////////////////////////////////////
               SetAttribute_RwRGBAReal(m_ParticleDynamics.c3, attrIt);
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_Set_c4:
               //////////////////////////////////////////////////////////////////
               SetAttribute_RwRGBAReal(m_ParticleDynamics.c4, attrIt);
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_rwRENDERSTATECULLMODE:
               //////////////////////////////////////////////////////////////////
               switch (attrIt->GetAs_RwUInt32())
               {
               case 0: m_rwRENDERSTATECULLMODE = rwCULLMODECULLNONE;break;
               case 1: m_rwRENDERSTATECULLMODE = rwCULLMODECULLBACK;break;
               case 2: m_rwRENDERSTATECULLMODE = rwCULLMODECULLFRONT;break;
               }
               break;
               //////////////////////////////////////////////////////////////////
               case CMD_rwRENDERSTATESRCBLEND:
                  //////////////////////////////////////////////////////////////////
                  m_rwRENDERSTATESRCBLEND = Map_AttributeListToBlendMode(attrIt->GetAs_RwInt32());
                  break;
                  //////////////////////////////////////////////////////////////////
               case CMD_rwRENDERSTATEDESTBLEND:
                  //////////////////////////////////////////////////////////////////
                  m_rwRENDERSTATEDESTBLEND = Map_AttributeListToBlendMode(attrIt->GetAs_RwInt32());
                  break;
                  //////////////////////////////////////////////////////////////////
               case CMD_rwRENDERSTATEVERTEXALPHAENABLE:
                  //////////////////////////////////////////////////////////////////
                  m_rwRENDERSTATEVERTEXALPHAENABLE = attrIt->GetAs_RwInt32();
                  break;
                  //////////////////////////////////////////////////////////////////
               case CMD_rwRENDERSTATEZWRITEENABLE:
                  //////////////////////////////////////////////////////////////////
                  m_rwRENDERSTATEZWRITEENABLE = attrIt->GetAs_RwInt32();
                  break;
                  //////////////////////////////////////////////////////////////////
               case CMD_rwRENDERSTATEZTESTENABLE:
                  //////////////////////////////////////////////////////////////////
                  m_rwRENDERSTATEZTESTENABLE = attrIt->GetAs_RwInt32();
                  break;
            }
            ++attrIt;
         }
         
         RWS_RETURNVOID();
      }
   }//namespace
}
