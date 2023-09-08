#include "stdafx.h"
#include "RenderWareAnim.h"
#include "gameframeworkanim/skinhelper.h"
#include "gameframeworkanim/animhelper.h"
#include "gameframeworkanim/skinsafe.h"

namespace RW
{
   ////////////////////////////////////////////////////////////////////////////
   //
   RenderWareAnim::RenderWareAnim () : m_pMainHierarchy (0), m_pClump (0),
                                       m_bHasAnim (FALSE), m_bHasMorph (FALSE)
   {
      m_MorphParameters.duration = 0;
      m_MorphParameters.numInterp = 0;
      m_MorphParameters.numMorph = 0;
      m_MorphParameters.position = 0;
   }

   ////////////////////////////////////////////////////////////////////////////
   //
   RenderWareAnim::~RenderWareAnim ()
   {
   }

   ////////////////////////////////////////////////////////////////////////////
   //
   RwBool RenderWareAnim::SetClump (RpClump* pClump)
   {
      // Clean up old hierarchy and animation
      m_pMainHierarchy = 0;

      if (m_pClump = pClump) // Assignment intentional
      {
         RwFrame* pClumpFrame = RpClumpGetFrame (pClump);
         RWS::FindHAnimHierarchyCallback (pClumpFrame, &m_pMainHierarchy);
         if (m_pMainHierarchy)
         {
            // Make sure that it's set to the default pose (if any)
            if (RpSkin* pFirstSkin = RWS::SkinHelper::FindFirstSkin (*pClump))
               RWS::ApplyDefaultPoseToHierarchy (*m_pMainHierarchy, *pFirstSkin);

            // Set flags to update all matrices 
            RpHAnimHierarchyFlag Flags = RpHAnimHierarchyGetFlags (m_pMainHierarchy);
            RpHAnimHierarchySetFlags (m_pMainHierarchy,
                                      RpHAnimHierarchyFlag(Flags |
                                      rpHANIMHIERARCHYUPDATELTMS |
                                      rpHANIMHIERARCHYUPDATEMODELLINGMATRICES));

            // Attach the hierarchy to the RwFrame hierarchy 
            RpHAnimHierarchyAttach (m_pMainHierarchy);

            // Now plug all the clump's skins into the hierarchy
            RWS::AttachHAnimHierarchy (*pClump, *m_pMainHierarchy);
         }

         m_bHasMorph = ClumpSetupInterpolators (pClump);
      }

      return HasClump ();
   }

   ////////////////////////////////////////////////////////////////////////////
   //
   RwBool RenderWareAnim::HasClump () const
   {
      return (m_pClump != 0);
   }

   ////////////////////////////////////////////////////////////////////////////
   //
   RwBool RenderWareAnim::SetAnimation (const RwChar* szFileName)
   {
      if (HasClump () && szFileName)
      {
         RtAnimAnimation * pAnim = 0;

         // Try to read the anim from a stream. Calling RtAnimAnimationRead seems
         // to keep the animation file locked.
         if (RwStream* pStream = RwStreamOpen (rwSTREAMFILENAME, rwSTREAMREAD, szFileName))
         {
            if (RwStreamFindChunk (pStream, rwID_HANIMANIMATION, 0, 0))
               pAnim = RtAnimAnimationStreamRead (pStream);

            RwStreamClose (pStream, 0);
         }

         if (pAnim)
         {
            // Incompatible animations will cause RenderWare to crash.
            if (RWS::AnimationIsCompatibleWithHierarchy (*pAnim, *m_pMainHierarchy))
            {
               // Set the new animation and destroy the old one.
               RtAnimAnimation* pOldAnim = RpHAnimHierarchyGetCurrentAnim (m_pMainHierarchy);
               RpHAnimHierarchySetCurrentAnim (m_pMainHierarchy, pAnim);
               RtAnimAnimationDestroy (pOldAnim);
               m_bHasAnim = TRUE;
               return TRUE;
            }
            else
            {
               RtAnimAnimationDestroy (pAnim);
            }
         }
      }

      return FALSE;
   }

   ////////////////////////////////////////////////////////////////////////////
   //
   RwBool RenderWareAnim::HasAnimation ()
   {
      return (m_bHasAnim || m_bHasMorph);
   }

