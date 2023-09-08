/*****************************************************************************
*
* File :      CFXPartSpray.h
*
* Abstract :  Particle spray object using RW PTank
*
*****************************************************************************
*
* This file is a product of Criterion Software Ltd.
*
* This file is provided as is with no warranties of any kind and is
* provided without any obligation on Criterion Software Ltd. or
* Canon Inc. to assist in its use or modification.
*
* Criterion Software Ltd. will not, under any
* circumstances, be liable for any lost revenue or other damages arising
* from the use of this file.
*
* Copyright (c) 2002 Criterion Software Ltd.
* All Rights Reserved.
*
* RenderWare is a trademark of Canon Inc.
*
*****************************************************************************/

#include "framework/core/attributehandler/cattributehandler.h"
#include "framework/core/eventhandler/ceventhandler.h"
#include "framework/toolkits/world/smartptrs/catomicptr.h"
#include "framework/mainloop/logic.h"
#include "framework/toolkits/particles/particles.h"

#include "framework/core/attributehandler/allocation policies/LinearAllocationPolicy.h"

namespace RWS
{
   using namespace RWS::Particle;

   namespace FX
   {
      /**
      *
      * \ingroup Mod_FX
      *
      * Behavior using the particle toolkits to provide a simple, 2 stage particle system. Controls
      * are provided to allow adjustment of particle size, color, velocity, gravity, drag and
      * acceleration. These can also be animated over the particles life-time.
      *
      * Each of the 2 stages has a start and end. The initial values must all be set, however, the
      * end and secondary start/end values can be optionally enabled. Any which are enabled in an end
      * state will be animated between the start values and the values held in the end stage. In the
      * case of the secondary start, any values set here will be immediately flipped to as the particle
      * enters the second stage (if it is enabled). If the value is not 'on' in the 2nd stage start, but
      * is enabled in the 2nd stage end, then smooth animation will be applied between the 1st stage
      * end values (or 1st stage start if not enabled in the 1st stage end) and the 2nd stage end values.
      *
      * The system can be enabled and disabled using messages. A second message allows the rate of flow
      * to be adjusted between 0 and 100 percent of the set values. A third message allows the particle
      * system to be linked to a second object, thereby allowing smoke trails, etc to be supported. In
      * the case of this linkage, the translation and angular offsets can be adjusted. There are also
      * messages to specify the emit particles signal (normally iMsgRunningTick) and to process the
      * lifetimes of emitted particles (again, normally iMsgRunningTick). Finally, there is a rendering
      * message (normally iMsgDoRender) which allows control of the camera during rendering, i.e. for use
      * with split screens, etc.
      *
      * The system can be set to emit when it receives any message (does not attempt to use any passed
      * data) and to update when it receives any message. The render message has to have a RwCamera* as
      * it's data.
      *
      * The maximum number of particles can be set and each instance uses it's own 'tank' of particles.
      * Values can be adjusted on the fly, but in release mode the size of the 'tank' will only be set
      * to the edited value ONCE. It is not set to a default value and then adjusted to the edit value.
      *
      * NOTE: when adjusting the values of the particle system, it may restart or have some particles
      * which exhibit (VERY) odd behavior. This is due to modification of data while they are running. To
      * obtain a completely true indication of the particle effect, the system should be stopped and
      * restarted. It may take several seconds for the particle to stop if the lifetime has been set to
      * a very large value. To enable / disable, use the test fire button on the Enable message.
      *
      * Texture related functionality.
      *
      * Particles can now have textures applied to them. The textures are extracted from the asset by name.
      * The texture name is specified EXACTLY, but without a path or extension. Each particle system can
      * only have on texture. However, to facilitate animation, this texture can be sub-divided into a
      * number of sub-textures. Animation can be used to rotate textures, etc. The sub-textures can be
      * in several formats, none (1x1), 1x2, 2x2, 2x4, 4x4, 4x8 and 8x8. In each case the indices are
      * automatically assigned. The top-left sub-texture is always index zero, the indices are then
      * incremented from left to right and top to bottom. So, for example, with a 2x4 sub-texture...
      *
      * \verbatim

        +---+---+---+---+
        |   0   |   1   |
        +---+---+---+---+
        |   2   |   3   |
        +---+---+---+---+
        |   4   |   5   |
        +---+---+---+---+
        |   6   |   7   |
        +---+---+---+---+

        \endverbatim
      *
      * Textured can be animated from on index to another, either forwards or backwards. They can also
      * be made to do additional loops. With zero loops they animate from start to end. With one loop
      * they animate from start to end, then flip back to start and animate to end a second time. The
      * loop can be set to fractional values and random loop values can also be applied. The values for
      * animation are provided in the primary end state (as with other animation).
      *
      * The texture indices at all stages can be randomly selected, between, for example 0 and 5. The
      * behavior will randomly choose a texture for each particle at the animation stage, between the
      * values specified (inclusive). It should be noted that these values are chosen ONCE. So with
      * animation looping, the loop will be between the same to values on any particular particle.
      *
      * When the second stage is used, the animation operates in two modes. If a secondary start animation
      * value is set, the system will FLIP to the texture index specified (or random range). It then
      * animates (if one is set) to the secondary end values, including looping, etc. If no secondary
      * start animation is specified, then the secondary stage animation will be from the primary end
      * animations finishing texture index, to the one specified in the secondary end stage (if one). If
      * random looping is used in the primary stage animation, the system will still cope and will
      * automatically choose the correct animation direction. That is, if the primary animation ends with
      * values in the range of 0 to 9, and the secondary end animation (no secondary start animation used)
      * is set to 5, then the animation direction for primary end value of 0 to 4 will be forward and for
      * values of 6 to 9 will be backwards. For values of 5, there will be no animation (since there are
      * no animation sets from 5 to 5). If looping is also applied the loop will be (per particle) from the
      * primary stage end index to the secondary end. So if the primary ended at 3 and the specified secondary
      * end is at 5, with 1 loop, the animation sequence would be 3, 4, 5, 3, 4, 5. If the end value for
      * another particle was 7, the sequence would be 7, 6, 5, 7, 6, 5.
      *
      * NOTE: that if a texture only has 8 sub-textures (as above, 0 to 7) and an index of 8 or above is
      * specified, it will be clipped to the 7th index and not wrapped. If the sub-texture type is then
      * modified to, say a 4 x 4, then higher indices will then be used (but will be clipped at 15 instead).
      *
      * Uses the CPB_TexPart class.
      *
      * \see RWS::Particle::CPB_TexPart
      *
      */

