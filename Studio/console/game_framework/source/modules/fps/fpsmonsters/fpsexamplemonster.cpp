/*****************************************************************************
*
* File :     FPSExampleMonster.cpp
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
// RenderWare Studio Includes
//
#include "fpsexamplemonster.h"

namespace RWS
{
   namespace FPS
   {
      RWS_IMPLEMENT_CLASSID(FPSExampleMonster);
      RWS_REGISTERCLASS(FPSExampleMonster);
      
      
      /**
      *        
      * \ref FPSExampleMonster. Constructor for FPSExampleMonster.
      *
      * \see ~FpsExampleMonster.
      *
      */
      FPSExampleMonster::FPSExampleMonster(const CAttributePacket& attr)
         :  FPSMonster(attr)
      {
         RWS_FUNCTION("RWS::FPS::FPSExampleMonster::FPSExampleMonster");
         
         // Can't use initializer lists for these as not member of this class
         m_height = 40.0f;
         m_height_on_ground = 20.0f;
         m_width = 20.0f;
         
         RWS_RETURNVOID();
      }
      
      
      /**
      *
      * \ref HandleAttributes. Handle attribute updates.
      *
      * \param attr reference to a CAttributePacket.
      *
      */ 
      void FPSExampleMonster::HandleAttributes(const CAttributePacket& attr)
      {
         RWS_FUNCTION("RWS::FPS::FPSExampleMonster::HandleAttributes");
         
         FPSMonster::HandleAttributes(attr);
         
         CAttributeCommandIterator attrIt(attr, 
            RWS_CLASSID_OF(FPSExampleMonster));
         
         while (!attrIt.IsFinished())
         {
            switch (attrIt->GetCommandId())
            {
            case CMD_m_height:
               attrIt->GetCommandData(m_height);
               break;
               
            case CMD_m_height_on_ground:
               attrIt->GetCommandData(m_height_on_ground);
               break;
               
            case CMD_m_width:
               attrIt->GetCommandData(m_width);
               break;
            }
            ++attrIt;
         }
         
         RWS_RETURNVOID();
      }
   } // namespace FPS
}// namespace RWS
