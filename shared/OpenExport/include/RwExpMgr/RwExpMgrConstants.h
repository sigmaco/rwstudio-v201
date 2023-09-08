#ifndef RWEXPMGRCONSTANTS_H
#define RWEXPMGRCONSTANTS_H

// This is the number of asset templates defined,
// the rest will be project templates.
#define NUM_ASSET_TEMPLATES             7

#define ANIMATION_TEMPLATE              L"Animation.rwt"
#define STATIC_WORLD_TEMPLATE           L"Static World.rwt"
#define ANIMATED_HIERARCHY_TEMPLATE     L"Animated Hierarchy.rwt"
#define SPLINE_TEMPLATE                 L"Spline.rwt"
#define AUDIOWAVEDICT_TEMPLATE          L"AudioWaveDict.rwt"
#define AUDIOEVENTDICT_TEMPLATE         L"AudioEventDict.rwt"
#define AUDIOSTREAM_TEMPLATE            L"AudioStream.rwt"

#define GAMECUBE_TEMPLATE               L"GameCube.rwt"
#define GENERIC_TEMPLATE                L"Generic.rwt"
#define PS2_TEMPLATE                    L"PS2.rwt"
#define XBOX_TEMPLATE                   L"Xbox.rwt"
#define RWSTUDIO_TEMPLATE               L"RwStudio.rwt"

// list indices
#define ANIMATION_LIST_INDEX            0
#define STATIC_WORLD_LIST_INDEX         1
#define ANIMATION_HIERARCHY_LIST_INDEX  2
#define SPLINE_LIST_INDEX               3

#define AUDIOWAVEDICT_LIST_INDEX        4
#define AUDIOEVENTDICT_LIST_INDEX       5
#define AUDIOSTREAM_LIST_INDEX          6

#define GAMECUBE_LIST_INDEX             7
#define GENERIC_LIST_INDEX              8
#define PS2_LIST_INDEX                  9
#define XBOX_LIST_INDEX                 10

enum ExpMgrTemplateScopeType
{
    NO_SCOPE         = 0x00000000,
    GRAPHICS_SCOPE   = 0x00000001,
    AUDIO_SCOPE      = 0x00000002,
    ALL_SCOPE        = 0xFFFFFFFF
};

static RwWString rwExpMgrLocationList[] =
{
    L"anm", L"bsp", L"dff", L"spl", L"audiowavedict", L"audioeventdict", L"audiostream",
    L"gamecube", L"generic", L"ps2", L"xbox", L"rwstudio"
};

static RwWString rwExpMgrTemplateList[] =
{
    ANIMATION_TEMPLATE, STATIC_WORLD_TEMPLATE,
    ANIMATED_HIERARCHY_TEMPLATE, SPLINE_TEMPLATE,
    AUDIOWAVEDICT_TEMPLATE, AUDIOEVENTDICT_TEMPLATE, AUDIOSTREAM_TEMPLATE,
    GAMECUBE_TEMPLATE, GENERIC_TEMPLATE, PS2_TEMPLATE, XBOX_TEMPLATE, RWSTUDIO_TEMPLATE
};

static ExpMgrTemplateScopeType rwExpMgrScopeList[] =
{
    GRAPHICS_SCOPE, GRAPHICS_SCOPE, GRAPHICS_SCOPE, GRAPHICS_SCOPE,
    AUDIO_SCOPE, AUDIO_SCOPE, AUDIO_SCOPE,
    ALL_SCOPE, ALL_SCOPE, ALL_SCOPE, ALL_SCOPE, ALL_SCOPE
};

#endif // RWEXPMGRCONSTANTS_H