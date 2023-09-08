#include <iostream.h>
#include <maya/MPxNode.h>
#include <maya/MString.h>
#include <maya/MTypeId.h>
#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h> 
#include <maya/MArrayDataHandle.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnLightDataAttribute.h>
#include <maya/MFnTypedAttribute.h> 
#include <maya/MFnStringData.h>
#include <maya/MFloatVector.h>
#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>

// CHECK_MSTATUS macro doesn't exist in Maya 4
#ifdef MAYA_40
#define CHECK_MSTATUS(x) (x)
#endif

class lambert : public MPxNode
{
public:
            lambert();
    virtual ~lambert();

    static  void *creator();
    virtual MStatus compute(const MPlug &plug, MDataBlock &block);
    static  MStatus initialize();

    virtual void postConstructor(); 

    static MTypeId id;

protected:
    // Red component of surface color
    static MObject aColorR;

    // Green component of surface color
    static MObject aColorG;

    // Blue component of surface color
    static MObject aColorB;

    // Surface color
    static MObject aColor;

    // Red component of incandescence
    static MObject aIncandescenceR;

    // Green component of incandescence
    static MObject aIncandescenceG;

    // Blue component of incandescence
    static MObject aIncandescenceB;

    // Incandescence
    static MObject aIncandescence;

    // Red component of output color
    static MObject aOutColorR;

    // Green component of output color
    static MObject aOutColorG;

    // Blue component of output color
    static MObject aOutColorB;

    // Output color
    static MObject aOutColor;

    // X component of surface normal
    static MObject aNormalCameraX;

    // Y component of surface normal
    static MObject aNormalCameraY;

    // Z component of surface normal
    static MObject aNormalCameraZ;

   // Surface normal
    static MObject aNormalCamera;

    // X component of light direction vector
    static MObject aLightDirectionX;

    // Y component of light direction vector
    static MObject aLightDirectionY;

    // Z component of light direction vector
    static MObject aLightDirectionZ;

    // Light direction vector
    static MObject aLightDirection;

    // Red component of light intensity
    static MObject aLightIntensityR;

    // Green component of light intensity
    static MObject aLightIntensityG;

    // Blue component of light intensity
    static MObject aLightIntensityB;

    // Light Intensity vector
    static MObject aLightIntensity;

    // Ambient flag
    static MObject aLightAmbient;

    // Diffuse flag
    static MObject aLightDiffuse;

    // Specular flag
    static MObject aLightSpecular;

    // Shadow Fraction flag
    static MObject aLightShadowFraction;

    // Pre Shadow Intensity
    static MObject aPreShadowIntensity;

    // Light blind data
    static MObject aLightBlindData;

    // Light data array
    static MObject aLightData;
};

// Node ID
MTypeId lambert::id(0x57331);

// Attributes
MObject lambert::aColor;
MObject lambert::aColorR;
MObject lambert::aColorG;
MObject lambert::aColorB;
MObject lambert::aIncandescence;
MObject lambert::aIncandescenceR;
MObject lambert::aIncandescenceG;
MObject lambert::aIncandescenceB;
MObject lambert::aOutColor;
MObject lambert::aOutColorR;
MObject lambert::aOutColorG;
MObject lambert::aOutColorB;

MObject lambert::aNormalCamera;
MObject lambert::aNormalCameraX;
MObject lambert::aNormalCameraY;
MObject lambert::aNormalCameraZ;
MObject lambert::aLightData;
MObject lambert::aLightDirection;
MObject lambert::aLightDirectionX;
MObject lambert::aLightDirectionY;
MObject lambert::aLightDirectionZ;
MObject lambert::aLightIntensity; 
MObject lambert::aLightIntensityR;
MObject lambert::aLightIntensityG;
MObject lambert::aLightIntensityB;

MObject lambert::aLightAmbient;
MObject lambert::aLightDiffuse;
MObject lambert::aLightSpecular;
MObject lambert::aLightShadowFraction;
MObject lambert::aPreShadowIntensity;
MObject lambert::aLightBlindData;

lambert::lambert()
{

}

lambert::~lambert()
{

}

