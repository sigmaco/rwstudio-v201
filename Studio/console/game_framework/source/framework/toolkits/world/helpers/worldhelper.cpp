/*****************************************************************************
*
* File :     WorldHelper.h
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

//#define RWS_CALLSTACKENABLE
//#define RWS_TRACEENABLE
//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "worldhelper.h"
#include "framework/core/macros/debugmacros.h"

namespace RWS
{
   namespace 
   {
      RpAtomic   *SubstituteWorld_RpAtomicCallBack (RpAtomic * atomic, void *data)
      {
         RWS_FUNCTION("RWS::NULL::SubstituteWorld_RpAtomicCallBack");

         RWS_TRACE("atomic ["<<atomic<<"]");

         RpWorld *_RpWorld = reinterpret_cast<RpWorld*>(data);

         RpWorld *__RpWorld = RpAtomicGetWorld(atomic);

         if (__RpWorld) RpWorldRemoveAtomic(__RpWorld, atomic);

         RpWorldAddAtomic(_RpWorld, atomic);

         RWS_RETURN(atomic);
      }

      RpClump    *SubstituteWorld_RpClumpCallBack (RpClump * clump, void *data)
      {
         RWS_FUNCTION("RWS::NULL::SubstituteWorld_RpClumpCallBack");

         RWS_TRACE("clump ["<<clump<<"]");

         RpWorld *_RpWorld = reinterpret_cast<RpWorld*>(data);

         RpWorld *__RpWorld = RpClumpGetWorld(clump);

         if (__RpWorld) RpWorldRemoveClump(__RpWorld, clump);

         RpWorldAddClump(_RpWorld, clump);

         RWS_RETURN(clump);
      }

      RpLight    *SubstituteWorld_RpLightCallBack (RpLight * light, void *data)
      {
         RWS_FUNCTION("RWS::NULL::SubstituteWorld_RpLightCallBack");

         RWS_TRACE("light ["<<light<<"]");

         RpWorld *_RpWorld = reinterpret_cast<RpWorld*>(data);

         RpWorld *__RpWorld = RpLightGetWorld (light);

         if (__RpWorld) RpWorldRemoveLight(__RpWorld, light);

         RpWorldAddLight(_RpWorld, light);

         RWS_RETURN(light);
      }

      RpWorldSector *SubstituteWorld_RpWorldSectorCallBack (RpWorldSector *sector, void *data)
      {
         RWS_FUNCTION("RWS::NULL::SubstituteWorld_RpWorldSectorCallBack");

         RWS_TRACE("sector ["<<sector<<"]");

         RpWorldSectorForAllAtomics(sector, SubstituteWorld_RpAtomicCallBack, data);

         RWS_RETURN(sector);
      }
   }

   namespace WorldHelper
   {
      /**
      *
      *  \ingroup WorldHelper
      *
      *  Remove lights/atomics/clumps linked to _src_RpWorld world, and add them to the 
      *  _dst_RpWorld world.
      *
      */
      void MoveAllObjects (RpWorld *_src_RpWorld, RpWorld *_dst_RpWorld)
      {
         RWS_FUNCTION("RWS::WorldHelper::SubstituteWorld");

         RWS_TRACE("From ["<<_src_RpWorld<<"] to ["<<_dst_RpWorld<<"]");

         RWS_PRE(_src_RpWorld);
         RWS_PRE(_dst_RpWorld);

         if (_src_RpWorld != _dst_RpWorld)
         {
            RpWorldForAllClumps(_src_RpWorld, SubstituteWorld_RpClumpCallBack, _dst_RpWorld);

            RpWorldForAllLights(_src_RpWorld, SubstituteWorld_RpLightCallBack, _dst_RpWorld);

            RpWorldForAllWorldSectors(_src_RpWorld, SubstituteWorld_RpWorldSectorCallBack, _dst_RpWorld);
         }

         RWS_RETURNVOID();
      }
   }
}