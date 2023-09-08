// RwMIAnimProp.h: interface for the RwMIAnimProp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWMIANIMPROP_H__CB778B2B_302C_4184_802F_8372E0F76EF5__INCLUDED_)
#define AFX_RWMIANIMPROP_H__CB778B2B_302C_4184_802F_8372E0F76EF5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwCommAnimatable.h"

/**
*   \ingroup mihelper
*
*   This is a template struct for storing key frame values. It contains
*   the key's time (frame) and its actual value.
*/
template<class T>
struct RwMIAnimPropKey
{
    RwMIAnimPropKey(RwCommReal time, const T &value) : m_time(time), m_value(value) {}

    /** The frame of the key*/
    RwCommReal  m_time;

    /** The key value*/
    T           m_value;
};

template<class T>
struct RwMIAnimPropKeyPred
{
    bool operator() (const RwMIAnimPropKey<T> &k1, const RwMIAnimPropKey<T> &k2) const
                    { return k1.m_time < k2.m_time; }
};

template<class T>
struct RwMIAnimPropNoInterp
{
    T operator() (RwCommReal factor, const T &v1, const T &v2) const
    {
        if (factor == 0.0)
        {
            return v1;
        }
        else
        {
            return v2;
        }
    }
};

template<class T, class Interp = RwMIAnimPropNoInterp<T> >
class RwMIAnimProp
{
public:
    RwMIAnimProp() {}
    virtual ~RwMIAnimProp() {}

    int     NumKeys()
    {
        return m_keys.size();
    }

    void    AddKey(RwCommReal time, const T &value)
    {
        _STL::pair<RwSet< RwMIAnimPropKey<T>, RwMIAnimPropKeyPred<T> >::iterator, bool> ins =
            m_keys.insert(RwMIAnimPropKey<T>(time, value));

        if (!ins.second)
        {
            // Lose the constness of the reference we get from a set
            RwMIAnimPropKey<T> &existingKey = const_cast<RwMIAnimPropKey<T> &>(*ins.first);
            
            existingKey.m_value = value;
        }
    }

    void    AddKeys(RwVector< RwMIAnimPropKey<T> >  &vKeys)
    {
        for (unsigned int i = 0; i < vKeys.size(); i++)
        {
            AddKey(vKeys[i].m_time, vKeys[i].m_value);
        }
    }

    RwCommError    GetKey(RwCommReal time, T &value) const
    {
        RwCommError eResult = RWCOMM_FAIL;

        if (m_keys.size() > 0)
        {
            Interp interp;
            RwMIAnimPropKey<T> key(time, value);
            RwSet< RwMIAnimPropKey<T>, RwMIAnimPropKeyPred<T> >::const_iterator k1, k2;
            RwCommReal factor = 0.0;

            k2 = m_keys.lower_bound(key);

            if (k2 == m_keys.end())
            {
               k2--;
               k1 = k2;
            }
            else if (k2 == m_keys.begin())
            {
                k1 = k2;
            }
            else
            {
                k1 = k2;
                k1--;
            }
            
            // Check that time is within range
            if (time > k2->m_time)
            {
                time = k2->m_time;
            }
            else if (time < k1->m_time)
            {   
                time = k1->m_time;
            }   

            if (k2->m_time != k1->m_time)
            {
                factor = (time - k1->m_time)/(k2->m_time - k1->m_time);
            }

            // Interpolate keys
            value = interp(factor, k1->m_value, k2->m_value);
            eResult = RWCOMM_SUCCESS;
        }

        return eResult;
    }

    void    GetKeys(RwVector< RwMIAnimPropKey<T> > &vKeys) const
    {
        RwSet< RwMIAnimPropKey<T> , RwMIAnimPropKeyPred<T> >::const_iterator it;
        for (it = m_keys.begin(); it != m_keys.end(); it++)
        {
            vKeys.push_back(*it);
        }
    }

protected:
    RwSet< RwMIAnimPropKey<T>, RwMIAnimPropKeyPred<T> > m_keys;
};

#endif // !defined(AFX_RWMIANIMPROP_H__CB778B2B_302C_4184_802F_8372E0F76EF5__INCLUDED_)
