#ifndef RW_MAYA_UTILS_H
#define RW_MAYA_UTILS_H

RWMAYA_API RwCommMatrix         RwMayaMatrixToCommMatrix(const MMatrix & m);
RWMAYA_API RwCommVec3           RwMayaPointToCommVec3(const MPoint & p);
RWMAYA_API RwCommVec3           RwMayaVectorToCommVec3(const MVector & v);
RWMAYA_API RwCommVec3           RwMayaColorToCommVec3(const MColor & c);
RWMAYA_API RwCommBoundingBox    RwMayaBoundingBoxToRwCommBoundingBox(MBoundingBox mayaBBox);
RWMAYA_API RwWString             RwMayaGetShortFileName(const RwWString &filename);

RWMAYA_API bool     RwMayaGetConnectedSetsAndMembers(MDagPath shapeDagPath, MObjectArray &sets,
                                                     MObjectArray &comps);
RWMAYA_API void     RwMayaAddToInitialShadingGroup(MObjectArray &sets, MObjectArray &comps);
RWMAYA_API void     RwMayaGetConnectedShadersAndSets(MDagPath shapeDagPath, MObjectArray &shaders,
                                                     MObjectArray &usedSets);

RWMAYA_API bool     RwMayaAreShapesEqual(MDagPath path0, MDagPath path1);
RWMAYA_API RwWString RwMayaGetObjectName(MObject object);
RWMAYA_API bool     RwMayaIsNodeVisible(MDagPath dagPath);
RWMAYA_API bool     RwMayaIsNodeAndParentsVisible(MDagPath dagPath);
RWMAYA_API bool     RwMayaIsShapeIntermediate(MDagPath dagPath);
RWMAYA_API bool     RwMayaIsNodeLayerVisible(MDagPath dagPath);


typedef bool        (*RwMayaMayaObjectCallBack)(MObject object, void *pData);
typedef bool        (*RwMayaMayaDagPathCallBack)(MDagPath dagPath, void *pData);

RWMAYA_API void     RwMayaForAllNodesInScene(MFn::Type fnType, bool bAllowCompatible,
                                             RwMayaMayaObjectCallBack callBack, void *pData);
RWMAYA_API bool     RwMayaForAllNodesInHierarchy(MDagPath &dagPath,
                                             RwMayaMayaDagPathCallBack callBack, void *pData);
RWMAYA_API bool     RwMayaBlendShapeListCallBack(MObject object, void *pData);
RWMAYA_API bool     RwMayaGeometryFilterCallBack(MObject object, void *pData);
RWMAYA_API bool     RwMayaGetSelectionRoot(MDagPath &dagPath);
RWMAYA_API MDagPath RwMayaGetHierarchyRoot(const MDagPath dagPath);

RWMAYA_API bool     RwMayaSkinListCallBack(MObject object, void *pData);

RWMAYA_API bool     RwMayaDoesMelAttributeExist(RwWString object, RwWString attribute);
RWMAYA_API RwWString RwMayaGetMelQueryAsString(RwWString query, RwWString object, RwWString field,
                                              bool checkExists);
RWMAYA_API void     RwMayaGetMelQueryAsStringArray(RwWString query, RwWString object,RwWString field,
                                                   RwVector<RwWString> &stringArray, bool checkExists);
RWMAYA_API RwWString RwMayaQueryMelAttributeAlias(RwWString name);

RWMAYA_API bool     RwMayaGetObjectAttributeAsInt(MObject object, RwWString attribute,
                                                     int index, int &result);
RWMAYA_API bool     RwMayaGetObjectAttributeAsFloat(MObject object, RwWString attribute,
                                                     int index, float &result);
RWMAYA_API bool     RwMayaGetObjectAttributeAsDouble(MObject object, RwWString attribute,
                                                     int index, double &result);
RWMAYA_API bool     RwMayaGetObjectAttributeAsString(MObject object, RwWString attribute,
                                                     int index, RwWString &result);

RWMAYA_API bool     RwMayaGetDagPathAttributeAsInt(MDagPath dagPath, RwWString tagName, int *val);
RWMAYA_API bool     RwMayaGetDagPathAttributeAsBool(MDagPath dagPath, RwWString tagName, bool *val);

RWMAYA_API MObject  RwMayaGetObjectConnectionAsObject(MObject object, RwWString attribute, int index);
RWMAYA_API bool     RwMayaAreObjectAttributesConnected(MObject object0, RwWString attribute0, int index0,
                                    MObject object1, RwWString attribute1, int index1);
RWMAYA_API MPlug    RwMayaGetPlugChild(MPlug &plug, RwWString childPlugName);

RWMAYA_API bool     RwMayaGetNodeConnectionAsObject(MFnDependencyNode &node,
                                                    RwWString attributeName, MObject &value);
RWMAYA_API bool     RwMayaGetNodeConnectionAsObjectArray(MFnDependencyNode &node, RwWString attributeName,
                                       RwVector<MObject> &array);
RWMAYA_API bool     RwMayaGetNodeArrayConnectionAsObjectArray(MFnDependencyNode &node, RwWString attributeName,
                                       RwVector<MObject> &array, int size);
RWMAYA_API bool     RwMayaIsAttributeConnected(const MFnDependencyNode &depNode, const RwWString &attribute);

RWMAYA_API bool     RwMayaSetObjectAttributeAsInt(MObject object, RwWString attribute, int index,
                                                  const int value);
RWMAYA_API bool     RwMayaSetObjectAttributeAsString(MObject object, RwWString attribute, int index,
                                                     const RwWString &value);

class RWMAYA_API RwMayaSkinCluster
{
public:
    MObject             cluster;
    MDagPath            skin;
    RwVector<MDagPath>  joints;
};


template <class T> bool RwMayaGetNodeAttribute(MFnDependencyNode &node, RwWString attributeName, T &value)
{
    MPlug   plug;

    plug = node.findPlug(RwUtilW2A(attributeName).c_str());
    if (plug.getValue(value) != MStatus::kSuccess)
    {
        return false;
    }

    return true;
}

template <class T> bool RwMayaGetNodeAttributeArray(MFnDependencyNode &node, RwWString attributeName,
                                              RwVector<T> &array, int size, T defaultValue)
{
    MPlug   plug, elemPlug;
    MObject arrayObject;
    MStatus status;
    T       value;

    // Clear the return array
    array.clear();

    // Populate the return array to the requested size
    for (int i = 0; i < size; i++)
    {
        array.push_back(defaultValue);
    }

    // Get the plug
    plug = node.findPlug(RwUtilW2A(attributeName).c_str());

    if (plug.isNull() || !plug.isArray())
    {
        return false;
    }

    // Iterate through the plug array
    for (unsigned int j = 0; j < plug.numElements(); j++)
    {
        // Get this element
        elemPlug = plug.elementByPhysicalIndex(j);
        if (elemPlug.isNull())
        {
            return false;
        }

        // Check it's in range
        if (elemPlug.logicalIndex() < static_cast<unsigned int>(size))
        {
            // Store it's value
            status = elemPlug.getValue (value);
            if (status == MStatus::kFailure)
            {
                return false;
            }

            array[elemPlug.logicalIndex()] = value;
        }

    }

    return true;
}

#endif //RW_MAYA_UTILS_H