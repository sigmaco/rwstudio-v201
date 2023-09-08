
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
 * font.h
 *
 ****************************************************************************/

#ifndef _FONT_H__
#define _FONT_H__

RwInt32 WriteOutMaestroFontFiles(Rt2dObject *pObject);
void WriteOutMaestroFontFile(void);
void TraverseSceneAndGetFont(Rt2dObject *pObject);
RwBool FontFoundAlready(RwChar *pFontName);

#endif