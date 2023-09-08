#ifndef __RWS_FILE_PATH_FUNCTIONS_H__
#define __RWS_FILE_PATH_FUNCTIONS_H__

#include <string>

namespace RWS { namespace FilePath {

   /* ---------------------------------------------------------------------- */
   // Build a fully qualified file path from resource root and relative file
   void MakeFullPath (const std::basic_string<RWSChar> &strResourceRoot,
                      const std::basic_string<RWSChar> &strRelativeFilePath,
                      std::basic_string<RWSChar> &strFullFilePath);

   /* ---------------------------------------------------------------------- */
   // Build a path relative to the resource root, given a fully qualified file
   void MakeRelativePath (const std::basic_string<RWSChar> &strResourceRoot,
                          const std::basic_string<RWSChar> &strFullFilePath,
                          std::basic_string<RWSChar> &strRelativeFilePath);


/* -------------------------------------------------------------------------- */

}} // namespace RWS::FilePath

#endif // __RWS_FILE_PATH_FUNCTIONS_H__