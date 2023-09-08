/*****************************************************************************
 *
 * File :     CAnimSet.cpp
 *
 * Abstract : Provides basic animation services, including storage, update and
 *            simple blending.
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

//#define RWS_CALLSTACKENABLE
//#define RWS_TRACEENABLE

//////////////////////////////////////////////////////////////////
//
// RenderWare Includes
//
#include <rwcore.h>
#include <rphanim.h>
#include <rpskin.h>

#ifdef _MSC_VER
#pragma comment (lib, "rwcore.lib")
#pragma comment (lib, "RpHAnim.lib")
#pragma comment (lib, "RpSkin.lib")
#endif

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "canimset.h"
#include "animhelper.h"
#include "skinsafe.h"
#include "framework/toolkits/math/maths.h"
#include "framework/toolkits/world/helpers/clumphelper.h"
#include "framework/toolkits/world/helpers/skinhelper.h"
#include "framework/core/macros/debugmacros.h"
#include "framework/core/attributehandler/cattributehandler.h"
#include "framework/core/resourcemanager/cresourcemanager.h"


namespace
{
   using namespace RWS;

   /**
   *
   *  Clones an RpHAnimHierarchy, including its flags
   *
   *  \param cloneMe A reference to a RpHAnimHierarchy object/
   *  \return Returns if successful a pointer to the new RpHAnimHierarchy, otherwise 0.
   *
   */
   RpHAnimHierarchy* CloneRpHAnimHierarchy( const RpHAnimHierarchy& cloneMe )
   {
      RWS_FUNCTION("RWS::CloneRpHAnimHierarchy");

// RenderWare Version Precedes 3.4
//      
#if (rwLIBRARYCURRENTVERSION < 0x34002)
      RWS_RETURN( RpHAnimHierarchyCreateFromHierarchy(
         unconst(&cloneMe),
         static_cast<RpHAnimHierarchyFlag>(RpHAnimHierarchyGetFlags(unconst(&cloneMe))),
         cloneMe.maxKeyFrameSize
         ) );
// RenderWare Version 3.4xxx
//      
#elif (rwLIBRARYCURRENTVERSION >= 0x34002 && rwLIBRARYCURRENTVERSION < 0x35000)
      RWS_RETURN( RpHAnimHierarchyCreateFromHierarchy(
         const_cast<RpHAnimHierarchy*>(&cloneMe),
         static_cast<RpHAnimHierarchyFlag>(RpHAnimHierarchyGetFlags(const_cast<RpHAnimHierarchy*>(&cloneMe))),
         cloneMe.currentAnim->maxKeyFrameSize
         ) );
// RenderWare Version 3.5 and above
//      
#elif (rwLIBRARYCURRENTVERSION >= 0x35000)
      RWS_RETURN( RpHAnimHierarchyCreateFromHierarchy(
         const_cast<RpHAnimHierarchy*>(&cloneMe),
         static_cast<RpHAnimHierarchyFlag>(RpHAnimHierarchyGetFlags(const_cast<RpHAnimHierarchy*>(&cloneMe))),
         cloneMe.currentAnim->maxInterpKeyFrameSize
         ) );
#endif
   }

   /**
   *
   *  Configures an RpHAnimHierarchy to use a given set of animation data, starting
   *  at a given time.
   *  
   *  \param hierarchy Reference to a RenderWare Gfx RpHAnimHierarchy object, the animation hierarchy.
   *  \param anim Reference to a RenderWare Gfx RpHAnimAnimation object, the animation.
   *  \param startTime The start time used to determine where to start within the animation.
   *
   */
   void SetCurrentAnim( RpHAnimHierarchy& hierarchy, const RpHAnimAnimation& anim, RwReal startTime )
   {
      RWS_FUNCTION("RWS::SetCurrentAnim")

      RWS_PRE( AnimationIsCompatibleWithHierarchy(anim, hierarchy) )

      // RpHAnimHierarchySetCurrentAnim() resets the hierarchy time, so make sure the time is set
      // AFTER the anim...
      RpHAnimHierarchySetCurrentAnim( (&hierarchy), const_cast<RpHAnimAnimation*>(&anim) );
      RpHAnimHierarchySetCurrentAnimTime( (&hierarchy), startTime );

      RWS_RETURNVOID();
   }

   /**
   *
   *  Modifies a time increment so that the current time for an animation stays
   *  within bounds.
   *
   *  \param unClampedDeltaTime The unclamped delta time.
   *  \param currTime The current time.
   *  \param duration The duration of the animation.
   *
   *  \return Returns unClampedDeltaTime clamped so that when added to currTime the new 
   *  time is within 0 and duration.
   *
   */
   RwReal ClampedTimeDelta( RwReal unClampedDeltaTime, RwReal currTime, RwReal duration )
   {
      RWS_FUNCTION("RWS::ClampedTimeDelta")

      const RwReal demandedTargetTime = currTime + unClampedDeltaTime;
      const RwReal clampedTargetTime = Clamp( demandedTargetTime, RwReal(0), duration );

      RWS_RETURN(clampedTargetTime - currTime);
   }
}

