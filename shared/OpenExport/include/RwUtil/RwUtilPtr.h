// RwUtilPtr.h
// This header file contains some helpful base classes which can be used for managing the lifetimes of objects

#pragma once
#include "RwUtil.h"
#include <assert.h>

/**
*  \ingroup rwutilbase
*  This templated class implements a smart pointer for objects that can do reference counting.  It is provided
*  primarily for support of classes derived from RwUtilReferenceCounted.
*  
*/

template <class T>
class RwUtilRCAutoPtr
{
public:

    // the pointer itself...
	T* ptr;

// Constructors for various situations

    // Default constructor
    RwUtilRCAutoPtr() : ptr(0)
	{}

    // Copy constructor
	RwUtilRCAutoPtr(const RwUtilRCAutoPtr<T>& toCopy) : ptr(toCopy.ptr)
	{
        // If we have something to point to then add a reference to it
		if (0 != ptr)
			ptr->AddRef();
	}

    // assignment constructor
	RwUtilRCAutoPtr(T* toPointTo) : ptr(toPointTo)
	{
        // If we have something to point to then add a reference to it
		if (0 != ptr)
			ptr->AddRef();
	}

    // Destructor - releases the class pointed to.
	~RwUtilRCAutoPtr()
	{
		if (0 != ptr)
			ptr->Release();
	}

//Assignement operators

    // the '=' operator used for assignment from a smart pointer
	T* operator = (const RwUtilRCAutoPtr<T>& rhs)
	{
        //If we are holding on to something then release it
        if(0 != ptr)
            ptr->Release();

        // Note how this line mirrors the code written to call this operator 
        ptr = rhs.ptr;

        //If we have been assigned something to point to then add ref it.
        if(0 != ptr)
            ptr->AddRef();

        return ptr;
	}

    // the '=' operator used for assignment from a dumb pointer
	T* operator = (T* rhs)
	{
        //If we are holding on to something then release it
        if(0 != ptr)
            ptr->Release();

        // Note how this line mirrors the code written to call this operator 
        ptr = rhs;

        //If we have been assigned something to point to then add ref it.
        if(0 != ptr)
            ptr->AddRef();

        return ptr;
	}

// Operators to allow us to use this class as a pointer

    // the '->' operator
	T* operator -> () const
	{ 
        // if this operator has been used ptr is about to be passed as a this parameter
        // to an object method so we should chek it is not null
		assert(ptr!=NULL);
		return ptr; 
	}

    // the '*' operator
	T& operator * () const
	{
        // check that ptr is not null before de-referencing it
		assert(ptr!=NULL);
		return *ptr;
	}

    //The '(T*)' cast operator
	operator T* () const
	{
		return ptr;
	}

// Operators for comparissons between pointers

    //the '!=' operator used for equality checks
	bool operator != (T* rhs) const
	{
		return ptr != rhs;
	}

    //the '==' operator used for equality checks
	bool operator == (T* rhs) const
	{
		return ptr == rhs;
	}

    //the '<' operator - this is used in some STL containers for sorting
	bool operator < (T* rhs) const
	{
		return ptr < rhs;
	}

    //the '!' operator used to check if a pointer is set to 0
	bool operator ! () const
	{
		return (0 == ptr);
	}


//Some helpful functions for doing pointer management


    /*
    *
    * This function will attach to an existing pointer.  No reference counting is done on the new pointer, but any
    * held pointers are released
    *
    * \param toPointTo the pointer to attach to
    */
	void Attach(T* toPointTo)
	{
		if (ptr)
			ptr->Release();
		ptr = toPointTo;
	}

    /*
    *
    * This function will detach from the held pointer without releasing it
    *
    *  \returns the address of the object pointed to.
    *
    */
	T* Detach()
	{
		T* oldPointer = ptr;
		ptr = 0;
		return oldPointer;
	}

    /*
    *
    * This function releases the object pointed to and sets this pointer to point to null
    *
    */

	void Release()
	{
	    ptr->Release();
	    ptr = 0;
	}

    /*
    *
    * This function allows you to copy the value of the pointer in to a "dumb" pointer.
    * It will automatically add a reference for the object pointed to.
    *
    * \param toCopyTo address of the pointer to copy to
    *
    */
	void CopyTo(T** toCopyTo)
	{
		assert(0 != toCopyTo);
		*toCopyTo = ptr;
		if (ptr)
			ptr->AddRef();
	}
    /*
    *
    * This function gets the raw pointer value
    *
    *  \returns the address of the object pointed to.
    */
	T* GetPtr()
	{
		return ptr;
	}

};
