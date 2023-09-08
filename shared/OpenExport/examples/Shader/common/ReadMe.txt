========================================================================
       DYNAMIC LINK LIBRARY : FurShader
========================================================================


This is an example of how to write your own custom shader, how to
export it, and how to stream it to and from an rf3 file. For clarity the
example simply supports a shader with a single color (the "fur" color) plus
a single texture (the "fur" texture).

The example is divided into two plugin dlls:
1. The common FurShader.dll, extending the RwExp, RwMI and RwRf3 libraries.
This dll is responsible for exporting the FurShader and streaming it to
and from an rf3 file.

2. The specific max/maya plugin dll, extending the RwMax/RwMaya libraries.
This dll is responsible for the creation of the FurShader in max/maya,
and for building it, using the RwMaxBuilder/RwMayaBuilder.

Compiling
---------
1. Compile the common plugin and copy it to your main OpenExportPlugins directory.
2. Compile the max/maya specific plugin (the dll will be built directly to
the max/maya plugin directory).

Note: maya users will also need to copy the provided AEMayaShaderTemplate.mel
script into the maya script directory.

Running
-------
Run max/maya, create the custom FurShader material and assign it 
to a geometry. You can now export it directly into RenderWare or
to an rf3 file.

About This Plugin
-----------------
FurShader.cpp is the entry point of the plugin, registering
an rf3 creator (RwRf3FurShaderCreator) and export objects (FurShaderExporter, 
Rf3ExpFurShader) for the example fur shader.

Classes:

CCommFurShader, the common fur shader base class. This class defines
a method for extracting the fur color, and also a fur texture type,
which can be extracted using RwCommShader::GetTextursByType().

FurShaderExporter, deriving from the RwExpRpMaterial class. This
class exports the fur shader into RenderWare. For the clarity of
the example the class extracts the fur color and texture and assigns
them to a standatd RpMaterial.

MIFurShader, a modeller independant implementation of the FurShader.
The class derives from the RwMIShader class and the CCommFurShader class,
adding its implementation of the CCommFurShader methods. To do so,
the class caches the fur color, and uses it's base RwMIShader 
implementation for caching the fur texture. The class is instanced
when reading in an rf3 file.

Rf3ExpFurShader, deriving from the RwRf3ExpShader class. The class is
responsible for streaming out the fur color and texture to the rf3 file.

RwRf3FurShaderCreator, deriving from the RwRf3ShaderCreator class. The
class is responsible for creating an instance of the MIFurShader class,
from an rf3 fur shader description.


See Also
--------
The max/maya FurShader projects.

/////////////////////////////////////////////////////////////////////////////
