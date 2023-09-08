/*****************************************************************************
*
* File : 	 TBMakeTexDict.cpp
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
* Copyright (c) 2002 Criterion Software Ltd.
* All Rights Reserved.
*
* RenderWare is a trademark of Canon Inc.
*
*****************************************************************************/
#include "precomp.h"

#ifdef RWS_DESIGN

// Left to its own devices, the MW linker will dead-strip the registrar ctors, so...
#ifdef __MWERKS__
#pragma force_active on
#pragma exceptions off // Turn off exceptions due to bug in MW Dwarf Generator for PS2, present in CW 3.04
#endif

//////////////////////////////////////////////////////////////////
//
// RenderWare Includes
//
#include <rtpitexd.h>

#ifdef _MSC_VER
#pragma comment (lib, "rtpitexd.lib")
#endif

//#define RWS_CALLSTACKENABLE
//#define RWS_TRACEENABLE

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "tbmaketexdict.h"
#include "framework/core/macros/debugmacros.h"
#include "framework/core/netstream/nettools.h"


namespace RWS
{
   namespace ToolBox
   {
      RWS_REGISTERCLASS(ATBMakeTexDict);
      RWS_IMPLEMENT_CLASSID(ATBMakeTexDict);

      /**
      *
      *  Construct ATBMakeTexDict object.
      *
      *  \param rAttr Reference to a CAttributePacket.
      */
      ATBMakeTexDict::ATBMakeTexDict(const CAttributePacket& rAttr)
         : InitCEventHandler(0)
      {
      }
      
      /**
      *
      *  Destroy ATBMakeTexDict object.
      */
      ATBMakeTexDict::~ATBMakeTexDict()
      {
          UnLinkMsg(m_MakeTexDict);
          UnRegisterMsg(m_MakeTexDict);
      }

      /**
      *
      * Handle Events for ATBMakeTexDict object.
      *
      *  \param pMsg Reference to a CMsg object.
      */
      void ATBMakeTexDict::HandleEvents(CMsg &pMsg)
      {
         RWS_FUNCTION("ATBMakeTexDict::HandleEvents");
         
         if (pMsg.Id == m_MakeTexDict)
         {
            RWS_TRACE("ATBMakeTexDict::HandleEvents");

            RwTexDictionary *pTexDict = RwTexDictionaryGetCurrent ();

            if (pTexDict)
            {
               RwMemory memory;

               if (RwStream *pStream = RwStreamOpen (rwSTREAMMEMORY, rwSTREAMWRITE, &memory))
               {
#if defined(_WINDOWS)
                  RtPITexDictionaryStreamWrite (pTexDict, pStream);
#else
                  RwTexDictionaryStreamWrite (pTexDict, pStream);
#endif

                  RwStreamClose (pStream, &memory);
               }

   #if defined(_WINDOWS)
               NetTools::SendFile("pcTextDict.rws", "Platform Independent Texture Dictionary", memory.start, memory.length);
   #elif defined (_XBOX)
               NetTools::SendFile("xboxTextDict.rx1", "XBOX Texture Dictionary", memory.start, memory.length);
   #elif defined (SKY)
               NetTools::SendFile("ps2TextDict.rp2", "PS2 Texture Dictionary", memory.start, memory.length); 
   #elif defined (DOLPHIN)
               NetTools::SendFile("gcnTextDict.rg1", "GCN Texture Dictionary", memory.start, memory.length);
   #endif
               RwFree(memory.start);
            }
         }

         RWS_RETURNVOID();
      }
      
      /**
      *
      *  Handle Attributes for ATBMakeTexDict object.
      *
      *  \param rAttr Reference to a CAttributePacket.
      */
      void ATBMakeTexDict::HandleAttributes(const CAttributePacket& rAttr)
      {
         RWS_FUNCTION("ATBMakeTexDict::HandleAttributes");
         
         CAttributeHandler::HandleAttributes(rAttr);
         
         CAttributeCommandIterator attrIt(rAttr, RWS_CLASSID_OF(ATBMakeTexDict));
         
         while (!attrIt.IsFinished())
         {
            switch (attrIt->GetCommandId())
            {
                case CMD_GenerateTextDict:

                   RWS_TRACE("ATBMakeTexDict::HandleAttributes");

                   ReplaceLinkedMsg(m_MakeTexDict, attrIt->GetAs_RwChar_ptr(), 0);
                   break;
            }
            ++attrIt;
         }
         RWS_RETURNVOID();
      }
   }
}
#endif