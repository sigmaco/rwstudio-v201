/*****************************************************************************
*
* File :     FPSToggle.cpp
*
* Abstract : Extend FPSToggleInterface to provide helper functions for generation the transformations
*            required by doors and buttons.
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
// Left to its own devices, the MW linker will dead-strip the registrar ctors, so...
#ifdef __MWERKS__
#pragma force_active on
#endif

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//

#include "fpstoggle.h"
#include "framework/toolkits/math/maths.h" 

namespace RWS
{
   namespace FPS
   {
      /**
      *
      * the FPS module doors/ buttons etc use an angle and the dimensions 
      * of the geometry in order to create the transformation matrices, 
      * this function calculates these matrices.
      *
      */        
      void FPSToggle::GenerateAngleTransform(void)
      {
         RWS_FUNCTION("RWS::FPS::FPSToggle::GenerateAngleTransform");
         
         RwV3d translation;
         
         // The FPS module uses -2 and -1 as Special values to enable doors/buttons to move vertically
         if (m_angle == -2)
         {
            translation.x = 0.0f;
            translation.y = -1.0f;
            translation.z = 0.0f;
         }
         else if (m_angle == -1)
         {
            translation.x = 0.0f;
            translation.y = 1.0f;
            translation.z = 0.0f;
         }
         else
         {
            translation.x = static_cast<RwReal>(RwSin(RWDEG2RAD(m_angle)));
            translation.y = 0;
            translation.z = static_cast<RwReal>(RwCos(RWDEG2RAD(m_angle)));
         }
         
         // Need to Generate Bounding Box in order to determine distance to travel
         RwBBox bbox = {{-9999.0f, -9999.0f, -9999.0f}, {9999.0f, 9999.0f, 9999.0}};
         CalculateBoundingBox(m_pAtomic, &bbox);
         
         RwV3d size;
         
         // Size of geometry
         RwV3dSub(&size, &bbox.sup, &bbox.inf); 
         
         RwReal distance = static_cast<RwReal>(RwFabs(size.x * translation.x + size.z * translation.z + size.y * translation.y));
         distance = distance - m_lip;
         
         RwReal tdistance;
         
         if ( (Get_m_spawnflags() & Door_Starts_Open) == Door_Starts_Open )
         {
            // Reverse direction
            tdistance = -distance;  
         }
         else
         {
            tdistance = distance;
         }
         
         RwV3dScale(&translation, &translation, tdistance );
         
         RwV3d rotation = {0.0f, 0.0f, 0.0f};
         
         // m_speed -> units per second
         //
         // m_frame_count_off_to_on -> number of frames to complete movement
         // m_frame_count_on_to_off -> number of frames to complete movement
         //
         // distance -> units
         //
         // distance / m_speed -> units / (units / second) -> seconds
         //
         // seconds * RWS::MainLoop::Logic::Rate fps -> frames to complete movement
         //
         RwReal var = RWS::MainLoop::Logic::Rate * (distance / static_cast<RwReal>(m_speed));
         
         Set_m_frame_count_off_to_on(static_cast<RwUInt32>(var));
         Set_m_frame_count_on_to_off(static_cast<RwUInt32>(var));
         
         RWS_TRACE("FPSToggle::GenerateAngleTransform "
            <<translation.x<<" "
            <<translation.y<<" "
            <<translation.z<<" "
            <<rotation.x<<" "
            <<rotation.y<<" "
            <<rotation.z);
         
         FPSToggleInterface::GenerateTransformationMatrices(translation, rotation);
         
         RWS_RETURNVOID();
      }
      
      
      /**
      *
      * Generate height transform matrices for the FPS module platforms.
      *
      */        
      bool FPSToggle::GenerateHeightTransform(RwV3d *ret_translation)
      {
         RWS_FUNCTION("RWS::FPS::FPSToggle::GenerateHeightTransform");
         
         RwV3d translation;
         
         translation.x = 0;
         translation.y = 1;
         translation.z = 0;
         
         RwReal distance;
         
         if ((m_height == 0))
         {
            // Need to Generate Bounding Box in order to determine distance to travel
            RwBBox bbox = {{-9999.0f, -9999.0f, -9999.0f}, {9999.0f, 9999.0f, 9999.0}};   
            CalculateBoundingBox(m_pAtomic, &bbox);
            
            RwV3d size;
            
            // Size of geometry
            RwV3dSub(&size, &bbox.sup, &bbox.inf); 
            
            // size.x * translation.x + size.z * translation.z;
            distance = static_cast<RwReal>(RwFabs(size.y * translation.y));
         }
         else
         { 
            distance = m_height;
         }
         
         distance = distance - m_lip;
         
         RwV3dScale(&translation, &translation, distance);
         RwV3d rotation = {0.0f, 0.0f, 0.0f};
         
         // m_speed -> units per second
         //
         // m_frame_count_off_to_on -> number of frames to complete movement
         // m_frame_count_on_to_off -> number of frames to complete movement
         // distance -> units
         //
         // distance / m_speed -> units / (units / second) -> seconds
         //
         // seconds * RWS::MainLoop::Logic::Rate fps -> frames to complete movement
         //
         RwReal var = RWS::MainLoop::Logic::Rate * (distance / static_cast<RwReal>(m_speed));
         
         Set_m_frame_count_off_to_on(static_cast<RwUInt32>(var));
         Set_m_frame_count_on_to_off(static_cast<RwUInt32>(var));
         
         FPSToggleInterface::GenerateTransformationMatrices(translation, rotation);
         
         if (ret_translation) *ret_translation = translation;
         
         RWS_RETURN(true);
      }
      
      
      RpAtomic   *FPSToggle::CalculateBoundingBox(RpAtomic * atomic, void *data)
      {
         RWS_FUNCTION("RWS::FPS::FPSToggle::CalculateBoundingBox");
         
         RWS_PRE( atomic )
            RWS_PRE( data )
            
            RwBBox *bbox = reinterpret_cast<RwBBox*>(data);
         
         RpGeometry *geom = RpAtomicGetGeometry(atomic);
         
         if (geom)
         {
            RpMorphTarget *morph = RpGeometryGetMorphTarget(geom, 0);
            
            if (morph)
            {
               RwBBox tbbox;
               
               RwBBoxCalculate(&tbbox, RpMorphTargetGetVertices(morph), RpGeometryGetNumVertices(geom));
               RwMatrix *ltm = RwFrameGetLTM(RpAtomicGetFrame(atomic));
               
               RwV3d points[8];
               points[0].x = tbbox.inf.x; points[0].y = tbbox.inf.y; points[0].z = tbbox.inf.z; 
               points[1].x = tbbox.sup.x; points[1].y = tbbox.inf.y; points[1].z = tbbox.inf.z; 
               points[2].x = tbbox.sup.x; points[2].y = tbbox.sup.y; points[2].z = tbbox.inf.z; 
               points[3].x = tbbox.inf.x; points[3].y = tbbox.sup.y; points[3].z = tbbox.inf.z; 
               
               points[4].x = tbbox.inf.x; points[4].y = tbbox.inf.y; points[4].z = tbbox.sup.z; 
               points[5].x = tbbox.sup.x; points[5].y = tbbox.inf.y; points[5].z = tbbox.sup.z; 
               points[6].x = tbbox.sup.x; points[6].y = tbbox.sup.y; points[6].z = tbbox.sup.z; 
               points[7].x = tbbox.inf.x; points[7].y = tbbox.sup.y; points[7].z = tbbox.sup.z; 
               
               RwV3dTransformPoints(points, points, 8, ltm);
               RwBBoxCalculate(bbox, points, 8);
            }
         }
         
         RWS_RETURN(atomic);
      }
      
      
      void FPSToggle::Set_m_angle(const CAttributeCommand& cmd)
      {
         RWS_FUNCTION("RWS::FPS::FPSToggle::Set_m_angle");
         
         cmd.GetCommandData(m_angle);
         
         GenerateAngleTransform();
         
         // Because the FPS module uses -2 and -1 as special values for doors
         // that act as lifts, we must disable cancel event (which
         // re-opens a door if the player is stood
         // in the way while it is closing ) otherwise lift
         // would cancel moving while the player was stood on it
         if (m_angle == -2 || m_angle == -1)
         {
            Set_m_msg_rx_on_to_off_cancel(ERROR_NOT_A_VALID_MESSAGE_ID);
         }
         else
         {
            Set_m_msg_rx_on_to_off_cancel("ACTN_PLAYERTOUCH");
         }
         
         RWS_RETURNVOID();
      }
      
      
      void FPSToggle::Set_m_wait(const CAttributeCommand& cmd)
      {
         RWS_FUNCTION("RWS::FPS::FPSToggle::Set_m_wait");
         
         RwReal wait;
         
         cmd.GetCommandData(wait);
         
         if ( wait == -1 )
         {
            Set_m_msg_rx_on_to_off(ERROR_NOT_A_VALID_MESSAGE_ID);
         }
         else
         {
            Set_m_msg_rx_on_to_off(iMsgRunningTick);
            m_num_msg_rx_on_to_off = static_cast<RwUInt32>(wait*RWS::MainLoop::Logic::Rate);
         }
         
         RWS_RETURNVOID();
      }
      
      
      void FPSToggle::Set_m_lip(const CAttributeCommand& cmd)
      {
         RWS_FUNCTION("RWS::FPS::FPSToggle::Set_m_lip");
         cmd.GetCommandData(m_lip);
         GenerateAngleTransform();
         RWS_RETURNVOID();
      }
      
      
      void FPSToggle::Set_m_speed(const CAttributeCommand& cmd)
      {
         RWS_FUNCTION("RWS::FPS::FPSToggle::Set_m_speed");
         cmd.GetCommandData(m_speed);
         RWS_RETURNVOID();
      }
      
      
      void FPSToggle::Set_m_spawnflags(const CAttributeCommand& cmd)
      {
         RWS_FUNCTION("RWS::FPS::FPSToggle::Set_m_spawnflags");
         cmd.GetCommandData(m_spawnflags);
         RWS_RETURNVOID();
      }
      
      
      RwUInt32 FPSToggle::Get_m_wait(void)
      {
         RWS_FUNCTION("RWS::FPS::FPSToggle::Set_m_wait");
         RWS_RETURN(static_cast<RwUInt32>(m_num_msg_rx_on_to_off / RWS::MainLoop::Logic::Rate)); 
      }
      
      
      RwReal FPSToggle::Get_m_angle(void)
      {
         RWS_FUNCTION("RWS::FPS::FPSToggle::Set_m_angle");
         RWS_RETURN(m_angle);
      }
      
      
      RwUInt32 FPSToggle::Get_m_lip(void)
      {
         RWS_FUNCTION("RWS::FPS::FPSToggle::Get_m_lip");
         RWS_RETURN(m_lip);
      }
      
      
      RwReal FPSToggle::Get_m_height(void)
      {
         RWS_FUNCTION("RWS::FPS::FPSToggle::Get_m_height");
         RWS_RETURN(m_height);
      }
      
      
      RwUInt32 FPSToggle::Get_m_speed(void)
      {
         RWS_FUNCTION("RWS::FPS::FPSToggle::Get_m_speed");
         RWS_RETURN(m_speed);
      }
      
      
      RwUInt32 FPSToggle::Get_m_spawnflags(void)
      {
         RWS_FUNCTION("RWS::FPS::FPSToggle::Get_m_spawnflags");
         RWS_RETURN(m_spawnflags);
      }
      
      
      void FPSToggle::Set_m_height(const CAttributeCommand& cmd)
      {
         RWS_FUNCTION("RWS::FPS::FPSToggle::Set_m_height");
         
         // Remove old height offset
         RwFrame* pFrame = RpAtomicGetFrame(m_pAtomic.ptr());
         
         // If the height applied flag is set then we need to remove the old translation 
         
         if (m_height_applied)
         {
            if (pFrame)
            {
               RwV3d translation;
               
               if (GenerateHeightTransform(&translation))
               {
                  RwFrameTranslate(pFrame, &translation, rwCOMBINEPOSTCONCAT);
               }
            }
            
            cmd.GetCommandData(m_height);
            
            // Apply new height offset
            if (pFrame)
            {
               RwV3d translation;
               
               if (GenerateHeightTransform(&translation))
               {
                  translation.y = - translation.y;
                  RwFrameTranslate(pFrame, &translation, rwCOMBINEPOSTCONCAT);
               }
            }
         }
         else
         {            
            // Set the height applied flag to indicate we now need to remove the old translation
            m_height_applied = true;
            
            cmd.GetCommandData(m_height);
         }
         
         RWS_RETURNVOID();
      }
      
      
      void FPSToggle::GetAngleTransform(RwV3d &translation)
      {
         RWS_FUNCTION("RWS::FPS::FPSToggle::GetAngleTransform");
         
         // The FPS module uses -2 and -1 as Special values to enable doors/buttons to move vertically
         if (m_angle == -2)
         {
            translation.x = 0.0f;
            translation.y = -1.0f;
            translation.z = 0.0f;
         }
         else if (m_angle == -1)
         {
            translation.x = 0.0f;
            translation.y = 1.0f;
            translation.z = 0.0f;
         }
         else
         {
            translation.x = static_cast<RwReal>(RwSin(RWDEG2RAD(m_angle)));
            translation.y = 0;
            translation.z = static_cast<RwReal>(RwCos(RWDEG2RAD(m_angle)));
         }
         
         // Need to Generate Bounding Box in order to determine distance to travel
         RwBBox bbox = {{-9999.0f, -9999.0f, -9999.0f},{9999.0f, 9999.0f, 9999.0}};
         CalculateBoundingBox(m_pAtomic, &bbox);
         
         RwV3d size;
         
         // Size of geometry
         RwV3dSub(&size, &bbox.sup, &bbox.inf); 
         RwReal distance = static_cast<RwReal>(RwFabs(size.x * translation.x + size.z * translation.z + size.y * translation.y));
         distance = distance - m_lip;
         RwV3dScale(&translation, &translation, distance );
         
         RWS_RETURNVOID();
      }
   } // namespace FPS
}   // namespace RWS
