/*****************************************************************************
*
* File :     CLevel.h
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

#ifndef __CLevel_H__
#define __CLevel_H__

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


namespace RWS
{
   /**
   *
   *  \ingroup CLevel
   *
   *  CLevel contains the global data relevant to a level. A level typically
   *  consists of several RpWorld's which are used to manage the static
   *  geometry of the scene, and the spatial subdivision of dynamic objects
   *  within the scene i.e. sectors containing atomics and lights. CLevel by
   *  default provides functionality for managing two RpWorlds, an opaque world
   *  and a translucent world. CLevel provides similar functionality to RpWorld
   *  i.e. AddAtomic, AddClump, AddLight but decides which world the atomic, clump
   *  or light is actually inserted into.\n
   *
   *  CLevel makes this decision by creating a plugin to RpGeometry which is used
   *  to provide a hint as to which world an atomic or clump should be added to, these
   *  hints can be set using the CSetCLevelHint behavior.
   *
   *  Calling SetTranslucentWorld or SetOpaqueWorld also sets the "active world", the
   *  rules for adding atomics, clumps and lights are as follows.
   *
   *  \li Lights, are always added to the active world.
   *  \li Atomics, use the hint to decide which world to add to that is either
   *  translucent or opaque if available, if the hint world is unavailable or not set
   *  then the active world is used.
   *  \li Clumps, use the hint to decide which world to add to that is either translucent
   *  or opaque if available, if the hint world is unavailable or not set then the active
   *  world is used.
   *
   */
   class CLevel
   {
      public:
        /**
         *  Get the pointer to the global opaque world stored within CLevel.  
         *
         *  \returns Returns a pointer to the global opaque world 
         */
         static RpWorld *GetOpaqueWorld(void) {return sm_pOpaqueWorld;}

         /**
         *  Set the pointer to the global opaque world stored within CLevel.  
         *
         *  \param pWorld Pointer to a RpWorld object, Sets global opaque world to pWorld
         */
         static void SetOpaqueWorld(RpWorld * const pWorld) {
            sm_pOpaqueWorld =
            sm_pActiveWorld = pWorld;
         }

         /**
         *  Get the pointer to the global translucent world stored within CLevel.  
         *
         *  \returns Returns a pointer to the global translucent world 
         */
         static RpWorld *GetTranslucentWorld(void) {return sm_pTranslucentWorld;}

         /**
         *  Set the pointer to the global translucent world stored within CLevel.  
         *
         *  \param pWorld Pointer to a RpWorld object, Sets global translucent world to pWorld
         */
         static void SetTranslucentWorld(RpWorld * const pWorld) {
            sm_pTranslucentWorld =  
            sm_pActiveWorld = pWorld;
         }

         static RpWorld* AddAtomic(RpAtomic *atomic);
         static RpWorld* AddLight(RpLight *light);
         static RpWorld *AddClump(RpClump *clump);

         /**
         *
         *  Remove atomic from the world it is attached to if any.
         *
         *  \param pAtomic Pointer to a RenderWare Gfx RpAtomic object.
         *
         *  \return Returns the result of RpWorldRemoveAtomic or 0.
         *  
         *  \see CLevel::AddAtomic
         */
         static RpWorld* RemoveAtomic(RpAtomic * const pAtomic)
         {
            RpWorld *pWorld = RpAtomicGetWorld(pAtomic);

            if (pWorld)
            {
               return(RpWorldRemoveAtomic( pWorld, pAtomic));
            }

            return(0);
         }

         /**
         *
         *  Remove clump from the world it is attached to if any.
         *
         *  \param pClump Pointer to a RenderWare Gfx RpClump object.
         *
         *  \return Returns the result of RpWorldRemoveClump or 0.
         *  
         *  \see CLevel::AddClump
         */
         static RpWorld* RemoveClump(RpClump * const pClump)
         {
            RpWorld *pWorld = RpClumpGetWorld(pClump);
            
            if (pWorld)
            {
               return(RpWorldRemoveClump(pWorld, pClump));
            }

            return(0);
         }

         /**
         *
         *  Remove light from the world it is attached to if any.
         *
         *  \param pLight Pointer to a RenderWare Gfx RpLight object.
         *
         *  \return Returns the result of RpWorldRemoveLight or 0.
         *  
         *  \see CLevel::AddLight
         */
         static RpWorld* RemoveLight(RpLight * const pLight)
         {
            RpWorld *pWorld = RpLightGetWorld(pLight);
            
            if (pWorld)
            {
               return(RpWorldRemoveLight(pWorld, pLight));
            }

            return(0);
         }

         /**
         *
         *  Hint used to indicate which world either opaque or translucent an object should be added to.
         *  \see CLevel::SetPreferredWorld, CLevel::GetPreferredWorld.
         *
         */
         enum CLevelHint
         {
            HINT_OPAQUE_WORLD,         /**< Hint that this object should be added to the opaque world.*/
            HINT_TRANSLUCENT_WORLD,    /**< Hint that this object should be added to the translucent world.*/
            HINT_NO_HINT               /**< No hint has been set for this object.*/
         };

         static RwBool PluginAttach();

         static void SetPreferredWorldHint(RpAtomic * const pAtomic, CLevelHint Hint);
         static CLevelHint GetPreferredWorldHint(const RpAtomic * const pAtomic);

         static void SetPreferredWorldHint(RpClump * const pClump, CLevelHint Hint);
         static CLevelHint GetPreferredWorldHint(const RpClump * const pClump);

         static RpWorld *GetPreferredWorldPtr(const RpAtomic * const pAtomic);
         static RpWorld *GetPreferredWorldPtr(const RpClump * const pClump);

      private:
         static void* Plugin_Constructor(void* pObject, RwInt32 iOffsetInObject, RwInt32 iSizeInObject);
         static void* Plugin_Copy(void* pDstObject, const void* pSrcObject, RwInt32 iOffset, RwInt32 iSize);

         static RpWorld *sm_pOpaqueWorld;          /**< Pointer to global opaque world */
         static RpWorld *sm_pTranslucentWorld;     /**< Pointer to global translucent
                                                        geometry */
         static RpWorld *sm_pActiveWorld;          /**< Pointer to active world,
                                                        active world is last world
                                                        set by either SetOpaqueWorld,
                                                        or SetTranslucentWorld*/
   };
}//namespace

#endif
