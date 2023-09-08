/*****************************************************************************
*
* File :     FPSTeleportDestination.cpp
*
* Abstract : 
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
// Left to its own devices, the MW linker will dead-strip the registrar ctors, so...
#ifdef __MWERKS__
#pragma force_active on
#pragma exceptions off // Turn off exceptions due to bug in MW Dwarf Generator for PS2, present in CW 3.04
#endif

//////////////////////////////////////////////////////////////////
//
// RenderWare Includes
//
#include <rwcore.h>
#include <rpworld.h>

#ifdef _MSC_VER
#pragma comment (lib, "rwcore.lib")
#pragma comment (lib, "rpworld.lib")
#endif

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "fpsteleportdestination.h"

namespace RWS
{
   namespace FPS
   {
      RWS_IMPLEMENT_CLASSID(FPSTeleportDestination);
      RWS_REGISTERCLASS(FPSTeleportDestination);
      
      
      /**
      *
      * \ref FPSTeleportDestination. Destructor for FPSTeleportDestination.
      *
      */
      FPSTeleportDestination::~FPSTeleportDestination(void)
      {
         RWS_FUNCTION("RWS::FPS::FPSTeleportDestination::~FPSTeleportDestination");
         UnLinkMsg(m_targetname);
         UnRegisterMsg(m_targetname);
         RWS_RETURNVOID();
      }
      
      
      /**
      *
      * \ref HandleEvents(). Handle events.    
      *
      * \param pMsg Standard Message Package.
      *  
      * \ref iMsgRunningTick
      */
      void FPSTeleportDestination::HandleEvents(CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::FPS::FPSTeleportDestination::HandleEvents");
         
         if (pMsg.Id == m_targetname)
         {
            RWS_TRACE("FPSTeleportDestination::HandleEvents");
            
            RwFrame **pRwFrame = static_cast<RwFrame**>(pMsg.pData);
            
            // Copy the position of this entity
            if (pRwFrame)
            {
               *pRwFrame = RpAtomicGetFrame(m_pAtomic.ptr());
            }
         }
         
         RWS_RETURNVOID();
      }
      
      
      /**
      *
      * \ref HandleAttributes. Handle attribute updates.
      *
      * \param attr reference to a CAttributePacket.
      *
      */   
      void FPSTeleportDestination::HandleAttributes(const CAttributePacket& attr)
      {
         RWS_FUNCTION("RWS::FPS::FPSTeleportDestination::HandleAttributes");
         
         CAttributeHandler::HandleAttributes(attr);
         m_pAtomic.HandleSystemCommands(attr);
         CAttributeCommandIterator attrIt(attr, RWS_CLASSID_OF(FPSTeleportDestination));
         
         while (!attrIt.IsFinished())
         {
            switch (attrIt->GetCommandId())
            {
            case CMD_targetname:
               ReplaceLinkedMsg(m_targetname, attrIt->GetAs_RwChar_ptr(), "RwFrame**");
               RWS_TRACE("FPSTeleportDestination::HandleAttributes CMD_targetname "<<attrIt->GetAs_RwChar_ptr());
               
               break;
            }
            ++attrIt;
         }
         
         RWS_RETURNVOID();
      }
      
   }// namespace FPS
}// namespace RWS