namespace RWS
{
   /**
   *
   *  Default constructor for CAnimSet
   *
   */
   CAnimSet::CAnimSet()
      :  pMainHierarchy_(0),
         pBlendSrcHierarchy_(0),
         pBlendDstHierarchy_(0),
         blendDuration_(RwReal(0)),
         elapsedBlendTime_(RwReal(0)),
         dstAnimIndex_(0),
         srcAnimIndex_(0),
         bBlendIntoNext_(false),
         isBlending_(false),
         isAnimating_(false)
   {
      RWS_FUNCTION("RWS::CAnimSet::CAnimSet")

      RWS_POST( !HasTarget() );
      RWS_POST( RwReal(0) == GetBlendDuration() );

      RWS_RETURNVOID()
   }

   /**
   *
   *  Destructor for CAnimSet
   *
   */
   CAnimSet::~CAnimSet()
   {
      RWS_FUNCTION("RWS::CAnimSet::~CAnimSet")

      // Destroy cloned hierarchies, if any
      if ( pBlendSrcHierarchy_ ) RpHAnimHierarchyDestroy( pBlendSrcHierarchy_ );
      if ( pBlendDstHierarchy_ ) RpHAnimHierarchyDestroy( pBlendDstHierarchy_ );

      RWS_RETURNVOID()
   }

   /**
   *
   *  Check if we have a target, \see SetTarget.
   *
   *  \return Returns true if a target has been specified
   *
   */
   bool CAnimSet::HasTarget() const
   {
      RWS_FUNCTION("RWS::CAnimSet::HasTarget")
      RWS_RETURN(0 != pMainHierarchy_);
   }

   /**
   *
   *  Associates this CAnimSet with the RpClump which will display the animation.
   *  
   *  This can fail if the target doesn't have an RpHAnimHierarchy attached.
   *
   *  This needs to be done before the CAnimSet can be used to play any animations.
   *
   *  \param target Reference to a RenderWare Gfx RpClump object.
   *
   *  \return True if target is associated successfully, otherwise false.
   */
   bool CAnimSet::SetTarget( RpClump& target )
   {
      RWS_FUNCTION("RWS::CAnimSet::SetTarget")

      RWS_PRE( !HasTarget() )

      // Find any attached hierarchy (can't actually do a great deal without it)
      pMainHierarchy_ = FindHAnimHierarchy(target);

      if ( pMainHierarchy_)
      {
         // Make sure that it's set to the default pose (if any)
         RpSkin* pFirstSkin = SkinHelper::FindFirstSkin(target);

         if ( pFirstSkin) 
         {
            ApplyDefaultPoseToHierarchy(*pMainHierarchy_, *pFirstSkin);
         }

         /*
          * Set flags to update all matrices 
          */
         (void)RpHAnimHierarchySetFlags(pMainHierarchy_,
                                          (RpHAnimHierarchyFlag)
                                          ( RpHAnimHierarchyGetFlags(pMainHierarchy_) | 
                                          rpHANIMHIERARCHYUPDATELTMS |
                                          rpHANIMHIERARCHYUPDATEMODELLINGMATRICES) );

         /* 
          * Attach the hierarchy to the RwFrame hierarchy 
          */
         RpHAnimHierarchyAttach(pMainHierarchy_);        

         // Now plug all the clump's skins into the hierarchy
         RWS_ASSERT( IsCompatibleWithAllAnims(*pMainHierarchy_), "Incompatible hierarchy" );
         AttachHAnimHierarchy(target, *pMainHierarchy_);

         // Create copies of the hierarchy for blending src/dst
         pBlendSrcHierarchy_ = CloneRpHAnimHierarchy(*pMainHierarchy_);
         pBlendDstHierarchy_ = CloneRpHAnimHierarchy(*pMainHierarchy_);
      }

      RWS_RETURN(HasTarget())
   }