      class CFXPartSpray : public CSystemCommands, public CAttributeHandler, public CEventHandler, public LinearAllocationPolicy
      {
      public:

         RWS_MAKENEWCLASS(CFXPartSpray);
         RWS_DECLARE_CLASSID(CFXPartSpray);
         RWS_CATEGORY("FX");
         RWS_DESCRIPTION("Particle spray", "Particle generator with ability to connect to other objects");

         RWS_BEGIN_COMMANDS
            RWS_SEPARATOR("Messages", 0)

            RWS_MESSAGE(MSG_rate, "Set rate", "Name of event which changes the output rate. Data points to a percentage (0 to 100). No effect if test fire.", RECEIVE, "RwUInt32*", 0)
            RWS_MESSAGE(MSG_enable, "Enable", "Name of event which enables/disables system. Data holds flag (0 off, 1 on). Test firing toggles.", RECEIVE, "RwBool*", 0)
            RWS_MESSAGE(MSG_pos, "Position", "Name of event which sets emitter position. Data holds new position and orientation. No effect if test fire.", RECEIVE, "RwFrame*", 0)
            RWS_MESSAGE(MSG_emit, "Emit on", "Name of event which cause emitter to fire. No data needed. Test fire cause emission.", RECEIVE, 0, "iMsgRunningTick")
            RWS_MESSAGE(MSG_update, "Update on", "Name of event which causes system to update. No data needed. Test fire causes single update.", RECEIVE, 0, "iMsgRunningTick")

