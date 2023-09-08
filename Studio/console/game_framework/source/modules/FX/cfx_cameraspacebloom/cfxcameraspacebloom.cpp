/*****************************************************************************
 *
 * File :     CFXCameraSpaceBloom.cpp
 *
 * Abstract : 
 *            
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
#pragma exceptions off // Turn off exceptions due to bug in MW Dwarf Generator for PS2, present in CW 3.04
#endif


//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//

#include "framework/core/macros/debugmacros.h"
#include "framework/toolkits/world/crplevel.h"
#include "framework/toolkits/math/maths.h"
#include "framework/mainloop/render.h"
#include "framework/core/resourcemanager/cresourcemanager.h"
#include "cfxcameraspacebloom.h"


namespace RWS
{
                                                  
   namespace FX 
   {
                            
      RWS_IMPLEMENT_CLASSID(CFXCameraSpaceBloom);
      RWS_REGISTERCLASS(CFXCameraSpaceBloom);

      /**
      *
      *  Construct a CFXCameraSpaceBloom object.
      *
      */
      CFXCameraSpaceBloom::CFXCameraSpaceBloom(const CAttributePacket& rAttr) : InitCEventHandler(0)
      {
         RWS_FUNCTION("RWS::FX::CFXCameraSpaceBloom::CFXCameraSpaceBloom");
                    

         RWS_RETURNVOID();
      }

      /**
      *
      *  Destroy a CFXCameraSpaceBloom object.
      *
      */
      CFXCameraSpaceBloom::~CFXCameraSpaceBloom()
      {
         RWS_FUNCTION("RWS::FX::CFXCameraSpaceBloom::~CFXCameraSpaceBloom");

         UnLinkMsg(m_StartRender);
         UnRegisterMsg(m_StartRender);
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
      void CFXCameraSpaceBloom::HandleEvents(CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::FX::CFXCameraSpaceBloom::HandleEvents");

         if (pMsg.Id == m_StartRender)
         {
            ProcessCamera(reinterpret_cast<RwCamera*>(pMsg.pData));
         }

         RWS_RETURNVOID();
      }

      /**
      *
      *  Update attributes.
      *
      */
      void CFXCameraSpaceBloom::HandleAttributes(const CAttributePacket& attr)
      {
         RWS_FUNCTION("RWS::FX::CFXCameraSpaceBloom::HandleAttributes");
         
         CAttributeHandler::HandleAttributes(attr);
         
         // deal with the attributes related to the environment map behaviour
         CAttributeCommandIterator attrIt(attr, RWS_CLASSID_OF(CFXCameraSpaceBloom));
         
         while (!attrIt.IsFinished())
         {
            switch (attrIt->GetCommandId())
            {
               case CMD_Start_Render:

                  // If already linked, unlink and unregister as we are about to register with
                  // a new event.
                  UnLinkMsg(m_StartRender);
                  UnRegisterMsg(m_StartRender);
                  // register the new event
                  RegisterMsg(m_StartRender, attrIt->GetAs_RwChar_ptr(), "RwCamera*");
                  LinkMsg(m_StartRender, "RwCamera*");
                  break;
               case CMD_Priority:
                  //////////////////////////////////////////////////////////////////
                  m_link_priority = attrIt->GetAs_RwUInt32();

                  // If already linked, unlink as were about to relink at a different priority
                  //
                  UnLinkMsg(m_StartRender);
                  LinkMsg(m_StartRender, "RwCamera*",m_link_priority);
                  break;
               case CMD_Set_Brightness:
               {
                  m_Brightness = attrIt->GetAs_RwReal();
                  break;
               }


               case CMD_Set_StartColor:
               {
                  RwUInt32 Color;
                     
                  // Get color data from attribute form
                  attrIt->GetCommandData(Color);

                  // normalize RGBA elements and convert to RwReal
                  m_StartColor.red   = (RwReal) ((Color&0x00ff0000)>>16) / 255.0f;
                  m_StartColor.green = (RwReal) ((Color&0x0000ff00)>>8)  / 255.0f;
                  m_StartColor.blue  = (RwReal) ((Color&0x000000ff))     / 255.0f;
                  m_StartColor.alpha = (RwReal) ((Color&0xff000000)>>24) / 255.0f;
                  
                  break;
               }

               case CMD_Set_EndColor:
               {
                  RwUInt32 Color;
                     
                  // Get color data from attribute form
                  attrIt->GetCommandData(Color);
                                    
                  // normalize RGBA elements and convert to RwReal
                  m_EndColor.red   = (RwReal) ((Color&0x00ff0000)>>16) / 255.0f;
                  m_EndColor.green = (RwReal) ((Color&0x0000ff00)>>8)  / 255.0f;
                  m_EndColor.blue  = (RwReal) ((Color&0x000000ff))     / 255.0f;
                  m_EndColor.alpha = (RwReal) ((Color&0xff000000)>>24) / 255.0f;
                  
                  break;
               }
               case CMD_Set_Seperation:
               {
                  m_Separation = attrIt->GetAs_RwUInt32();
                  break;
               }
               case CMD_Set_SeperationStep:
               {
                  m_SeparationStep = attrIt->GetAs_RwUInt32();
                  break;
               }


            }
            ++attrIt;
         }
         
         RWS_RETURNVOID();
      }

      RwCamera *CFXCameraSpaceBloom::CreateOffScreenRasterCamera(RwUInt32 Width, RwUInt32 Height, RwUInt32 Depth)
      {
         RWS_FUNCTION("RWS::FX::CFXCameraSpaceBloom::CreateOffScreenRasterCamera");
         RwRaster *Raster;
         RwRaster *RasterZ;

         RwCamera *Camera = RwCameraCreate();
   
         RWS_POST(Camera);

         Raster = RwRasterCreate(Width, Height, 0, rwRASTERTYPECAMERATEXTURE);

         RasterZ = RwRasterCreate(Width, Height, Depth, rwRASTERTYPEZBUFFER);

         RwCameraSetRaster (Camera, Raster);
         RwCameraSetZRaster(Camera, RasterZ);

         RwFrame *Frame = RwFrameCreate();
         RwFrameSetIdentity(Frame);
   
         RwCameraSetFrame (Camera, Frame);
   
         RwCameraSetProjection (Camera, rwPARALLEL);

         //-----------------------------------------------------
         //
         RwCameraSetNearClipPlane (Camera, 1.0f);
         RwCameraSetFarClipPlane (Camera, 5000.0f);

         //-----------------------------------------------------
         //
         RwReal Aspect;
      
         if (Height != 0) 
         {
            Aspect = (RwReal)Width / (RwReal)Height;
   
            RwV2d  Viewwindow;

            Viewwindow.x = 1.0f;
            Viewwindow.y = 1.0f;
   
            RwCameraSetViewWindow (Camera, &Viewwindow);
         }

         RWS_RETURN(Camera);
      }


   } // namespace FX
} //namespace RWS
