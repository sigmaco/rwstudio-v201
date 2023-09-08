#ifndef _RWXMLDLL_H
#define _RWXMLDLL_H


#ifdef RWXML_DLL

#ifdef RWXML_EXPORTS
#define RWXML_API __declspec(dllexport)
#else
#define RWXML_API __declspec(dllimport)
#endif /* RWXML_EXPORTS */

#else

#define RWXML_API

#endif /* RWXML_DLL */


#endif /*_RWXMLDLL_H*/