            RWS_SEPARATOR("Display", 0)
            RWS_MESSAGE(MSG_render, "Render", "Name of event which initializes rendering. Data holds camera.", RECEIVE, "RwCamera*", "iMsgDoRender")
            RWS_ATTRIBUTE(CMD_order, "Render priority", "Rendering priority of particles (0 = last).", SLIDER, RwUInt32, RANGE(0, 0, 65535))
            RWS_ATTRIBUTE(CMD_rendMode, "Rendering mode", "Set the type of rendering. NO Z WRITE works particles with alpha. Z WRITE works if several particle systems overlap. DUAL PASS is slower but works in both cases.", LIST, RwUInt32, LIST("@NO Z WRITE|Z WRITE|DUAL PASS"))
            RWS_ATTRIBUTE(CMD_srcBlend, "Source blend", "Sets the RenderWare source blend mode for the particle system", LIST, RwUInt32, LIST("rwBLENDZERO|rwBLENDONE|rwBLENDSRCCOLOR|rwBLENDINVSRCCOLOR|@rwBLENDSRCALPHA|rwBLENDINVSRCALPHA|rwBLENDDESTALPHA|rwBLENDINVDESTALPHA|rwBLENDDESTCOLOR|rwBLENDINVDESTCOLOR|rwBLENDSRCALPHASAT"))
            RWS_ATTRIBUTE(CMD_destBlend, "Destination blend", "Sets the RenderWare destination blend mode for the particle system", LIST, RwUInt32, LIST("rwBLENDZERO|rwBLENDONE|rwBLENDSRCCOLOR|rwBLENDINVSRCCOLOR|rwBLENDSRCALPHA|@rwBLENDINVSRCALPHA|rwBLENDDESTALPHA|rwBLENDINVDESTALPHA|rwBLENDDESTCOLOR|rwBLENDINVDESTCOLOR|rwBLENDSRCALPHASAT"))
            RWS_ATTRIBUTE(CMD_posOff, "Positional offset", "Positional offset of the emitter from object sending the position update message.", VECTOR, RwV3d, RANGES((-1000, 0, 1000), (-1000, 0, 1000), (-1000, 0, 1000)))
            RWS_ATTRIBUTE(CMD_posHead, "Positional heading", "Positional heading offset of the emitter from object sending the position update message, in degrees.", SLIDER, RwReal, RANGE(-180, 0, 180))
            RWS_ATTRIBUTE(CMD_posDef, "Positional deflection", "Positional deflection of the emitter from object sending the position update message, in degrees.", SLIDER, RwReal, RANGE(-180, 0, 180))

            RWS_SEPARATOR("Emitter", 0)
            RWS_ATTRIBUTE(CMD_max, "Max particles", "Maximum number of particles in the system. Should be as low as possible without causing starvation.", SLIDER, RwUInt32, RANGE(1, 5000, 50000))
            RWS_ATTRIBUTE(CMD_rate, "Output rate", "Num of particles emitted each time.", SLIDER, RwUInt32, RANGE(0, 100, 5000))
            RWS_ATTRIBUTE(CMD_rateSpread, "Output rate spread", "Random additional particles emitted each time.", SLIDER, RwUInt32, RANGE(0, 0, 500))
            RWS_ATTRIBUTE(CMD_life, "Lifetime", "Lifetime of particles in seconds.", SLIDER, RwReal, RANGE(0, 0.5, 10))
            RWS_ATTRIBUTE(CMD_lifeSpread, "Lifetime spread", "Random additional lifetime of particles in seconds.", SLIDER, RwReal, RANGE(0, 0, 5))
            RWS_ATTRIBUTE(CMD_emitSpread, "Emitter spread", "Emitter size in the X, Y and Z axes. Randomly applied.", VECTOR, RwV3d, RANGES((0, 0, 500), (0, 0, 500), (0, 0, 500)))
            RWS_ATTRIBUTE(CMD_spreadX, "Angular spread X", "Emitter's angular variance on X axis. Randomly applied.", SLIDER, RwReal, RANGE(0.0, 10.0, 360.0))
            RWS_ATTRIBUTE(CMD_spreadZ, "Angular spread Z", "Emitter's angular variance on Z axis. Randomly applied.", SLIDER, RwReal, RANGE(0.0, 10.0, 360.0))
            RWS_ATTRIBUTE(CMD_texName, "Texture name", "Name of texture to use. Blank = none. Texture is taken from textures on associated asset. Name must be EXACT (no path, no extension).", EDIT, RwChar, DEFAULT(""))
            RWS_ATTRIBUTE(CMD_subTexType, "Sub-texture types", "The number of sub-textures on the selected texture (if one). Sub-tex indices are auto assigned, top-left = 0, then increase left to right, top to bottom.", LIST, RwUInt32, LIST("NONE|1x2|2x2|2x4|4x4|4x8|8x8"))