   /**
   *
   *  Handles system commands by looking for CMD_AttachResource commands and adding
   *  any specified animations to the set.
   *
   *  \param attr A reference to a CAttributePacket packet.
   *
   */
   void CAnimSet::HandleSystemCommands( const CAttributePacket& attr )
   {
      RWS_FUNCTION("RWS::CAnimSet::HandleSystemCommands")

      CAttributeCommandIterator attrIt(attr, RWS_CLASSID_OF(CSystemCommands));
      while (!attrIt.IsFinished())
      {
         // Intercept CMD_AttachResource's only
         if ( CMD_AttachResource == attrIt->GetCommandId() )
         {
            const RWSGUID * pResourceId;
            attrIt->GetCommandData(&pResourceId);
            AddAnimation(pResourceId);
         }

         // Next section
         ++attrIt;
      }

      RWS_RETURNVOID()
   }

   /**
   *
   *  Adds the specified animation to the set
   *
   *  \param ResourceID A pointer to a RwChar, containing the resource Id of the animation to attach.
   *
   */
   void CAnimSet::AddAnimation( const RWSGUID * pResourceId )
   {
      RWS_FUNCTION("RWS::CAnimSet::AddAnimation")

      // Look up the object in the global store
      const RwChar *ResourceType;
      const RwChar *ResourceName;
      const void *pObject = RWS::CResourceManager::FindById (pResourceId, &ResourceType, 0, &ResourceName);

      if (pObject && !rwstrcmp ("rwID_HANIMANIMATION", ResourceType))
      {
#ifdef _DEBUG
         // We found some RpHAnim anim data - assume it's compatible with any existing
         // target
         const RpHAnimAnimation* pAnim = static_cast<const RpHAnimAnimation*>(pObject);

         RWS_ASSERT(
            (!HasTarget() || AnimationIsCompatibleWithHierarchy(*pAnim, *pMainHierarchy_)),
            "Incompatible anim data or no target for animation."
            );
#endif
         // Push the anim into the collection (no need to take a copy - we have no
         // intention of altering the anim data)
         animData_.push_back(AnimData (static_cast<const RpHAnimAnimation*>(pObject), ResourceName));
      }

      RWS_RETURNVOID()
   }

   /**
   *
   *  Sets the rate multiplier for the specified animation. A multiplier of
   *  1.0f will play the animation at its default rate. Use a negative rate
   *  if reverse-running is required.
   *
   *  \param animIndex An index used to specify which animation we are setting
   *  the rate of.
   *
   *  \param rate The new rate value.
   *
   */
   void CAnimSet::SetRateMultiplier( RwUInt32 animIndex, RwReal rate )
   {
      RWS_FUNCTION("RWS::CAnimSet::SetRateMultiplier")
      
      RWS_PRE( animIndex < GetAnimationCount() )

      animData_[animIndex].rateMultiplier_ = rate;

      RWS_RETURNVOID()
   }

