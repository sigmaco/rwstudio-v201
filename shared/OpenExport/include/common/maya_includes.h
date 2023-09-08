#ifndef MAYA_INCLUDES_H
#define MAYA_INCLUDES_H

#define REQUIRE_IOSTREAM

#include <maya/MStatus.h>
#include <maya/MGlobal.h>
#include <maya/MTime.h>
#include <maya/MDistance.h>
#include <maya/MPoint.h>
#include <maya/MVector.h>

#include <maya/MPxNode.h>
#include <maya/MPxCommand.h>
#include <maya/MPxFileTranslator.h>
#include <maya/MCommandResult.h>

#include <maya/MMatrix.h>
#include <maya/MTransformationMatrix.h>

#include <maya/MAnimControl.h>
#include <maya/MAnimUtil.h>
#include <maya/MArgList.h>
#include <maya/MBoundingBox.h>
#include <maya/MObject.h>
#include <maya/MDagPath.h>
#include <maya/MPlug.h>
#include <maya/MSelectionList.h>
#include <maya/MMaterial.h>
#include <maya/MDrawData.h>
#include <maya/MPxSurfaceShape.h>
#include <maya/MPxSurfaceShapeUI.h>
#include <maya/MEventMessage.h>
#include <maya/MFileIO.h>

#include <maya/MFnDagNode.h>
#include <maya/MFnTransform.h>
#include <maya/MFnAnimCurve.h>
#include <maya/MFnMesh.h>
#include <maya/MFnLambertShader.h>
#include <maya/MFnIntArrayData.h>
#include <maya/MFnDoubleArrayData.h>
#include <maya/MFnIkHandle.h>
#include <maya/MFnBlendShapeDeformer.h>
#include <maya/MFnNurbsSurface.h>
#include <maya/MFnNurbsCurve.h>
#include <maya/MFnSet.h>
#include <maya/MFnSkinCluster.h>
#include <maya/MFnComponentListData.h>
#include <maya/MFnSingleIndexedComponent.h>
#include <maya/MFnDoubleIndexedComponent.h>
#include <maya/MFnLight.h>
#include <maya/MFnSpotLight.h>
#include <maya/MFnNonAmbientLight.h>
#include <maya/MFnCamera.h>
#include "maya/MFnMotionPath.h"

#include <maya/MFnNumericAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnLightDataAttribute.h> 

#include <maya/MFnStringArrayData.h>
#include <maya/MFnStringData.h>

#include <maya/MItDag.h>
#include <maya/MItDependencyNodes.h>
#include <maya/MItDependencyGraph.h>
#include <maya/MItGeometry.h>
#include <maya/MItMeshVertex.h>
#include <maya/MItMeshEdge.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MItSelectionList.h>

#include <maya/MDagPathArray.h>
#include <maya/MFloatArray.h>
#include <maya/MObjectArray.h>
#include <maya/MPointArray.h>
#include <maya/MPlugArray.h>
#include <maya/MStringArray.h>

#include <maya/ilib.h>

// Because of changes to the Maya MPlug API we need to use ints to query int
// plugs on Maya 4.0 and longs for previous versions. It's cleanest to use
// a define here to hide this.
#if !MAYA_30
#define MPLUG_COMPAT_INT int
#else
#define MPLUG_COMPAT_INT long
#endif

#endif // MAYA_INCLUDES_H