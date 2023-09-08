// RwCommAssetAutoCreator.h: interface for the RwCommAssetAutoCreator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWCOMMASSETAUTOCREATOR_H__C713DBD7_BACF_4A42_A255_656C6A4D3D8D__INCLUDED_)
#define AFX_RWCOMMASSETAUTOCREATOR_H__C713DBD7_BACF_4A42_A255_656C6A4D3D8D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwCommAsset.h"

typedef RwCommError (*RwCommInitOptionsFunc)(RwCommExportOptions &options, int type);

/**
*  \ingroup commscenegraph
*
*   This is a singleton class responsible for auto-creating assets. It will
*   analyze the scene and create an asset for each logical entity in the scene.
*   The class's main method, AutoCreateAssets(), starts the entire auto creation
*   proccess. 
*
*   \see RwCommAsset
*/
class RWCOMM_API RwCommAssetAutoCreator  
{
public:
    RwCommAssetAutoCreator();
    virtual ~RwCommAssetAutoCreator();
    
    /**
    *   \param pInstance of the derived class to register. RwCommAssetAutoCreator
    *          is a singleton.
    */
    static void RegisterInstance(RwCommAssetAutoCreator *pInstance);
    /**
    *   \return static instance of the class. RwCommAssetAutoCreator is a
    *           singleton.
    */
    static RwCommAssetAutoCreator *TheInstance();

    /**
    *   Registers an RwCommExportOptions initialization function.
    */
    static void RegisterInitOptionsFunc(RwCommInitOptionsFunc func) { m_pInitOptionsFunc = func; }

    /**
    *   Calls the RwCommExportOptions initialization function. This method is called
    *   for each new RwCommAsset we create, for initializing its RwCommOptions.
    *
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    static RwCommError InitOptions(RwCommExportOptions &options, int type);

    /**
    *   Analyzes the scene and calls AutoCreateAsset for each logical entity in the
    *   scene. If bSelectionOnly is set to true then only the current selection will
    *   be analyzed.
    *
    *   \param bSelectionOnly Boolean indicating whether the whole scene or just
    *          the current selection should be analyzed.
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    virtual RwCommError AutoCreateAssets(bool bSelectionOnly = false) = 0;
    /**
    *   Analyzes the current selection in the scene and creates an asset for the
    *   selected objects, with appropriate options.
    *
    *   \return the auto-created asset.
    */
    virtual RwCommAsset *AutoCreateAsset() = 0;

private:
    static RwCommAssetAutoCreator   *m_pTheInstance;
    static RwCommInitOptionsFunc    m_pInitOptionsFunc;
};

#endif // !defined(AFX_RWCOMMASSETAUTOCREATOR_H__C713DBD7_BACF_4A42_A255_656C6A4D3D8D__INCLUDED_)