   /**
   *
   *  Gets the rate multiplier for the specified animation.
   *
   *  \param animIndex An index used to specify which animation we are getting
   *  the rate of.
   *
   *  \return Returns the rate of the animation \see SetRateMultiplier.
   */
   RwReal CAnimSet::GetRateMultiplier( RwUInt32 animIndex ) const
   {
      RWS_FUNCTION("RWS::CAnimSet::GetRateMultiplier")

      RWS_PRE( animIndex < GetAnimationCount() )

      RWS_RETURN(animData_[animIndex].rateMultiplier_);
   }

   /**
   *
   *  Determines what happens at the end of the specified anim. The possibilities
   *  are:
   *    LOOP - the anim loops back to the beginning (or end, if running reversed)
   *    HOLD - the anim maintains its final pose and any future call to SetDstAnimation()
   *           will blend into the new animation from this final pose.
   *
   *  \param animIndex An index used to specify which animation we are getting
   *  the rate of.
   *
   *  \param behavior The end behavior loop or hold.
   */
   void CAnimSet::SetEndBehavior( RwUInt32 animIndex, EndBehavior behavior )
   {
      RWS_FUNCTION("RWS::CAnimSet::SetEndBehavior")

      RWS_PRE( animIndex < GetAnimationCount() );

      animData_[animIndex].endBehavior_ = behavior;

      RWS_RETURNVOID()
   }


   /**
   *
   *  Gets the end behavior for the specified animation.
   *
   *  \param animIndex An index used to specify which animation we are getting
   *  the rate of.
   *
   *  \return Returns the end behavior either loop or hold.
   */
   CAnimSet::EndBehavior CAnimSet::GetEndBehavior( RwUInt32 animIndex ) const
   {
      RWS_FUNCTION("RWS::CAnimSet::GetEndBehavior")

      RWS_PRE( animIndex < GetAnimationCount() );

      RWS_RETURN(animData_[animIndex].endBehavior_);
   }

   /**
   *
   *  Gets the current 'destination' animation
   *
   *  \return Returns the index of the destination animation.
   */
   RwUInt32 CAnimSet::GetDstAnimation() const
   {
      RWS_FUNCTION("RWS::CAnimSet::GetDstAnimation")

      const RwUInt32 result = dstAnimIndex_;
      RWS_POST( result < GetAnimationCount() )

      RWS_RETURN(result);
   }

   /**
   *
   *  Gets the current 'source' animation
   *
   *  \return Returns the index of the source animation.
   */
   RwUInt32 CAnimSet::GetSrcAnimation() const
   {
      RWS_FUNCTION("RWS::CAnimSet::GetSrcAnimation")

      const RwUInt32 result = srcAnimIndex_;
      RWS_POST( result < GetAnimationCount() )

      RWS_RETURN(result);
   }

   /**
   *
   *  Gets the current 'destination' animation index (animIndex) from the given name (animName)
   *  Returns TRUE if successful.
   *  If the name is an empty string or the name is not found in the animation list
   *  then the result is FALSE.
   *
   *  \param animName A string containing the name of the animation.
   *
   *  \param animIndex A pointer to a RwInt32 filled in with the index of the animation.
   *
   *  \return Returns TRUE if the animation was found and animIndex was filled in with the
   *  index of the animation, otherwise FALSE.
   */
   RwBool CAnimSet::GetDstAnimationByName( const RwChar *animName, RwInt32 *animIndex )
   {
      RWS_FUNCTION("RWS::CAnimSet::GetDstAnimationByName")

      RWS_PRE( animName )
      RWS_PRE( animIndex )
      RWS_PRE( HasTarget() )

      RwUInt32 i = 0;
      HAnimDataVec::iterator ittr;

      // Iterate over all animations
      for (ittr = animData_.begin(); ittr != animData_.end(); ittr++)
      {
         if (ittr->pName_)
         {
            // Find matching anim
            if (rwstrcmp(animName, ittr->pName_) == 0)
            {
               // return the animation index
               *animIndex = i;
               RWS_RETURN(TRUE)
            }
         }

         i++;
      }

      // No matching animation so return error code
      RWS_RETURN(FALSE)
   }

