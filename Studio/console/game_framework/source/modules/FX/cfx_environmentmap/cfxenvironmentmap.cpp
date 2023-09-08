/*****************************************************************************
 *
 * File :     CFXEnvironmentMap.cpp
 *
 * Abstract : A behavior that allows control over RenderWare materials
 *            that contain environment maps
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

#include "rpmatfx.h"

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "cfxenvironmentmap.h"

#include "framework/core/macros/debugmacros.h"
#include "framework/toolkits/world/clevel.h"
#include "framework/toolkits/math/maths.h"
#include "framework/mainloop/render.h"
#include "framework/core/resourcemanager/cresourcemanager.h"

namespace RWS
{

   namespace FX
   {
      RWS_IMPLEMENT_CLASSID(CFXEnvironmentMap);
      RWS_REGISTERCLASS(CFXEnvironmentMap);

      /**
      *
      *  Construct a CFXEnvironmentMap object.
      *
      */
      CFXEnvironmentMap::CFXEnvironmentMap(const CAttributePacket& rAttr) : InitCEventHandler(0)
      {
         RWS_FUNCTION("RWS::FX::CFXEnvironmentMap::CFXEnvironmentMap");

         m_Source         = SOURCE_CAMERA;

         m_OrthoCount     = 0;

         m_UserRotation.x = 0.0f;
         m_UserRotation.y = 0.0f;
         m_UserRotation.z = 0.0f;

         m_AnimRotation.x = 0.0f;
         m_AnimRotation.y = 0.0f;
         m_AnimRotation.z = 0.0f;

         m_Scale          = 0;

         m_pEnvFrame   = RwFrameCreate();

         RWS_ASSERT(m_pEnvFrame, "Unable to create frame for environment maps");

         RwFrameSetIdentity(m_pEnvFrame);   

         m_pRotationMatrix = RwMatrixCreate();

         RWS_ASSERT(m_pRotationMatrix, "Unable to create matrix for environment maps");

         RwMatrixSetIdentity(m_pRotationMatrix);

         RegisterMsg(m_UpdateEvent, iMsgDoRenderStr, "RwCamera*");
         LinkMsg    (m_UpdateEvent, "RwCamera*");

         RWS_RETURNVOID();
      }

      /**
      *
      *  Destroy a CFXEnvironmentMap object.
      *
      */
      CFXEnvironmentMap::~CFXEnvironmentMap(void) 
      {
         RWS_FUNCTION("RWS::FX::CFXEnvironmentMap::~CFXEnvironmentMap")

         UnLinkMsg(m_UpdateEvent);
         UnRegisterMsg(m_UpdateEvent);

         if (m_pEnvFrame)
         {
            RwFrameDestroy(m_pEnvFrame); 
         }

         if (m_pRotationMatrix)
         {
            RwMatrixDestroy(m_pRotationMatrix);
         }

         RWS_RETURNVOID();
      }

      /**
      *
      *  Handle events.
      *  
      */
      void CFXEnvironmentMap::HandleEvents(CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::FX::CFXEnvironmentMap::HandleEvents");

         if (pMsg.Id == m_UpdateEvent)
         {


            switch (m_Source)
            {
               case SOURCE_CAMERA:
                  {
                     // set the environment map frame to match the camera frame
                     RwCamera *pRwCamera = reinterpret_cast<RwCamera*>(pMsg.pData);

                     if (pRwCamera)
                     {
                        if (m_pEnvFrame)
                        {
                           RwV3d        TVect;
                           RwFrame     *CameraFrame;
                           CameraFrame = RwCameraGetFrame(pRwCamera); 

                           TVect.x = m_Scale; TVect.y = m_Scale; TVect.z = 1.0f;
                           RwFrameScale (m_pEnvFrame, &TVect, rwCOMBINEREPLACE);   
 
                           RwFrameTransform(m_pEnvFrame, &CameraFrame->modelling, rwCOMBINEPOSTCONCAT); 
                        }
                     }
                  }
               break;

               case SOURCE_ANIMATED:

                  // because we are concatenating the new rotation matrix onto the current environment map frame
                  // we need to orthonormalize the environment map frames matrix. The RenderWare API documentation
                  // recommends this happen at least once every 128 concatenations
                  RwMatrixOptimize(m_pRotationMatrix, 0); 
                  RwFrameTransform(m_pEnvFrame, m_pRotationMatrix, rwCOMBINEPOSTCONCAT); 
 

                  m_OrthoCount++;
                  if (m_OrthoCount > 128)
                  {
                     RwFrameOrthoNormalize(m_pEnvFrame);
                     m_OrthoCount = 0;
                  }
               break;

               default:
                  break;
            }
 
         }

         RWS_RETURNVOID();
      }

      /**
      *
      *  Update attributes.
      *
      */
      void CFXEnvironmentMap::HandleAttributes(const CAttributePacket& attr)
      {
         RWS_FUNCTION("RWS::FX::CFXEnvironmentMap::HandleAttributes");
         
         CAttributeHandler::HandleAttributes(attr);
   
         // deal with the attributes related to the environment map behavior
         CAttributeCommandIterator attrIt(attr, RWS_CLASSID_OF(CFXEnvironmentMap));
         
         while (!attrIt.IsFinished())
         {
            switch (attrIt->GetCommandId())
            {
               case CMD_UpdateEvent:
                  ReplaceLinkedMsg (m_UpdateEvent, attrIt->GetAs_RwChar_ptr(), "RwCamera*");
                  break;

               case CMD_Set_m_Source:
                  attrIt->GetCommandData(m_Source);
                  RWS_TRACE("CMD_Set_m_Source "<<m_Source);
                  break;

               case CMD_Set_m_UserRotation_x:
                  attrIt->GetCommandData(m_UserRotation.x);

                  RWS_TRACE("CMD_Set_m_UserRotation_x "<<m_UserRotation.x);
                  break;

               case CMD_Set_m_UserRotation_y:  
                  attrIt->GetCommandData(m_UserRotation.y);
            
                  RWS_TRACE("CMD_Set_m_UserRotation_y "<<m_UserRotation.y);
                  break;

               case CMD_Set_m_UserRotation_z:
                  attrIt->GetCommandData(m_UserRotation.z);

                  RWS_TRACE("CMD_Set_m_UserRotation_z "<<m_UserRotation.z);
                  break;

               case CMD_Set_m_AnimRotation_x:
                  attrIt->GetCommandData(m_AnimRotation.x);

                  RWS_TRACE("CMD_Set_m_AnimRotation_x "<<m_AnimRotation.x);
                  break;

               case CMD_Set_m_AnimRotation_y:  
                  attrIt->GetCommandData(m_AnimRotation.y);
            
                  RWS_TRACE("CMD_Set_m_AnimRotation_y "<<m_AnimRotation.y);
                  break;

               case CMD_Set_m_AnimRotation_z:
                  attrIt->GetCommandData(m_AnimRotation.z);

                  RWS_TRACE("CMD_Set_m_AnimRotation_z "<<m_AnimRotation.z);
                  break;

               case CMD_Set_m_Scale:
                  attrIt->GetCommandData(m_Scale);

                  RWS_TRACE("CMD_Set_m_Scale "<<m_Scale);
                  break;
            };
            ++attrIt;
         }

         UpdateEnvFrame();

         ProcessAttachments(attr, m_pEnvFrame);
        
         RWS_RETURNVOID();
      }

      /**
      *
      *  Updates m_pEnvFrame to reflect the current source type.
      *  
      *  
      */
      void CFXEnvironmentMap::UpdateEnvFrame(void)
      {
         RWS_FUNCTION("RWS::FX::CFXEnvironmentMap::UpdateEnvFrame");
         RwV3d          TVect;

         switch (m_Source)
         {
            case SOURCE_NONE:
               // set environment map frame to identity
               RwFrameSetIdentity(m_pEnvFrame);
               break;

            case SOURCE_CAMERA:
               break;

            case SOURCE_ANIMATED:
               {
                  // generate the rotation matrix that is multiplied onto m_pEnvFrame each game frame

                  TVect.x = m_Scale; TVect.y = m_Scale; TVect.z = 1.0f;
                  RwMatrixScale(m_pRotationMatrix, &TVect, rwCOMBINEREPLACE);   

                  TVect.x = 1.0f; TVect.y = 0.0f; TVect.z = 0.0f;
                  RwMatrixRotate(m_pRotationMatrix, &TVect, m_AnimRotation.x, rwCOMBINEPOSTCONCAT);   

                  TVect.x = 0.0f; TVect.y = 1.0f; TVect.z = 0.0f;
                  RwMatrixRotate(m_pRotationMatrix, &TVect, m_AnimRotation.y, rwCOMBINEPOSTCONCAT);   

                  TVect.x = 0.0f; TVect.y = 0.0f; TVect.z = 1.0f;
                  RwMatrixRotate(m_pRotationMatrix, &TVect, m_AnimRotation.z, rwCOMBINEPOSTCONCAT);   

                  RwFrameSetIdentity(m_pEnvFrame);
               }
               break;

            case SOURCE_USER:
               {
                  // generate a rotation matrix based on the rotation described by m_UserRotation 

                  TVect.x = m_Scale; TVect.y = m_Scale; TVect.z = 1.0f;
                  RwFrameScale(m_pEnvFrame, &TVect, rwCOMBINEREPLACE);   

                  TVect.x = 1.0f; TVect.y = 0.0f; TVect.z = 0.0f;
                  RwFrameRotate(m_pEnvFrame, &TVect, m_UserRotation.x, rwCOMBINEPOSTCONCAT);   

                  TVect.x = 0.0f; TVect.y = 1.0f; TVect.z = 0.0f;
                  RwFrameRotate(m_pEnvFrame, &TVect, m_UserRotation.y, rwCOMBINEPOSTCONCAT);   

                  TVect.x = 0.0f; TVect.y = 0.0f; TVect.z = 1.0f;
                  RwFrameRotate(m_pEnvFrame, &TVect, m_UserRotation.z, rwCOMBINEPOSTCONCAT);   
               }
               break;
         };

         RWS_RETURNVOID();
      }

   } // namespace FX
} //namespace RWS
