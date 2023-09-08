/*****************************************************************************
*
* File :      Particle.h
*
* Abstract :  Particle toolkit using RW PTank
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

#include <rpptank.h> 
#include <string.h>

#include "framework/toolkits/world/smartptrs/catomicptr.h"
#include "framework/toolkits/math/maths.h"

#if defined(SKY)
#include <eekernel.h>
#include "framework/toolkits/playstation2/scratchpad.h"
#endif


namespace RWS
{
   namespace Particle
   {
      template<class T> class CycPartBuff;

      /**
      *
      * \ingroup Particles
      *
      * Class to provide communication of information to a base class on the location of
      * particles being processed or free slots for new particles. Is also used in the
      * reverse direction to allow the particle base class to request actions by the
      * buffer class.
      *
      * NOTE: Calls to the buffer class Activate / Deactivate functions will change the numActive value
      * and they will update the pActiveFlags array values too.
      *
      */

      class CPB_Marker
      {
      public:

         RwUInt32 internalA,           /**< Internal value, private. Used by buffer class only. */
               internalB;              /**< Internal value, private. Used by buffer class only. */
         bool inUsedList;              /**< Internal value, private. Used to indicate which list is being parsed. */

         // ---------

         RwUInt32 blockNum,            /**< Block being used (for linkage to local data). This is used by the 
                                            base particle class select the appropriate data blocks from it's
                                            internal arrays, lists, etc... */
               numInBlock,             /**< Total number of particles in the block. */
               numActive;              /**< Number of particles in block which are marked as active. */
         RwUInt32 index;               /**< Use depends on call, used to pass data back, etc... */
         RwUInt32 *pLifeTimes;         /**< Lifetime values for each particle. Caller must update these. */
         bool *pActiveFlags;           /**< Deactivate / activate via call to CycPartBuff functions ONLY. */
         RpAtomic *pTankAtomic;        /**< Caller must not change this. Provided to allow access to data. */
         RpPTankLockStruct sizeLock;   /**< pTank lock on size data for particle - DO NOT call
                                            'RpPTankAtomicUnlock'. Call 'GetFreeEnd' after every successful
                                            call to 'GetFree', before recalling 'GetFree'. When 'Deactivating'
                                            don't change deactivated particle's data after the call */
      };

      /**
      *
      * \ingroup Particles
      *
      * Flag to indicate acceleration or one-time-boost is enabled in stage or animation.
      *
      * \see CPB_BasePart
      *
      */

      const RwUInt32 CPB_BP_ANIM_ACCEL   = 0x00000001;

      /**
      *
      * \ingroup Particles
      *
      * Flag to indicate gravity is enabled.
      *
      * \see CPB_BasePart
      *
      */

      const RwUInt32 CPB_BP_ANIM_GRAVITY = 0x00000002;

      /**
      *
      * \ingroup Particles
      *
      * Flag to indicate drag is enabled.
      *
      * \see CPB_BasePart
      *
      */

      const RwUInt32 CPB_BP_ANIM_DRAG    = 0x00000004;

      /**
      *
      * \ingroup Particles
      *
      * Flag to indicate color animation or setting is enabled.
      *
      * \see CPB_BasePart
      *
      */

      const RwUInt32 CPB_BP_ANIM_COL     = 0x00000008;

      /**
      *
      * \ingroup Particles
      *
      * Flag to indicate size animation or setting is enabled.
      *
      * \see CPB_BasePart
      *
      */

      const RwUInt32 CPB_BP_ANIM_SIZE    = 0x00000010;

      /**
      *
      * \ingroup Particles
      *
      * Flag to indicate texture index animation or setting is enabled.
      *
      * \see CPB_TexPart
      *
      */

      const RwUInt32 CPB_BP_ANIM_SUB_TEX = 0x00000020;

      /**
      *
      * \ingroup Particles
      *
      * Used to specify the size of the particle blocks within the CycPartBuff template.
      *
      * \see CycPartBuff
      *
      */

      const RwUInt32 CPB_MAX_PARTICLES_IN_BLOCK = 100;

      /**
      *
      * \ingroup Particles
      *
      * Base particle type for use with cyclic particle buffer. Provides size, color, velocity, gravity,
      * drag and other attributes. Used by the CFXPartSpray behavior.
      *
      * \see FX::CFXPartSpray
      * \see AnimData
      * \see StateData
      *
      */

      class CPB_BasePart
      {
      public:

         /**
         *
         * Provides enum values for accessing section of each stage of the particle control.
         *
         */

         enum BASE_PART_STATE
         {
            BPS_PRIMARY_START = 0,  /**< Primary stage, start state */
            BPS_PRIMARY_END,        /**< Primary stage, end state */
            BPS_SECONDARY_START,    /**< Secondary stage, start state */
            BPS_SECONDARY_END,      /**< Secondary stage, end state */

            BPS_TOTAL               /**< MUST be end in list. Indicates total number of stages */
         };

#if _MSC_VER == 1200       // If Microsoft VC version 6...
      public:              // Bug in VC stops CPB_TexPart from using this class if it's protected.
#else                      // This is a known MS issue (v6 anyway).
      protected:
#endif

         /**
         *
         * \ingroup Particles
         *
         * Per particle animation states and data. The 'CPB_BP_ANIM_...' flags are used to indicate what,
         * if anything, is being animated. Protected sub-structure.
         *
         * \see CPB_BasePart
         *
         */

         class AnimData
         {
         public:

            RwV3d m_Vel;
            RwUInt32 m_flags;       /**< ORed 'CPB_BP_ANIM_...' values. */
            RwReal m_accel,         /**< Acceleration. Used on each frame (if animating). */
                  m_grav,           /**< Gravity. Used on each frame (animated or not). */
                  m_gravMod,        /**< Gravity modification value. Applied to grav on each frame. */
                  m_drag,           /**< Drag factor. Used on each frame (animated or not). */
                  m_dragMod;        /**< Drag modification value. Applied to drag on each frame. */

            RwInt16 m_r,           /**< Red component of animated color. RwReal needed as step can be < 1 */
                  m_rMod,           /**< Red animation step value, applied each frame (if animating). */
                  m_g,              /**< Green component of animated color. RwReal needed as step can be < 1 */
                  m_gMod,           /**< Green animation step value, applied each frame (if animating). */                    
                  m_b,              /**< Blue component of animated color. RwReal needed as step can be < 1 */
                  m_bMod,           /**< Blue animation step value, applied each frame (if animating). */                    
                  m_a,              /**< Alpha component of animated color. RwReal needed as step can be < 1 */
                  m_aMod;           /**< Alpha animation step value, applied each frame (if animating). */                    

            RwReal m_sizeMod;       /**< Size animation step value, applied each frame (if animating). */
         };


         /**
         *
         * \ingroup Particles
         *
         * Per state structure, i.e. start / end states. Holds information on what is being animated / set
         * and the values used. These are then used to setup the animation values. Protected sub-structure.
         *
         * \see CPB_BasePart
         *
         */

         class StateData
         {
         public:

            RwUInt32 m_partFlags,   /**< Ored 'CPB_BP_ANIM...' values. Primary start stage MUST have all flags. */
                  m_red,            /**< Base red value (0 -> 255). */
                  m_redSpread,      /**< Red randomness (0 -> 255). */
                  m_green,          /**< Base green value (0 -> 255). */
                  m_greenSpread,    /**< Green randomness (0 -> 255). */
                  m_blue,           /**< Base blue value (0 -> 255). */
                  m_blueSpread,     /**< Blue randomness (0 -> 255). */
                  m_alpha,          /**< Base alpha value (0 -> 255). */
                  m_alphaSpread;    /**< Alpha randomness (0 -> 255). */
            RwReal m_accel,         /**< Continuous acceleration or one of boost value. */
                  m_accelSpread,    /**< Randomness for acceleration or boost. */
                  m_gravity,        /**< Gravity value. */
                  m_drag,           /**< Drag factor value (0 -> 1). */
                  m_size,           /**< Base size value. */
                  m_sizeSpread;     /**< Randomness for size. */
            RwRGBA m_col;           /**< Base color value, used when no randomness. */
         };

         /**
         *
         * \ingroup Particles
         *
         * Used to access the particle structures during emit and tick update functions. This is just a structure
         * used to hold various pointers, counters, etc. These are modified by this class' functions and by the
         * tick and emit code.
         *
         * \see CPB_BasePart::Tick
         * \see CPB_BasePart::Emit
         *
         */

         class PartAccess
         {
         public:

            bool *m_pActive;                 /**< Pointer to list of active flags for block being processed. */
            RwUInt32 m_numFree,              /**< Number of free items in the block being processed. */
                  m_index;                   /**< Index of item being manipulated in block. */
            RpPTankLockStruct m_posLock,     /**< Positional access structure for current block. */
                  m_colLock;                 /**< Color access structure for current block. */
            RwV3d *m_pPos;                   /**< Pointer to current position in vertex array for block. */
            RwV2d *m_pSize;                  /**< Pointer to current size in particle data block. */
            RwRGBA *m_pCol;                  /**< Pointer to current color in particle data block. */
            RwUInt32 *m_pLife;               /**< Pointer to current lifetime in lifetimes block. */
            AnimData *m_pAnimBlock,          /**< Pointer to animation array for current block. */
                  *m_pAnim;                  /**< Pointer to current animation in animation array for block. */

            /**
            *
            * Just initialize the internal values.
            *
            */

            PartAccess(void) :
                  m_numFree(0)
            {
            }

            /**
            *
            * Finds the next inactive particle in the current block. Must have been 'Setup' first.
            *
            * \see Setup
            *
            */

            void FindNextFree(void)
            {
               // Find the next inactive item...

               while (*m_pActive)
               {
                  m_pActive++;
               }
            }

            /**
            *
            * Called to setup the data within the structure from that passed into CPB_Marker ready for use when setting
            * up particles during the emit phase.
            *
            * \see CPB_BasePart::EmitSetup
            * \see CycPartBuff
            * \param marker The current marker object being used to access the cyclic particle buffer.
            * \param ppVectBlocks A pointer to the array of vector blocks in the base particle object.
            * \param ppAnimBlocks A pointer to the array of animation blocks in the base particle object.
            *
            */

            void Setup(CPB_Marker marker, AnimData **ppAnimBlocks)
            {
               m_numFree = marker.numInBlock - marker.numActive;
               m_pActive = marker.pActiveFlags;
               RpPTankAtomicLock(marker.pTankAtomic, &m_posLock, rpPTANKLFLAGPOSITION, rpPTANKLOCKWRITE);
               RpPTankAtomicLock(marker.pTankAtomic, &m_colLock, rpPTANKLFLAGCOLOR, rpPTANKLOCKWRITE);
               m_pAnim = m_pAnimBlock = ppAnimBlocks[marker.blockNum];
               m_pPos = reinterpret_cast<RwV3d *>(m_posLock.data);
               m_pSize = reinterpret_cast<RwV2d *>(marker.sizeLock.data);
               m_pCol = reinterpret_cast<RwRGBA *>(m_colLock.data);
               m_pLife = marker.pLifeTimes;
               m_index = 0;
            }

            /**
            *
            * Moves the pointers within the object along one item in the animation, ptank and lifetime blocks being
            * accessed.
            *
            * \see Setup
            * \param marker The current marker object being used to access the cyclic particle buffer.
            *
            */

            void StepOne(CPB_Marker &marker)
            {
               m_pPos = reinterpret_cast<RwV3d *>(reinterpret_cast<RwUInt32>(m_pPos) + m_posLock.stride);
               m_pSize = reinterpret_cast<RwV2d *>(reinterpret_cast<RwUInt32>(m_pSize) + marker.sizeLock.stride);
               m_pCol = reinterpret_cast<RwRGBA *>(reinterpret_cast<RwUInt32>(m_pCol) + m_colLock.stride);
               m_pLife++;
               m_index++;
               m_pAnim++;
            }
         };

         /**
         *
         * \ingroup Particles
         *
         * Holds the axis information obtained from a particle's movement vector. This is used for secondary particle
         * emitting.
         *
         * \see CPB_BasePart::Tick
         * \see CPB_BasePart::SecEmitSetup
         *
         */

         class PartAxis
         {
         public:

            RwV3d m_partXAxis,            /**< Particle direction X axis. */
                  m_partYAxis,            /**< Particle direction Y axis. This is direction of particle movement. */
                  m_partZAxis;            /**< Particle direction Z axis. */
            RwReal m_inputLen,            /**< Size of incoming vector in unit vectors, i.e. particle's velocity. */
                  m_minAng,               /**< Minimum deflection angle for particle. */
                  m_angDiff;              /**< Difference between min & max deflection angle - the range. */

            /**
            *
            * Sets up the axis data from the passed in particle's movement vector and the min/max deflection angles.
            *
            * \param pVect The particle's movement vector (can cope with any vector value, doesn't need normalizing).
            * \param minAng The minimum deflection angle.
            * \param maxAng The maximum deflection angle.
            * \return True if the axis can be calculated and false if not. If the vector length is zero, then the
            * axis cannot be calculated.
            * \see CPB_BasePart::Tick
            *
            */

            bool Setup(RwV3d *pVect, RwReal minAng, RwReal maxAng)
            {
               RwV3d tempVect;

               // Setup the angular values...

               m_minAng = minAng;
               m_angDiff = maxAng - minAng;

               // Check the input length. MAY be zero and RwV3dNormalize cannot cope with that.

               m_inputLen = RwV3dLength(pVect);

               if (m_inputLen > 0.0f)
               {
                  // Calculate the axis required - only want to do this once as it's expensive.

                  RwV3dScale(&m_partYAxis, pVect, 1.0f / m_inputLen);     // Up (direction of movement) vector.

                  // Is this mainly along the world Y axis ?

                  if (m_partYAxis.y >= 0.5f || m_partYAxis.y <= -0.5f)
                  {
                     // Yep, so use right to create 2nd axis.

                     tempVect.x = 1.0f;
                     tempVect.y = 0.0f;
                     tempVect.z = 0.0f;
                  }
                  else
                  {
                     // No, so use up to create 2nd axis.

                     tempVect.x = 0.0f;
                     tempVect.y = 1.0f;
                     tempVect.z = 0.0f;
                  }

                  // Calculate and normalize the 2nd axis...

                  RwV3dCrossProduct(&m_partXAxis, &m_partYAxis, &tempVect);
                  RwV3dNormalize(&m_partXAxis, &m_partXAxis);

                  // Calculate final particle relative axis, will be normalized as 2 inputs are.

                  RwV3dCrossProduct(&m_partZAxis, &m_partYAxis, &m_partXAxis);

                  return(true);
               }
               else
               {
                  return(false);
               }
            }
         };

         RwReal CalcRemainingLifeTime(RwUInt32 curLife);
         RwUInt32 CalcPrimaryLifeTime(CPB_Marker &marker, PartAccess &access);
         void ModifyPartForSecStage(PartAccess &access, PartAxis &partAxis);
         RwReal EmitSetup(CPB_Marker &marker, PartAccess &access);
         void SecEmitSetup(PartAccess &addAccess, PartAccess &access, PartAxis &partAxis, CPB_Marker &addMarker);
         void DoStdPartSetup(const StateData &state, RwRGBA *pCol, RwV2d *pSize,
               AnimData *pAnim, bool isNormVect, bool isInitAccel);
         void DoPartAnimSetup(const StateData &state, const RwRGBA *pCol, const RwV2d *pSize,
               AnimData *pAnim, RwReal animFrames);

         virtual bool CreateNewPtrArray(RwUInt32 newNumBlocks);
         virtual void CopyPtrArrayElement(RwUInt32 index);
         virtual void DeleteOldPtrArrayElement(RwUInt32 index);
         virtual bool CreateNewPtrArrayElement(RwUInt32 index, RwUInt32 blockSize);
         virtual void SwapInNewPtrArray(void);

         AnimData **m_ppAnimBlocks;    /**< Array of pointers to arrays holding animation data of particles */

         StateData m_states[BPS_TOTAL];/**< Array (size BPS_TOTAL) of states of the particles system */
         bool m_secStageOn,            /**< Flag indicating if the secondary stage of system is enabled */
               m_enabled;              /**< Flag indicating if the system as a whole is enabled */
         RwUInt32 m_rate,              /**< Full output rate in particles per frame */
               m_rateSpread,           /**< Randomness added to output rate per frame */     
               m_useRate,              /**< Rate used per frame. Is rate modified by % message, etc... */
               m_useRateSpread,        /**< Rate randomness used per frame. Is modified by % message, etc */
               m_lifeTime,             /**< Maximum lifetime of particle, i.e. base + max randomness */
               m_baseLife,             /**< Base lifetime of particle */
               m_lifeSpread,           /**< Randomness added to lifetime of particle */
               m_secLifeAdj,           /**< Randomness for value added to particle lifetime as enters 2nd stage */
                                       /**< NOTE: This ONLY applies to particles EMITTED in 2nd stage.*/
               m_secLifeTime,          /**< Life time at which 1st to 2nd stage change occurs */
               m_secEmit,              /**< Full number of particles emitted in 2nd stage per primary particle */
               m_secUseEmit,           /**< Used number of particles emitted in 2nd stage. Modified by % message, etc */
               m_secEmitSpread,        /**< Full variance of number particles emitted in 2nd stage */ 
               m_secUseEmitSpread,     /**< Used variance of number particles in 2nd stage. Modified by % message, etc */
               m_numBlocks,            /**< Total number of blocks held in m_ppVectBlocks and m_ppAnimBlocks */
               m_blockSize;            /**< Number of particles in each block m_ppVectBlocks & m_ppAnimBlocks point to */
         RwV3d m_basePos,              /**< The 3D location in the world of the particle at 1st stage creation */
               m_yAxisVect,            /**< Particle vector at creation (without velocity applied). The up vector. */
               m_xAxisVect,            /**< Vector used for local X axis (right) */
               m_zAxisVect;            /**< Vector used for local Z axis (forward or look-at) */
         RwReal m_xAngSpread,          /**< Angular spread at creation (randomly applied in range) along X axis */
               m_zAngSpread,           /**< Angular spread at creation (randomly applied in range) along Y axis */
               m_emitSpreadX,          /**< Base position adjustment on X axis (randomly applied in range) */
               m_emitSpreadY,          /**< Base position adjustment on Y axis (randomly applied in range) */
               m_emitSpreadZ,          /**< Base position adjustment on Z axis (randomly applied in range) */
               m_secEmitMinAng,        /**< Secondary stage, minimum angle offset (randomly applied min to max) */
               m_secEmitMaxAng,        /**< Secondary stage, maximum angle offset */
               m_rateAdjust;           /**< Value from 0 to 1 */

         AnimData **m_ppNewAnimBlocks; /**< Used temporarily during a resize operation. Holds new pointer array. */

         PartAccess m_etAccess,        /**< Access structure used with the ET_... Emit / Tick processing functions. */
               m_etAddAccess;          /**< Access structure used with the ET_... Emit / Tick proc func - use to add. */
         bool *m_etpFlag;              /**< Active flag pointer used with the ET_... Emit/Tick processing functions. */
         PartAxis m_etPartAxis;        /**< Particle generated axis used with ET_... Emit/Tick processing functions. */

         bool m_etActiveFlags[CPB_MAX_PARTICLES_IN_BLOCK];  /**< Copy of flags used during Tick processing. */

#if defined(SKY)
         AnimData * m_pAnimOriginalAddr;
#endif

      public:

         CPB_BasePart(RwUInt32 numBlocks, RwUInt32 blockSize);
         void Resize(RwUInt32 newSize);

         virtual ~CPB_BasePart(void);
         virtual void DoAtomicSetup(RpAtomic *pAtomic, void *pParamData);

         
         /**
         *
         * Sets the FULL particle emitter rate per frame. The value is modified by the rate adjustment.
         *
         * \see SetRateAdj
         * \see SetRateSpread
         * \param rate The new particle rate.
         *
         */

         void SetRate(RwUInt32 rate)
         {
            m_rate = rate;
            m_useRate = static_cast<RwUInt32>(static_cast<RwReal>(m_rate) * m_rateAdjust);

            return;
         }

         /**
         *
         * Sets the rate adjustment value. This is used to 'throttle back' the full output rate to a
         * reduced rate. That is the output rate is the full rate * this adjustment value.
         *
         * \see SetRate
         * \see SetRateSpread
         * \param adjust The adjustment rate in the range of 0 to 1.
         *
         */

         void SetRateAdj(RwReal adjust)
         {
            RWS_ASSERT(adjust >= 0.0f, "adjust cannot be negative");
            RWS_ASSERT(adjust <= 1.0f, "adjust cannot be greater than 1");

            m_rateAdjust = adjust;
            m_useRate = static_cast<RwUInt32>(static_cast<RwReal>(m_rate) * m_rateAdjust);
            m_useRateSpread = static_cast<RwUInt32>(static_cast<RwReal>(m_rateSpread) * m_rateAdjust);
            m_secUseEmit = static_cast<RwUInt32>(static_cast<RwReal>(m_secEmit) * m_rateAdjust);
            m_secUseEmitSpread = static_cast<RwUInt32>(static_cast<RwReal>(m_secEmitSpread) * m_rateAdjust);

            return;
         }

         /**
         *
         * Enables the instance of the particle system as a whole.
         *
         * \see IsEnabled
         * \param flag True to enable, false to disable.
         *
         */

         void Enable(bool flag)
         {
            m_enabled = flag;

            return;
         }

         /**
         *
         * Used to enquire if the instance is enabled or not.
         *
         * \see Enable
         * \return True if enabled, false if not.
         *
         */

         bool IsEnabled(void)
         {
            return(m_enabled);
         }

         /**
         *
         * Adjusts the randomly applied rate adjustment. This is additional to the static rate. The value
         * is modified by the rate adjustment.
         *
         * \see SetRateAdj
         * \see SetRate
         * \param newSpread The new range over which the random value will be calculated
         *
         */

         void SetRateSpread(RwUInt32 newSpread)
         {
            m_rateSpread = newSpread;
            m_useRateSpread = static_cast<RwUInt32>(static_cast<RwReal>(m_rateSpread) * m_rateAdjust);

            return;
         }

         /**
         *
         * Sets the base life-time of the particles in frames.
         *
         * \see SetLifeSpread
         * \param lifeTime The lifetime to use in frames.
         *
         */

         void SetLifeTime(RwUInt32 lifeTime)
         {
            // Value is in FRAMES.

            m_baseLife = lifeTime;
            m_lifeTime = m_baseLife + m_lifeSpread;

            return;
         }

         /**
         *
         * Sets the range of the randomly applied life-time. This is added to the static value.
         *
         * \see SetLifeTime
         * \param newSpread The range of which the additional random life-time will be calculated.
         *
         */

         void SetLifeSpread(RwUInt32 newSpread)
         {
            RWS_ASSERT(newSpread <= RAND_MAX, "Spread larger than random number range");

            m_lifeSpread = newSpread;
            m_lifeTime = m_baseLife + m_lifeSpread;

            return;
         }

         /**
         *
         * Sets the base 3D location used by the particle systems primary stage emitter. The location
         * is used for new particles and does NOT effect ones already emitted.
         *
         * \param newPos The new 3D world space position.
         *
         */

         void SetBase(RwV3d &newPos)
         {

            m_basePos = newPos;

            return;
         }

         /**
         *
         * Sets the up vector for the particle system. This must be of non-zero length. Particles are
         * emitted along this vector. It will be normalized and the velocity is applied to this normalized
         * vector to create the start vector of new particles.
         *
         * \see SetAccel
         * \see SetAccelSpread
         * \see SetEmitYSpread
         * \param newVect A non-zero length vector
         *
         */

         void SetYAxisVect(RwV3d &newVect)
         {
            RWS_ASSERT(RwV3dLength(&newVect) > 0.0f, "vector length CANNOT BE zero");

            // Essentially the 'up' vector.

            RwV3dNormalize(&m_yAxisVect, &newVect);

            return;
         }

         /**
         *
         * Sets the x axis used by the particle systems emitter. This is used to orientate the system.
         * Vector will be normalized, but cannot be zero length.
         *
         * \see SetAngSpreadX
         * \see SetEmitXSpread
         * \param newVect The new 'right' vector.
         *
         */

         void SetXAxisVect(RwV3d &newVect)
         {
            RWS_ASSERT(RwV3dLength(&newVect) > 0.0f, "vector length CANNOT BE zero");

            // Essentially the 'right' vector.

            RwV3dNormalize(&m_xAxisVect, &newVect);

            return;
         }

         /**
         *
         * Sets the z axis used by the particle systems emitter. This is used to orientate the system.
         * Vector will be normalized, but cannot be zero length.
         *
         * \see SetAngSpreadZ
         * \see SetEmitZSpread
         * \param newVect The new 'look-at' vector.
         *
         */

         void SetZAxisVect(RwV3d &newVect)
         {
            RWS_ASSERT(RwV3dLength(&newVect) > 0.0f, "vector length CANNOT BE zero");

            // Essentially the 'forward' vector.

            RwV3dNormalize(&m_zAxisVect, &newVect);

            return;
         }

         /**
         *
         * Sets the angular spread of the emitter along the x axis.
         *
         * \see SetXAxisVect
         * \see SetAngSpreadZ
         * \param newSpread The new angular spread limit, randomly applied -/+ from up vector, in degrees. The
         * value MUST be >= 0
         *
         */

         void SetAngSpreadX(RwReal newSpread)
         {
            RWS_ASSERT(newSpread >= 0.0f, "Spread cannot be negative");

            // Value comes in degrees, so convert to radians.

            m_xAngSpread = RWDEG2RAD(newSpread);

            return;
         }

         /**
         *
         * Sets the angular spread of the emitter along the z axis.
         *
         * \see SetZAxisVect
         * \see SetAngSpreadX
         * \param newSpread The new angular spread limit, randomly applied -/+ from up vector, in degrees. The
         * value MUST be >= 0
         *
         */

         void SetAngSpreadZ(RwReal newSpread)
         {
            RWS_ASSERT(newSpread >= 0.0f, "Spread cannot be negative");

            // Value comes in in degrees, so convert to radians.

            m_zAngSpread = RWDEG2RAD(newSpread);

            return;
         }

         /**
         *
         * Sets the limit of the randomly applied X axis jitter in the system.
         * The value is applied +/- along the axis.
         *
         * \see SetXAxisVect
         * \see SetEmitYSpread
         * \see SetEmitZSpread
         * \param newSpread The new limit value. This MUST be >= 0.
         *
         */

         void SetEmitXSpread(RwReal newSpread)
         {
            RWS_ASSERT(newSpread >= 0.0f, "Spread cannot be negative");

            // Value should be +ve. Is along the set X axis (so can be tilted).

            m_emitSpreadX = newSpread;

            return;
         }

         /**
         *
         * Sets the limit of the randomly applied Y axis jitter in the system.
         * The value is applied +/- along the axis.
         *
         * \see SetYAxisVect
         * \see SetEmitXSpread
         * \see SetEmitZSpread
         * \param newSpread The new limit value. This MUST be >= 0.
         *
         */

         void SetEmitYSpread(RwReal newSpread)
         {
            RWS_ASSERT(newSpread >= 0.0f, "Spread cannot be negative");

            // Value should be +ve. Is along the set Y axis (so can be tilted).

            m_emitSpreadY = newSpread;

            return;
         }

         /**
         *
         * Sets the limit of the randomly applied Z axis jitter in the system.
         * The value is applied +/- along the axis.
         *
         * \see SetZAxisVect
         * \see SetEmitXSpread
         * \see SetEmitYSpread
         * \param newSpread The new limit value. This MUST be >= 0.
         *
         */

         void SetEmitZSpread(RwReal newSpread)
         {
            RWS_ASSERT(newSpread >= 0.0f, "factor cannot be negative");

            // Value should be +ve. Is along the set Y axis (so can be tilted).

            m_emitSpreadZ = newSpread;

            return;
         }

         /**
         *
         * Sets one-time-boost or continuous acceleration of a particle. If the state is
         * a start state, then it is a one-time-boost. If it's an end state, then the
         * value becomes a continuous acceleration value.
         *
         * \see SetAccelSpread
         * \see SetFlag
         * \see ClearFlag
         * \param index The state index (BASE_PART_STATE) value.
         * \param newAccel The new accel value. MUST be >= 0.
         *
         */

         void SetAccel(BASE_PART_STATE index, RwReal newAccel)
         {
            RWS_ASSERT(newAccel >= 0.0f, "acceleration cannot be negative");

            m_states[index].m_accel = newAccel;

            return;
         }

         /**
         *
         * Sets one-time-boost or continuous acceleration randomness of a particle. If the
         * state is a start state, then it is a one-time-boost. If it's an end state,
         * then the value becomes a continuous acceleration value. This is added to the
         * static acceleration value.
         *
         * \see SetAccel
         * \param index The state index (BASE_PART_STATE) value.
         * \param newSpread The new random accel limiting value. MUST be >= 0.
         *
         */

         void SetAccelSpread(BASE_PART_STATE index, RwReal newSpread)
         {
            RWS_ASSERT(newSpread >= 0.0f, "acceleration spread cannot be negative");

            m_states[index].m_accelSpread = newSpread;

            return;
         }

         /**
         *
         * Sets the particle color or animation end color. If the state is a start state,
         * then it is a color set. If it's an end state, then the color animates between
         * last set color / end color and this one.
         *
         * \see SetRedSpread
         * \see SetGreenSpread
         * \see SetBlueSpread
         * \see SetAlphaSpread
         * \see SetFlag
         * \see ClearFlag
         * \param index The state index (BASE_PART_STATE) value.
         * \param newCol The new start / end color.
         *
         */

         void SetColor(BASE_PART_STATE index, RwRGBA newCol)
         {
            m_states[index].m_col = newCol;
            m_states[index].m_red = newCol.red;
            m_states[index].m_green = newCol.green;
            m_states[index].m_blue = newCol.blue;
            m_states[index].m_alpha = newCol.alpha;

            return;
         }

         /**
         *
         * Sets the red channel randomness limit. This works in collaboration with the SetColor
         * function. The variance is randomly applied -ve and +ve to the red set by that function.
         *
         * \see SetColor
         * \see SetGreenSpread
         * \see SetBlueSpread
         * \see SetAlphaSpread
         * \param index The state index (BASE_PART_STATE) value.
         * \param newSpread The limit of the randomness for the color channel (0 -> 255).
         *
         */

         void SetRedSpread(BASE_PART_STATE index, RwUInt32 newSpread)
         {
            RWS_ASSERT(newSpread <= 255, "spread must be in range of 0 to 255");

            m_states[index].m_redSpread = newSpread;

            return;
         }

         /**
         *
         * Sets the green channel randomness limit. This works in collaboration with the SetColor
         * function. The variance is randomly applied -ve and +ve to the green set by that function.
         *
         * \see SetColor
         * \see SetRedSpread
         * \see SetBlueSpread
         * \see SetAlphaSpread
         * \param index The state index (BASE_PART_STATE) value.
         * \param newSpread The limit of the randomness for the color channel (0 -> 255).
         *
         */

         void SetGreenSpread(BASE_PART_STATE index, RwUInt32 newSpread)
         {
            RWS_ASSERT(newSpread <= 255, "spread must be in range of 0 to 255");

            m_states[index].m_greenSpread = newSpread;

            return;
         }

         /**
         *
         * Sets the blue channel randomness limit. This works in collaboration with the SetColor
         * function. The variance is randomly applied -ve and +ve to the blue set by that function.
         *
         * \see SetColor
         * \see SetRedSpread
         * \see SetGreenSpread
         * \see SetAlphaSpread
         * \param index The state index (BASE_PART_STATE) value.
         * \param newSpread The limit of the randomness for the color channel (0 -> 255).
         *
         */

         void SetBlueSpread(BASE_PART_STATE index, RwUInt32 newSpread)
         {
            RWS_ASSERT(newSpread <= 255, "spread must be in range of 0 to 255");

            m_states[index].m_blueSpread = newSpread;

            return;
         }

         /**
         *
         * Sets the alpha channel randomness limit. This works in collaboration with the SetColor
         * function. The variance is randomly applied -ve and +ve to the alpha set by that function.
         *
         * \see SetColor
         * \see SetRedSpread
         * \see SetGreenSpread
         * \see SetBlueSpread
         * \param index The state index (BASE_PART_STATE) value.
         * \param newSpread The limit of the randomness for the color channel (0 -> 255).
         *
         */

         void SetAlphaSpread(BASE_PART_STATE index, RwUInt32 newSpread)
         {
            RWS_ASSERT(newSpread <= 255, "spread must be in range of 0 to 255");

            m_states[index].m_alphaSpread = newSpread;

            return;
         }

         /**
         *
         * Sets the gravity for the state. If this is an end state, then it's the end value for the
         * gravity animation. If it's a start state, then it's flipped to, without animation. The
         * value can be +ve or -ve.
         *
         * \see SetFlag
         * \see ClearFlag
         * \param index The state index (BASE_PART_STATE) value.
         * \param newGrav The new gravity value.
         *
         */

         void SetGravity(BASE_PART_STATE index, RwReal newGrav)
         {
            m_states[index].m_gravity = newGrav;

            return;
         }

         /**
         *
         * Sets the drag for the state. If this is an end state, then it's the end value for the
         * drag animation. If it's a start state, then it's flipped to, without animation. The
         * value is a coefficient.
         *
         * \see SetFlag
         * \see ClearFlag
         * \param index The state index (BASE_PART_STATE) value.
         * \param newDrag The new drag coefficient (0 -> 1)
         *
         */

         void SetDrag(BASE_PART_STATE index, RwReal newDrag)
         {
            RWS_ASSERT(newDrag >= 0.0f, "drag cannot be negative");
            RWS_ASSERT(newDrag <= 1.0f, "drag cannot be greater than one");

            m_states[index].m_drag = 1.0f - newDrag;

            return;
         }

         /**
         *
         * Sets the particle size for the state. If it's a start state, then the value is flipped to.
         * If it's an end state, then it will be animated to this value from the last active start/end
         * state.
         *
         * \see SetFlag
         * \see ClearFlag
         * \see SetSizeSpread
         * \param index The state index (BASE_PART_STATE) value.
         * \param newSize The new size of the particle. MUST be >= 0
         *
         */

         void SetSize(BASE_PART_STATE index, RwReal newSize)
         {
            RWS_ASSERT(newSize >= 0.0f, "size cannot be negative");

            m_states[index].m_size = newSize;

            return;
         }

         /**
         *
         * Sets the particle size randomness limit for the state. Used in connection with the SetSize function.
         * The size random value within the range is added to the value setup in that call.
         *
         * \see SetSize
         * \param index The state index (BASE_PART_STATE) value.
         * \param newSpread The new size randomness limit value. MUST be >= 0.
         *
         */

         void SetSizeSpread(BASE_PART_STATE index, RwReal newSpread)
         {
            RWS_ASSERT(newSpread >= 0.0f, "size spread cannot be negative");

            m_states[index].m_sizeSpread = newSpread;

            return;
         }

         /**
         *
         * Enables features within the start / end states. However, it is not possible to set a flag in the
         * primary start state. This controls what will be animated or set as the particle progresses
         * through the system. The data for each effect must be setup via separate calls.
         *
         * \see ClearFlag
         * \see SetColor
         * \see SetSize
         * \see SetGravity
         * \see SetDrag
         * \see SetAccel
         * \param index The state index (BASE_PART_STATE) value.
         * \param flags The flag(s) to be set (CPB_BP_ANIM_...).
         *
         */

         void SetFlag(BASE_PART_STATE index, RwUInt32 flags)
         {
            RWS_ASSERT(index != BPS_PRIMARY_START, "cannot change primary start state flags");

            // Use the CPB_BP_ANIM_... flags. Can do more than one at a time.

            m_states[index].m_partFlags |= flags;

            return;
         }

         /**
         *
         * Disables features within the start / end states. However, it is not possible to clear a flag in the
         * primary start state. This controls what will be animated or set as the particle progresses
         * through the system.
         *
         * \see SetFlag
         * \param index The state index (BASE_PART_STATE) value.
         * \param flags The flag(s) to be set (CPB_BP_ANIM_...).
         *
         */

         void ClearFlag(BASE_PART_STATE index, RwUInt32 flags)
         {
            RWS_ASSERT(index != BPS_PRIMARY_START, "cannot change primary start state flags");

            // Clear one or more CPB_BP_ANIM_... flags. Can do more than one at a time.

            m_states[index].m_partFlags &= ~flags;

            return;
         }

         /**
         *
         * Enables the secondary state change over. This occurs at a set time in the particles life. To avoid
         * this looking static use the lifetime spread.
         *
         * \see SetLifeSpread
         * \see SetSecLifeTime
         * \param flag True to enable, false to disable.
         *
         */

         void EnableSecState(bool flag)
         {

            m_secStageOn = flag;

            return;
         }

         /**
         *
         * Sets the life-time when the particle enters the second state.
         *
         * \see EnableSecState
         * \param lifeTime The point at which the particle enters the second state, in frames.
         *
         */

         void SetSecLifeTime(RwUInt32 lifeTime)
         {
            RWS_ASSERT(lifeTime > 0, "lifetime for secondary stage MUST be at least one");

            // Value is in FRAMES. Cannot be zero as if it was and it was enabled, the
            // change over would not occur (due to the PRE-increment of lifetime values).
            // Other code also relies on it being at least one.

            m_secLifeTime = lifeTime;

            return;
         }

         /**
         *
         * Sets the adjustment of lifetime made to any particles EMITTED at the second state. They use
         * the parent particles lifetime as a base and randomly add to it within the specified range.
         *
         * \see EnableSecState
         * \see SetSecEmit
         * \param spread The limit of the random lifetime adjustment. MUST be >= 0.
         *
         */

         void SetSecLifeAdj(RwUInt32 spread)
         {
            RWS_ASSERT(spread <= RAND_MAX, "spread cannot be larger than random number range");

            m_secLifeAdj = spread;

            return;
         }

         /**
         *
         * Sets the secondary emitter base rate. This is used to control how many particles are emitted
         * for each particle which enters the secondary state. If the value is zero and the base value is
         * zero, no new particles are created, the input particle will, however, continue.
         *
         * \see SetSecEmitSpread
         * \see EnableSecState
         * \see SetSecLifeTime
         * \see SetSecEmitMinAng
         * \see SetSecEmitMaxAng
         * \param emit The number of particles to be emitted for each input particle.
         *
         */

         void SetSecEmit(RwUInt32 emit)
         {

            // Number of new particles to be started from the position of the particle as
            // it enters the secondary stage. 0 = no new particles, just continue with the
            // single particle entering the stage.

            m_secEmit = emit;
            m_secUseEmit = static_cast<RwUInt32>(static_cast<RwReal>(m_secEmit) * m_rateAdjust);

            return;
         }

         /**
         *
         * Sets the randomly applied second state emitter value's range. This is added onto the base secondary
         * emitter rate. If the value is zero and the base value is zero, no new particles are created, the
         * input particle will, however, continue.
         *
         * \see SetSecEmit
         * \param spread The randomly applied range's limit.
         *
         */

         void SetSecEmitSpread(RwUInt32 spread)
         {

            // Number of new variance particles to be started from the position of the particle as
            // it enters the secondary stage. 0 = no new particles, just continue with the
            // single particle entering the stage.

            RWS_ASSERT(spread <= RAND_MAX, "spread cannot be bigger than the maximum random number value");

            m_secEmitSpread = spread;
            m_secUseEmitSpread = static_cast<RwUInt32>(static_cast<RwReal>(m_secEmitSpread) * m_rateAdjust);

            return;
         }

         /**
         *
         * Sets the minimum deflection angle for particles entering the second state, this includes all input
         * particles and any emitted at the second state. The angle is a deflection way from the direction of
         * travel of the particle. The rotational angle is randomly applied. Use of this or the maximum value
         * (setting to non-zero) requires considerable calculation for input particles. Any emitted particles
         * are basically free. Deflection will occur at angles between this and the maximum value.
         *
         * \see EnableSecState
         * \see SetSecEmit
         * \see SetSecEmitMaxAng
         * \param angle The minimum deflection angle in degrees (0 - 180).
         *
         */

         void SetSecEmitMinAng(RwReal angle)
         {
            RWS_ASSERT(angle >= 0, "angle cannot be negative");
            RWS_ASSERT(angle <= 180, "angle cannot be over 180 degrees");

            m_secEmitMinAng = RWDEG2RAD(angle);

            return;
         }

         /**
         *
         * Sets the maximum deflection angle for particles entering the second state, this includes all input
         * particles and any emitted at the second state. The angle is a deflection way from the direction of
         * travel of the particle. The rotational angle is randomly applied. Use of this or the maximum value
         * (setting to non-zero) requires considerable calculation for input particles. Any emitted particles
         * are basically free. Deflection will occur at angles between this and the minimum value.
         *
         * \see EnableSecState
         * \see SetSecEmit
         * \see SetSecEmitMinAng
         * \param angle The maximum deflection angle in degrees (0 - 180).
         *
         */

         void SetSecEmitMaxAng(RwReal angle)
         {
            RWS_ASSERT(angle >= 0, "angle cannot be negative");
            RWS_ASSERT(angle <= 180, "angle cannot be over 180 degrees");

            m_secEmitMaxAng = RWDEG2RAD(angle);

            return;
         }

         /**
         *
         * Processes the passed in particles movement using the passed in current vector.
         *
         * \see ProcessDrag
         * \see ProcessGravity
         * \see ProcessAccelAnim
         * \see ProcessGravityAnim
         * \see ProcessDragAnim
         * \see ProcessSizeAnim
         * \see ProcessColAnim
         * \param pPos The particles position value
         * \param pVect The particles current movement vector
         *
         */

         void ProcessMovement(RwV3d *pPos, RwV3d *pVect)
         {

            RwV3dAdd(pPos, pPos, pVect);

            return;
         }

         /**
         *
         * Processes the passed in particles movement vector's drag using passed in animation data block.
         *
         * \see ProcessMovement
         * \see ProcessGravity
         * \see ProcessAccelAnim
         * \see ProcessGravityAnim
         * \see ProcessDragAnim
         * \see ProcessSizeAnim
         * \see ProcessColAnim
         * \param pVect The particles current movement vector
         * \param pAnim The particles animation data block
         *
         */

         void ProcessDrag(RwV3d *pVect, AnimData *pAnim)
         {

            RwV3dScale(pVect, pVect, pAnim->m_drag);

            return;
         }

         /**
         *
         * Processes the passed in particles movement vector's gravity using passed in animation data block.
         *
         * \see ProcessMovement
         * \see ProcessDrag
         * \see ProcessAccelAnim
         * \see ProcessGravityAnim
         * \see ProcessDragAnim
         * \see ProcessSizeAnim
         * \see ProcessColAnim
         * \param pVect The particles current movement vector
         * \param pAnim The particles animation data block
         *
         */

         void ProcessGravity(RwV3d *pVect, AnimData *pAnim)
         {

            pVect->y -= pAnim->m_grav;

            return;
         }

         /**
         *
         * Processes the passed in particles movement vector's acceleration using passed in animation data block.
         * And update the animation of the acceleration in the block. Only applied if the acceleration animation
         * flag is set in the animation data block.
         *
         * \see ProcessMovement
         * \see ProcessDrag
         * \see ProcessGravity
         * \see ProcessGravityAnim
         * \see ProcessDragAnim
         * \see ProcessSizeAnim
         * \see ProcessColAnim
         * \param pVect The particles current movement vector
         * \param pAnim The particles animation data block
         *
         */

         void ProcessAccelAnim(RwV3d *pVect, AnimData *pAnim)
         {

            if ((pAnim->m_flags & CPB_BP_ANIM_ACCEL) != 0)
            {
               RwReal len;

               // Want a constant acceleration. So normalize vector first. This may be zero length and
               // RwV3dNormalize will crash if this is the case. So, check first...
               if ((len = RwV3dLength(pVect)) > 0.0f)
               {
                  RwV3d vect;

                  RwV3dScale(&vect, pVect, (1.0f / len) * pAnim->m_accel);
                  RwV3dAdd(pVect, pVect, &vect);
               }
            }

            return;
         }

         /**
         *
         * Animates the gravity value in the passed in animation block. Only applied if the gravity animation
         * flag is set in the animation data block.
         *
         * \see ProcessMovement
         * \see ProcessDrag
         * \see ProcessGravity
         * \see ProcessAccelAnim
         * \see ProcessDragAnim
         * \see ProcessSizeAnim
         * \see ProcessColAnim
         * \param pAnim The particles animation data block
         *
         */

         void ProcessGravityAnim(AnimData *pAnim)
         {

            if ((pAnim->m_flags & CPB_BP_ANIM_GRAVITY) != 0)
            {
               pAnim->m_grav += pAnim->m_gravMod;
            }

            return;
         }

         /**
         *
         * Animates the drag value in the passed in animation block. Only applied if the drag animation
         * flag is set in the animation data block.
         *
         * \see ProcessMovement
         * \see ProcessDrag
         * \see ProcessGravity
         * \see ProcessAccelAnim
         * \see ProcessGravityAnim
         * \see ProcessSizeAnim
         * \see ProcessColAnim
         * \param pAnim The particles animation data block
         *
         */

         void ProcessDragAnim(AnimData *pAnim)
         {

            if ((pAnim->m_flags & CPB_BP_ANIM_DRAG) != 0)
            {
               pAnim->m_drag += pAnim->m_dragMod;
            }

            return;
         }

         /**
         *
         * Animates the size value in the passed in animation block and updates the passed in particles size
         * value. Only applied if the size animation flag is set in the animation data block.
         *
         * \see ProcessMovement
         * \see ProcessDrag
         * \see ProcessGravity
         * \see ProcessAccelAnim
         * \see ProcessGravityAnim
         * \see ProcessDragAnim
         * \see ProcessColAnim
         * \param pSize The particles size
         * \param pAnim The particles animation data block
         *
         */

         void ProcessSizeAnim(RwV2d *pSize, AnimData *pAnim)
         {

            if ((pAnim->m_flags & CPB_BP_ANIM_SIZE) != 0)
            {
               pSize->x += pAnim->m_sizeMod;
               pSize->y += pAnim->m_sizeMod;
            }

            return;
         }

         /**
         *
         * Animates the color value in the passed in animation block and updates the passed in particles color
         * value. Only applied if the color animation flag is set in the animation data block.
         *
         * \see ProcessMovement
         * \see ProcessDrag
         * \see ProcessGravity
         * \see ProcessAccelAnim
         * \see ProcessGravityAnim
         * \see ProcessDragAnim
         * \see ProcessSizeAnim
         * \param pSize The particles size
         * \param pAnim The particles animation data block
         *
         */

         void ProcessColAnim(RwRGBA *pCol, AnimData *pAnim)
         {

            if ((pAnim->m_flags & CPB_BP_ANIM_COL) != 0)
            {
               pAnim->m_r += pAnim->m_rMod;
               pAnim->m_g += pAnim->m_gMod;
               pAnim->m_b += pAnim->m_bMod;
               pAnim->m_a += pAnim->m_aMod;

               pCol->red = ( pAnim->m_r >> 8 );
               pCol->green = ( pAnim->m_g >> 8 );
               pCol->blue = ( pAnim->m_b >> 8 );
               pCol->alpha = ( pAnim->m_a >> 8 );
            }

            return;
         }

         /**
         *
         * Returns the pTank creation flags needed for the particular particle system in use. This needs
         * to be implemented by any particle systems that are derived from this base particle type.
         *
         * \return The flags necessary to add the appropriate data into the RenderWare RpPTank system.
         *
         */

         virtual RwUInt32 GetCreateFlags(void)
         {
            return(rpPTANKDFLAGPOSITION | rpPTANKDFLAGCOLOR);
         }

         /**
         *
         * Used by CycPartBuff during tick and emit functions. Sets up the internal values ready for scanning the
         * block for processing. Must be called before data checking functions, such as ET_IsCurAlive, ET_IncCurLife,
         * etc.
         *
         * \see CycPartBuff
         * \see ET_IsCurActive
         * \see ET_IncCurLife
         * \param marker The marker object being used for scanning the particles in the block.
         *
         */

         void ET_SetupBlock(CPB_Marker &marker)
         {

            // Setup the access structure.

            m_etAccess.Setup(marker, m_ppAnimBlocks);

#if defined(SKY)
            // Copy other per-particle data to the scratchpad
            ET_TransferToSPR( marker );
#else
            // Flag states change during processing, so must ensure only process flags which are set
            // at the time of entry. To ensure this, copy the number and flag values...

            memcpy(m_etActiveFlags, marker.pActiveFlags, sizeof(bool) * marker.numInBlock);
            m_etpFlag = m_etActiveFlags;
#endif
            
            return;
         }

         /**
         *
         * Used by CycPartBuff during tick function. Performs miscellaneous tasks to 
         * clean up a block after it has been processed. Must be called after processing of
         * a block with ET_ProcessCur, etc. that was setup by a call to ET_SetupBlock.
         * etc.
         *
         * \see CycPartBuff
         * \see ET_SetupBlock
         * \param marker The marker object being used for scanning the particles in the block.
         *
         */
         
         void ET_CleanupBlock( CPB_Marker& marker )
         {

#if defined(SKY)
            // Transfer the data on the scratchpad back into main memory.
            ET_TransferFromSPR( marker );
#else
            // Currently no tidying up needed on other platforms.
#endif
            return;
         }

#if defined(SKY)
         /**
         *
         *  Copy onto scratchpad some of the per-particle data arrays. This
         *  helps to cut down on the number of data cache misses during
         *  CycPartBuff::Tick, speeding up the update.
         *
         *  \param marker The marker object for the block being updated.
         *  \see ET_TransferFromSPR
         */

         void ET_TransferToSPR( CPB_Marker &marker )
         {

            // Before transferring any data to the scratchpad, make sure
            //  main memory is in sync with any data currently waiting 
            //  to be written back in the data cache.
            FlushCache(WRITEBACK_DCACHE);

            // Flag states change during processing, so must ensure only process flags which are set
            // at the time of entry. To ensure this, copy the number and flag values...
            RwUInt32 uSPRDest = RWDMA_SCRATCHPAD;
            RwUInt32 uQWords;

            uQWords = ( ( ( sizeof(bool) * marker.numInBlock ) + 15 ) >> 4 );
            m_etpFlag = Sky::CScratchPad::Upload( uSPRDest, marker.pActiveFlags, uQWords );
            uSPRDest = ( uSPRDest + ( uQWords << 4 ));
            
            // Move some of the data used in the update loop onto the scratchpad
            uQWords = ( ( ( marker.numInBlock * sizeof(AnimData) ) + 15 ) >> 4 );
            m_pAnimOriginalAddr = m_etAccess.m_pAnimBlock;
            m_etAccess.m_pAnimBlock = Sky::CScratchPad::Upload( uSPRDest, m_etAccess.m_pAnimBlock, uQWords );
            m_etAccess.m_pAnim = m_etAccess.m_pAnimBlock;

            // Wait until all data has been transferred before continuing.
            Sky::CScratchPad::WaitOnUpload();

            return;
         }
#endif
         

#if defined(SKY)         
         /**
         *
         *  Copy back from the scratchpad data that was updated during the update
         *  of a particle block. Essentially this is the opposite of ET_TransferToSPR.
         *
         *  \param marker The marker object for the block which was updated.
         *  \see ET_TransferToSPR
         *
         */

         void ET_TransferFromSPR( CPB_Marker &marker )
         {

            RwUInt32 uQWords = ( ( ( marker.numInBlock * sizeof(AnimData) ) + 15 ) >> 4 );
            Sky::CScratchPad::Download( m_pAnimOriginalAddr, m_etAccess.m_pAnimBlock, uQWords );
            Sky::CScratchPad::WaitOnDownload();
            m_pAnimOriginalAddr = 0;

            return;
         }
#endif

         /**
         *
         * Used by CycPartBuff classes Tick and Emit functions. Returns the state of the current particle's active
         * flag. Must have called 'ET_SetupBlock' first.
         *
         * \see ET_SetupBlock
         * \see CycPartBuff
         * \return True if current particle is active and false if not.
         *
         */

         bool ET_IsCurActive(void)
         {
            return(*m_etpFlag);
         }

         /**
         *
         * Called by the CycPartBuff class' Tick function. Increments the lifetime of the current particle. The
         * 'ET_SetupBlock' function MUST have been called first.
         *
         * \see ET_SetupBlock
         * \see CycPartBuff
         *
         */

         void ET_IncCurLife(void)
         {

            (*m_etAccess.m_pLife)++;

            return;
         }

         /**
         *
         * Called by the CycPartBuff class' Tick function. Returns the index of the current particle in the block.
         * The ET_SetupBlock MUST have been called.
         *
         * \see CycPartBuff
         * \see ET_SetupBlock
         * \returns The index value.
         *
         */

         RwUInt32 ET_GetCurIndex(void)
         {
            return(m_etAccess.m_index);
         }

         /**
         *
         * Called by the CycPartBuff class' Tick function. Indicates if the current particle has reached the start of
         * a secondary (2nd, 3rd, 4th, ...) stage. MUST have called ET_SetupBlock first.
         *
         * \see ET_SetupBlock
         * \see CycPartBuff
         * \returns True if the current particle is at a secondary stage start or false otherwise.
         *
         */

         bool ET_IsCurAtSecStage(void)
         {
            return(m_secStageOn && *m_etAccess.m_pLife == m_secLifeTime);
         }

         /**
         *
         * Called by CycPartBuff to calculate the number of frames remaining in the particle's lifetime. This is
         * the 'animFrames' value passed into other functions. Value is calculated for the current particle. The
         * ET_SetupBlock function MUST have been called.
         *
         * \see CycPartBuff
         * \see ET_SetupBlock
         * \see ET_ProcCurEnteringSecState
         * \returns The remaining lifetime for the current particle.
         *
         */

         RwReal ET_CalcRemainingLifeTime(void)
         {

            if (*m_etAccess.m_pLife >= m_lifeTime)
            {
               return(1.0f);
            }
            else
            {
               return(static_cast<RwReal>(m_lifeTime - *m_etAccess.m_pLife));
            }
         }

         /**
         *
         * Called by CycPartBuff to provide particle data setup for particles entering a secondary stage.
         * The data setup is on the current particle. ET_SetupBlock MUST have been called.
         *
         * \see CycPartBuff
         * \see ET_SetupBlock
         * \see ET_CalcRemainingLifeTime
         * \param animFrames The number of frames the animation will be over. Calculated via 'ET_CalcRemainingLifeTime'.
         *
         */

         void ET_ProcCurEnteringSecState(RwReal animFrames)
         {

            // Are any particles being deflected ?

            if (m_secEmitMinAng != 0.0f || m_secEmitMaxAng != 0.0f)
            {
               // Calculate the particle based direction of movement based axis (if possible).
               // If possible to calculate, then modify this particle's movement using it...

               if (m_etPartAxis.Setup(&m_etAccess.m_pAnim->m_Vel, m_secEmitMinAng, m_secEmitMaxAng))
               {
                  // Modify the input particle for it's change into the second state.

                  ModifyPartForSecStage(m_etAccess, m_etPartAxis);
               }
            }

            // Do other setups...

            DoStdPartSetup(m_states[BPS_SECONDARY_START], m_etAccess.m_pCol, m_etAccess.m_pSize,
                  m_etAccess.m_pAnim, false, false);
            DoPartAnimSetup(m_states[BPS_SECONDARY_END], m_etAccess.m_pCol, m_etAccess.m_pSize, m_etAccess.m_pAnim,
                  animFrames);

            return;
         }

         /**
         *
         * Called by CycPartBuff class' Tick function. Informs that function if the current particle is attempting to
         * add particles as a result of entering a second stage (or 3rd, 4th, etc).
         *
         * \see CycPartBuff
         * \returns True if the current particle is attempting to add further particles, false otherwise.
         *
         */

         bool ET_IsAddingAtSecStage(void)
         {

            // Going to add ?

            if (m_enabled && (m_secUseEmit || m_secUseEmitSpread))
            {
               // MUST reset the number free value in the adding block. Need as code tick code will check
               // the 'ET_NumFreeInAddBlock' function and must INITIALLY get zero, forcing it to find the
               // first adding block.

               m_etAddAccess.m_numFree = 0;

               return(true);
            }

            return(false);
         }

         /**
         *
         * Used by CycPartBuff class' Tick function. Calculates the secondary emission rate. This may contain random
         * elements (depends on settings) and so should only be called ONCE per add cycle.
         *
         * \see CycPartBuff
         * \returns The number of particles to be emitted.
         *
         */

         RwUInt32 ET_CalcSecEmitRate(void)
         {

            if (m_secUseEmitSpread)
            {
               return(m_secUseEmit + (rand() % (m_secUseEmitSpread + 1)));
            }
            else
            {
               return(m_secUseEmit);
            }
         }

         /**
         *
         * Used by CycPartBuff class' Tick and Emit functions. initializes internal data ready for the adding of
         * particles.
         *
         * \see CycPartBuff
         * \param marker The marker object being used for the particle add.
         *
         */

         void ET_SetupForAdd(CPB_Marker &marker)
         {

            m_etAddAccess.Setup(marker, m_ppAnimBlocks);

#if defined(SKY)
            // If the marker being setup is the one that has partially
            // been moved onto the scratchpad, then patch the necessary
            // pointers
            if ( m_etAddAccess.m_pAnimBlock == m_pAnimOriginalAddr )
            {
               m_etAddAccess.m_pAnimBlock = m_etAccess.m_pAnimBlock;
               m_etAddAccess.m_pAnim = m_etAddAccess.m_pAnimBlock;
            }
#endif

            return;
         }

         /**
         *
         * Used by CycPartBuff class' Tick function. Returns the number of free (ready for particle addition) slots
         * in the current block. Before first use, the 'ET_IsAddingAtSecStage' function MUST have been called, as
         * this initializes internal structures.
         *
         * \see CycPartBuff
         * \see ET_IsAddingAtSecStage
         * \returns The number of slots left in the current add block for particle addition.
         *
         */

         RwUInt32 ET_NumFreeInAddBlock(void)
         {
            return(m_etAddAccess.m_numFree);
         }

         /**
         *
         * Called by the CycPartBuff to insert a new particle at the secondary stage (or 3rd, etc). Does NOT activate
         * the particle, but is expected to setup the index within the marker object so the caller can do so.
         *
         * \see CycPartBuff
         * \see ET_CalcRemainingLifeTime
         * \param marker The marker object used during setup. Must have it's index modified to point to the particle
         * being added (so caller can activate it).
         * \param animFrames The number of frames over which the secondary stage animation will occur. Obtained via a
         * call to ET_CalcRemainingLifeTime.
         *
         */

         void ET_InsertSecStage(CPB_Marker &marker, RwReal animFrames)
         {

            // Find a free slot...

            m_etAddAccess.FindNextFree();

            // Insert at this point...

            m_etAddAccess.m_index = m_etAddAccess.m_pActive - marker.pActiveFlags;
            marker.index = m_etAddAccess.m_index;
            m_etAddAccess.m_numFree--;

            // Setup the object...

            SecEmitSetup(m_etAddAccess, m_etAccess, m_etPartAxis, marker);
            DoStdPartSetup(m_states[BPS_SECONDARY_START], m_etAddAccess.m_pCol,
                  m_etAddAccess.m_pSize, m_etAddAccess.m_pAnim, false, false);
            DoPartAnimSetup(m_states[BPS_SECONDARY_END], m_etAddAccess.m_pCol, m_etAddAccess.m_pSize,
                  m_etAddAccess.m_pAnim, animFrames);

            return;
         }

         /**
         *
         * Called by the CycPartBuff::Tick function. It informs this function if the current particle has reached
         * the end of it's life. The particle is prepared for death using 'ET_PrepCurForKill'.
         *
         * \see CycPartBuff
         * \see ET_PrepCurForKill
         * \returns True if the particle needs to be killed and false otherwise.
         *
         */

         bool ET_NeedToKillCur(void)
         {
            return(*m_etAccess.m_pLife >= m_lifeTime);
         }

         /**
         *
         * Called by CycPartBuff::Tick to prepare a particle for deactivation (as a result to a positive call to
         * ET_NeedToKillCur). Does NOT deactivate the particle itself, the caller must do that, but it does setup
         * the marker objects index for this purpose.
         *
         * \see CycPartBuff
         * \see ET_NeedToKillCur
         * \param marker The marker object to set the index within ready for deactivation.
         *
         */

         void ET_PrepCurForKill(CPB_Marker &marker)
         {

            marker.index = m_etAccess.m_index;
            *m_etpFlag = false;

            return;
         }

         /**
         *
         * Called by CycPartBuff::Tick to process the current particles color, movement, etc. The ET_SetupBlock
         * function MUST have been called.
         *
         * \see CycPartBuff
         * \see ET_SetupBlock
         *
         */

         void ET_ProcessCur(void)
         {

            // Process movement...
            ProcessMovement(m_etAccess.m_pPos, &m_etAccess.m_pAnim->m_Vel);
            ProcessDrag(&m_etAccess.m_pAnim->m_Vel, m_etAccess.m_pAnim);
            ProcessGravity(&m_etAccess.m_pAnim->m_Vel, m_etAccess.m_pAnim);

            // Do animation if needed...
            ProcessAccelAnim(&m_etAccess.m_pAnim->m_Vel, m_etAccess.m_pAnim);
            ProcessGravityAnim(m_etAccess.m_pAnim);
            ProcessDragAnim(m_etAccess.m_pAnim);
            ProcessColAnim(m_etAccess.m_pCol, m_etAccess.m_pAnim);
            ProcessSizeAnim(m_etAccess.m_pSize, m_etAccess.m_pAnim);

            return;
         }

         /**
         *
         * Called by CycPartBuff::Tick to set to the next particle in the current block. Must have called ET_SetupBlock
         * before starting the processing of the first block.
         *
         * \see CycPartBuff
         * \see ET_SetupBlock
         * \param The marker for the current block.
         *
         */

         void ET_StepCur(CPB_Marker &marker)
         {

            m_etpFlag++;
            m_etAccess.StepOne(marker);

            return;
         }

         /**
         *
         * Used for Emit and Tick operations by CycPartBuff class. Indicates if particles are being added in the
         * primary (emitter) stage or not.
         *
         * \see CycPartBuff
         * \return True if emitting, false if not.
         *
         */

         bool ET_IsAddingAtPrimStage(void)
         {

            // Adding ?

            if (m_enabled && (m_useRate || m_useRateSpread))
            {
               // MUST initialize the value within the add access structure, because the code will request the
               // number free after this and the first call must return zero so the CycPartBuff code will find
               // the initial adding block.

               m_etAddAccess.m_numFree = 0;

               return(true);
            }

            return(false);
         }

         /**
         *
         * Used by the CycPartBuff tick and emit functions. Calculates the emit rate for the base emitter.
         * Value depends on the settings within the particle system, i.e. random emit levels, static emit
         * levels, etc.  The value should, therefore, be obtained only ONCE.
         *
         * \return The number of particles to emit.
         * \see CycPartBuff
         *
         */

         RwUInt32 ET_CalcEmitRate(void)
         {

            if (m_useRateSpread)
            {
               return(m_useRate + (rand() % (m_useRateSpread + 1)));
            }
            else
            {
               return(m_useRate);
            }
         }

         /**
         *
         * Called by the CycPartBuff to insert a new particle at the primary stage (emitter). Does NOT activate the
         * particle, but is expected to setup the index within the marker object so the caller can do so.
         *
         * \see CycPartBuff
         * \param marker The marker object used during setup. Must have it's index modified to point to the particle
         * being added (so caller can activate it).
         * \returns The number of animation frames in the primary stage.
         *
         */

         RwReal ET_InsertPrimStage(CPB_Marker &marker)
         {

            // Find a free slot...

            m_etAddAccess.FindNextFree();

            // Insert at this point...

            m_etAddAccess.m_index = m_etAddAccess.m_pActive - marker.pActiveFlags;
            marker.index = m_etAddAccess.m_index;
            m_etAddAccess.m_numFree--;

            // Setup the object...

            return(EmitSetup(marker, m_etAddAccess));
         }
      };

      /**
      *
      * \ingroup Particles
      *
      * Textured particle type for use with cyclic particle buffer. Provides size, color, velocity, gravity,
      * drag, texture UVs and other attributes. Used by the CFXPartSpray behavior.
      *
      * \see FX::CFXPartSpray
      * \see TexAnimData
      * \see TexStateData
      * \see TexAccess
      *
      */

      class CPB_TexPart : public CPB_BasePart
      {
      protected:

         /**
         *
         * \ingroup Particles
         *
         * Animation data for the textured particles.
         *
         * \see TexStateData
         *
         */

         class TexAnimData
         {
         public:

            bool m_countUp;            /**< Flag indicating direction of sub-texture animation. */
            RwReal m_subTexIndex,      /**< The sub-texture index & Particle increment. Converts to integer for index. */
                  m_subTexStep,        /**< The step value, per tick, for the sub-texture index. Uses loop value too. */
                  m_subTexEnd,         /**< End index, used when looping to check for overrun. Inclusive of looping. */
                  m_subTexStart;       /**< Start index, used when looping (when overruns) to reset value to start.
                                            Inclusive of looping (first value). */
         };

         /**
         *
         * \ingroup Particles
         *
         * Status information for textured particles - only contains info textures. Other info held in base class'
         * status structure & member variables.
         *
         * \see TexAnimData
         * \see CPB_BP_ANIM_SUB_TEX
         * \see CPB_BasePart::StateData
         *
         */

         class TexStateData
         {
         public:

            RwUInt32 m_minSubTex,      /**< Minimum sub-texture index used for selection in stage. */
                  m_maxSubTex;         /**< Maximum sub-texture index used for selection in stage. */
            RwReal m_subTexLoops,      /**< Number of times sub-texture is looped between start & end.
                                            Only used on end stages. It is possible to part loops (0.5, etc). */
                  m_subTexRndLoops;    /**< Number of randomly applied loops between start & end. Partial
                                            values can be used (0.5, etc). */
         };

         /**
         *
         * \ingroup Particles
         *
         * Holds additional access structures needed for the textured particle system.
         *
         * \see CPB_BasePart::PartAccess
         *
         */

         class TexAccess
         {
         public:

            RpPTankLockStruct m_texUVsLock;  /**< Texture UV access structure for current block. */
            RwV2d *m_pTexUVs;                /**< Current Texture UV pointer for current block. */
            TexAnimData *m_pTexAnim,         /**< Current textured animation data pointer for current block. */
                  *m_pTexAnimBlock;          /**< Pointer to animation array for current block. */

            /**
            *
            * Sets up the additional information needed for textured particles.
            *
            * \see CPB_BasePart::PartAccess::Setup
            * \param marker The marker to use for the setup.
            * \param ppTexAnimBlocks The array of texture animation blocks, from which the correct one will be indexed
            * for the setup (via the block number in the marker).
            *
            */

            void Setup(CPB_Marker marker, TexAnimData **ppTexAnimBlocks)
            {

               RpPTankAtomicLock(marker.pTankAtomic, &m_texUVsLock, rpPTANKLFLAGVTX2TEXCOORDS, rpPTANKLOCKWRITE);
               m_pTexUVs = reinterpret_cast<RwV2d *>(m_texUVsLock.data);
               m_pTexAnim = m_pTexAnimBlock = ppTexAnimBlocks[marker.blockNum];

               return;
            }

            /**
            *
            * Steps the additional pointers needed for the textured particles.
            *
            * \see CPB_BasePart::PartAccess::StepOne
            *
            */

            void StepOne(void)
            {

               m_pTexUVs = reinterpret_cast<RwV2d *>(reinterpret_cast<RwUInt32>(m_pTexUVs) + m_texUVsLock.stride);
               m_pTexAnim++;

               return;
            }
         };

         TexStateData m_texStates[BPS_TOTAL];/**< Array (size BPS_TOTAL) of states of the particles system. */
         TexAnimData **m_ppTexAnimBlocks,    /**< Array of pointers to arrays holding animation data of particles. */
               **m_ppNewTexAnimBlocks;       /**< Temporary array of pointers. Used during resizing operations. */
         RwUInt32 m_numSubTextures,          /**< Number of sub-textures within the currently set texture. */
               m_numSubTexOnRow;             /**< Number of sub-textures on a line in the current texture. */
         RwReal m_uScale,                    /**< The u scaling applied to UV position setting for sub-textures. */
               m_vScale;                     /**< The v scaling applied to UV position setting for sub-textures. */
         RwTexture *m_pCurTex;               /**< A pointer to the currently used texture - used to check for changes. */

         TexAccess m_etTexAccess,            /**< Access structure used during ET_... functions for tick & emit. */
               m_etTexAddAccess;             /**< Access structure used during ET_... functions for tick & emit, adding. */


         bool CreateNewPtrArray(RwUInt32 newNumBlocks);
         void CopyPtrArrayElement(RwUInt32 index);
         void DeleteOldPtrArrayElement(RwUInt32 index);
         bool CreateNewPtrArrayElement(RwUInt32 index, RwUInt32 blockSize);
         void SwapInNewPtrArray(void);
         void SecEmitSetup(TexAccess &addAccess, TexAccess &access, CPB_Marker &addMarker);
         void DoTexPartSetup(StateData &state, TexStateData &texState, RwV2d *pTexUVs, TexAnimData *pTexAnimData);
         void DoTexAnimSetup(StateData &state, TexStateData &texState, TexAnimData *pTexAnimData, RwReal animFrames);

         /**
         *
         * Sets up the UV values for a specific sub-texture (and clips for max range of sub-textures) from the passed
         * in index value.
         *
         * \param pTexUVs A pointer to the two corner texture UVs which need to be set up.
         * \param index The sub-texture index to use. Zero based. Will be clipped to maximum value.
         *
         */

         void SetSubTextureFromIndex(RwV2d *pTexUVs, RwUInt32 index)
         {

            RwUInt32 row,
                  col;

            // Clip...

            if (index >= m_numSubTextures)
            {
               index = m_numSubTextures - 1;
            }

            // Calculate row & column...

            row = index / m_numSubTexOnRow;
            col = index % m_numSubTexOnRow;

            // Set the UVs...

            pTexUVs[0].x = m_uScale * static_cast<RwReal>(col);
            pTexUVs[0].y = m_vScale * static_cast<RwReal>(row);

            pTexUVs[1].x = m_uScale * static_cast<RwReal>(col + 1);
            pTexUVs[1].y = m_vScale * static_cast<RwReal>(row + 1);

            return;
         }

         /**
         *
         * Processes the animation data for the textures within the particles. Only processed if required - that is the
         * CPB_BP_ANIM_SUB_TEX flag is set within the passed in animation structure.
         *
         * \param pTexUVs Pointer to current particle's texture UV values.
         * \param pAnim Pointer to the current particle's base animation data.
         * \param pTexAnim Pointer to the current particle's texture animation data.
         *
         */

         void ProcessTexAnim(RwV2d *pTexUVs, AnimData *pAnim, TexAnimData *pTexAnim)
         {

            // Need to do it ?

            if ((pAnim->m_flags & CPB_BP_ANIM_SUB_TEX) != 0)
            {
               RwUInt32 oldSubIndex = static_cast<RwUInt32>(pTexAnim->m_subTexIndex);

               // Step and check for limits.

               pTexAnim->m_subTexIndex += pTexAnim->m_subTexStep;

               if (pTexAnim->m_countUp && pTexAnim->m_subTexIndex >= (pTexAnim->m_subTexEnd + 1.0f))
               {
                  pTexAnim->m_subTexIndex -= ((pTexAnim->m_subTexEnd - pTexAnim->m_subTexStart) + 1.0f);
               }
               else if (!pTexAnim->m_countUp && pTexAnim->m_subTexIndex <= (pTexAnim->m_subTexEnd - 1.0f))
               {
                  // Need to add 1.999 here to get the number into the upper range of the value, otherwise
                  // the sub-texture slot does not get an even time on-screen.

                  pTexAnim->m_subTexIndex += ((pTexAnim->m_subTexStart - pTexAnim->m_subTexEnd) + 1.999f);
               }

               // Change UVs if needed.

               RwUInt32 newSubIndex = static_cast<RwUInt32>(pTexAnim->m_subTexIndex);

               if (oldSubIndex != newSubIndex)
               {
                  SetSubTextureFromIndex(pTexUVs, newSubIndex);
               }
            }

            return;
         }

      public:

         void SetTexture(RwTexture *pTex, CycPartBuff<CPB_TexPart> &buffer);
         void DoAtomicSetup(RpAtomic *pAtomic, void *pParamData);
         ~CPB_TexPart(void);

         /**
         *
         * Creates data structures & initializes them.
         *
         * \see CPB_TexPart::~CPB_TexPart
         * \param numBlocks Number of particle blocks to use.
         * \param blockSize Number of particles in each block.
         *
         */

         CPB_TexPart(RwUInt32 numBlocks, RwUInt32 blockSize) :
               CPB_BasePart(numBlocks, blockSize),
               m_ppTexAnimBlocks(0),
               m_numSubTextures(1),
               m_numSubTexOnRow(1),
               m_uScale(1.0f),
               m_vScale(1.0f),
               m_pCurTex(0)
         {

            // Initialize the state data.

            memset(m_texStates, 0, sizeof(TexStateData) * BPS_TOTAL);

            // Ensure texture animation is enabled on the primary start state (MUST HAVE ALL ENABLED).

            m_states[BPS_PRIMARY_START].m_partFlags |= CPB_BP_ANIM_SUB_TEX;

            // NOTE: Do NOT change the base flags, want texturing to be OFF by default.

            return;
         }

         /**
         *
         * Sets the minimum sub-texture index used for the stage. Is only used if texturing is enabled. The code
         * will randomly choose between this and the maximum value.
         *
         * \see SetMaxSubTex
         * \see SetSubTexLoops
         * \see SetTexture
         * \see SetNumSubTextures
         * \param index The state to change (see enum).
         * \param subTexIndex The sub-texture index to use.
         *
         */

         void SetMinSubTex(BASE_PART_STATE index, RwUInt32 subTexIndex)
         {

            m_texStates[index].m_minSubTex = subTexIndex;

            return;
         }

         /**
         *
         * Sets the maximum sub-texture index used for the stage. Is only used if texturing is enabled. The code
         * will randomly choose between this and the maximum value (both inclusive).
         *
         * \see SetMinSubTex
         * \see SetSubTexLoops
         * \see SetTexture
         * \see SetNumSubTextures
         * \param index The state to change (see enum).
         * \param subTexIndex The sub-texture index to use.
         *
         */

         void SetMaxSubTex(BASE_PART_STATE index, RwUInt32 subTexIndex)
         {

            m_texStates[index].m_maxSubTex = subTexIndex;

            return;
         }

         /**
         *
         * Sets the number of times the texture is animated between the start and end indices during the stage.
         * This can be 1, 2, 3 or 2.5, 3.25, etc. This is the number of LOOPS. The number of runs through is
         * this loop value plus 1.
         *
         * \see SetMinSubTex
         * \see SetMaxSubTex
         * \see SetTexture
         * \see SetNumSubTextures
         * \see SetRndSubTexLoops
         * \param index The state to change (see enum).
         * \param subTexLoops The number of additional times to go through the texture index animation.
         *
         */

         void SetSubTexLoops(BASE_PART_STATE index, RwReal subTexLoops)
         {
            RWS_ASSERT(subTexLoops >= 0.0f, "Number of loops must be zero or greater");

            m_texStates[index].m_subTexLoops = subTexLoops;

            return;
         }

         /**
         *
         * Sets the random number of times the texture is animated between the start and end indices during the stage.
         * This can be 1, 2, 3 or 2.5, 3.25, etc. This is the number of LOOPS. The number of runs through is
         * this loop value plus 1. Chooses a random value between 0 and this value.
         *
         * \see SetMinSubTex
         * \see SetMaxSubTex
         * \see SetTexture
         * \see SetNumSubTextures
         * \see SetNumRndSubTexLoops
         * \param index The state to change (see enum).
         * \param subTexLoops The number of additional times to go through the texture index animation.
         *
         */

         void SetRndSubTexLoops(BASE_PART_STATE index, RwReal subTexLoops)
         {
            RWS_ASSERT(subTexLoops >= 0.0f, "Number of loops must be zero or greater");

            m_texStates[index].m_subTexRndLoops = subTexLoops;

            return;
         }

         /**
         *
         * Sets the number of sub-textures within the texture. The 'numIn...' values MUST be powers of two. If both
         * numbers are 1, then the texture has NO sub-textures.
         *
         * \see SetTexture
         * \param numInU The number of sub-textures in the U direction. Can be 1, 2, 4, etc. MUST be a power of 2.
         * \param numInV The number of sub-textures in the V direction. Can be 1, 2, 4, etc. MUST be a power of 2.
         *
         */

         void SetNumSubTextures(RwUInt32 numInU, RwUInt32 numInV)
         {
            RWS_ASSERT(IsPowerOf2(numInU), "Number of sub-textures in U is NOT a power of 2.");
            RWS_ASSERT(IsPowerOf2(numInV), "Number of sub-textures in V is NOT a power of 2.");

            m_numSubTextures = numInU * numInV;
            m_numSubTexOnRow = numInU;
            m_uScale = 1.0f / static_cast<RwReal>(numInU);
            m_vScale = 1.0f / static_cast<RwReal>(numInV);

            return;
         }

         /**
         *
         * Returns the creation flags needed for the texture particle system.
         *
         * \see CPB_BasePart::GetCreateFlags
         * \return The bit flags needed for pTank particle system creation.
         *
         */

         RwUInt32 GetCreateFlags(void)
         {
            return(rpPTANKDFLAGPOSITION | rpPTANKDFLAGCOLOR | rpPTANKDFLAGVTX2TEXCOORDS);
         }

         /**
         *
         * Override version of base particle. Does additional setup needed for textured particles.
         *
         * \see CPB_BasePart::ET_SetupBlock
         * \param marker The marker object for the scanning block.
         *
         */

         void ET_SetupBlock(CPB_Marker &marker)
         {

            // Call base to do most of the work...

            CPB_BasePart::ET_SetupBlock(marker);

            // Do extra for texturing...

            m_etTexAccess.Setup(marker, m_ppTexAnimBlocks);

            return;
         }

         /**
         *
         *  Override version of base particle. Does additional work needed for textured particles
         *
         *  \param marker The marker object that has just been updated.
         *  \see CPD_BasePart::ET_CleanupBlock
         *
         */

         void ET_CleanupBlock( CPB_Marker& marker )
         {

            // Clean up the extra data for textured particles first.
            // Note : nothing to do

            // Clean up base particle data
            CPB_BasePart::ET_CleanupBlock( marker );

            return;
         }

         /**
         *
         * Override version of base particle. Does additional setup needed for textured particles.
         *
         * \see CPB_BasePart::ET_ProcCurEnteringSecStage
         * \param animFrames The number of frames to animate over - the remaining lifetime of the particle.
         *
         */

         void ET_ProcCurEnteringSecState(RwReal animFrames)
         {

            // Call base function to do most of the work...

            CPB_BasePart::ET_ProcCurEnteringSecState(animFrames);

            // Do additional stuff for textured particles...

            DoTexPartSetup(this->m_states[BPS_SECONDARY_START], m_texStates[BPS_SECONDARY_START], m_etTexAccess.m_pTexUVs,
                  m_etTexAccess.m_pTexAnim);
            DoTexAnimSetup(m_states[BPS_SECONDARY_END], m_texStates[BPS_SECONDARY_END], m_etTexAccess.m_pTexAnim,
                  animFrames);

            return;
         }

         /**
         *
         * Override version of base particle. Does additional setup needed for textured particles.
         *
         * \see CPB_BasePart::ET_SetupForAdd
         * \param marker The marker object holding the block to add to.
         *
         */

         void ET_SetupForAdd(CPB_Marker &marker)
         {

            // Call base to do most of the work...

            CPB_BasePart::ET_SetupForAdd(marker);

            // Do additional stuff for textured...

            m_etTexAddAccess.Setup(marker, m_ppTexAnimBlocks);

            return;
         }

         /**
         *
         * Override version of base particle. Does additional setup needed for textured particles.
         *
         * \see CPB_BasePart::ET_InsertSecStage
         * \param marker The marker object holding the block to add to.
         * \param animFrames The number of frames to animate over - the remaining lifetime of the particle.
         *
         */

         void ET_InsertSecStage(CPB_Marker &marker, RwReal animFrames)
         {

            // Call base to do most of work...

            CPB_BasePart::ET_InsertSecStage(marker, animFrames);

            // Do extra stuff for textured particles...

            SecEmitSetup(m_etTexAddAccess, m_etTexAccess, marker);
            DoTexPartSetup(m_states[BPS_SECONDARY_START], m_texStates[BPS_SECONDARY_START], m_etTexAddAccess.m_pTexUVs,
                  m_etTexAddAccess.m_pTexAnim);
            DoTexAnimSetup(m_states[BPS_SECONDARY_END], m_texStates[BPS_SECONDARY_END], m_etTexAddAccess.m_pTexAnim,
                  animFrames);

            return;
         }

         /**
         *
         * Override version of base particle. Does additional setup needed for textured particles.
         *
         * \see CPB_BasePart::ET_ProcessCur
         *
         */

         void ET_ProcessCur(void)
         {
            // Call base to do most of work...

            CPB_BasePart::ET_ProcessCur();

            // Do extra stuff for textured particles...

            ProcessTexAnim(m_etTexAccess.m_pTexUVs, m_etAccess.m_pAnim, m_etTexAccess.m_pTexAnim);
         }

         /**
         *
         * Override version of base particle. Does additional setup needed for textured particles.
         *
         * \see CPB_BasePart::ET_StepCur
         * \param marker The marker object for the block in which the step must occur.
         *
         */

         void ET_StepCur(CPB_Marker &marker)
         {
            // Call base to do most of the work...

            CPB_BasePart::ET_StepCur(marker);

            // Do extra for textured...

            m_etTexAccess.StepOne();
         }

         /**
         *
         * Override version of base particle. Does additional setup needed for textured particles. Modifies the state
         * of the texture UV pointer within the internal adding access block for the textured particles - setting it
         * for the current particle.
         *
         * \see CPB_BasePart::ET_InsertPrimStage
         * \param marker The marker object for the block in which the addition is to occur.
         *
         */

         void ET_InsertPrimStage(CPB_Marker &marker)
         {
            RwReal animFrames;

            // Call base function to do most of the work...

            animFrames = CPB_BasePart::ET_InsertPrimStage(marker);

            // Setup pointers...

            m_etTexAddAccess.m_pTexUVs = reinterpret_cast<RwV2d *>(m_etTexAddAccess.m_texUVsLock.data +
                  m_etTexAddAccess.m_texUVsLock.stride * marker.index);

            // Do extra work for textured...

            DoTexPartSetup(m_states[BPS_PRIMARY_START], m_texStates[BPS_PRIMARY_START], m_etTexAddAccess.m_pTexUVs,
                  m_etTexAddAccess.m_pTexAnim + marker.index);
            DoTexAnimSetup(m_states[BPS_PRIMARY_END], m_texStates[BPS_PRIMARY_END],
                  m_etTexAddAccess.m_pTexAnim + marker.index, animFrames);
         }
      };

      /**
      *
      * \ingroup Particles
      *
      * Container template class for the particle system. Provides a cyclic buffering system. Base
      * particle classes are used with this template to form the full particle system.
      *
      * \see CPB_BasePart
      *
      */

      template<class T> class CycPartBuff
      {
      public:

         /**
         *
         * Rendering mode used for the particles. Dual pass solves the inter-particle and inter-particle system
         * rendering problems, but is slower. No z-write solves the inter-particle, but not inter-particle system
         * problems and z-write solves inter-particle system but not inter-particle.
         *
         */

         enum RENDER_MODE
         {
            RM_NO_Z_WRITE,
            RM_Z_WRITE,
            RM_DUAL_PASS
         };

      private:

         /**
         *
         * \ingroup Particles
         *
         * Holds information on a particle block within the CycPartBuff template class. Private sub-structure.
         *
         * \see CycPartBuff
         *
         */

         struct CPB_Node
         {
            RpAtomic *m_pTankAtomic;   /**< Atomic for the pTank object held within the node */
            bool *m_pActiveFlags;      /**< Array of flags indicating if each particle is active or not (true = active) */
            RwUInt32 *m_pLifeTimes;    /**< Array of lifetimes, one for each particle */
            CPB_Node *m_pNext,         /**< Pointer to next CPB_Node in free or used list (as appropriate) */
                  *m_pPrev;            /**< Pointer to previous CPB_Node in free or used list (as appropriate) */
            RwUInt32 m_numHeld,        /**< Number of particles held within the arrays / pTank object */
                  m_numActive,         /**< Number of currently active particles within the node */
                  m_blockNum,          /**< Block number, used for CycPartBuff -> Base Particle system
                                            connection. Fixed at creation time, do NOT change. */
                  m_lockCount;         /**< The number of times this node had been locked by GetFirstFree,
                                            GetFree, GetNextFree, etc... */

            /**
            *
            * Builds a node of the required size, using the required RenderWare RpPTank flags and assign the
            * passed in block number.
            *
            * \param size The number of particles in the block
            * \param flags The RenderWare RpPTank flags for creation of the internal PTank object
            * \param blockNum The block number to assign. This is part of the linkage system for the CycPartBuff
            * to CPB_BasePart classes. Each block number MUST be incremental from 0 and MUST be different for
            * each block.
            *
            */

            CPB_Node(RwUInt32 size, RwUInt32 flags, RwUInt32 blockNum)
            {

               // Create the particle tank...

               m_pTankAtomic = RpPTankAtomicCreate(size, flags | rpPTANKDFLAGSIZE, 0);
               RWS_ASSERT(m_pTankAtomic, "Failed to create ptank atomic");
 
               RwFrame *pFrame = RwFrameCreate();

               RwMatrixSetIdentity(RwFrameGetMatrix(pFrame));
               RpAtomicSetFrame(m_pTankAtomic, pFrame);

               // Setup basic particle information...

               RpPTankAtomicSetActiveParticlesCount(m_pTankAtomic,
                     RpPTankAtomicGetMaximumParticlesCount(m_pTankAtomic));

               // Allocate other arrays.

               m_pActiveFlags = ::RWS_NEW bool[size];
               RWS_ASSERT(m_pActiveFlags, "Failed to allocate active flag array");
               memset(m_pActiveFlags, 0, sizeof(bool) * size);

               m_pLifeTimes = ::RWS_NEW RwUInt32[size];
               RWS_ASSERT(m_pActiveFlags, "Failed to allocate life time array");

               // Initialise all the size values to zero - must be initialized to 'not visible'.

               RpPTankLockStruct sizeLock;
               RpPTankAtomicLock(m_pTankAtomic, &sizeLock, rpPTANKLFLAGSIZE, rpPTANKLOCKWRITE);

               for (RwUInt32 count = 0; count < size; count++)
               {
                  RwV2d *pSize = reinterpret_cast<RwV2d *>(sizeLock.data + sizeLock.stride * count);
                  pSize->x = 0.0f;
                  pSize->y = 0.0f;
               }

               RpPTankAtomicUnlock(m_pTankAtomic);

               // Setup other values.

               m_numHeld = size;
               m_numActive = 0;
               m_blockNum = blockNum;
               m_lockCount = 0;

               return;
            }

            /**
            *
            * Frees up arrays of data and the particle tanks.
            *
            */

            ~CPB_Node(void)
            {

               delete[] m_pActiveFlags;
               delete[] m_pLifeTimes;
               RpPTankAtomicDestroy(m_pTankAtomic);

               return;
            }

            /**
            *
            * Unlinks this node from the list it is in. The root pointer of that list must be passed in
            * and will be modified if this node is at the root.
            *
            * \param pRoot Pointer to the root node of the list.
            *
            */

            void Unlink(CPB_Node *&pRoot)
            {

               if (pRoot == this)
               {
                  pRoot = pRoot->m_pNext;
               }
               if (m_pNext)
               {
                  m_pNext->m_pPrev = m_pPrev;
               }
               if (m_pPrev)
               {
                  m_pPrev->m_pNext = m_pNext;
               }

               return;
            }

            /**
            *
            * Links this node at the head of the passed in list. The root pointer will be modified to
            * reflect this.
            *
            * \param pRoot Pointer to the root node of the list.
            *
            */

            void LinkAtRoot(CPB_Node *&pRoot)
            {

               if (pRoot)
               {
                  pRoot->m_pPrev = this;
               }
               m_pPrev = 0;
               m_pNext = pRoot;
               pRoot = this;

               return;
            }

            /**
            *
            * Locks the object. When locking in code ALWAYS CALL THIS FUNCTION. If locking for multiple
            * elements (i.e. position, size, color, etc) just call on the first of these. This is handled
            * by the 'GetFirstActive', 'GetNextActive', 'GetFree' and 'GetFreeEnd' functions in the
            * CycPartBuff so user should not need to call (hence private status).
            *
            */

            void Lock(void)
            {
               m_lockCount++;

               return;
            }

            /**
            *
            * Unlocks the object. When unlocking in code DO NOT CALL "RpPTankAtomicUnlock" directly,
            * ALWAYS CALL THIS FUNCTION instead. 'GetFirstActive', 'GetNextActive', 'GetFree' and
            * 'GetFreeEnd' all use this function, so should be no need for use to call (hence the
            * private status).
            *
            */

            void Unlock(void)
            {

               if (--m_lockCount == 0)
               {
                  // Unlock the particle tank too.

                  RpPTankAtomicUnlock(m_pTankAtomic);
               }

               return;
            }
         };

         CPB_Node *m_pFreeRoot,        /**< Pointer to the head of the non-processed, free, particle blocks. */
               *m_pUsedRoot;           /**< Pointer to the head of the processed & displayed particle blocks. */
         T *m_pBase;                   /**< Pointer to the base particle processing object, called to do work. */
         RwUInt32 m_maxParticles;      /**< The maximum number of particles the buffer can hold. */
         RENDER_MODE m_renderMode;     /**< The currently selected rendering mode. */
         RwBlendFunction m_srcBlend,   /**< The currently selected source blending mode. */
               m_destBlend;            /**< The currently selected destination blending mode. */

      public:

         /**
         *
         * Setup buffer object, node lists, contained class' objects, etc... pParamData is specific
         * to the base class and is passed to it's constructor during setup. The maxParticles can be
         * set to zero and the system will still construct, but won't do anything. This can be used, in      
         * conjunction with the 'Resize' function to remove memory fragmentation problems to do with
         * 'size to default and then resize' game release cases.
         *
         * \see CPB_BasePart
         * \see CPB_BasePart::DoAtomicSetup
         * \see Resize
         * \param maxParticles The maximum number of particles to be displayted / processed in this instance.
         * \param pParamData Data specific to the contained base particle class, see it's "DoAtomicSetup"
         * function.
         *
         */

         CycPartBuff(RwUInt32 maxParticles, void *pParamData) :
               m_renderMode(RM_NO_Z_WRITE),
               m_srcBlend(rwBLENDSRCALPHA),
               m_destBlend(rwBLENDINVSRCALPHA)
         {

            RwUInt32 numBlocks;

            m_pFreeRoot = 0;
            m_pUsedRoot = 0;

            // Build base object.

            numBlocks = maxParticles / CPB_MAX_PARTICLES_IN_BLOCK;
            if (maxParticles % CPB_MAX_PARTICLES_IN_BLOCK)
            {
               numBlocks++;
            }

            m_pBase = ::RWS_NEW T(numBlocks, CPB_MAX_PARTICLES_IN_BLOCK);
            RWS_ASSERT(m_pBase, "Failed to get base class object");

            // Build 'n' link objects and place them onto the 'free' list.

            m_maxParticles = 0;

            Resize(maxParticles, pParamData);

            return;
         }

         // Destructor - free it all up.

         ~CycPartBuff(void)
         {

            CPB_Node *pDel;

            delete m_pBase;

            while (m_pFreeRoot)
            {
               pDel = m_pFreeRoot;
               m_pFreeRoot = m_pFreeRoot->m_pNext;
               delete pDel;
            }

            while (m_pUsedRoot)
            {
               pDel = m_pUsedRoot;
               m_pUsedRoot = m_pUsedRoot->m_pNext;
               delete pDel;
            }

            return;
         }

         /**
         *
         * Resizes the internal blocks of the particle systems. To avoid memory fragmentation caused
         * by 'default size' and then 'resize', set the size in the constructor (default size) to
         * ZERO and then use this to set to the required size. This way only one allocation occurs,
         * UNLESS editing, i.e. release has only one allocation. The 'paramData' is the same data as
         * is passed into the constructor, it's used by the base particle type (T) to setup each
         * particle tank. NOTE: When this function is called the processing of particles will be
         * interrupted and so the display of the particle system will become temporarily corrupted.
         *
         * \param newSize The new maximum number of particles to process / display
         * \param pParamData The base particle class specific data, same as sent to constructor.
         *
         */

         void Resize(RwUInt32 newSize, void *pParamData)
         {

            // More or less than before (or the same) ?

            if (newSize != m_maxParticles)
            {
               if (newSize < m_maxParticles)
               {
                  RwUInt32 numNodeKills = 0,
                        numPartKills,
                        binRemainder,
                        firstKillBinIdent;
                  CPB_Node *pNode;

                  // Smaller, so reduce number of bins / resize them. Must adjust / remove highest numbered
                  // bins first. How many bins / particles to be removed ?

                  numPartKills = m_maxParticles - newSize;

                  // Remove remainder (non-full bin) if one.

                  binRemainder = m_maxParticles % CPB_MAX_PARTICLES_IN_BLOCK;
                  if (numPartKills > binRemainder && binRemainder != 0)
                  {
                     numNodeKills++;
                     numPartKills -= binRemainder;
                  }

                  // How many additional bins...

                  numNodeKills += numPartKills / CPB_MAX_PARTICLES_IN_BLOCK;
                  numPartKills %= CPB_MAX_PARTICLES_IN_BLOCK;

                  // Which bin number has to be killed first ?

                  firstKillBinIdent = m_maxParticles / CPB_MAX_PARTICLES_IN_BLOCK;
                  if (m_maxParticles % CPB_MAX_PARTICLES_IN_BLOCK)
                  {
                     firstKillBinIdent++;
                  }
                  firstKillBinIdent -= numNodeKills;

                  // Kill any required bins - MUST remove from highest number down.

                  if (numNodeKills)
                  {
                     // Remove all notes with the required index or higher.

                     pNode = m_pUsedRoot;
                     while (pNode)
                     {
                        if (pNode->m_blockNum >= firstKillBinIdent)
                        {
                           CPB_Node *pDel = pNode;

                           pNode = pNode->m_pNext;
                           pDel->Unlink(m_pUsedRoot);
                           delete pDel;
                        }
                        else
                        {
                           pNode = pNode->m_pNext;
                        }
                     }

                     pNode = m_pFreeRoot;
                     while (pNode)
                     {
                        if (pNode->m_blockNum >= firstKillBinIdent)
                        {
                           CPB_Node *pDel = pNode;

                           pNode = pNode->m_pNext;
                           pDel->Unlink(m_pFreeRoot);
                           delete pDel;
                        }
                        else
                        {
                           pNode = pNode->m_pNext;
                        }
                     }
                  }

                  // Kill individual particles out of the highest numbered bin.

                  if (numPartKills)
                  {
                     CPB_Node *pNewNode;

                     // Find last block left in list...

                     pNode = m_pUsedRoot;
                     while (pNode)
                     {
                        if (pNode->m_blockNum == (firstKillBinIdent - 1))
                        {
                           pNode->Unlink(m_pUsedRoot);
                           break;
                        }

                        pNode = pNode->m_pNext;
                     }

                     if (!pNode)
                     {
                        pNode = m_pFreeRoot;
                        while (pNode)
                        {
                           if (pNode->m_blockNum == (firstKillBinIdent - 1))
                           {
                              pNode->Unlink(m_pFreeRoot);
                              break;
                           }

                           pNode = pNode->m_pNext;
                        }
                     }

                     RWS_ASSERT(pNode, "could not find last element in particle buffer to change size of");

                     // Remove the current node and add a new one with the same block ident (number) but
                     // with the NEW required number of particles. Cannot transfer info from old particle,
                     // so new one will be 'empty'. Particle already unlinked above.

                     pNewNode = ::RWS_NEW CPB_Node(pNode->m_numHeld - numPartKills, m_pBase->GetCreateFlags(),
                           pNode->m_blockNum);
                     RWS_ASSERT(pNewNode, "Cannot change number of particles in bin");

                     delete pNode;
                     pNewNode->LinkAtRoot(m_pFreeRoot);
                     m_pBase->DoAtomicSetup(pNewNode->m_pTankAtomic, pParamData);
                  }
               }
               else
               {
                  RwUInt32 lastBinSpace,
                        numExtra,
                        lastBinIdent;

                  // Must be larger (since not same size), so increase number of bins / resize them.
                  // Is the last bin at full size ? If it's completely empty it means that is doesn't
                  // exist!

                  lastBinSpace = CPB_MAX_PARTICLES_IN_BLOCK - (m_maxParticles % CPB_MAX_PARTICLES_IN_BLOCK);
                  numExtra = newSize - m_maxParticles;
                  lastBinIdent = m_maxParticles / CPB_MAX_PARTICLES_IN_BLOCK;

                  if (lastBinSpace && lastBinSpace != CPB_MAX_PARTICLES_IN_BLOCK)
                  {
                     CPB_Node *pNode,
                           *pNewNode;
                     RwUInt32 reqdSize;

                     // Increase the size of the last bin, find it first...

                     pNode = m_pFreeRoot;
                     while (pNode)
                     {
                        if (pNode->m_blockNum == lastBinIdent)
                        {
                           pNode->Unlink(m_pFreeRoot);
                           break;
                        }
                        pNode = pNode->m_pNext;
                     }

                     if (!pNode)
                     {
                        pNode = m_pUsedRoot;
                        while (pNode)
                        {
                           if (pNode->m_blockNum == lastBinIdent)
                           {
                              pNode->Unlink(m_pUsedRoot);
                              break;
                           }
                           pNode = pNode->m_pNext;
                        }
                     }

                     RWS_ASSERT(pNode, "could not find last node in list to update it");

                     // Remove this node and add a new one of the correct size (already unlinked).

                     delete pNode;

                     if (numExtra <= lastBinSpace)
                     {
                        reqdSize = CPB_MAX_PARTICLES_IN_BLOCK - (lastBinSpace - numExtra);
                        numExtra = 0;
                     }
                     else
                     {
                        reqdSize = CPB_MAX_PARTICLES_IN_BLOCK;
                        numExtra -= lastBinSpace;
                     }
                     pNewNode = ::RWS_NEW CPB_Node(reqdSize, m_pBase->GetCreateFlags(), lastBinIdent);
                     RWS_ASSERT(pNewNode, "Failed to resize last block");
                     pNewNode->LinkAtRoot(m_pFreeRoot);
                     m_pBase->DoAtomicSetup(pNewNode->m_pTankAtomic, pParamData);

                     lastBinIdent++;
                  }

                  // Now do all additional blocks needed...

                  while (numExtra)
                  {
                     CPB_Node *pNode;

                     if (numExtra > CPB_MAX_PARTICLES_IN_BLOCK)
                     {
                        pNode = ::RWS_NEW CPB_Node(CPB_MAX_PARTICLES_IN_BLOCK, m_pBase->GetCreateFlags(), lastBinIdent);
                        RWS_ASSERT(pNode, "Failed to get block");
                        numExtra -= CPB_MAX_PARTICLES_IN_BLOCK;
                     }
                     else
                     {
                        pNode = ::RWS_NEW CPB_Node(numExtra, m_pBase->GetCreateFlags(), lastBinIdent);
                        RWS_ASSERT(pNode, "Failed to get final block");
                        numExtra = 0;
                     }

                     // Setup and link into free list.

                     m_pBase->DoAtomicSetup(pNode->m_pTankAtomic, pParamData);
                     pNode->LinkAtRoot(m_pFreeRoot);

                     lastBinIdent++;
                  }
               }

               // Change size of base object and store new number.

               m_pBase->Resize(newSize);
               m_maxParticles = newSize;
            }

            return;
         }

         /**
         *
         * Gets a pointer to the contained base particle class. Do NOT delete the returned pointer.
         *
         * \return Pointer to the contained base particle system instance. Do NOT delete.
         *
         */

         T *GetBase(void)
         {
            return(m_pBase);
         }

         /**
         *
         * Call to update the particle system, such as on each frame. Calls the base particle system's
         * functions to do the actual processing. This code just provides the processing loop. The particle
         * system's functions should be inlined to provide required speed.
         *
         */

         void Tick(void)
         {

            CPB_Marker marker;
            bool found;

            // Do all active blocks...

            found = GetFirstActive(marker);
            while (found)
            {
               RwUInt32 numProcessed = 0,
                     numActive;


               // Setup block pointers, etc...

               m_pBase->ET_SetupBlock(marker);
               numActive = marker.numActive;

               // Handle each active particle in this list...

               while (numProcessed < numActive && m_pBase->ET_GetCurIndex() < marker.numInBlock)
               {
                  // Is the current particle alive ?

                  if (m_pBase->ET_IsCurActive())
                  {
                     // Update lifetime...

                     m_pBase->ET_IncCurLife();

                     // If AT secondary life time (and it's enabled). ONLY do once, so do
                     // when ON change over frame.

                     if (m_pBase->ET_IsCurAtSecStage())
                     {
                        RwReal animFrames = m_pBase->ET_CalcRemainingLifeTime();

                        // Calculate remaining life-time, in frames.

                        m_pBase->ET_ProcCurEnteringSecState(animFrames);

                        // Any particles being added ?

                        if (m_pBase->ET_IsAddingAtSecStage())
                        {
                           bool addDidAddFlag = false;
                           CPB_Marker addMarker;
                           RwUInt32 addNumSecEmit;

                           addNumSecEmit = m_pBase->ET_CalcSecEmitRate();

                           for (RwUInt32 addCount = 0; addCount < addNumSecEmit; addCount++)
                           {
                              // Need a new block ?

                              if (!m_pBase->ET_NumFreeInAddBlock())
                              {
                                 if (addDidAddFlag)
                                 {
                                    GetFreeEnd(addMarker);
                                 }

                                 if ((addDidAddFlag = GetFree(addMarker)) == true)
                                 {
                                    m_pBase->ET_SetupForAdd(addMarker);
                                 }
                                 else
                                 {
                                    break;
                                 }
                              }

                              // Find free item in the list...

                              m_pBase->ET_InsertSecStage(addMarker, animFrames);
                              Activate(addMarker);
                           }

                           if (addDidAddFlag)
                           {
                              GetFreeEnd(addMarker);
                           }
                        }
                     }

                     // Is particle dead ?

                     if (m_pBase->ET_NeedToKillCur())
                     {
                        m_pBase->ET_PrepCurForKill(marker);
                        Deactivate(marker);
                        numActive--;
                     }
                  }

                  // If still active, update movement...

                  if (m_pBase->ET_IsCurActive())
                  {
                     // Do standard processing...

                     m_pBase->ET_ProcessCur();
                     numProcessed++;
                  }

                  m_pBase->ET_StepCur(marker);
               }

               // May need to clean up certain data after processing the block.

               m_pBase->ET_CleanupBlock(marker);

               // Do next block...

               found = GetNextActive(marker);
            }

            return;
         }

         /**
         *
         * Call to emit new particles, such as on each frame. Calls the base particle systems functions to
         * do the actual processing. This code provides only the basic loop. Particle systems functions should
         * be inlined to provide required performance.
         *
         */

         void Emit(void)
         {

            CPB_Marker marker;

            // Start new particles, as many as possible up to rate limit.

            if (m_pBase->ET_IsAddingAtPrimStage())
            {
               bool didAddFlag = false;
               RwUInt32 curRate;

               // Add particles...

               curRate = m_pBase->ET_CalcEmitRate();                 // Calculate ONCE, as may return random values.

               for (RwUInt32 count = 0; count < curRate; count++)
               {
                  // Need a new block ?

                  if (!m_pBase->ET_NumFreeInAddBlock())
                  {
                     if (didAddFlag)
                     {
                        GetFreeEnd(marker);
                     }

                     if ((didAddFlag = GetFree(marker)) == true)
                     {
                        m_pBase->ET_SetupForAdd(marker);
                     }
                     else
                     {
                        break;
                     }
                  }

                  // Setup the particle for primary stage emission - expects function to setup the index
                  // value within the marker so it can then be activated.

                  m_pBase->ET_InsertPrimStage(marker);
                  Activate(marker);
               }

               // Unlock the last used atomic.

               if (didAddFlag)
               {
                  GetFreeEnd(marker);
               }
            }

            return;
         }

         /**
         *
         * Returns the first block with active items in it. Sets up the marker object
         * with blocks numbers, active particle flag pointer, etc. The index value will
         * be undefined. When deactivating particles use the 'Deactivate' function ONLY.
         *
         * NOTE: This and the 'GetNextActive' functions lock and unlock the data streams in
         * the particle tank atomic for size (the data is returned in the marker structure)
         * so do NOT call the "RpPTankAtomicUnlock" function on the "tankAtomic" element.
         *
         * \see GetNextActive
         * \see GetNext
         * \see GetFirst
         * \see Deactivate
         * \param marker The marker object to be filled with access data.
         * \return True if an active block was found, false if not.
         *
         */

         bool GetFirstActive(CPB_Marker &marker)
         {

            if (m_pUsedRoot)
            {
               // Lock the 'size' data.

               m_pUsedRoot->Lock();
               RpPTankAtomicLock(m_pUsedRoot->m_pTankAtomic, &marker.sizeLock, rpPTANKLFLAGSIZE, rpPTANKLOCKWRITE);

               // Setup the data...

               marker.blockNum = m_pUsedRoot->m_blockNum;
               marker.internalA = reinterpret_cast<RwUInt32>(m_pUsedRoot);
               marker.internalB = reinterpret_cast<RwUInt32>(m_pUsedRoot->m_pNext);
               marker.numActive = m_pUsedRoot->m_numActive;
               marker.numInBlock = m_pUsedRoot->m_numHeld;
               marker.pActiveFlags = m_pUsedRoot->m_pActiveFlags;
               marker.pLifeTimes = m_pUsedRoot->m_pLifeTimes;
               marker.pTankAtomic = m_pUsedRoot->m_pTankAtomic;
               marker.inUsedList = true;

               RWS_ASSERT(CheckActive(marker), "invalid active particles");
               return(true);
            }
            else
            {
               return(false);
            }
         }

         /**
         *
         * Returns the first block of items in it - regardless of if it has any active items.
         * Sets up the marker object with blocks numbers, active particle flag pointer, etc.
         * The index value will be undefined. When deactivating particles use the 'Deactivate'
         * function ONLY.
         *
         * NOTE: This and the 'GetNext' functions lock and unlock the data streams in
         * the particle tank atomic for size (the data is returned in the marker structure)
         * so do NOT call the "RpPTankAtomicUnlock" function on the "tankAtomic" element.
         *
         * \see GetNext
         * \see GetNextActive
         * \see GetFirstActive
         * \see Deactivate
         * \param marker The marker object to be filled with access data.
         * \return True if a block was found, false if not.
         *
         */

         bool GetFirst(CPB_Marker &marker)
         {
            if (m_pUsedRoot)
            {
               // Lock the 'size' data.

               m_pUsedRoot->Lock();
               RpPTankAtomicLock(m_pUsedRoot->m_pTankAtomic, &marker.sizeLock, rpPTANKLFLAGSIZE, rpPTANKLOCKWRITE);

               // Setup the data...

               marker.blockNum = m_pUsedRoot->m_blockNum;
               marker.internalA = reinterpret_cast<RwUInt32>(m_pUsedRoot);
               marker.internalB = reinterpret_cast<RwUInt32>(m_pUsedRoot->m_pNext);
               marker.numActive = m_pUsedRoot->m_numActive;
               marker.numInBlock = m_pUsedRoot->m_numHeld;
               marker.pActiveFlags = m_pUsedRoot->m_pActiveFlags;
               marker.pLifeTimes = m_pUsedRoot->m_pLifeTimes;
               marker.pTankAtomic = m_pUsedRoot->m_pTankAtomic;
               marker.inUsedList = true;

               RWS_ASSERT(CheckActive(marker), "invalid active particles");
               return(true);
            }
            else if (m_pFreeRoot)
            {
               // Use the 'free list' node instead... Lock the 'size' data.

               m_pFreeRoot->Lock();
               RpPTankAtomicLock(m_pFreeRoot->m_pTankAtomic, &marker.sizeLock, rpPTANKLFLAGSIZE, rpPTANKLOCKWRITE);

               // Setup the data...

               marker.blockNum = m_pFreeRoot->m_blockNum;
               marker.internalA = reinterpret_cast<RwUInt32>(m_pFreeRoot);
               marker.internalB = reinterpret_cast<RwUInt32>(m_pFreeRoot->m_pNext);
               marker.numActive = m_pFreeRoot->m_numActive;
               marker.numInBlock = m_pFreeRoot->m_numHeld;
               marker.pActiveFlags = m_pFreeRoot->m_pActiveFlags;
               marker.pLifeTimes = m_pFreeRoot->m_pLifeTimes;
               marker.pTankAtomic = m_pFreeRoot->m_pTankAtomic;
               marker.inUsedList = false;

               RWS_ASSERT(CheckActive(marker), "invalid active particles");
               return(true);
            }
            else
            {
               return(false);
            }
         }

         /**
         *
         * Returns the next active value. Called repeatedly, but must be started with a
         * call to 'GetFirstActive'. Pass in same 'marker' object each time. For the
         * system to work correctly this function MUST be repeatedly called until it
         * returns a value of false. Function returns this when no more block with
         * active particles exist. When deactivating particles use the 'Deactivate' function ONLY.
         *
         * NOTE: This and the 'GetFirstActive' functions lock and unlock the data streams in
         * the particle tank atomic for size (the data is returned in this structure)
         * so do NOT call the "RpPTankAtomicUnlock" function on the "tankAtomic" element.
         *
         * \see GetFirstActive
         * \see GetFirst
         * \see GetNext
         * \see Deactivate
         * \param marker The marker object passed into GetFirstActive or previous calls to this function.
         * \return True if an active block was found, false if not.
         *
         */

         bool GetNextActive(CPB_Marker &marker)
         {
            CPB_Node *pCur = reinterpret_cast<CPB_Node *>(marker.internalA),
                  *pNext = reinterpret_cast<CPB_Node *>(marker.internalB);

            // UNlock previous atomic's data.

            pCur->Unlock();

            // Is there another one ?

            if (pNext)
            {
               // Move to next and unlock the size data.

               pCur = pNext;
               pNext = pNext->m_pNext;
               pCur->Lock();
               RpPTankAtomicLock(pCur->m_pTankAtomic, &marker.sizeLock, rpPTANKLFLAGSIZE, rpPTANKLOCKWRITE);

               // Setup the rest of the structure...

               marker.blockNum = pCur->m_blockNum;
               marker.internalA = reinterpret_cast<RwUInt32>(pCur);
               marker.internalB = reinterpret_cast<RwUInt32>(pNext);
               marker.numActive = pCur->m_numActive;
               marker.numInBlock = pCur->m_numHeld;
               marker.pActiveFlags = pCur->m_pActiveFlags;
               marker.pLifeTimes = pCur->m_pLifeTimes;
               marker.pTankAtomic = pCur->m_pTankAtomic;

               RWS_ASSERT(CheckActive(marker), "invalid active particles");
               return(true);
            }
            else
            {
               return(false);
            }
         }

         /**
         *
         * Returns the next block - regardless of if it is active or not. Called repeatedly,
         * but must be started with a call to 'GetFirst'. Pass in same 'marker' object each
         * time. For the system to work correctly this function MUST be repeatedly called until it
         * returns a value of false. Function returns this when no more block with
         * active particles exist. When deactivating particles use the 'Deactivate' function ONLY.
         *
         * NOTE: This and the 'GetFirst' functions lock and unlock the data streams in
         * the particle tank atomic for size (the data is returned in this structure)
         * so do NOT call the "RpPTankAtomicUnlock" function on the "tankAtomic" element.
         *
         * \see GetFirstActive
         * \set GetNextActive
         * \see GetFirst
         * \see Deactivate
         * \param marker The marker object passed into GetFirst or previous calls to this function.
         * \return True if a block was found, false if not.
         *
         */

         bool GetNext(CPB_Marker &marker)
         {
            CPB_Node *pCur = reinterpret_cast<CPB_Node *>(marker.internalA),
                  *pNext = reinterpret_cast<CPB_Node *>(marker.internalB);

            // UNlock previous atomic's data.

            pCur->Unlock();

            // Check to see if the lists need to be swapped.

            if (!pNext && marker.inUsedList)
            {
               // Finished doing used list, so move next to first node in the free list.

               pNext = this->m_pFreeRoot;
               marker.inUsedList = false;
            }

            // Is there another one ?

            if (pNext)
            {
               // Move to next and unlock the size data.

               pCur = pNext;
               pNext = pNext->m_pNext;
               pCur->Lock();
               RpPTankAtomicLock(pCur->m_pTankAtomic, &marker.sizeLock, rpPTANKLFLAGSIZE, rpPTANKLOCKWRITE);

               // Setup the rest of the structure...

               marker.blockNum = pCur->m_blockNum;
               marker.internalA = reinterpret_cast<RwUInt32>(pCur);
               marker.internalB = reinterpret_cast<RwUInt32>(pNext);
               marker.numActive = pCur->m_numActive;
               marker.numInBlock = pCur->m_numHeld;
               marker.pActiveFlags = pCur->m_pActiveFlags;
               marker.pLifeTimes = pCur->m_pLifeTimes;
               marker.pTankAtomic = pCur->m_pTankAtomic;

               RWS_ASSERT(CheckActive(marker), "invalid active particles");
               return(true);
            }
            else
            {
               return(false);
            }
         }

         /**
         *
         * Checks the active object list in the passed in object (debug) to ensure they are
         * correctly setup. REQUIRES that the 'size lock' has been activated. Must be compiled
         * in (not as default). When compiled in a warning message will be emitted, as this
         * function is not designed to be used in a release version.
         *
         * \param marker The marker object to be checked.
         * \returns True if data is okay, false if not.
         *
         */

         bool CheckActive(CPB_Marker &marker)
         {
#if 0
#pragma RWS_COMP_WARN(CycPartBuff::CheckActive is compiled in (DEBUG & SLOW))

            RwUInt32 count;

            for (count = 0; count < marker.numInBlock; count++)
            {
               RwV2d *pSize = reinterpret_cast<RwV2d *>(marker.sizeLock.data + marker.sizeLock.stride * count);

               if (marker.pActiveFlags[count] == false && (pSize->x != 0.0f || pSize->y != 0.0f))
               {
                  return(false);
               }
            }

            return(true);
#else
            return(true);
#endif
         }

         /**
         *
         * Changes the active state of the 'Nth' particle in the block pointed to by the
         * marker data. The 'Nth' particle is controlled via the 'index' element of the
         * structure (which is ZERO based). After this the particle will NOT be drawn.
         * Particles MUST be deactivated via this call or the system will fail. After a
         * call to this function do NOT change any parameters in the ptank for the
         * deactivated particle.
         *
         * \see Activate
         * \see GetFirstActive
         * \see GetNextActive
         * \param marker The marker object referencing the particle to be activated. Set its index as appropriate.
         *
         */

         void Deactivate(CPB_Marker &marker)
         {
            RWS_ASSERT(marker.index < marker.numInBlock, "deactivation index is too high, index >= numInBlock");

            // Set size of appropriate particle to zero & clear active flag.

            RwV2d *pSize = reinterpret_cast<RwV2d *>(marker.sizeLock.data + marker.sizeLock.stride * marker.index);
            RWS_ASSERT(marker.pActiveFlags[marker.index], "active flag is ALREADY false");

            pSize->x = 0.0f;
            pSize->y = 0.0f;
            marker.pActiveFlags[marker.index] = false;       // Same array as in the CPB_Node.

            // Decrement the active count on the block and check to see if still has any active in it.

            CPB_Node *pCur = reinterpret_cast<CPB_Node *>(marker.internalA);
            
            RWS_ASSERT(marker.numActive, "Active count is zero BEFORE decrement");

            marker.numActive--;
            pCur->m_numActive--;

            if (pCur->m_numActive == 0)
            {
               // No longer need to process this block, so move to the free list. Then link onto free list.

               pCur->Unlink(m_pUsedRoot);
               pCur->LinkAtRoot(m_pFreeRoot);
            }

            return;
         }

         /**
         *
         * Returns a block with at least one free (inactive) particle in it (if returns true).
         * If you need more they MAY be in the returned block. If they are not then call
         * 'GetFreeEnd' and then this call again to get another block. When activating blocks
         * use the 'Activate' function ONLY.
         *
         * NOTE: Unlike 'GetFirstActive' and 'GetNextActive' this call does NOT lock the size and
         * so the 'marker.sizeLock' is invalid, as will the index value. If you need to lock
         * the particles to change values (size should be set) then call 'GetFreeEnd' to complete
         * the process. DO NOT DIRECTLY CALL 'RpPTankAtomicUnlock' function.
         *
         * \see GetFreeEnd
         * \see Activate
         * \param marker A marker object to be populated with information on block numbers, active flags, etc...
         * \return True if block is found with AT LEAST 1 inactive particle, false if not block found.
         *
         */

         bool GetFree(CPB_Marker &marker)
         {
            CPB_Node *pCur = m_pUsedRoot;

            // Scan used list first, don't want to start a new block unless absolutely necessary.

            while (pCur)
            {
               if (pCur->m_numActive < pCur->m_numHeld)
               {
                  // Lock the 'size' data.

                  pCur->Lock();
                  RpPTankAtomicLock(pCur->m_pTankAtomic, &marker.sizeLock, rpPTANKLFLAGSIZE, rpPTANKLOCKWRITE);

                  // Setup the rest of the structure...

                  marker.blockNum = pCur->m_blockNum;
                  marker.internalA = reinterpret_cast<RwUInt32>(pCur);
                  marker.numActive = pCur->m_numActive;
                  marker.numInBlock = pCur->m_numHeld;
                  marker.pActiveFlags = pCur->m_pActiveFlags;
                  marker.pLifeTimes = pCur->m_pLifeTimes;
                  marker.pTankAtomic = pCur->m_pTankAtomic;

                  RWS_ASSERT(CheckFree(marker), "free Particle check failed");

                  return(true);
               }
               pCur = pCur->m_pNext;
            }

            // Didn't find one, so move free to used and return it (if one).

            if (m_pFreeRoot)
            {
               CPB_Node *pNode = m_pFreeRoot;

               // Unlink from free list and move to head of used list...

               pNode->Unlink(m_pFreeRoot);
               pNode->LinkAtRoot(m_pUsedRoot);

               // Lock the 'size' data.

               pNode->Lock();
               RpPTankAtomicLock(pNode->m_pTankAtomic, &marker.sizeLock, rpPTANKLFLAGSIZE, rpPTANKLOCKWRITE);

               // Setup data and return...

               marker.blockNum = pNode->m_blockNum;
               marker.internalA = reinterpret_cast<RwUInt32>(pNode);
               marker.numActive = pNode->m_numActive;
               marker.numInBlock = pNode->m_numHeld;
               marker.pActiveFlags = pNode->m_pActiveFlags;
               marker.pLifeTimes = pNode->m_pLifeTimes;
               marker.pTankAtomic = pNode->m_pTankAtomic;

               RWS_ASSERT(CheckFree(marker), "free Particle check failed");

               return(true);
            }
            else
            {
               return(false);
            }
         }

         /**
         *
         * Checks that the number indicated as free matches the number of free flags (debug). Must
         * be compiled in (not as default). When compiled in a warning message will be emitted, as
         * this function is not designed to be used in a release version.
         *
         * \param marker The marker object to be checked.
         * \return True if number matches flags, false if not.
         *
         */

         bool CheckFree(CPB_Marker &marker)
         {
#if 0
#pragma RWS_COMP_WARN(CycPartBuff::CheckFree is compiled in (DEBUG & SLOW))

            RwUInt32 countActive = 0,
                  count;

            for (count = 0; count < marker.numInBlock; count++)
            {
               if (marker.pActiveFlags[count])
               {
                  countActive++;
               }
            }

            if (countActive == marker.numActive)
            {
               return(true);
            }
            else
            {
               RWS_ASSERT(countActive == marker.numActive, "num free in structure does NOT match flags");
               return(false);
            }
#else
            return(true);
#endif
         }

         /**
         *
         * Closes the block after adding / activation is complete. MUST be called after each 'GetFree' that
         * returned successfully (true) and caller has completed activation. Call BEFORE any other call to
         * 'GetFree'.
         *
         * \see GetFree
         * \param marker The marker object obtained from the 'GetFree' function.
         *
         */

         void GetFreeEnd(CPB_Marker &marker)
         {
            CPB_Node *pCur = reinterpret_cast<CPB_Node *>(marker.internalA);

            // Lock previous atomic's data.

            pCur->Unlock();

            return;
         }

         /**
         *
         * Call this function to activate a particle in a block. Do NOT do this directly as it will
         * break the control system. The particle to activate is specified by setting the 'index'
         * element of the 'marker' structure, which is ZERO based.
         *
         * \see GetFree
         * \see Deactivate
         * \param marker The marker specifying the particle to be activated via the 'index' element.
         *
         */

         void Activate(CPB_Marker &marker)
         {
            RWS_ASSERT(marker.index < marker.numInBlock, "deactivation index is too high, index >= numInBlock");

            // Set active and increment the active count for the block.

            RWS_ASSERT(!marker.pActiveFlags[marker.index], "active flag is ALREADY true");

            marker.pActiveFlags[marker.index] = true;

            CPB_Node *pCur = reinterpret_cast<CPB_Node *>(marker.internalA);
            
            RWS_ASSERT(marker.numActive <= marker.numInBlock, "Active already at maximum BEFORE increment");

            marker.numActive++;
            pCur->m_numActive++;

            return;
         }

         /**
         *
         * Renders all particle tanks which contain active particles (ONLY). The camera must have been
         * added to the world, etc. This calls ONLY the atomic rendering functions, render state calls,
         * etc... If no particles are active this function will do nothing.
         *
         */

         void Render(void)
         {
            if (m_pUsedRoot)
            {
               const RwBool trueVal = TRUE,
                     falseVal = FALSE;
               RwBool writeState,
                     testState;
               CPB_Node *pCur;

               // Get current modes...

               RwRenderStateGet(rwRENDERSTATEZWRITEENABLE, &writeState);
               RwRenderStateGet(rwRENDERSTATEZTESTENABLE, &testState);

               // Set modes needed.

               if (m_renderMode == RM_NO_Z_WRITE || m_renderMode == RM_DUAL_PASS)
               {
                  RwRenderStateSet(rwRENDERSTATEZTESTENABLE, reinterpret_cast<void*>(trueVal));
                  RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, reinterpret_cast<void*>(falseVal));
               }
               else
               {
                  // must be z-write mode...

                  RwRenderStateSet(rwRENDERSTATEZTESTENABLE, reinterpret_cast<void*>(trueVal));
                  RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, reinterpret_cast<void*>(trueVal));
               }

               // Render (First pass)...

               pCur = m_pUsedRoot;
               while (pCur)
               {
                  RpPTankAtomicSetBlendModes(pCur->m_pTankAtomic, m_srcBlend, m_destBlend);
                  RpAtomicRender(pCur->m_pTankAtomic);
                  pCur = pCur->m_pNext;
               }

               // If dual-pass, do second pass...

               if (m_renderMode == RM_DUAL_PASS)
               {
                  // Set modes so data is written to z-buffer ONLY - no change in color / alpha buffers.

                  RwRenderStateSet(rwRENDERSTATEZTESTENABLE, reinterpret_cast<void*>(trueVal));
                  RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, reinterpret_cast<void*>(trueVal));

                  // Render...

                  pCur = m_pUsedRoot;
                  while (pCur)
                  {
                     RpPTankAtomicSetBlendModes(pCur->m_pTankAtomic, rwBLENDZERO, rwBLENDONE);
                     RpAtomicRender(pCur->m_pTankAtomic);
                     pCur = pCur->m_pNext;
                  }
               }

               // Restore old modes...

               RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, reinterpret_cast<void *>(writeState));
               RwRenderStateSet(rwRENDERSTATEZTESTENABLE, reinterpret_cast<void *>(testState));
            }

            return;
         }

         /**
         *
         * Sets the mode used during the rendering of the particles.
         *
         * \param mode The rendering mode to use.
         *
         */

         void SetRenderMode(RENDER_MODE mode)
         {
            m_renderMode = mode;

            return;
         }

         /**
         *
         * Sets the source blending mode used by the particle system during rendering.
         *
         * \param mode The new RenderWare blending mode to use.
         *
         */

         void SetSrcBlend(RwBlendFunction mode)
         {
            m_srcBlend = mode;

            return;
         }

         /**
         *
         * Sets the destination blending mode used by the particle system during rendering.
         *
         * \param mode The new RenderWare blending mode to use.
         *
         */

         void SetDestBlend(RwBlendFunction mode)
         {
            m_destBlend = mode;

            return;
         }
      };
   }     //namespace Particle
}        //namespace RWS
