// ExportManager.cpp: implementation of the CExportManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ExportManager.h"

#include "RwExpStreamAsset.h"

#define NUM_REPS  3

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CExportManager::CExportManager()
{

}

CExportManager::~CExportManager()
{

}

RwCommError     
CExportManager::StreamAssets(RwExpStreamAssetList *pAssetList, const RwWString &filename)
{
    // If this is a view operation, call parent implementation
    if (m_projectOptions.GetOption(EXPORTTOVIEW).GetBool())
    {
        return RwExpMgrExportManager::StreamAssets(pAssetList, filename);
    }
    else // This is an export operation
    {
        const RwList<RwExpStreamAsset*> &lAssets = pAssetList->GetAssets();
        RwList<RwExpStreamAsset*>::const_iterator it;

        for (int i = 0; i < NUM_REPS; i++)
        {
            RwExpStreamAssetList tAssetList;
            wostringstream str;
            RwWString repFile(filename);

            // Generate new file name 
            repFile.erase(filename.size() - 4, 4);
            str << repFile << L"." << i << L".rws";
            repFile = str.str();

            for (it = lAssets.begin(); it != lAssets.end(); it++)
            {
                // Fetch the i-th asset
                const RwExpStreamAsset *pAsset = *it;

                // Check if it's a clump
                unsigned id = pAsset->GetPluginID();
                if (id == rwID_CLUMP)
                {
                    // Clone the clump
                    RpClump *pClump = RpClumpClone((RpClump*)pAsset->GetData());

                    // Modify the clump
                    ModifyClump(pClump, i);

                    tAssetList.AddEntry(new RwExpStreamAsset(id, pClump, RwWString(L"")));
                }

            
            }

            // Stream the new asset list
            tAssetList.StreamToFile(repFile, false);

            // Clear all the assets
            tAssetList.ClearAssets();

        }
    }

    return RWCOMM_SUCCESS;
}


void    
CExportManager::ModifyClump(RpClump *pClump, int rep)
{
    // Do some clump modification.
    // In our case, for simplicity, we just modify all of the clumps' material colors
    RpClumpForAllAtomics(pClump, AtomicCallback, &rep);
}

RpAtomic  *
CExportManager::AtomicCallback(RpAtomic * pAtomic, void *data)
{
    int rep = *((int *)data);

    RpGeometry *pGeometry = RpAtomicGetGeometry(pAtomic);
    RpGeometryForAllMaterials(pGeometry, MaterialCallback, &rep);

    return pAtomic;
}

RpMaterial  *
CExportManager::MaterialCallback(RpMaterial *pMaterial, void *data)
{
    RwRGBA color;
    int rep = *((int *)data);
    
    if (rep == 0)
    {
        color.red = 255;
        color.green = 0;
        color.blue = 0;
        color.alpha = 255;
        RpMaterialSetColor(pMaterial, &color);
    }
    else if (rep == 1)
    {
        color.red = 0;
        color.green = 255;
        color.blue = 0;
        color.alpha = 255;
        RpMaterialSetColor(pMaterial, &color);
    }
    else if (rep == 2)
    {
        color.red = 0;
        color.green = 0;
        color.blue = 255;
        color.alpha = 255;
        RpMaterialSetColor(pMaterial, &color);
    }


    return pMaterial;
}