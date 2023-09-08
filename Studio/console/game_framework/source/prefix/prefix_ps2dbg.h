#include <prefix_ps2_debug_tc296.h>
#include <prefix_ps2_rw.h>

#pragma	divbyzerocheck		on	/*	break if divided by zero	*/

#if !defined(DEBUG)
#define	DEBUG					/*	just for debugging	*/
#endif
#define _DEBUG

#define	RWDEBUG		1
#define RWADEBUG    1

#define RWS_DISABLE_MEMORY_CHECKING
