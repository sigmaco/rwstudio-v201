// RwUtilManagedObj.h
// This header file contains some helpful base classes which can be used for managing the lifetimes of objects

#pragma once
#include "RwUtil.h"
#include "RwUtilAlloc.h"

/**
*  \ingroup rwutilbase
*  This class implements memory management.  Simply derive off this class and all your classes
*  will be allocated using the RwUtil memory management functions
*
*/

class RWUTIL_API RwUtilMemoryManaged
{
public:
    static void *operator new(size_t size);
    static void operator delete(void *memory);
    static void *operator new[] (size_t size);
    static void operator delete[] (void *memory);
};

/**
*  \ingroup rwutilbase
*  This templated class implements reference counting.  Simply derive off this class and all your classes
*  will the have reference counting functions AddRef() and Release()
*
*/

// loosely based on More Effective C++ Item 29...
class RWUTIL_API RwUtilReferenceCounted
{
    int m_refCount;
public:

    //construct and initialise ref count
    RwUtilReferenceCounted() : m_refCount(0)
    {}
    RwUtilReferenceCounted(const RwUtilReferenceCounted& rhs) : m_refCount(0)
    {}
    //maintain reference count accross assignment
    RwUtilReferenceCounted& operator=(const RwUtilReferenceCounted& rhs)
    { return *this;}

    // This is very important.  Because this is here the compiler will generate a scalar (or vector) deleting destructor
    // for the class.  This calls both the derived class's destructor AND ITS APPROPRIATE OPERATOR DELETE!
    // This means that we don't need templates or any other fancy code stuff in release() - calling "delete this"
    // will call the relevant (possibly overloaded) operator.
    virtual ~RwUtilReferenceCounted() = 0;
     /**
     *  Increments an object's reference count.
     *  \return how many objects are referencing this object.
     *
     */
    int AddRef()
    {
        return ++m_refCount;
    }

    /**
     *  Decrements an object's reference count and deletes the object if the ref count is zero
     *  \return how many objects are referencing this object after the call.
     *
     */
    int Release()
    {
        int rc = --m_refCount;

        if (0 == m_refCount)
        {
            // this calls the overloaded delete operator - must make sure that the
            // destructor is virtual otherwise this will cause errors
            delete this;
        }

        return rc;
    }

};

/**
*  \ingroup rwutilbase
*  This class looks like it is doing reference counting but it isn't  Useful if you want to change
*  your implementation without having to change some client code from doing reference management.
*
*/
class RWUTIL_API RwUtilFakeReferenceCounted
{
public:
     /**
     *  Does Nothing.
     *  \return always 1.
     *
     */
    int AddRef()
    {
        return 1;
    }

    /**
     *  Does nothing
     *  \return always 0.
     *
     */
    int Release()
    {
        return 0;
    }
};

/**
*  \ingroup rwutilbase
*  This class includes all the most common management classes in one abstract base class to be derived off.
*  Currently this means that classes derived off RwUtilManagedObj are both reference counted and memory managed.
*
*/
class RWUTIL_API RwUtilManagedObj : public RwUtilReferenceCounted, public RwUtilMemoryManaged
{
public:
    virtual ~RwUtilManagedObj() = 0;
};