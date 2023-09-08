/*****************************************************************************
 *
 * File :     attributemacros.h
 *
 * Abstract : Definition of automated form generation macros
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
 * Copyright (c) 2001 Criterion Software Ltd.
 * All Rights Reserved.
 *
 * RenderWare is a trademark of Canon Inc.
 *
 *****************************************************************************/

#ifndef __ATTRIBUTEMACROS_H__
#define __ATTRIBUTEMACROS_H__

#ifdef DOLPHIN
#include <string.h>
#endif

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Interface
//
// Macros for defining interface between RenderWare Studio Console App and the editor,
// these are macros and not constants or templates etc because most of what is defined
// is discarded by the preprocessor.
//

/**
*
*  \ingroup Forms
*
*  \page FormsOverview Overview
*
*  The following macros are recognized by the RenderWare Studio Workspace
*  source code parser. They tell the Workspace what attributes and/or events
*  belong to a behavior and how to create the user interface for editing 
*  them.
*
*/

/**
*
*  \ingroup Forms
*
*  Disable auto form generation, this keyword is picked up by the RenderWare Studio API,
*  causing it to ignore the code \ref RWS_END_MACRO_DEFINITION
*
*/

#define RWS_BEGIN_MACRO_DEFINITION

#ifdef DOXYGEN

/**
*
*  \ingroup Forms
*
*  Define the start of a new user interface associated with this class.
*
*/

#define RWS_BEGIN_COMMANDS

#else

#define RWS_BEGIN_COMMANDS    enum LoadParameterTypes {

#endif

#ifdef DOXYGEN

/**
*
*  \ingroup Forms
*
*  Define the end of the user interface associated with this class.
*
*/

#define RWS_END_COMMANDS

#else

#define RWS_END_COMMANDS     SIZE_ATTRIBUTE, LAST_ATTRIBUTE = RWFORCEENUMSIZEINT}

#endif

#ifdef DOXYGEN
/**
*
* \ingroup Forms
*
* Define a control within the user interface associated with this class, used to specify an event that will
* be sent or received by this behavior.
*
* \li x Enum Id
* \li s Short Text Description
* \li t Long Help Text Description
* \li a Event/Message Type Transmit Or Receive
* \li b Data Type associated with this event/message
* \li d Default message
*
* Example 
*
*  \verbatim

   RWS_MESSAGE( CMD_targetname,"(targetname) Trigger the platform","",RECEIVE,0,"ACTN_PLAYERTOUCH")
   
   \endverbatim
*
*/

#define RWS_MESSAGE(x, s, t, a, b, d)

#else

#define RWS_MESSAGE(x, s, t, a, b, d)  x,

#endif

