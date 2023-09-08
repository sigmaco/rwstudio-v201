
#include "tchar.h"
#include "atlconv.h"

/* Used for new RW 3.7 FileSystem - Win32 version */

static const RwUInt32 UNC_PATH_SIZE = 256;   /* should be sufficient */
static const RwUInt32 MAX_NB_FILES_PER_FS = 20;

static RwBool psInstallFileSystem(void)
{
    _TCHAR      curDirBuffer[_MAX_PATH];
    RwUInt32    retValue;
    RtFileSystem *wfs;

    USES_CONVERSION;

    /* === Get the current directory === */
    retValue = GetCurrentDirectory(_MAX_PATH, curDirBuffer);
    if (0 == retValue || retValue > _MAX_PATH)
    {
        return FALSE;
    }

    /* This is a unc path. */
    if (curDirBuffer[1] != ':')
    {
        /* tag a \ to the end of the current directory 
           only fails if the buffer size is exhausted */
        _tcscat( curDirBuffer, _T("\\") );

        /* Setup the file system manager */
        if ((wfs = RtWinFSystemInit(MAX_NB_FILES_PER_FS, T2A(curDirBuffer), "unc")) != 0)
        {
            // Now actually register the file system
            if (RtFSManagerRegister(wfs) == FALSE)
            {
                return (FALSE);
            }
            else
            {
                RtFSManagerSetDefaultFileSystem(wfs);
            }
        }
        else
        {
            return (FALSE);
        }
    }
    else
    {
        RwUInt32 drivesMask;
        RwInt32 drive;
        RwChar  fsName[2];

        CharUpper(curDirBuffer);

      
        /* loop through all logical drives and register a file system for each
           valid one.
           Start at 2: this removes a: */
        drivesMask = GetLogicalDrives();

        for( drive = 2, drivesMask >>= 1; drivesMask != 0; drive++, drivesMask >>= 1)
        {
            if (drivesMask & 0x01)
            {
                RwInt32 ret;
                _TCHAR  deviceName[4];

                deviceName[0] = drive + _T('A') - 1;
                deviceName[1] = _T(':');
                deviceName[2] = _T('\\');   /* Needed for Win98 */
                deviceName[3] = _T('\0');

                ret = GetDriveType(deviceName);

                if ( ret != DRIVE_UNKNOWN && ret != DRIVE_NO_ROOT_DIR )
                {
                    /* Fix device name */
                    deviceName[2] = _T('\0');

                    fsName[0] = static_cast<RwChar>(deviceName[0]);
                    fsName[1] = '\0';

                    /* Setup the file system manager */
                    if ((wfs = RtWinFSystemInit(MAX_NB_FILES_PER_FS, T2A(deviceName), fsName)) != 0)
                    {
                        /* Now actually register the file system */
                        if (RtFSManagerRegister(wfs) == FALSE)
                        {
                            return (FALSE);
                        }
                        else
                        {
                            if (deviceName[0] == curDirBuffer[0])
                            {
                                RtFSManagerSetDefaultFileSystem(wfs);
                            }
                        }
                    }
                    else
                    {
                        return (FALSE);
                    }
                }
            }
        }
    }

    return (TRUE);

}   
