/* NOTE : this header has no guards and is MEANT for multiple inclusion !
 * If you are using "header protection" option with your compiler,
 * please also find #pragma which disables it and put it here, to
 * allow reentrancy of this header.
 */

/* If the platform provides any specific epilog actions,
   like #pragmas, do include platform-specific prolog file */
# if defined (_STLP_HAS_SPECIFIC_PROLOG_EPILOG)
#  include <config/_epilog.h>
# endif

# ifndef _STLP_NO_POST_COMPATIBLE_SECTION
#  include <stl/_config_compat_post.h>
# endif

/* RW3 RenderWare3.7: Fixed the default behaviour in stlport 4.6 not to redefine the namespace. */
#  if defined(_STLP_USE_NAMESPACES) && (defined(_STLP_USE_OWN_NAMESPACE) && defined(_STLP_REDEFINE_STD))
/*  We redefine "std" to "stlport", so that user code may use std:: transparently */
#   undef  std
#   define std STLPORT
# else
# if defined(__cplusplus)
#  ifndef _STLP_CONFIG_H
#   include <stl/_config.h>
#  endif

# endif /* __cplusplus */
# endif








