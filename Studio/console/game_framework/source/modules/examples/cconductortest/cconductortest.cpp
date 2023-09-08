/****************************************************************************
*
* File :      CConductorTest.cpp
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

#include "precomp.h"

// Left to its own devices, the MW linker will dead strip the registrar ctors, so...

#ifdef __MWERKS__
#pragma force_active on
#pragma exceptions off     // Turn off exceptions due to bug in MW Dwarf
#endif                     // Generator for PS2, present in CW 3.04

/////////////////////////////
//
// RenderWare Studio Includes
//

#include "framework/toolkits/world/factory.h"

#include "cconductortest.h"

namespace RWS
{
   namespace Examples
   {
      ///////////////////////////////////////////////////////////////
      //
      // Range constants - must match min & max values of attributes.
      //
      //

      const RwUInt8 CCT_RWUINT8_MAX    = 100;
      const RwUInt16 CCT_RWUINT16_MAX  = 10000;
      const RwUInt32 CCT_RWUINT32_MAX  = 1000000;
         
      const RwInt8 CCT_RWINT8_MIN   = -100;
      const RwInt8 CCT_RWINT8_MAX   = 100;
      const RwInt16 CCT_RWINT16_MIN = -10000;
      const RwInt16 CCT_RWINT16_MAX = 10000;
      const RwInt32 CCT_RWINT32_MIN = -1000000;
      const RwInt32 CCT_RWINT32_MAX = 1000000;

      const RwReal CCT_RWREAL_MIN = -1000000.0f;
      const RwReal CCT_RWREAL_MAX = 1000000.0f;

      /////////////////////////////
      //
      // CConductor class functions.
      //
      //

      RWS_IMPLEMENT_CLASSID(CConductorTest);
      RWS_REGISTERCLASS(CConductorTest);

      /**
      *
      * Constructor - just initialise.
      *
      */

      CConductorTest::CConductorTest(const CAttributePacket &attr) :
         InitCEventHandler(&m_pAtomic)
      {
         RWS_FUNCTION("RWS::Examples::CConductorTest::CConductorTest");

         // Create atomic...

         m_pAtomic = CreateAtomicInWorldFromResource(attr, this);
         RWS_ASSERT(m_pAtomic, "Failed to create atomic");

         // Create matrix to hold base position (before modification).

         m_pBaseMatrix = RwMatrixCreate();
         RWS_ASSERT(m_pBaseMatrix, "Failed to create matrix.");

         RwMatrixSetIdentity(m_pBaseMatrix);

         RWS_RETURNVOID();
      }

      /**
      *
      * Destructor - free up any used memory and unlink from messages.
      *
      */

      CConductorTest::~CConductorTest(void)
      {
         RWS_FUNCTION("RWS::Examples::CConductorTest::~CConductorTest");

         RwMatrixDestroy(m_pBaseMatrix);
         
         RWS_RETURNVOID();
      }

      /**
      *
      * Standard RenderWare Studio event handler.
      *
      */

      void CConductorTest::HandleEvents(CMsg &msg)
      {
         RWS_FUNCTION("RWS::Examples::CConductorTest::HandleEvents");
         RWS_RETURNVOID();
      }

      /**
      *
      * Standard RenderWare Studio attribute handler.
      *
      */

      void CConductorTest::HandleAttributes(const CAttributePacket &attr)
      {
         RWS_FUNCTION("RWS::Examples::CConductorTest::HandleAttributes");

         RwBool updateNeeded = FALSE;

         // Call standard handlers...

         CAttributeHandler::HandleAttributes(attr);

         if (HandleSystemCommands(attr))
         {
            updateNeeded = TRUE;
         }

         // Handle own attributes...
      
         CAttributeCommandIterator attrIt(attr, RWS_CLASSID_OF(CConductorTest));
         while (!attrIt.IsFinished())
         {
            switch (attrIt->GetCommandId())
            {
            case CMD_OFFSET_VECT:

               attrIt->GetCommandData(m_offsetVect);
               updateNeeded = TRUE;
               break;

            case CMD_OFFSET_TYPE:

               m_offsetType = static_cast<OffsetType>(attrIt->GetAs_RwUInt32());
               updateNeeded = TRUE;
               break;

/*
            case CMD_UNSIGNED_8:

               m_uint8Val = attrIt->GetAs_RwUInt8();
               updateNeeded = TRUE;
               break;

            case CMD_UNSIGNED_16:

               m_uint16Val = attrIt->GetAs_RwUInt16();
               updateNeeded = TRUE;
               break;
*/
            case CMD_UNSIGNED_32:

               m_uint32Val = attrIt->GetAs_RwUInt32();
               updateNeeded = TRUE;
               break;

/*
            case CMD_SIGNED_8:

               m_int8Val = attrIt->GetAs_RwInt8();
               updateNeeded = TRUE;
               break;

            case CMD_SIGNED_16:

               m_int16Val = attrIt->GetAs_RwInt16();
               updateNeeded = TRUE;
               break;
*/
            case CMD_SIGNED_32:

               m_int32Val = attrIt->GetAs_RwInt32();
               updateNeeded = TRUE;
               break;

            case CMD_REAL:

               m_realVal = attrIt->GetAs_RwReal();
               updateNeeded = TRUE;
               break;

            case CMD_COLOR:

               attrIt->GetCommandData(m_color);
               break;
            }

            ++attrIt;
         }

         // Need to update position ?

         if (updateNeeded)
         {
            UpdatePos();
         }

         RWS_RETURNVOID();
      }      

      /**
      *
      * Intercepts CSystemCommands commands in order to store of the original position
      * matrix of the object / any updates to it.
      *
      * \param attr Standard attribute data packet.
      *
      */

      RwBool CConductorTest::HandleSystemCommands(const CAttributePacket &attr)
      {
         RWS_FUNCTION("RWS::Examples::CConductorTest::HandleSystemCommands");

         RwBool updateNeeded = FALSE;

         // Let entity update itself first...

         m_pAtomic.HandleSystemCommands(attr);

         // Trap the matrix update command...

         CAttributeCommandIterator attrIt(attr, RWS_CLASSID_OF(CSystemCommands));

         while (!attrIt.IsFinished())
         {
            switch (attrIt->GetCommandId())
            {
            case CMD_LoadMatrix:
               {
                  const RwMatrix* pUnalignedMat = static_cast<const RwMatrix*>
                     (attrIt->GetAs_void_ptr());

                  RwMatrixCopy(m_pBaseMatrix, pUnalignedMat);
                  updateNeeded = TRUE;
               }
               break;               
            }

            ++attrIt;
         }

         RWS_RETURN(updateNeeded);
      }

      /**
      *
      * Called to update the position of the object using it's original location,
      * the offset vector, the type being used for the offset (modulation of it)
      * and the actual value of that type.
      *
      */

      void CConductorTest::UpdatePos(void)
      {
         RWS_FUNCTION("RWS::Examples::CConductorTest::UpdatePos");

         RwReal modVal,
            curVal,
            minVal,
            maxVal;

         // Get minimum and maximum, depends on type & current value of that type.

         switch (m_offsetType)
         {
         case OT_UINT_8:

            minVal = 0.0f;
            curVal = static_cast<RwReal>(m_uint8Val);
            maxVal = static_cast<RwReal>(CCT_RWUINT8_MAX);
            break;

         case OT_UINT_16:

            minVal = 0.0f;
            curVal = static_cast<RwReal>(m_uint16Val);
            maxVal = static_cast<RwReal>(CCT_RWUINT16_MAX);
            break;

         case OT_UINT_32:

            minVal = 0.0f;
            curVal = static_cast<RwReal>(m_uint32Val);
            maxVal = static_cast<RwReal>(CCT_RWUINT32_MAX);
            break;

         case OT_INT_8:

            minVal = static_cast<RwReal>(CCT_RWINT8_MIN);
            curVal = static_cast<RwReal>(m_int8Val);
            maxVal = static_cast<RwReal>(CCT_RWINT8_MAX);
            break;

         case OT_INT_16:

            minVal = static_cast<RwReal>(CCT_RWINT16_MIN);
            curVal = static_cast<RwReal>(m_int16Val);
            maxVal = static_cast<RwReal>(CCT_RWINT16_MAX);
            break;

         case OT_INT_32:

            minVal = static_cast<RwReal>(CCT_RWINT32_MIN);
            curVal = static_cast<RwReal>(m_int32Val);
            maxVal = static_cast<RwReal>(CCT_RWINT32_MAX);
            break;

         case OT_REAL:

            minVal = CCT_RWREAL_MIN;
            curVal = m_realVal;
            maxVal = CCT_RWREAL_MAX;
            break;

         default:

            RWS_ASSERTFAIL("Unknown type selected.");
            break;
         }

         // Calculate the modulate value, such that it is between -1 and +1, where
         // -1 = min value & +1 = max value. 0 is always 0.

         if (curVal > 0.0f)
         {
            modVal = curVal / maxVal;
         }
         else if (curVal < 0.0f)
         {
            modVal = -(curVal / minVal);
         }
         else
         {
            modVal = 0.0f;
         }

         // Create the offset position, base on standard position & offset & modulate value.

         RwV3d transVect;
         RwMatrix *pMatrix = RwMatrixCreate();
         RWS_ASSERT(pMatrix, "Failed to get matrix.");

         transVect.x = m_offsetVect.x * modVal;
         transVect.y = m_offsetVect.y * modVal;
         transVect.z = m_offsetVect.z * modVal;

         RwMatrixTranslate(pMatrix, &transVect, rwCOMBINEREPLACE);

         // Get matrix of asset.

         RwFrame *pAtomicFrame = RpAtomicGetFrame(m_pAtomic.ptr());
         RWS_ASSERT(pAtomicFrame, "Failed to get frame from asset's atomic.");
         RwMatrix *pAtomicMatrix = RwFrameGetMatrix(pAtomicFrame);
         RWS_ASSERT(pAtomicMatrix, "Failed to get matrix from asset's frame.");

         RwMatrixMultiply(pAtomicMatrix, pMatrix, m_pBaseMatrix);
         RwMatrixUpdate(pAtomicMatrix);
         RwFrameUpdateObjects(pAtomicFrame);

         // Tidy up.

         RwMatrixDestroy(pMatrix);

         RWS_RETURNVOID();
      }
   }     // namespace Examples.
}        // namespace RWS