#ifdef DOXYGEN
/**
*
*  \ingroup Forms
*
* Define a control within the user interface associated with this class, used to specify an attribute.
* An attribute must be one of the following pre-defined types.
*
* \param id Attribute Id, must be unique within the scope of the class.
* \param short_txt Short Text Description.
* \param long_txt Long Text Description.
* \param type Control Type.\n
* \b NULL - No Control\n
* \b BOOLEAN - Boolean Tick Box control, \a data_type must be RwUInt32, \a init_data must specify the
*              default DEFAULT(0)\n
* \b SLIDER - Slider control, \a data_type can be RwUInt32, RwInt32, or RwReal, \a init_data must
*             specify a range RANGE(min,default,max)\n
* \b COLOUR - Color picker control, \a data_type should be RwUInt32, \a init_data must specify the
*             default value DEFAULT(46761)\n
* \b COLOR - see COLOUR\n
* \b LIST - List control, \a data_type should be RwUInt32, \a init_data must define a list of items
*           LIST("A|B|@C|D") note the @ character at the start of a list element is used to defined
*           the default value.\n
* \b RADIO_BUTTON - As for list except user interface is a set of radio buttons.\n
* \b VECTOR - Vector control, \a data_type should be RwV3d, \a init_data must define a set of ranges
*             RANGES((min,default,max),(min,default,max),(min,default,max))\n
* \b BITFIELD - Bitfield control, \a data_type should be RwUInt32, \a init_data must define a list of
*               items LIST("A|B|C|D")\n
* \b MATRIX - Matrix control, \a data_type should be RwMatrix, \a init_data is ignored.\n
* \b EDIT - Single Line edit control, \a data_type should be RwChar, RwInt32, RwUInt32, or RwReal, \a
*           default value defined using DEFAULT(...) \n
* \b TEXTEDIT - Multi Line Text edit control, \a data_type should be RwChar, \a init_data should be
*               DEFAULT("Enter Text Here...")
* \param data_type Data type\n
* \b RwUInt32 - Unsigned 32 bit integer.\n
* \b RwInt32 - Signed 32 bit integer.\n
* \b RwReal - Signed 32 bit real.\n
* \b RwChar - RenderWare char type.\n
* \b RwV3d - RenderWare vector type.\n
* \b RwMatrix - RenderWare matrix type.\n
* \b Custom - Custom controls can handle custom data types.
* \param init_data Depends on (Control Type)\n
* \b DEFAULT - Specify a default value\n
* \b RANGE - Specify the range of the slider\n
* \b LIST - Specify a list of items, default item is tagged with @ character\n
* \b RANGES - Specify ranges used by the vector control.\n
*
* Examples 
*
*  \verbatim

   RWS_ATTRIBUTE( CMD_SetDebug, "Debug",
      "Enable/Disable debugging, each attribute handler has a RwUint32 m_debug var "
      "which is available for use by any derived class", BOOLEAN, RwUInt32, DEFAULT(0))
   RWS_ATTRIBUTE( CMD_height, "(height) How high it will rise", "If height is zero, "
      "the size of the geometry is used.", SLIDER, RwReal, RANGE(0,0,999))
   RWS_ATTRIBUTE( CMD_Set_t_color, "Color", "Set light color, see RpLightSetColor",
      COLOR, RwUInt32, DEFAULT(0xffffffff))
   RWS_ATTRIBUTE( CMD_rwRENDERSTATESRCBLEND, "rwRENDERSTATESRCBLEND", "", LIST,
      RwUInt32, LIST("rwBLENDZERO|rwBLENDONE|rwBLENDSRCCOLOR|rwBLENDINVSRCCOLOR|@rwBLENDSRCALPHA"
      "|rwBLENDINVSRCALPHA|rwBLENDDESTALPHA|rwBLENDINVDESTALPHA|rwBLENDDESTCOLOR|rwBLENDINVDESTCOLOR"
      "|rwBLENDSRCALPHASAT"))
   RWS_ATTRIBUTE( CMD_Set_m_center_offset, "Offset for center of collision", "",
      VECTOR, RwV3d, RANGES((-50, 0, 50),(-50, 0, 50),(-50, 0, 50)))
   RWS_ATTRIBUTE( CMD_lightmapflags, "Lightmap Flags", "", BITFIELD, RwUInt32,
      LIST("rpLTMAPSTYLERENDERBASE|rpLTMAPSTYLERENDERLIGHTMAP|rpLTMAPSTYLEPOINTSAMPLE"))
   RWS_ATTRIBUTE( CMD_LoadMatrix, "Matrix",
      "Specify a transformation matrix associated with this entity", MATRIX, RwMatrix, DEFAULT(0))
   RWS_ATTRIBUTE( CMD_TEXTCMD, "Text here", "Type text here.", EDIT, RwChar, DEFAULT("Enter Text Here ..."))
   RWS_ATTRIBUTE( CMD_MLTEXTCMD, "Text here", "Type text here.",
      TEXTEDIT, RwChar, DEFAULT("Enter Text Here ..."))

   \endverbatim
*
*/
#define RWS_ATTRIBUTE(id, short_txt, long_txt, type, data_type, init_data)

#else

#define RWS_ATTRIBUTE(id, short_txt, long_txt, type, data_type, init_data)  id,

#endif