            RWS_SEPARATOR("Primary start state", 0)
            RWS_ATTRIBUTE(CMD_vel, "Velocity", "Initial velocity of particles", SLIDER, RwReal, RANGE(0.01, 75.0, 1000.0))
            RWS_ATTRIBUTE(CMD_velSpread, "Velocity spread", "Initial additional velocity of particles. Randomly applied.", SLIDER, RwReal, RANGE(0, 0, 500.0))
            RWS_ATTRIBUTE(CMD_gravity, "Gravity", "Initial gravity rate on particles.", SLIDER, RwReal, RANGE(-25.0, 9.81, 25.0))
            RWS_ATTRIBUTE(CMD_drag, "Drag", "Initial rate of drag on particles.", SLIDER, RwReal, RANGE(0.0, 0.1, 1.0))
            RWS_ATTRIBUTE(CMD_col, "Color", "Initial color of particles.", COLOR, RwRGBA, DEFAULT(4294967295))
            RWS_ATTRIBUTE(CMD_rVar, "Red spread", "+/- variance of red in particle's color. Randomly applied.", SLIDER, RwUInt32, RANGE(0, 0, 255))
            RWS_ATTRIBUTE(CMD_gVar, "Green spread", "+/- variance of green in particle's color. Randomly applied.", SLIDER, RwUInt32, RANGE(0, 0, 255))
            RWS_ATTRIBUTE(CMD_bVar, "Blue spread", "+/- variance of blue in particle's color. Randomly applied.", SLIDER, RwUInt32, RANGE(0, 0, 255))
            RWS_ATTRIBUTE(CMD_aVar, "Alpha spread", "+/- variance of alpha in particle's color. Randomly applied.", SLIDER, RwUInt32, RANGE(0, 0, 255))
            RWS_ATTRIBUTE(CMD_size, "Particle size", "Initial size of particles.", SLIDER, RwReal, RANGE(0.1, 5.0, 100.0))
            RWS_ATTRIBUTE(CMD_sizeSpread, "Particle size spread", "Additional initial size of particles. Randomly applied.", SLIDER, RwReal, RANGE(0.0, 0.0, 50.0))
            RWS_ATTRIBUTE(CMD_psMinSubTex, "Minimum sub-tex index", "The sub-texture index is randomly selected between min and max (if using a texture).", SLIDER, RwUInt32, RANGE(0, 0, 63))
            RWS_ATTRIBUTE(CMD_psMaxSubTex, "Maximum sub-tex index", "The sub-texture index is randomly selected between min and max (if using a texture).", SLIDER, RwUInt32, RANGE(0, 0, 63))
         
