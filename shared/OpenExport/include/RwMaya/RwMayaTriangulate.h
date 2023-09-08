#ifndef RW_MAYA_TRIANGULATE_H
#define RW_MAYA_TRIANGULATE_H

typedef RwPair<int, int>                        vertEdge;
typedef RwSet<vertEdge>                         meshEdges;
typedef RwSet<vertEdge>::iterator               meshEdgesIt;
typedef _STL::pair<RwSet<int>::iterator, bool>  intSetPair;

#if (!defined(DOXYGEN))

struct polyFlipEdge
{
    int         id;
    vertEdge    edge;
};

// =============================================================================
// --------------------------  RwTriangulate CLASS  ----------------------------
// =============================================================================

class RWMAYA_API RwTriangulate : public MPxCommand
{
public:
                            RwTriangulate()                 {};
    virtual                 ~RwTriangulate()                {}; 
                            
    static void             *creator();
    bool                    isUndoable() const;
    MStatus                 doIt(const MArgList&);
    MStatus                 redoIt();
    MStatus                 undoIt();

private:
    MSelectionList          selList;
    MGlobal::MSelectionMode selMode;
    MDagPath                meshDagPath;
    MObject                 meshComps;
    RwSet<int>              meshPolys;
    RwVector<meshEdges>     allFlipEdges;
                            
    bool                    BuildMeshQuadList();
    bool                    BuildMeshFlipEdgeList();
};

#endif // (!defined(DOXYGEN))

#endif // RW_MAYA_TRIANGULATE_H