#ifdef DOXYGEN
/**
*
*  \ingroup Forms
*
*  This is the same as \ref RWS_ATTRIBUTE except that the attribute cannot be shared within the workspace.
*
*/
#define RWS_PRIVATE_ATTRIBUTE(id, short_txt, long_txt, type, data_type, init_data)

#else

#define RWS_PRIVATE_ATTRIBUTE(id, short_txt, long_txt, type, data_type, init_data)  id,

#endif

/**
*
*  \ingroup Forms
*
* Define a Separator
*
* \li s Name Of Separator 
* \li p Initial state\n
*    0 Closed\n
*    1 Open\n
*
* Example 
*
*  \verbatim

   RWS_SEPARATOR("Strafe Movement Attributes", 1)
   
   \endverbatim
*
*/

#define RWS_SEPARATOR(s, p) 

/**
*
*  \ingroup Forms
*
* Re-enable auto form generation \ref RWS_BEGIN_MACRO_DEFINITION
*
*/

#define RWS_END_MACRO_DEFINITION

/**
*
*  \ingroup Forms
*
* Specify that the attributes of this class should be included as data associated with this class.
* If a class is derived from a base class then the attributes of the base class are concatenated with
* those of the derived class. This macro can be used to force this concatenation without deriving from
* the base class.
*
*/
#define RWS_IMPORT_COMMANDS(class_identifier) enum{ObFuScAtE_##class_identifier = RWFORCEENUMSIZEINT}

/**
*
*  \ingroup Forms
*
*  Provide a category for the behavior, used by the workspace to sort behaviors into groups.
*
*/
#define RWS_CATEGORY(description) enum{ObFuScAtE_CATEGORY = RWFORCEENUMSIZEINT}

/**
*
*  \ingroup Forms
*
*  Provide a short and long description for a behavior, used by the workspace to document behaviors.
*
*/
#define RWS_DESCRIPTION(short_description, long_description) enum{ObFuScAtE_DESCRIPTION = RWFORCEENUMSIZEINT}

/*****************************************************************************
 *
 * The following section defines the macros and supporting code for the direct-set
 * attribute table mechanism. The direct-set attribute table mechanism can be used
 * by code running on the console to directly manipulate attributes of an
 * entity or call update functions within the behavior's code.
 *
 *****************************************************************************/

/**
*
* Used with the "RWS_CMD_TABLE_ENTRY" and "RWS_CMD_TABLE_OVERRIDE_ENTRY" macros. This informs the
* system that there is no data offset for this table entry and that 'direct-set' should not be used
* to write to the behaviour instance's memory.
*
*/

#define RWS_CMD_DATA_BLANK             0x80000000

/**
*
* Used with the "RWS_CMD_TABLE_ENTRY" macro ONLY. Do NOT use for the "RWS_CMD_TABLE_OVERRIDE_ENTRY" macro.
* This informs the system that there is a data offset for this table entry and that 'direct-set' should
* be used to write to the behaviour instance's memory. Also builds the offset for the memory location.
*
* NOTE: Can use with "RWS_CMD_TABLE_OVERRIDE_ENTRY" ONLY if the behavior name is the one which is being
*       overridden and the variable is also in the behavior being overridden. If either are the top level
*       behaviour or any other behavior in the inheritance tree the system will be HIGHLY LIKELY to cause
*       CORRUPTION when used.
*
* \param behaviour The behaviour which this direct-set table belongs to ONLY. This is why this macro
*        CANNOT be used with the "RWS_CMD_TABLE_OVERRIDE_ENTRY" - because this macro is to override the
*        settings for a DIFFERENT behaviour. The problem come in that the system will be supplied with
*        the 'this' pointer for the behaviour and NOT for the overridden behaviour - therefore the
*        offset is highly likely to be incorrect and 'direct-setting' will cause corruption instead of
*        setting the variable correctly.
* \param var The variable to be written directly into, just it's name, do NOT use '&' before the name.
*
*/

#define RWS_CMD_DATA(behavior, var)   reinterpret_cast<RwUInt32>(&((behavior*)0)->var)

/**
*
* Used with the "RWS_CMD_TABLE_ENTRY" and "RWS_CMD_TABLE_OVERRIDE_ENTRY" macros. This informs the
* system that there is no update function for this table entry.
*
*/

