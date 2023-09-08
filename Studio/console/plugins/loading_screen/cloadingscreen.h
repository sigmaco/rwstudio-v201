/*****************************************************************************
*
* File :     CLoadingScreen.h
*
* Abstract : Implementation of CLoadingScreen class.
*
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
* Copyright (c) 2000 Criterion Software Ltd.
* All Rights Reserved.
*
* RenderWare is a trademark of Canon Inc.
*
*****************************************************************************/

#ifndef __CLoadingScreen_H__
#define __CLoadingScreen_H__

#ifdef RWS_DESIGN

#include <rtcharse.h>

namespace RWS
{
    /**
    *
    * Behaviour class, simple default world renderer. Renders the world in response
    * to the \ref iMsgDoRender event.
    *
    */

    class CLoadingScreen
    {
        public:
            CLoadingScreen();
            ~CLoadingScreen();

            void Render(RwCamera *pRwCamera, RwChar *message);

        private:


            RwTexDictionary *m_splashTXD;
            RtCharset *m_Charset;

            void RenderOverlay(RwRaster *texRaster, RwUInt8 alpha, RwCamera *camera,
                               RwReal minU, RwReal minV, RwReal maxU, RwReal maxV);
            RwTexDictionary *ReadSplashTXD(void);
    };
}

#endif //__CLoadingScreen_H__
#endif