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

2. The specific max plugin dll, extending the RwMax library.
This dll is responsible for the creation of the FurShader in max,
and for building it, using the RwMaxBuilder.

Compiling
---------
1. Compile the common plugin and copy it to your main OpenExportPlugins directory.
2. Compile the max specific plugin (the dll will be built directly to
the max plugin directory).

Running
-------
Run max, create the custom FurShader material and assign it 
to a geometry. You can now export it directly into RenderWare or
to an rf3 file.

About This Plugin
-----------------
MaxFurShaderMain.cpp is the entry point of the plugin, registering
the MaxFurShader creator (MaxFurShaderCreator). In addition it is
responsible for initializing the max plugin.

Classes:

MaxMaterial, deriving from the max Mtl class. This is simply an 
implementation of our own custom material, with a color slider
and a texture chooser.

MaxFurShader, deriving from the RwMaxShader class and the CCommFurShader. This
class adds the fur functionality to the RwMaxShader class. The class
overrides the RwCommShader::GetTexturesByType() method and the
CCommFurShader::GetFurColor() method. 

MaxFurShaderCreator, creating the MaxFurShader instance from the MaxMaterial.


See Also
--------
The common FurShader project.


/////////////////////////////////////////////////////////////////////////////