   /**
   *
   *  Schedules an animation for playback. A newly scheduled animation will play
   *  immediately if no other animation is already playing, otherwise a blend will
   *  take place from the existing animation (referred to as the 'source' for the
   *  duration of the blend) to the new animation (referred to as the 'destination').
   *  Once blending is finished, the source is evicted and playback continues using
   *  only the destination.
   *
   *  \param animIndex An index used to specify the animation.
   */
   void CAnimSet::SetDstAnimation( RwUInt32 animIndex )
   {
      RWS_FUNCTION("RWS::CAnimSet::SetDstAnimation")

      RWS_PRE( HasTarget() )
      RWS_PRE( animIndex < GetAnimationCount() )

      if ( IsBlendIntoNext() )
      {
         if ( IsBlending() ) InitiateBlend( *pBlendDstHierarchy_, animIndex );
         else InitiateBlend( *pMainHierarchy_, animIndex );
      }
      else
      {
         SetCurrentAnim( *pMainHierarchy_, GetRawAnimationData(animIndex), RwReal(0) );
         dstAnimIndex_ = animIndex;
         SetBlendIntoNext(true);
      }

      RWS_RETURNVOID()
   }

   /**
   *
   *  Updates any animations that are being played back on the target. Call this as
   *  often as possible for the smoothest animation (ideally every frame).
   *
   *  \param timeDelta Delta time, time step used to advance the animation.
   *
   *  \return TRUE if the animation is still playing, FALSE if the animation has ended.
   */
   bool CAnimSet::Update( RwReal timeDelta )
   {
      RWS_FUNCTION("RWS::CAnimSet::Update")

      RWS_PRE( timeDelta >= RwReal(0) );

      bool ret = false;

      if ( IsBlendIntoNext() )
      {
         if ( IsBlending() )
         {
            elapsedBlendTime_ += timeDelta;
            if ( elapsedBlendTime_ >= blendDuration_ )
            {
               TerminateBlend();

               ret = UpdateUnblended(timeDelta);
            }
            else
            {
               ret = UpdateBlended(timeDelta);
            }
         }
         else ret = UpdateUnblended(timeDelta);

         RpHAnimHierarchyUpdateMatrices(pMainHierarchy_);
      }

      RWS_RETURN(ret)
   }

   /**
   *
   *  Updates the playback of an unblended animation.
   *
   *  \param timeDelta Delta time, time step used to advance the animation.
   *
   *  \return TRUE if the animation is still playing, FALSE if the animation has ended.
   *
   *  \note Used by Update.
   */
   bool CAnimSet::UpdateUnblended( RwReal timeDelta )
   {
      RWS_FUNCTION("RWS::CAnimSet::UpdateUnblended")

      RWS_PRE( timeDelta >= RwReal(0) )
      RWS_PRE( HasTarget() )
      RWS_PRE( !IsBlending() )

      bool ret = UpdateHierarchy( *pMainHierarchy_, dstAnimIndex_, timeDelta );

      RWS_RETURN(ret)
   }

   /**
   *
   *  Updates the playback of a blended animation.
   *
   *  \param timeDelta Delta time, time step used to advance the animation.
   *
   *  \return TRUE if the animation is still playing, FALSE if the destination animation has ended.
   *
   *  \note Used by Update.
   */
   bool CAnimSet::UpdateBlended( RwReal timeDelta )
   {
      RWS_FUNCTION("RWS::CAnimSet::UpdateBlended")

      RWS_PRE( timeDelta >= RwReal(0) )
      RWS_PRE( HasTarget() )
      RWS_PRE( IsBlending() )
      RWS_PRE( elapsedBlendTime_ < blendDuration_ );

      // Update both inputs and blend 'em
      UpdateHierarchy( *pBlendSrcHierarchy_, srcAnimIndex_, timeDelta );

      bool ret = UpdateHierarchy( *pBlendDstHierarchy_, dstAnimIndex_, timeDelta );

      RpHAnimHierarchyBlend( pMainHierarchy_, pBlendSrcHierarchy_, pBlendDstHierarchy_, 
                                                         elapsedBlendTime_ / blendDuration_ );

      RWS_RETURN(ret)
   }

