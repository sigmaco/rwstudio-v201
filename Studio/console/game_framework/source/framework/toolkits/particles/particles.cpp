/*****************************************************************************
*
* File :      Particle.cpp
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

#include "precomp.h"

// Left to its own devices, the MW linker will dead-strip the registrar ctors, so...

#ifdef __MWERKS__
#pragma force_active on
#endif

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//

#include <rpptank.h>
#include <rpworld.h>
#include <rprandom.h>

#include "framework/core/macros/debugmacros.h"
#include "framework/toolkits/math/maths.h"
#include "framework/mainloop/render.h"

#include "particles.h"


namespace RWS
{
   namespace Particle
   {
      ////////////////////////////////////////////////////////////////////////////
      //
      // CPB_BasePart class functions (Doxygen comments for class in header file).
      //
      ////////////////////////////////////////////////////////////////////////////

      /**
      *
      * Allocate particle data arrays and sets up the particle handling. Called by the
      * CycPartBuff. The numBlocks value can be zero, if one time allocation (via the
      * Resize function) is to be used. This is useful when used with behaviors because
      * of the attribute passing system being used for initialization instead of the
      * constructor (with default values).
      *
      * \see CycPartBuff
      * \see Resize
      * \param numBlocks The number of particle blocks to create
      * \param blockSize The size of each block.
      *
      */

      CPB_BasePart::CPB_BasePart(RwUInt32 numBlocks, RwUInt32 blockSize)
            : m_ppAnimBlocks(0),
            m_secStageOn(false),
            m_enabled(true),
            m_rate(0),
            m_rateSpread(0),
            m_useRate(0),
            m_useRateSpread(0),
            m_lifeTime(0),
            m_lifeSpread(0),
            m_secLifeAdj(0),
            m_secLifeTime(1),       // Zero will cause problems, minimum value is 1. Disable via separate flag.
            m_secEmit(0),
            m_secUseEmit(0),
            m_secEmitSpread(0),
            m_secUseEmitSpread(0),
            m_numBlocks(numBlocks),
            m_blockSize(blockSize),
            m_xAngSpread(0.0f),
            m_zAngSpread(0.0f),
            m_emitSpreadX(0.0f),
            m_emitSpreadY(0.0f),
            m_emitSpreadZ(0.0f),
            m_secEmitMinAng(0.0f),
            m_secEmitMaxAng(0.0f),
            m_rateAdjust(1.0f)
#if defined(SKY)
            , m_pAnimOriginalAddr(0)
