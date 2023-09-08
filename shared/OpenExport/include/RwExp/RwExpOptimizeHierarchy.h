// RwExpOptimizeHierarchy.h: interface for the RwExpOptimizeHierarchy class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWEXPOPTIMIZEHIERARCHY_H__45BD110D_F6F7_4D49_A88F_D512C8763B83__INCLUDED_)
#define AFX_RWEXPOPTIMIZEHIERARCHY_H__45BD110D_F6F7_4D49_A88F_D512C8763B83__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwExp.h"
#include "RwExpTraverseAction.h"

const RwCommType RwExpOptimizeHierarchyType("RwExpOptimizeHierarchy");

/**
*
*  \ingroup traverseactions
*
*   This class removes all redundant bones from the traverse list. Unlike the RwExpOptimizeBones action, which only
*   optimizes leaf bones, at the end of the hierarchy, the RwExpOptimizeHierarchy action also removes intermediate
*   bones.
*   
*   The action uses an optimization criteria, specified by the virtual 
*   RwExpOptimizeHierarchy::CanOptimizeObject() method. Users deriving from the RwExpOptimizeHierarchy class, can
*   introduce their own optimization criteria by overriding this method. The current implemented criteria does NOT
*   remove an object from the traverse list, if any of the following conditions are satisfied:
*   \li The object is not a transformation.
*   \li The transformation has a shape attached to it in the traverse list.
*   \li The transformation is a bone affecting a skinned geometry.
*   \li The user tagged the transformation.
*   \li The transformation is the root of the hierarchy and has more than one child.
*   \li The transformation has rotation animation and one of its children has a translation.
*
*   In all other cases the transformation is removed from the traverse list.
*   
*   The RwExpOptimizeHierarchy is enabled only if the DFF_OPTIMIZEHIERARCHY option is on.
*
*/

class RWEXP_API RwExpOptimizeHierarchy : public RwExpTraverseAction  
{
public:
    RwExpOptimizeHierarchy();
    virtual ~RwExpOptimizeHierarchy();
    
    // RwCommObject method
    virtual RwCommType  GetType() const { return RwExpOptimizeHierarchyType; }

    // RwExpTraverseAction methods
    virtual bool        Init(const RwCommExportOptions *pExportOptions);
    virtual RwCommError Apply(RwVector<RwExpTraverseObject> &vTraverseList, const RwCommExportOptions *pExportOptions);

    
    RwCommTransform     *GetNewRootObject();
    bool                HasRotationAnimation(RwCommTransform *pTransform);
    bool                HasTranslationAnimation(RwCommTransform *pTransform);

protected:

    /** The optimization criteria.
    *
    *   \param vTraverseList the traverse list.
    *   \param index an index of the object being tested within the traverse list.
    *   \param pExportOptions a pointer to the export options structure.
    *
    *   \returns true if the object can be removed, false otherwise.
    */
    virtual bool        CanOptimizeObject(RwVector<RwExpTraverseObject> &vTraverseList, int index,
                                            const RwCommExportOptions *pExportOptions);

protected:
    RwSet<RwCommTransform *>    hierarchyBones;
    RwCommTransform                 *m_pRootTransform;

};

#endif // !defined(AFX_RWEXPOPTIMIZEHIERARCHY_H__45BD110D_F6F7_4D49_A88F_D512C8763B83__INCLUDED_)
