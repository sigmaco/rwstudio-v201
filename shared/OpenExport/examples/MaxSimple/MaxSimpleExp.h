// MaxSimpleExp.h: interface for the MaxSimpleExp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAXSIMPLEEXP_H__D4FD7009_2760_486D_B6BE_8A9B60D66652__INCLUDED_)
#define AFX_MAXSIMPLEEXP_H__D4FD7009_2760_486D_B6BE_8A9B60D66652__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define SIMPLEEXP_CLASS_ID	Class_ID(0x85548e0c, 0x4a26450d)

#include "RwUtilTypes.h"

class MaxSimpleExp  : public SceneExport
{
public:
	MaxSimpleExp();
	virtual ~MaxSimpleExp();

	// SceneExport methods
	int    ExtCount();     
	const TCHAR * Ext(int n);     
	const TCHAR * LongDesc();     
	const TCHAR * ShortDesc();    
	const TCHAR * AuthorName();    
	const TCHAR * CopyrightMessage();   
	const TCHAR * OtherMessage1();   
	const TCHAR * OtherMessage2();   
	unsigned int Version();     
	void	ShowAbout(HWND hWnd);  
	int		DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts=FALSE, DWORD options=0); // Export	file
	BOOL	SupportsOptions(int ext, DWORD options);

private:

    void StripToPath(RwString &inString);
    void StripToFileName(RwString &inString);
    void StripToExtension(RwString &inString);
    void StripOffExtension(RwString &inString);

};

class MaxSimpleExpDesc:public ClassDesc {
public:
	int				IsPublic() { return 1; }
	void*			Create(BOOL loading = FALSE) { return new MaxSimpleExp; } 
	const TCHAR*	ClassName() { return _T("MaxSimpleExp"); }
	SClass_ID		SuperClassID() { return SCENE_EXPORT_CLASS_ID; } 
	Class_ID		ClassID() { return SIMPLEEXP_CLASS_ID; }
	const TCHAR*	Category() { return _T("Standard"); }
};

#endif // !defined(AFX_MAXSIMPLEEXP_H__D4FD7009_2760_486D_B6BE_8A9B60D66652__INCLUDED_)
