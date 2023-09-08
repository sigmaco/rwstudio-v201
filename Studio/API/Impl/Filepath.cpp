#include <atlbase.h>
#include <string>
#include <shlwapi.h>

#include "API/Impl/RWSString.h"
#include "API/Inc/RWSCore.h"

#pragma comment(lib, "Shlwapi.lib")

namespace RWS { namespace FilePath {

/* -------------------------------------------------------------------------- */
// Build a fully qualified file path from resource root and relative file
void MakeFullPath (const std::basic_string<RWSChar> &strResourceRoot,
                   const std::basic_string<RWSChar> &strRelativeFilePath,
                   std::basic_string<RWSChar> &strFullFilePath)
{
   // If file begins with "*:\", or "\", replace rather than append
   if ((strRelativeFilePath.length () > 1 &&
                        _T(':') == strRelativeFilePath[1]) ||
       (_T('\\') == strRelativeFilePath[0] ||
                        _T('/') == strRelativeFilePath[0]))
   {
      strFullFilePath = strRelativeFilePath;
   }
   else
   {
      strFullFilePath = strResourceRoot;

      // Append relative file path onto resource root and canonicalise
      if (!strRelativeFilePath.empty ())
      {
         // Append a trailing backslash
         if (strResourceRoot.length () >= 1 &&
             strResourceRoot[strResourceRoot.length () - 1] != _T('\\'))
         {
            strFullFilePath += _T("\\");
         }
         strFullFilePath += strRelativeFilePath;

         RWSChar strTemp[MAX_PATH];
         if (PathCanonicalize (strTemp, strFullFilePath.c_str ()))
            strFullFilePath = strTemp;
      }
   }
   
   // Remove any trailing '\'
   UINT nPos = strFullFilePath.find_last_not_of (RWSSTRING ('\\')) + 1;
   if (nPos < int (strFullFilePath.size ()))
      strFullFilePath.erase (nPos, strFullFilePath.size () - nPos);
}

/* -------------------------------------------------------------------------- */
// Build a path relative to the resource root, given a fully qualified file
void MakeRelativePath (const std::basic_string<RWSChar> &strResourceRoot,
                       const std::basic_string<RWSChar> &strFullFilePath,
                       std::basic_string<RWSChar> &strRelativeFilePath)
{
   // Make the given path relative to strResourceRoot
   // Make sure the resource root ends with a "\"
   std::basic_string<RWSChar> strGoodResourceRoot (strResourceRoot);
   int nLength = strGoodResourceRoot.length ();
   if (nLength && _T('\\') != strGoodResourceRoot[nLength-1])
   {
      strGoodResourceRoot.append (L"\\");
   }
   RWSChar szTempFileName [MAX_PATH];
   if (PathRelativePathTo (szTempFileName, strGoodResourceRoot.c_str (),
                           FILE_ATTRIBUTE_DIRECTORY,
                           strFullFilePath.c_str (), FILE_ATTRIBUTE_NORMAL))
   {
      if (szTempFileName[0] == _T('.') && szTempFileName[1] == _T('\\'))
      {
         // If we end up with a ".\" at the start, remove it
         strRelativeFilePath = &szTempFileName[2];
      }
      else if (szTempFileName[0] == _T('\\'))
      {
         // If we end up with a "\" at the start, remove it
         strRelativeFilePath = &szTempFileName[1];
      }
      else
         strRelativeFilePath = szTempFileName;
   }
   else
   {
      strRelativeFilePath = strFullFilePath;
   }

   // Remove any trailing '\'
   UINT nPos = strRelativeFilePath.find_last_not_of (RWSSTRING ('\\')) + 1;
   if (nPos < int (strRelativeFilePath.size ()))
      strRelativeFilePath.erase (nPos, strRelativeFilePath.size () - nPos);

}

/* -------------------------------------------------------------------------- */

}} // namespace RWS::FilePath
