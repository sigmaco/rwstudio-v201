/*****************************************************************************
*
* File :     CDirectorsCamera.cpp
*
* Notes :
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
#include "precomp.h"

#ifdef RWS_DESIGN

//#define RWS_CALLSTACKENABLE
//#define RWS_TRACEENABLE

#ifdef __MWERKS__
#pragma force_active on // Left to its own devices, the MW linker will dead-strip the registrar ctors, so...
#pragma exceptions off // Turn off exceptions due to bug in MW Dwarf Generator for PS2, present in CW 3.04
#endif

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "cdirectorscamera.h"

#include "framework/core/macros/debugmacros.h"
#include "framework/toolkits/debugtools/debugswitches.h"
#include "framework/toolkits/world/clevel.h"
#include "framework/toolkits/world/helpers/camerahelper.h"
#include "framework/startup/startup.h"
#include "framework/toolkits/math/maths.h"
#include "framework/mainloop/render.h"

namespace RWS
{
   namespace Generic
   {
      using namespace StartUp;

      RWS_IMPLEMENT_CLASSID(CDirectorsCamera);
      RWS_REGISTERCLASS(CDirectorsCamera);
      
      /**
      *
      *  Construct a CDirectorsCamera object.
      *
      */
      CDirectorsCamera::CDirectorsCamera(const CAttributePacket& rAttr) : InitCEventHandler(0)
      {
         RWS_FUNCTION("RWS::Generic::CDirectorsCamera::CDirectorsCamera");
         
         LinkMsg(iMsgDoRenderDirectorsCamera, "RwCamera*");
         
         RWS_RETURNVOID();
      }

      /**
      *
      *  Destroy a CDirectorsCamera object
      *
      */
      CDirectorsCamera::~CDirectorsCamera()
      {
         RWS_FUNCTION("RWS::Generic::CDirectorsCamera::~CDirectorsCamera");

         UnLinkMsg(iMsgDoRenderDirectorsCamera);
         UnRegisterMsg(m_Render_Out);

         RWS_RETURNVOID();
      }

      /**
      *
      *  Handle events
      *
      */
      void CDirectorsCamera::HandleEvents(CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::Generic::CDirectorsCamera::HandleEvents");

         if (pMsg.Id == iMsgDoRenderDirectorsCamera)
         {
            CMsg Msg(m_Render_Out, pMsg.pData);

            SendMsg(Msg);
         }

         RWS_RETURNVOID();
      }

     /**
      *
      *  Handle attributes
      *
      */
      void CDirectorsCamera::HandleAttributes(const CAttributePacket& attr)
      {
         RWS_FUNCTION("RWS::Generic::CDirectorsCamera:HandleAttributes");
         
         CAttributeCommandIterator attrIt(attr, RWS_CLASSID_OF(CDirectorsCamera));
         
         while (!attrIt.IsFinished())
         {
            switch (attrIt->GetCommandId())
            {
                  //////////////////////////////////////////////////////////////////
               case CMD_RenderOut:
                  //////////////////////////////////////////////////////////////////
                  ReplaceRegisteredMsg(m_Render_Out, attrIt->GetAs_RwChar_ptr(), "RwCamera*");
               break;
            }
            ++attrIt;
         }
         
         RWS_RETURNVOID();
      }
   }//namespace Tutorial
}//namespace RWS
#endif