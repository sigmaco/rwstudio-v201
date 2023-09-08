#if !defined(RW_MAYA_PROGRESS_BAR_H)
#define RW_MAYA_PROGRESS_BAR_H

/**
*  \ingroup mayahelper 
*
*  This is the RwMaya implementation of RwCommProgressBar. It implements progress bar functionality
*  (including cancellation) using the "progressBar" MEL command.
*
*   \see RwCommProgressBar
*/
class RWMAYA_API RwMayaProgressBar: public RwCommProgressBar 
{
public:
	RwMayaProgressBar();
	virtual ~RwMayaProgressBar();
    
    virtual RwCommError StartGUI();
    virtual RwCommError EndGUI();
    virtual RwCommError UpdateGUI(const RwCommReal total);
    virtual RwCommError IsCancelled(bool &bCancelled);

protected:
};

#endif // !defined(RW_MAYA_PROGRESS_BAR_H)
