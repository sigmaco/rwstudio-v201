// RwExpScene.h: interface for the RwExpScene class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWEXPSCENE_H__CBFDBEC6_57DC_45E3_9D81_203FE33D7823__INCLUDED_)
#define AFX_RWEXPSCENE_H__CBFDBEC6_57DC_45E3_9D81_203FE33D7823__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#pragma warning(disable : 4786)
#pragma warning(disable : 4251)

#include <map>
#include <set>

#include "RwCommObject.h"
#include "RwCommShader.h"
#include "RwCommTransform.h"
#include "RwCommExportOptions.h"
#include "RwCommAsset.h"

/**
 *  \ingroup commscenegraph 
 *
 *  This class is the main container of all the scene graph elements. Through this class you can
 *  reach and access every scene graph object in the scene. Only one RwCommScene instance is
 *  created per scene graph.
 */
class RWCOMM_API RwCommScene : public RwCommObject 
{
public:
    RwCommScene();
    virtual ~RwCommScene();

    // Root object methods
    void AddRootObject(RwCommObject *pObject);

    /**
     *  The scene holds a list of all of its root nodes, each holding its own hierarchy.
     *  Use this function to get all the root objects.
     *
     *  \return a vector of root objects.
     */
    const RwVector<RwCommObject *> &GetRootObjects() const { return m_vRoots; }

    // Add an object to the scene
    void AddObject(RwCommObject *pObject);

    // Get an object by name
    RwCommObject* GetCommObject(const RwWString &name);

    // Get objects by type
    void GetAllObjectsOfType(RwCommType type, RwVector<RwCommObject *> &vObjects) const;

    // Get all objects in the scene
    void GetAllObjects(RwVector<RwCommObject *> &vObjects) const;

    /**
     *  The scene holds a list of all of its asset nodes.
     *  Use this function to get all the asset objects.
     *
     *  \return a vector of asset nodes.
     */
    const RwVector<RwCommAsset *> &GetAssets() const { return m_vAssets; }
    
    // Add an asset to the scene
    void AddAsset(RwCommAsset *pAsset);

    // Scene settings
    /**
     *  Converts frames to seconds.
     *
     *  \param frame to be converted to seconds.
     *
     *  \return the time in seconds.
     */
    virtual RwCommReal  FramesToSec(RwCommReal frame) const = 0;

    /**
     *  Each modeling tool has its own idea of its native modeling axis system, if it is a
     *  right-handed or left-handed system and if Z is up or Y is down. The RwComm scene
     *  graph assumes we are working with a right handed orthogonal coordinate system,
     *  where Z points into the screen. Modeling tools which work with a different axis
     *  system implement the abstract method GetAxisSystem() that returns a world space
     *  matrix reorienting the default axis system. To export the 3D scene graph elements
     *  in the native modeling tool axis system, make sure to transform them by this matrix.    
     *
     *  \param matrix is filled with the transformation to the RwComm coordinate system.
     *
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError GetAxisSystem(RwCommMatrix &matrix) const = 0;

    /** Gets the scene's ambient color.
     *
     *  \param color returns the ambient color of the scene.
     *
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError GetAmbientColor(RwCommVec3 &color) const { return RWCOMM_NOT_IMPLEMENTED; }

    /** Gets the scene's background color.
     *
     *  \param color returns the background color of the scene.
     *
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError GetBackgroundColor(RwCommVec3 &color) const { return RWCOMM_NOT_IMPLEMENTED; }

    /** Gets the scene's start frame.*/
    virtual RwCommReal GetStartFrame() const = 0;

    /** Gets the scene's end frame.*/
    virtual RwCommReal GetEndFrame() const = 0;
   
    /** Gets the scene's description.*/
    virtual RwCommError GetDescription(RwWString &description) const = 0;

    /** Gets the scene's guid.*/
    virtual RwCommError GetGUID(RwWString &guid) const = 0;

    /** Gets the scene's bounding box.
     *
     *  \return the scene's bounding box.
    */
    virtual RwCommBoundingBox GetBoundingBox() const;

    /** Specifies a relative order between all RwCommObjects in the scene. */
    void Rehash();

private:
    RwMap<RwWString, RwCommObject *> m_mNameTable;
    RwVector<RwCommObject *>        m_vRoots;
    RwSet<RwCommObject *>           m_sObjects;
    RwVector<RwCommAsset *>         m_vAssets;

    // mutable means they can be modified inside a const function...
    mutable RwCommBoundingBox       m_Bounds;
    mutable bool                    m_BoundsNeedRecalculating;
};

#endif // !defined(AFX_RWEXPSCENE_H__CBFDBEC6_57DC_45E3_9D81_203FE33D7823__INCLUDED_)
