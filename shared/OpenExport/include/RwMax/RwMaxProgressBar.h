// RwMaxProgressBar.h: interface for the RwMaxProgressBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWMAXPROGRESSBAR_H__59DCF125_D0CA_4CDE_B9DC_CC4982ED7AB9__INCLUDED_)
#define AFX_RWMAXPROGRESSBAR_H__59DCF125_D0CA_4CDE_B9DC_CC4982ED7AB9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
*  \ingroup maxhelper 
*
*   This is the RwMax implementation of RwCommProgressBar. It implements progress and cancellation
*   functionality using the functions provided by the core interface.
*
*   \see RwCommProgressBar
*/
class RwMaxProgressBar : public RwCommProgressBar  
{
public:
	RwMaxProgressBar();
	virtual ~RwMaxProgressBar();

    virtual RwCommError StartGUI();
    virtual RwCommError EndGUI();
    virtual RwCommError UpdateGUI(const RwCommReal total);
    virtual RwCommError IsCancelled(bool &bCancelled);

protected:

    RwCommReal m_total;
};

#endif // !defined(AFX_RWMAXPROGRESSBAR_H__59DCF125_D0CA_4CDE_B9DC_CC4982ED7AB9__INCLUDED_)
