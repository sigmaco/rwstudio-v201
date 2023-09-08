// RwExpRpLight.h: interface for the RwExpRpLight class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWEXPRPLIGHT_H__5381B06F_BBFF_4725_9105_7BD131251D96__INCLUDED_)
#define AFX_RWEXPRPLIGHT_H__5381B06F_BBFF_4725_9105_7BD131251D96__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwExp.h"
#include "RwCommExportObject.h"

class RwCommTransform;
class RwCommLight;

const RwCommType RwExpRpLightType("RwExpRpLight");

/**
*  \ingroup exportobject
*   This is the class responsible for exporting RpLight objects. It is mapped to the RwCommLight object in the Dff
*    exporter.
*   In its main Export() method the class creates an RpLight, sets its light type, sets its color and intensity,
*   and adjusts its radius and cone angle if necessary. Later on, it attaches the light to its appropriate frame.
*   
*   Users wishing to customize this step can override the Attach() method. 
*   
*   At the end of the Export() method the light is added to the clump.
*/
class RWEXP_API RwExpRpLight  : public RwCommExportObject
{
public:
    RwExpRpLight();
    virtual ~RwExpRpLight();

    virtual RwCommType      GetType() const { return RwExpRpLightType; }

    virtual RwCommError     Export(RwCommObject *pObj,
                                RwCommObject *pParent,
                                RwCommScene *pScene,
                                const RwCommExportOptions *pExportOptions );

    /**
    *   This function adds the passed user data onto the light as RpUserData arrays.
    *   It iterates through the members of userDataColl and skips any which have types not equal
    *   to RwCommUserDataCollection::lightUserDataType or which have zero elements. For valid
    *   members it creates an RpUserData array on the light using RpLightAddUserDataArray. It then
    *   calls RwExpCommUserDataToRpUserDataArray to fill in the data.
    *
    *   \param userDataColl the user data to be added
    *
    *   \returns RWCOMM_SUCCESS
    */
    virtual RwCommError     AddUserData(RwCommUserDataCollection &userDataColl);

    /** Get the generated RpLight object.
     *
     *  \return the light generated, null if there was an error
     */
    RpLight                 *GetRpLight() const { return m_pRpLight; }

protected:

    // Attach camera to frame
    virtual RwCommError Attach(RpLight *pLight, RwCommTransform *pTransform);
    virtual RwCommError AddObjectNameAsUserData(const RwWString &userDataEntry, RwCommLight *pLight);

protected:

    RpLight *m_pRpLight;
};

#endif // !defined(AFX_RWEXPRPLIGHT_H__5381B06F_BBFF_4725_9105_7BD131251D96__INCLUDED_)
