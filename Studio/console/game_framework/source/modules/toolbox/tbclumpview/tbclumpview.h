/*****************************************************************************
*
* File :      TBClumpView.h
*
* Abstract :  Behaviour that displays a RpClump in-game, and can show
*             information about it such as tristrips, normals, etc.
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

#ifndef TBCLUMPVIEW_H_
#define TBCLUMPVIEW_H_

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "framework/core/attributehandler/cattributehandler.h"
#include "framework/core/eventhandler/ceventhandler.h"
#include "framework/toolkits/world/smartptrs/cclumpptr.h"
#include "framework/mainloop/logic.h"

#include "framework/core/attributehandler/allocation policies/LinearAllocationPolicy.h"

namespace RWS
{
   namespace ToolBox
   {
      /**
      *
      *  \ingroup ToolBox
      *
      *  This behavior can be used to display information about the geometry of a RpClump
      *  in various different ways. 
      *
      *  - Display vertex normals as lines.
      *  - Display the RpClump in wire-frame.
      *  - Visualize the tristrips in a RpClump by drawing each strip
      *    flat shaded, in a different color.
      *  - Visualize the length of the tristrips in a RpClump by drawing the first
      *    N triangles in red, where N is a user-specified number.
      *
      *  \note The RenderWare Visualizer performs all of the functions of this behavior
      *        and is better suited to general use for checking art assets on the target
      *        platform. However, this behavior also serves as an example of how to 
      *        access geometry data within a RpClump.
      *
      */
      class ATBClumpView : public CSystemCommands, public CAttributeHandler, public CEventHandler, public LinearAllocationPolicy
      {
      public:

         RWS_MAKENEWCLASS(ATBClumpView);
         RWS_DECLARE_CLASSID(ATBClumpView);
         RWS_CATEGORY("Tools");
         RWS_DESCRIPTION("View a RpClump", "Visualize some of the rendering data for a RpClump, "
                                           "for example tristrips and vertex normals.");

         RWS_BEGIN_COMMANDS

         RWS_SEPARATOR("Rendering", 1)
            RWS_MESSAGE( CMD_Start_Render, "Render Event", 
                         "Render the RpClump with the RwCamera passed in the message.", 
                         RECEIVE , RwCamera*, 0)
            RWS_ATTRIBUTE( CMD_RenderPriority, "Render Priority", 
                           "Set the priority of this entity in the rendering event",
                           SLIDER, RwUInt32, RANGE(0,65535,65535))
            RWS_ATTRIBUTE( CMD_MeshShow, "Display Meshes", 
                           "Toggle display of the tristripped RpMesh(es) of the RpClump flat-shaded and untextured.", 
                           BOOLEAN, RwUInt32, DEFAULT(0))
            RWS_ATTRIBUTE( CMD_WireShow, "Display WireMeshes", 
                           "Toggle display of the RpMesh(es) of the RpClump in wire-frame.", 
                           BOOLEAN, RwUInt32, DEFAULT(0))
            RWS_ATTRIBUTE( CMD_StripShow, "Display TriStrips",
                           "Toggle display of the tristrips of the RpClump.", 
                           BOOLEAN, RwUInt32, DEFAULT(0))
            RWS_ATTRIBUTE( CMD_StripShowLength, "Display TriStrip Length", 
                           "Toggle display of tristrips shorter than the current tristrip length. ",
                           BOOLEAN, RwUInt32, DEFAULT(0))
            RWS_ATTRIBUTE( CMD_StripSize, "TriStrip Length", "Tristrips shorter than the given length "
                           "are drawn in red, if 'Display TriStrip Length' is checked.",
                           SLIDER, RwInt32, RANGE(0,1,300))
            RWS_ATTRIBUTE( CMD_NormalShow, "Display Normals", "Toggle display of the vertex normals of the RpClump.", 
                           BOOLEAN, RwUInt32, DEFAULT(0))
            RWS_ATTRIBUTE( CMD_ScaleFactor, "Normal Size", "Length of the lines used to draw the vertex normals.", 
                           SLIDER, RwReal, RANGE(0,1,300))

         RWS_SEPARATOR("Rotation", 0)
            RWS_ATTRIBUTE( CMD_SpinToggle, "Spin", "Toggle automatic spinning on / off.", 
                           BOOLEAN, RwUInt32, DEFAULT(0))
            RWS_ATTRIBUTE( CMD_rot_x, "X Rotation", "Specify the x axis rotation", 
                           SLIDER, RwReal, RANGE(0,0,360))
            RWS_ATTRIBUTE( CMD_rot_y, "Y Rotation", "Specify the y axis rotation", 
                           SLIDER, RwReal, RANGE(0,0,360))
            RWS_ATTRIBUTE( CMD_rot_z, "Z Rotation", "Specify the z axis rotation", 
                           SLIDER, RwReal, RANGE(0,0,360))

         RWS_END_COMMANDS;

         virtual void HandleEvents(CMsg &pMsg);
         virtual void HandleAttributes(const CAttributePacket& attr);

         ATBClumpView(const CAttributePacket& attr);
         ~ATBClumpView(void);

      protected:

         RwMatrix m_Matrix;      /**< Matrix used for rotating object. */
         CClumpPtr m_pClump;     /**< Behavior's Clump. */
         RwReal m_Rot[3];        /**< RwReal array for storing rotation values. */
         CEventId m_StartRender; /**< Event used to specify when rendering should begin with the specified RwCamera. */
         RwUInt32 m_RenderPriority; /**< Priority of this eventhandler in the rendering event. */

         RwUInt32	m_bStripShow;  /**< Flag to toggle  rendering tristrips on / off. */
         RwUInt32	m_bSpinToggle; /**< Flag to toggle  automatic spinning on / off. */
         RwUInt32	m_bMeshShow;   /**< Flag to toggle  rendering flat-shaded mesh on / off. */
         RwUInt32	m_bWireShow;   /**< Flag to toggle  rendering wire-frame mesh on / off. */
         RwUInt32	m_bNormalShow; /**< Flag to toggle  rendering vertex normals on / off. */
         RwReal m_NormalsScaleFactor;  /**< Length of lines used to represent vertex normals. */
         RwUInt32 m_bStripShowLength;  /**< Flag to toggle rendering tristrips based on their length. */
         RwInt32 m_NumTriStripTriangles;  /**< Number of triangles to highlight in a strip, 
                                                when drawing strips by length. */
      };

   } // namespace ToolBox

} // namespace RWS

#endif // ifndef TBCLUMPVIEW_H_