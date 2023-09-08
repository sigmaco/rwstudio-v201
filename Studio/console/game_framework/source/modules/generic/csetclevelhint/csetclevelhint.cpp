/*****************************************************************************
 *
 * File :     CSetCLevelHint.cpp
 *
 * Abstract:   Implementation of CSetCLevelHint class
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
 * Copyright (c) 2001 Criterion Software Ltd.
 * All Rights Reserved.
 *
 * RenderWare is a trademark of Canon Inc.
 *
 *****************************************************************************/
#include "precomp.h"

//#define RWS_CALLSTACKENABLE
//#define RWS_TRACEENABLE

// Left to its own devices, the MW linker will dead-strip the registrar ctors, so...
#ifdef __MWERKS__
#pragma force_active on
#pragma exceptions off // Turn off exceptions due to bug in MW Dwarf Generator for PS2, present in CW 3.04
#endif

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "CSetCLevelHint.h"

#include "framework/core/macros/debugmacros.h"
#include "framework/core/resourcemanager/cresourcemanager.h"

namespace RWS
{
   namespace Generic
   {
      RWS_IMPLEMENT_CLASSID(CSetCLevelHint);
      RWS_REGISTERCLASS(CSetCLevelHint);

      /**
      *  Construct a CSetCLevelHint object.
      *  \param rAttr Reference to a CAttributePacket.
      *
      */
      CSetCLevelHint::CSetCLevelHint(const CAttributePacket& rAttr)
      {
         RWS_FUNCTION("RWS::Generic::CSetCLevelHint::CSetCLevelHint");

         RWS_RETURNVOID();
      }

      /**
      *  Handle attributes of CSetCLevelHint object.
      *  \param rAttr Reference to a CAttributePacket.
      *
      */
      void CSetCLevelHint::HandleAttributes(const CAttributePacket& attr)
      {
         RWS_FUNCTION("RWS::Generic::CSetCLevelHint::HandleAttributes");

         // Process attributes.
         //
         CAttributeCommandIterator AttrIt(attr, RWS_CLASSID_OF(CSetCLevelHint));

         while (!AttrIt.IsFinished())
         {
            switch (AttrIt->GetCommandId())
            {
               case CMD_CLevelHint:
                  switch (AttrIt->GetAs_RwUInt32())
                  {
                     default:
                     case 0:
                        m_CLevelHint = CLevel::HINT_OPAQUE_WORLD;
                        break;
                     case 1:
                        m_CLevelHint = CLevel::HINT_TRANSLUCENT_WORLD;
                        break;
                  }
                  RWS_TRACE("Warning: Changing CLevelHint requires the level to be reset to take effect,"
                     " resetting the level will attach the atomics/clumps to the correct worlds.");
               break;
            }
            ++AttrIt;
         }

         // Find and process each of the attached assets.
         //
         CAttributeCommandIterator sysattrIt(attr, RWS_CLASSID_OF(CSystemCommands));

         while (!sysattrIt.IsFinished())
         {
            switch (sysattrIt->GetCommandId())
            {
               case CSystemCommands::CMD_AttachResource:
                  {
                     const RwChar *resourceType;
      
                     // Const cast is requires as we are going to modify the actual resource.
                     //
                     const RWSGUID * pResourceId = 0;
                     sysattrIt->GetCommandData(&pResourceId);
                     void *pObject = const_cast<void*>(CResourceManager::FindById(pResourceId, &resourceType));
      
                     RWS_ASSERT(pObject, "Resource::Find failed, id = " << pResourceId);

                     if (resourceType)
                     {
                        if (!rwstrcmp("rwID_ATOMIC", resourceType))
                        {
                           CLevel::SetPreferredWorldHint(static_cast<RpAtomic *>(pObject), m_CLevelHint);
                        }
                        else if (!rwstrcmp("rwID_CLUMP", resourceType))
                        {
                           CLevel::SetPreferredWorldHint(static_cast<RpClump *>(pObject), m_CLevelHint);
                        }
                        else
                        {
                           RWS_ASSERTFAIL("CSetCLevelHint can only process atomics and clumps.");
                        }
                     }
                  }
                  break;
            }; 
            ++sysattrIt;
         }

         RWS_RETURNVOID();
      }
   }
}