void
lambert::postConstructor()
{
    setMPSafe(true);
}

void *
lambert::creator()
{
    return new lambert();
}

MStatus
lambert::initialize()
{
    MFnNumericAttribute nAttr; 
    MFnLightDataAttribute lAttr;
    MFnTypedAttribute tAttr; 
    //MFnStringData sAttr;

    MStatus status;

    // Attribute Initialization:
    aColorR = nAttr.create("colorR", "cr", MFnNumericData::kFloat, 0, &status);
    CHECK_MSTATUS(status);
    CHECK_MSTATUS(nAttr.setKeyable(true));
    CHECK_MSTATUS(nAttr.setStorable(true));
    CHECK_MSTATUS(nAttr.setDefault(1.0f));

    aColorG = nAttr.create("colorG", "cg", MFnNumericData::kFloat, 0, &status);
    CHECK_MSTATUS(status);
    CHECK_MSTATUS(nAttr.setKeyable(true));
    CHECK_MSTATUS(nAttr.setStorable(true));
    CHECK_MSTATUS(nAttr.setDefault(1.0f));

    aColorB = nAttr.create("colorB", "cb", MFnNumericData::kFloat, 0, &status);
    CHECK_MSTATUS(status);
    CHECK_MSTATUS(nAttr.setKeyable(true));
    CHECK_MSTATUS(nAttr.setStorable(true));
    CHECK_MSTATUS(nAttr.setDefault(1.0f));

    aColor = nAttr.create("color", "c", aColorR, aColorG, aColorB, &status);
    CHECK_MSTATUS(status);
    CHECK_MSTATUS(nAttr.setKeyable(true));
    CHECK_MSTATUS(nAttr.setStorable(true));
    CHECK_MSTATUS(nAttr.setDefault(1.0f, 1.0f, 1.0f));
    CHECK_MSTATUS(nAttr.setUsedAsColor(true));

    aIncandescenceR = nAttr.create("incandescenceR", "ir",
                                   MFnNumericData::kFloat, 0, &status);
    CHECK_MSTATUS(status);
    CHECK_MSTATUS(nAttr.setKeyable(true));
    CHECK_MSTATUS(nAttr.setStorable(true));
    CHECK_MSTATUS(nAttr.setDefault(0.0f));

    aIncandescenceG = nAttr.create("incandescenceG", "ig",
                                   MFnNumericData::kFloat, 0, &status);
    CHECK_MSTATUS(status);
    CHECK_MSTATUS(nAttr.setKeyable(true));
    CHECK_MSTATUS(nAttr.setStorable(true));
    CHECK_MSTATUS(nAttr.setDefault(0.0f));

    aIncandescenceB = nAttr.create("incandescenceB", "ib",
                                   MFnNumericData::kFloat, 0, &status);
    CHECK_MSTATUS(status);
    CHECK_MSTATUS(nAttr.setKeyable(true));
    CHECK_MSTATUS(nAttr.setStorable(true));
    CHECK_MSTATUS(nAttr.setDefault(0.0f));

    aIncandescence = nAttr.create("incandescence", "ic", aIncandescenceR,
                                  aIncandescenceG, aIncandescenceB, &status);
    CHECK_MSTATUS(status);
    CHECK_MSTATUS(nAttr.setKeyable(true));
    CHECK_MSTATUS(nAttr.setStorable(true));
    CHECK_MSTATUS(nAttr.setDefault(0.0f, 0.0f, 0.0f));
    CHECK_MSTATUS(nAttr.setUsedAsColor(true));

    // Output Attributes

    // Color Output
    aOutColorR = nAttr.create("outColorR", "ocr", MFnNumericData::kFloat, 0, &status);
    CHECK_MSTATUS(status);
    
    aOutColorG = nAttr.create("outColorG", "ocg", MFnNumericData::kFloat, 0, &status);
    CHECK_MSTATUS(status);
    
    aOutColorB = nAttr.create("outColorB", "ocb", MFnNumericData::kFloat, 0, &status);
    CHECK_MSTATUS(status);
    
    aOutColor = nAttr.create("outColor", "oc", aOutColorR, aOutColorG,
                             aOutColorB, &status);
    CHECK_MSTATUS(status);
    
    CHECK_MSTATUS(nAttr.setHidden(false));
    CHECK_MSTATUS(nAttr.setReadable(true));
    CHECK_MSTATUS(nAttr.setWritable(false));

    // Camera Normals
    aNormalCameraX = nAttr.create("normalCameraX", "nx",
                                  MFnNumericData::kFloat, 0, &status);
    CHECK_MSTATUS(status);
    CHECK_MSTATUS(nAttr.setStorable(false));
    CHECK_MSTATUS(nAttr.setDefault(1.0f));

    aNormalCameraY = nAttr.create("normalCameraY", "ny",
                                  MFnNumericData::kFloat, 0, &status);
    CHECK_MSTATUS(status);
    CHECK_MSTATUS(nAttr.setStorable(false));
    CHECK_MSTATUS(nAttr.setDefault(1.0f));

    aNormalCameraZ = nAttr.create("normalCameraZ", "nz",
                                  MFnNumericData::kFloat, 0, &status);
    CHECK_MSTATUS(status);
    CHECK_MSTATUS(nAttr.setStorable(false));
    CHECK_MSTATUS(nAttr.setDefault(1.0f));

    aNormalCamera = nAttr.create("normalCamera","n", aNormalCameraX,
                                 aNormalCameraY, aNormalCameraZ, &status);
    CHECK_MSTATUS(status);
    CHECK_MSTATUS(nAttr.setStorable(false));
    CHECK_MSTATUS(nAttr.setDefault(1.0f, 1.0f, 1.0f));
    CHECK_MSTATUS(nAttr.setHidden(true));

    // Light Direction
    aLightDirectionX = nAttr.create("lightDirectionX", "ldx",
                                    MFnNumericData::kFloat, 0, &status);
    CHECK_MSTATUS(status);
    CHECK_MSTATUS(nAttr.setStorable(false));
    CHECK_MSTATUS(nAttr.setHidden(true));
    CHECK_MSTATUS(nAttr.setReadable(false));
    CHECK_MSTATUS(nAttr.setDefault(1.0f));

    aLightDirectionY = nAttr.create("lightDirectionY", "ldy",
                                    MFnNumericData::kFloat, 0, &status);
    CHECK_MSTATUS(status);
    CHECK_MSTATUS(nAttr.setStorable(false));
    CHECK_MSTATUS(nAttr.setHidden(true));
    CHECK_MSTATUS(nAttr.setReadable(false));
    CHECK_MSTATUS(nAttr.setDefault(1.0f));

    aLightDirectionZ = nAttr.create("lightDirectionZ", "ldz",
            MFnNumericData::kFloat, 0, &status);
    CHECK_MSTATUS(status);
    CHECK_MSTATUS(nAttr.setStorable(false));
    CHECK_MSTATUS(nAttr.setHidden(true));
    CHECK_MSTATUS(nAttr.setReadable(false));
    CHECK_MSTATUS(nAttr.setDefault(1.0f));

    aLightDirection = nAttr.create("lightDirection", "ld",
                                   aLightDirectionX, aLightDirectionY,
                                   aLightDirectionZ, &status);
    CHECK_MSTATUS(status);
    CHECK_MSTATUS(nAttr.setStorable(false));
    CHECK_MSTATUS(nAttr.setHidden(true));
    CHECK_MSTATUS(nAttr.setReadable(false));
    CHECK_MSTATUS(nAttr.setDefault(1.0f, 1.0f, 1.0f));

    // Light Intensity
    aLightIntensityR = nAttr.create("lightIntensityR", "lir",
                                    MFnNumericData::kFloat, 0, &status);
    CHECK_MSTATUS(status);
    CHECK_MSTATUS(nAttr.setStorable(false));
    CHECK_MSTATUS(nAttr.setHidden(true));
    CHECK_MSTATUS(nAttr.setReadable(false));
    CHECK_MSTATUS(nAttr.setDefault(1.0f));

    aLightIntensityG = nAttr.create("lightIntensityG", "lig",
                                    MFnNumericData::kFloat, 0, &status);
    CHECK_MSTATUS(status);
    CHECK_MSTATUS(nAttr.setStorable(false));
    CHECK_MSTATUS(nAttr.setHidden(true));
    CHECK_MSTATUS(nAttr.setReadable(false));
    CHECK_MSTATUS(nAttr.setDefault(1.0f));

    aLightIntensityB = nAttr.create("lightIntensityB", "lib",
                                    MFnNumericData::kFloat, 0, &status);
    CHECK_MSTATUS(status);
    CHECK_MSTATUS(nAttr.setStorable(false));
    CHECK_MSTATUS(nAttr.setHidden(true));
    CHECK_MSTATUS(nAttr.setReadable(false));
    CHECK_MSTATUS(nAttr.setDefault(1.0f));

    aLightIntensity = nAttr.create("lightIntensity", "li", 
                                   aLightIntensityR, aLightIntensityG,
                                   aLightIntensityB, &status);
    CHECK_MSTATUS(status);
    CHECK_MSTATUS(nAttr.setStorable(false));
    CHECK_MSTATUS(nAttr.setHidden(true));
    CHECK_MSTATUS(nAttr.setReadable(false));
    CHECK_MSTATUS(nAttr.setDefault(1.0f, 1.0f, 1.0f));
   
    // Light
    aLightAmbient = nAttr.create("lightAmbient", "la",
                                 MFnNumericData::kBoolean, 0, &status);
    CHECK_MSTATUS(status);
    CHECK_MSTATUS(nAttr.setStorable(false));
    CHECK_MSTATUS(nAttr.setHidden(true));
    CHECK_MSTATUS(nAttr.setReadable(false));
    CHECK_MSTATUS(nAttr.setDefault(true));

    aLightDiffuse = nAttr.create("lightDiffuse", "ldf",
                                 MFnNumericData::kBoolean, 0, &status);
    CHECK_MSTATUS(status);
    CHECK_MSTATUS(nAttr.setStorable(false));
    CHECK_MSTATUS(nAttr.setHidden(true));
    CHECK_MSTATUS(nAttr.setReadable(false));
    CHECK_MSTATUS(nAttr.setDefault(true));

    aLightSpecular = nAttr.create("lightSpecular", "ls",
                                  MFnNumericData::kBoolean, 0, &status);
    CHECK_MSTATUS(status);
    CHECK_MSTATUS(nAttr.setStorable(false));
    CHECK_MSTATUS(nAttr.setHidden(true));
    CHECK_MSTATUS(nAttr.setReadable(false));
    CHECK_MSTATUS(nAttr.setDefault(false));

    aLightShadowFraction = nAttr.create("lightShadowFraction", "lsf",
                                        MFnNumericData::kFloat, 0, &status);
    CHECK_MSTATUS(status);
    CHECK_MSTATUS(nAttr.setStorable(false));
    CHECK_MSTATUS(nAttr.setReadable(true));
    CHECK_MSTATUS(nAttr.setWritable(true));
    CHECK_MSTATUS(nAttr.setDefault(1.0f));

    aPreShadowIntensity = nAttr.create("preShadowIntensity", "psi",
                                       MFnNumericData::kFloat, 0, &status);
    CHECK_MSTATUS(status);
    CHECK_MSTATUS(nAttr.setStorable(false));
    CHECK_MSTATUS(nAttr.setHidden(true));
    CHECK_MSTATUS(nAttr.setReadable(false));
    CHECK_MSTATUS(nAttr.setDefault(1.0f));

    aLightBlindData = nAttr.create("lightBlindData", "lbld",
                                   MFnNumericData::kLong, 0, &status);
    CHECK_MSTATUS(status);
    CHECK_MSTATUS(nAttr.setStorable(false));
    CHECK_MSTATUS(nAttr.setHidden(true));
    CHECK_MSTATUS(nAttr.setReadable(false));
    CHECK_MSTATUS(nAttr.setDefault(0));

    aLightData = lAttr.create("lightDataArray", "ltd", aLightDirection,
                              aLightIntensity, aLightAmbient, aLightDiffuse,
                              aLightSpecular, aLightShadowFraction,
                              aPreShadowIntensity, aLightBlindData,
                              &status);
    CHECK_MSTATUS(status);
    CHECK_MSTATUS(lAttr.setArray(true));
    CHECK_MSTATUS(lAttr.setStorable(false));
    CHECK_MSTATUS(lAttr.setHidden(true));
    CHECK_MSTATUS(lAttr.setDefault(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                                   true, true, false, 1.0f, 1.0f, 0));

    // Next we will add the attributes we have defined to the node
    CHECK_MSTATUS(addAttribute(aColor));
    CHECK_MSTATUS(addAttribute(aIncandescence));
    CHECK_MSTATUS(addAttribute(aOutColor));
    CHECK_MSTATUS(addAttribute(aNormalCamera));
    CHECK_MSTATUS(addAttribute(aLightDirection));
    CHECK_MSTATUS(addAttribute(aLightIntensity));
    CHECK_MSTATUS(addAttribute(aLightAmbient));
    CHECK_MSTATUS(addAttribute(aLightDiffuse));
    CHECK_MSTATUS(addAttribute(aLightSpecular));
    CHECK_MSTATUS(addAttribute(aLightShadowFraction));
    CHECK_MSTATUS(addAttribute(aPreShadowIntensity));
    CHECK_MSTATUS(addAttribute(aLightBlindData));
    CHECK_MSTATUS(addAttribute(aLightData));

    // Dependencies
    CHECK_MSTATUS(attributeAffects(aColorR, aOutColor));
    CHECK_MSTATUS(attributeAffects(aColorG, aOutColor));
    CHECK_MSTATUS(attributeAffects(aColorB, aOutColor));
    CHECK_MSTATUS(attributeAffects(aColor, aOutColor));

    CHECK_MSTATUS(attributeAffects(aIncandescenceR, aOutColor));
    CHECK_MSTATUS(attributeAffects(aIncandescenceG, aOutColor));
    CHECK_MSTATUS(attributeAffects(aIncandescenceB, aOutColor));
    CHECK_MSTATUS(attributeAffects(aIncandescence, aOutColor));

    CHECK_MSTATUS(attributeAffects(aLightIntensityR, aOutColor));
    CHECK_MSTATUS(attributeAffects(aLightIntensityB, aOutColor));
    CHECK_MSTATUS(attributeAffects(aLightIntensityG, aOutColor));
    CHECK_MSTATUS(attributeAffects(aLightIntensity, aOutColor));
    CHECK_MSTATUS(attributeAffects(aNormalCameraX, aOutColor));
    CHECK_MSTATUS(attributeAffects(aNormalCameraY, aOutColor));
    CHECK_MSTATUS(attributeAffects(aNormalCameraZ, aOutColor));
    CHECK_MSTATUS(attributeAffects(aNormalCamera, aOutColor));
    CHECK_MSTATUS(attributeAffects(aLightDirectionX, aOutColor));
    CHECK_MSTATUS(attributeAffects(aLightDirectionY, aOutColor));
    CHECK_MSTATUS(attributeAffects(aLightDirectionZ, aOutColor));
    CHECK_MSTATUS(attributeAffects(aLightDirection, aOutColor));
    CHECK_MSTATUS(attributeAffects(aLightAmbient, aOutColor));
    CHECK_MSTATUS(attributeAffects(aLightSpecular, aOutColor));
    CHECK_MSTATUS(attributeAffects(aLightDiffuse, aOutColor));
    CHECK_MSTATUS(attributeAffects(aLightShadowFraction, aOutColor));
    CHECK_MSTATUS(attributeAffects(aPreShadowIntensity, aOutColor));
    CHECK_MSTATUS(attributeAffects(aLightBlindData, aOutColor));
    CHECK_MSTATUS(attributeAffects(aLightData, aOutColor));

    return (MS::kSuccess);
}

