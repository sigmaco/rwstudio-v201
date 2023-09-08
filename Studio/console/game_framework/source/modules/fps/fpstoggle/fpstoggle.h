/*****************************************************************************
*
* File :     FPSToggle.h
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

#ifndef __FPS_TOGGLE_H__
#define __FPS_TOGGLE_H__

#if defined (_MSC_VER) // C4786 : identifier was truncated to '255' characters in the browser information (Doesn't work for the map)
#pragma warning ( push )
#pragma warning ( disable : 4786 )
#endif // defined (_MSC_VER)

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "fpstoggleinterface.h"

namespace RWS
{
   namespace FPS
   {
      /**
      *
      *  \ingroup Mod_FPS
      *
      * Extend FPSToggleInterface to provide helper functions to generation the transformations
      * required by doors and buttons.
      *
      */
      class FPSToggle : public FPSToggleInterface
      {
      public:
         FPSToggle(const CAttributePacket& attr, RwReal angle, RwUInt32 lip, RwReal height, RwUInt32 speed) : 
            FPSToggleInterface(attr), 
               m_angle(angle), 
               m_lip(lip), 
               m_height(height), 
               m_speed(speed), 
               m_spawnflags(0),
               m_height_applied(false) {}
            
      protected:
         void Set_m_wait(const CAttributeCommand& cmd);
         void Set_m_angle(const CAttributeCommand& cmd);
         void Set_m_lip(const CAttributeCommand& cmd);
         void Set_m_height(const CAttributeCommand& cmd);
         void Set_m_speed(const CAttributeCommand& cmd);
         void Set_m_spawnflags(const CAttributeCommand& cmd);
         
         RwUInt32 Get_m_wait(void);
         RwReal Get_m_angle(void);
         RwUInt32 Get_m_lip(void);
         RwReal Get_m_height(void);
         RwUInt32 Get_m_speed(void);
         RwUInt32 Get_m_spawnflags(void);
         
         static RpAtomic *CalculateBoundingBox(RpAtomic * atomic, void *data);
         void GenerateAngleTransform(void);                      /**< Used by func_door func_button */
         void GetAngleTransform(RwV3d &translation);             /**< Used when modifying Set_start_open */           
         bool GenerateHeightTransform(RwV3d *translation = 0);   /**< Used by func_plat, returns translation */
         
         enum {
            Door_Starts_Open = 1,
               Door_Not_Linked = 4
         };
         
      private:
         RwReal m_angle;         /**< angle toggle object moves, note -2 -1 have special meanings */
         RwUInt32 m_lip;         /**< Lip, modifies movement distance allows buttons to remain proud of a surface */
         RwReal m_height;        /**< Used by platforms func_plat */
         RwUInt32 m_speed;       /**< Speed units per second */
         RwUInt32 m_spawnflags;  /**< spawnflags */            
         RwBool m_start_open;    /**< Flag specifies that this toggle starts in the open position, 
                                       i.e. need to reverse transform and set poition to end of transform */            
         RwBool m_height_applied;/**< Indicates if the height attribute for a platform has been applied to the
                                       transformation matrix */
      };
   }
}
#endif