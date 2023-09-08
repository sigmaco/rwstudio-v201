========================================================================
       DYNAMIC LINK LIBRARY : Stream
========================================================================


This example demonstrates how to post process the entire list of exported
RenderWare assets, and how to customize the stream process for streaming
them out. 

It does this by overriding the Export Manager class, and implementing
its StreamAssets() method. The method traverses the asset list, looking
for RpClumps. Each time a clump is encountered in the list, it is cloned
and modified by some trivial modification function (that just changes
all the RpMaterial colors in the clump). The modified clump is added to
a new asset list and is streamed out. We stream out multiple reprsenations
of the clump, each one containing different data (in our case - different
color).

This mechanism can be used, for example, for generating multiple LOD
representations of the same mesh data. Each representation can be streamed
out to a seperate file, where it can be loaded dynamically, at runtime,
according to the LOD view criteria.

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
