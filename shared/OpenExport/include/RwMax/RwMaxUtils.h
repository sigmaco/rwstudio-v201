#ifndef RWMAXUTILS_H
#define RWMAXUTILS_H

#include "stdafx.h"

#include "RwCommError.h"

#include "RwMax.h"
#include "RwCommPrimitives.h"

class RwCommUserData;
class RwCommUserDataCollection;

RWMAX_API RwCommReal            RwMaxTicksToFrames(RwCommReal ticks);
RWMAX_API RwCommVec3            RwMaxPoint3ToRwExpVec3(const Point3 &p);
RWMAX_API RwCommVec3            RwMaxColorToRwExpVec3(const Color &c);
RWMAX_API RwCommMatrix          RwMaxMat3ToRwExpMatrix(const Matrix3 &m);
RWMAX_API Matrix3               RwMaxRwCommMatrixToMatrix3(const RwCommMatrix &m);
RWMAX_API RwString              RwMaxGetFileVersion(const char *filename, WORD languageID );
RWMAX_API void                  RwMaxCenterDialogWithinOwner(HWND hwndDlg, HWND hWndInsertAfter = HWND_TOP);
RWMAX_API RwCommError           RwMaxGetUserData(ICustAttribContainer *pAttribs,
                                        RwCommUserDataCollection &userDataColl);
RWMAX_API void                  RwMaxRoundZeroValues(Matrix3 &m);

RWMAX_API INode                 *RwMaxFindNodeByName(INode *root, char *name);
RWMAX_API INode                 *RwMaxFindTopOfHierarchy(INode *node);

typedef bool                    (RwMaxFindFirstNodeInHierarchyCompareFn)(INode *);
RWMAX_API INode                 *RwMaxFindFirstNodeInHierarchy(INode *node, RwMaxFindFirstNodeInHierarchyCompareFn f);
RWMAX_API Modifier              *RwMaxFindModifier(INode *node, const Class_ID &type);
RWMAX_API BOOL                  RwMaxNodeHasAncestor(INode *node, INode *ancestor);
RwVector<RWMAX_API Modifier *>  RwMaxFindAllModifiers(INode *node, const Class_ID &type);

// Call the given function with each node in the heirarchy
// as an argument along a depth-first search.
template <class F> void
ForEachNodeInHierarchy(INode *node, F &f)
{
    f(node);

    for (int i = 0; i < node->NumChildren(); i++)
    {
        ForEachNodeInHierarchy<F>(node->GetChildNode(i), f);
    }
}

RWMAX_API int               RwMaxGetRenderSubMtlIndex(Mtl *pMtl);

struct RwMaxFxFileTexture
{
    RwString    sId;
    RwString    sSemantic;
    RwString    sUIName;
    int         iMapChannel;
};

RWMAX_API bool              RwMaxGetFxFileTextures(const RwWString &sFilename, RwVector<RwMaxFxFileTexture> &vTextures);

#endif