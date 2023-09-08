// MaxSimpleExp.cpp: implementation of the MaxSimpleExp class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"

class RwExpAnmExporter;
class RwExpBspExporter;
class RwExpSplExporter;
class RwExpDffExporter;
class RwRf3Builder;
class RwRf3Exporter;
class RwExpStreamAssetList;
struct RwRf3AssetInfo;

#include "RwUtilString.h"
#include "MaxSimpleExp.h"
#include "MaxSimpleBuilder.h"
#include "MaxSimpleAsset.h"

#include "RwCommExporter.h"

#include "RwExpExportOptions.h"
#include "RwExpMgr.h"
#include "RwExpMgrExportManager.h"

static bool bInitBuilder = false;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MaxSimpleExp::MaxSimpleExp()
{
    if (!bInitBuilder)
    {
        if (RwExpMgrExportManager::TheInstance()->GetBuilder() == NULL)
        {
            // Assign our builder to the export manager
            RwExpMgrExportManager::TheInstance()->SetBuilder(new MaxSimpleBuilder);

            bInitBuilder = true;
        }
        else
        {
            MessageBox(NULL, "The RwExpMgrExportManager has already got a registered builder. Please remove the rwexp.dle plugin form the 3dsmax plugin directory.",
                "MaxSimple Example", MB_OK|MB_ICONSTOP);
        }
    }

}

MaxSimpleExp::~MaxSimpleExp()
{

}

int MaxSimpleExp::ExtCount()
{
	return 1;
}

const TCHAR * MaxSimpleExp::Ext(int n)
{
	switch(n) {
	case 0:
		// This cause a static string buffer overwrite
		// return GetString(IDS_EXTENSION1);
		return _T("rws");
	}
	return _T("");
}

const TCHAR * MaxSimpleExp::LongDesc()
{
	return _T("RenderWare Sample Exporter");
}

const TCHAR * MaxSimpleExp::ShortDesc()
{
	return _T("RenderWare Stream");
}

const TCHAR * MaxSimpleExp::AuthorName() 
{
	return _T("");
}

const TCHAR * MaxSimpleExp::CopyrightMessage() 
{
	return _T("");
}

const TCHAR * MaxSimpleExp::OtherMessage1() 
{
	return _T("");
}

const TCHAR * MaxSimpleExp::OtherMessage2() 
{
	return _T("");
}

unsigned int MaxSimpleExp::Version()
{
	return 100;
}

void MaxSimpleExp::ShowAbout(HWND hWnd)
{
	
}

BOOL MaxSimpleExp::SupportsOptions(int ext, DWORD options)
{
	assert(ext == 0);	// We only support one extension
	return(options == SCENE_EXPORT_SELECTED) ? TRUE : FALSE;
}

// Dummy function for progress bar
DWORD WINAPI fn(LPVOID arg)
{
	return(0);
}

// Start the exporter!
// This is the real entrypoint to the exporter. After the user has selected
// the filename (and he's prompted for overwrite etc.) this method is called.
int MaxSimpleExp::DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts, DWORD options) 
{
    if (bInitBuilder)
    {
        RwString path(name), filename(name);
        RwVector<RwCommAsset *> vAssets;
        MaxSimpleAsset asset;

        // Format path & filename
        StripToPath(path);
        StripToFileName(filename);
        StripOffExtension(filename);

        // Create a static world asset
        asset.SetAssetTemplateName(L"Static World.rwt");
        asset.SetName(RwUtilA2W(filename));
        vAssets.push_back(&asset);

	    // Set a global prompt display switch
	    BOOL exportSelected = (options & SCENE_EXPORT_SELECTED) ? TRUE : FALSE;

	    // Startup the progress bar.
	    i->ProgressStart(_T(""), TRUE, fn, NULL);
    
        // Export the scene, using the generic project template
        RwExpMgrExportManager::TheInstance()->Export(vAssets, L"Generic.rwt", RwUtilA2W(filename), RwUtilA2W(path));

	    // We're done. Finish the progress bar.
	    i->ProgressEnd();
    }
    else
    {
        MessageBox(NULL, "MaxSimpleBuilder failed to register",
                "MaxSimple Example", MB_OK|MB_ICONSTOP);
    }

	return 1;
}

void
MaxSimpleExp::StripToPath(RwString &inString)
{
    int slashpos = inString.find_last_of('\\');
    
    if (slashpos != -1)
    {
        inString.erase(slashpos + 1);
    }
}

void
MaxSimpleExp::StripToFileName(RwString &inString)
{
    int slashpos = inString.find_last_of('\\');
    
    if (slashpos != -1)
    {
        inString = inString.substr(slashpos + 1);
    }
}

void
MaxSimpleExp::StripToExtension(RwString &inString)
{
    int dotPos = inString.find_last_of('.');
    int slashPos = inString.find_last_of('\\');
 
    
    if (dotPos <= slashPos)
    {
        // no extension
        inString = "";
    }
    else
    {
        inString = inString.substr(dotPos + 1);
    }
}

void 
MaxSimpleExp::StripOffExtension(RwString &inString)
{
    int dotPos = inString.find_last_of('.');
    int slashPos = inString.find_last_of('\\');
     
    if (dotPos>slashPos)
    {
        inString = inString.erase(dotPos);
    }
}
