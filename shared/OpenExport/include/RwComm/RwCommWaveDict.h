// RwCommWaveDict.h: interface for the RwCommWaveDict class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWCOMMWAVEDICT_H__60B5CE8C_BDF9_4BD5_98EF_0CDFC53AD40D__INCLUDED_)
#define AFX_RWCOMMWAVEDICT_H__60B5CE8C_BDF9_4BD5_98EF_0CDFC53AD40D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwCommAudioObject.h"
#include "RwCommWave.h"

const RwCommType RwCommWaveDictType("RwCommWaveDict");

class RWCOMM_API RwCommWaveDict : public RwCommAudioObject  
{
public:
	RwCommWaveDict();
	virtual ~RwCommWaveDict();

    // Type methods
    virtual RwCommType  GetType() const { return RwCommWaveDictType; }
    virtual void        GetTypeHierarchy(vector<RwCommType> &vTypeVec) const;

    // Wave managment
    int             GetNumWaves() const;
    void            AddWave(RwCommWave *pWave);
    RwCommWave      *GetWave(int index);

protected:

    vector<RwCommWave *>    m_vWaves;

};

#endif // !defined(AFX_RWCOMMWAVEDICT_H__60B5CE8C_BDF9_4BD5_98EF_0CDFC53AD40D__INCLUDED_)
