/*****************************************************************************
*
* File :     CClassFactory.cpp
*
* Abstract :
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
#include "precomp.h"
//#define RWS_CALLSTACKENABLE
//#define RWS_TRACEENABLE

#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

//////////////////////////////////////////////////////////////////
//
// System Includes
//
#ifdef DOLPHIN
#include <string.h>
#endif

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "cclassfactory.h"
#include "../attributehandler/cattributehandler.h"
#include "../macros/debugmacros.h"

//////////////////////////////////////////////////////////////////
//
// Standard Includes
//
#include <map>

namespace
{
   /*
   *
   *  Predicate for class factory map. Allows const char* to be used as the lookup
   *  key, which is far more efficient than stl::string.
   *
   */
   struct cstrless : std::binary_function<const char*, const char*, bool>
   {
      bool operator() (const char* x, const char* y) const
      {
         return strcmp(x, y) < 0;
      }
   };

   /*
   *  Structure used to store information of each class that is registered with
   *  the class factory. i.e. the new function and the size of the class
   */
   struct SClassInfo
   {
      RWS::MakeNewPtr  MakeNewFunc; /**< Pointer to class's make new function */
      size_t      size;             /**< Size of class in bytes */
   };

   /*
   *
   *  Typedef for class factory map.
   *
   */
   typedef std::map<const char*, SClassInfo*, cstrless> NameToPtrMap;

   /*
   *
   *  Singleton for class factory map. Preferable to a global variable because it eliminates
   *  ordering issues and ensures that the map is created on demand.
   *
   */
   NameToPtrMap& GetNameToPtrMap()
   {
      static NameToPtrMap theNameToPtrMap;
      return theNameToPtrMap;
   }
}

namespace RWS
{
   namespace CClassFactory
   {
   
      /**
      *
      *  \ingroup CClassFactory
      *
      *  Returns true if a class has been registered with the class factory
      *
      *  \param pClassName A string specifying the name of the class to register.
      *
      *  \return Returns size of registered class in bytes, otherwise 0.
      */
      size_t GetRegisteredClassSize(const char* pClassName)
      {
         RWS_FUNCTION("RWS::CClassFactory::GetRegisteredClassSize")

         RWS_PRE(pClassName)

         NameToPtrMap::iterator foundIt = GetNameToPtrMap().find(pClassName);
         SClassInfo * pclassInfo = GetNameToPtrMap().end() != foundIt ? (*foundIt).second : 0;

         if (!pclassInfo)
            RWS_RETURN(0);


         RWS_RETURN(pclassInfo->size)
      }

      /**
      *
      *  \ingroup CClassFactory
      *
      *  Returns true if a class has been registered with the class factory.
      *
      *  \param pClassName A string specifying the name of the class to register.
      *
      *  \return Returns true if the class is registered otherwise false.
      */
      bool ClassIsRegistered(const char* pClassName)
      {
         RWS_FUNCTION("RWS::CClassFactory::ClassIsRegistered")

         RWS_PRE(pClassName)

         bool result = GetNameToPtrMap().end() != GetNameToPtrMap().find(pClassName);

         RWS_RETURN(result)
      }

      /**
      *
      *  \ingroup CClassFactory
      *
      *  Registers a class with the class factory.
      *
      *  \param pClassName A string specifying the name of the class to register.
      *
      *  \param makeNewFn A pointer to a MakeNewPtr function.
      *
      *  \param size The size of the class to be registered.
      *
      *  \param UseLinearMemoryManager Flag to use the Linear Memory Manager.
      */
      void RegisterClass(const char* pClassName, MakeNewPtr makeNewFn, size_t size, RwBool UseLinearMemoryManager)
      {
         RWS_FUNCTION("RWS::CClassFactory::RegisterClass")

         RWS_PRE(pClassName)
         RWS_PRE( !ClassIsRegistered(pClassName) )

         SClassInfo* pClassInfo = (SClassInfo *)new(SClassInfo);

         pClassInfo->MakeNewFunc = makeNewFn;
         pClassInfo->size = 0;

         if (UseLinearMemoryManager)
         {
            pClassInfo->size = size;

            if (size % m_AlignmentSize)
               pClassInfo->size += m_AlignmentSize - (size % m_AlignmentSize);

            RWS_TRACE(" "<<pClassName<<" Actual size : "<<size<<" Aligned size: "<<pClassInfo->size);
         }
         else
         {
            RWS_TRACE(" "<<pClassName<<" Actual size : "<<size);
         }


         GetNameToPtrMap().insert(std::make_pair(pClassName, pClassInfo));

         RWS_POST( ClassIsRegistered(pClassName) )

         RWS_RETURNVOID()
      }

      /**
      *
      *  \ingroup CClassFactory
      *
      *  Returns a pointer to the specified class or 0.
      *
      *  \param pClassName A string specifying the name of a class.
      *  \param attr A reference to a CAttributePacket used to initialize the CAttributeHandler.
      *
      *  \return pointer to a CAttributeHandler object.
      *
      */
      CAttributeHandler *MakeNew(const char* pClassName, const CAttributePacket& rAttr)
      {
         RWS_FUNCTION("RWS::CClassFactory::GetMakeNew")

         RWS_PRE(pClassName)

         NameToPtrMap::iterator foundIt = GetNameToPtrMap().find(pClassName);
         SClassInfo * pclassInfo = GetNameToPtrMap().end() != foundIt ? (*foundIt).second : 0;
         
         RWS_WARNING(pclassInfo, pClassName<<" is not registered on this target and so cannot be created.");

         if (pclassInfo && pclassInfo->MakeNewFunc)
         {
            CAttributeHandler *pCAttributeHandler = pclassInfo->MakeNewFunc(rAttr);

            if (pCAttributeHandler)
            {
               pCAttributeHandler->HandleAttributes(rAttr);
            }
            RWS_RETURN(pCAttributeHandler);
         }

         RWS_RETURN(0);
      }

      /**
      *
      *  Registers a class with the class factory.
      *
      *  \param pClassName A string specifying the name of the class to register.
      *  \param makeNewFn A pointer to a MakeNewPtr function.
      *  \param size The size of the class being registered.
      *  \param UseLinearMemoryManager Flag to use the Linear Memory Manager.
      */
      CRegistrar::CRegistrar(const char* pClassName, MakeNewPtr makeNewFn, size_t size, RwBool UseLinearMemoryManager)
      {
         RWS_FUNCTION("RWS::CClassFactory::CRegistrar::CRegistrar")

         RWS_PRE(pClassName)

         RegisterClass(pClassName, makeNewFn, size, UseLinearMemoryManager);

         RWS_RETURNVOID()
      }

      /**
      *
      *  Destructor for Registrar.
      *
      *  Deletes any SClassInfo structures created to hold class information.
      */
      CRegistrar::~CRegistrar()
      {
         RWS_FUNCTION("RWS::CClassFactory::CRegistrar::~CRegistrar")

         NameToPtrMap::iterator it = GetNameToPtrMap().begin();

         while (it != GetNameToPtrMap().end())
         {
            delete (*it).second;

            it++;
         }

         GetNameToPtrMap().clear();
         
         RWS_RETURNVOID()
      }
   }
}
