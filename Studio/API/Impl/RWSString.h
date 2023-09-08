/*****************************************************************************
 *
 * File :     RWSString.h
 *
 * Abstract:  Access to either c library or win32 string manipulation routines.
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


#if !defined (CSL_RWSSTRING_H)
#define CSL_RWSSTRING_H

#define RWSSTRING(s) L##s
#define rwsstok wcstok    // C tokeniser. No Win32 equivalent.
#define rwsstrstr wcsstr  // Search for a sub-string

#ifdef _WINBASE_  // If we can, link to the Win32 string manipulation routines

#define rwsslen lstrlenW    // Get the length of a string
#define rwsscpy lstrcpyW    // Copy a string
#define rwsscmp lstrcmpW    // Compare strings
#define rwssicmp lstrcmpiW  // Perform a lowercase comparison of strings
#define rwsscat lstrcatW    // Append a string


#else // Else, we can use the standard library


#define rwsslen wcslen    // Get the length of a string
#define rwsscpy wcscpy    // Copy a string
#define rwsscmp wcscmp    // Compare strings
#define rwssicmp wcsicmp  // Perform a lowercase comparison of strings
#define rwsscat wcscat    // Append a string


#endif // _WINBASE_

#endif // !defined (CSL_RWSSTRING_H)
