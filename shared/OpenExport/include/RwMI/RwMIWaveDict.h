// RwMIWaveDict.h: interface for the RwMIWaveDict class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWMIWAVEDICT_H__143BBEE9_8895_43B9_A40B_5110C21F9483__INCLUDED_)
#define AFX_RWMIWAVEDICT_H__143BBEE9_8895_43B9_A40B_5110C21F9483__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwMI.h"
#include "RwCommWaveDict.h"

class RWMI_API RwMIWaveDict : public RwCommWaveDict  
{
public:
	RwMIWaveDict();
	virtual ~RwMIWaveDict();

    RwCommError RemoveWave(RwCommWave *pWave);
};

#endif // !defined(AFX_RWMIWAVEDICT_H__143BBEE9_8895_43B9_A40B_5110C21F9483__INCLUDED_)
