========================================================================
       DYNAMIC LINK LIBRARY : GeomFilter
========================================================================

This is an example of a traverse action that filters out all nodes containing a certain name.
It demonstrates the use of traverse actions together with traverse lists. 

The action is registered in the Bsp exporter and is activated everytime the Bsp exporter
is activated.

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
