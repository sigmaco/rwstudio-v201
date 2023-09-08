/*****************************************************************************
*
* File :      CSeqPos.h
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

#ifndef C_SEQ_POS_HEADER
#define C_SEQ_POS_HEADER

#include "framework/core/attributehandler/cattributehandler.h"
#include "framework/core/eventhandler/ceventhandler.h"
#include "framework/core/attributehandler/allocation policies/LinearAllocationPolicy.h"
#include "framework/core/macros/debugmacros.h"
#include "framework/toolkits/world/smartptrs/catomicptr.h"

#include "rpspline.h"
#include "rtquat.h"
#include "rtslerp.h"

namespace RWS
{
   namespace SEQUENCE
   {
      /**
      *
      * Holds a matrix and elements of it (cached) for rapid access.
      *
      */

      const RwUInt32 MTX_CACHE_MATRIX_CHANGED      = 0x00000001;
      const RwUInt32 MTX_CACHE_ROTATION_CACHED     = 0x00000002;
      const RwUInt32 MTX_CACHE_SCALE_CACHED        = 0x00000004;
      const RwUInt32 MTX_CACHE_QUATERNION_CACHED   = 0x00000008;

      class MatrixCache
      {
      public:

         MatrixCache(void);
         MatrixCache(const RwMatrix *pRefMatrix);
         ~MatrixCache(void);

         const RtQuat *Quat(void);
         const RwMatrix *RotMatrix(void);
         const RwV3d *Pos(void);
         const RwV3d *Scale(void);

         RwBool HasChanged(void);
         void ClearChangedFlag(void);

         void SetMatrixFromUnaligned(const RwMatrix *pRefMatrix);
         void SetMatrix(const RwMatrix *pRefMatrix);
         void SetMatrix(const RwV3d &pos, const RwV3d &at, const RwV3d *pWorldUp);
         
      private:

         RtQuat m_quat;
         RwMatrix *m_pBaseMatrix,
            *m_pRotOnlyMatrix;
         RwV3d m_scale;
         RwUInt32 m_flags;             /**< MTX_CACHE_... values OR'ed. */
      };

      const RwUInt32 MAX_NUM_SRC_MATRICES = 5;

      class CSeqPos : public CSystemCommands, public CAttributeHandler, public CEventHandler,
         public LinearAllocationPolicy
      {
      public:

         RWS_MAKENEWCLASS(CSeqPos);
         RWS_DECLARE_CLASSID(CSeqPos);
         RWS_CATEGORY("Sequence");
         RWS_DESCRIPTION("Sequencable position",
            "Sequencable behaviour for controlling an entity's position.");

         RWS_BEGIN_COMMANDS            
            RWS_SEPARATOR("Spline", 0)
               RWS_ATTRIBUTE(CMD_SPLINE_T_VAL,
                  "Spline T value",
                  "Position along the spline controlled by t-value. "
                     "0 = start of spline. 1 = end of spline",
                  SLIDER,
                  RwReal,
                  RANGE(0, 0, 1))
            RWS_SEPARATOR("Position", 0)
               RWS_ATTRIBUTE(CMD_POS_A_FROM,
                  "Position from (A)",
                  "Where the position is set from for position channel A.",
                  LIST,
                  RwUInt32,
                  LIST("Matrix|Pos in message A|Pos in message B|Pos in message C|"
                     "Pos in message D|Pos in message E|Spline"))
               RWS_ATTRIBUTE(CMD_POS_B_FROM,
                  "Position from (B)",
                  "Where the position is set from for position channel B.",
                  LIST,
                  RwUInt32,
                  LIST("Matrix|Pos in message A|Pos in message B|Pos in message C|"
                     "Pos in message D|Pos in message E|Spline"))
               RWS_ATTRIBUTE(CMD_POS_BLEND,
                  "Position blend",
                  "Blending of positions from channel A and B. "
                     "0 = 100% channel A. 1 = 100% channel B.",
                  SLIDER,
                  RwReal,
                  RANGE(0, 0, 1))
               RWS_ATTRIBUTE(CMD_IN_POS_OFFSET,
                  "Position offset",
                  "Offset applied to position, aligned to axes, after blending.",
                  VECTOR,
                  RwV3d,
                  RANGES((-1000, 0, 1000), (-1000, 0, 1000), (-1000, 0, 1000)))
            RWS_SEPARATOR("Rotation", 0)
               RWS_ATTRIBUTE(CMD_ROT_A_FROM,
                  "Rotation from - alpha",
                  "How the rotation is set - for channel alpha.",
                  LIST,
                  RwUInt32,
                  LIST("Matrix|Look at pos in msg A|Look at pos in msg B|"
                     "Look at pos in msg C|Look at pos in msg D|Look at pos in msg E|"
                     "Rotation in pos msg A|Rotation in pos msg B|Rotation in pos msg C|"
                     "Rotation in pos msg D|Rotation in pos msg E|Spline tangent"))
               RWS_ATTRIBUTE(CMD_ROT_B_FROM,
                  "Rotation from - beta",
                  "How the rotation is set - for channel beta.",
                  LIST,
                  RwUInt32,
                  LIST("Matrix|Look at pos in msg A|Look at pos in msg B|"
                     "Look at pos in msg C|Look at pos in msg D|Look at pos in msg E|"
                     "Rotation in pos msg A|Rotation in pos msg B|Rotation in pos msg C|"
                     "Rotation in pos msg D|Rotation in pos msg E|Spline tangent"))
               RWS_ATTRIBUTE(CMD_ROT_BLEND,
                  "Rotation blend",
                  "Blending of rotations from channel A and B. "
                     "0 = 100% channel alpha. 1 = 100% channel beta.",
                  SLIDER,
                  RwReal,
                  RANGE(0, 0, 1))
               RWS_ATTRIBUTE(CMD_IN_ROT_OFFSET,
                  "Rotation offset",
                  "Offset applied to rotation. In degrees.",
                  VECTOR,
                  RwV3d,
                  RANGES((-360, 0, 360), (-360, 0, 360), (-360, 0, 360)))
            RWS_SEPARATOR("Scale", 0)
               RWS_ATTRIBUTE(CMD_SCALE_FROM,
                  "Scale from",
                  "Where the scale is set from.",
                  LIST,
                  RwUInt32,
                  LIST("Matrix|Scale in message A|Scale in message B|"
                     "Scale in message C|Scale in message D|Scale in message E"))
               RWS_ATTRIBUTE(CMD_IN_SCALE_OFFSET,
                  "Scale offset",
                  "Offset applied to scale. Added, ie final = base scale + offset.",
                  VECTOR,
                  RwV3d,
                  RANGES((-100, 0, 100), (-100, 0, 100), (-100, 0, 100)))
            RWS_SEPARATOR("Messages", 0)
               RWS_MESSAGE(MSG_FRAME_IN_A,
                  "Frame in A",
                  "Name of event sending frame of other entity (A).",
                  RECEIVE,
                  RwFrame*,
                  0)
               RWS_MESSAGE(MSG_FRAME_IN_B,
                  "Frame in B",
                  "Name of event sending frame of other entity (B).",
                  RECEIVE,
                  RwFrame*,
                  0)
               RWS_MESSAGE(MSG_FRAME_IN_C,
                  "Frame in C",
                  "Name of event sending frame of other entity (C).",
                  RECEIVE,
                  RwFrame*,
                  0)
               RWS_MESSAGE(MSG_FRAME_IN_D,
                  "Frame in D",
                  "Name of event sending frame of other entity (D).",
                  RECEIVE,
                  RwFrame*,
                  0)
               RWS_MESSAGE(MSG_FRAME_IN_E,
                  "Frame in E",
                  "Name of event sending frame of other entity (E).",
                  RECEIVE,
                  RwFrame*,
                  0)
               RWS_MESSAGE(MSG_FRAME_OUT,
                  "Frame out",
                  "Name of event to use to send frame of this entity. Data will be "
                     "send if name changes or other values change.",
                  TRANSMIT,
                  RwFrame*,
                  0)
               RWS_ATTRIBUTE(CMD_OUT_POS_OFFSET,
                  "Position offset",
                  "Offset applied to position before sending, aligned to rotation of entity.",
                  VECTOR,
                  RwV3d,
                  RANGES((-1000, 0, 1000), (-1000, 0, 1000), (-1000, 0, 1000)))
         RWS_END_COMMANDS;

         RWS_DS_TABLE_START

            RWS_DS_ENABLE_OVERRIDE(CSystemCommands)

            RWS_DS_START_CMD_SECTION

               RWS_DS_CMD(CMD_SPLINE_T_VAL,
                  RWS_CMD_DATA(CSeqPos, m_splineTVal),
                  RWS_CMD_FUNC(CSeqPos, UpdateSplineTVal))
               RWS_DS_CMD(CMD_POS_A_FROM,
                  RWS_CMD_DATA(CSeqPos, m_posSourceA),
                  RWS_CMD_FUNC(CSeqPos, UpdatePosSource))
               RWS_DS_CMD(CMD_POS_B_FROM,
                  RWS_CMD_DATA(CSeqPos, m_posSourceB),
                  RWS_CMD_FUNC(CSeqPos, UpdatePosSource))
               RWS_DS_CMD(CMD_POS_BLEND,
                  RWS_CMD_DATA(CSeqPos, m_posBlendTVal),
                  RWS_CMD_FUNC(CSeqPos, UpdatePosBlendTVal))
               RWS_DS_CMD(CMD_IN_POS_OFFSET,
                  RWS_CMD_DATA(CSeqPos, m_inPosOffset),
                  RWS_CMD_FUNC_BLANK)
               RWS_DS_CMD(CMD_ROT_A_FROM,
                  RWS_CMD_DATA(CSeqPos, m_rotSourceA),
                  RWS_CMD_FUNC(CSeqPos, UpdateRotSource))
               RWS_DS_CMD(CMD_ROT_B_FROM,
                  RWS_CMD_DATA(CSeqPos, m_rotSourceB),
                  RWS_CMD_FUNC(CSeqPos, UpdateRotSource))
               RWS_DS_CMD(CMD_ROT_BLEND,
                  RWS_CMD_DATA(CSeqPos, m_rotBlendTVal),
                  RWS_CMD_FUNC(CSeqPos, UpdateRotBlendTVal))
               RWS_DS_CMD(CMD_IN_ROT_OFFSET,
                  RWS_CMD_DATA(CSeqPos, m_rotOffset),
                  RWS_CMD_FUNC_BLANK)
               RWS_DS_CMD(CMD_SCALE_FROM,
                  RWS_CMD_DATA(CSeqPos, m_scaleSource),
                  RWS_CMD_FUNC_BLANK)
               RWS_DS_CMD(CMD_IN_SCALE_OFFSET,
                  RWS_CMD_DATA(CSeqPos, m_scaleOffset),
                  RWS_CMD_FUNC_BLANK)
               RWS_DS_CMD(MSG_FRAME_IN_A,
                  RWS_CMD_DATA_BLANK,
                  RWS_CMD_FUNC(CSeqPos, UpdateInMatrixAMsg))
               RWS_DS_CMD(MSG_FRAME_IN_B,
                  RWS_CMD_DATA_BLANK,
                  RWS_CMD_FUNC(CSeqPos, UpdateInMatrixBMsg))
               RWS_DS_CMD(MSG_FRAME_IN_C,
                  RWS_CMD_DATA_BLANK,
                  RWS_CMD_FUNC(CSeqPos, UpdateInMatrixCMsg))
               RWS_DS_CMD(MSG_FRAME_IN_D,
                  RWS_CMD_DATA_BLANK,
                  RWS_CMD_FUNC(CSeqPos, UpdateInMatrixDMsg))
               RWS_DS_CMD(MSG_FRAME_IN_E,
                  RWS_CMD_DATA_BLANK,
                  RWS_CMD_FUNC(CSeqPos, UpdateInMatrixEMsg))
               RWS_DS_CMD(MSG_FRAME_OUT,
                  RWS_CMD_DATA_BLANK,
                  RWS_CMD_FUNC(CSeqPos, UpdateOutMatrixMsg))
               RWS_DS_CMD(CMD_OUT_POS_OFFSET,
                  RWS_CMD_DATA(CSeqPos, m_outPosOffset),
                  RWS_CMD_FUNC_BLANK)

            RWS_DS_END_CMD_SECTION(CSeqPos)

            RWS_DS_START_OVERRIDE_SECTION(CSystemCommands, CSeqPos)
               RWS_DS_OVERRIDE_CMD(CSystemCommands,
                  CMD_LoadMatrix,
                  RWS_CMD_DATA_BLANK,
                  RWS_CMD_FUNC(CSeqPos, UpdateMatrixDS))
            RWS_DS_END_OVERRIDE_SECTION(CSystemCommands)

         RWS_DS_TABLE_END

         RWS_REGISTER_DS_POST_UPDATE_FUNC(CSeqPos, UpdateDataDirect);

         CSeqPos(const CAttributePacket &attr);
         virtual ~CSeqPos(void);

         virtual void HandleEvents(CMsg &msg);
         virtual void HandleAttributes(const CAttributePacket &attr);

      private:

         enum pos_source                     // MUST match order of lists in attributes.
         {
            PS_MATRIX = 0,
            PS_MSG_A,
            PS_MSG_B,
            PS_MSG_C,
            PS_MSG_D,
            PS_MSG_E,
            PS_SPLINE,

            PS_TOTAL                         // MUST be last in list.
         };

         enum rot_source                     // MUST match order of lists in attributes.
         {
            RS_MATRIX = 0,
            RS_LOOK_AT_A,
            RS_LOOK_AT_B,
            RS_LOOK_AT_C,
            RS_LOOK_AT_D,
            RS_LOOK_AT_E,
            RS_ROT_IN_A,
            RS_ROT_IN_B,
            RS_ROT_IN_C,
            RS_ROT_IN_D,
            RS_ROT_IN_E,
            RS_SPLINE,

            RS_TOTAL                         // MUST be last in list.
         };

         enum scale_source                   // MUST match order of lists in attributes.
         {
            SS_MATRIX = 0,
            SS_MSG_A,
            SS_MSG_B,
            SS_MSG_C,
            SS_MSG_D,
            SS_MSG_E,

            SS_TOTAL                         // MUST be last in list.
         };

         // 'Direct-set' callback functions...

         /**
         *
         * Causes the system to 'flag' that the t-value of the spline has changed. It does NOT
         * change the t-value directy, use the data pointer in the attribute table to do this 
         * as it does not require a temporary copy of the value to be created.
         *
         */

         static void UpdateSplineTVal(void *pThis, const void *pData)
         {
            CSeqPos *pSeqPos = static_cast<CSeqPos *>(pThis);
            RWS_ASSERT(pSeqPos->m_splineTVal >= 0.0f && pSeqPos->m_splineTVal <= 1.0f,
               "new spline t-value is not in range of 0 to 1 (inclusive).");
            pSeqPos->m_updatedSplineTVal = TRUE;
         }

         /**
         *
         * Causes the system to 'flag' that the position is invalid within the object (ie it has
         * changed). Called when several values have changed - ie the pData value is effectively
         * unknown, in that it may be m_posSourceA or m_posSourceB, however it WILL be a
         * 'pos_source' value.  It does NOT change the source directly, use the data pointer in 
         * the attribute table to do this as it does not require a temporary copy of the value to 
         * be created.
         *
         */

         static void UpdatePosSource(void *pThis, const void *pData)
         {
            CSeqPos *pSeqPos = static_cast<CSeqPos *>(pThis);
            const pos_source *pSource = static_cast<const pos_source *>(pData);
            RWS_ASSERT(*pSource < PS_TOTAL, "Invalid position source value.");
            pSeqPos->m_curPosInvalid = TRUE;
         }

         /**
         *
         * Causes the system to 'flag' that the positional blending t-value has changed. It does
         * NOT change the t-value directly, use the data pointer in the attribute table to do this
         * as it does not require a temporary copy of the value to be created.
         *
         */

         static void UpdatePosBlendTVal(void *pThis, const void *pData)
         {
            CSeqPos *pSeqPos = static_cast<CSeqPos *>(pThis);
            RWS_ASSERT(pSeqPos->m_posBlendTVal >= 0.0f && pSeqPos->m_posBlendTVal <= 1.0f,
               "new position blend t-value is not in range of 0 to 1 (inclusive).");
            pSeqPos->m_updatedPosBlendTVal = TRUE;
            pSeqPos->m_curPosInvalid = TRUE;
         }

         /**
         *
         * Causes the system to 'flag' that the rotation is invalid within the object (ie it has
         * changed). Called when several values have changed - ie the pData value is effectively
         * unknown, in that it may be m_rotSourceA or m_rotSourceB, however it WILL be a
         * 'rot_source' value.  It does NOT change the source directly, use the data pointer in 
         * the attribute table to do this as it does not require a temporary copy of the value to 
         * be created.
         *
         */

         static void UpdateRotSource(void *pThis, const void *pData)
         {
            CSeqPos *pSeqPos = static_cast<CSeqPos *>(pThis);
            const rot_source *pSource = static_cast<const rot_source *>(pData);
            RWS_ASSERT(*pSource < RS_TOTAL, "Invalid rotation source value.");
            pSeqPos->m_quatInvalid = TRUE;
         }

         /**
         *
         * Causes the system to 'flag' that the rotation blending t-value has changed. It does
         * NOT change the t-value directly, use the data pointer in the attribute table to do this
         * as it does not require a temporary copy of the value to be created.
         *
         */

         static void UpdateRotBlendTVal(void *pThis, const void *pData)
         {
            CSeqPos *pSeqPos = static_cast<CSeqPos *>(pThis);
            RWS_ASSERT(pSeqPos->m_posBlendTVal >= 0.0f && pSeqPos->m_posBlendTVal <= 1.0f,
               "new position blend t-value is not in range of 0 to 1 (inclusive).");
            pSeqPos->m_updatedRotBlendTVal = TRUE;
            pSeqPos->m_quatInvalid = TRUE;
         }

         /**
         *
         * Called to update the message linkage for input matrix A message. Data holds the name
         * of the event to change to. This function actually updates the value and so the pData
         * pointer MUST be a valid string.
         *
         */

         static void UpdateInMatrixAMsg(void *pThis, const void *pData)
         {
            RWS_ASSERT(pData, "Invalid message name string.");
            CSeqPos *pSeqPos = static_cast<CSeqPos *>(pThis);
            const char *pMsgName = static_cast<const char *>(pData);
            ReplaceLinkedMsg(*pSeqPos, pSeqPos->m_frameInMsg[0], pMsgName, "RwFrame*");
            pSeqPos->m_updatedInMtx[0] = TRUE;
         }

         /**
         *
         * Called to update the message linkage for input matrix B message. Data holds the name
         * of the event to change to. This function actually updates the value and so the pData
         * pointer MUST be a valid string.
         *
         */

         static void UpdateInMatrixBMsg(void *pThis, const void *pData)
         {
            RWS_ASSERT(pData, "Invalid message name string.");
            CSeqPos *pSeqPos = static_cast<CSeqPos *>(pThis);
            const char *pMsgName = static_cast<const char *>(pData);
            ReplaceLinkedMsg(*pSeqPos, pSeqPos->m_frameInMsg[1], pMsgName, "RwFrame*");
            pSeqPos->m_updatedInMtx[1] = TRUE;
         }

         /**
         *
         * Called to update the message linkage for input matrix C message. Data holds the name
         * of the event to change to. This function actually updates the value and so the pData
         * pointer MUST be a valid string.
         *
         */

         static void UpdateInMatrixCMsg(void *pThis, const void *pData)
         {
            RWS_ASSERT(pData, "Invalid message name string.");
            CSeqPos *pSeqPos = static_cast<CSeqPos *>(pThis);
            const char *pMsgName = static_cast<const char *>(pData);
            ReplaceLinkedMsg(*pSeqPos, pSeqPos->m_frameInMsg[2], pMsgName, "RwFrame*");
            pSeqPos->m_updatedInMtx[2] = TRUE;
         }

         /**
         *
         * Called to update the message linkage for input matrix D message. Data holds the name
         * of the event to change to. This function actually updates the value and so the pData
         * pointer MUST be a valid string.
         *
         */

         static void UpdateInMatrixDMsg(void *pThis, const void *pData)
         {
            RWS_ASSERT(pData, "Invalid message name string.");
            CSeqPos *pSeqPos = static_cast<CSeqPos *>(pThis);
            const char *pMsgName = static_cast<const char *>(pData);
            ReplaceLinkedMsg(*pSeqPos, pSeqPos->m_frameInMsg[3], pMsgName, "RwFrame*");
            pSeqPos->m_updatedInMtx[3] = TRUE;
         }

         /**
         *
         * Called to update the message linkage for input matrix E message. Data holds the name
         * of the event to change to. This function actually updates the value and so the pData
         * pointer MUST be a valid string.
         *
         */

         static void UpdateInMatrixEMsg(void *pThis, const void *pData)
         {
            RWS_ASSERT(pData, "Invalid message name string.");
            CSeqPos *pSeqPos = static_cast<CSeqPos *>(pThis);
            const char *pMsgName = static_cast<const char *>(pData);
            ReplaceLinkedMsg(*pSeqPos, pSeqPos->m_frameInMsg[4], pMsgName, "RwFrame*");
            pSeqPos->m_updatedInMtx[4] = TRUE;
         }

         /**
         *
         * Called to update the message linkage for the output matrix message. Data holds the name
         * of the event to change to. This function actually updates the value and so the pData
         * pointer MUST be a valid string.
         *
         */

         static void UpdateOutMatrixMsg(void *pThis, const void *pData)
         {
            RWS_ASSERT(pData, "Invalid message name string.");
            CSeqPos *pSeqPos = static_cast<CSeqPos *>(pThis);
            const char *pMsgName = static_cast<const char *>(pData);
            ReplaceLinkedMsg(*pSeqPos, pSeqPos->m_frameOutMsg, pMsgName, "RwFrame*");
         }

         /**
         *
         * Called to update the position matrix. Data holds the new matrix (unaligned).
         * This function actually updates the value and so the pData pointer MUST be a
         * valid matrix. This function 'overrides' the CSystemCommands matrix direct-set.
         *
         */

         static void UpdateMatrixDS(void *pThis, const void *pData)
         {
            RWS_ASSERT(pData, "Invalid matrix pointer.");
            CSeqPos *pSeqPos = static_cast<CSeqPos *>(pThis);
            const RwMatrix *pUnalignedMtx = static_cast<const RwMatrix *>(pData);

            if (pSeqPos->m_pAtomic)
            {
               RwFrame *pFrame = RpAtomicGetFrame(pSeqPos->m_pAtomic.ptr());
               if (pFrame)
               {
                  CSystemCommands::UpdateFrame(*pFrame, pUnalignedMtx);
                  pSeqPos->m_baseMtxCache.SetMatrixFromUnaligned(pUnalignedMtx);
                  pSeqPos->m_updated = TRUE;
               }
            }
         }

         /**
         *
         * Called when all attribute updates have been applied to allow the internal calculations
         * to be performed. This 'lazy update' stops the system from doing unnecessary calculations.
         * If this function is called then an update did occur.
         *
         * NOTE: In derived classes, use the virtual 'Update' function and do NOT use another
         *       'lazy update' function. This way, the derived class will be updated here and
         *       when matrices are changed, etc.
         *
         */

         static void UpdateDataDirect(void *pThis)
         {
            CSeqPos *pSeqPos = static_cast<CSeqPos *>(pThis);
            pSeqPos->m_updated = TRUE;

            // Call virtual function, to allow derived classes to update themselves.

            pSeqPos->Update();
         }

         // Other functions within class...

         void HandleSystemCommands(const CAttributePacket &attr);
         void AddResource(const RWSGUID *pResourceID);
         void UpdateMatrix(const RwFrame *pFrame, RwUInt32 index);
         const RwV3d *GetCurrentObjectPosition(RwBool &changed);
         void SendPositionOut(RwMatrix *pMatrix);
         MatrixCache *GetSelObjRotation(rot_source source);
         MatrixCache *GetSelObjPosition(pos_source source);
         MatrixCache *GetSelObjScale(scale_source source);
         void UpdateSplineInfo(void);
         void ClearSourceMtxCacheFlags(rot_source source);
         void ClearSourceMtxCacheFlags(pos_source source);

         // Stored reference data - from messages...

         MatrixCache m_inMtxCache[MAX_NUM_SRC_MATRICES],
            m_baseMtxCache,
            m_splineMtxCache,
            m_lookAtMtx[MAX_NUM_SRC_MATRICES];

         // From attributes...

         RwReal m_splineTVal,
            m_rotBlendTVal,
            m_posBlendTVal;
         pos_source m_posSourceA,
            m_posSourceB;
         rot_source m_rotSourceA,
            m_rotSourceB;
         scale_source m_scaleSource;
         CEventId m_frameInMsg[MAX_NUM_SRC_MATRICES],
            m_frameOutMsg;
         RwV3d m_inPosOffset,
            m_rotOffset,
            m_outPosOffset,
            m_scaleOffset,
            m_blendedPosCache;               /**< If blending positions, this holds the last result. */

         // Internal data.

         RwBool m_updatedSplineTVal,
            m_updatedSplineData,
            m_updatedRotBlendTVal,
            m_updatedPosBlendTVal,
            m_updatedInMtx[MAX_NUM_SRC_MATRICES],
            m_quatInvalid,
            m_curPosInvalid;
         RwMatrix *m_pBlendedRotMatrix;
         RpSpline *m_pSpline;                /**< Pointer to the RenderWare Spline. */
         RtQuatSlerpCache m_slerpCache;      /**< Used when rotation blending t-value is not 0 or 1. */

      protected:

         void HandleAttributesOnly(const CAttributePacket &attr);
         void UpdateObjectData(RwMatrix *pNonScaledMatrix = 0);
         virtual void Update(void);

         RwBool m_updated;
         CAtomicPtr m_pAtomic;               /**< Atomic for instance. */
      };
   }     // SEQUENCE
}        // RWS
#endif   // #ifndef C_SEQ_POS_HEADER
