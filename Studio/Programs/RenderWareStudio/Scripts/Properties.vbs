
' Properties.vbs

' This file is auto-generated from RWSHints.h and contains the
' list of RenderWare Studio reserved properties and associated
' helper functions.

Option Explicit ' All variables must be explicitly declared

'System flags for use by the custom properties
Const sysflagSaveProperty     = 0
Const sysflagDontSaveProperty = 1
Const sysflagCopyProperty     = 2

'Flags for the entity display options property
Const dispOptionsFlags_Freeze = 1

' Properties
Const conVersionControlStatus = "RWSPROP_VERSIONCONTROLSTATUS" ' Version control status of files.
Const conResourceStatus = "RWSPROP_RESOURCESTATUS" ' Version control status of files.
Const conStreamEditData = "RWSPROP_STREAMEDIT_DATA" ' Stream editor data
Const conHullShape = "RWSPROP_HULL_SHAPE" ' Hull shape property
Const conHullTexture = "RWSPROP_HULL_TEXTURE" ' Hull texture property
Const conLightMapLight = "RWSPROP_LIGHTMAPPER_LIGHT" ' LightMapTool light entity
Const conEntityDisplayOptions = "RWSPROP_ENTITYDISPLAY_OPTIONS" ' Entity display options
Const conVersionControlUsername = "RWSPROP_VERSIONCONTROLUSERNAME" ' The username the user is connected as.

' Constants defining the data types that a property can hold
const conDTBool   = 0
const conDTInt32  = 1
const conDTUInt32 = 2
const conDTReal   = 3
const conDTDouble = 4
const conDTByte   = 5
const conDTChar   = 6

