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
 * Copyright (c) 2003 Criterion Software Ltd.
 * All Rights Reserved.
 *
 */

/****************************************************************************
 *
 * maestro.h
 *
 ****************************************************************************/

#ifndef MAESTRO_H__
#define MAESTRO_H__

RwBool ExportFonts(Rt2dMaestro *pMaestro);
RwBool ExportMaestroTextureDictionary(Rt2dMaestro *pMaestro, RwChar *pFileName);
RwBool ExportMaestroData(RwChar *pFileName);


#endif // MAESTRO_H__