            RWS_SEPARATOR("Primary end / anim state", 0)
            RWS_ATTRIBUTE(CMD_peAccelOn, "Acceleration on", "Allow continuous acceleration of particle on primary stage.", BOOLEAN, RwUInt32, DEFAULT(0))
            RWS_ATTRIBUTE(CMD_peAccel, "Acceleration", "Level of continuous acceleration of particles during primary stage.", SLIDER, RwReal, RANGE(0, 0, 20.0))
            RWS_ATTRIBUTE(CMD_peAccelSpread, "Acceleration spread", "Additional acceleration of particles during primary stage. Randomly applied.", SLIDER, RwReal, RANGE(0, 0, 10.0))
            RWS_ATTRIBUTE(CMD_peGravityOn, "Gravity on", "Allow animation of gravity over primary stage.", BOOLEAN, RwUInt32, DEFAULT(0))
            RWS_ATTRIBUTE(CMD_peGravity, "Gravity", "Stage end gravity rate on particle.", SLIDER, RwReal, RANGE(-25.0, 9.81, 25.0))
            RWS_ATTRIBUTE(CMD_peDragOn, "Drag on", "Allow animation of drag over primary stage.", BOOLEAN, RwUInt32, DEFAULT(0))
            RWS_ATTRIBUTE(CMD_peDrag, "Drag", "Stage end rate of drag on particle.", SLIDER, RwReal, RANGE(0.0, 0.1, 1.0))
            RWS_ATTRIBUTE(CMD_peColOn, "Color on", "Allow animation of color over primary stage.", BOOLEAN, RwUInt32, DEFAULT(0))
            RWS_ATTRIBUTE(CMD_peCol, "Color", "Stage end color of particles.", COLOR, RwRGBA, DEFAULT(4294967295))
            RWS_ATTRIBUTE(CMD_peRVar, "Red spread", "Stage end +/- variance of red in particle's color. Randomly applied.", SLIDER, RwUInt32, RANGE(0, 0, 255))
            RWS_ATTRIBUTE(CMD_peGVar, "Green spread", "Stage end +/- variance of green in particle's color. Randomly applied.", SLIDER, RwUInt32, RANGE(0, 0, 255))
            RWS_ATTRIBUTE(CMD_peBVar, "Blue spread", "Stage end +/- variance of blue in particle's color. Randomly applied.", SLIDER, RwUInt32, RANGE(0, 0, 255))
            RWS_ATTRIBUTE(CMD_peAVar, "Alpha spread", "Stage end +/- variance of alpha in particle's color. Randomly applied.", SLIDER, RwUInt32, RANGE(0, 0, 255))
            RWS_ATTRIBUTE(CMD_peSizeOn, "Size on", "Allow animation of particle size over primary stage.", BOOLEAN, RwUInt32, DEFAULT(0))
            RWS_ATTRIBUTE(CMD_peSize, "Particle size", "Stage end size of particles.", SLIDER, RwReal, RANGE(0.1, 0.5, 100.0))
            RWS_ATTRIBUTE(CMD_peSizeSpread, "Particle size spread", "Additional stage end size of particles. Randomly applied.", SLIDER, RwReal, RANGE(0.0, 0.0, 50.0))
            RWS_ATTRIBUTE(CMD_peSubTexOn, "Sub-texture on", "Allows animation of the sub-texture index over the primary stage.", BOOLEAN, RwUInt32, DEFAULT(0))
            RWS_ATTRIBUTE(CMD_peMinSubTex, "Minimum end sub-tex index", "The sub-texture index is randomly selected between min and max (if using a texture). This index is animated from the value in the stage start.", SLIDER, RwUInt32, RANGE(0, 0, 63))
            RWS_ATTRIBUTE(CMD_peMaxSubTex, "Maximum end sub-tex index", "The sub-texture index is randomly selected between min and max (if using a texture). This index is animated from the value in the stage start.", SLIDER, RwUInt32, RANGE(0, 0, 63))
            RWS_ATTRIBUTE(CMD_peSubTexLoop, "Number of sub-tex loops", "Number of times to loop from start index to end index of the sub-texture", SLIDER, RwReal, RANGE(0.0, 0.0, 100.0))
            RWS_ATTRIBUTE(CMD_peSubTexRndLoop, "No. sub-tex loops spread", "Number of random loops from start index to end index of the sub-texture.", SLIDER, RwReal, RANGE(0.0, 0.0, 50.0))
         
