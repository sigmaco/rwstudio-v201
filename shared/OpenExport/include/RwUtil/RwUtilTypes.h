#ifndef RWUTIL_TYPES_H
#define RWUTIL_TYPES_H

#pragma warning(disable : 4786)
#pragma warning(disable : 4251)

#include "RwUtil.h"
#include "RwUtilAlloc.h"

/**
*  \ingroup rwutiltypes
*   This custom STL allocator is used by all the STL containers used
*   within OpenExport. This is done to ensure that when plugins
*   manipulate these containers all memory allocations and de-allocations
*   use a single runtime library.
*/
template<typename T> class RwAllocator: public _STL::allocator<T> 
{
public:
    // These templated types are defined in the base class, but for some reason we
    // are getting warnings for them in the gcc3+, so we'll define them here as well.
    typedef size_t  rw_size_type;
    typedef T*      rw_pointer;

    rw_pointer
    allocate(rw_size_type n, const void * = 0)
    {
        return (rw_pointer)(RwUtilMalloc(n * sizeof(T)));
    }

    void
    deallocate(rw_pointer p, rw_size_type)
    {
        RwUtilFree((void *)p);
    }

#ifdef _STLP_NO_MEMBER_TEMPLATE_CLASSES
    // Redefine __stl_alloc_rebind and __stl_alloc_create.
    // Need to do this for any compiler that doesn't support member templates
    template <class _Tp1, class _Tp2> inline RwAllocator<_Tp2>&
    __stl_alloc_rebind(RwAllocator<_Tp1>& __a, const _Tp2*)
    {
        return ((RwAllocator<_Tp2>&)(__a));
    }

    template <class _Tp1, class _Tp2> inline RwAllocator<_Tp2>
    __stl_alloc_create(const RwAllocator<_Tp1>&, const _Tp2*)
    {
        RwAllocator<_Tp2> defAllocator;
        return (defAllocator);
    }
#endif // _STLP_NO_MEMBER_TEMPLATE_CLASSES
};

/**
*  \ingroup rwutiltypes
*   An STL pair that uses RwAllocator.  See \ref utiltypesoverview for why these classes are used in the Open Export SDK.
*/

template <class T1, class T2> class RwPair : public _STL::pair< T1, T2 >
{
public:
    RwPair() : _STL::pair<T1, T2> () {}
    RwPair(const T1& a, const T2& b) : _STL::pair<T1, T2> (a, b) {}
};

/**
*  \ingroup rwutiltypes
*   A basic_string using char types and RwAllocator.  See \ref utiltypesoverview for why these classes are used in the Open Export SDK.
*/
namespace
{
using _STL::char_traits;
typedef _STL::basic_string<char, char_traits<char> , RwAllocator<char> > RwString;

/**
*  \ingroup rwutiltypes
*   A basic_string using wchar_t types and RwAllocator.  See \ref utiltypesoverview for why these classes are used in the Open Export SDK.
*/
typedef _STL::basic_string<wchar_t, char_traits<wchar_t> , RwAllocator<wchar_t> > RwWString;
}


/**
*  \ingroup rwutiltypes
*   A vector container that uses RwAllocator.  See \ref utiltypesoverview for why these classes are used in the Open Export SDK.
*/
template <class T> class RwVector : public _STL::vector< T, RwAllocator<T> >
{
public:
    explicit RwVector() : _STL::vector<T, RwAllocator<T> > () {}
    RwVector(const RwVector<T> &_x) : _STL::vector<T, RwAllocator<T> > (_x) {}
    explicit RwVector(size_type _n) : _STL::vector<T, RwAllocator<T> > (_n) {}
    RwVector(size_type _n, const T &_val) : _STL::vector<T, RwAllocator<T> > (_n, _val) {}
#ifndef _STLP_NO_MEMBER_TEMPLATES
    template <class _InputIterator>
    RwVector(_InputIterator _first, _InputIterator _last)
        : _STL::vector<T, RwAllocator<T> > (_first, _last) {}
#else // _STLP_MEMBER_TEMPLATES
    RwVector(const T *_first, const T *_last) : _STL::vector<T, RwAllocator<T> > (_first, _last) {}
#endif // _STLP_MEMBER_TEMPLATES
  };

