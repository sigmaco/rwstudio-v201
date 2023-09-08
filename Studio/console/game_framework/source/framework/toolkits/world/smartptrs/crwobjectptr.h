/*****************************************************************************
*
* File :     CRwObjectPtr.h
*
* Abstract : Generic lowest-common-denominator smart pointer for RwObjects.
*            Intended as a base class for more specific smart pointers.            
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
* Copyright (c) 2001 Criterion Software Ltd.
* All Rights Reserved.
*
* RenderWare is a trademark of Canon Inc.
*
*****************************************************************************/


#ifndef __CRWOBJECTPTR_H__
#define __CRWOBJECTPTR_H__

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "framework/core/eventhandler/ceventvisualisation.h"
#include "framework/core/macros/debugmacros.h"

namespace RWS
{
   /**
   *
   *  \ingroup WorldSmartPointers
   *
   *  Generic lowest-common-denominator smart pointer for RwObjects.
   *  A template with which to create more specific smart pointers.
   *
   */
   template<class T>
   class CRwObjectPtr
   {
   public:

      /**
      *
      *  Constructs a 'NULL' smart pointer
      *
      */
      CRwObjectPtr() : p_(0) {}

      /**
      *
      *  Constructs a smart pointer to manage the given RwObject
      *
      */
      CRwObjectPtr(T* p) : p_(p) {}

      /**
      *
      *  Type conversion operator. Allows the smart pointer to be
      *  used in exactly the same way as an equivalent C pointer.
      * \code

         void MyFunc(void)
         {
            RpAtomic* pAtomic = RpAtomicCreate();

            MyRpAtomicFunc(pAtomic);   // Prototype MyRpAtomicFunc(RpAtomic *ptr)
         }
         
         Produces the same code as...

         void MyFunc(void)
         {
            CRwObjectPtr<RpAtomic> smartAtomicPtr = RpAtomicCreate();

            MyRpAtomicFunc(smartAtomicPtr);  // Prototype MyRpAtomicFunc(RpAtomic *ptr)
         }

         \endcode

      *  \return Returns a pointer to the managed RwObject.
      *
      */
      operator T*() const { return p_; }

      /**
      *
      *  Dereferences the smart pointer, assuming it's non-NULL
      *
      *  \return A pointer to the managed RwObject
      *
      */
      T* operator->() const { RWS_PRE(*this); return p_; }

      /**
      *
      *  Dereferences the smart pointer, assuming it's non-NULL
      *
      *  \return A reference to the managed RwObject
      *
      */
      T& operator*() const { RWS_PRE(*this); return *p_; }

      /**
      *
      *  \return A pointer to the managed RwObject
      *
      */
      T* ptr() const { return p_; }

      /**
      *
      * Force update of pointer to the managed RwObject
      *
      */
      void setptr(T* p) {p_ = p;}

   protected:

      T* p_;

   private:

      // Revoked ops (we don't know enough about T to allow copy/assign)
      CRwObjectPtr(const CRwObjectPtr&);
      CRwObjectPtr& operator=(const CRwObjectPtr&);
   };
}

#endif