   /**
   *
   *  Updates the given RpHAnimHierarchy with the given animation data.
   *
   *  \param hierarchy A pointer to the RenderWare Gfx RpHAnimHierarchy object.
   *  \param animIndex An index used to select the animation.
   *  \param timeDelta A delta time value used to advance the animation.
   *
   *  \return TRUE if the animation is still playing, FALSE if the animation has ended.
   *
   */
   bool CAnimSet::UpdateHierarchy( RpHAnimHierarchy& hierarchy, RwUInt32 animIndex, RwReal timeDelta )
   {
      RWS_FUNCTION("RWS::CAnimSet::UpdateHierarchy")
         
      RWS_PRE( timeDelta >= RwReal(0) )

      isAnimating_ = TRUE;

      // Apply rate multiplier
      const RwReal rateAdjustedTimeDelta = timeDelta * GetRateMultiplier(animIndex);

      // Clamp, if necessary
      RwReal clampedTimeDelta = rateAdjustedTimeDelta;
      switch (GetEndBehavior(animIndex))
      {
      case HOLD:
         {
            const RpHAnimAnimation& animData = GetRawAnimationData(animIndex);

// RenderWare Version Precedes 3.4
//      
#if (rwLIBRARYCURRENTVERSION < 0x34002)
            clampedTimeDelta = ClampedTimeDelta( rateAdjustedTimeDelta, hierarchy.currentTime, animData.duration );
// RenderWare Version Exceeds 3.4
//      
#elif (rwLIBRARYCURRENTVERSION >= 0x34002)
            clampedTimeDelta = ClampedTimeDelta( rateAdjustedTimeDelta, hierarchy.currentAnim->currentTime, animData.duration );
#endif

            // Animation is complete
            //
            if (clampedTimeDelta == RwReal(0))
            {
               isAnimating_ = FALSE;
            }
         }
         break;
      case LOOP:
         // Unclamped delta is fine - RpHAnim handles the looping for us
         break;
      default:
         RWS_ASSERT( false, "Unknown behaviour for animation index [" << animIndex <<
               "]. Need to add support for new end behavior.");
         break;
      }

// RenderWare Version Precedes 3.4
//      
#if (rwLIBRARYCURRENTVERSION < 0x34002)
      if ( clampedTimeDelta >= RwReal(0) ) RpHAnimHierarchyAddAnimTime(&hierarchy, clampedTimeDelta);
      else RpHAnimHierarchySubAnimTime(&hierarchy, ( - clampedTimeDelta));
// RenderWare Version Exceeds 3.4
//      
#elif (rwLIBRARYCURRENTVERSION >= 0x34002)
      if ( clampedTimeDelta >= RwReal(0) ) RpHAnimHierarchyAddAnimTime((&hierarchy), clampedTimeDelta);
      else RpHAnimHierarchySubAnimTime((&hierarchy), ( - clampedTimeDelta));
#endif


      RWS_RETURN(isAnimating_)
   }

   /**
   *
   *  Sets the duration of any blending between successive animations. Blend duration
   *  is taken to be specified in client time (i.e. before any rate multiplier is applied).
   *  Use a blend duration of 0.0f if no blending is required
   *
   *  \param Set the duration of the blend operation.
   */
   void CAnimSet::SetBlendDuration( RwReal duration )
   {
      RWS_FUNCTION("RWS::CAnimSet::SetBlendDuration")

      RWS_PRE( duration >= RwReal(0) );

      blendDuration_ = duration;

      RWS_RETURNVOID()
   }

   /**
   *
   *  Gets the duration of any blending between successive animations.
   *
   *  \return Returns the duration of the blend operation.
   */
   RwReal CAnimSet::GetBlendDuration() const
   {
      RWS_FUNCTION("RWS::CAnimSet::GetBlendDuration")

      const RwReal result = blendDuration_;
      RWS_POST( result >= RwReal(0) );

      RWS_RETURN(result);
   }

