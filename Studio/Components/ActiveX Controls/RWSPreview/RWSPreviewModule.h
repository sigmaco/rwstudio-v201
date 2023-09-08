#ifndef __RWSPREVIEWMODULE_H__
#define __RWSPREVIEWMODULE_H__

class CRWSPreviewControl;

//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
class CRWSPreviewModule : public CComModule
{
public:
	LONG Unlock();
	DWORD dwThreadID;
	HANDLE hEventShutdown;
	void MonitorShutdown();
	bool StartMonitor();
	bool bActivity;

   void ProcessMessageLoop ();
   void SetIdleHandler (CRWSPreviewControl * pIdleHandler = 0);

   // time for exe to be PAUSE before shutting down
   // TIMEOUT to wait for threads to finish up
   enum {PAUSE = 100, TIMEOUT = 500};

protected:
   CRWSPreviewControl * m_pIdleHandler;
};

#endif // __RWSPREVIEWMODULE_H__