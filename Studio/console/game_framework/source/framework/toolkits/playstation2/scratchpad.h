/*****************************************************************************
*
* File :      scratchpad.h
*
* Abstract :  Functions to simplify using the scratchpad on 
*              PlayStation2.
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

#if defined(SKY)

#ifndef __SCRATCHPAD_H_
#define __SCRATCHPAD_H_

// In the RWDMA_SPR_WAIT_ON_TO macro, the line 
//    while (*(volatile RwUInt32*)(0x1000d400) & 0x100);
// produces a warning from the MW compiler about the semi-colon. So,
// turn off the warning to avoid generating it every time the functions
// in this file are inlined.
#if defined(__MWERKS__)
#pragma warn_possunwant off
#endif
            
#include "precomp.h"


namespace RWS
{
   namespace Sky
   {
      /**
      *
      *  \ingroup PlayStation2Toolkit
      *
      *  Size of the scratchpad, in bytes
      */
      static const RwUInt32 SCRATCHPAD_SIZE = 16384;  

      /**
      *
      *  \ingroup PlayStation2Toolkit
      *
      *  Functions to simplify moving data to and from scratchpad memory.
      *
      */
      class CScratchPad
      {
      public:
         
         /**
         *
         *  Transfers data to the scratchpad, using the ToSPR DMA channel.
         *  \note This function will first wait for any DMA transfers on
         *        the ToSPR channel to finish before carrying on.
         *
         *  \param pDst Destination address on the scratchpad.
         *              Must be 16Byte aligned and within the scratchpad
         *              memory range (0x70000000 - 0x70004000).
         *  \param pSrc Source address in main memory. 
         *              Must be 16Byte aligned.
         *  \param uSize Number of quad words of data to transfer 
         *                 (1 QWord = 16Bytes).
         *
         *  \return Pointer to address the data was transferred to on the 
         *           scratchpad.
         *
         *  \see CScratchPad::Download
         *  \see CScratchPad::WaitOnUpload
         *
         */
         template<class SrcType, class DestType>
         static SrcType Upload( DestType pDst, SrcType pSrc, RwUInt32 uQWords )
         {
            RWS_ASSERT( ((RwUInt32)pDst % 16) == 0, 
                        "Destination address is not 16Byte aligned.");

            RWS_ASSERT( (((RwUInt32)pDst >= RWDMA_SCRATCHPAD)
                         && ((RwUInt32)pDst <= (RWDMA_SCRATCHPAD + SCRATCHPAD_SIZE))),
                        "Destination address is not within the scratchpad.");

            RWS_ASSERT( ((RwUInt32)pSrc % 16) == 0, 
                        "Source address is not 16Byte aligned.");

            RWS_ASSERT( uQWords < (SCRATCHPAD_SIZE >> 4),
                        "There is not enough room on the scratchpad for this data.");

            RWS_ASSERT( RwCameraGetCurrentCamera() == NULL,
                        "Cannot use the scratchpad during rendering - RenderWare \
                         Graphics uses it to build DMA packets.");

            RWDMA_SPR_WAIT_ON_TO();            
            RWDMA_SPR_CPY_TO( pDst, pSrc, uQWords );

            return( reinterpret_cast<SrcType>(pDst) );
         }

         /**
         *
         *  Transfers data from the scratchpad, using the FromSPR DMA channel.
         *  \note This function will first wait for any DMA transfers on
         *        the FromSPR channel to finish before carrying on.
         *
         *  \param pDst Destination address in main memory. 
         *              Must be 16Byte aligned.
         *  \param pSrc Source address in scratchpad memory. 
         *              Must be 16Byte aligned and within the scratchpad
         *              memory range (0x70000000 - 0x70004000).
         *  \param uSize Number of quad words of data to transfer 
         *                 (1 QWord = 16Bytes).
         *
         *  \return The address the data was transferred to
         *
         *  \see CScratchPad::Upload
         *  \see CScratchPad::WaitOnDownload
         *
         */
         template<class SrcType, class DestType>
         static DestType Download( DestType pDst, SrcType pSrc, RwUInt32 uQWords )
         {
            RWS_ASSERT( ((RwUInt32)pDst % 16) == 0, 
                        "Destination address is not 16Byte aligned");

            RWS_ASSERT( ((RwUInt32)pSrc % 16) == 0, 
                        "Source address is not 16Byte aligned");

            RWS_ASSERT( (((RwUInt32)pSrc >= RWDMA_SCRATCHPAD)
                         && ((RwUInt32)pSrc <= (RWDMA_SCRATCHPAD + SCRATCHPAD_SIZE))),
                        "Source address is not within the scratchpad.");

            RWS_ASSERT( uQWords < (SCRATCHPAD_SIZE >> 4), 
                        "Cannot transfer more data than is on the scratchpad.");

            RWS_ASSERT( RwCameraGetCurrentCamera() == NULL,
                        "Cannot use the scratchpad during rendering - RenderWare \
                         Graphics uses it to build DMA packets.");

            RWDMA_SPR_WAIT_ON_FROM();
            RWDMA_SPR_CPY_FROM( pDst, pSrc, uQWords );

            return( reinterpret_cast<DestType>(pDst) );
         }

         /**
         *
         *  Wait while the ToSPR DMA channel is busy.
         *
         */
         static void WaitOnUpload()
         {
            RWDMA_SPR_WAIT_ON_TO();       
         }

         /**
         *
         *  Wait while the FromSPR DMA channel is busy.
         *
         */
         static void WaitOnDownload()
         {
            RWDMA_SPR_WAIT_ON_FROM();
         }
      };
   }
}

#endif // __SCRATCHPAD_H_

#endif // SKY