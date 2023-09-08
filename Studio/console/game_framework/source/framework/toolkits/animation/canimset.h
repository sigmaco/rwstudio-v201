/*****************************************************************************
 *
 * File :     CAnimSet.h
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

#ifndef __CANIMSET_H__
#define __CANIMSET_H__
                  
//////////////////////////////////////////////////////////////////
//
// Standard Includes
//
#include <vector>

//////////////////////////////////////////////////////////////////
//
// RenderWare Includes
//
#include <rwcore.h>
#include <rphanim.h>
#include <rtcmpkey.h>


#ifdef _MSC_VER
#pragma comment (lib, "rwcore.lib")
#pragma comment (lib, "rphanim.lib")
#endif

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "framework/core/attributehandler/csystemcommands.h"

namespace RWS
{
   // Fwd. decls
   class CAttributePacket;

   /**
   *
   *  \ingroup Animation
   *
   *  CAnimSet allows a set of RpHAnim animations to be applied to a specified target.
   *  It supports simple animation and blending, and allows basic customization of the
   *  way each animation gets applied.
   *  
   *  Animations can be added to the set explicitly, using 'AddAnimation()', or implicitly
   *  based on the contents of an attribute packet, using 'HandleSystemCommands()'.
   *
   *  To schedule animations for playback on a target, the target first needs to be
   *  specified using 'SetTarget()' the target is a RenderWare Gfx RpClump object with
   *  a valid animation hierarchy.
   *
   *  Animations are scheduled for playback using 'SetDstAnimation()'. Animation blending
   *  is achieved by blending from a souce animation to the destination animation over a 
   *  period of time.
   *
   */
   class CAnimSet : public CSystemCommands
   {
   public:

      // RWS_POST( !HasTarget() )
      // RWS_POST( !IsAnimating() )
      // RWS_POST( RwReal(0) == GetBlendDuration() )
      CAnimSet();
      ~CAnimSet();

      bool HasTarget() const;

      // RWS_PRE( !HasTarget() )
      bool SetTarget( RpClump& target );

      void HandleSystemCommands( const CAttributePacket& attr );

      void AddAnimation( const RWSGUID * pResourceId );

      //RWS_PRE( HasTarget() )
      //RWS_PRE( IsBlendIntoNext() )
      /**
      *
      *  \return Returns true if the anim set is playing an animation on the target, used to determine
      *  if the current animation should blend into the start of the next animation. i.e. if an animation
      *  is player IsBlendIntoNext is true, if a second animation is requested then a blend from the 
      *  first to the second is performed.
      *
      *  \see SetBlendIntoNext
      */
      bool IsBlendIntoNext() const {return bBlendIntoNext_;}

      /**
      *
      *  Set the is Animating flag, \see IsBlendIntoNext. If an animation is completed i.e. the HOLD
      *  flag is set, and Update returns FALSE, the next requested animation will still force a blend
      *  from the end if this animation into the new animation. Setting SetBlendIntoNext to false prevents
      *  this blend operation.
      *
      * SetBlendIntoNext(true)
      *
      *  |--------------| Anim 1
      *           |~~~Blend~~~|--------------| Anim 2
      * 
      *
      *  |--------------| Anim 1
      *                 |~~~Blend~~~|--------------| Anim 2
      *
      *
      * SetBlendIntoNext(false)
      *
      *  |--------------| Anim 1
      *                 |--------------| Anim 2
      *
      *  \param flag If flag is TRUE then blend into next is enabled, otherwise disabled.
      */
      void SetBlendIntoNext(bool flag) {bBlendIntoNext_ = flag;}

      /**
      *
      *  \return Returns true if the current animation time is less than the duration of
      *  the animation, otherwise false.
      *
      */
      bool IsAnimating() const {return isAnimating_;}

      /**
      *
      *  \return Returns true if the anim set is blending from one animation to the next
      *
      */
      bool IsBlending() const {return isBlending_;}

      /**
      *  \return Returns the number of animations in the set
      */
      RwUInt32 GetAnimationCount() const {return animData_.size();}

      // RWS_POST( result < GetAnimationCount() )
      RwUInt32 GetDstAnimation() const;

      // RWS_POST( result < GetAnimationCount() )
      RwUInt32 GetSrcAnimation() const;

      // RWS_PRE( HasTarget() )
      // RWS_PRE( animIndex < GetAnimationCount() )
      void SetDstAnimation( RwUInt32 animIndex );

      // RWS_PRE( HasTarget() )
      RwBool GetDstAnimationByName( const RwChar *animName, RwInt32 *animIndex);

      // RWS_PRE( animIndex < GetAnimationCount() )
      void SetRateMultiplier( RwUInt32 animIndex, RwReal rate );
      // RWS_PRE( animIndex < GetAnimationCount() )
      RwReal GetRateMultiplier( RwUInt32 animIndex ) const;

      enum EndBehavior { LOOP, HOLD };
      // RWS_PRE( animIndex < GetAnimationCount() )
      void SetEndBehavior( RwUInt32 animIndex, EndBehavior behavior );
      // RWS_PRE( animIndex < GetAnimationCount() )
      EndBehavior GetEndBehavior( RwUInt32 animIndex ) const;

      // RWS_PRE( duration >= RwReal(0) )
      void SetBlendDuration( RwReal duration );
      // RWS_POST( result >= RwReal(0) )
      RwReal GetBlendDuration() const;

      // RWS_PRE( timeDelta >= RwReal(0) )
      bool Update( RwReal timeDelta );

   protected:

      // Revoked ops
      CAnimSet( const CAnimSet& );
      CAnimSet& operator = ( const CAnimSet& );

      // Implementation
      void InitiateBlend( RpHAnimHierarchy& currDstHierarchy, RwUInt32 newDstAnimIndex );

      //RWS_PRE( HasTarget() )
      void TerminateBlend();
      
      bool UpdateUnblended( RwReal timeDelta );
      bool UpdateBlended( RwReal timeDelta );

      bool UpdateHierarchy( RpHAnimHierarchy& hierarchy, RwUInt32 animIndex, RwReal timeDelta );
      const RpHAnimAnimation& GetRawAnimationData( RwUInt32 animIndex ) const;
      bool IsCompatibleWithAllAnims( const RpHAnimHierarchy& hierarchy );

      // Data
      struct AnimData
      {
         AnimData()
            : pRawData_(0), rateMultiplier_(RwReal(1)), endBehavior_(LOOP), pName_(RWSTRING(""))
         {}

         AnimData( const RpHAnimAnimation* pRawData, const RwChar* pName )
            : pRawData_(pRawData), rateMultiplier_(RwReal(1)), endBehavior_(LOOP), pName_(pName)
         {}

         const RpHAnimAnimation* pRawData_;
         RwReal rateMultiplier_;
         EndBehavior endBehavior_;
         const RwChar *pName_;
      };

      typedef std::vector<AnimData> HAnimDataVec;
      
      HAnimDataVec animData_;

      RpHAnimHierarchy* pMainHierarchy_;
      RpHAnimHierarchy* pBlendSrcHierarchy_;
      RpHAnimHierarchy* pBlendDstHierarchy_;

      RwReal blendDuration_;
      RwReal elapsedBlendTime_;

      RwUInt32 dstAnimIndex_;
      RwUInt32 srcAnimIndex_;

      bool bBlendIntoNext_;
      bool isBlending_;
      bool isAnimating_;
   };
}

#endif