            RWS_SEPARATOR("Secondary change over", 0)
            RWS_ATTRIBUTE(CMD_secOn, "Secondary on", "Allow secondary state.", BOOLEAN, RwUInt32, DEFAULT(0))
            RWS_ATTRIBUTE(CMD_secTime, "Secondary lifetime change", "Life time at which change to secondary state occurs in seconds.", SLIDER, RwReal, RANGE(0.1, 1, 15.0))
            RWS_ATTRIBUTE(CMD_secEmit, "Secondary emitter", "Number of extra particles emitted on secondary state emitter. For each first stage particle, this number will be emitted. Only noticeable if emitter angle is non-zero.", SLIDER, RwUInt32, RANGE(0, 0, 500))
            RWS_ATTRIBUTE(CMD_secEmitSpread, "Secondary emit spread", "Number of additional extra particles PER primary stage particle. Randomly applied.", SLIDER, RwUInt32, RANGE(0, 0, 250))
            RWS_ATTRIBUTE(CMD_secLifeSpread, "Secondary emit life adjust", "Adjustment of lifetime on second stage emitted particles ONLY. Based on lifetime of source primary stage particle. Variance in seconds. Randomly applied.", SLIDER, RwReal, RANGE(0, 0, 10))
            RWS_ATTRIBUTE(CMD_secMinAng, "Secondary emitter min angle", "Minimum angular deflection of particles. Randomly applied between min & max. Slower if non-zero. Heading change random.", SLIDER, RwReal, RANGE(0, 0, 180.0))
            RWS_ATTRIBUTE(CMD_secMaxAng, "Secondary emitter max angle", "Maximum angular deflection of particles. Randomly applied between min & max. Slower if non-zero. Heading change random.", SLIDER, RwReal, RANGE(0, 0, 180.0))
         
            RWS_SEPARATOR("Secondary start state", 0)
            RWS_ATTRIBUTE(CMD_ssVelOn, "Secondary boost on", "Enable one time only secondary boost.", BOOLEAN, RwUInt32, DEFAULT(0))
            RWS_ATTRIBUTE(CMD_ssVel, "Secondary boost", "Secondary state emitter particle velocity boost, once only. Instantly applied.", SLIDER, RwReal, RANGE(0.0, 0.0, 1000.0))
            RWS_ATTRIBUTE(CMD_ssVelSpread, "Secondary boost spread", "Additional secondary particle velocity boost. Instant, random application.", SLIDER, RwReal, RANGE(0.0, 0.0, 500.0))
            RWS_ATTRIBUTE(CMD_ssGravityOn, "Gravity on", "Allow change of gravity.", BOOLEAN, RwUInt32, DEFAULT(0))
            RWS_ATTRIBUTE(CMD_ssGravity, "Gravity", "Gravity rate per second on particle. Instantly applied.", SLIDER, RwReal, RANGE(-25.0, 9.81, 25.0))
            RWS_ATTRIBUTE(CMD_ssDragOn, "Drag on", "Allow change of drag.", BOOLEAN, RwUInt32, DEFAULT(0))
            RWS_ATTRIBUTE(CMD_ssDrag, "Drag", "Rate of drag on particle. Instantly applied.", SLIDER, RwReal, RANGE(0.0, 0.1, 1.0))
            RWS_ATTRIBUTE(CMD_ssColOn, "Color on", "Allow change of color.", BOOLEAN, RwUInt32, DEFAULT(0))
            RWS_ATTRIBUTE(CMD_ssCol, "Color", "Start color of particle. Instantly applied.", COLOR, RwRGBA, DEFAULT(4294967295))
            RWS_ATTRIBUTE(CMD_ssRVar, "Red spread", "+/- variance of red in particles. Randomly applied.", SLIDER, RwUInt32, RANGE(0, 0, 255))
            RWS_ATTRIBUTE(CMD_ssGVar, "Green spread", "+/- variance of green in particles. Randomly applied.", SLIDER, RwUInt32, RANGE(0, 0, 255))
            RWS_ATTRIBUTE(CMD_ssBVar, "Blue spread", "+/- variance of blue in particles. Randomly applied.", SLIDER, RwUInt32, RANGE(0, 0, 255))
            RWS_ATTRIBUTE(CMD_ssAVar, "Alpha spread", "+/- variance of alpha in particles. Randomly applied.", SLIDER, RwUInt32, RANGE(0, 0, 255))
            RWS_ATTRIBUTE(CMD_ssSizeOn, "Size on", "Allow change of particle size.", BOOLEAN, RwUInt32, DEFAULT(0))
            RWS_ATTRIBUTE(CMD_ssSize, "Particle size", "Start size of particles.", SLIDER, RwReal, RANGE(0.1, 0.5, 100.0))
            RWS_ATTRIBUTE(CMD_ssSizeSpread, "Particle size spread", "Additional extra size of particles. Randomly applied.", SLIDER, RwReal, RANGE(0.0, 0.0, 50.0))
            RWS_ATTRIBUTE(CMD_ssSubTexOn, "Sub-texture on", "Allows snap change of the sub-texture index as enters the secondary stage.", BOOLEAN, RwUInt32, DEFAULT(0))
            RWS_ATTRIBUTE(CMD_ssMinSubTex, "Minimum sub-tex index", "The sub-texture index is randomly selected between min and max (if using a texture). Snap change of index occurs.", SLIDER, RwUInt32, RANGE(0, 0, 63))
            RWS_ATTRIBUTE(CMD_ssMaxSubTex, "Maximum sub-tex index", "The sub-texture index is randomly selected between min and max (if using a texture). Snap change of index occurs.", SLIDER, RwUInt32, RANGE(0, 0, 63))
         
