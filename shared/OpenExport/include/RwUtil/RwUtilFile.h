#ifndef RWUTIL_FILE_H
#define RWUTIL_FILE_H

#include "RwUtil.h"
#include "RwUtilTypes.h"

/**
*   This function can be used to determine if a given path points to an
*   existing directory.
*
*   \ingroup rwutilfile
*
*   \param path the path to be checked
*   \returns true if the path exists and is a directory, false otherwise
*
*/
RWUTIL_API bool            RwUtilIsPathAvailable(const RwWString &path);

/**
*   This function determines whether a given path is relative or absolute.
*
*   \ingroup rwutilfile
*
*   \param path the path to be checked
*   \returns true if path is relative, false otherwise
*
*/
RWUTIL_API bool            RwUtilIsRelativePath(const RwWString &path);

/**
*   This function can be used to create a directory. If the path passed does not
*   include a drive letter the drive from the current path is used.
*
*   \ingroup rwutilfile
*
*   \param path the directory to be created
*   \returns true if the path was created or already existed, false if the path could not be created
*
*/
RWUTIL_API bool            RwUtilCreatePath(const RwWString &path);

/**
*   This function can be used to get the path to a temporary directory
*
*   \ingroup rwutilfile
*
*   \returns the path to the temporary directory.
*
*/
RWUTIL_API RwWString        RwUtilGetTempPath();

/**
*   This function can be used to get a full path to a temporary file. The file will
*   be prefixed with "RWS".
*
*   \ingroup rwutilfile
*
*   \returns the path to the temporary file
*
*/
RWUTIL_API RwWString        RwUtilGetTempFile();

/**
*   This function is used to concatenate a directory path and a realtive file path
*
*   \ingroup rwutilfile
*
*   \param path1 the directory path
*   \param path2 the filename to be added
*   \returns the concatenated path
*
*/
RWUTIL_API RwWString        RwUtilConcatenatePaths(const RwWString &path1, const RwWString &path2);

/**
*   Get the current path.
*
*   \ingroup rwutilfile
*
*   \returns the current path
*
*/
RWUTIL_API RwWString        RwUtilGetCurrentPath();

/**
*   The function determines whether the given file exists or not.
*
*   \ingroup rwutilfile
*
*   \param fileName the file to check
*   \returns true if the file exists, false otherwise
*
*/
RWUTIL_API bool            RwUtilFileExists(const RwWString &fileName);

/**
*   This function finds all files of the given extension in the given folder.
*
*   \ingroup rwutilfile
*
*   \param folderPath the folder to search from
*   \param extension the extension of files to find. If empty defaults to *
*   \param fileNames returns the filenames found
*   \param fullpath whether to return full path names or just filenames
*   \param recursive whether to recurse sub-directories
*   \returns true if the find was successful, false otherwise
*
*/
RWUTIL_API bool            RwUtilFindFilesAndFolders(RwWString folderPath,
                                          RwWString extension,
                                          RwVector<RwWString> &fileNames,
                                          bool fullpath = false,
                                          bool recursive = false);

/**
*   Determine whether the given file is writable or not.
*
*   \ingroup rwutilfile
*
*   \param fileName the file to check
*   \param bWritable returns whether the file was writable.
*   \returns true if the file was found and checked, false otherwise
*
*/
RWUTIL_API bool            RwUtilGetFileWritable(const RwWString &fileName, bool &bWritable);

/**
*   Set the write flag of a given file.
*
*   \ingroup rwutilfile
*
*   \param fileName the file to be altered
*   \param bWritable whether the file should now be writable or not
*   \returns true if the file was found and altered, false otherwise
*
*/
RWUTIL_API bool            RwUtilSetFileWritable(const RwWString &fileName, bool bWritable);

/**
*   Get a string representation of the last error.
*
*   \ingroup rwutilfile
*
*   \param errorMsg returns the error message
*   \returns true if the last error was not ERROR_SUCCESS 
*
*/
RWUTIL_API bool            RwUtilGetLastErrorString(RwWString &errorMsg);

/**
*   Class RwUtilTemporaryFile
*   On creation generates a temporary file
*   On destruction attempts to delete that file
*
*   \ingroup rwutilfile
*
*/
class RWUTIL_API RwUtilTemporaryFile
{
public:
    RwUtilTemporaryFile();
    ~RwUtilTemporaryFile();

    operator const RwWString&() const { return filename; }
    inline RwWString AsString() const { return filename; }
private:
    // disable these
    RwUtilTemporaryFile(const RwUtilTemporaryFile& rhs);
    RwUtilTemporaryFile& operator=(const RwUtilTemporaryFile& rhs);

    RwWString filename;
};

#endif // RWUTIL_FILE_H
