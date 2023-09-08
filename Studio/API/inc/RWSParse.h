/*****************************************************************************
 *
 * File :     RWSParse.h
 *
 * Abstract:  RenderWare Studio Manager API Toolkit - Source Parsing
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
 * Copyright © 2002 Criterion Software Ltd. All rights reserved.
 * All Rights Reserved.
 *
 *****************************************************************************/


#ifndef CSL_RWS_SOURCE_PARSER
#define CSL_RWS_SOURCE_PARSER

#if _MSC_VER > 1000
#ifdef RWSPARSE_EXPORTS
#define RWSPARSE_API __declspec(dllexport)
#else
#define RWSPARSE_API __declspec(dllimport)
#endif
#endif /* _MSC_VER > 1000 */

#include "RWSCore.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * \ingroup GSourceParser
 * \typedef RWSParserErrorCallback
 * Callback function which gets called whenever an error occurs during the
 * parsing of a file.
 * \param UserData User specific data.
 * \param szFileName The filename of the file being parsed.
 * \param nLineNo The line number the error was detected on.
 * \param szError A description of the error.
 * \param szSyntax The text that was detected where the error occurred.
 */
typedef void (*RWSParserErrorCallback)(RWSUserData UserData,
                 const RWSChar * const szFileName, RWSUInt32 nLineNo,
                 const RWSChar * const szError, const RWSChar * const szSyntax);

RWSPARSE_API void RWSParserInitialize (RWSID ParentID);
RWSPARSE_API void RWSParserFileLoad (const RWSChar * const szFilename);
RWSPARSE_API void RWSParserErrorSetCallback (RWSUserData UserData,
                                          RWSParserErrorCallback ErrorCallback);


/* Backwards compatibility macro */
#define RWSFileParse RWSParserFileLoad

#ifdef __cplusplus
}
#endif


#endif /* CSL_RWS_SOURCE_PARSER */