#endif
      {
         RWS_FUNCTION("RWS::Particle::CPB_BasePart::CPB_BasePart");

         m_basePos.x = 0.0f;
         m_basePos.y = 0.0f;
         m_basePos.z = 0.0f;

         m_xAxisVect.x = 1.0f;
         m_xAxisVect.y = 0.0f;
         m_xAxisVect.z = 0.0f;
         
         m_yAxisVect.x = 0.0f;
         m_yAxisVect.y = 1.0f;
         m_yAxisVect.z = 0.0f;
         
         m_zAxisVect.x = 0.0f;
         m_zAxisVect.y = 0.0f;
         m_zAxisVect.z = 1.0f;

         memset(m_states, 0, sizeof(StateData) * BPS_TOTAL);

         // First state MUST setup everything.

         m_states[BPS_PRIMARY_START].m_partFlags = CPB_BP_ANIM_ACCEL | CPB_BP_ANIM_GRAVITY | CPB_BP_ANIM_DRAG |
               CPB_BP_ANIM_COL | CPB_BP_ANIM_SIZE;

         // Block size & number may be zero in cases where 'Resize' is used to initialize instead.

         RWS_ASSERT(blockSize, "Cannot have a zero block size, this is REQUIRED for resizing");
         Resize(numBlocks);

         RWS_RETURNVOID();
      }

      /**
      *
      * Frees the particle data arrays, closes down the system, etc...
      *
      */

      CPB_BasePart::~CPB_BasePart(void)
      {
         RWS_FUNCTION("RWS::Particle::CPB_BasePart::~CPB_BasePart");

         if (m_ppAnimBlocks)
         {
            for (RwUInt32 count = 0; count < m_numBlocks; count++)
            {
               if (m_ppAnimBlocks[count])
               {
                  //delete[] m_ppAnimBlocks[count];
                  delete (m_ppAnimBlocks[count]);
               }
            }
            delete[] m_ppAnimBlocks;
         }

         RWS_RETURNVOID();
      }

      /**
      * 
      * Used to change the size of or initialize the blocks in the class. Will assert on failure. Called
      * by the CycPartBuff's Resize function.
      *
      * This function should not be used in release versions, it will cause memory fragmentation. Should be
      * used in editor version, as allows minimum required particle tank size to be easily set.
      *
      * \see CycPartBuff
      * \see CycPartBuff::Resize
      * \param newSize The new number of blocks. You cannot change the block's individual sizes after construction.
      *
      */

      void CPB_BasePart::Resize(RwUInt32 newSize)
      {
         RWS_FUNCTION("RWS::Particle::CPB_BasePart::Resize");

         RwUInt32 newNumBlocks;
         bool retVal;

         // Calculate new number of blocks.

         newNumBlocks = newSize / m_blockSize;

         if (newSize % m_blockSize)
         {
            newNumBlocks++;
         }

         // Need to change size ?

         if (newNumBlocks != m_numBlocks)
         {
            // Create new pointer array.

            retVal = CreateNewPtrArray(newNumBlocks);
            RWS_ASSERT(retVal, "could not allocate new blocks");

            // Copy across still required blocks.

            for (RwUInt32 count = 0; count < m_numBlocks && count < newNumBlocks; count++)
            {
               CopyPtrArrayElement(count);
            }

            // Delete or add ?

            if (newNumBlocks < m_numBlocks)
            {
               // Delete extra blocks.

               for (RwUInt32 count = newNumBlocks; count < m_numBlocks; count++)
               {
                  DeleteOldPtrArrayElement(count);
               }
            }
            else
            {
               // Add new ones.

               for (RwUInt32 count = m_numBlocks; count < newNumBlocks; count++)
               {
                  retVal = CreateNewPtrArrayElement(count, m_blockSize);
                  RWS_ASSERT(retVal, "could not allocate new block inside array");
               }
            }

            // Release, if allocated, and insert new ones.

            SwapInNewPtrArray();
            m_numBlocks = newNumBlocks;
         }

         RWS_RETURNVOID();
      }

      /**
      *
      * Called for the resize function. Allocates a new array of pointers, which is held in limbo until the
      * SwapInNewPtrArray function is called.
      *
      * \return True if allocation is successful and false otherwise.
      * \param newNumBlocks The new number of blocks to allocate.
      * \see SwapInNewPtrArray
      *
      */

      bool CPB_BasePart::CreateNewPtrArray(RwUInt32 newNumBlocks)
      {
         RWS_FUNCTION("RWS::Particle::CPB_BasePart::CreateNewPtrArray");

         if ((m_ppNewAnimBlocks = ::RWS_NEW AnimData *[newNumBlocks]) == 0)
         {
            RWS_RETURN(false);
         }
         memset(m_ppNewAnimBlocks, 0, sizeof(AnimData *) * newNumBlocks);

         RWS_RETURN(true);
      }

      /**
      *
      * Called to copy data from the current block array to the new block array.
      *
      * \param index The index of the block to be copied.
      *
      */

      void CPB_BasePart::CopyPtrArrayElement(RwUInt32 index)
      {
         RWS_FUNCTION("RWS::Particle::CPB_BasePart::CopyPtrArrayElement");

         // Copy specified element from current array to new array...

//         m_ppNewVectBlocks[index] = m_ppVectBlocks[index];
         m_ppNewAnimBlocks[index] = m_ppAnimBlocks[index];

         RWS_RETURNVOID();
      }

      /**
      *
      * Removes a block which is no longer needed from the current array of blocks. The block is not going to be
      * transferred to the new block pointer array and some must be released.
      *
      * \param index The index of the block to free.
      *
      */

      void CPB_BasePart::DeleteOldPtrArrayElement(RwUInt32 index)
      {
         RWS_FUNCTION("RWS::Particle::CPB_BasePart::DeleteOldPtrArrayElement");

         // delete the specified block from the current block array.

         delete (m_ppAnimBlocks[index]);

         RWS_RETURNVOID();
      }

      /**
      *
      * Allocates a block into the specified slot. This occurs a block in the new block array is being a) resized or
      * b) is completely new.
      *
      * \return True on success and false on failure.
      * \param index The block number to be allocated in the NEW block array.
      * \param blockSize The size of block to be allocated.
      *
      */

      bool CPB_BasePart::CreateNewPtrArrayElement(RwUInt32 index, RwUInt32 blockSize)
      {
         RWS_FUNCTION("RWS::Particle::CPB_BasePart::CreateNewPtrArrayElement");

         // On PS2, need to allocate a block that is a multiple of 16Bytes in 
         // size, to allow the array to be sent to the scratchpad via DMA 
         // transfer. (DMA transfers are in blocks of 16Bytes).

         RwUInt32 uBytesToAlloc = ( ( ( sizeof(AnimData) * blockSize ) + 15 ) >> 4 ) << 4;

         if ((m_ppNewAnimBlocks[index] = reinterpret_cast<AnimData *>(::RWS_OP_NEW(uBytesToAlloc)) ) == 0)
         {
            RWS_RETURN(false);
         }

         RWS_RETURN(true);
      }

      /**
      *
      * Called to swap the new pointer block array with the old one. The new one MUST be setup. The old one will be
      * freed.
      *
      */

      void CPB_BasePart::SwapInNewPtrArray(void)
      {
         RWS_FUNCTION("RWS::Particle::CPB_BasePart::SwapInNewPtrArray");

         // Free up the old ones - if any...

         if (m_ppAnimBlocks)
         {
            //delete[] m_ppAnimBlocks;
            delete (m_ppAnimBlocks);
         }


         // Swap in the new ones...

         m_ppAnimBlocks = m_ppNewAnimBlocks;

         m_ppNewAnimBlocks = 0;

         RWS_RETURNVOID();
      }

      /**
      *
      * Takes in state data and uses it to setup the other passed in particle structures.
      * The vector is assumed to be pointing in the correct direction already. The isNormVect
      * flag is used to indicate if it should be normalized before velocities are applied. If
      * the flag is true is will NOT be normalized. The color, size and some fields of the
      * anim data will be setup by this function. The vector, color, size and animation data
      * will normally be elements of the arrays held within this class.
      *
      * \see DoPartAnimSetup
      * \param state The state data structure to be used for setup. This is the start or initial state data.
      * \param pVect The vectors to be adjusted by the call. Acceleration is applied to it. See 'isNormVect'.
      * \param pCol The color to be setup by the call.
      * \param pSize The size to be setup by the call.
      * \param pAnim The animation data block to be PARTLY setup by this call. See 'DoPartAnimSet' function.
      * \param isNormVect True if the input vector is a unit vector, false if it must be normalized before
      * acceleration can be correctly applied.
      * \param isInitAccel True if the state holds initial acceleration, i.e. there is not current velocity
      * in the particle which needs to be kept. This would only be used on the primary start state.
      *
      */

      void CPB_BasePart::DoStdPartSetup(const StateData &state, RwRGBA *pCol, RwV2d *pSize,
            AnimData *pAnim, bool isNormVect, bool isInitAccel)
      {
         RWS_FUNCTION("RWS::Particle::CPB_BasePart::DoStdPartSetup");

         // Setup velocity...

         if ((state.m_partFlags & CPB_BP_ANIM_ACCEL) != 0)
         {
            RwV3d vect;
            RwReal accel;

            // Normalize vector ?

            if (isNormVect)
            {
               vect = pAnim->m_Vel;
            }
            else
            {
               RwReal len;

               // Length MAY be zero, so do manual normalize as RwV3dNormalize cannot cope.

               if ((len = RwV3dLength(&pAnim->m_Vel)) > 0.0f)
               {
                  RwV3dScale(&vect, &pAnim->m_Vel, 1.0f / len);
               }
               else
               {
                  vect = pAnim->m_Vel;
               }
            }

            // Accelerate the particle (once only).

            if (state.m_accelSpread == 0.0f)
            {
               accel = state.m_accel;
            }
            else
            {
               accel = state.m_accel + state.m_accelSpread * RealRand();
            }

            // Initializing or adding ?

            if (isInitAccel)
            {
               pAnim->m_Vel.x = vect.x * accel;
               pAnim->m_Vel.y = vect.y * accel;
               pAnim->m_Vel.z = vect.z * accel;
            }
            else
            {
               // Adding (boost).

               pAnim->m_Vel.x += vect.x * accel;
               pAnim->m_Vel.y += vect.y * accel;
               pAnim->m_Vel.z += vect.z * accel;
            }
         }

         // Setup gravity...

         if ((state.m_partFlags & CPB_BP_ANIM_GRAVITY) != 0)
         {
            pAnim->m_grav = state.m_gravity;
         }

         // Setup drag...

         if ((state.m_partFlags & CPB_BP_ANIM_DRAG) != 0)
         {
            pAnim->m_drag = state.m_drag;
         }

         // Setup color...

         if ((state.m_partFlags & CPB_BP_ANIM_COL) != 0)
         {
            if (!state.m_redSpread && !state.m_greenSpread && !state.m_blueSpread && !state.m_alphaSpread)
            {
               *pCol = state.m_col;
            }
            else
            {
               RwInt32 r,
                     g,
                     b,
                     a;

               // Calculate individual, random, color values.

               r = static_cast<RwInt32>(static_cast<RwReal>(state.m_red) +
                     static_cast<RwReal>(state.m_redSpread) * ((RealRand() * 2) - 1.0f));
               g = static_cast<RwInt32>(static_cast<RwReal>(state.m_green) +
                     static_cast<RwReal>(state.m_greenSpread) * ((RealRand() * 2) - 1.0f));
               b = static_cast<RwInt32>(static_cast<RwReal>(state.m_blue) +
                     static_cast<RwReal>(state.m_blueSpread) * ((RealRand() * 2) - 1.0f));
               a = static_cast<RwInt32>(static_cast<RwReal>(state.m_alpha) +
                     static_cast<RwReal>(state.m_alphaSpread) * ((RealRand() * 2) - 1.0f));

               // Clip ranges.

               if (r < 0)
               {
                  r = 0;
               }
               else if (r > 255)
               {
                  r = 255;
               }

               if (g < 0)
               {
                  g = 0;
               }
               else if (g > 255)
               {
                  g = 255;
               }

               if (b < 0)
               {
                  b = 0;
               }
               else if (b > 255)
               {
                  b = 255;
               }

               if (a < 0)
               {
                  a = 0;
               }
               else if (a > 255)
               {
                  a = 255;
               }

               // Store color value...

               pCol->red = static_cast<RwUInt32>(r);
               pCol->green = static_cast<RwUInt32>(g);
               pCol->blue = static_cast<RwUInt32>(b);
               pCol->alpha = static_cast<RwUInt32>(a);
            }
         }

         // Setup size...

         if ((state.m_partFlags & CPB_BP_ANIM_SIZE) != 0)
         {
            pSize->x = state.m_size;
            pSize->y = state.m_size;

            if (state.m_sizeSpread != 0.0f)
            {
               RwReal rnd = RealRand();

               pSize->x += state.m_sizeSpread * rnd;
               pSize->y += state.m_sizeSpread * rnd;
            }
         }

         RWS_RETURNVOID();
      }

      /**
      *
      * Takes in state data, color and size info and uses it to setup the animation data.
      * The animFrames parameter is the number of frames to animate over. This CANNOT BE
      * ZERO (set to 1 instead). Function requires that the 'DoStdPartSetup' has been
      * called FIRST.
      *
      * \see DoStdPartSetup
      * \param state The END state data to be used for the setup. This is the state at the END of the animation.
      * \param pCol The color information for the particle. Is not changed, but used for animation calculation.
      * MUST be the value setup by the 'DoStdPartSetup' function.
      * \param pSize The size information for the particle. Is not changed, but used for animation calculation.
      * MUST be the value setup by the 'DoStdPartSetup' function.
      * \param pAnim The animation data block for the particle. MUST be the one passed into the 'DoStdPartSetup'
      * function. This call will complete the data setup in the object.
      * \param animFrames The number of frames the animation is to be over.
      *
      */

      void CPB_BasePart::DoPartAnimSetup(const StateData &state, const RwRGBA *pCol, const RwV2d *pSize,
            AnimData *pAnim, RwReal animFrames)
      {
         RWS_FUNCTION("RWS::Particle::CPB_BasePart::DoPartAnimSetup");
         RWS_ASSERT(animFrames, "number of animation frames CANNOT be 0 - divide by zero error");

         // Copy flags over...

         pAnim->m_flags = state.m_partFlags;

         // Calculate each required animation - acceleration...

         if ((state.m_partFlags & CPB_BP_ANIM_ACCEL) != 0)
         {
            if (state.m_accelSpread != 0.0f)
            {
               pAnim->m_accel = state.m_accel + RealRand() * state.m_accelSpread;
            }
            else
            {
               pAnim->m_accel = state.m_accel;
            }
         }

         // Gravity...

         if ((state.m_partFlags & CPB_BP_ANIM_GRAVITY) != 0)
         {
            pAnim->m_gravMod = (state.m_gravity - pAnim->m_grav) / animFrames;
         }

         // Drag...

         if ((state.m_partFlags & CPB_BP_ANIM_DRAG) != 0)
         {
            pAnim->m_dragMod = (state.m_drag - pAnim->m_grav) / animFrames;
         }
         else
         {
            pAnim->m_dragMod = 0.0f;
         }

         // Color...

         if ((state.m_partFlags & CPB_BP_ANIM_COL) != 0)
         {
            RwReal endR,
                  endG,
                  endB,
                  endA;
            
            // Choose end color - red...

            if (state.m_redSpread == 0)
            {
               endR = static_cast<RwReal>(state.m_red);
            }
            else
            {
               endR = state.m_red + (RealRand() * 2.0f - 1.0f) * state.m_redSpread;

               if (endR < 0.0f)
               {
                  endR = 0.0f;
               }
               else if (endR > 255.0f)
               {
                  endR = 255.0f;
               }
            }

            // Green...

            if (state.m_greenSpread == 0)
            {
               endG = static_cast<RwReal>(state.m_green);
            }
            else
            {
               endG = state.m_green + (RealRand() * 2.0f - 1.0f) * state.m_greenSpread;

               if (endG < 0.0f)
               {
                  endG = 0.0f;
               }
               else if (endG > 255.0f)
               {
                  endG = 255.0f;
               }
            }

            // Blue...

            if (state.m_blueSpread == 0)
            {
               endB = static_cast<RwReal>(state.m_blue);
            }
            else
            {
               endB = state.m_blue + (RealRand() * 2.0f - 1.0f) * state.m_blueSpread;

               if (endB < 0.0f)
               {
                  endB = 0.0f;
               }
               else if (endB > 255.0f)
               {
                  endB = 255.0f;
               }
            }

            // Alpha...

            if (state.m_alphaSpread == 0)
            {
               endA = static_cast<RwReal>(state.m_alpha);
            }
            else
            {
               endA = state.m_alpha + (RealRand() * 2.0f - 1.0f) * state.m_alphaSpread;

               if (endA < 0.0f)
               {
                  endA = 0.0f;
               }
               else if (endA > 255.0f)
               {
                  endA = 255.0f;
               }
            }

            // Setup start color.

            pAnim->m_r = ( pCol->red );
            pAnim->m_g = ( pCol->green );
            pAnim->m_b = ( pCol->blue );
            pAnim->m_a = ( pCol->alpha );

            pAnim->m_r = ( pAnim->m_r << 8 );
            pAnim->m_g = ( pAnim->m_g << 8 );
            pAnim->m_b = ( pAnim->m_b << 8 );
            pAnim->m_a = ( pAnim->m_a << 8 );

            // Setup steps...

            // Note: The values calculated below need to be in the range 0-255, 
            //       shifted left by 8 bits (for greater accuracy). Hence, the 
            //       multiply by 256.0f, which is the same as shifting left by 
            //       8 (2^8 = 256).
            pAnim->m_rMod = static_cast<RwInt16>( ( ( endR - static_cast<RwReal>(pCol->red) ) / animFrames ) * 256.0f );
            pAnim->m_gMod = static_cast<RwInt16>( ( ( endG - static_cast<RwReal>(pCol->green) ) / animFrames ) * 256.0f );
            pAnim->m_bMod = static_cast<RwInt16>( ( ( endB - static_cast<RwReal>(pCol->blue) ) / animFrames ) * 256.0f );
            pAnim->m_aMod = static_cast<RwInt16>( ( ( endA - static_cast<RwReal>(pCol->alpha) ) / animFrames ) * 256.0f );
         }

         // Size...

         if ((state.m_partFlags & CPB_BP_ANIM_SIZE) != 0)
         {
            RwReal endSize;

            if (state.m_sizeSpread != 0.0f)
            {
               endSize = state.m_size + RealRand() * state.m_sizeSpread;
            }
            else
            {
               endSize = state.m_size;
            }

            pAnim->m_sizeMod = (endSize - pSize->x) / animFrames;
         }

         RWS_RETURNVOID();
      }

      /**
      *
      * Called to setup the specific parameters for this particular class of particle.
      * This is once-only initialization per pTank atomic. Called by the CycPartBuff
      * during construction and Resize operations to setup the atomics held within it.
      *
      * The param data for this particle system is not used.
      *
      * \see CycPartBuff
      * \param pAtomic The particle (pTank) object's RenderWare atomic.
      * \param pParamData The specific parameter data for this class of particle (see code).
      *
      */

      void CPB_BasePart::DoAtomicSetup(RpAtomic *pAtomic, void *pParamData)
      {
         RWS_FUNCTION("RWS::Particle::CPB_BasePart::DoAtomicSetup");

         RpPTankAtomicSetVertexAlpha(pAtomic, TRUE);
         RpPTankAtomicSetBlendModes(pAtomic, rwBLENDSRCALPHA, rwBLENDINVSRCALPHA);

         RWS_RETURNVOID();
      }

      /**
      *
      * Sets up the new particle data based on the reference data passed in and the particle axis information.
      * Does not do a complete setup, you need to call the 'DoStdPartSetup' and 'DoPartAnimSetup' functions too.
      * This function modifies the pointers within the 'addAccess' structure (pos, size, col, anim, vect, etc) to
      * correctly position the 'current' values for the add. The index value within the 'addAccess' object must
      * have been setup to point to the required element - used for setting pos, size, col, etc.
      *
      * \see CycPartBuff::Tick
      * \see DoStdPartSetup
      * \see DoPartAnimSetup
      * \param addAccess The access structure indicating the particle to be used for the setup.
      * \param access The access structure indicating the particle to be used for reference.
      * \param partAxis The particle axis information used to setup the new particles direction of movement.
      * \param addMarker The marker object for the particle being setup.
      *
      */

      void CPB_BasePart::SecEmitSetup(PartAccess &addAccess, PartAccess &access, PartAxis &partAxis,
            CPB_Marker &addMarker)
      {
         RWS_FUNCTION("RWS::Particle::CPB_BasePart::SecEmitSetup");

         RwReal addRnd,
               addSinVal,
               addCosVal;
         RwV3d addTempVect;

         // Lifetime...

         if (m_secLifeAdj)
         {
            addMarker.pLifeTimes[addAccess.m_index] = *access.m_pLife + (rand() % m_secLifeAdj);
         }
         else
         {
            addMarker.pLifeTimes[addAccess.m_index] = *access.m_pLife;
         }

         // Do specific setup...

         addAccess.m_pPos = reinterpret_cast<RwV3d *>(addAccess.m_posLock.data + addAccess.m_posLock.stride *
               addAccess.m_index);
         addAccess.m_pSize = reinterpret_cast<RwV2d *>(addMarker.sizeLock.data + addMarker.sizeLock.stride *
               addAccess.m_index);
         addAccess.m_pCol = reinterpret_cast<RwRGBA *>(addAccess.m_colLock.data + addAccess.m_colLock.stride *
               addAccess.m_index);
         addAccess.m_pAnim = addAccess.m_pAnimBlock + addAccess.m_index;

         addRnd = RealRand();
         addSinVal = RwSin(addRnd * TWO_PI);
         addCosVal = RwCos(addRnd * TWO_PI);

         addTempVect.x = partAxis.m_partXAxis.x * addSinVal + partAxis.m_partZAxis.x * addCosVal;
         addTempVect.y = partAxis.m_partXAxis.y * addSinVal + partAxis.m_partZAxis.y * addCosVal;
         addTempVect.z = partAxis.m_partXAxis.z * addSinVal + partAxis.m_partZAxis.z * addCosVal;

         // Calculate the deflection angle off the direction of travel.
         // ASSUMES 'angDiff' & 'inputLen' have already been setup.

         addRnd = RealRand();
         addSinVal = RwSin(addRnd * partAxis.m_angDiff + partAxis.m_minAng);
         addCosVal = RwCos(addRnd * partAxis.m_angDiff + partAxis.m_minAng);

         addTempVect.x = (addTempVect.x * addSinVal + partAxis.m_partYAxis.x * addCosVal) * partAxis.m_inputLen;
         addTempVect.y = (addTempVect.y * addSinVal + partAxis.m_partYAxis.y * addCosVal) * partAxis.m_inputLen;
         addTempVect.z = (addTempVect.z * addSinVal + partAxis.m_partYAxis.z * addCosVal) * partAxis.m_inputLen;

         // Do copy parent particle data then do other setup... ASSUMES the value of
         // 'animFrames' has been setup already.

         *addAccess.m_pPos = *access.m_pPos;
         *addAccess.m_pCol = *access.m_pCol;
         *addAccess.m_pSize = *access.m_pSize;
         *addAccess.m_pAnim = *access.m_pAnim;

         // Set the velocity vector of the new particle

         addAccess.m_pAnim->m_Vel.x = addTempVect.x;
         addAccess.m_pAnim->m_Vel.y = addTempVect.y;
         addAccess.m_pAnim->m_Vel.z = addTempVect.z;

         RWS_RETURNVOID();
      }

      /**
      *
      * Modifies the particle's flight path using the passed in partAxis data. The particle modified is the one
      * currently setup in the passed in access structure.
      *
      * \see CycPartBuff::Tick
      * \see PartAxis::Setup
      * \param access The particle access structure with the particle to be modified setup within it.
      * \param partAxis The particle direction of movement axis structure. Must have been setup.
      *
      */

      void CPB_BasePart::ModifyPartForSecStage(PartAccess &access, PartAxis &partAxis)
      {
         RWS_FUNCTION("RWS::Particle::CPB_BasePart::ModifyPartForSecState");

         RwReal rnd,
               sinVal,
               cosVal;
         RwV3d tempVect;

         // Modify the input particle, calculate the rotational direction around the
         // particle's direction of travel (partYAxis within the partAxis object).

         rnd = RealRand();
         sinVal = RwSin(rnd * TWO_PI);
         cosVal = RwCos(rnd * TWO_PI);

         tempVect.x = partAxis.m_partXAxis.x * sinVal + partAxis.m_partZAxis.x * cosVal;
         tempVect.y = partAxis.m_partXAxis.y * sinVal + partAxis.m_partZAxis.y * cosVal;
         tempVect.z = partAxis.m_partXAxis.z * sinVal + partAxis.m_partZAxis.z * cosVal;

         // Calculate the deflection angle off the direction of travel.

         rnd = RealRand();
         sinVal = RwSin(rnd * partAxis.m_angDiff + partAxis.m_minAng);
         cosVal = RwCos(rnd * partAxis.m_angDiff + partAxis.m_minAng);

         access.m_pAnim->m_Vel.x = (tempVect.x * sinVal + partAxis.m_partYAxis.x * cosVal) *
               partAxis.m_inputLen;
         access.m_pAnim->m_Vel.y = (tempVect.y * sinVal + partAxis.m_partYAxis.y * cosVal) *
               partAxis.m_inputLen;
         access.m_pAnim->m_Vel.z = (tempVect.z * sinVal + partAxis.m_partYAxis.z * cosVal) *
               partAxis.m_inputLen;

         RWS_RETURNVOID();
      }

      /**
      *
      * Does the setup for a single particle. This is called by the Emit function and is provided so that derived
      * classes can also call it for base particle setup during the emitting stage. This function modifies several
      * data pointers within the passed in PartAccess class (pointers to position, color, size, etc). These are
      * modified via the index within the PartAccess class - which must be correctly set by the caller.
      *
      * \see CycPartBuff::Emit
      * \see CycPartBuff::Tick
      * \param marker The marker object for the block to add to.
      * \param access The access structure for the block to add to.
      * \returns The number of animation frames used for the emit.
      *
      */

      RwReal CPB_BasePart::EmitSetup(CPB_Marker &marker, PartAccess &access)
      {
         RWS_FUNCTION("RWS::Particle::CPB_BasePart::EmitSetup")

         // Do start specific data setup...

         access.m_pPos = reinterpret_cast<RwV3d *>(access.m_posLock.data + access.m_posLock.stride * access.m_index);
         *access.m_pPos = m_basePos;

         if (m_emitSpreadX != 0.0f)
         {
            RwReal rnd = RealRand() - 0.5f;

            access.m_pPos->x += m_xAxisVect.x * m_emitSpreadX * rnd;
            access.m_pPos->y += m_xAxisVect.y * m_emitSpreadX * rnd;
            access.m_pPos->z += m_xAxisVect.z * m_emitSpreadX * rnd;
         }

         if (m_emitSpreadY != 0.0f)
         {
            RwReal rnd = RealRand() - 0.5f;

            access.m_pPos->x += m_yAxisVect.x * m_emitSpreadY * rnd;
            access.m_pPos->y += m_yAxisVect.y * m_emitSpreadY * rnd;
            access.m_pPos->z += m_yAxisVect.z * m_emitSpreadY * rnd;
         }

         if (m_emitSpreadZ != 0.0f)
         {
            RwReal rnd = RealRand() - 0.5f;

            access.m_pPos->x += m_zAxisVect.x * m_emitSpreadZ * rnd;
            access.m_pPos->y += m_zAxisVect.y * m_emitSpreadZ * rnd;
            access.m_pPos->z += m_zAxisVect.z * m_emitSpreadZ * rnd;
         }

         // Movement vector...

         if (m_xAngSpread == 0.0f && m_zAngSpread == 0.0f)
         {
            access.m_pAnimBlock[access.m_index].m_Vel = m_yAxisVect;
         }
         else
         {
            RwV3d vect1,
                  vect2;

            // Calculate vector based on all 3 axis vectors (baseVect, xAxisVect & zAxisVect).
            // Do x and y first...

            if (m_xAngSpread != 0.0f)
            {
               RwReal sinVal,
                     cosVal,
                     rnd;

               rnd = RealRand() - 0.5f;
               sinVal = RwSin(m_xAngSpread * rnd);
               cosVal = RwCos(m_xAngSpread * rnd);
               vect1.x = m_yAxisVect.x * cosVal + m_xAxisVect.x * sinVal;
               vect1.y = m_yAxisVect.y * cosVal + m_xAxisVect.y * sinVal;
               vect1.z = m_yAxisVect.z * cosVal + m_xAxisVect.z * sinVal;
            }
            else
            {
               vect1 = m_yAxisVect;
            }

            // Apply z to this...

            if (m_zAngSpread != 0.0f)
            {
               RwReal sinVal,
                     cosVal,
                     rnd;

               rnd = RealRand() - 0.5f;
               sinVal = RwSin(m_zAngSpread * rnd);
               cosVal = RwCos(m_zAngSpread * rnd);
               vect2.x = vect1.x * cosVal + m_zAxisVect.x * sinVal;
               vect2.y = vect1.y * cosVal + m_zAxisVect.y * sinVal;
               vect2.z = vect1.z * cosVal + m_zAxisVect.z * sinVal;
            }
            else
            {
               vect2 = vect1;
            }

            access.m_pAnimBlock[access.m_index].m_Vel = vect2;
         }

         // Lifetime...

         if (m_lifeSpread)
         {
            marker.pLifeTimes[access.m_index] = rand() % m_lifeSpread;
         }
         else
         {
            marker.pLifeTimes[access.m_index] = 0;
         }

         // Ensure that, if the 2nd stage is on, the lifetime does
         // not exceed that value, i.e. clip to it.

         if (m_secStageOn && marker.pLifeTimes[access.m_index] >= m_secLifeTime)
         {
            // Since pre-increments, need to set to one before 2nd lifetime occurs.
            // m_secLifeTime SHOULD be guaranteed not to be zero.

            marker.pLifeTimes[access.m_index] = m_secLifeTime - 1;
         }

         // Apply standard setup...

         access.m_pSize = reinterpret_cast<RwV2d *>(marker.sizeLock.data + marker.sizeLock.stride * access.m_index);
         access.m_pCol = reinterpret_cast<RwRGBA *>(access.m_colLock.data + access.m_colLock.stride * access.m_index);

         DoStdPartSetup(m_states[BPS_PRIMARY_START], access.m_pCol, access.m_pSize,
               access.m_pAnimBlock + access.m_index, true, true);

         // Setup animation...

         RwReal animFrames;

         if (m_secStageOn && m_secLifeTime <= m_lifeTime)
         {
            if (marker.pLifeTimes[access.m_index] >= m_secLifeTime)
            {
               animFrames = 1.0f;
            }
            else
            {
               animFrames = static_cast<RwReal>(m_secLifeTime - marker.pLifeTimes[access.m_index]);
            }
         }
         else
         {
            if (marker.pLifeTimes[access.m_index] >= m_lifeTime)
            {
               animFrames = 1.0f;
            }
            else
            {
               animFrames = static_cast<RwReal>(m_lifeTime - marker.pLifeTimes[access.m_index]);
            }
         }

         DoPartAnimSetup(m_states[BPS_PRIMARY_END], access.m_pCol, access.m_pSize, access.m_pAnimBlock + access.m_index,
               animFrames);

         RWS_RETURN(animFrames);
      }

      //*****************************
      //
      // CPB_TexPart class functions.
      //
      //*****************************

      /**
      *
      * Frees up any used memory and cleans up.
      *
      * \see CPB_TexPart::CPB_TexPart
      *
      */

      CPB_TexPart::~CPB_TexPart(void)
      {
         RWS_FUNCTION("RWS::Particle::CPB_TexPart::~CPB_TexPart");

         if (m_ppTexAnimBlocks)
         {
            for (RwUInt32 count = 0; count < m_numBlocks; count++)
            {
               if (m_ppTexAnimBlocks[count])
               {
                  delete[] m_ppTexAnimBlocks[count];
               }
            }
            delete[] m_ppTexAnimBlocks;
         }

         RWS_RETURNVOID();
      }

      /**
      *
      * Sets the texture to be used by the particles system. The texture should NOT be changed often as the function
      * may have to change several particle system objects each time.
      *
      * \see CPB_TexPart::CPB_TexPart
      * \see SetNumSubTextures
      * \param pTex Pointer to RenderWare texture to use.
      * \param buffer The cyclic particle buffer holding the particle system.
      *
      */

      void CPB_TexPart::SetTexture(RwTexture *pTex, CycPartBuff<CPB_TexPart> &buffer)
      {
         RWS_FUNCTION("RWS::Particle::CPB_TexPart::SetTexture");

         // Different texture ?

         if (pTex != m_pCurTex)
         {
            CPB_Marker marker;
            bool found;

            // For each particle tank object...

            found = buffer.GetFirst(marker);
            while (found)
            {
               // Change the texture to the passed in one...

               RpPTankAtomicSetTexture(marker.pTankAtomic, pTex);

               // Get the next one...

               found = buffer.GetNext(marker);
            }

            m_pCurTex = pTex;
         }

         RWS_RETURNVOID();
      }

      /**
      *
      * Called to setup the specific parameters for this particular class of particle.
      * This is once-only initialization per pTank atomic. Called by the CycPartBuff
      * during construction and Resize operations to setup the atomics held within it.
      *
      * The param data for this particle system is not used.
      *
      * \see CycPartBuff
      * \see CPB_BasePart::DoAtomicSetup
      * \param pAtomic The particle (pTank) object's RenderWare atomic.
      * \param pParamData The specific parameter data for this class of particle.
      *
      */

      void CPB_TexPart::DoAtomicSetup(RpAtomic *pAtomic, void *pParamData)
      {
         RWS_FUNCTION("RWS::Particle::CPB_TexPart::DoAtomicSetup");

         // Do base setup...

         CPB_BasePart::DoAtomicSetup(pAtomic, pParamData);

         // Do setup for textured particles...

         RpPTankAtomicSetTexture(pAtomic, m_pCurTex);

         RWS_RETURNVOID();
      }

      /**
      *
      * Sets up the texture static data from the passed in state object. This function DOES NOT call any CPB_BasePart
      * functions. This must be performed by the caller.
      *
      * \param state The base state data used to setup the particle (holds flags).
      * \param texState The texture state to use to setup the particle data.
      * \param pTexUVs The texture UV values to be setup.
      * \param pTexAnimData The animation data for the texture particle.
      *
      */

      void CPB_TexPart::DoTexPartSetup(StateData &state, TexStateData &texState, RwV2d *pTexUVs,
            TexAnimData *pTexAnimData)
      {
         RWS_FUNCTION("RWS::Particle::CPB_TexPart::DoTexPartSetup");

         // Setup texture...

         if ((state.m_partFlags & CPB_BP_ANIM_SUB_TEX) != 0)
         {
            // Need to do random selection ?

            if (texState.m_minSubTex != texState.m_maxSubTex)
            {
               RwUInt32 min,
                     max;

               // Check for min > max - need to store values in case it's being edited (and min changed first), but must
               // also cope with values too. If not, could end up with MASSIVE number for modulus.

               min = texState.m_minSubTex;
               max = texState.m_maxSubTex;

               if (min > max)
               {
                  RwUInt32 temp = min;

                  min = max;
                  max = temp;
               }

               // Yep, choose one between in and max.

               pTexAnimData->m_subTexIndex = static_cast<RwReal>(min + (rand() % ((max - min) + 1)));
               SetSubTextureFromIndex(pTexUVs, static_cast<RwUInt32>(pTexAnimData->m_subTexIndex));
            }
            else
            {
               // No, just set.

               pTexAnimData->m_subTexIndex = static_cast<RwReal>(texState.m_minSubTex);
               SetSubTextureFromIndex(pTexUVs, texState.m_minSubTex);
            }

            pTexAnimData->m_subTexStart = pTexAnimData->m_subTexIndex;
         }
         else
         {
            // Necessary to set start to value of current - need when no secondary start set, to
            // ensure looping from primary end -> secondary end works correctly. This value needs
            // to be floored to work correctly, so also floor the index value.

            pTexAnimData->m_subTexStart = RwFloor(pTexAnimData->m_subTexIndex);
            pTexAnimData->m_subTexIndex = pTexAnimData->m_subTexStart;
         }

         RWS_RETURNVOID();
      }

      /**
      *
      * Sets up the texture animation data from the passed in state object. This function DOES NOT call any CPB_BasePart
      * functions. This must be performed by the caller. Code assumes non-animation data has been setup FIRST via a
      * call to DoTexPartSetup (immediately before - as it uses values setup by it for the animation setup).
      *
      * NOTE: If a secondary stage is used and the secondary start state does not modify the texture index, then
      * the animation for the secondary state will be from the primary end state to the secondary end state (if
      * one is set). If this is looped, then the process is primary end -> secondary end -> (flip) primary end ->
      * secondary end, etc.
      *
      * \see DoTexPartSetup
      * \param state The state used to setup the particle's animation (holds flags).
      * \param texState The texture state to use to setup the particle's animation from.
      * \param pTexAnimData The texture animation data to setup.
      * \param animFrames The number of frames to spread the animation over.
      *
      */

      void CPB_TexPart::DoTexAnimSetup(StateData &state, TexStateData &texState, TexAnimData *pTexAnimData,
            RwReal animFrames)
      {
         RWS_FUNCTION("RWS::Particle::CPB_TexPart::DoTexAnimSetup");

         // Need to calculate the animation data for the texture ?

         if ((state.m_partFlags & CPB_BP_ANIM_SUB_TEX) != 0)
         {
            // Random or fixed end frame ?

            if (texState.m_minSubTex != texState.m_maxSubTex)
            {
               // Random...

               pTexAnimData->m_subTexEnd = static_cast<RwReal>(texState.m_minSubTex + (rand() %
                     (texState.m_maxSubTex - texState.m_minSubTex)));
            }
            else
            {
               // Fixed...

               pTexAnimData->m_subTexEnd = static_cast<RwReal>(texState.m_minSubTex);
            }

            // Calculate the number of loops - can contain a random element.

            RwReal numLoops;

            if (texState.m_subTexRndLoops != 0.0f)
            {
               // Random element is active.

               numLoops = texState.m_subTexLoops + RealRand() * texState.m_subTexRndLoops;
            }
            else
            {
               numLoops = texState.m_subTexLoops;
            }

            // Calculate the stepping rate - must take looping into account here... NOTE: Animate to end+0.999
            // because 0->0.999 is 0, 1->1.999 is 1, etc. So to make same length of anim on each stage, need to
            // end on <end>+1. Must take off 0.001 at the end (rather than adding 0.999) because there may be
            // N loops and N*0.999 is less accurate then (N*1)-0.001. The latter always give a value slightly
            // less than the next full number, the former will become less accurate the greater the number of
            // loops. ALSO, the use of subTexIndex -> subTexEnd is necessary for correct secondary animation. If
            // use start -> end and animate 0->5 (1st) then ?->9 (2nd) get 0->5->9->0->5 if do NOT use index
            // (use start) or (correctly) 0->5->9 if DO use index.

            if (pTexAnimData->m_subTexEnd >= pTexAnimData->m_subTexIndex)
            {
               // Animating forward...

               pTexAnimData->m_subTexStep = (((pTexAnimData->m_subTexEnd - pTexAnimData->m_subTexIndex + 1.0f) *
                     (1.0f + numLoops)) - 0.001f) / animFrames;
               pTexAnimData->m_countUp = true;
            }
            else
            {
               // Animating backward...

               pTexAnimData->m_subTexStep = (((pTexAnimData->m_subTexEnd - pTexAnimData->m_subTexIndex - 1.0f) *
                     (1.0f + numLoops)) + 0.001f) / animFrames;
               pTexAnimData->m_countUp = false;

               // Need to ensure first element get an even time on screen, so add 0.999 to it.

               pTexAnimData->m_subTexIndex += 0.999f;
            }
         }

         RWS_RETURNVOID();
      }

      /**
      *
      * Same as base class function (which it called) but does additional work for textured system.
      *
      * \see CPB_BasePart::CreateNewPtrArray
      * \return True on success and false on failure.
      * \param newNumBlocks The new number of blocks to use.
      *
      */

      bool CPB_TexPart::CreateNewPtrArray(RwUInt32 newNumBlocks)
      {
         RWS_FUNCTION("RWS::Particle::CPB_TexPart::CreateNewPtrArray");

         if (!CPB_BasePart::CreateNewPtrArray(newNumBlocks))
         {
            RWS_RETURN(false);
         }

         // Do stuff for textured version...

         if ((m_ppNewTexAnimBlocks = ::RWS_NEW TexAnimData *[newNumBlocks]) == 0)
         {
            RWS_RETURN(false);
         }
         memset(m_ppNewTexAnimBlocks, 0, sizeof(TexAnimData *) * newNumBlocks);

         RWS_RETURN(true);
      }

      /**
      *
      * Same as base class function (which it calls) but does additional work for textured system.
      *
      * \see CPB_BasePart::CopyPtrArrayElement
      * \param index Block index to copy.
      *
      */

      void CPB_TexPart::CopyPtrArrayElement(RwUInt32 index)
      {
         RWS_FUNCTION("RWS::Particle::CPB_TexPart::CopyPtrArrayElement");

         CPB_BasePart::CopyPtrArrayElement(index);

         // Do stuff for textured version...

         m_ppNewTexAnimBlocks[index] = m_ppTexAnimBlocks[index];

         RWS_RETURNVOID();
      }

      /**
      *
      * Same as base class function (which it calls) but does additional work for textured system.
      *
      * \see CPB_BasePart::DeleteOldPtrArrayElement
      * \param index Block index to delete.
      *
      */

      void CPB_TexPart::DeleteOldPtrArrayElement(RwUInt32 index)
      {
         RWS_FUNCTION("RWS::Particle::CPB_TexPart::DeleteOldPtrArrayElement");

         CPB_BasePart::DeleteOldPtrArrayElement(index);

         // Do texture related stuff...
         
         delete (m_ppTexAnimBlocks[index]);

         RWS_RETURNVOID();
      }

      /**
      *
      * Same as base class function (which it calls) but does additional work for textured system.
      *
      * \see CPB_BasePart::CreateNewPtrArrayElement
      * \param index Block index to create.
      * \param blockSize Size of the block to allocate.
      * \return True on success and false on failure.
      *
      */

      bool CPB_TexPart::CreateNewPtrArrayElement(RwUInt32 index, RwUInt32 blockSize)
      {
         RWS_FUNCTION("RWS::Particle::CPB_TexPart::CreateNewPtrArrayElement");

         if (!CPB_BasePart::CreateNewPtrArrayElement(index, blockSize))
         {
            RWS_RETURN(false);
         }

         // Do stuff for textured version...

         // On PS2, need to allocate a block that is a multiple of 16Bytes in 
         // size, to allow the array to be sent to the scratchpad via DMA 
         // transfer. (DMA transfers are in blocks of 16Bytes).

         RwUInt32 uBytesToAlloc = ( ( ( sizeof(TexAnimData) * blockSize ) + 15 ) >> 4 ) << 4;

         if ((m_ppNewTexAnimBlocks[index] = reinterpret_cast<TexAnimData *>(::RWS_OP_NEW(uBytesToAlloc)) ) == 0)
         {
            RWS_RETURN(false);
         }

         RWS_RETURN(true);
      }

      /**
      *
      * Same as base class function (which it calls) but does additional work for textured system.
      *
      * \see CPB_BasePart::SwapInNewPtrArray
      *
      */

      void CPB_TexPart::SwapInNewPtrArray(void)
      {
         RWS_FUNCTION("RWS::Particle::CPB_TexPart::SwapInNewPtrArray");

         // Do base class first...

         CPB_BasePart::SwapInNewPtrArray();

         // Free up the old ones - if any...

         if (m_ppNewAnimBlocks)
         {
            delete[] m_ppNewAnimBlocks;
         }

         // Swap in the new ones...

         m_ppTexAnimBlocks = m_ppNewTexAnimBlocks;
         m_ppNewTexAnimBlocks = 0;

         RWS_RETURNVOID();
      }

      /**
      *
      * Sets up the new particle data based on the reference data passed in. Only does information for the
      * textured particle - does NOT call the base class to perform other setup (caller must do this). Does NOT
      * do a complete setup, you need to call the 'DoTexPartSetup' and 'DoTexAnimSetup' functions too.
      * This function modifies the pointers within the 'addAccess' structure to correctly position the 'current'
      * values for the add. The index value within the 'addAccess' object must have been setup to point to the
      * required element.
      *
      * \see CPB_BasePart::SecEmitSetup
      * \see DoStdPartSetup
      * \see DoPartAnimSetup
      * \param addAccess The access structure indicating the particle to be used for the setup.
      * \param access The access structure indicating the particle to be used for reference.
      * \param partAxis The particle axis information used to setup the new particles direction of movement.
      *
      */

      void CPB_TexPart::SecEmitSetup(TexAccess &addAccess, TexAccess &access, CPB_Marker &addMarker)
      {
         RWS_FUNCTION("RWS::Particle::CPB_TexPart::SecEmitSetup");

         // Do texture setup...

         addAccess.m_pTexUVs = reinterpret_cast<RwV2d *>(addAccess.m_texUVsLock.data +
               addAccess.m_texUVsLock.stride * addMarker.index);
         addAccess.m_pTexAnim = addAccess.m_pTexAnimBlock + addMarker.index;

         addAccess.m_pTexUVs[0] = access.m_pTexUVs[0];
         addAccess.m_pTexUVs[1] = access.m_pTexUVs[1];
         *addAccess.m_pTexAnim = *access.m_pTexAnim;

         RWS_RETURNVOID();
      }

   }     //namespace Particle
}        //namespace RWS