   RwBool RenderWareAnim::HasMorph()
   {
      return (m_bHasMorph);
   }


   ////////////////////////////////////////////////////////////////////////////
   //
   RwBool RenderWareAnim::SetAnimationTimeIncrement (RwReal Time, RwBool* pbComplete)
   {
      if (m_bHasAnim)
      {
         RtAnimAnimation * pAnim = m_pMainHierarchy ? RpHAnimHierarchyGetCurrentAnim (m_pMainHierarchy) : 0;

         // Add time to HAnimation.
         if (pAnim)
         {
            if ((m_pMainHierarchy->currentAnim->currentTime + Time) >= pAnim->duration)
            {
               if (pbComplete)
                  *pbComplete = TRUE;
            }
            else if (pbComplete)
               *pbComplete = FALSE;

            RpHAnimHierarchyAddAnimTime (m_pMainHierarchy, Time);
            RpHAnimHierarchyUpdateMatrices (m_pMainHierarchy);
         }
      }
      // Add time to morph anim.
      else if (m_bHasMorph && m_pClump)
      {
         MorphIncTime Mit = {Time, FALSE};
         RpClumpForAllAtomics (m_pClump, AtomicMorphAddTime,
                               reinterpret_cast <void*>(&Mit));

         if (pbComplete)
            *pbComplete = Mit.m_bComplete;
      }

      return TRUE;
   }

   ////////////////////////////////////////////////////////////////////////////
   //
   RwBool RenderWareAnim::SetAnimationTime (RwReal Time)
   {
      // Set Hanim time.
      if (m_bHasAnim)
      {         
         RtAnimAnimation * pAnim = m_pMainHierarchy ? RpHAnimHierarchyGetCurrentAnim (m_pMainHierarchy) : 0;

         if (pAnim)
         {
            RpHAnimHierarchySetCurrentAnimTime (m_pMainHierarchy, Time);
            RpHAnimHierarchyUpdateMatrices (m_pMainHierarchy);            
         }
      }
      // Set Morph time.
      else if (m_bHasMorph && m_pClump)
      {
         m_MorphParameters.position = 0;

         MorphIncTime Mit = {Time, FALSE};
         RpClumpForAllAtomics (m_pClump, AtomicMorphSetTime, reinterpret_cast <void*>(&Mit));
      }
     
      return TRUE;
   }

   ////////////////////////////////////////////////////////////////////////////
   //
   RwReal RenderWareAnim::GetAnimationDuration( void )
   {
      RwReal Duration = 0.0f;

      // Get duration of HAnim.
      if (m_bHasAnim)
      {
         RtAnimAnimation * pAnim = m_pMainHierarchy ?
                            RpHAnimHierarchyGetCurrentAnim (m_pMainHierarchy) : 0;
         if (pAnim)
         {
            Duration = pAnim->duration;
         } 
      }
      // Get duration for morph
      else if (m_bHasMorph && m_pClump)
      {
         m_MorphParameters.duration = 0;
         RpClumpForAllAtomics (m_pClump, AtomicMorphGetDuration,reinterpret_cast<void*>(&m_MorphParameters));
         Duration = m_MorphParameters.duration;
      }

      return Duration;
   }

   ////////////////////////////////////////////////////////////////////////////
   //
   RwReal RenderWareAnim::GetAnimationTime( void )
   {
      RwReal Time = 0.0f;

      // Get HAnims current time.
      if (m_bHasAnim)
      {
         RtAnimAnimation * pAnim = m_pMainHierarchy ?
                            RpHAnimHierarchyGetCurrentAnim (m_pMainHierarchy) : 0;

         if (pAnim)
         {
            Time = m_pMainHierarchy->currentAnim->currentTime;
         }
      }
      // Get morphs current time.
      else if (m_bHasMorph && m_pClump)
      {
         m_MorphParameters.position = 0;

         RpClumpForAllAtomics(m_pClump, AtomicMorphGetPosition, reinterpret_cast<void*>(&m_MorphParameters));

         Time = m_MorphParameters.position;
      }

      return Time;
   }


   RpClump *RenderWareAnim::GetClump( void )
   {
      return m_pClump;
   }

}
