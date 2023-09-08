/*****************************************************************************
*
* File :     gftime.h
*
* Abstract : Time measurement services
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

/**
*
*  \ingroup Time
*
*  \page TimeOverview Overview
*
*  Provides time measurement services for the game framework.
*
*/

#ifndef _RWSTIME_H_
#define _RWSTIME_H_

namespace RWS
{
   /**
   *  \ingroup Time
   *
   *  Namespace used to group Time API functions.
   */
   namespace Time
   {
      /**
      *
      *  \ingroup Time
      *
      *  Initializes time measurement services.
      *
      */
      void Open();

      /**
      *
      *  \ingroup Time
      *
      *  Shutdown time measurement services.
      *
      */
      void Close();

      /**
      *
      *  \ingroup Time
      *
      *  Gets the current time, in milliseconds.
      *
      */
      RwUInt32 GetTime();
   }
}

#endif // _RWSTIME_H_