            RWS_SEPARATOR("Secondary end / anim state", 0)
            RWS_ATTRIBUTE(CMD_seAccelOn, "Acceleration on", "Allow continuous acceleration over secondary stage.", BOOLEAN, RwUInt32, DEFAULT(0))
            RWS_ATTRIBUTE(CMD_seAccel, "Acceleration", "Level of continuous acceleration of particles during stage.", SLIDER, RwReal, RANGE(0, 0, 20.0))
            RWS_ATTRIBUTE(CMD_seAccelSpread, "Acceleration spread", "Additionally extra acceleration of particles. Randomly applied.", SLIDER, RwReal, RANGE(0, 0, 10.0))
            RWS_ATTRIBUTE(CMD_seGravityOn, "Gravity on", "Allow animation of gravity over secondary stage.", BOOLEAN, RwUInt32, DEFAULT(0))
            RWS_ATTRIBUTE(CMD_seGravity, "Gravity", "Stage end gravity rate per second on particle.", SLIDER, RwReal, RANGE(-25.0, 9.81, 25.0))
            RWS_ATTRIBUTE(CMD_seDragOn, "Drag on", "Allow animation of drag over secondary stage.", BOOLEAN, RwUInt32, DEFAULT(0))
            RWS_ATTRIBUTE(CMD_seDrag, "Drag", "Stage end rate of drag on particle.", SLIDER, RwReal, RANGE(0.0, 0.1, 1.0))
            RWS_ATTRIBUTE(CMD_seColOn, "Color on", "Allow animation of color over secondary stage.", BOOLEAN, RwUInt32, DEFAULT(0))
            RWS_ATTRIBUTE(CMD_seCol, "Color", "Stage end color of particles.", COLOR, RwRGBA, DEFAULT(4294967295))
            RWS_ATTRIBUTE(CMD_seRVar, "Red spread", "Stage end +/- variance of red in particles. Randomly applied.", SLIDER, RwUInt32, RANGE(0, 0, 255))
            RWS_ATTRIBUTE(CMD_seGVar, "Green spread", "Stage end +/- variance of green in particles. Randomly applied.", SLIDER, RwUInt32, RANGE(0, 0, 255))
            RWS_ATTRIBUTE(CMD_seBVar, "Blue spread", "Stage end +/- variance of blue in particles. Randomly applied.", SLIDER, RwUInt32, RANGE(0, 0, 255))
            RWS_ATTRIBUTE(CMD_seAVar, "Alpha spread", "Stage end +/- variance of alpha in particles. Randomly applied.", SLIDER, RwUInt32, RANGE(0, 0, 255))
            RWS_ATTRIBUTE(CMD_seSizeOn, "Size on", "Allow animation of particle size over stage.", BOOLEAN, RwUInt32, DEFAULT(0))
            RWS_ATTRIBUTE(CMD_seSize, "Particle size", "Stage end size of particles.", SLIDER, RwReal, RANGE(0.1, 0.5, 100.0))
            RWS_ATTRIBUTE(CMD_seSizeSpread, "Particle size spread", "Stage end additional size of particles. Randomly applied.", SLIDER, RwReal, RANGE(0.0, 0.0, 50.0))
            RWS_ATTRIBUTE(CMD_seSubTexOn, "Sub-texture on", "Allows animation of the sub-texture index over the primary stage.", BOOLEAN, RwUInt32, DEFAULT(0))
            RWS_ATTRIBUTE(CMD_seMinSubTex, "Minimum end sub-tex index", "The sub-texture index is randomly selected between min and max (if using a texture). This index is animated from the value in the stage start.", SLIDER, RwUInt32, RANGE(0, 0, 63))
            RWS_ATTRIBUTE(CMD_seMaxSubTex, "Maximum end sub-tex index", "The sub-texture index is randomly selected between min and max (if using a texture). This index is animated from the value in the stage start.", SLIDER, RwUInt32, RANGE(0, 0, 63))
            RWS_ATTRIBUTE(CMD_seSubTexLoop, "Number of sub-tex loops", "Number of times to loop from start index to end index of the sub-texture. Start index here is the index on entry to the secondary stage.", SLIDER, RwReal, RANGE(0.0, 0.0, 100.0))
            RWS_ATTRIBUTE(CMD_seSubTexRndLoop, "No. sub-tex loops spread", "Number of random loops from start index to end index of the sub-texture. Start index here is the index on entry to the secondary stage.", SLIDER, RwReal, RANGE(0.0, 0.0, 50.0))

