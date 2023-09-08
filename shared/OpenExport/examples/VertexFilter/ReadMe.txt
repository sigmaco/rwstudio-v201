========================================================================
       DYNAMIC LINK LIBRARY : PreLight
========================================================================


This is an example of a vertex filter which pre-lights a scene. It uses a very
basic phong lighting equation:

I = Ia*Ka + Id*Kd(N*L), where

Ia - light's ambient intensity
Id - light's diffuse intensity
Ka - material's ambient valiue
Kd - material's diffuse value
N - the vertex's normal
L - Vector form the vertex facing the light

It calcualtes the color value for each vertex and bakes it in each
vertex's color.

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
