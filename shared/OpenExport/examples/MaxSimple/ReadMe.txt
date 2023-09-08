========================================================================
       DYNAMIC LINK LIBRARY : MaxSimple
========================================================================

This is an example of how to write your own custom builder and export
application. To demonstrate this we used a simplified 3dsmax exporter.
Please note that this implementation is different from the original RwMax
implemntation, and contains only a small subset of the RwMax export features.

Compiling
---------
Change the include directory to point to your installed maxsdk/include directory.
Change the output file path to point to your installed max plugin directory. 

Configurations
--------------
The example dsp comes with multiple configurations for both 3ds max
4 and 5:

Release
This configuration builds optimised binaries and links against
the release runtime and OpenExport libraries. It should be used
when you do not wish to debug.

Debug
This configuration builds debug binaries and links against the debug
runtime and OpenExport libraries. It should be used when you wish
to debug and want assert and call stack information from the debug
OpenExport dlls.

Hybrid
This configuration builds debug binaries and links against the release
runtime and OpenExport libraries. It should be used when debugging and
you wish to use the standard, release mode OpenExport dlls.

Running
-------
From the Max File -> Export menu, choose export to an RWS file. Specify the
rws file to export to and press the Save button.


About the Example
-----------------

MaxSimpleExp Class - This is the main export application class. The class
simply calls the ExportManager::Export() method, passing is a single asset.
(See the MaxSimpleExp::DoExport() method). The asset we created for this
example is a dummy asset class, which only holds the asset template name.
We assign a "Static World" template to the asset, which causes the export
manager to perform a BSP export.

MaxSimpleBuilder Class - This is a simplified version of the RwMaxBuilder
class. The class holds a lookup table, mapping Max SuperClassIDs to
RwCommObjCreator objects. The builder traverses the Max scene and  attempts
to build each Max object it encounters, using the lookup table. For this
example we've implemented three RwCommObject classes : MaxSimpleTransform,
MaxSimpleMesh and MaxSimpleShader. All these classes are registered by default
in the MaxSimpleBuilder. (They are registered in the class' constructor). An
instance of the MaxSimpleBuilder class is attached to the export manager, so
that each time we call the RwExpMgrExportmanager::Export() method, our own custom
scene graph is built using our builder.

/////////////////////////////////////////////////////////////////////////////