/**
*  \ingroup rwutiltypes
*   A list container that uses RwAllocator.  See \ref utiltypesoverview for why these classes are used in the Open Export SDK.
*/
template <class T> class RwList : public _STL::list< T, RwAllocator<T> >
{
public:
    RwList() : _STL::list<T, RwAllocator<T> > () {}
    RwList(const RwList<T> & _x) : _STL::list<T, RwAllocator<T> > (_x) {}
    explicit RwList(size_type _n) : _STL::list<T, RwAllocator<T> > (_n) {}
    RwList(size_type _n, const T &_val) : _STL::list<T, RwAllocator<T> > (_n, _val) {}
#ifndef _STLP_NO_MEMBER_TEMPLATES
    template <class _InputIterator>
    RwList(_InputIterator _first, _InputIterator _last) : _STL::list<T, RwAllocator<T> > (_first, _last) {}
#else // _STLP_MEMBER_TEMPLATES
    RwList(const T *_first, const T *_last) : _STL::list<T, RwAllocator<T> > (_first, _last) {}
    RwList(const_iterator _first, const_iterator _last) : _STL::list<T, RwAllocator<T> > (_first, _last) {}
#endif // _STLP_MEMBER_TEMPLATES
};

/**
*  \ingroup rwutiltypes
*   A set container that uses RwAllocator.  See \ref utiltypesoverview for why these classes are used in the Open Export SDK.
*/
template <class T, class comp = _STL::less<T> > class RwSet : public _STL::set< T, comp, RwAllocator<T> >
{
public:
    RwSet() : _STL::set<T, comp, RwAllocator<T> > () {}
    explicit RwSet(const comp &_comp) : _STL::set<T, _comp, RwAllocator<T> >() {}
    RwSet(const RwSet<T> &_x) : _STL::set<T, comp, RwAllocator<T> > (_x) {}
#ifndef _STLP_NO_MEMBER_TEMPLATES
    template <class _InputIterator>
    RwSet(_InputIterator _first, _InputIterator _last)
        : _STL::set<T, comp, RwAllocator<T> > (_first, _last) {}
    template <class _InputIterator>
    RwSet(_InputIterator _first, _InputIterator _last, const comp &_comp)
        : _STL::set<T, comp, RwAllocator<T> > (_first, _last, _comp) {}
#else // _STLP_MEMBER_TEMPLATES
    RwSet(const value_type *_first, const value_type *_last) 
        : _STL::set<T, comp, RwAllocator<T> > (_first, _last) {}
    RwSet(const value_type *_first, const value_type *_last, const comp &_comp)
        : _STL::set<T, comp, RwAllocator<T> > (_first, _last, _comp) {}
    RwSet(const_iterator _first, const_iterator _last)
        : _STL::set<T, comp, RwAllocator<T> > (_first, _last) {}
    RwSet(const_iterator _first, const_iterator _last, const comp &_comp)
        : _STL::set<T, comp, RwAllocator<T> > (_first, _last, _comp) {}
#endif // _STLP_MEMBER_TEMPLATES
};

/**
*  \ingroup rwutiltypes
*   A map container that uses RwAllocator.  See \ref utiltypesoverview for why these classes are used in the Open Export SDK.
*/
template <class K, class T, class comp = _STL::less<K> > class RwMap :
    public _STL::map< K, T, comp, RwAllocator<T> >
{
public:
    RwMap() : _STL::map<K, T, comp, RwAllocator<T> > () {}
    explicit RwMap(const comp &_comp) : _STL::map<K, T, _comp, RwAllocator<T> >() {}
    RwMap(const RwMap<K, T, comp> &_x) : _STL::map<K, T, comp, RwAllocator<T> > (_x) {}
#ifndef _STLP_NO_MEMBER_TEMPLATES
    template <class _InputIterator>
    RwMap(_InputIterator _first, _InputIterator _last)
        : _STL::map<K, T, comp, RwAllocator<T> > (_first, _last) {}
    template <class _InputIterator>
    RwMap(_InputIterator _first, _InputIterator _last, const comp &_comp)
        : _STL::map<K, T, comp, RwAllocator<T> > (_first, _last,  __comp) {}
#else // _STLP_MEMBER_TEMPLATES
    RwMap(const value_type* _first, const value_type* _last)
        : _STL::map<K, T, comp, RwAllocator<T> > (_first, _last) {}
    RwMap(const value_type* _first, const value_type* _last, const comp &_comp)
        : _STL::map<K, T, comp, RwAllocator<T> > (_first, _last, _comp) {}
    RwMap(const_iterator _first, const_iterator _last)
        : _STL::map<K, T, comp, RwAllocator<T> > (_first, _last) {}
    RwMap(const_iterator _first, const_iterator _last, const comp &_comp)
        : _STL::map<K, T, comp, RwAllocator<T> > (_first, _last, _comp) {}
#endif // _STLP_MEMBER_TEMPLATES
};

#endif // RWUTIL_TYPES_H