#define RWS_CMD_FUNC_BLANK             0

/**
*
* Used with the "RWS_CMD_TABLE_ENTRY" as well as the "RWS_CMD_TABLE_OVERRIDE_ENTRY" macros. This informs
* the system that there is an update function for this table entry. The function can exist in this
* behaviour or any other behaviour. This is because these update functions are static functions. However,
* the function will be supplied with a pointer to the instance to be updated (as a void *) and so the
* static function MUST handle this correctly, ie you cannot call a function which casts the pointer to
* the wrong class.
*
* \param behavior The name of the behavior which holds the static function to be called (can be any).
* \param func The name of the static function within that behavior to call (can be any that take the
*             from the behavior).
*
*/

#define RWS_CMD_FUNC(behavior, func)  (behavior::func)

/**
*  
* Type for attribute table entries function pointers.
*
*/
typedef void (*CAttributeTableEntryUpdateFunction)(void *pThis, const void *pData);

/**
*
* Type used for direct-set system's post update function pointers.
*
*/
typedef void (*CAttributeTablePostUpdateFunction)(void *pThis);

/**
*
* Used to hold a data offset and function pointer used to update an instance of a behavior
* directly, instead of through the attribute packet system. The data offset is from the 'this'
* pointer of the instance. The function pointer will be passed the 'this' pointer for the
* instance.
*
* To get a pointer to a variable in memory (m_dataOffset is NOT RWS_CMD_DATA_BLANK), assuming the
* variable is an RwUInt32...
*
* RwUInt32 *pUInt = reinterpret_cast<RwUInt32 *>(reinterpret_cast<RwUInt8 *>(this) + m_dataOffset +
*                                                m_thisOffset);
*
* ...To calculate the correct 'this' pointer to pass to the m_pFunc...
*
* void *pRealThis = reinterpet_cast<void *>(reinterpret_cast<RwUInt8 *>(this) + m_thisOffset);
*
* ...Note: In many cases 'm_thisOffset' will be zero, however, do NOT remove it from the calculations
* otherwise you may end up with a difficult to track down memory corruption issue when inheritance is used.
*
*/
class CAttributeTableEntry
{
public:

   RwUInt32 m_dataOffset;                          /**< Holds the 'this' pointer offset needed to get the
                                                        memory location of where the data variable is or
                                                        RWS_CMD_DATA_BLANK if none. See 'm_thisOffset'
                                                        as this may need to be adjusted. Add this value. */
   CAttributeTableEntryUpdateFunction m_pFunc;     /**< Points to function to be called when data changes or
                                                        RWS_CMD_FUNC_BLANK if there isn't one. */
   RwInt32 m_thisOffset;                           /**< During some types of overloading (most noticable in
                                                        multiple-inheritance) the this pointer will needed to
                                                        be adjusted before calling functions in the none
                                                        overridden base class. This value is used to correct
                                                        the this pointer in these cases / pointer to
                                                        variables. Add this value. */
   /**
   *
   * Constructor initialises the data offset and function pointers to safe, non-set, values.
   *
   */

   CAttributeTableEntry(void) :
      m_dataOffset(RWS_CMD_DATA_BLANK),
      m_pFunc(RWS_CMD_FUNC_BLANK),
      m_thisOffset(0) {}
};

/**
*
* Class used to hold flags indicating if an attribute table has been built or not.
* This is necessary as the flags MUST be defaulted to FALSE and not left in an
* undetermined state.
*
*/
class CAttribTableBuiltFlag
{
public:

   RwBool m_flag;    /**< Flag to indicate if table has been built yet or not. */

   /**
   *
   * Constructor initialises the flag to FALSE, so it is in a known state.
   *
   */

   CAttribTableBuiltFlag(void) :
      m_flag(FALSE) {}
};

//-------------------------------------
// Direct-set macros for the .h file...
//-------------------------------------

