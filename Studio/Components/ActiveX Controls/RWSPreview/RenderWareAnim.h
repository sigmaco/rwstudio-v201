#ifndef __RENDERWAREANIM_H__
#define __RENDERWAREANIM_H__

#include <vector>

#include "RenderWareHelpers.h"

namespace RW
{

   class RenderWareAnim
   {
   public:
      RenderWareAnim ();
      ~RenderWareAnim ();

      // Sets the clump that will be animated
      RwBool SetClump (RpClump* pClump);
      inline RwBool HasClump () const;

      // Adds animations and sets animations active
      RwBool SetAnimation (const RwChar* szFileName);
      RwBool HasAnimation ();
      RwBool HasMorph();
      RwBool SetAnimationTimeIncrement (RwReal Time, RwBool* pbComplete);
      RwBool SetAnimationTime (RwReal Time);
      RwReal GetAnimationDuration( void );
      RwReal GetAnimationTime( void );
      RpClump *GetClump( void );
      RpHAnimHierarchy* m_pMainHierarchy;
      MorphParams       m_MorphParameters;

   protected:
      RwBool            m_bHasMorph; // Is the anim a morph.
      RwBool            m_bHasAnim;  // Is the anim a HANIM.
      RpClump*          m_pClump;    // Pointer to a clump.
   };
}

#endif // __RENDERWAREANIM_H__