   /**
   *
   *  Initiates a blend from the current destination animation to some specified replacement.
   *
   *  \param currDstHierarchy Reference to a RenderWare Gfx RpHAnimHierarchy object, the animation hierarchy.
   *
   *  \param newDstAnimIndex An index used to specify the animation.
   */
   void CAnimSet::InitiateBlend( RpHAnimHierarchy& currDstHierarchy, RwUInt32 newDstAnimIndex )
   {
      RWS_FUNCTION("RWS::CAnimSet::InitiateBlend")

      RWS_PRE( HasTarget() )
      RWS_PRE( IsBlendIntoNext() )

      // Src

// RenderWare Version Precedes 3.4
//      
#if (rwLIBRARYCURRENTVERSION < 0x34002)
      SetCurrentAnim( *pBlendSrcHierarchy_, GetRawAnimationData(dstAnimIndex_), currDstHierarchy.currentTime );
// RenderWare Version Exceeds 3.4
//      
#elif (rwLIBRARYCURRENTVERSION >= 0x34002)
      SetCurrentAnim( *pBlendSrcHierarchy_, GetRawAnimationData(dstAnimIndex_), currDstHierarchy.currentAnim->currentTime );
#endif

      srcAnimIndex_ = dstAnimIndex_;
      isBlending_ = true;
      elapsedBlendTime_ = RwReal(0);

      // Dst
      SetCurrentAnim( *pBlendDstHierarchy_, GetRawAnimationData(newDstAnimIndex), RwReal(0) );
      dstAnimIndex_ = newDstAnimIndex;

      RWS_POST( IsBlendIntoNext() )
      RWS_POST( IsBlending() )

      RWS_RETURNVOID()
   }

   /**
   *
   *  If a blend operation is in progress, terminates the operation, this may cause the animation to jump 
   *  depending on the animations and the point in the blend.
   *
   */
   void CAnimSet::TerminateBlend()
   {
      RWS_FUNCTION("RWS::CAnimSet::TerminateBlend")

      RWS_PRE( HasTarget() )

      if (IsBlending())
      {
// RenderWare Version Precedes 3.4
//      
#if (rwLIBRARYCURRENTVERSION < 0x34002)
         SetCurrentAnim( *pMainHierarchy_, GetRawAnimationData(dstAnimIndex_), pBlendDstHierarchy_->currentTime );
// RenderWare Version Exceeds 3.4
//      
#elif (rwLIBRARYCURRENTVERSION >= 0x34002)
         SetCurrentAnim( *pMainHierarchy_, GetRawAnimationData(dstAnimIndex_), pBlendDstHierarchy_->currentAnim->currentTime );
#endif

         isBlending_ = false;
      }

      RWS_RETURNVOID() 
   }

   /**
   *
   *  Retrieves raw animation data for the specified animation.
   *
   */
   const RpHAnimAnimation& CAnimSet::GetRawAnimationData( RwUInt32 animIndex ) const
   {
      RWS_FUNCTION("RWS::CAnimSet::GetRawAnimationData")

      RWS_PRE( animIndex < GetAnimationCount() )
      RWS_RETURN (*animData_[animIndex].pRawData_);
   }

   /**
   *
   *  Returns true if the given RpHAnimHierarchy is compatible with all animations
   *  already in the set.
   *
   *  \param hierarchy Reference to a RenderWare Gfx RpHAnimHierarchy object, the animation
   *  hierarchy.
   *
   *  \return Returns TRUE if all attached animations are compatible with this hierarchy,
   *  otherwise FALSE.
   */
   bool CAnimSet::IsCompatibleWithAllAnims( const RpHAnimHierarchy& hierarchy )
   {
      RWS_FUNCTION("RWS::CAnimSet::IsCompatibleWithAllAnims")

      bool isCompatible = true;
      for ( RwUInt32 i = 0; i < GetAnimationCount() && isCompatible; ++i )
      {
          isCompatible = AnimationIsCompatibleWithHierarchy( GetRawAnimationData(i), hierarchy );
      }

      RWS_RETURN(isCompatible)
   }
}
