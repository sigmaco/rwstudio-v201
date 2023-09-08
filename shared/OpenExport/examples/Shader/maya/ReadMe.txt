========================================================================
       DYNAMIC LINK LIBRARY : MayaFurShader
========================================================================


========================================================================
       DYNAMIC LINK LIBRARY : MaxFurShader
========================================================================


This is an example of how to write your own custom shader, how to
export it, and how to stream it to and from an rf3 file. For clarity the
example simply supports a shader with a single color (the "fur" color) plus
a single texture (the "fur" texture).

The example is divided into two plugin dlls:
1. The common FurShader.dll, extending the RwExp, RwMI and RwRf3 libraries.
This dll is responsible for exporting the FurShader to RenderWare and 
streaming it to and from an rf3 file.

2. The specific maya plugin dll, extending the RwMaya library.
This dll is responsible for the creation of the FurShader in maya,
and for building it, using the RwMayaBuilder.

Compiling
---------
1. Compile the common plugin and copy it to your main OpenExportPlugins directory.
2. Compile the maya specific plugin (the dll will be built directly to
the maya plugin directory).
3.Copy the provided AEMayaShaderTemplate.mel script into the maya script directory.

Running
-------
Run maya, create the custom FurShader material and assign it 
to a geometry. You can now export it directly into RenderWare or
to an rf3 file.

About This Plugin
-----------------
MayaFurShaderPlugin.cpp is the entry point of the plugin, registering
the MayaFurShader creator (MayaFurShaderCreator). 

Classes:

lambert, deriving from the maya MPxNode class. This is simply an 
implementation of our own custom material, with a color slider
and a texture chooser.

MayaFurShader, deriving from the RwMayaMaterial class and the CCommFurShader. This
class adds the fur functionality to the RwMayaMaterial class. The class
overrides the RwCommShader::GetTexturesByType() method and the
CCommFurShader::GetFurColor() method. 

MayaFurShaderCreator, creating the MayaFurShader instance from the lambert object.


See Also
--------
The common FurShader project.


/////////////////////////////////////////////////////////////////////////////

