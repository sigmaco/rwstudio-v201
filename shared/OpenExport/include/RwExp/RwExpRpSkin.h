// RwExpRpSkin.h: interface for the RwExpRpSkin class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWEXPRPSKIN_H__9E496C14_005A_4803_B1F2_F72B3112E705__INCLUDED_)
#define AFX_RWEXPRPSKIN_H__9E496C14_005A_4803_B1F2_F72B3112E705__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwExp.h"
#include "RwCommExportObject.h"

#include "RwCommSkin.h"

class RwCommGeometry;

const RwCommType RwExpRpSkinType("RwExpRpSkin");

/**
*  \ingroup exportobject
*   This is the class responsible for creating and attaching the RpSkin object to its skinned geometry.
*   The class is mapped to the RwCommSkin modifier in the Dff exporter. In its main Export() method the class
*   extracts all the vertex Ids from the vertex collection of the exported skinned geometry. 
*   It then extracts the skin weights, (using the vertex Ids), from the RwCommSkin object. Later on it gets the list
*   of bones from the RpHAnimHierarchy, calculates their inverse matrices and sets them to the RpSkin structure.
*   After the RpSkin has been created and initialized it is attached to the skinned geometry.

*/
class RWEXP_API RwExpRpSkin : public RwCommExportObject  
{
public:
    RwExpRpSkin();
    virtual ~RwExpRpSkin();

    virtual RwCommType      GetType() const { return RwExpRpSkinType; }

    virtual RwCommError     Export(RwCommObject *pObj,
                                RwCommObject *pParent,
                                RwCommScene *pScene,
                                const RwCommExportOptions *pExportOptions );

    /** Get the generated RpSkin object. */
    RpSkin                  *GetRpSkin() const                   { return m_pRpSkin; }

    /**
    *   Not implemented.
    *
    *   \param userDataColl the user data to be added
    *
    *   \returns RWCOMM_NOT_IMPLEMENTED
    */
    virtual RwCommError     AddUserData(RwCommUserDataCollection &userDataColl);

    int                     GetNumberWeights() const            { return m_numWeights; }
    bool                    HasUnweightedVerts() const          { return m_unweightedVerts; }

protected:
    void                    CalculateBoneInverseMatrix(RwMatrix *pMatrix,
                                RwFrame *pRootFrame, RwFrame *pFrame);
    void                    CalculateBoneInverseMatrix(RwMatrix *pMatrix,
                                RwCommTransform *pRootTransform, RwCommTransform *pTransform);
    void                    NormalizeWeights(RwVector<RwCommSkinWeight> &vWeights);
    RwCommSkin              *GetSkinFromGeometry(RwCommGeometry *pGeometry);

protected:
    RpSkin                  *m_pRpSkin; 
    int                     m_numWeights;
    bool                    m_unweightedVerts;
};

#endif // !defined(AFX_RWEXPRPSKIN_H__9E496C14_005A_4803_B1F2_F72B3112E705__INCLUDED_)
