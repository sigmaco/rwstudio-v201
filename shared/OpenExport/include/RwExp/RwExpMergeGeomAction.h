// RwExpMergeGeomAction.h: interface for the RwExpMergeGeomAction class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWEXPMERGEGEOMACTION_H__F70974B4_D5C5_42BE_A72C_4C988F4591CC__INCLUDED_)
#define AFX_RWEXPMERGEGEOMACTION_H__F70974B4_D5C5_42BE_A72C_4C988F4591CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwExp.h"
#include "RwExpTraverseAction.h"

class RwCommGeometry;

struct RwExpMergeGroup
{
    RwCommGeometry                  *pLeader;
    RwVector<RwCommGeometry *>  geoms;
};

const RwCommType RwExpMergeGeomActionType("RwExpMergeGeomAction");

/**
*
*  \ingroup traverseactions
*
*   This class traverses all RwCommGeometry objects in the traverse list, and attempts to merge all geometries
*   belonging to the same merge group. The merge group is tagged on the geometry's RwCommTransform object, and
*   can be extracted using the RwCommTransform::GetAttribute() method, using the RWEXP_MERGEGROUP attribute name.
*   Merged RwCommGeometry objects are arranged in merge groups.
*   Each merge group is identified by an index value,
*   corresponding to the original RWEXP_MERGEGROUP attribute value. In addition, each merge group has a group leader,
*   which is the target merged geometry, containing all the merged geometries.
*   
*   The RwExpMergeGeomAction will merge two RwCommGeometry objects, tagged with the same RWEXP_MERGEGROUP attribute,
*   if the merge criteria is satisfied. The merge criteria is specified by the virtual method 
*   RwExpMergeGeomAction::CanMerge(). Users deriving from the RwExpMergeGeomAction class, can introduce their own
*   merge criteria by overriding this method.
*   
*   \see RwCommGeometry
*/

class RWEXP_API RwExpMergeGeomAction : public RwExpTraverseAction 
{
public:
    RwExpMergeGeomAction();
    virtual ~RwExpMergeGeomAction();

    //RwCommObject method
    virtual RwCommType GetType() const { return RwExpMergeGeomActionType; }

    //RwExpTraverseAction methods

    // Apply the action
    virtual RwCommError Apply(RwVector<RwExpTraverseObject> &vTraverseList, const RwCommExportOptions *pExportOptions);

    virtual RwCommError CleanUp();

    /** Get the merge group number of a given geometry.
    *   \param pGeometry a pointer to the geometry.
    *   \param nMergeGroup a reference to the group number.
    *
    *   \returns RWCOMM_SUCCESS if the geometry belongs to a merge group , RWCOMM_FAIL oterwise.
    */
    RwCommError GetMergeGroup(RwCommGeometry *pGeometry, int &nMergeGroup) const;

    /** Get all the members belonging to a merge group.
    *   \param nMergeGroup the merge group number.
    *   \param vMembers a vector that will hold all the members.
    *
    *   \returns RWCOMM_SUCCESS if the merge group exists, RWCOMM_FAIL oterwise.
    */
    RwCommError GetGroupMembers(int nMergeGroup, RwVector<RwCommGeometry *> &vMembers) const;

    /** Get the leader of a merge group.
    *   \param nMergeGroup the merge group number.
    *   \param pGeometry a reference to a pointer that will hold the leader.
    *
    *   \returns RWCOMM_SUCCESS if the merge group exists, RWCOMM_FAIL oterwise.
    */
    RwCommError GetGroupLeader(int nMergeGroup, RwCommGeometry *&pGeometry) const;

protected:

    /** The merge criteria. This method should return true if the given two geometries can be merged.
    *   \param pGeom1 a pointer to the first geometry.
    *   \param pGeom2 a pointer to the second geometry..
    *
    *   \returns true if the criteria is satisfied, false otherwise.
    */
    virtual bool CanMerge(RwCommGeometry *pGeom1, RwCommGeometry *pGeom2) const;

private:

    RwMap<RwCommGeometry *, int>    m_mGeomMap;
    RwMap<int, RwExpMergeGroup>     m_mMergeGroups;
};

#endif // !defined(AFX_RWEXPMERGEGEOMACTION_H__F70974B4_D5C5_42BE_A72C_4C988F4591CC__INCLUDED_)
