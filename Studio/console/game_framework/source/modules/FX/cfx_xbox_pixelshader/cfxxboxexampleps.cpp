/*****************************************************************************
 *
 * File :     CFXXBoxExamplePS.cpp
 *
 * Abstract : Example of how to apply a pixel shader to an atomic.
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
#include "rwcore.h"

// Left to its own devices, the MW linker will dead-strip the registrar ctors, so...
#ifdef __MWERKS__
#pragma force_active on
#endif


//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "cfxxboxexampleps.h"
#include "exampleps.h"

#include "framework/core/macros/debugmacros.h"
#include "framework/toolkits/world/clevel.h"
#include "framework/toolkits/math/maths.h"
#include "framework/mainloop/render.h"
#include "framework/core/resourcemanager/cresourcemanager.h"

namespace RWS
{

   namespace FX
   {

      RWS_IMPLEMENT_CLASSID(CFXXBoxExamplePS);
      RWS_REGISTERCLASS(CFXXBoxExamplePS);

      /**
      *
      *  Construct a CFXXBoxExamplePS object.
      *
      */
      CFXXBoxExamplePS::CFXXBoxExamplePS(const CAttributePacket& rAttr) : InitCEventHandler(0),
                                          m_RampDir(0), m_Blend(0.0f)
      {
         RWS_FUNCTION("RWS::FX::CFXXBoxExamplePS::CFXXBoxExamplePS");

         LinkMsg (iMsgDoRender, "RwCamera*");
                        
         m_PixelShader = RpXboxPixelShaderCreate (dwExamplePSPixelShader);
         m_RampDir   = 0;
         m_Blend     = 0.0f;

         RWS_RETURNVOID();
      }

      /**
      *
      *  Destroy a CFXXBoxExamplePS object.
      *
      */
      CFXXBoxExamplePS::~CFXXBoxExamplePS()
      {
         RWS_FUNCTION("RWS::FX::CFXXBoxExamplePS::~CFXXBoxExamplePS");

         UnLinkMsg(iMsgDoRender);

         if (m_PixelShader)
         {
            RpXboxPixelShaderDestroy(m_PixelShader);
         }

         RWS_RETURNVOID();
      }


      /**
      *
      * \ref HandleEvents(). Handle events.  
      *
      * \param pMsg Standard Message Package
      *  
      * \ref iMsgRunningTick
      */
      void CFXXBoxExamplePS::HandleEvents(CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::FX::CFXXBoxExamplePS::HandleEvents");

         if (pMsg.Id == iMsgDoRender)
         {
            const RwReal   SpeedReduction = 0.25f;
            // ramp the blend value up and down
            if (m_RampDir)
            {
               m_Blend -= (m_Speed * SpeedReduction);
               if (m_Blend < 0.0f)
               {                   
                  m_RampDir = 0;
                  m_Blend = 0.0f;
               }               
            }
               else
            {
               m_Blend += (m_Speed * SpeedReduction);
               if (m_Blend > 1.0f)
               {
                  m_RampDir = 1;
                  m_Blend = 1.0f;
               }               
            }
            // blend vect stores the amount to blend between the grey scale and normal version
            // of the pixel
            RwV4d BlendVect;
            BlendVect.x = BlendVect.y = BlendVect.z = BlendVect.w = m_Blend;
            RpXboxPixelShaderSetConstant(m_PixelShader, 0, &BlendVect, 1); 

            // pass 1.0f, 1.0f, 1.0f, 1.0f into c1
            RwV4d One;
            One.x = One.y = One.z = One.w = 1.0f;
            RpXboxPixelShaderSetConstant(m_PixelShader, 1, &One, 1); 

         }

         RWS_RETURNVOID();
      }

      /**
      *
      *  Update attributes.
      *
      */
      void CFXXBoxExamplePS::HandleAttributes(const CAttributePacket& attr)
      {
         RWS_FUNCTION("RWS::FX::CFXXBoxExamplePS::HandleAttributes");
         
         CAttributeHandler::HandleAttributes(attr);
         
         CAttributeCommandIterator sysattrIt(attr, RWS_CLASSID_OF(CSystemCommands));
         while (!sysattrIt.IsFinished())
         {
            switch (sysattrIt->GetCommandId())
            {
               case CSystemCommands::CMD_AttachResource:
                  {
                     const RWSGUID * pResourceId = 0;
                     sysattrIt->GetCommandData(&pResourceId);

                     const RwChar *resourceType;
      
                     const void *pObject = 0;
                     if (pResourceId)
                     {
                        pObject = CResourceManager::FindById(pResourceId, &resourceType);
                     }
      
                     RWS_ASSERT (pObject, "Resource::Find failed for id = " << pResourceId);

                     if (resourceType)
                     {
                        if (!rwstrcmp("rwID_ATOMIC", resourceType))
                        {
                           AtomicSetPixelShader(const_cast<RpAtomic*>(reinterpret_cast<const RpAtomic *>(pObject)),
                                                m_PixelShader);
                        }
                           else if (!rwstrcmp("rwID_CLUMP", resourceType))
                        {
                           ClumpSetPixelShader(const_cast<RpClump*>(reinterpret_cast<const RpClump *>(pObject)),
                                               m_PixelShader);
                        }
                     }
                  }
                  break;
            }; 
            ++sysattrIt;
         }

         // deal with the attributes related to the pixel shader behaviour
         CAttributeCommandIterator attrIt(attr, RWS_CLASSID_OF(CFXXBoxExamplePS));
         
         while (!attrIt.IsFinished())
         {
            switch (attrIt->GetCommandId())
            {
               case CMD_Set_m_Speed:
                  attrIt->GetCommandData(m_Speed);
                  RWS_TRACE("CMD_Set_m_Speed "<<m_Speed);
                  break;
            }
            ++attrIt;
         }

         RWS_RETURNVOID();
      }
   } // namespace FX
} //namespace RWS
