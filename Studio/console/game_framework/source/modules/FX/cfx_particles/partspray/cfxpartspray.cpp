/*****************************************************************************
*
* File :      CFXPartSpray.cpp
*
* Abstract :  Particle spray object using RW PTank
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


// Left to its own devices, the MW linker will dead strip the registrar ctors, so...

#ifdef __MWERKS__
#pragma force_active on
#pragma exceptions off // Turn off exceptions due to bug in MW Dwarf Generator for PS2, present in CW 3.04
#endif

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//

#include "framework/core/macros/debugmacros.h"
#include "framework/toolkits/math/maths.h"
#include "framework/mainloop/render.h"
#include "framework/toolkits/world/clevel.h"
#include "framework/toolkits/world/factory.h"

#include "cfxpartspray.h"

namespace RWS
{
   using namespace Particle;

   namespace FX
   {
      /**
      *
      * CFXPartSpray class functions.
      *
      */

      RWS_IMPLEMENT_CLASSID(CFXPartSpray);
      RWS_REGISTERCLASS(CFXPartSpray);

      // Setup the static variables.

      // Must match CMD_subTexType.
      RwUInt32 CFXPartSpray::m_subTexLookupU[] = {1, 1, 2, 2, 4, 4, 8};
      
      // Must match CMD_subTexType.
      RwUInt32 CFXPartSpray::m_subTexLookupV[] = {1, 2, 2, 4, 4, 8, 8};
      
      // Must match CMD_srcBlend & CMD_destBlend
      RwBlendFunction CFXPartSpray::m_blendLookup[11] =                
            {
            rwBLENDZERO,
            rwBLENDONE,
            rwBLENDSRCCOLOR,
            rwBLENDINVSRCCOLOR,
            rwBLENDSRCALPHA,
            rwBLENDINVSRCALPHA,
            rwBLENDDESTALPHA,
            rwBLENDINVDESTALPHA,
            rwBLENDDESTCOLOR,
            rwBLENDINVDESTCOLOR,
            rwBLENDSRCALPHASAT
            };
      CycPartBuff<CPB_TexPart>::RENDER_MODE CFXPartSpray::m_renderModeLookup[3] =   // Must match CMD_rendMode
            {
            CycPartBuff<CPB_TexPart>::RM_NO_Z_WRITE,
            CycPartBuff<CPB_TexPart>::RM_Z_WRITE,
            CycPartBuff<CPB_TexPart>::RM_DUAL_PASS
            };

      /**
      *
      * Construct the spray object using the standard method. Link to the running tick message and create a
      * new particle system.
      *
      * \param attr Standard attribute setup packet
      *
      */

      CFXPartSpray::CFXPartSpray(const CAttributePacket &attr)
            : InitCEventHandler(&m_pAtomic),
            m_ftHeading(0),
            m_ftDeflect(0),
            m_renderPrior(0)
      {
         RWS_FUNCTION("RWS::FX::CFXPartSpray::CFXPartSpray");

         m_pAtomic = CreateAtomicInWorldFromResource(attr, this);
         RWS_ASSERT(m_pAtomic, "Failed to create atomic");

         // Setup the particles... Special case of start size of ZERO. This is due to the problem of memory
         // fragmentation when the 'Resize' function is called in HandleAttributes. This way the particle
         // system is created (but cannot be used) without allocating the a default number of particles. When
         // the 'Resize' function is called it will set the size to what is required (so no resize in game
         // release).

         m_pParticles = ::RWS_NEW CycPartBuff<CPB_TexPart>(0, 0);
         RWS_ASSERT(m_pParticles, "Failed to allocate particle system.");

         RWS_RETURNVOID();
      }

      /**
      *
      * Gets the specified texture from the asset. If the asset has zero textures, then the function returns
      * zero. It will return a pointer to the texture that EXACTLY matches the passed in name, otherwise it
      * will return zero.
      *
      * \see GetTexCallBack
      * \returns A pointer to the specified texture or zero if either; a) no textures or b) texture not found.
      * \param pName The name of the texture to attempt to return. No path or extension required. Is case sensitive.
      *
      */

      RwTexture *CFXPartSpray::GetTexture(const char *pName)
      {
         RWS_FUNCTION("RWS::FX::CFXPartSpray::GetTexture");
      
         tex_select texSelect;

         // Set up the texture to search for...

         texSelect.m_pName = pName;

         // Scan for the texture...

         RpGeometry *pGeom = RpAtomicGetGeometry(m_pAtomic);
         RpGeometryForAllMaterials(pGeom, GetTexCallBack, &texSelect);

         RWS_RETURN(texSelect.m_pTexture);
      }

      /**
      *
      * RenderWare callback, used to set the texture of the particle tank via a call to a
      * RpGeometryForAllMaterials. See RenderWare documentation for more information. 
      *
      * The data passed into this function MUST be a 'tex_select' structure. The data in this
      * structure will be updated / modified by the callback.
      *
      * \returns Pointer to passed in material if wants next material or 0 if completed.
      * \param pMaterial Pointer to the currently being processed material.
      * \param pData Pointer to the 'user data' - in this case this MUST be a tex_select structure
      * with the texture name to look for setup.
      * \see GetTexture
      *
      */

      RpMaterial *CFXPartSpray::GetTexCallBack(RpMaterial *pMaterial, void *pData)
      {
         RWS_FUNCTION("RWS::FX::CFXPartSpray::GetTexCallBack");

         tex_select *pTexSelect = static_cast<tex_select *>(pData);
         RwTexture *pTex;

         // Get texture if one...

         pTex = RpMaterialGetTexture(pMaterial);

         if (pTex)
         {
            // Is this the one they wanted ?

            if (!strcmp(RwTextureGetName(pTex), pTexSelect->m_pName))
            {
               // Yes, store & stop processing...

               pTexSelect->m_pTexture = pTex;
               RWS_RETURN(0);
            }
            else
            {
               // No, attempt to find more. Must decrement the count too.

               RWS_RETURN(pMaterial);
            }

         }
         else
         {
            // No texture here, get the next one...

            RWS_RETURN(pMaterial);
         }
      }

      /**
      *
      * Destroy controller object in the standard way. Unlinking from messages and destroying the
      * particle system.
      *
      */

      CFXPartSpray::~CFXPartSpray(void)
      {
         RWS_FUNCTION("RWS::FX::CFXPartSpray::~CFXPartSpray");

         // Appears that the m_pPartAtom is deleted automatically by system, but do need to remove.

         delete m_pParticles;

         // Do registered messages, unlink, then unregister...

         UnLinkMsg(m_posMsg);
         UnRegisterMsg(m_posMsg);
         UnLinkMsg(m_enableMsg);
         UnRegisterMsg(m_enableMsg);
         UnLinkMsg(m_rateMsg);
         UnRegisterMsg(m_rateMsg);
         UnLinkMsg(m_emitMsg);
         UnRegisterMsg(m_emitMsg);
         UnLinkMsg(m_updateMsg);
         UnRegisterMsg(m_updateMsg);
         UnLinkMsg(m_renderMsg);
         UnRegisterMsg(m_renderMsg);

         RWS_RETURNVOID();
      }

      /**
      *
      * Handle events in the standard way.
      *
      * \param msg Standard message package
      *
      */

      void CFXPartSpray::HandleEvents(CMsg &msg)
      {
         RWS_FUNCTION("RWS::FX::CFXPartSpray::HandleEvents");

         if (msg.Id == m_updateMsg)
         {
            // Should do this BEFORE emit, update old ones, before stating any new ones.

            m_pParticles->Tick();
         }
         if (msg.Id == m_posMsg && msg.pData)
         {
            // Perform recalculation on frame translator (will only recalculate if needed), then
            // transform from the passed in frame to the frame required locally.
            //
            // NOTE: Data pointer cannot be 0 (NULL) as deference it below.
            //
            // ALSO: This processing MUST OCCUR BEFORE THE EMIT OF NEW PARTICLES or
            //       they may appear in the incorrect position.

            RwFrame *pFrame = RpAtomicGetFrame(m_pAtomic.ptr());

            m_frameTrans.Recalc();
            m_frameTrans.Trans(*pFrame, *(reinterpret_cast<RwFrame *>(msg.pData)));
         }
         if (msg.Id == m_enableMsg)
         {
            // Enable / disable the system... DO BEFORE EMIT.

            if (msg.pData)
            {
               // Go from data...

               m_pParticles->GetBase()->Enable(*reinterpret_cast<bool *>(msg.pData));
            }
            else
            {
               // No data, so toggle...

               m_pParticles->GetBase()->Enable(!m_pParticles->GetBase()->IsEnabled());
            }
         }
         if (msg.Id == m_rateMsg && msg.pData)
         {
            RwUInt32 percent;

            // Set new value in system... DO BEFORE EMIT.

            percent = *reinterpret_cast<RwUInt32 *>(msg.pData);
            m_pParticles->GetBase()->SetRateAdj(static_cast<RwReal>(percent) / 100.0f);
         }
         if (msg.Id == m_emitMsg)
         {
            RwMatrix *pMtx = RwFrameGetMatrix(RpAtomicGetFrame(m_pAtomic.ptr()));

            // Make sure the position is up to date BEFORE emitting...

            m_pParticles->GetBase()->SetBase(*RwMatrixGetPos(pMtx));
            m_pParticles->GetBase()->SetYAxisVect(*RwMatrixGetUp(pMtx));
            m_pParticles->GetBase()->SetXAxisVect(*RwMatrixGetRight(pMtx));
            m_pParticles->GetBase()->SetZAxisVect(*RwMatrixGetAt(pMtx));

            // Do emitting...

            m_pParticles->Emit();
         }
         if (msg.Id == m_renderMsg && msg.pData)
         {
            RwCamera *pCamera = reinterpret_cast<RwCamera *>(msg.pData);

            if (pCamera)
            {
               RpWorldAddCamera(CLevel::GetOpaqueWorld(), pCamera);
            
               // Matrix flags are sometimes discarded, RwCameraBeginUpdate will assert if these are not set
               //
               rwMatrixSetFlags(RwFrameGetLTM(RwCameraGetFrame(pCamera)),
                  rwMATRIXTYPENORMAL | rwMATRIXTYPEORTHOGONAL);

               if (RwCameraBeginUpdate(pCamera))
               {
                  m_pParticles->Render();
                  RwCameraEndUpdate(pCamera);
               }

               RpWorldRemoveCamera(CLevel::GetOpaqueWorld(), pCamera);
            }
         }

         RWS_RETURNVOID();
      }

      /**
      *
      * Handle attributes in standard way.
      *
      * \param attr Standard attribute packets.
      *
      */

      void CFXPartSpray::HandleAttributes(const CAttributePacket &attr)
      {
         RWS_FUNCTION("RWS::FX::CFXPartSpray:HandleAttributes");

         CAttributeHandler::HandleAttributes(attr);
         m_pAtomic.HandleSystemCommands(attr);

         // Handle changes to control selections.

         CAttributeCommandIterator attrIt(attr, RWS_CLASSID_OF(CFXPartSpray));

         while (!attrIt.IsFinished())
         {
            switch (attrIt->GetCommandId())
            {

            // Messages...

            case MSG_pos:

               // Unlink and unregister old one - doesn't matter if there isn't an old one...

               UnLinkMsg(m_posMsg);
               UnRegisterMsg(m_posMsg);

               // Link to new message...

               RegisterMsg(m_posMsg, attrIt->GetAs_RwChar_ptr(), "RwFrame*");
               LinkMsg(m_posMsg);
               break;

            case MSG_enable:

               // Unlink and unregister old one - doesn't matter if there isn't an old one...

               UnLinkMsg(m_enableMsg);
               UnRegisterMsg(m_enableMsg);

               // Link to new message...

               RegisterMsg(m_enableMsg, attrIt->GetAs_RwChar_ptr(), "RwBool*");
               LinkMsg(m_enableMsg);
               break;

            case MSG_rate:

               // Unlink and unregister old one - doesn't matter if there isn't an old one...

               UnLinkMsg(m_rateMsg);
               UnRegisterMsg(m_rateMsg);

               // Link to new message...

               RegisterMsg(m_rateMsg, attrIt->GetAs_RwChar_ptr(), "RwUInt32");
               LinkMsg(m_rateMsg);
               break;

            case MSG_emit:

               // Unlink and unregister old one - doesn't matter if there isn't an old one...

               UnLinkMsg(m_emitMsg);
               UnRegisterMsg(m_emitMsg);

               // Link to new message...

               RegisterMsg(m_emitMsg, attrIt->GetAs_RwChar_ptr(), 0);
               LinkMsg(m_emitMsg);
               break;

            case MSG_update:

               // Unlink and unregister old one - doesn't matter if there isn't an old one...

               UnLinkMsg(m_updateMsg);
               UnRegisterMsg(m_updateMsg);

               // Link to new message...

               RegisterMsg(m_updateMsg, attrIt->GetAs_RwChar_ptr(), 0);
               LinkMsg(m_updateMsg);
               break;

            // Emitter properties...

            case CMD_max:
               {
                  // NOTE: this will recreate the particle tanks, etc. Could be slow. Is also used to set the
                  //       initial size of the tank, so no DEFAULT size is used. This will stop problems with
                  //       memory fragmentation due to size once, then expand/shrink in release versions.

                  m_pParticles->Resize(attrIt->GetAs_RwUInt32(), 0);
               }
               break;

            case CMD_rate:

               m_pParticles->GetBase()->SetRate(attrIt->GetAs_RwUInt32());
               break;

            case CMD_rateSpread:

               m_pParticles->GetBase()->SetRateSpread(attrIt->GetAs_RwUInt32());
               break;

            case CMD_life:
               {
                  RwUInt32 time = static_cast<RwUInt32>(attrIt->GetAs_RwReal() * RWS::MainLoop::Logic::Rate);
                  m_pParticles->GetBase()->SetLifeTime(time);
               }
               break;

            case CMD_lifeSpread:
               {
                  RwUInt32 time = static_cast<RwUInt32>(attrIt->GetAs_RwReal() * RWS::MainLoop::Logic::Rate);
                  m_pParticles->GetBase()->SetLifeSpread(time);
               }
               break;

            case CMD_spreadX:

               m_pParticles->GetBase()->SetAngSpreadX(attrIt->GetAs_RwReal());
               break;

            case CMD_spreadZ:

               m_pParticles->GetBase()->SetAngSpreadZ(attrIt->GetAs_RwReal());
               break;

            case CMD_emitSpread:
               {
                  RwV3d pos;

                  attrIt->GetCommandData(pos);
                  m_pParticles->GetBase()->SetEmitXSpread(pos.x);
                  m_pParticles->GetBase()->SetEmitYSpread(pos.y);
                  m_pParticles->GetBase()->SetEmitZSpread(pos.z);
               }
               break;

            // Display properties...

            case MSG_render:

               // Unlink and unregister old one - doesn't matter if there isn't an old one...

               UnLinkMsg(m_renderMsg);
               UnRegisterMsg(m_renderMsg);

               // Link to new message...

               RegisterMsg(m_renderMsg, attrIt->GetAs_RwChar_ptr(), "RwCamera*");
               LinkMsg(m_renderMsg, "RwCamera*", m_renderPrior);
               break;

            case CMD_order:

               m_renderPrior = static_cast<RwUInt16>(attrIt->GetAs_RwUInt32());
               UnLinkMsg(m_renderMsg);
               LinkMsg(m_renderMsg, "RwCamera*", m_renderPrior);
               break;

            case CMD_rendMode:

               m_pParticles->SetRenderMode(m_renderModeLookup[attrIt->GetAs_RwUInt32()]);
               break;

            case CMD_srcBlend:

               m_pParticles->SetSrcBlend(m_blendLookup[attrIt->GetAs_RwUInt32()]);
               break;

            case CMD_destBlend:

               m_pParticles->SetDestBlend(m_blendLookup[attrIt->GetAs_RwUInt32()]);
               break;

            case CMD_posOff:
               {
                  RwV3d pos;

                  attrIt->GetCommandData(pos);
                  m_frameTrans.SetOffset(pos);
               }
               break;

            case CMD_posHead:

               m_ftHeading = attrIt->GetAs_RwReal();
               m_frameTrans.SetFromAngles(m_ftDeflect, m_ftHeading);
               break;

            case CMD_posDef:

               m_ftDeflect = attrIt->GetAs_RwReal();
               m_frameTrans.SetFromAngles(m_ftDeflect, m_ftHeading);
               break;

            // Primary state...

            case CMD_vel:

               m_pParticles->GetBase()->SetAccel(CPB_BasePart::BPS_PRIMARY_START, attrIt->GetAs_RwReal());
               break;

            case CMD_velSpread:

               m_pParticles->GetBase()->SetAccelSpread(CPB_BasePart::BPS_PRIMARY_START, attrIt->GetAs_RwReal());
               break;

            case CMD_gravity:

               m_pParticles->GetBase()->SetGravity(CPB_BasePart::BPS_PRIMARY_START, attrIt->GetAs_RwReal());
               break;

            case CMD_drag:

               m_pParticles->GetBase()->SetDrag(CPB_BasePart::BPS_PRIMARY_START, attrIt->GetAs_RwReal());
               break;

            case CMD_col:
               { 
                  m_pParticles->GetBase()->SetColor(CPB_BasePart::BPS_PRIMARY_START, attrIt->GetAs_RwRGBA());
               }
               break;

            case CMD_rVar:

               m_pParticles->GetBase()->SetRedSpread(CPB_BasePart::BPS_PRIMARY_START, attrIt->GetAs_RwUInt32());
               break;

            case CMD_gVar:

               m_pParticles->GetBase()->SetGreenSpread(CPB_BasePart::BPS_PRIMARY_START, attrIt->GetAs_RwUInt32());
               break;

            case CMD_bVar:

               m_pParticles->GetBase()->SetBlueSpread(CPB_BasePart::BPS_PRIMARY_START, attrIt->GetAs_RwUInt32());
               break;

            case CMD_aVar:

               m_pParticles->GetBase()->SetAlphaSpread(CPB_BasePart::BPS_PRIMARY_START, attrIt->GetAs_RwUInt32());
               break;

            case CMD_size:

               m_pParticles->GetBase()->SetSize(CPB_BasePart::BPS_PRIMARY_START, attrIt->GetAs_RwReal());
               break;

            case CMD_sizeSpread:

               m_pParticles->GetBase()->SetSizeSpread(CPB_BasePart::BPS_PRIMARY_START, attrIt->GetAs_RwReal());
               break;

            // Primary End values (PE).

            case CMD_peAccelOn:

               if (attrIt->GetAs_RwUInt32())
               {
                  m_pParticles->GetBase()->SetFlag(CPB_BasePart::BPS_PRIMARY_END, CPB_BP_ANIM_ACCEL);
               }
               else
               {
                  m_pParticles->GetBase()->ClearFlag(CPB_BasePart::BPS_PRIMARY_END, CPB_BP_ANIM_ACCEL);
               }
               break;

            case CMD_peAccel:

               m_pParticles->GetBase()->SetAccel(CPB_BasePart::BPS_PRIMARY_END, attrIt->GetAs_RwReal());
               break;

            case CMD_peAccelSpread:

               m_pParticles->GetBase()->SetAccelSpread(CPB_BasePart::BPS_PRIMARY_END, attrIt->GetAs_RwReal());
               break;

            case CMD_peGravityOn:

               if (attrIt->GetAs_RwUInt32())
               {
                  m_pParticles->GetBase()->SetFlag(CPB_BasePart::BPS_PRIMARY_END, CPB_BP_ANIM_GRAVITY);
               }
               else
               {
                  m_pParticles->GetBase()->ClearFlag(CPB_BasePart::BPS_PRIMARY_END, CPB_BP_ANIM_GRAVITY);
               }
               break;

            case CMD_peGravity:

               m_pParticles->GetBase()->SetGravity(CPB_BasePart::BPS_PRIMARY_END, attrIt->GetAs_RwReal());
               break;

            case CMD_peDragOn:

               if (attrIt->GetAs_RwUInt32())
               {
                  m_pParticles->GetBase()->SetFlag(CPB_BasePart::BPS_PRIMARY_END, CPB_BP_ANIM_DRAG);
               }
               else
               {
                  m_pParticles->GetBase()->ClearFlag(CPB_BasePart::BPS_PRIMARY_END, CPB_BP_ANIM_DRAG);
               }
               break;

            case CMD_peDrag:

               m_pParticles->GetBase()->SetDrag(CPB_BasePart::BPS_PRIMARY_END, attrIt->GetAs_RwReal());
               break;

            case CMD_peColOn:

               if (attrIt->GetAs_RwUInt32())
               {
                  m_pParticles->GetBase()->SetFlag(CPB_BasePart::BPS_PRIMARY_END, CPB_BP_ANIM_COL);
               }
               else
               {
                  m_pParticles->GetBase()->ClearFlag(CPB_BasePart::BPS_PRIMARY_END, CPB_BP_ANIM_COL);
               }
               break;

            case CMD_peCol:
               {
                  m_pParticles->GetBase()->SetColor(CPB_BasePart::BPS_PRIMARY_END, attrIt->GetAs_RwRGBA());
               }
               break;

            case CMD_peRVar:

               m_pParticles->GetBase()->SetRedSpread(CPB_BasePart::BPS_PRIMARY_END, attrIt->GetAs_RwUInt32());
               break;

            case CMD_peGVar:

               m_pParticles->GetBase()->SetGreenSpread(CPB_BasePart::BPS_PRIMARY_END, attrIt->GetAs_RwUInt32());
               break;

            case CMD_peBVar:

               m_pParticles->GetBase()->SetBlueSpread(CPB_BasePart::BPS_PRIMARY_END, attrIt->GetAs_RwUInt32());
               break;

            case CMD_peAVar:

               m_pParticles->GetBase()->SetAlphaSpread(CPB_BasePart::BPS_PRIMARY_END, attrIt->GetAs_RwUInt32());
               break;

            case CMD_peSizeOn:

               if (attrIt->GetAs_RwUInt32())
               {
                  m_pParticles->GetBase()->SetFlag(CPB_BasePart::BPS_PRIMARY_END, CPB_BP_ANIM_SIZE);
               }
               else
               {
                  m_pParticles->GetBase()->ClearFlag(CPB_BasePart::BPS_PRIMARY_END, CPB_BP_ANIM_SIZE);
               }
               break;

            case CMD_peSize:

               m_pParticles->GetBase()->SetSize(CPB_BasePart::BPS_PRIMARY_END, attrIt->GetAs_RwReal());
               break;

            case CMD_peSizeSpread:

               m_pParticles->GetBase()->SetSizeSpread(CPB_BasePart::BPS_PRIMARY_END, attrIt->GetAs_RwReal());
               break;

            // Secondary stage master control.

            case CMD_secOn:
            
               m_pParticles->GetBase()->EnableSecState((attrIt->GetAs_RwUInt32() ? true : false));
               break;

            case CMD_secTime:
               {
                  RwUInt32 time = static_cast<RwUInt32>(attrIt->GetAs_RwReal() * RWS::MainLoop::Logic::Rate);
                  m_pParticles->GetBase()->SetSecLifeTime(time);
               }
               break;

            case CMD_secLifeSpread:
               {
                  RwUInt32 time = static_cast<RwUInt32>(attrIt->GetAs_RwReal() * RWS::MainLoop::Logic::Rate);
                  m_pParticles->GetBase()->SetSecLifeAdj(time);
               }
               break;

            case CMD_secEmit:

               m_pParticles->GetBase()->SetSecEmit(attrIt->GetAs_RwInt32());
               break;

            case CMD_secEmitSpread:

               m_pParticles->GetBase()->SetSecEmitSpread(attrIt->GetAs_RwInt32());
               break;

            case CMD_secMinAng:

               m_pParticles->GetBase()->SetSecEmitMinAng(attrIt->GetAs_RwReal());
               break;

            case CMD_secMaxAng:

               m_pParticles->GetBase()->SetSecEmitMaxAng(attrIt->GetAs_RwReal());
               break;

            // Secondary start state.

            case CMD_ssVelOn:

               if (attrIt->GetAs_RwUInt32())
               {
                  m_pParticles->GetBase()->SetFlag(CPB_BasePart::BPS_SECONDARY_START, CPB_BP_ANIM_ACCEL);
               }
               else
               {
                  m_pParticles->GetBase()->ClearFlag(CPB_BasePart::BPS_SECONDARY_START, CPB_BP_ANIM_ACCEL);
               }
               break;

            case CMD_ssVel:

               m_pParticles->GetBase()->SetAccel(CPB_BasePart::BPS_SECONDARY_START, attrIt->GetAs_RwReal());
               break;

            case CMD_ssVelSpread:

               m_pParticles->GetBase()->SetAccelSpread(CPB_BasePart::BPS_SECONDARY_START, attrIt->GetAs_RwReal());
               break;

            case CMD_ssGravityOn:

               if (attrIt->GetAs_RwUInt32())
               {
                  m_pParticles->GetBase()->SetFlag(CPB_BasePart::BPS_SECONDARY_START, CPB_BP_ANIM_GRAVITY);
               }
               else
               {
                  m_pParticles->GetBase()->ClearFlag(CPB_BasePart::BPS_SECONDARY_START, CPB_BP_ANIM_GRAVITY);
               }
               break;

            case CMD_ssGravity:

               m_pParticles->GetBase()->SetGravity(CPB_BasePart::BPS_SECONDARY_START, attrIt->GetAs_RwReal());
               break;

            case CMD_ssDragOn:

               if (attrIt->GetAs_RwUInt32())
               {
                  m_pParticles->GetBase()->SetFlag(CPB_BasePart::BPS_SECONDARY_START, CPB_BP_ANIM_DRAG);
               }
               else
               {
                  m_pParticles->GetBase()->ClearFlag(CPB_BasePart::BPS_SECONDARY_START, CPB_BP_ANIM_DRAG);
               }
               break;

            case CMD_ssDrag:

               m_pParticles->GetBase()->SetDrag(CPB_BasePart::BPS_SECONDARY_START, attrIt->GetAs_RwReal());
               break;

            case CMD_ssColOn:

               if (attrIt->GetAs_RwUInt32())
               {
                  m_pParticles->GetBase()->SetFlag(CPB_BasePart::BPS_SECONDARY_START, CPB_BP_ANIM_COL);
               }
               else
               {
                  m_pParticles->GetBase()->ClearFlag(CPB_BasePart::BPS_SECONDARY_START, CPB_BP_ANIM_COL);
               }
               break;

            case CMD_ssCol:
               {
                  m_pParticles->GetBase()->SetColor(CPB_BasePart::BPS_SECONDARY_START, attrIt->GetAs_RwRGBA());
               }
               break;

            case CMD_ssRVar:

               m_pParticles->GetBase()->SetRedSpread(CPB_BasePart::BPS_SECONDARY_START, attrIt->GetAs_RwUInt32());
               break;

            case CMD_ssGVar:

               m_pParticles->GetBase()->SetGreenSpread(CPB_BasePart::BPS_SECONDARY_START, attrIt->GetAs_RwUInt32());
               break;

            case CMD_ssBVar:

               m_pParticles->GetBase()->SetBlueSpread(CPB_BasePart::BPS_SECONDARY_START, attrIt->GetAs_RwUInt32());
               break;

            case CMD_ssAVar:

               m_pParticles->GetBase()->SetAlphaSpread(CPB_BasePart::BPS_SECONDARY_START, attrIt->GetAs_RwUInt32());
               break;

            case CMD_ssSizeOn:

               if (attrIt->GetAs_RwUInt32())
               {
                  m_pParticles->GetBase()->SetFlag(CPB_BasePart::BPS_SECONDARY_START, CPB_BP_ANIM_SIZE);
               }
               else
               {
                  m_pParticles->GetBase()->ClearFlag(CPB_BasePart::BPS_SECONDARY_START, CPB_BP_ANIM_SIZE);
               }
               break;

            case CMD_ssSize:

               m_pParticles->GetBase()->SetSize(CPB_BasePart::BPS_SECONDARY_START, attrIt->GetAs_RwReal());
               break;

            case CMD_ssSizeSpread:

               m_pParticles->GetBase()->SetSizeSpread(CPB_BasePart::BPS_SECONDARY_START, attrIt->GetAs_RwReal());
               break;

            // Secondary end state.

            case CMD_seAccelOn:

               if (attrIt->GetAs_RwUInt32())
               {
                  m_pParticles->GetBase()->SetFlag(CPB_BasePart::BPS_SECONDARY_END, CPB_BP_ANIM_ACCEL);
               }
               else
               {
                  m_pParticles->GetBase()->ClearFlag(CPB_BasePart::BPS_SECONDARY_END, CPB_BP_ANIM_ACCEL);
               }
               break;

            case CMD_seAccel:

               m_pParticles->GetBase()->SetAccel(CPB_BasePart::BPS_SECONDARY_END, attrIt->GetAs_RwReal());
               break;

            case CMD_seAccelSpread:

               m_pParticles->GetBase()->SetAccelSpread(CPB_BasePart::BPS_SECONDARY_END, attrIt->GetAs_RwReal());
               break;

            case CMD_seGravityOn:

               if (attrIt->GetAs_RwUInt32())
               {
                  m_pParticles->GetBase()->SetFlag(CPB_BasePart::BPS_SECONDARY_END, CPB_BP_ANIM_GRAVITY);
               }
               else
               {
                  m_pParticles->GetBase()->ClearFlag(CPB_BasePart::BPS_SECONDARY_END, CPB_BP_ANIM_GRAVITY);
               }
               break;

            case CMD_seGravity:

               m_pParticles->GetBase()->SetGravity(CPB_BasePart::BPS_SECONDARY_END, attrIt->GetAs_RwReal());
               break;

            case CMD_seDragOn:

               if (attrIt->GetAs_RwUInt32())
               {
                  m_pParticles->GetBase()->SetFlag(CPB_BasePart::BPS_SECONDARY_END, CPB_BP_ANIM_DRAG);
               }
               else
               {
                  m_pParticles->GetBase()->ClearFlag(CPB_BasePart::BPS_SECONDARY_END, CPB_BP_ANIM_DRAG);
               }
               break;

            case CMD_seDrag:

               m_pParticles->GetBase()->SetDrag(CPB_BasePart::BPS_SECONDARY_END, attrIt->GetAs_RwReal());
               break;

            case CMD_seColOn:

               if (attrIt->GetAs_RwUInt32())
               {
                  m_pParticles->GetBase()->SetFlag(CPB_BasePart::BPS_SECONDARY_END, CPB_BP_ANIM_COL);
               }
               else
               {
                  m_pParticles->GetBase()->ClearFlag(CPB_BasePart::BPS_SECONDARY_END, CPB_BP_ANIM_COL);
               }
               break;

            case CMD_seCol:

               {
                  m_pParticles->GetBase()->SetColor(CPB_BasePart::BPS_SECONDARY_END, attrIt->GetAs_RwRGBA());
               }
               break;

            case CMD_seRVar:

               m_pParticles->GetBase()->SetRedSpread(CPB_BasePart::BPS_SECONDARY_END, attrIt->GetAs_RwUInt32());
               break;

            case CMD_seGVar:

               m_pParticles->GetBase()->SetGreenSpread(CPB_BasePart::BPS_SECONDARY_END, attrIt->GetAs_RwUInt32());
               break;

            case CMD_seBVar:

               m_pParticles->GetBase()->SetBlueSpread(CPB_BasePart::BPS_SECONDARY_END, attrIt->GetAs_RwUInt32());
               break;

            case CMD_seAVar:

               m_pParticles->GetBase()->SetAlphaSpread(CPB_BasePart::BPS_SECONDARY_END, attrIt->GetAs_RwUInt32());
               break;

            case CMD_seSizeOn:

               if (attrIt->GetAs_RwUInt32())
               {
                  m_pParticles->GetBase()->SetFlag(CPB_BasePart::BPS_SECONDARY_END, CPB_BP_ANIM_SIZE);
               }
               else
               {
                  m_pParticles->GetBase()->ClearFlag(CPB_BasePart::BPS_SECONDARY_END, CPB_BP_ANIM_SIZE);
               }
               break;

            case CMD_seSize:

               m_pParticles->GetBase()->SetSize(CPB_BasePart::BPS_SECONDARY_END, attrIt->GetAs_RwReal());
               break;

            case CMD_seSizeSpread:

               m_pParticles->GetBase()->SetSizeSpread(CPB_BasePart::BPS_SECONDARY_END, attrIt->GetAs_RwReal());
               break;

            // Texture related - general...

            case CMD_texName:
               {
                  const char *pName;

                  // Get name to use...

                  attrIt->GetCommandData(&pName);

                  // Is this blank ?

                  if (*pName == '\0')
                  {
                     // No texture.

                     m_pParticles->GetBase()->SetTexture(0, *m_pParticles);
                  }
                  else
                  {
                     // Search for texture specified...

                     m_pParticles->GetBase()->SetTexture(GetTexture(pName), *m_pParticles);
                  }
               }
               break;

            case CMD_subTexType:
               {
                  RwUInt32 typeIndex = attrIt->GetAs_RwUInt32();

                  // Translate value from xy size lookup table and feed into call...

                  m_pParticles->GetBase()->SetNumSubTextures(m_subTexLookupU[typeIndex], m_subTexLookupV[typeIndex]);
               }
               break;

            // Primary start (textured)...

            case CMD_psMinSubTex:

               m_pParticles->GetBase()->SetMinSubTex(CPB_BasePart::BPS_PRIMARY_START, attrIt->GetAs_RwUInt32());
               break;

            case CMD_psMaxSubTex:

               m_pParticles->GetBase()->SetMaxSubTex(CPB_BasePart::BPS_PRIMARY_START, attrIt->GetAs_RwUInt32());
               break;

            // Primary end (textured)...

            case CMD_peSubTexOn:

               if (attrIt->GetAs_RwUInt32())
               {
                  m_pParticles->GetBase()->SetFlag(CPB_BasePart::BPS_PRIMARY_END, CPB_BP_ANIM_SUB_TEX);
               }
               else
               {
                  m_pParticles->GetBase()->ClearFlag(CPB_BasePart::BPS_PRIMARY_END, CPB_BP_ANIM_SUB_TEX);
               }
               break;

            case CMD_peMinSubTex:

               m_pParticles->GetBase()->SetMinSubTex(CPB_BasePart::BPS_PRIMARY_END, attrIt->GetAs_RwUInt32());
               break;

            case CMD_peMaxSubTex:

               m_pParticles->GetBase()->SetMaxSubTex(CPB_BasePart::BPS_PRIMARY_END, attrIt->GetAs_RwUInt32());
               break;

            case CMD_peSubTexLoop:

               m_pParticles->GetBase()->SetSubTexLoops(CPB_BasePart::BPS_PRIMARY_END, attrIt->GetAs_RwReal());
               break;

            case CMD_peSubTexRndLoop:

               m_pParticles->GetBase()->SetRndSubTexLoops(CPB_BasePart::BPS_PRIMARY_END, attrIt->GetAs_RwReal());
               break;

            // Secondary start (textured)...

            case CMD_ssSubTexOn:

               if (attrIt->GetAs_RwUInt32())
               {
                  m_pParticles->GetBase()->SetFlag(CPB_BasePart::BPS_SECONDARY_START, CPB_BP_ANIM_SUB_TEX);
               }
               else
               {
                  m_pParticles->GetBase()->ClearFlag(CPB_BasePart::BPS_SECONDARY_START, CPB_BP_ANIM_SUB_TEX);
               }
               break;

            case CMD_ssMinSubTex:

               m_pParticles->GetBase()->SetMinSubTex(CPB_BasePart::BPS_SECONDARY_START, attrIt->GetAs_RwUInt32());
               break;

            case CMD_ssMaxSubTex:

               m_pParticles->GetBase()->SetMaxSubTex(CPB_BasePart::BPS_SECONDARY_START, attrIt->GetAs_RwUInt32());
               break;

            // Secondary end (textured)...

            case CMD_seSubTexOn:

               if (attrIt->GetAs_RwUInt32())
               {
                  m_pParticles->GetBase()->SetFlag(CPB_BasePart::BPS_SECONDARY_END, CPB_BP_ANIM_SUB_TEX);
               }
               else
               {
                  m_pParticles->GetBase()->ClearFlag(CPB_BasePart::BPS_SECONDARY_END, CPB_BP_ANIM_SUB_TEX);
               }
               break;

            case CMD_seMinSubTex:

               m_pParticles->GetBase()->SetMinSubTex(CPB_BasePart::BPS_SECONDARY_END, attrIt->GetAs_RwUInt32());
               break;

            case CMD_seMaxSubTex:

               m_pParticles->GetBase()->SetMaxSubTex(CPB_BasePart::BPS_SECONDARY_END, attrIt->GetAs_RwUInt32());
               break;

            case CMD_seSubTexLoop:

               m_pParticles->GetBase()->SetSubTexLoops(CPB_BasePart::BPS_SECONDARY_END, attrIt->GetAs_RwReal());
               break;

            case CMD_seSubTexRndLoop:

               m_pParticles->GetBase()->SetRndSubTexLoops(CPB_BasePart::BPS_SECONDARY_END, attrIt->GetAs_RwReal());
               break;
            }
            ++attrIt;
         }

         RWS_RETURNVOID();
      }
   }        //namespace FX
}
