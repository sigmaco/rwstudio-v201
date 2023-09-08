#ifndef RWUTIL_STRING_H
#define RWUTIL_STRING_H

#include "RwUtil.h"
#include "RwUtilTypes.h"

/**
*   Converts a wide sting to an ascii string
*
*   \ingroup rwutilstring
*
*   \param wString the input wide string
*
*/
RWUTIL_API RwString RwUtilW2A(const RwWString &wString);

/**
*   Converts an ascii sting to a wide string
*
*   \ingroup rwutilstring
*
*   \param inString the input ascii string
*
*/
RWUTIL_API RwWString RwUtilA2W(const RwString &inString);
/**
*   Removes the path from a full file name, leaving just the file name.
*
*   \ingroup rwutilstring
*
*   \param inString the full path to be stripped
*
*/
RWUTIL_API void        RwUtilStripToFileName(RwWString &inString);

/**
*   Removes the filename from the given full file name, leaving just the path.
*
*   \ingroup rwutilstring
*
*   \param inString the full path to be stripped
*
*/
RWUTIL_API void        RwUtilStripToPath(RwWString &inString);

/**
*   Removes the path filename from the given full file name, leaving just the file extension.
*
*   \ingroup rwutilstring
*
*   \param inString the full file name to be stripped
*
*/
RWUTIL_API void        RwUtilStripToExtension(RwWString &inString);

/**
*   Removes the extension from a filename
*
*   \ingroup rwutilstring
*
*   \param inString the file name to be stripped
*
*/
RWUTIL_API void        RwUtilStripOffExtension(RwWString &inString);

/**
*   Change the extension of a given filename
*
*   \ingroup rwutilstring
*
*   \param oldFileName the file name to be changed
*   \param newExtension the new extension to be used
*
*   \returns 
*/
RWUTIL_API RwWString    RwUtilChangeFileExtension(RwWString oldFilename, RwWString newExtension);

/**
*   This function converts all forward slashes to back slashes in the given string.
*
*   \ingroup rwutilstring
*
*   \param inString the string to be edited
*
*/
RWUTIL_API void        RwUtilConvertForwardSlashesToBack(RwWString &inString);

/**
*   This function transforms all characters in the given string to lowercase.
*
*   \ingroup rwutilstring
*
*   \param inString the string to be transformed
*
*/
RWUTIL_API void        RwUtilMakeLowerCase(RwWString &inString);

/**
*   This function transforms all characters in the given string to uppercase.
*
*   \ingroup rwutilstring
*
*   \param inString the string to be transformed
*
*/
RWUTIL_API void        RwUtilMakeUpperCase(RwWString &inString);


typedef enum
{
    RwUtilFileTypeInvalid,
    RwUtilFileTypeRWS,
    RwUtilFileTypeBSP,
    RwUtilFileTypeDFF,
    RwUtilFileTypeANM,
    RwUtilFileTypeDMA,
    RwUtilFileTypeSPL,
} RwUtilFileType;


/**
*   This function determines the filetype of a given filename.
*
*   \ingroup rwutilstring
*
*   \param filename
*
*   \returns a member of the RwUtilFileType enum indicating the file type. 
*/
RWUTIL_API RwUtilFileType   RwUtilGetFileType(RwWString fileName);

/**
*   Convert a given string to an integer.
*
*   \ingroup rwutilstring
*
*   \param inString the string to be converted
*
*   \returns the integer representation of the string.
*/
RWUTIL_API int         RwUtilStringToInt(const RwWString &inString);

/**
*   Convert a given integer to a string.
*
*   \ingroup rwutilstring
*
*   \param val the integer to be converted
*
*   \returns the string representation of the integer
*/
RWUTIL_API RwWString    RwUtilIntToString(const int val);

/**
*   This functions splits the input string into a vector of sub-strings. The sub-strings are
*   delimited by instances of token in the input string.
*
*   \ingroup rwutilstring
*
*   \param in the string to tokenise
*   \param vOut the vector of tokenised sub-strings
*   \param token the delimiter to split by
*
*/
RWUTIL_API void        RwUtilTokenise (const RwWString &in, RwVector<RwWString> &vOut, const wchar_t token);

/**
*   This function finds the first instance of inString2 within inString1. It ignores case
*   when doing this search. The function returns the position of the first occurrence. If no
*   instance of inString2 is found in inString1 the function returns the length of inString1.
*
*   \ingroup rwutilstring
*
*   \param inString1 the string to be searched
*   \param inString2 the string to search for
*
*   \returns the index of the first occurrence of inString2 in inString1
*/
RWUTIL_API int         RwUtilFindWithoutCase(RwWString &inString1, RwWString &inString2);

/**
*   The function returns a relative path from path ref to path absolute.
*
*   \ingroup rwutilstring
*
*   \param absolute the destination of the relative path to be returned
*   \param ref the source of the relative path returned
*
*   \returns the relative path from ref to absolute
*/
RWUTIL_API RwWString    RwUtilGetRelativePath(const RwWString &absolute, const RwWString &ref);

/**
*   Compares two strings ignoring case.
*
*   \ingroup rwutilstring
*
*   \param inString1 the first string to be compared
*   \param inString2 the second string to be compared
*
*   \returns a value < 0 if inString1 is less than inString2, a value > 0 if inString1 is greater than inString2, zero if they are equal
*/
RWUTIL_API bool        RwUtilCaselessStringEquals(const RwWString &inString1,const RwWString &inString2);

/**
*   Returns whether the passed character is a letter (in the range 'a'-'z' or 'A'-'Z').
*
*   \ingroup rwutilstring
*
*   \param c the character to be checked
*
*   \returns true if the character is a letter, false otherwise
*/
RWUTIL_API bool        RwUtilIsLetter(const wchar_t c);

/**
*   Returns whether the passed character is a digit (in the range '0'-'9').
*
*   \ingroup rwutilstring
*
*   \param c the character to be checked
*
*   \returns true if the character is a digit, false otherwise
*/
RWUTIL_API bool        RwUtilIsDigit(const wchar_t c);

/**
*   Returns whether the passed character is whitespace (space, tab, newline or carriage return).
*
*   \ingroup rwutilstring
*
*   \param c the character to be checked
*
*   \returns true if the character is whitespace, false otherwise
*/
RWUTIL_API bool        RwUtilIsWhiteSpace(const wchar_t c);

/**
*   Returns whether the passed string is entirely whitespace (spaces, tabs, newlines or carriage returns).
*
*   \ingroup rwutilstring
*
*   \param inString the string to be checked
*
*   \returns true if the string is entirely composed of whitespace, false otherwise
*/
RWUTIL_API bool        RwUtilIsWhiteSpace(const RwWString &inString);

/**
*   Returns the short pathname version of the passed filename.
*
*   \ingroup rwutilstring
*
*   \param filename the filename to be converted
*
*   \returns the short version of the passed filename
*/
RWUTIL_API RwWString     RwUtilGetShortFileName(const RwWString &filename);

#endif // RWUTIL_STRING_H
