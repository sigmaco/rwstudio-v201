========================================================================
       DYNAMIC LINK LIBRARY : ScaleAnim
========================================================================

This example adds support for animated scale to the RwExp layer.

The RwExpFrame and RwExpMatrixAnimation creators are overridden and the
CalcTransform method is modified to leave scale in place.

A new RpHAnim keyframe type is registered that supports scale.

The RwExpHAnimHierarchy and RwExpBuildHAnimAnimation actions are overriden
in order to ensure the RpHAnimHierarchy and RpHAnimAnimation created make
use of the new keyframe type.

The RwExpScaleFilter and RwExpOffsetFilter vertex filters are overriden
so that scale is not burnt into geometries.

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


RenderWare Graphics Run-Time Usage
==================================

Support for the scale animation implemented in this exporter example
is not included by default in RenderWare Graphics, however adding support
is relatively simple. The files scalekey.c and scalekey.h contain all the
necessary code to allow usage of scale animations in RenderWare Graphics
applications.
 
To use this support you must compile scalekey.c into your application
and call HAnimFullKeyFrameRegister from the scalekey source after
initialization of RenderWare Graphics and prior to loading any scalekey
based assets. Once this has been completed animations exported containing
scale information can be treated in the same way all other animations are
treated.

Limitations:
============

This plugin assumes that any scale on an object will be applied to children.
3ds Max and Maya allow the user to turn this off and if this is done the 
RenderWare data will not match the scene data.

In 3ds Max this is done using the Inherit options in the Link Info section of
the Hierarchy tab.

In Maya this is done using the Segment Scale Compensate option on joint nodes.
