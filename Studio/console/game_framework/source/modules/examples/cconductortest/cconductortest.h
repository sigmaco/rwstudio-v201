/*****************************************************************************
*
* File :      CConductorTest.h
*
* Abstract :  Sequence controller testing behaviour.
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

#ifndef C_CONDUCTOR_TEST_HEADER
#define C_CONDUCTOR_TEST_HEADER

#include "framework/core/attributehandler/cattributehandler.h"
#include "framework/core/eventhandler/ceventhandler.h"
#include "framework/core/attributehandler/allocation policies/LinearAllocationPolicy.h"
#include "framework/core/macros/debugmacros.h"
#include "framework/toolkits/world/smartptrs/catomicptr.h"

namespace RWS
{
   namespace Examples
   {
      class CConductorTest : public CSystemCommands, public CAttributeHandler, public CEventHandler,
         public LinearAllocationPolicy
      {
      public:

         RWS_MAKENEWCLASS(CConductorTest);
         RWS_DECLARE_CLASSID(CConductorTest);
         RWS_CATEGORY("Examples");
         RWS_DESCRIPTION("Conductor Test",
            "Provided as an example / test of the CConductor sequence controller.");

         RWS_BEGIN_COMMANDS
            RWS_SEPARATOR("Offset", 0)
               RWS_ATTRIBUTE(CMD_OFFSET_VECT,
                  "offset vector",
                  "the offset vector at full range on the offset value",
                  VECTOR,
                  RwV3d,
                  RANGES((-1000, 0, 1000), (-1000, 0, 1000), (-1000, 0, 1000)))
               RWS_ATTRIBUTE(CMD_OFFSET_TYPE,
                  "offset type",
                  "type of offset to use, signed will cause negative offset when less than zero",
                  LIST,
                  RwUInt32,
                  LIST("RwUInt8|RwUInt16|RwUInt32|RwInt8|RwInt16|RwInt32|RwReal"))
/*
                  RWS_ATTRIBUTE(CMD_UNSIGNED_8,
                  "uint8",
                  "RwUInt8 based offset value",
                  SLIDER,
                  RwUInt8,
                  RANGE(0, 0, 100))
               RWS_ATTRIBUTE(CMD_UNSIGNED_16,
                  "uint16",
                  "RwUInt16 based offset value",
                  SLIDER,
                  RwUInt16,
                  RANGE(0, 0, 10000))
*/
               RWS_ATTRIBUTE(CMD_UNSIGNED_32,
                  "uint32",
                  "RwUInt32 based offset value",
                  SLIDER,
                  RwUInt32,
                  RANGE(0, 0, 1000000))
/*
               RWS_ATTRIBUTE(CMD_SIGNED_8,
                  "int8",
                  "RwInt8 based offset value",
                  SLIDER,
                  RwInt8,
                  RANGE(-100, 0, 100))
               RWS_ATTRIBUTE(CMD_SIGNED_16,
                  "int16",
                  "RwInt16 based offset value",
                  SLIDER,
                  RwInt16,
                  RANGE(-10000, 0, 10000))
*/
               RWS_ATTRIBUTE(CMD_SIGNED_32,
                  "int32",
                  "RwInt32 based offset value",
                  SLIDER,
                  RwInt32,
                  RANGE(-1000000, 0, 1000000))
               RWS_ATTRIBUTE(CMD_REAL,
                  "real",
                  "RwReal based offset value",
                  SLIDER,
                  RwReal,
                  RANGE(-1000000, 0, 1000000))
            RWS_SEPARATOR("Color", 0)
               RWS_ATTRIBUTE(CMD_COLOR,
                  "color",
                  "color value of object",
                  COLOR,
                  RwRGBA,
                  DEFAULT(4294967295))
         RWS_END_COMMANDS;

         CConductorTest(const CAttributePacket &attr);
         ~CConductorTest(void);

         virtual void HandleEvents(CMsg &msg);
         virtual void HandleAttributes(const CAttributePacket &attr);

      private:

         RwBool HandleSystemCommands(const CAttributePacket &attr);
         void UpdatePos(void);

         enum OffsetType                  // MUST match list in CMD_OFFSET_TYPE attribute.
         {
            OT_UINT_8 = 0,
            OT_UINT_16,
            OT_UINT_32,
            OT_INT_8,
            OT_INT_16,
            OT_INT_32,
            OT_REAL
         };

         CAtomicPtr m_pAtomic;            /**< Position of the camera. */
         RwMatrix *m_pBaseMatrix;         /**< Holds set base matrix, set from system commands attributes. */

         RwV3d m_offsetVect;
         OffsetType m_offsetType;

         RwRGBA m_color;

         RwInt32 m_int32Val;
         RwInt16 m_int16Val;
         RwInt8 m_int8Val;

         RwUInt32 m_uint32Val;
         RwUInt16 m_uint16Val;
         RwUInt8 m_uint8Val;

         RwReal m_realVal;
      };
   }     // Examples
}        // RWS
#endif   // #ifndef C_CONDUCTOR_TEST_HEADER
