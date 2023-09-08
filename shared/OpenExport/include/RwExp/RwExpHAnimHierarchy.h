// RwExpHAnimHierarchy.h: interface for the RwExpHAnimHierarchy class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWEXPHANIMHIERARCHY_H__A4F22DC2_40FA_4CE2_9694_E275FA5793FA__INCLUDED_)
#define AFX_RWEXPHANIMHIERARCHY_H__A4F22DC2_40FA_4CE2_9694_E275FA5793FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwExp.h"
#include "RwExpTraverseAction.h"

#define AUTOGENERATED_TAGS_OFFSET   2000

class RwCommTransform;
class RwExpFrame;
class RwExpExportOptions;

struct RwExpHAnimEntry
{
    RwExpHAnimEntry() : id(-1), index(-1) {}
    RwExpHAnimEntry(int i1, int i2) : id(i1), index(i2) {}

    int id;
    int index;
};

const RwCommType RwExpHAnimHierarchyType("RwExpHAnimHierarchy");

/**
*
*  \ingroup traverseactions
*
*   This class builds an RpHAnimHierarchy structure from the traverse list. It traverses all RwCommTransform
*   objects in the traverse list and builds the animation hierarchy. For each RwCommTransform it maintains a
*   node ID and a node index.
*   Usually, these values are identical, unless the user has tagged his transformations, setting the node ID value
*   to the tagged value.
*
*   \see RpHAnimHierarchy
*   \see RwCommTransform
*/

class RWEXP_API RwExpHAnimHierarchy  : public RwExpTraverseAction 
{
public:
    RwExpHAnimHierarchy();
    virtual ~RwExpHAnimHierarchy();

    //RwCommObject method
    virtual RwCommType GetType() const { return RwExpHAnimHierarchyType; }

    // RwExpTraverseAction methods
    virtual RwCommError     Apply(RwVector<RwExpTraverseObject> &vTraverseList,
                                const RwCommExportOptions *pExportOptions);
    virtual RwCommError     CleanUp();

    /**
    *  Get the node ID value.
    *
    *  \param pTransform the transform to look up.
    *  \return the ID stored if successful, -1 if the transform was not found.
    */
    int GetNodeID(RwCommTransform *pTransform) const;

    /**
    *  Get the node index value, which is the index of the RwCommTransform in the animation hierarchy list.
    *
    *  \param pTransform the transform to look up.
    *  \return the index stored if successful, -1 if the transform was not found.
    */
    int GetNodeIndex(RwCommTransform *pTransform) const;

    /**
    *  Build an RpHAnimHierarchy
    *
    *  \return the RpHAnimHierarchy built if successful, null otherwise.
    */
    RpHAnimHierarchy *BuildHAnimHierarchy(RwCommExportOptions *pGlobalData);

    /**
    *  This functions gets a vector of all objects in the hierarchy, sorted by index.
    *  \return a vector of all RwCommTransform objects in the animation hiererachy.
    */
    const RwVector<RwCommTransform *> &GetNodesInHierarchy() const { return m_vHierarchyNodes; }

protected:
    void AddNode(RwCommTransform *pTransform, bool bRemap = false);

    /**
    *
    *  Create the RpHAnimHierarchy structure.
    */
    virtual RpHAnimHierarchy *CreateHAnimHierarchy(RwInt32 numNodes, RwUInt32 *nodeFlags,
                       RwInt32 *nodeIDs, RpHAnimHierarchyFlag flags, RwInt32 maxKeyFrameSize);

protected:

    int                             m_nodeTagId;
    int                             m_lastChildIndex;
    RwVector<RwUInt32>          m_hAnimNodeFlags;
    RwVector<RwInt32>           m_hAnimNodeTagIDs;
    RwVector<RwCommTransform *> m_vHierarchyNodes;
    bool                            m_bSkinned;
 
private:
    RwMap<RwCommTransform *, RwExpHAnimEntry> m_mIds;

};

#endif // !defined(AFX_RWEXPHANIMHIERARCHY_H__A4F22DC2_40FA_4CE2_9694_E275FA5793FA__INCLUDED_)