/**
*
* This is the first macro for the .h file and is required. There are a series of macros which
* must be used to setup the direct-set functionality. These are as follows (in the order in
* which they MUST be used)...
*
* RWS_DS_TABLE_START                         // Start table - REQUIRED.
*
*    RWS_DS_ENABLE_OVERRIDE(...)             // Optional, one for each behaviour in the
*                                            // hierarchy which is to be fully or partly
*                                            // overridden. See the RWS_DS_REGISTER_OVERRIDE
*                                            // macro. Add all needed then...
*
*    RWS_DS_START_CMD_SECTION                // Starts the list of commands which are to
*                                            // be direct-set in this behavior - REQUIRED.
*
*       RWS_DS_CMD(...)                      // One for each attribute to be direct-set.
*
*    RWS_DS_END_CMD_SECTION(...)             // Marks end of the list of commands to
*                                            // direct-set - REQUIRED.
*
*    RWS_DS_START_OVERRIDE_SECTION(...)      // Optional, but one is REQUIRED for each
*                                            // ..._ENABLE_OVERRIDE macro used. Each
*                                            // overridden behavior is listed in turn
*                                            // and consists of a START, one or more
*                                            // CMD macros and then a END macro. After
*                                            // the END macro, then next overridden
*                                            // behavior's START macro can be inserted
*                                            // and so on.
*
*       RWS_DS_OVERRIDE_CMD(...)             // Optional, see ..._START_OVERRRIDE_SECTION.
*                                            // Indicates the attributes to be overridden
*                                            // in the overridden behavior. Insert all that
*                                            // are needed and then...
*
*    RWS_DS_END_OVERRIDE_SECTION(...)        // Indicates the end of the overrides for the
*                                            // specified behavior - see the
*                                            // ...START_OVERRIDE_SECTION.
*
* RWS_DS_TABLE_END                           // End table - REQUIRED.
*
* ...This particular macro notes the start of the direct-set table.
*
*/

