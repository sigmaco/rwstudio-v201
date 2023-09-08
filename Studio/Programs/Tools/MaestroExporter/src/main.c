/****************************************************************************
 *
 * This file is a product of Criterion Software Ltd.
 *
 * This file is provided as is with no warranties of any kind and is
 * provided without any obligation on Criterion Software Ltd.
 * or Canon Inc. to assist in its use or modification.
 *
 * Criterion Software Ltd. and Canon Inc. will not, under any
 * circumstances, be liable for any lost revenue or other damages
 * arising from the use of this file.
 *
 * Copyright (c) 2002 Criterion Software Ltd.
 * All Rights Reserved.
 *
 */

/****************************************************************************
 *
 * main.c
 *
 ****************************************************************************/

#include "windows.h"

// Renderware Includes
#include "rwcore.h"
#include "rpworld.h"
#include "rt2d.h"
#include "rt2danim.h"

#include "maestro.h"
#include "startup.h"

// ******************************************************************************
int main(int argc, char *argv[], char *envp[])
{
    RwInt32 i;

    if (Startup(0) != 0)
    {
       // Error occurred
       return 0;
    }

    /* Do stream dump for each file specified on command line */
    if (argc > 1)
    {
        for (i=1; i < argc; i++)
        {
           if (!ExportMaestroData(argv[i]))
           {
              break;
           }
        }

        printf("\n");
    }
    else
    {
        printf("Usage: maestroexp file1 file2 ...\n\n");
    }

    Close();

    return 0;
}