MStatus
lambert::compute(const MPlug &plug, MDataBlock &block) 
{ 
    // The plug parameter will allow us to determine which output attribute
    // needs to be calculated.
    //
    if (plug == aOutColor   ||
        plug == aOutColorR ||
        plug == aOutColorG ||
        plug == aOutColorB)
    {
        MStatus status;
        MFloatVector resultColor(0.0, 0.0, 0.0); 

    
        // Get surface shading parameters from input block
        MFloatVector &surfaceNormal = block.inputValue(aNormalCamera,
                                      &status).asFloatVector();
        CHECK_MSTATUS(status);

        MFloatVector &surfaceColor =  block.inputValue(aColor,
                                      &status).asFloatVector();
        CHECK_MSTATUS(status);

        MFloatVector &incandescence = block.inputValue(aIncandescence,
                                      &status).asFloatVector();
        CHECK_MSTATUS(status);

        // Get light list
        MArrayDataHandle lightData = block.inputArrayValue(aLightData, &status);
        CHECK_MSTATUS(status);

        int numLights = lightData.elementCount(&status);
        CHECK_MSTATUS(status);

        // Calculate the effect of the lights in the scene on the color

        // Iterate through light list and get ambient/diffuse values
        for (int count = 1; count <= numLights; count++)
        {
            // Get the current light out of the array
            //
            MDataHandle currentLight = lightData.inputValue(&status); 
            CHECK_MSTATUS(status);


            // Get the intensity of that light
            //
            MFloatVector &lightIntensity = currentLight.child(
                                           aLightIntensity).asFloatVector();

            // Find ambient component
            if (currentLight.child(aLightAmbient).asBool())
            {
                resultColor += lightIntensity;
            }

            // Find diffuse component
            if (currentLight.child(aLightDiffuse).asBool())
            {
                MFloatVector &lightDirection = currentLight.child(
                                               aLightDirection).asFloatVector();

                float cosln = lightDirection * surfaceNormal;
   
                if (cosln > 0.0f)
                {
                    resultColor += lightIntensity * cosln;
                }
            }

            // Advance to the next light.
            //
            if (count < numLights)
            {
                status = lightData.next();
                CHECK_MSTATUS(status);
            }
        }

        // Surface color and incandescence, the RenderWare way
        resultColor[0] = resultColor[0]*surfaceColor[0] + incandescence[0];
        resultColor[1] = resultColor[1]*surfaceColor[1] + incandescence[1];
        resultColor[2] = resultColor[2]*surfaceColor[2] + incandescence[2];

        // Set output color attribute
        if (plug == aOutColor  ||
            plug == aOutColorR ||
            plug == aOutColorG ||
            plug == aOutColorB)
        {
            // Get the handle to the attribute
            MDataHandle outColorHandle = block.outputValue(aOutColor, &status);
            CHECK_MSTATUS(status);
            MFloatVector &outColor = outColorHandle.asFloatVector();
        
            outColor = resultColor;     // Set the output value
            outColorHandle.setClean();  // Mark the output value as clean
        }
    } 
    else
    {
        return (MS::kUnknownParameter); // We got an unexpected plug
    }

    return (MS::kSuccess);
}


__declspec(dllexport) MStatus
initializePlugin(MObject obj)
{ 
    const MString UserClassify("shader/surface");

    MString command("if(`window -exists createRenderNodeWindow`) \
                    {refreshCreateRenderNodeWindow(\"");

    MFnPlugin plugin(obj, "RenderWare Exporters - Example", "4.5", "Any");

    CHECK_MSTATUS(plugin.registerNode("MayaShader", lambert::id, 
                  lambert::creator, lambert::initialize,
                  MPxNode::kDependNode, &UserClassify));

    command += UserClassify;
    command += "\");}\n";

    CHECK_MSTATUS(MGlobal::executeCommand(command));

    return (MS::kSuccess);
}

__declspec(dllexport) MStatus
uninitializePlugin(MObject obj)
{
    const MString UserClassify("shader/surface");

    MString command("if(`window -exists createRenderNodeWindow`) \
                    {refreshCreateRenderNodeWindow(\"");

    MFnPlugin plugin(obj);

    CHECK_MSTATUS(plugin.deregisterNode(lambert::id));

    command += UserClassify;
    command += "\");}\n";

    CHECK_MSTATUS(MGlobal::executeCommand(command));

    return MS::kSuccess;
}
