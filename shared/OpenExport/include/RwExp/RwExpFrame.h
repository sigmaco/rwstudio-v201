// RwExpFrame.h: interface for the RwExpFrame class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWEXPFRAME_H__919C836C_A390_4353_B5DE_0C230E430C62__INCLUDED_)
#define AFX_RWEXPFRAME_H__919C836C_A390_4353_B5DE_0C230E430C62__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwExp.h"
#include "RwCommExportObject.h"

#include "RwExpTransform.h"

class RwCommTransform;

const RwCommType RwExpFrameType("RwExpFrame");

/**
*  \ingroup exportobject
*
*   This class is responsible for exporting an RwFrame object from an RwCommTransform object.
*   In its main Export() method the class extracts the RwCommTransform's object space matrix, removes
*   its scaling and builds an RwFrame object from the matrix.
*
*   \see RwCommTransform
*   \see RwFrame
*/
class RWEXP_API RwExpFrame  : public RwExpTransform
{
public:
    RwExpFrame();
    virtual ~RwExpFrame();

    virtual RwCommType  GetType() const { return RwExpFrameType; }

    virtual RwCommError Export(RwCommObject *pObj,
                            RwCommObject *pParent,
                            RwCommScene *pScene,
                            const RwCommExportOptions *pExportOptions );

    /**
    *   This function adds the passed user data onto the exported RwFrame as RpUserData arrays.
    *   Simply calls AddUserDataToRwFrame.
    *
    *   \param userDataColl the user data to be added
    *
    *   \returns the value returned by AddUserDataToRwFrame
    */
    virtual RwCommError     AddUserData(RwCommUserDataCollection &userDataColl);

    /**
    *   This function adds the passed user data onto the passed RwFrame as RpUserData arrays.
    *   It iterates through the members of userDataColl and skips any which have types not equal
    *   to RwCommUserDataCollection::transformUserDataType or which have zero elements. For valid
    *   members it creates an RpUserData array on the frame using RwFrameAddUserDataArray. It then
    *   calls RwExpCommUserDataToRpUserDataArray to fill in the data.
    *
    *   \param pFrame the frame that user data is to be added to
    *   \param userDataColl the user data to be added
    *
    *   \returns RWCOMM_SUCCESS
    */
    static RwCommError      AddUserDataToRwFrame(RwFrame *pFrame,
                                RwCommUserDataCollection &userDataColl);

    /**
     *  Get the generated RwFrame object.
     *  \return the frame generated
     */
    RwFrame     *GetFrame() { return m_pFrame; }

protected:

    virtual RwCommError Attach(RwFrame *pParentFrame, RwFrame *pFrame);
    virtual RwCommError AddObjectNameAsUserData(const RwWString &userDataEntry, RwCommTransform *pTransform);

protected:

    RwFrame     *m_pFrame;
};

#endif // !defined(AFX_RWEXPFRAME_H__919C836C_A390_4353_B5DE_0C230E430C62__INCLUDED_)
