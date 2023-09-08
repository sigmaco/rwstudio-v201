========================================================================
       DYNAMIC LINK LIBRARY : CustInfoExport
========================================================================

This is an example project that shows how information about an export
can be dumped in a custom format. 

The dll provides an over-ridden version of RwExpBspExporter. This custom 
version over-rides the PostExport method and saves some information 
about the meshes contained within the BSP asset being expored. 

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
