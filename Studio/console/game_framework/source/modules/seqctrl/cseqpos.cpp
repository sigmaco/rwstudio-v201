/****************************************************************************
*
* File :      CSeqPos.cpp
*
* Abstract :  Sequencable positioning behaviour.
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
#pragma exceptions off     // Turn off exceptions due to bug in MW Dwarf
#endif                     // Generator for PS2, present in CW 3.04

#if defined(DOLPHIN) && defined(__MWERKS__)
   using namespace std;
#endif

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//

#include "framework/core/resourcemanager/cresourcemanager.h"
#include "framework/toolkits/world/factory.h"

#include "cseqpos.h"

namespace RWS
{
   namespace SEQUENCE
   {
      /**
      *
      * Constants used for vector comparison within MatrixCache class.
      *
      */

      const RwReal MTX_CACHE_NORMAL_MIN         = 0.9999f;
      const RwReal MTX_CACHE_MIN_VECT_DOT_PROD  = 0.9999f;
      const RwReal MTX_CACHE_LENGTH_MIN         = 0.0001f;

      /////////////////////////
      //
      // MatrixCache functions.
      //

      /**
      *
      * Create the matrices and initialize the flags, etc.
      *
      */

      MatrixCache::MatrixCache(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::MatrixCache::MatrixCache");

         m_pBaseMatrix = RwMatrixCreate();
         RWS_ASSERT(m_pBaseMatrix, "Failed to create base matrix.");

         m_pRotOnlyMatrix = RwMatrixCreate();
         RWS_ASSERT(m_pRotOnlyMatrix, "Failed to create rotation matrix.");

         RwMatrixSetIdentity(m_pBaseMatrix);
         m_flags = MTX_CACHE_MATRIX_CHANGED;

         RWS_RETURNVOID();
      }

      /**
      *
      * Initialize the object and then set to the passed in matrix.
      *
      */

      MatrixCache::MatrixCache(const RwMatrix *pRefMatrix)
      {
         RWS_FUNCTION("RWS::SEQUENCE::MatrixCache::MatrixCache (RwMatrix *)");

         MatrixCache();
         SetMatrix(pRefMatrix);

         RWS_RETURNVOID();
      }

      /**
      *
      * Destroy matrices, etc.
      *
      */

      MatrixCache::~MatrixCache(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::MatrixCache::~MatrixCache");

         RwMatrixDestroy(m_pBaseMatrix);
         RwMatrixDestroy(m_pRotOnlyMatrix);

         RWS_RETURNVOID();
      }

      /**
      *
      * Returns the quaternion within the cache, calculating it if necessary.
      *
      */

      const RtQuat *MatrixCache::Quat(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::MatrixCache::Quat");

         // Does it need updating ?

         if ((m_flags & MTX_CACHE_QUATERNION_CACHED) == 0)
         {
            // Update quaternion & unset the flag.

            RtQuatConvertFromMatrix(&m_quat, RotMatrix());
            m_flags |= MTX_CACHE_QUATERNION_CACHED;
         }

         RWS_RETURN(&m_quat);
      }

      /**
      *
      * Returns a pointer to the rotation ONLY matrix, calculates it if necessary.
      *
      */

      const RwMatrix *MatrixCache::RotMatrix(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::MatrixCache::RotMatrix");

         // Does it need updating ?

         if ((m_flags & MTX_CACHE_ROTATION_CACHED) == 0)
         {
            RwV3d scale,
               *pPos;

            // Copy the base matrix & get the scales.

            RwMatrixCopy(m_pRotOnlyMatrix, m_pBaseMatrix);
            scale = *Scale();

            if (scale.x != 1.0f || scale.y != 1.0f || scale.z != 1.0f)
            {
               RwMatrixScale(m_pRotOnlyMatrix, &scale, rwCOMBINEPOSTCONCAT);
            }

            pPos = RwMatrixGetPos(m_pRotOnlyMatrix);
            pPos->x = pPos->y = pPos->z = 0.0f;

            RwMatrixUpdate(m_pRotOnlyMatrix);

            m_flags |= MTX_CACHE_ROTATION_CACHED;
         }

         RWS_RETURN(m_pRotOnlyMatrix);
      }

      /**
      *
      * Returns the position within the cached matrix.
      *
      */

      const RwV3d *MatrixCache::Pos(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::MatrixCache::Pos");
         RWS_RETURN(RwMatrixGetPos(m_pBaseMatrix));
      }

      /**
      *
      * Returns the x,y & z scales within the cached matrix, calculates them
      * if necessary.
      *
      */

      const RwV3d *MatrixCache::Scale(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::MatrixCache::XScale");

         if ((m_flags & MTX_CACHE_SCALE_CACHED) == 0)
         {
            // Extract the axis scales from the matrix.

            m_scale.x = RwV3dLength(RwMatrixGetRight(m_pBaseMatrix));
            m_scale.y = RwV3dLength(RwMatrixGetUp(m_pBaseMatrix));
            m_scale.z = RwV3dLength(RwMatrixGetAt(m_pBaseMatrix));

            m_flags |= MTX_CACHE_SCALE_CACHED;
         }

         RWS_RETURN(&m_scale);
      }

      /**
      *
      *
      *
      */

      RwBool MatrixCache::HasChanged(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::HasChanged");
         RWS_RETURN((m_flags & MTX_CACHE_MATRIX_CHANGED) != 0);
      }

      /**
      *
      * Clear the 'changed' flag within the matrix cache.
      *
      */

      void MatrixCache::ClearChangedFlag(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::ClearChangedFlag");

         m_flags &= ~MTX_CACHE_MATRIX_CHANGED;

         RWS_RETURNVOID();
      }

      /**
      *
      * Sets the internally held matrix to the passed in value, sets the changed flag
      * and clears the 'cached' flags for scale, quaternion, etc. Thereby forcing them
      * to recalculate if requested.
      *
      * This version takes an unaligned (or not know to be aligned) matrix.
      *
      */

      void MatrixCache::SetMatrixFromUnaligned(const RwMatrix *pRefMatrix)
      {
         RWS_FUNCTION("RWS::SEQUENCE::MatrixCache::SetMatrix (RwMatrix)");

         RwMatrix alignedMatrix;

         // Copy from unaligned to aligned.

         memcpy(&alignedMatrix, pRefMatrix, sizeof(RwMatrix));

         // Copy into local matrix.

         RwMatrixCopy(m_pBaseMatrix, &alignedMatrix);
         RwMatrixUpdate(m_pBaseMatrix);
         m_flags = MTX_CACHE_MATRIX_CHANGED;

         RWS_RETURNVOID();
      }

      /**
      *
      * Sets the internally held matrix to the passed in value, sets the changed flag
      * and clears the 'cached' flags for scale, quaternion, etc. Thereby forcing them
      * to recalculate if requested.
      *
      */

      void MatrixCache::SetMatrix(const RwMatrix *pRefMatrix)
      {
         RWS_FUNCTION("RWS::SEQUENCE::MatrixCache::SetMatrix (RwMatrix)");

         RwMatrixCopy(m_pBaseMatrix, pRefMatrix);
         RwMatrixUpdate(m_pBaseMatrix);
         m_flags = MTX_CACHE_MATRIX_CHANGED;

         RWS_RETURNVOID();
      }

      /**
      *
      * Sets the matrix up from the passed in position and at vector (which can be any
      * vector - ie it does not need to be normalized as this function will do that). The
      * world up vector should be specified if it is not (0, 1, 0). In this case the pointer
      * can be set to zero, as (0, 1, 0) is the default. Set the 'changed' flag and clears
      * the 'cached' flag, thereby forcing the quaternion, etc to be recalculated if they
      * are requested.
      *
      * NOTE: If the at & world vectors are collinear, then the matrix WILL NOT BE UPDATED, so
      *       it will stay as it was before. This will be fine for looking towards an incoming
      *       object, as it will continue to point towards the incoming object until it
      *       passes through the same point (when it will stay in the same direction). When the
      *       object moves away, the calculations will resume. The only problem occurs when this is
      *       the start state (collinear vectors) and the previous matrix does not point towards
      *       the 'look at' point. Flags will be cleared, as position is always updated.
      *
      * ALSO: If at vector has zero length, then vector will not be updated, but code will NOT
      *       assert. Flags will be cleared, as position is always updated.
      *
      */

      void MatrixCache::SetMatrix(const RwV3d &pos, const RwV3d &at, const RwV3d *pWorldUp)
      {
         RWS_FUNCTION("RWS::SEQUENCE::MatrixCache::SetMatrix (RwV3d, RwV3d)");

         const RwV3d up = {0.0f, 1.0f, 0.0f};

         RwV3d *pMtxAt,
            *pMtxUp,
            *pMtxRight;
         RwV3d unitAt;

         // Set position...

         *RwMatrixGetPos(m_pBaseMatrix) = pos;

         // Is at vector valid ?

         if (RwV3dLength(&at) < MTX_CACHE_LENGTH_MIN)
         {
            // Too short, cannot update.

            RWS_RETURNVOID();
         }

         // Get unit (normalized) at vector.

         unitAt = at;
         RwV3dNormalize(&unitAt, &unitAt);

         // Using default world up & right ?

         if (pWorldUp == 0)
         {
            pWorldUp = &up;
         }

         RWS_ASSERT(RwV3dLength(pWorldUp) >= MTX_CACHE_NORMAL_MIN,
            "Non unit length world up vector.");

         pMtxAt = RwMatrixGetAt(m_pBaseMatrix);
         pMtxUp = RwMatrixGetUp(m_pBaseMatrix);
         pMtxRight = RwMatrixGetRight(m_pBaseMatrix);

         // Is vector too close to up ?

         if (RwFabs(RwV3dDotProduct(pMtxAt, pWorldUp)) < MTX_CACHE_MIN_VECT_DOT_PROD)
         {
            // Okay, use world up - calc matrix right using world up & matrix at, then use
            // these to calculate the matrix's up. Store unit at vector too.

            *pMtxAt = unitAt;
            RwV3dCrossProduct(pMtxRight, pWorldUp, pMtxAt);
            RwV3dNormalize(pMtxRight, pMtxRight);
            RwV3dCrossProduct(pMtxUp, pMtxAt, pMtxRight);
         }

         // Update the matrix, clear all 'cached' flags & set 'changed' flag.

         RwMatrixUpdate(m_pBaseMatrix);
         m_flags = MTX_CACHE_MATRIX_CHANGED;

         RWS_RETURNVOID();
      }

      /**
      *
      * The minimum number of points allowed in the spline used by the CSeqPos behaviour.
      *
      */

      const RwUInt32 MIN_SPLINE_CTRL_PTS = 4;

      /**
      *
      * CSeqPos class functions.
      *
      */

      RWS_IMPLEMENT_CLASSID(CSeqPos);
      RWS_REGISTERCLASS(CSeqPos);
      RWS_DS_REGISTER_START(CSeqPos)
      RWS_DS_REGISTER_END

      /**
      *
      * Constructor - initialize the object.
      *
      */

      CSeqPos::CSeqPos(const CAttributePacket &attr) :
         InitCEventHandler(&m_pAtomic),
         m_splineTVal(0),
         m_rotBlendTVal(0),
         m_posBlendTVal(0),
         m_posSourceA(PS_MATRIX),
         m_posSourceB(PS_MATRIX),
         m_rotSourceA(RS_MATRIX),
         m_rotSourceB(RS_MATRIX),
         m_scaleSource(SS_MATRIX),
         m_updatedSplineTVal(FALSE),
         m_updatedSplineData(FALSE),
         m_updatedRotBlendTVal(FALSE),
         m_updatedPosBlendTVal(FALSE),
         m_quatInvalid(TRUE),
         m_curPosInvalid(TRUE),
         m_pSpline(0),
         m_updated(FALSE)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqPos::CSeqPos");

         // Create atomic.

         m_pAtomic = CreateAtomicInWorldFromResource(attr, this);
         RWS_ASSERT(m_pAtomic, "Failed to create atomic");

         // Initialize other values.

         memset(&m_inPosOffset, 0, sizeof(RwV3d));
         memset(&m_rotOffset, 0, sizeof(RwV3d));
         memset(&m_outPosOffset, 0, sizeof(RwV3d));
         memset(&m_scaleOffset, 0, sizeof(RwV3d));
         memset(&m_blendedPosCache, 0, sizeof(RwV3d));
         memset(m_updatedInMtx, 0, sizeof(RwBool) * MAX_NUM_SRC_MATRICES);

         // Create temp matrix for blending matrix.

         m_pBlendedRotMatrix = RwMatrixCreate();
         RWS_ASSERT(m_pBlendedRotMatrix, "Failed to create blending matrix.");
         RwMatrixSetIdentity(m_pBlendedRotMatrix);

         // Initialize internal values - only want to do this class's, so do
         // NOT call virtual update function.

         UpdateObjectData();

         RWS_RETURNVOID();
      }

      /**
      *
      * Destructor - unlink from messages, etc.
      *
      */

      CSeqPos::~CSeqPos(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqPos::~CSeqPos");

         // Release messages.

         UnLinkMsg(m_frameOutMsg);
         UnRegisterMsg(m_frameOutMsg);

         // Release memory & messages for matrices.

         for (RwUInt32 index = 0; index < MAX_NUM_SRC_MATRICES; index++)
         {
            UnLinkMsg(m_frameInMsg[index]);
            UnRegisterMsg(m_frameInMsg[index]);
         }

         RwMatrixDestroy(m_pBlendedRotMatrix);

         RWS_RETURNVOID();
      }

      /**
      *
      * Standard RenderWare Studio event handler.
      *
      */

      void CSeqPos::HandleEvents(CMsg &msg)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqPos::HandleEvents");

         // Got new position matrix in ?

         for (RwUInt32 index = 0; index < MAX_NUM_SRC_MATRICES; index++)
         {
            if (msg.Id == m_frameInMsg[index] && msg.pData)
            {
               UpdateMatrix(static_cast<RwFrame *>(msg.pData), index);
            }
         }

         RWS_RETURNVOID();
      }

      /**
      *
      * Standard RenderWare Studio attribute handler.
      *
      */

      void CSeqPos::HandleAttributes(const CAttributePacket &attr)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqPos::HandleAttributes");

         HandleAttributesOnly(attr);

         if (m_updated)
         {
            // Do NOT call virtual one here, as this function should NOT be called by
            // the derived class's own HandleAttributes. Instead it should call the
            // 'HandleAttributesOnly' function, then do it's own update, then call it's
            // own update function (which should also call this class's UpDateObjectData
            // function).

            UpdateObjectData();
         }

         RWS_RETURNVOID();
      }

      /**
      *
      * Updates the attributes using the passed in packet ONLY, does not perform the
      * 'lazy update' function.
      *
      */

      void CSeqPos::HandleAttributesOnly(const CAttributePacket &attr)
      {
         RwBool sendPos = FALSE;

         // Allow system and atomic to handle their attributes.

         CAttributeHandler::HandleAttributes(attr);
         HandleSystemCommands(attr);
         m_pAtomic.HandleSystemCommands(attr);

         // Handle changes to control selections.

         CAttributeCommandIterator attrIt(attr, RWS_CLASSID_OF(CSeqPos));

         while (!attrIt.IsFinished())
         {
            switch (attrIt->GetCommandId())
            {
               // Messages.

            case MSG_FRAME_IN_A:

               UpdateInMatrixAMsg(this, attrIt->GetAs_void_ptr());
               m_updated = TRUE;
               break;

            case MSG_FRAME_IN_B:

               UpdateInMatrixBMsg(this, attrIt->GetAs_void_ptr());
               m_updated = TRUE;
               break;

            case MSG_FRAME_IN_C:

               UpdateInMatrixCMsg(this, attrIt->GetAs_void_ptr());
               m_updated = TRUE;
               break;

            case MSG_FRAME_IN_D:

               UpdateInMatrixDMsg(this, attrIt->GetAs_void_ptr());
               m_updated = TRUE;
               break;

            case MSG_FRAME_IN_E:

               UpdateInMatrixEMsg(this, attrIt->GetAs_void_ptr());
               m_updated = TRUE;
               break;

            case MSG_FRAME_OUT:

               UpdateOutMatrixMsg(this, attrIt->GetAs_void_ptr());
               sendPos = TRUE;
               break;

               // Attributes.

            case CMD_SPLINE_T_VAL:
               {
                  RwReal newVal = attrIt->GetAs_RwReal();

                  if (newVal != m_splineTVal)
                  {
                     m_splineTVal = newVal;
                     UpdateSplineTVal(this, &m_splineTVal);
                     m_updated = TRUE;
                  }
               }
               break;

            case CMD_POS_A_FROM:
               {
                  pos_source newSrc = static_cast<pos_source>(attrIt->GetAs_RwUInt32());

                  if (newSrc != m_posSourceA)
                  {                     
                     m_posSourceA = newSrc;
                     UpdatePosSource(this, &m_posSourceA);
                     m_updated = TRUE;
                  }
               }
               break;

            case CMD_POS_B_FROM:
               {
                  pos_source newSrc = static_cast<pos_source>(attrIt->GetAs_RwUInt32());

                  if (newSrc != m_posSourceB)
                  {                     
                     m_posSourceB = newSrc;
                     UpdatePosSource(this, &m_posSourceB);
                     m_updated = TRUE;
                  }
               }
               break;

            case CMD_POS_BLEND:
               {
                  RwReal newVal = attrIt->GetAs_RwReal();

                  if (newVal != m_posBlendTVal)
                  {
                     m_posBlendTVal = newVal;
                     UpdatePosBlendTVal(this, &m_posBlendTVal);
                     m_updated = TRUE;
                  }
               }
               break;

            case CMD_IN_POS_OFFSET:

               attrIt->GetCommandData(m_inPosOffset);
               m_updated = TRUE;
               break;

            case CMD_ROT_A_FROM:
               {
                  rot_source newSrc = static_cast<rot_source>(attrIt->GetAs_RwUInt32());

                  if (newSrc != m_rotSourceA)
                  {
                     m_rotSourceA = newSrc;
                     UpdateRotSource(this, &m_rotSourceA);
                     m_updated = TRUE;
                  }
               }
               break;

            case CMD_ROT_B_FROM:
               {
                  rot_source newSrc = static_cast<rot_source>(attrIt->GetAs_RwUInt32());

                  if (newSrc != m_rotSourceB)
                  {
                     m_rotSourceB = newSrc;
                     UpdateRotSource(this, &m_rotSourceB);
                     m_updated = TRUE;
                  }
               }
               break;

            case CMD_ROT_BLEND:
               {
                  RwReal newVal = attrIt->GetAs_RwReal();

                  if (newVal != m_rotBlendTVal)
                  {
                     m_rotBlendTVal = newVal;
                     UpdateRotBlendTVal(this, &m_rotBlendTVal);
                     m_updated = TRUE;
                  }
               }
               break;

            case CMD_IN_ROT_OFFSET:

               attrIt->GetCommandData(m_rotOffset);
               m_updated = TRUE;
               break;

            case CMD_SCALE_FROM:
               {
                  scale_source newSrc = static_cast<scale_source>(attrIt->GetAs_RwUInt32());

                  // Value check in update code.

                  if (newSrc != m_scaleSource)
                  {
                     m_scaleSource = newSrc;
                     m_updated = TRUE;
                  }
               }
               break;

            case CMD_IN_SCALE_OFFSET:

               attrIt->GetCommandData(m_scaleOffset);
               m_updated = TRUE;
               break;

            case CMD_OUT_POS_OFFSET:

               attrIt->GetCommandData(m_outPosOffset);
               m_updated = TRUE;
               break;
            }
            ++attrIt;
         }

         // If object not updated, but position message needs to be sent...

         if (!m_updated && sendPos)
         {
            // Send new position out...

            SendPositionOut(0);
         }
      }

      /**
      *
      * Intercepts CSystemCommands commands, searching for 'attach resource'
      * and 'load matrix' messages.
      *
      * \param attr Standard attribute data packet.
      *
      */

      void CSeqPos::HandleSystemCommands(const CAttributePacket &attr)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqPos::HandleSystemCommands");

         CAttributeCommandIterator attrIt(attr, RWS_CLASSID_OF(CSystemCommands));

         while (!attrIt.IsFinished())
         {
            switch (attrIt->GetCommandId())
            {
            case CSystemCommands::CMD_AttachResource:
               {
                  const RWSGUID * pResourceId;
                  attrIt->GetCommandData(&pResourceId);
                  AddResource(pResourceId);
                  m_updated = TRUE;
                  m_updatedSplineData = TRUE;
               }
               break;

            case CSystemCommands::CMD_LoadMatrix:
               {
                  UpdateMatrixDS(this, attrIt->GetAs_void_ptr());
               }
               break;
            }
            ++attrIt;
         }

         RWS_RETURNVOID();
      }

      /**
      *
      *  A Spline resource has been added, code to request it from the resource handler.
      *
      *  \param pResourceID  Pointer to determine which resource to add.
      *
      */

      void CSeqPos::AddResource(const RWSGUID *pResourceID)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqPos::AddResource");

         // Look up the object in the global store.

         const RwChar *ResourceType;
         const RwChar *ResourceName;
         const void *pObject = RWS::CResourceManager::FindById (pResourceID, 
            &ResourceType, 0, &ResourceName);

         if (pObject)
         {
            if (*static_cast<RwInt32 *>(const_cast<void *>(pObject)) == -1)
            {
               m_pSpline = 0;
            }
            else
            {
               if (!rwstrcmp ("rwID_SPLINE", ResourceType))
               {
                  // Store RenderWare Graphics spline data.

                  m_pSpline = static_cast<RpSpline*>(const_cast<void *>(pObject));
               }
            }
         }

         RWS_RETURNVOID();
      }

      /**
      *
      * Updates the stored matrix value with the on from the passed in frame, for
      * specified index. Also updates any position or rotation which uses the specified
      * matrix.
      *
      */

      void CSeqPos::UpdateMatrix(const RwFrame *pFrame, RwUInt32 index)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqPos::UpdateMatrix");
         RWS_ASSERT(pFrame, "Invalid frame pointer.");
         RWS_ASSERT(index < MAX_NUM_SRC_MATRICES, "invalid matrix index.");

         // Copy data out of matrix for later reference.

         RwMatrix *pRefMtx = RwFrameGetMatrix(const_cast<RwFrame *>(pFrame));
         RWS_ASSERT(pRefMtx, "Frame does not contain matrix.");

         m_inMtxCache[index].SetMatrix(pRefMtx);

         // Update anything that is using this matrix - need to call virtual one here to
         // give the derived classes a chance to update themselves.

         m_updated = TRUE;
         Update();

         RWS_RETURNVOID();
      }

      /**
      *
      * Virtual function, which is called to update the object. Derived classes should implement
      * their own version of this to allow them to be correctly update when matrices change, etc.
      *
      */

      void CSeqPos::Update(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqPos::Update");

         // Call the update objec data function and in so doing update the base object.

         UpdateObjectData();

         RWS_RETURNVOID();
      }

      /**
      *
      * Updates the internal values of the object, using the data passed in through attributes
      * and messages.
      *
      */

      void CSeqPos::UpdateObjectData(RwMatrix *pNonScaledMatrix)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqPos::UpdateObjectData");

         MatrixCache *pAlphaCache,
            *pBetaCache,
            *pScaleCache;

         // Needs to be updated ? MUST check for direct updates too.

         if (!m_updated)
         {
            RWS_RETURNVOID();
         }

         // Get matrix from atomic...

         RwFrame *pAtomicFrame = RpAtomicGetFrame(m_pAtomic.ptr());
         RWS_ASSERT(pAtomicFrame, "Atomic has no frame.");
         RwMatrix *pAtomicMatrix = RwFrameGetMatrix(pAtomicFrame);
         RWS_ASSERT(pAtomicMatrix, "Frame has no matrix.");

         // Apply rotation.

         pAlphaCache = GetSelObjRotation(m_rotSourceA);
         pBetaCache = GetSelObjRotation(m_rotSourceB);

         // Blending ?

         if (m_rotBlendTVal > 0.0f && m_rotBlendTVal < 1.0f)
         {
            // Need to blend the two, has either matrix been updated ?

            if (pAlphaCache->HasChanged() || pBetaCache->HasChanged() ||
               m_quatInvalid)
            {
               // Recalculate the slerp.

               RtQuatSetupSlerpCache(const_cast<RtQuat *>(pAlphaCache->Quat()),
                  const_cast<RtQuat *>(pBetaCache->Quat()), &m_slerpCache);
            }

            if (pAlphaCache->HasChanged() || pBetaCache->HasChanged() ||
               m_updatedRotBlendTVal || m_quatInvalid)
            {
               RtQuat blendQuat;

               // Get the 'blended' rotation matrix.

               RtQuatSlerp(&blendQuat, const_cast<RtQuat *>(pAlphaCache->Quat()),
                  const_cast<RtQuat *>(pBetaCache->Quat()), m_rotBlendTVal, &m_slerpCache);

               RtQuatConvertToMatrix(&blendQuat, m_pBlendedRotMatrix);

               m_updatedRotBlendTVal = FALSE;
            }

            // Use the blended matrix.

            RwMatrixCopy(pAtomicMatrix, m_pBlendedRotMatrix);

            // Clear flags.

            m_quatInvalid = FALSE;
         }
         else if (m_rotBlendTVal == 0.0f)
         {
            // Just using alpha.

            RwMatrixCopy(pAtomicMatrix, pAlphaCache->RotMatrix());
         }
         else
         {
            // Just using beta.

            RwMatrixCopy(pAtomicMatrix, pBetaCache->RotMatrix());
         }

         // Apply the user offset rotation.

         if (m_rotOffset.z != 0.0f)
         {
            const RwV3d zAxis = {0.0f, 0.0f, 1.0f};
            RwMatrixRotate(pAtomicMatrix, &zAxis, m_rotOffset.z, rwCOMBINEPRECONCAT);
         }

         if (m_rotOffset.y != 0.0f)
         {
            const RwV3d yAxis = {0.0f, 1.0f, 0.0f};
            RwMatrixRotate(pAtomicMatrix, &yAxis, m_rotOffset.y, rwCOMBINEPRECONCAT);
         }

         if (m_rotOffset.x != 0.0f)
         {
            const RwV3d xAxis = {1.0f, 0.0f, 0.0f};
            RwMatrixRotate(pAtomicMatrix, &xAxis, m_rotOffset.x, rwCOMBINEPRECONCAT);
         }

         // Update the position of the object & get the matrix cache for it and place 
         // position from this into the atomic...

         RwV3d *pPosVect;
         RwBool posChanged;

         pPosVect = RwMatrixGetPos(pAtomicMatrix);

         *pPosVect = *GetCurrentObjectPosition(posChanged);

         pPosVect->x += m_inPosOffset.x;
         pPosVect->y += m_inPosOffset.y;
         pPosVect->z += m_inPosOffset.z;

         // Get matrix being used for scaling...

         pScaleCache = GetSelObjScale(m_scaleSource);

         // If non-scaled matrix pointer, copy at this point...

         if (pNonScaledMatrix)
         {
            RwMatrixCopy(pNonScaledMatrix, pAtomicMatrix);
         }
         
         // Add in the scale (before everything else).

         RwV3d scaleVect;

         // Build scale vector.

         scaleVect = *pScaleCache->Scale();
         scaleVect.x = scaleVect.x + m_scaleOffset.x;
         scaleVect.y = scaleVect.y + m_scaleOffset.y;
         scaleVect.z = scaleVect.z + m_scaleOffset.z;

         RwMatrixScale(pAtomicMatrix, &scaleVect, rwCOMBINEPRECONCAT);

         // Update and send on the update information, if needed.

         if (pNonScaledMatrix)
         {
            RwMatrixUpdate(pNonScaledMatrix);
         }

         RwMatrixUpdate(pAtomicMatrix);
         RwFrameUpdateObjects(pAtomicFrame);

         SendPositionOut(pAtomicMatrix);

         // Clear flags.

         ClearSourceMtxCacheFlags(m_rotSourceA);
         ClearSourceMtxCacheFlags(m_rotSourceB);
         ClearSourceMtxCacheFlags(m_posSourceA);
         ClearSourceMtxCacheFlags(m_posSourceB);
         m_updated = FALSE;

         RWS_RETURNVOID();
      }

      /**
      *
      * Gets the objects position (updating if necessary) and returns a pointer to this value.
      * Will always return a valid pointer. The passed in changed flag, on exit, will be FALSE
      * if no change occured and TRUE if the position had to be recalculated.
      *
      */

      const RwV3d *CSeqPos::GetCurrentObjectPosition(RwBool &changed)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqPos::SendPositionOut");

         // Blending ?

         if (m_posBlendTVal > 0.0f && m_posBlendTVal < 1.0f)
         {
            MatrixCache *pPosAMtxCache,
               *pPosBMtrCache;

            // Get matrices involved.

            pPosAMtxCache = GetSelObjPosition(m_posSourceA);
            pPosBMtrCache = GetSelObjPosition(m_posSourceB);

            // Need to update the value ?

            if (pPosAMtxCache->HasChanged() || pPosBMtrCache->HasChanged() ||
               m_updatedPosBlendTVal || m_curPosInvalid)
            {
               m_blendedPosCache.x = pPosAMtxCache->Pos()->x +
                  (pPosBMtrCache->Pos()->x - pPosAMtxCache->Pos()->x) * m_posBlendTVal;
               m_blendedPosCache.y = pPosAMtxCache->Pos()->y +
                  (pPosBMtrCache->Pos()->y - pPosAMtxCache->Pos()->y) * m_posBlendTVal;
               m_blendedPosCache.z = pPosAMtxCache->Pos()->z +
                  (pPosBMtrCache->Pos()->z - pPosAMtxCache->Pos()->z) * m_posBlendTVal;

               m_updatedPosBlendTVal = FALSE;
               m_curPosInvalid = FALSE;
               changed = TRUE;
            }
            else
            {
               changed = FALSE;
            }

            RWS_RETURN(&m_blendedPosCache);
         }
         else if (m_posBlendTVal == 0.0f)
         {
            MatrixCache *pPosMtxCache = GetSelObjPosition(m_posSourceA);

            changed = pPosMtxCache->HasChanged();
            RWS_RETURN(pPosMtxCache->Pos());
         }
         else
         {
            MatrixCache *pPosMtxCache = GetSelObjPosition(m_posSourceB);

            changed = pPosMtxCache->HasChanged();
            RWS_RETURN(pPosMtxCache->Pos());
         }
      }

      /**
      *
      * If there is an output position message set up, then the function prepares the data and
      * sends it out on that event. The matrix for the atomic can (optionally) be passed in, if
      * this pointer is zero, the function will extract the matrix for itself.
      *
      */
      
      void CSeqPos::SendPositionOut(RwMatrix *pMatrix)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqPos::SendPositionOut");

         // Need to send the message ?

         if (IsLinked(m_frameOutMsg))
         {
            RwFrame *pOutFrame;
            RwMatrix *pOutMatrix;
            RwV3d posVect;
            RwMatrix *pAtomicMatrix;

            // Need to get frame or matrix from atomic ?

            if (pMatrix == 0)
            {
               RwFrame *pAtomicFrame = RpAtomicGetFrame(m_pAtomic.ptr());
               RWS_ASSERT(pAtomicFrame, "Atomic has no frame.");
               pAtomicMatrix = RwFrameGetMatrix(pAtomicFrame);
               RWS_ASSERT(pAtomicMatrix, "Frame has no matrix.");
            }
            else
            {
               pAtomicMatrix = pMatrix;
            }

            // Need to create temporary frame and matrix as will be modifying
            // the matrix before sending...

            pOutFrame = RwFrameCreate();
            RWS_ASSERT(pOutFrame, "Failed to build temp output frame.");
            pOutMatrix = RwFrameGetMatrix(pOutFrame);
            RWS_ASSERT(pOutMatrix, "Failed to build temp output matrix.");

            // Copy in current matrix.

            RwMatrixCopy(pOutMatrix, pAtomicMatrix);

            // Apply axis aligned positioning, x-axis first...

            if (m_outPosOffset.x != 0.0f)
            {
               posVect = *RwMatrixGetRight(pOutMatrix);

               RwV3dNormalize(&posVect, &posVect);

               posVect.x *= m_outPosOffset.x;
               posVect.y *= m_outPosOffset.x;
               posVect.z *= m_outPosOffset.x;

               RwMatrixTranslate(pOutMatrix, &posVect, rwCOMBINEPOSTCONCAT);
            }

            // ...now y-axis...

            if (m_outPosOffset.y != 0.0f)
            {
               posVect = *RwMatrixGetUp(pOutMatrix);

               RwV3dNormalize(&posVect, &posVect);

               posVect.x *= m_outPosOffset.y;
               posVect.y *= m_outPosOffset.y;
               posVect.z *= m_outPosOffset.y;

               RwMatrixTranslate(pOutMatrix, &posVect, rwCOMBINEPOSTCONCAT);
            }

            // ...now z-axis...

            if (m_outPosOffset.z != 0.0f)
            {
               posVect = *RwMatrixGetAt(pOutMatrix);

               RwV3dNormalize(&posVect, &posVect);

               posVect.x *= m_outPosOffset.z;
               posVect.y *= m_outPosOffset.z;
               posVect.z *= m_outPosOffset.z;

               RwMatrixTranslate(pOutMatrix, &posVect, rwCOMBINEPOSTCONCAT);
            }

            // Update and send...

            RwMatrixUpdate(pOutMatrix);
            RwFrameUpdateObjects(pOutFrame);

            CMsg msg(m_frameOutMsg, static_cast<void *>(pOutFrame));
            SendMsg(msg);

            RwFrameDestroy(pOutFrame);
         }

         RWS_RETURNVOID();
      }

      /**
      *
      * Looks at the matrix cache being used and any it relies on and updates if
      * needed. Returns a pointer to the top level matrix cache object.
      *
      */

      MatrixCache *CSeqPos::GetSelObjRotation(rot_source source)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqPos::GetSelObjRotation");

         MatrixCache *pMtxCache;

         // Get matrix that is being used and update if sub-matrices have changed.

         switch (source)
         {
         case RS_MATRIX:

            pMtxCache = &m_baseMtxCache;
            break;

         case RS_LOOK_AT_A:
            {
               RwBool changed;
               const RwV3d *pFromPos = GetCurrentObjectPosition(changed);

               if (m_inMtxCache[0].HasChanged() || m_updatedInMtx[0] || changed)
               {
                  const RwV3d *pLookPos;
                  RwV3d atVect;

                  pLookPos = m_inMtxCache[0].Pos();

                  atVect.x = pLookPos->x - pFromPos->x;
                  atVect.y = pLookPos->y - pFromPos->y;
                  atVect.z = pLookPos->z - pFromPos->z;

                  m_lookAtMtx[0].SetMatrix(*pFromPos, atVect, 0);
               }
               pMtxCache = &m_lookAtMtx[0];
            }
            break;

         case RS_LOOK_AT_B:
            {
               RwBool changed;
               const RwV3d *pFromPos = GetCurrentObjectPosition(changed);

               if (m_inMtxCache[1].HasChanged() || m_updatedInMtx[1] || changed)
               {
                  const RwV3d *pLookPos;
                  RwV3d atVect;

                  pLookPos = m_inMtxCache[1].Pos();

                  atVect.x = pLookPos->x - pFromPos->x;
                  atVect.y = pLookPos->y - pFromPos->y;
                  atVect.z = pLookPos->z - pFromPos->z;

                  m_lookAtMtx[1].SetMatrix(*pFromPos, atVect, 0);
               }
               pMtxCache = &m_lookAtMtx[1];
            }
            break;

         case RS_LOOK_AT_C:
            {
               RwBool changed;
               const RwV3d *pFromPos = GetCurrentObjectPosition(changed);

               if (m_inMtxCache[2].HasChanged() || m_updatedInMtx[2] || changed)
               {
                  const RwV3d *pLookPos;
                  RwV3d atVect;

                  pLookPos = m_inMtxCache[2].Pos();

                  atVect.x = pLookPos->x - pFromPos->x;
                  atVect.y = pLookPos->y - pFromPos->y;
                  atVect.z = pLookPos->z - pFromPos->z;

                  m_lookAtMtx[2].SetMatrix(*pFromPos, atVect, 0);
               }
               pMtxCache = &m_lookAtMtx[2];
            }
            break;

         case RS_LOOK_AT_D:
            {
               RwBool changed;
               const RwV3d *pFromPos = GetCurrentObjectPosition(changed);

               if (m_inMtxCache[3].HasChanged() || m_updatedInMtx[3] || changed)
               {
                  const RwV3d *pLookPos;
                  RwV3d atVect;

                  pLookPos = m_inMtxCache[3].Pos();

                  atVect.x = pLookPos->x - pFromPos->x;
                  atVect.y = pLookPos->y - pFromPos->y;
                  atVect.z = pLookPos->z - pFromPos->z;

                  m_lookAtMtx[3].SetMatrix(*pFromPos, atVect, 0);
               }
               pMtxCache = &m_lookAtMtx[3];
            }
            break;

         case RS_LOOK_AT_E:
            {
               RwBool changed;
               const RwV3d *pFromPos = GetCurrentObjectPosition(changed);

               if (m_inMtxCache[4].HasChanged() || m_updatedInMtx[4] || changed)
               {
                  const RwV3d *pLookPos;
                  RwV3d atVect;

                  pLookPos = m_inMtxCache[4].Pos();

                  atVect.x = pLookPos->x - pFromPos->x;
                  atVect.y = pLookPos->y - pFromPos->y;
                  atVect.z = pLookPos->z - pFromPos->z;

                  m_lookAtMtx[4].SetMatrix(*pFromPos, atVect, 0);
               }
               pMtxCache = &m_lookAtMtx[4];
            }
            break;

         case RS_ROT_IN_A:

            pMtxCache = &m_inMtxCache[0];
            break;

         case RS_ROT_IN_B:

            pMtxCache = &m_inMtxCache[1];
            break;

         case RS_ROT_IN_C:

            pMtxCache = &m_inMtxCache[2];
            break;

         case RS_ROT_IN_D:

            pMtxCache = &m_inMtxCache[3];
            break;

         case RS_ROT_IN_E:

            pMtxCache = &m_inMtxCache[4];
            break;

         case RS_SPLINE:

            UpdateSplineInfo();
            pMtxCache = &m_splineMtxCache;
            break;

         default:
            
            RWS_ASSERTFAIL("Unknown source matrix type.");
            break;
         }

         RWS_RETURN(pMtxCache);
      }

      /**
      *
      * Looks at the matrix cache being used and any it relies on and updates if
      * needed. Returns a pointer to the top level matrix cache object.
      *
      */

      MatrixCache *CSeqPos::GetSelObjPosition(pos_source source)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqPos::GetSelObjPosition");

         MatrixCache *pMtxCache;

         // Get matrix that is being used and update if sub-matrices have changed.

         switch (source)
         {
         case PS_MATRIX:

            pMtxCache = &m_baseMtxCache;
            break;

         case PS_MSG_A:

            pMtxCache = &m_inMtxCache[0];
            break;

         case PS_MSG_B:

            pMtxCache = &m_inMtxCache[1];
            break;

         case PS_MSG_C:

            pMtxCache = &m_inMtxCache[2];
            break;

         case PS_MSG_D:

            pMtxCache = &m_inMtxCache[3];
            break;

         case PS_MSG_E:

            pMtxCache = &m_inMtxCache[4];
            break;

         case PS_SPLINE:

            UpdateSplineInfo();
            pMtxCache = &m_splineMtxCache;
            break;

         default:
            
            RWS_ASSERTFAIL("Unknown source matrix type.");
            break;
         }

         RWS_RETURN(pMtxCache);
      }

      /**
      *
      * Looks at the matrix cache being used and any it relies on and updates if
      * needed. Returns a pointer to the top level matrix cache object.
      *
      */

      MatrixCache *CSeqPos::GetSelObjScale(scale_source source)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqPos::GetSelObjScale");

         MatrixCache *pMtxCache;

         switch (source)
         {
         case SS_MATRIX:

            pMtxCache = &m_baseMtxCache;
            break;

         case SS_MSG_A:

            pMtxCache = &m_inMtxCache[0];
            break;

         case SS_MSG_B:

            pMtxCache = &m_inMtxCache[1];
            break;

         case SS_MSG_C:

            pMtxCache = &m_inMtxCache[2];
            break;

         case SS_MSG_D:

            pMtxCache = &m_inMtxCache[3];
            break;

         case SS_MSG_E:

            pMtxCache = &m_inMtxCache[4];
            break;

         default:
            
            RWS_ASSERTFAIL("Unknown source matrix type.");
            break;
         }

         RWS_RETURN(pMtxCache);
      }

      /**
      *
      * Checks to see if the spline information has been updated, if so it updates
      * the spline matrix cache object and clears the spline related flags (ONLY).
      *
      */

      void CSeqPos::UpdateSplineInfo(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqPos::UpdateSplineInfo");

         if (m_updatedSplineTVal || m_updatedSplineData)
         {
            if (m_pSpline)
            {
               RwV3d pos,
                  at;

               RpSplineFindPosition(m_pSpline, rpSPLINEPATHSMOOTH, m_splineTVal, &pos, &at);
               m_splineMtxCache.SetMatrix(pos, at, 0);
            }

            // Clear flags, as matrix holding result has now been updated.

            m_updatedSplineData = FALSE;
            m_updatedSplineTVal = FALSE;
         }

         RWS_RETURNVOID();
      }
   
      /**
      *
      * Clears the 'changed' flag in the matrix and sub-matrices used by the specified
      * source for rotation.
      *
      */

      void CSeqPos::ClearSourceMtxCacheFlags(rot_source source)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqPos::ClearSourceMtxCacheFlags");

         // Get matrix that is being used and update if sub-matrices have changed.

         switch (source)
         {
         case RS_MATRIX:

            m_baseMtxCache.ClearChangedFlag();
            break;

         case RS_LOOK_AT_A:

            m_baseMtxCache.ClearChangedFlag();
            m_inMtxCache[0].ClearChangedFlag();
            m_lookAtMtx[0].ClearChangedFlag();
            m_updatedInMtx[0] = FALSE;
            break;

         case RS_LOOK_AT_B:

            m_baseMtxCache.ClearChangedFlag();
            m_inMtxCache[1].ClearChangedFlag();
            m_lookAtMtx[1].ClearChangedFlag();
            m_updatedInMtx[1] = FALSE;
            break;

         case RS_LOOK_AT_C:

            m_baseMtxCache.ClearChangedFlag();
            m_inMtxCache[2].ClearChangedFlag();
            m_lookAtMtx[2].ClearChangedFlag();
            m_updatedInMtx[2] = FALSE;
            break;

         case RS_LOOK_AT_D:

            m_baseMtxCache.ClearChangedFlag();
            m_inMtxCache[3].ClearChangedFlag();
            m_lookAtMtx[3].ClearChangedFlag();
            m_updatedInMtx[3] = FALSE;
            break;

         case RS_LOOK_AT_E:

            m_baseMtxCache.ClearChangedFlag();
            m_inMtxCache[4].ClearChangedFlag();
            m_lookAtMtx[4].ClearChangedFlag();
            m_updatedInMtx[4] = FALSE;
            break;

         case RS_ROT_IN_A:

            m_inMtxCache[0].ClearChangedFlag();
            break;

         case RS_ROT_IN_B:

            m_inMtxCache[1].ClearChangedFlag();
            break;

         case RS_ROT_IN_C:

            m_inMtxCache[2].ClearChangedFlag();
            break;

         case RS_ROT_IN_D:

            m_inMtxCache[3].ClearChangedFlag();
            break;

         case RS_ROT_IN_E:

            m_inMtxCache[4].ClearChangedFlag();
            break;

         case RS_SPLINE:

            m_splineMtxCache.ClearChangedFlag();
            break;

         default:
            
            RWS_ASSERTFAIL("Unknown source matrix type.");
            break;
         }

         RWS_RETURNVOID();
      }

      /**
      *
      * Clears the 'changed' flag in the matrix and sub-matrices used by the specified
      * source for position.
      *
      */

      void CSeqPos::ClearSourceMtxCacheFlags(pos_source source)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqPos::ClearSourceMtxCacheFlags");

         // Get matrix that is being used and update if sub-matrices have changed.

         switch (source)
         {
         case PS_MATRIX:

            m_baseMtxCache.ClearChangedFlag();
            break;

         case PS_MSG_A:

            m_inMtxCache[0].ClearChangedFlag();
            break;

         case PS_MSG_B:

            m_inMtxCache[1].ClearChangedFlag();
            break;

         case PS_MSG_C:

            m_inMtxCache[2].ClearChangedFlag();
            break;

         case PS_MSG_D:

            m_inMtxCache[3].ClearChangedFlag();
            break;

         case PS_MSG_E:

            m_inMtxCache[4].ClearChangedFlag();
            break;

         case PS_SPLINE:

            m_splineMtxCache.ClearChangedFlag();
            break;

         default:
            
            RWS_ASSERTFAIL("Unknown source matrix type.");
            break;
         }

         RWS_RETURNVOID();
      }
   }  // SEQUENCE
}     // RWS
