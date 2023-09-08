/*****************************************************************************
*
* File :     FPSDoor.cpp
*
* Abstract : Doors can either move horizontally or vertically depending on 
*            the setting of angle
*
*            They are based on the FPSToggleInterface class, but directly derived from 
*            FPSToggle which handles initializing the FPSToggleInterface class.
* 
*            Doors which overlap, will act as a single door
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

// Unexposed RenderWare function...used to update atomic to sector info, this is needed otherwise
// collision tests don't work until after a RwCameraBeginUpdate, I believe this is being exposed in
// 3.2 release.
extern "C" 
{
   RwBool _rwFrameSyncDirty(void);
}


//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "fpsdoor.h"


namespace RWS
{
   namespace FPS
   {
      RWS_IMPLEMENT_CLASSID(FPSDoor);
      RWS_REGISTERCLASS(FPSDoor);
      
      CEventId FPSDoor::m_Inq_FPS_func_door;
      
      
      /**
      *
      * \ref FPSDoor(). Constructor for FPSDoor
      *
      * \see ~FPSDoor
      *
      */
      FPSDoor::FPSDoor(const CAttributePacket& attr) :
      FPSToggle(attr, 0, 8, 0, 100),
         m_bStartOpen (false),
         pLinked(0)
      {
         RWS_FUNCTION("RWS::FPS::FPSDoor::FPSDoor");
         
         // Initial state is State_off           
         Set_m_msg_rx_off_to_on("ACTN_PLAYERTOUCH");              // Receive Message off to on
         m_num_msg_rx_off_to_on = 1;                              // Number of messages off to on required to trigger
         
         Set_m_msg_rx_on_to_off(iMsgRunningTick);
         m_num_msg_rx_on_to_off = (3*RWS::MainLoop::Logic::Rate); // Number of messages on to off required to trigger
         Set_m_msg_rx_on_to_off_cancel("ACTN_PLAYERTOUCH");       // Receive Message off to on Reverse Event
         
         // This event is sent to a touching object (once it has been positioned) to determine
         // if it is a FPSDoor, pData is set to the sender, and the receiver stores this and sets it to its
         // this pointer, both doors then have a pointer to the other door stored in pLinked.
         //
         UnRegisterMsg(m_Inq_FPS_func_door);
         RegisterMsg(m_Inq_FPS_func_door, "m_Inq_FPS_func_door", "FPSDoor* - return FPSDoor*");
         
         LinkMsg(iMsgOnDeleteEntity, "CEventHandler*");
         
         RWS_RETURNVOID();
      }
      
      
      /**
      *
      * \ref ~FPSDoor(). Destructor for FPSDoor
      *      
      * \see FPSDoor
      *
      */                
      FPSDoor::~FPSDoor(void)
      {
         RWS_FUNCTION("RWS::FPS::FPSDoor::~FPSDoor");
         
         UnRegisterMsg(m_Inq_FPS_func_door);
         
         UnLinkMsg(iMsgOnDeleteEntity);
         
         RWS_RETURNVOID();
      }
      
      
      class CollisionData
      {
      public:
         
         CollisionData(RwBBox tbbox, FPSDoor *pFPS_func_door,FPSDoor **pLinked) :
            bbox(tbbox), pFPS_func_door(pFPS_func_door), pLinked(pLinked) {};
         
         RwBBox bbox;
         
         FPSDoor *pFPS_func_door;
         
         FPSDoor **pLinked;
      };
      
      
      /**
      *
      * \ref CheckIfTouching().  callback to check if 2 doors are touching.
      *
      * \param  intersection  Pointer to the intersection primitive.  
      * \param  sector        Pointer to the world sector containing the intersected triangles.  
      * \param  atomic        Pointer to the intersected atomic.  
      * \param  distance      The collision distance. The distance returned depends on the intersection
      *         type which is defined in RpIntersectType. 
      *
      *         rpINTERSECTPOINT Distance of point from atomic's bounding sphere center, normalized to
      *           sphere's radius. 
      *
      *         rpINTERSECTLINE Distance of atomic's bounding-sphere center from start of line, projected
      *           onto the line, normalized to length of line. Note that by this definition, if the line
      *           starts or ends inside the sphere, this distance maybe negative or greater than one. 
      *
      *         rpINTERSECTSPHERE Distance of atomic's bounding-sphere center from sphere's center, normalized
      *           to sum of spheres' radii. 
      *
      *         rpINTERSECTBOX Distance undefined 
      *
      *         rpINTERSECTATOMIC Distance between atomics' bounding-sphere centers, normalized to sum
      *           of spheres' radii. 
      *
      * \param  data  User defined data pointer 
      *
      * \return RpAtomic of a touching door,  0 otherwise.
      *
      * \see SearchForLinks, ClearLinks
      *
      */
      RpAtomic   *FPSDoor::CheckIfTouching(RpIntersection * intersection,
         RpWorldSector * sector,
         RpAtomic * atomic,
         RwReal distance,
         void *data)
      {
         RWS_FUNCTION("RWS::FPS::FPSDoor::CheckIfTouching");
         
         // Check if the bounding box's overlap,
         CollisionData *pdata = reinterpret_cast<CollisionData*>(data);
         
         // Does overlap
         CEventHandler *pCEventHandler = RpAtomicToEventHandler::GetEventHandler( (*atomic) );
         
         if (pCEventHandler && pCEventHandler != static_cast<CEventHandler*>(pdata->pFPS_func_door))
         {
            CMsg msg(m_Inq_FPS_func_door, pdata->pFPS_func_door);
            
            // Note: need to force use of RWS::SendMsgToEventHandler otherwise
            // picks up RWS::CEventHandler::SendMsgToEventHandler
            //
            RWS::SendMsgToEventHandler(msg, pCEventHandler);
            
            if (msg.pData != pdata->pFPS_func_door)
            {
               RWS_TRACE("FPSDoor::CheckIfTouching Linking Doors");
               
               // Store pointer to other
               *pdata->pLinked = static_cast<FPSDoor*>(msg.pData); 
               
               RWS_TRACE("FPSDoor::CheckIfTouching Door overlaps, setting linkage from "
                  <<RWS_HEX(pdata->pFPS_func_door)<<" to "<<RWS_HEX(*pdata->pLinked));
               
               // Don't search any further
               RWS_RETURN( 0 );   
            }
         }
         
         RWS_RETURN( atomic );
      }
      
      
      /**
      *
      * \ref SearchForLinks(). Check if this object overlaps any others as doors that overlap
      * work as a pair....
      *                
      * Method : Find intersection to determine overlapping atomic/clump
      * using RenderWare collision detection.
      *        
      * The atomic can gain a pointer to the RpAtomic via the plugin
      * and from this a pointer to the event handler CEventhandler.
      *        
      * Need to query the CEventHandlers to determine if they are FPSDoor's,
      * To do this Register an event and send it to the event handler if the event handler
      * process's the event return it's (this) pointer then its a FPSDoor
      * 
      * \see CheckIfTouching, ClearLinks
      */
      void FPSDoor::SearchForLinks(void)
      {
         RWS_FUNCTION("RWS::FPS::FPSDoor::SearchForLinks");
         
         // Clear any existing linkage
         ClearLinks();
         
         // Only search if necessary
         if (!NotLinked())
         {               
            RwBBox bbox;
            
            CalculateBoundingBox(m_pAtomic, &bbox);
            
            CollisionData data(bbox, this, &pLinked);
            
            RpIntersection i;
            i.type = rpINTERSECTBOX;
            i.t.box = bbox;
            
            // Fix for testing collisions before the atomic has been rendered
            _rwFrameSyncDirty(); 
            
            RpWorldForAllAtomicIntersections( RpAtomicGetWorld(m_pAtomic), &i, CheckIfTouching, &data );
         }
         RWS_RETURNVOID();
      }
      
      
      /**
      *
      * \ref ClearLinks()
      *
      * \see CheckIfTouching, SearchForLinks
      */
      void FPSDoor::ClearLinks(void)
      {
         RWS_FUNCTION("RWS::FPS::FPSDoor::ClearLinks");
         
         if (pLinked)
         {
            pLinked->pLinked = 0;
            pLinked = 0;
         }
         
         RWS_RETURNVOID();
      }
      
      
      /**
      *       
      * \ref StartsOpen().  Check if the door starts open.
      *
      * \return TRUE if door starts open and FALSE otherwise.
      *
      * \see NotLinked
      *
      */
      RwBool FPSDoor::StartsOpen()
      {
         RWS_FUNCTION("RWS::FPS::FPSDoor::StartsOpen");
         
         RwBool result = (0 != (Get_m_spawnflags() & Door_Starts_Open));
         
         RWS_RETURN(result);
      }
      
      
      /**
      *
      * \ref NotLinked().  Check if the door is linked to another door.
      *
      * \return TRUE if not linked and FALSE if linked.
      *
      * \see StartOpen()
      *
      */
      RwBool FPSDoor::NotLinked()
      {
         RWS_FUNCTION("RWS::FPS::FPSDoor::NotLinked");
         
         RwBool result = (0 != (Get_m_spawnflags() & Door_Not_Linked));
         
         RWS_RETURN(result);
      }
      
      
      /**
      *
      * \ref ApplyOpenTranslation().  Open the doors. Uses \ref GetAngleTransform
      *
      * \param scale Scale to scale the movement of the doors opening.
      */
      void FPSDoor::ApplyOpenTranslation(RwReal scale)
      {
         RWS_FUNCTION("RWS::FPS::FPSDoor::ApplyOpenTranslation")
            
            RwFrame* pFrame = RpAtomicGetFrame(m_pAtomic.ptr());
         
         if (pFrame)
         {
            RwV3d openXLation;
            GetAngleTransform(openXLation);
            RwV3dScale(&openXLation, &openXLation, scale);
            RwFrameTranslate(pFrame, &openXLation, rwCOMBINEPOSTCONCAT);
         }
         
         RWS_RETURNVOID();
      }
      
      
      /**
      *     
      * \ref HandleAttributes().  Handle Attributes for this behavior
      *
      * \param attr Standard Attribute Package       
      *
      */
      void FPSDoor::HandleAttributes(const CAttributePacket& attr)
      {
         RWS_FUNCTION("RWS::FPS::FPSDoor::HandleAttributes");
         
         // Give base class first dibs
         FPSToggleInterface::HandleAttributes(attr);
         
         // Work through the effects of any load matrix command - it could
         // affect linkage to other doors and/or the door may have been
         // reset from its initial open position
         RwBool doSearchForLinks = false;
         
         CAttributeCommandIterator attrIt(attr, RWS_CLASSID_OF(CSystemCommands));
         
         while (!attrIt.IsFinished())
         {
            switch (attrIt->GetCommandId())
            {
            case CSystemCommands::CMD_LoadMatrix:
               {
                  // If the door is initially open, its position will have
                  // been overwritten by the load matrix command, so we need
                  // to re-open it
                  if (StartsOpen()) ApplyOpenTranslation(1.0f);
                  
                  // Search for links (NB: deferred until end of handler for
                  // efficiency - other commands could trigger a linkage search
                  // too)
                  doSearchForLinks = true;
               }
               break;
            }
            ++attrIt;
         }
         
         // Now handle FPSDoor commands
         attrIt = CAttributeCommandIterator(attr, RWS_CLASSID_OF(FPSDoor));
         
         while (!attrIt.IsFinished())
         {
            switch (attrIt->GetCommandId())
            {
            case CMD_targetname:
               RWS_TRACE("FPSDoor::HandleAttributes CMD_targetname");
               
               {
                  const RwChar *name;
                  
                  attrIt->GetCommandData(&name);
                  
                  Set_m_msg_rx_off_to_on(name);
               }
               break;
               
            case CMD_angle:
               RWS_TRACE("FPSDoor::HandleAttributes CMD_angle");
               
               // This is likely to change the direction in which the door opens,
               // so if the door is already open, return it to its closed position,
               // change the angle and re-open. This could also affect linkage, so
               // check that as well.
               if (StartsOpen())
               {
                  ApplyOpenTranslation(-1.0f);
                  Set_m_angle(*attrIt);
                  ApplyOpenTranslation(1.0f);
                  doSearchForLinks = true;
               }
               else
               {
                  Set_m_angle(*attrIt);
               }
               break;
               
            case CMD_wait:
               RWS_TRACE("FPSDoor::HandleAttributes CMD_wait");
               
               Set_m_wait(*attrIt);
               break;
               
            case CMD_lip:
               RWS_TRACE("FPSDoor::HandleAttributes CMD_lip");
               
               // As for CMD_angle, this can change the open position.
               if (StartsOpen())
               {
                  ApplyOpenTranslation(-1.0f);
                  Set_m_lip(*attrIt);
                  ApplyOpenTranslation(1.0f);
                  doSearchForLinks = true;
               }
               else
               {
                  Set_m_lip(*attrIt);
               }
               break;
               
            case CMD_speed:
               RWS_TRACE("FPSDoor::HandleAttributes CMD_speed");
               
               Set_m_speed(*attrIt);
               
               GenerateAngleTransform();
               break;
               
            case CMD_spawnflags:
               RWS_TRACE("FPSDoor::HandleAttributes CMD_spawnflags");
               {
                  RwUInt32 spawnflags;
                  
                  attrIt->GetCommandData(spawnflags);
                  
                  // Check if door is linked
                  if ( (spawnflags&Door_Not_Linked) == Door_Not_Linked )
                  {
                     ClearLinks();
                  }
                  
                  // Check if door starts open
                  if ( (spawnflags&Door_Starts_Open) != Door_Starts_Open )   // Starts Open not set i.e. starts closed
                  {
                     // Was Starts Open, need to remove transform
                     if (StartsOpen()) 
                     {
                        ApplyOpenTranslation(-1.0f);
                        doSearchForLinks = true;
                     }
                  }
                  else   // Starts Open
                  {
                     // Was Start closed, need to apply transform
                     if (!StartsOpen()) 
                     {
                        ApplyOpenTranslation(1.0f);
                        doSearchForLinks = true;
                     }
                  }
                  
                  // Update spawnflags
                  Set_m_spawnflags(*attrIt);
               }
               break;
            }
            ++attrIt;
         }
          
         // Search for links, if necessary
         if (doSearchForLinks) SearchForLinks();
          
         RWS_RETURNVOID();
      }
       
       
      /**
      *
      * \ref HandleEvents(). Handle events       
      *
      * \param msg Standard Message Package
      *  
      * \ref iMsgRunningTick
      */
      void FPSDoor::HandleEvents(CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::FPS::FPSDoor::HandleEvents");
         
         RWS_TRACE("FPSDoor " << this << " " <<pMsg.Id->p_msgname << " "<< Get_m_state());
         
         // Query Door Object ?
         if (pMsg.Id == m_Inq_FPS_func_door)
         {
            pLinked = static_cast<FPSDoor*>(pMsg.pData);
            
            RWS_TRACE("FPSDoor::HandleEvents link from "<<RWS_HEX(this)<<" to "<<pLinked);
            
            pMsg.pData = this;
            
            // Processing complete
            RWS_RETURNVOID(); 
         }
         
         // Because this event handler maintains a pointer to another eventhandler, it needs to
         // monitor the iMsgOnDeleteEntity
         if (pMsg.Id == iMsgOnDeleteEntity)
         {
            if (pMsg.pData == pLinked) 
               pLinked = 0;
            
            // Processing complete
            RWS_RETURNVOID();  
         }
         
         switch (Get_m_state())
         {
         case State_off:
            // If this door is triggered, pass on event to other door
            if (pMsg.Id == Get_m_msg_rx_off_to_on()) 
            {
               GenerateAngleTransform();
               
               // pLinked points to another FPSDoor, need to make the other door
               // believe it has received a m_msg_rx_off_to_on event, note we could
               // send a m_msg_rx_off_to_on event to the other door, but it would then
               // relay that event back to this door.
               if (pLinked) 
               {
                  pLinked->GenerateAngleTransform();
                  pLinked->State_off_Proc_m_msg_rx();
               }
            }
            break;
         case State_off_to_on:
            break;
         case State_off_to_on_reverse:
            break;
         case State_on:
            break;
         case State_on_to_off:
            if (pMsg.Id == Get_m_msg_rx_on_to_off_cancel())
            {
               // If the door is closing, and the m_msg_rx_on_to_off_cancel event occurs
               // i.e. the player is blocking the door, then the door cancels the close
               // movement, this is emulated in the linked door by simulating a m_msg_rx_on_to_off_cancel
               // event.                   
               if (pLinked)
               {
                  pLinked->State_on_to_off_proc_m_msg_rx_on_to_off_cancel();
               }
            }
            break;
         case State_on_to_off_reverse:
            break;
         }
         
         // Pass events to base class...
         FPSToggle::HandleEvents(pMsg);
         
         RWS_RETURNVOID();
      }
   }//namespace FPS
}// namespace RWS