         RWS_END_COMMANDS;

         CFXPartSpray(const CAttributePacket &attr);
         ~CFXPartSpray(void);

         virtual void HandleEvents(CMsg &msg);
         virtual void HandleAttributes(const CAttributePacket &attr);

      protected:

         /**
         *
         * Used to select the appropriate texture within the 'GetTexCallBack' function.
         *
         * \see GetTexCallBack.
         * \see GetTexture
         *
         */

         class tex_select
         {
         public:

            RwTexture *m_pTexture;  /**< Pointer to texture. This is setup by calls to 'GetTexCallBack' via RenderWare. */
            const char *m_pName;    /**< Pointer to a string of the texture name to search for - not path or extension. */

            /**
            *
            * Just initialize the values.
            *
            */

            tex_select(void) :
                  m_pTexture(0),
                  m_pName(0)
            {
            }
         };

         static RwUInt32 m_subTexLookupU[7];             /**< Lookup table for number of sub-tex in U - CMD_subTexType. */
         static RwUInt32 m_subTexLookupV[7];             /**< Lookup table for number of sub-tex in V - CMD_subTexType. */
         static RwBlendFunction m_blendLookup[11];       /**< Lookup table for RenderWare blend modes -
                                                              CMD_srcBlend & CMD_destBlend. */

         static CycPartBuff<CPB_TexPart>::RENDER_MODE m_renderModeLookup[3];   /**< Lookup table for CycPartBuff
                                                                                    rendering modes. CMD_rendMode. */

         static RpMaterial *GetTexCallBack(RpMaterial *pMaterial, void *pData);
         RwTexture *GetTexture(const char *pName);

         // Be careful re-ording these items...
         //
         CAtomicPtr m_pAtomic;                        /**< Atomic for instance. */
         RwReal m_ftHeading,                          /**< Heading adjustment angle for other asset linkage. */
                m_ftDeflect;                          /**< Deflection adjustment angle for other asset linkage. */

         CEventId m_posMsg;                           /**< Event id for other asset linkage update. */

         // ...m_frameTrans contains an RwMatrix which needs to be 64 byte alligned
         //
         FrameTrans m_frameTrans;                     /**< Object to allow linkage to other asset instance. */

         CycPartBuff<CPB_TexPart> *m_pParticles;      /**< Object containing particle system. */

         CEventId m_rateMsg,                          /**< Event id for rate adjustment message. */
                  m_enableMsg,                        /**< Event id for enabling message. */
                  m_emitMsg,                          /**< Event id for particle emit trigger message. */
                  m_updateMsg,                        /**< Event id for particle tick or update message. */
                  m_renderMsg;                        /**< Event id for rendering message. */

         RwUInt16 m_renderPrior;                      /**< The rendering priority of the particle system. */
      };
   }        //namespace FX
}
