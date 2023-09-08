/*****************************************************************************
*
* File :     CResourceHandler.h
*
* Abstract : 
*
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

#ifndef _CRESOURCEHANDLER_H_
#define _CRESOURCEHANDLER_H_

//////////////////////////////////////////////////////////////////
//
// RenderWare Includes
//
#include <rwcore.h>

#ifdef _MSC_VER
#pragma comment (lib, "rwcore.lib")
#endif

namespace RWS
{
   /**
   *
   * \ingroup Resource
   *
   * Abstract Base Class, defines the functionality required to implement a resource handler. Resource
   * handlers obtain the data for a resource from a RenderWare stream, this means that the client doesn't
   * need to understand the source of the data i.e. the stream pointer may point to either a memory stream or 
   * a file stream.
   *
   */
   class CResourceHandler
   {
      public:

         /**
         *
         *  Called to initialize the resource handler when CResourceManager::Open is called.
         *
         */
         virtual void Open(void) {};

         /**
         *
         *  Called to uninitialize the resource handler when CResourceManager::Close is called.
         *
         */
         virtual void Close(void) {};


         /**
         *
         *  \ref CResourceHandler::IsHandled This function is called when a new resource is loaded, it provides the 
         *  answers to the question can this resource type be processed by this resource handler.
         *
         *  \param psType A pointer to a string defining the resource type.
         * 
         */
         virtual RwBool IsHandled(const RwChar *psType) = 0;
  
         /**
         *
         * This function is responsible for loading the specified resource. RenderWare 
         * Streams store data in chunks. It is vitally important that when reading data
         * from a stream that the position of the stream is left at the start of the following
         * chunk.To ensure that the position of the stream is left at the start of the next chunk
         * each resource handler must ensure that uiStreamSize bytes have either skipped or read
         * from the stream passed to the load method. The result of loading from the stream should be returned. For
         * example a resource handler that loads BMP files may load the file from the stream and process the loaded
         * data into some other format it is the pointer to the processed data that should be returned, the streamed
         * data being discarded.
         * 
         *
         *  \param psName A pointer to a string defining the resource name.
         *
         *  \param psType A pointer to a string defining the resource type.
         *
         *  \param psResourcePath A pointer to a string containing a path where files referenced
         *  by this file can be found.
         *
         *  \param pStream A pointer to a RenderWare stream.
         *
         *  \param uiStreamSize The size of the file associated with this resource.
         *
         *  \param uiResourceSize A reference to an unsigned int, this can be filled in to store the size of the loaded
         *  resource. \see CResourceManager::FindById, CResourceManager::FindByName,
         *  CResource::GetSize, CResource::SetSize
         *
         *  \returns If successful returns a pointer to the loaded resource, otherwise 0.
         *
         *  \image html cresourcehandlerload.png
         */
         virtual void *Load(const RwChar *psName,
            const RwChar *psType,
            const RwChar *psResourcePath,
            RwStream* pStream,
            RwUInt32 uiStreamSize,
            RwUInt32 &uiResourceSize) = 0;

         /**
         *
         *  \ref CResourceHandler::UnLoad This function is responsible for unloading the specified resource.
         *
         *  \param pStrType A pointer to a string defining the resource type.
         *
         *  \param pResource A pointer to the resource.
         *
         *  \returns True If the resource was successfully unloaded otherwise false.
         *
         */
         virtual RwBool UnLoad(const RwChar *pStrType, void *pResource) = 0;
         
         /**
         *  
         *  \ref CResourceHandler::Update
         *  
         *  \param pResData A pointer to the resource data.
         *  \param pData A pointer to the data used to update the resource.
         * 
         */
         virtual void Update(const void *pResData,
                             const void *pData,
                             const RwChar *pStrType) = 0;

         CResourceHandler(void);

         virtual ~CResourceHandler(void);

         static CResourceHandler *FindHandler(const RwChar *const psType);

         static void OpenAll(void);
         static void CloseAll(void);

      private:

         CResourceHandler(const CResourceHandler& rhs);               // Disable copy constructor 
         CResourceHandler &operator = (const CResourceHandler& rhs);  // Disable copy operator,
   };
}

#endif
