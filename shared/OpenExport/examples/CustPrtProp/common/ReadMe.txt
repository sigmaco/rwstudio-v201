========================================================================
       DYNAMIC LINK LIBRARY : CustInfoExport
========================================================================

This example shows how to extend the particle object to include a custom 
defector plane emitter attribute.

The example shows how to override the RwRF3, RwExp, RwComm, RwMax and RwMaya 
class layers of the RenderWare particle emitter's OpenExport API to create a 
custom RenderWare Emitter property for export from both 3dsmax and Maya.

Configurations
==================================

The example dsp comes with multiple configurations:

Release
This configuration builds optimised binaries and links against
the release runtime and OpenExport libraries. It should be used
when the standard 3ds max/maya plugins and OpenExport dlls are
installed and you do not wish to debug.

Debug
This configuration builds debug binaries and links against the debug
runtime and OpenExport libraries. It should only be used when the debug
3ds max/maya plugins and OpenExport dlls are installed. See the "What you
should use to build" section of the OpenExport chm for more information on
installing the debug 3ds max/maya plugins and OpenExport dlls.

Hybrid
This configuration builds debug binaries and links against the release
runtime and OpenExport libraries. It should be used when debugging and
the standard 3ds max/maya plugins and OpenExport dlls are installed.