#define RWS_DS_TABLE_START                                                                \
                                                                                          \
         class ObFuScAtE_AttributeTable;                                                  \
         friend class ObFuScAtE_AttributeTable;                                           \
                                                                                          \
         class ObFuScAtE_AttributeTable                                                   \
         {                                                                                \
         private:                                                                         \
                                                                                          \
            static struct data_tables                                                     \
            {                                                                             \
               CAttributeTableEntry m_thisTable[SIZE_ATTRIBUTE];

/**
*
* Specifies a behavior in the hierarchy which is to be overridden. This is used in
* combination with the RWS_DS_START_OVERRIDE_SECTION, RWS_DS_OVERRIDE_CMD and
* RWS_DS_END_OVERRIDE_SECTION macros.
*
* \param overriddenBehavior The class name of the behavior being overridden.
*
*/

#define RWS_DS_ENABLE_OVERRIDE(overriddenBehavior)                                        \
                                                                                          \
               CAttributeTableEntry m_##overriddenBehavior##Table[                        \
                  ##overriddenBehavior##::SIZE_ATTRIBUTE];                                \
               CAttribTableBuiltFlag m_##overriddenBehavior##Built;

/**
*
* Sets the start of the command section for this behaviour.
*
*/

#define RWS_DS_START_CMD_SECTION                                                          \
                                                                                          \
            }                                                                             \
            sm_dataTables;                                                                \
                                                                                          \
         public:                                                                          \
                                                                                          \
            ObFuScAtE_AttributeTable(void)                                                \
            {

/**
*
* Builds the data offset or sets the static function to be called for the specified
* attribute within this behavior.
*
* \param attr The enum name of the attribute.
* \param dataVar The offset to the data variable, built using either the
*                RWS_CMD_DATA or RWS_CMD_DATA_BLANK macros.
* \param staticFunc The static function to be called, built using either the
*                RWS_CMD_FUNC or RWS_CMD_FUNC_BLANK macros.
*
*/

#define RWS_DS_CMD(attr, dataVar, staticFunc)                                             \
                                                                                          \
               sm_dataTables.m_thisTable[attr].m_dataOffset = dataVar;                    \
               sm_dataTables.m_thisTable[attr].m_pFunc = staticFunc;


/**
*
* Ends the command section for the behavior having the direct-set table built for it.
*
* \param behavior The name of this behavior, the one the direct-set table is being
*                 setup for.
*
*/

#define RWS_DS_END_CMD_SECTION(behavior)                                                  \
                                                                                          \
            }                                                                             \
                                                                                          \
            static CAttributeTableEntry *GetTable(const RwChar *classID,                  \
               RwBool ignoreTopLevel, RwUInt32 &tableSize)                                \
            {                                                                             \
               CAttributeTableEntry *pRet = 0;                                            \
                                                                                          \
               if (!ignoreTopLevel && !rwstrcmp(classID, #behavior))                      \
               {                                                                          \
                  pRet = sm_dataTables.m_thisTable;                                       \
                  tableSize = SIZE_ATTRIBUTE;                                             \
               }

/**
*
* Starts a table of commands to override in the specified overridden behavior. For this to
* work a RWS_DS_ENABLE_OVERRIDE must be defined for the overridden behavior and the .cpp
* fill should contain a RWS_DS_REGISTER_BASE if necessary (see macro's help).
*
* \param overriddenBehavior The class name of the behaviour (within the hierarchy) with
*                           is to be overridden.
* \param behavior The class name of the behaviour which is doing the overriding.
*
* NOTE: code needs to build the 'this offsets' for the class which is being overridden as it's
*       this pointer will (under some circumstances) not be the same as the this point of the
*       inherited class. This is problematic, as these may already have been overridden by the
*       behaviour which is being overridden here. Therefore offsets have to be cumulative and not
*       set to an absolute value.
*/

#define RWS_DS_START_OVERRIDE_SECTION(overriddenBehavior, behavior)                       \
                                                                                          \
               if (!pRet && !ignoreTopLevel && !rwstrcmp(classID, #overriddenBehavior))   \
               {                                                                          \
                  if (!sm_dataTables.m_##overriddenBehavior##Built.m_flag)                \
                  {                                                                       \
                     CAttributeTableEntry *pSubTable;                                     \
                     RwUInt32 void32;                                                     \
                     void *pVoidThis;                                                     \
                     pSubTable = GetThisAttribTable(classID,                              \
                        void32, pVoidThis, TRUE, 0);                                      \
                                                                                          \
                     if (pSubTable)                                                       \
                     {                                                                    \
                        RwUInt32 index;                                                   \
                        overriddenBehavior *pOverridden =                                 \
                           reinterpret_cast<##overriddenBehavior## *>(0x80000000);        \
                        behavior *pBehavior = static_cast<##behavior## *>(pOverridden);   \
                        RwInt32 offset = reinterpret_cast<RwInt32>(pOverridden) -         \
                           reinterpret_cast<RwInt32>(pBehavior);                          \
                                                                                          \
                        memcpy(sm_dataTables.m_##overriddenBehavior##Table,               \
                           pSubTable, overriddenBehavior##::SIZE_ATTRIBUTE *              \
                           sizeof(CAttributeTableEntry));                                 \
                                                                                          \
                        for (index = 0; index < overriddenBehavior##::SIZE_ATTRIBUTE;     \
                           index++)                                                       \
                        {                                                                 \
                           sm_dataTables.m_##overriddenBehavior##Table[index].            \
                              m_thisOffset += offset;                                     \
                        }                                                                 \
                     }

/**
*
* Indicates that an attribute within the overridden behavior is to be overridden. It is
* possible to disable the direct-set for a overridden behaviour. Say it normally sets a
* variable and calls an update function. If the ..._BLANK macros are used, then this
* behavior would be disabled in the inherited behavior.
*
* NOTE: ONLY the RWS_CMD_DATA_BLANK macro can be used for data. It is not possible to
*       override the direct-set data write, as it uses the original class's this poiter
*       and the offset would be incorrectly calculated and result in corruption.
*
* \param overridenBehavior The class name of the behavior in the hierarchy which is being
*                          overridden.
* \param attr The enum name of the attribute in the overridden behaviour. Only the attribute
*             enum is specified, there is no need to add the class name, ie <class>::<attr>
*             as this is performed automatically.
* \param dataVar The data offset macro. THIS MUST BE the "RWS_CMD_DATA_BLANK" macro, if not
*                corruption of the behavior instance's memory may occur (is undefined).
* \param staticFunc The update function to be called, use either the RWS_CMD_FUNC or
*                   RWS_CMD_FUNC_BLANK macros.
*
*/

#define RWS_DS_OVERRIDE_CMD(overridenBehavior, attr, dataVar, staticFunc)                 \
                                                                                          \
                     sm_dataTables.m_##overridenBehavior##Table[                          \
                        ##overridenBehavior##::attr].m_dataOffset = dataVar;              \
                     sm_dataTables.m_##overridenBehavior##Table[                          \
                        ##overridenBehavior##::attr].m_pFunc = staticFunc;                \
                     sm_dataTables.m_##overridenBehavior##Table[                          \
                        ##overridenBehavior##::attr].m_thisOffset = 0;

/**
*
* This macro denotes the end of an override section for a base class within the behavior's
* class hierarchy.
*
* \param overriddenBehavior The class name of the behavior in the class hierarchy of this
*                           behavior.
*
*/

#define RWS_DS_END_OVERRIDE_SECTION(overriddenBehavior)                                   \
                                                                                          \
                     sm_dataTables.m_##overriddenBehavior##Built.m_flag = TRUE;           \
                  }                                                                       \
                                                                                          \
                  pRet = sm_dataTables.m_##overriddenBehavior##Table;                     \
                  tableSize = ##overriddenBehavior##::SIZE_ATTRIBUTE;                     \
               }

/**
*
* Denotes the end of the direct-set table for this behavior.
*
*/

#define RWS_DS_TABLE_END                                                                  \
                                                                                          \
               return(pRet);                                                              \
            }                                                                             \
         };                                                                               \
                                                                                          \
         virtual CAttributeTableEntry *GetAttributeTable(const RwChar *classID,           \
            RwUInt32 &rNumEntries, void *&rpThis, RwBool ignoreTopLevel = FALSE);         \
                                                                                          \
         static CAttributeTableEntry *GetThisAttribTable(const RwChar *classID,           \
            RwUInt32 &rNumEntries, void *&rpThis, RwBool ignoreTopLevel,                  \
            void *pInstance);

/**
*
* Used to specify the function to be called after ALL the updates to the behavior have been
* made. This function does not have to exist, so the macro is optional.
*
* NOTE: despite the 'register' nature of this macro it is used in the HEADER file.
*
* \param behaviorName The class name of the behavior for which the post-update function is
*                     being specified.
* \param postUpdateStaticFunc The static post-update function to call. This will have the
*                             'this' pointer of the behavior instance passed in.
*
*/

#define RWS_REGISTER_DS_POST_UPDATE_FUNC(behaviorName, postUpdateStaticFunc)              \
                                                                                          \
         virtual CAttributeTablePostUpdateFunction GetAttrPostUpdateFunc(void *&rpThis)   \
            {                                                                             \
            rpThis = this;                                                                \
            return(##behaviorName##::postUpdateStaticFunc);                               \
            }

//-----------------
// For .cpp file...
//-----------------

/**
*
* Start the macros needed for the .cpp code file for direct-set functionality. The macro
* layout within the file is as follows...
*
* RWS_DS_REGISTER_START(...)                    // REQUIRED - begins the registration.
*
*    RWS_DS_REGISTER_BASE(...)                  // Optional, but MUST exist for any base
*                                               // behavior (directly inherited from) that
*                                               // has direct-set defined, otherwise the
*                                               // override and direct-set functionality
*                                               // will not operate correctly. However, do
*                                               // NOT add if the base behavior does NOT
*                                               // have a direct-set table set up. Add as
*                                               // many of these are required, then...
*
* RWS_DS_REGISTER_END                           // REQUIRED - ends the registration.
*
* \param behaviorName The class name of this behavior.
*
*/

#ifdef RWS_DS_DEBUG        // Define to get debug information set to console for direct-set.

#define RWS_DS_REGISTER_START(behaviorName)                                               \
                                                                                          \
         behaviorName##::ObFuScAtE_AttributeTable::data_tables behaviorName##::           \
            ObFuScAtE_AttributeTable::sm_dataTables;                                      \
                                                                                          \
         static behaviorName##::ObFuScAtE_AttributeTable                                  \
            g##behaviorName##ObFuScAtEattribTable;                                        \
                                                                                          \
         CAttributeTableEntry * behaviorName##::GetAttributeTable(                        \
            const RwChar *classID, RwUInt32 &rNumEntries,                                 \
            void *&rpThis, RwBool ignoreTopLevel)                                         \
         {                                                                                \
            return(GetThisAttribTable(classID, rNumEntries, rpThis,                       \
               ignoreTopLevel, this));                                                    \
         }                                                                                \
                                                                                          \
         CAttributeTableEntry * behaviorName##::GetThisAttribTable(                       \
            const RwChar *classID, RwUInt32 &rNumEntries,                                 \
            void *&rpThis, RwBool ignoreTopLevel, void *pInstance)                        \
         {                                                                                \
            CAttributeTableEntry *pRet = behaviorName##::ObFuScAtE_AttributeTable::       \
               GetTable(classID, ignoreTopLevel, rNumEntries);                            \
                                                                                          \
            RWS_DEBUGSTREAM("Attrib table, searching [" << #behaviorName <<               \
               "]" << std::endl);                                                         \
                                                                                          \
            if (pRet)                                                                     \
            {                                                                             \
               rpThis = pInstance;                                                        \
            }

#else       // #ifdef RWS_DS_DEBUG

#define RWS_DS_REGISTER_START(behaviorName)                                               \
                                                                                          \
         behaviorName##::ObFuScAtE_AttributeTable::data_tables behaviorName##::           \
            ObFuScAtE_AttributeTable::sm_dataTables;                                      \
                                                                                          \
         static behaviorName##::ObFuScAtE_AttributeTable                                  \
            g##behaviorName##ObFuScAtEattribTable;                                        \
                                                                                          \
         CAttributeTableEntry * behaviorName##::GetAttributeTable(                        \
            const RwChar *classID, RwUInt32 &rNumEntries,                                 \
            void *&rpThis, RwBool ignoreTopLevel)                                         \
         {                                                                                \
            return(GetThisAttribTable(classID, rNumEntries, rpThis,                       \
               ignoreTopLevel, this));                                                    \
         }                                                                                \
                                                                                          \
         CAttributeTableEntry * behaviorName##::GetThisAttribTable(                       \
            const RwChar *classID, RwUInt32 &rNumEntries,                                 \
            void *&rpThis, RwBool ignoreTopLevel, void *pInstance)                        \
         {                                                                                \
            CAttributeTableEntry *pRet = behaviorName##::ObFuScAtE_AttributeTable::       \
               GetTable(classID, ignoreTopLevel, rNumEntries);                            \
                                                                                          \
            if (pRet)                                                                     \
            {                                                                             \
               rpThis = pInstance;                                                        \
            }

#endif      // #ifdef RWS_DS_DEBUG

/**
*
* Defines a base behavior of this behavior THAT HAS A DIRECT-SET TABLE. Do NOT use if the
* base behavior does not have one set up.
*
* \param behaviour 
* \param baseBehavior 
*
*/

#define RWS_DS_REGISTER_BASE(behaviour, baseBehavior)                                     \
                                                                                          \
            if (!pRet)                                                                    \
            {                                                                             \
               behaviour *pInstThis = reinterpret_cast< behaviour *>(pInstance);          \
                                                                                          \
               pRet = baseBehavior##::GetThisAttribTable(classID, rNumEntries, rpThis,    \
                  FALSE, static_cast< baseBehavior *>(pInstThis));                        \
            }

/**
*
* Denotes the end of the direct-set registration in the .cpp file.
*
*/

#define RWS_DS_REGISTER_END                                                               \
                                                                                          \
            return(pRet);                                                                 \
         }

#endif
