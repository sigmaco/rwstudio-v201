/*****************************************************************************
*
* File :     CClassFactory.h
*
* Abstract : 
*
* In order for the RenderWare Studio Workspace to create new behaviors/entities
* we need a way of identifying the class to create. We could have used a GUID or
* enum, but this would add unnecessary work for the programmer so we chose to use
* the name of the class, as this is already unique to the system.
*
* CClassFactory maintains a mapping of name/function pointer pairs which
* allow a named class to be created. 
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
* Copyright (c) 2003 Criterion Software Ltd.
* All Rights Reserved.
*
* RenderWare is a trademark of Canon Inc.
*
*****************************************************************************/

/**
*
*  \ingroup CClassFactory
*
*  \page CClassFactoryOverview Overview
*
*  All behaviors (classes derived from RWS::CAttributeHandler) are created
*  via a global class factory. To do this, each behaviour needs to first 
*  register with the class factory, using the macros RWS_MAKENEWCLASS and 
*  RWS_REGISTERCLASS. Instances of the class can then be created as needed
*  via RWS::CClassFactory::MakeNew. See 
*  RWS::CDefaultStreamHandler::On_strfunc_CreateEntity to see how
*  RWS::CClassFactory::MakeNew is used.
*
*/

#ifndef __CCLASSFACTORY_H__
#define __CCLASSFACTORY_H__

namespace RWS
{
   class CAttributeHandler;
   class CAttributePacket;

   /**
   *
   *  \ingroup CClassFactory
   *
   *  This typedef MakeNewPtr is used by CClassFactory to provide the pointer 
   *  to the factory function (MakeNew) which actually creates an instance of 
   *  our class. We do this so that the code for creating instances of entities 
   *  can reference them by name and is therefore much simpler, smaller, and
   *  flexible.
   *
   */
   typedef CAttributeHandler *(*MakeNewPtr)(const RWS::CAttributePacket&); 

   /**
   *
   */
   namespace CClassFactory
   {
      CAttributeHandler *MakeNew(const char* pClassName, const CAttributePacket& rAttr);
      size_t GetRegisteredClassSize(const char* pClassName);


      /* This value is used to determine the boundary that allocation can take place within.
      // the linear memory manager.
      // E.G. With this value set to 4, all allocations will be on a word aligned boundary.
      //      With this value set to 2, all allocations will be on a short aligned boundary.
      */
      const RwUInt32 m_AlignmentSize = sizeof(RwUInt8) * 64;

      /**
      *
      *  \ingroup CClassFactory
      *
      *  Registers behaviors with the class factory at static initialization time.
      *
      */
      class CRegistrar
      {
         public:
            ~CRegistrar();
            CRegistrar(const char* pClassName, MakeNewPtr makeNewFn, size_t size, RwBool UseLinearMemoryManager);

      };

      /**
      *
      *  \ingroup CClassFactory
      *
      *  Declares and initializes an instance of a CRegistrar. The instance 
      *  will be constructed at static initialization time, and this will effectively 
      *  register the named class.
      *
      */
#ifdef DOXYGEN
      #define RWS_REGISTERCLASS(className)
#else
      #define RWS_REGISTERCLASS(className) RWS::CClassFactory::CRegistrar className##Registrar(#className, className::MakeNew, sizeof(className), className::UseLinearMemory())
#endif

      /**
      *
      *  \ingroup CClassFactory
      *
      *  Defines a factory function (MakeNew) used to create an instance of the 
      *  specified class.
      *
      *  \verbatim
         static RWS::CAttributeHandler* MakeNew(void)
         {
            RWS::CAttributeHandler *ptr = static_cast<RWS::CAttributeHandler*>( new classname);
            return ptr;
         }
         \endverbatim
      */
#ifdef DOXYGEN
      #define RWS_MAKENEWCLASS(classname)
#else
      #define RWS_MAKENEWCLASS(classname) static RWS::CAttributeHandler* MakeNew(const RWS::CAttributePacket& attr){RWS::CAttributeHandler *ptr = static_cast<RWS::CAttributeHandler*>(RWS_NEW classname(attr));return ptr;}
#endif

   }
}

#endif
