//	===========================================================================
//	PREFIX_PS2_RW.h			©2000 Criterion Software Ltd. All rights reserved.
//	===========================================================================
//

#define	SKY
#define	PS2



// Select video mode
//
//#define VIDEO_MODE_PAL
#define VIDEO_MODE_NTSC

// There's a problem wherever #include <stddef.h> comes after #include "rwcore.h". It
// generates compile warnings complaining about the redefinition of 'offsetof'. GCC
// <stddef.h> has already been fixed (see http://gcc.gnu.org/ml/gcc-patches/2001-04/msg01412.html).
// However, it's a very recent fix and we're unlikely to see it in our GCC source for
// some time, hence this (hopefully temporary) workaround... 
//
#ifdef __need_size_t 
#undef __need_size_t
#endif
#ifdef __need_ptrdiff_t
#undef __need_ptrdiff_t
#endif

#define RW_USE_SPF   // If this is defined maths funcs use single not double precision

//#define WITH_AUDIO   // Enable Renderware Audio Integration