/*****************************************************************************
 *
 * File :     RWS3Dext.h
 *
 * Abstract : Design View extension interfaces.
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

/**
\mainpage Introduction
This C++ API allows you to extend the <a href="ms-its:RWStudio.chm::/ui_gameview.htm">Design View</a> window
of the RenderWare Studio Workspace.
For example, you can add geometry to the Design View window,
and get notification when the Workspace user picks an object in the Design View window.

Any geometry generated with this API resides entirely in Workspace
and is not downloaded to the target console.
*/

/**
\defgroup RWS3DExtObject Adding geometry
*/

/*@{*/

#ifndef __RWS3DEXT_H__
#define __RWS3DEXT_H__

// Included for 'RWS....' types
#include "rwscore.h"

class CRWS3DExtObject;

/////////////////////////////////////////////////////////////////////////////////////////////
//
// Types used by objects
//

/**
Represents a 3D point or vector.
*/
struct RWSPosition3D
{
   RWSPosition3D () : x(0.0f), y(0.0f), z(0.0f) {} ;
   RWSPosition3D (float _x, float _y, float _z) : x(_x), y(_y), z(_z) {} ;
   RWSReal x;  /**< X coordinate */
   RWSReal y;  /**< Y coordinate */
   RWSReal z;  /**< Z coordinate */
};

typedef struct RWSPosition3D RWSVector3D;



/**
Represents a color + alpha value.
*/
struct RWSColorAlpha
{
   RWSColorAlpha () : r(1.0f), g(1.0f), b(1.0f), a(1.0f) {};
   RWSReal r;  /**< Red components range 0 to 1.0 */
   RWSReal g;  /**< Green components range 0 to 1.0 */
   RWSReal b;  /**< Blue components range 0 to 1.0 */
   RWSReal a;  /**< Alpha components range 0 to 1.0 */
};

/**
Represents a 4x4 transformation matrix.
*/
struct RWSMatrix4
{
   RWSReal m[4][4];  /**< Two-dimensional array of reals containing the matrix elements */

   RWSMatrix4()
   {
      // Initially, set up to be unit matrix
      m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.0f;
      m[0][1] = m[0][2] = m[0][3] = 0.0f;
      m[1][0] = m[1][2] = m[1][3] = 0.0f;
      m[2][0] = m[2][1] = m[2][3] = 0.0f;
      m[3][0] = m[3][1] = m[3][2] = 0.0f;
   };
};


/**
Represents the result of ray-testing or line-testing the 3D scene.
The result of ray/line testing the scene will be either:
\li An RWSID of an <a href="ms-its:RWStudioManagerAPI.chm::/structRWSEntity.htm">RWSEntity</a> or
\li An IRWS3DExtObject.
*/
struct RWSRayTestResult
{
   RWSPosition3D    ix_point;   /**< Intersection point, in world coordinates */
   RWSVector3D      normal;     /**< Face normal at intersection point */
   RWSID            rwsID;      /**< RWSID of entity which has been intersected. */
   CRWS3DExtObject *pElement;   /**< Pointer to  CRWS3DExtObject which has been intersected. */
};


/////////////////////////////////////////////////////////////////////////////////////////////
//
// RWS3DExtObject -
//

/**
Indicates the cause of a callback.
*/
typedef enum { 
   TRWS3DPicked = 0, /**< Object has been picked, the user has picked the object in the Design View.*/
   TRWS3DSelected,   /**< Object has been selected, edit handles have been attached to the object.*/
   TRWS3DDeselected, /**< Object has been deselected, edit handles have been removed from the object. */
   TRWS3DMoving,     /**< Object has moved. */
   TRWS3DMoved       /**< Object has finished moving. */
}  CallbackReason;

/**
Definition of an IRWS3DExtObject callback function.
\param object Pointer to an IRWS3DExtObject object.
\param callback_reason Indicates the cause of the callback.
*/
typedef void (*RWS3DExtObjectEventCB) (struct IRWS3DExtObject *object, CallbackReason callback_reason);



/**
Specifies axis alignment when creating a cone object.
\see IRWS3DExtObject::AddCone
*/
typedef enum { 
   PositiveXAxis = 0,   /**< Align object along the positive X axis. */
   NegativeXAxis,       /**< Align object along the negative X axis. */
   PositiveYAxis,       /**< Align object along the positive Y axis. */
   NegativeYAxis,       /**< Align object along the negative Y axis. */
   PositiveZAxis,       /**< Align object along the positive Z axis. */
   NegativeZAxis        /**< Align object along the negative Z axis. */
} AxisAlignment;


/**
Object property flag: object is visible in view.
\see IRWS3DExtObject::SetProperties, IRWS3DExtObject::InqProperties.
*/
const RWSUInt32 RWS3DExtVisible    = 0x01;   // visible in view

/**
Object property flag: object is pickable in view (generating a pick event).
\see IRWS3DExtObject::SetProperties, IRWS3DExtObject::InqProperties.
*/
const RWSUInt32 RWS3DExtPickable   = 0x02;   // can be picked by raytest

/**
Object property flag: edit handles can be attached in view.
\see IRWS3DExtObject::SetProperties, IRWS3DExtObject::InqProperties.
*/
const RWSUInt32 RWS3DExtSelectable = 0x04;   // can have edit handles attached to it

/**
Object property flag: object is always displayed as wireframe.
\see IRWS3DExtObject::SetProperties, IRWS3DExtObject::InqProperties.
*/
const RWSUInt32 RWS3DExtWireframe = 0x08;    // Is always displayed wirefrmae

struct __declspec (uuid ("6B5A984A-D85E-4A14-A514-256F1670E534"))
RWS3DExtObject;

/**
The 3D extensions interface provides a simple method of extending Design View, allowing developers to add
simple primitives such as lines, polygons, boxes, and spheres. In addition, notification of 
events within the views such as picking with the mouse pointer are supported.

Here is an example of creating an instance of IRWS3DExtObject:
\code
CLSID CLSID_RWS3DExtObject = __uuidof(RWS3DExtObject);
IID IID_IRWS3DExtObject  = __uuidof(IRWS3DExtObject);

IRWS3DExtObject *pTestObject = 0;

HRESULT hr = CoCreateInstance (CLSID_RWS3DExtObject, 0, CLSCTX_SERVER, IID_IRWS3DExtObject,
										 reinterpret_cast<void **>(&pTestObject));
if (SUCCEEDED(hr))
{
	// Use the IRWS3DExtObject object
	//
	// pTestObject->AddPolyline(...)
	// pTestObject->AddSphere(...)
	// pTestObject->AddBlock(...)

	// When geometry creation in complete, need to lock the geometry so
	// that it can be rendered. After this, if we need to change the
	// geometry, it will have to be unlocked again.
	
	pTestObject->LockGeometry()
}
\endcode
and to destroy the object:
\code
pTestObject->Release();
\endcode
Objects created are automatically added to the 3D world, but initially have no geometry
so nothing will be seen. Once an instance of the IRWS3DExtObject has been created methods
are provided for adding lines, spheres, blocks etc to the object which will then be drawn in
Design View. Any geometry added to the object can later be removed by calling the
ClearGeometry() method.

Initially, the geometry is created in an 'unlocked' state. In this state, you can change the
geometry (add polygons, etc.), but it will not be rendered. In order to render the geometry,
you need to call the LockGeometry() method. Once locked, if you need
to change the geometry later, you need to call the UnlockGeometry()
method.
Geometry is added in the local coordinate space of the object; this is combined with the 
world coordinates of the object when it is displayed. For example, if we were creating spheres
then we would probably want to keep the center of the sphere at the local origin (0, 0, 0), and
position the sphere in the world using the objects world coordinates.

The object's world coordinates are accessed using the following methods of a IRWS3DExtObject:
\code
void GetTransform(RWSMatrix4 &xform);
void SetTransform(const RWSMatrix4 &xform);
\endcode
When an object is selected in the view, and dragged about using the edit handles, it is
the object's world coordinates or transformation that is being modified.

A IRWS3DExtObject also has properties which control how the user can interact with the object
within a 3D view. These properties can control whether the object is visible,
pickable or selectable. If the object is pickable it means that the user can click on the object
in the 3D view generating a pick event. Similarly if the object is selectable it means that the
standard edit handles can be attached to the object when it is picked and used to move and rotate
the object.

A callback can be specified, when the pick, select, or move events occur. Here is an
example of an RWS3DExtObjectEventCB:\n
\code
void ObjectCallback (IRWS3DExtObject* object, CallbackReason callback_reason)
{
	switch (callback_reason)
	{
		case (TRWS3DPicked) :   // This IRWS3DExtObject has been picked.
		break;

		case (TRWS3DSelected) : // This IRWS3DExtObject has been selected.
		break; 
		 
		case (TRWS3DDeselected) : // This IRWS3DExtObject has been deselected.
		break;  

		case (TRWS3DMoving) :   // This IRWS3DExtObject is being moved.
		break;

		case (TRWS3DMoved) :    // This IRWS3DExtObject has finished moving
		break;

	}                               
}
\endcode
and to setup the callback for the object:
\code
pTestObject->SetEventCallback(ObjectCallback);
\endcode
TRWS3DMoving event occurs when the transformation of the object has been changed - for example, if the user moves the object 
using the edit handles. For any object to be moved by the edit handles, it must have the pickable and 
selectable properties set (::RWS3DExtPickable and ::RWS3DExtSelectable).
The TRWS3Moved event occurs when the user has finished moving an object (e.g., they have stopped using the edit handles).
Arbitary data can be associate with an object for example to maintain a unique ID, the following methods are provided
to attach, remove and obtain this user data.
\code
void  SetUserData (void *data);
void *RemoveUserData (void);
void *InqUserData (void);
\endcode
The user data is just a void *, and can be used for any purpose. If the developer uses it
to store a pointer to a block of memory, then it is the developers responsibility to manage 
the memory that it points to i.e. when the IRWS3DExtObject is released this memory isn't automatically
returned to the system.

If you need many extension objects, which have the same geometry but different transformations, then
you can create 'instances' of existing objects. These share the same geometry (so adding polygons
to one object will be seen on all the instances created from it), but can have their own transformation
and properties. This can improve both performance when creating many object, and reduce memory requirements.

To create an instance, just call the CreateInstance() method:
\code
IRWS3DExtensionObject *pTestInstance = pTestObject->CreateInstance();

if (pInstance)
{
	// Change the transform / properties on pInstance
	pTestInstance->SetTransform (pTranform)
}
\endcode
and to destroy the instance, simply release the pointer:
\code
pTestInstance->Release();
\endcode
*/
#ifdef DOXYGEN
// Allow doxygen to pickup class definitions.
//
class IRWS3DExtObject : public IUnknown
{
   public:
#else
// Real definition of class.
//
interface __declspec (uuid ("ABB7697C-2E29-44E9-9007-E9D68ACD26D2"))
IRWS3DExtObject : public IUnknown
{
#endif
   // Geometry creation / destruction

   /**
   Removes all geometry previously added to this object.
   */
   STDMETHOD_(void, ClearGeometry)(void) = 0;

   /**
   Locks the geometry, so it can be rendered. Once locked, geometry cannot be altered unless
   you first call UnlockGeometry().
   */

   STDMETHOD_(void, LockGeometry)(void) = 0;

   /**
   Unlocks the geometry, so that it can be altered (e.g. more polygons added). Unlocked geometry
   will not be rendered, so you need to call LockGeometry() in order to see it.
   */

   STDMETHOD_(void, UnlockGeometry)(void) = 0;

   /**
   Adds a line made of a number of line segments to this object., a minimum of two points are required to form a line
   \param pts Pointer to an array of RWSPosition3D objects containing the coordinates of the end points of the line segments.
   \param n The number of points in the array pointer to by pts.
   \param color A reference to a RWSColorAlpha object. Specifies the color of the polyline.
   */
   STDMETHOD_(void, AddPolyline)(const RWSPosition3D *pts, RWSUInt32 n, RWSColorAlpha &color) = 0;

   /**
   Adds a polygon consisting of a number of edges to this object..
   \param pts Pointer to an array of RWSPosition3D objects containing the coordinates of the vertices of the polygons.
   \param n The number of points in the array pointed to by pts.
   \param color A reference to a RWSColorAlpha object. Specifies the color of the polygon.
   */
   STDMETHOD_(void, AddPolygon)(const RWSPosition3D *pts, RWSUInt32 n, RWSColorAlpha &color) = 0;
   /**
   Adds an axis aligned 3D box to this object drawn as a solid.
   \param min_pt A reference to a RWSPosition3D object, the box is defined using two corners representing the minimum extent of the box
   min_pt is one of these extents.
   \param max_pt A reference to a RWSPosition3D object, the box is defined using two corners representing the maximum extent of the box, 
   max_pt is the other of these extents.
   \param color A reference to a RWSColorAlpha object, used to specify the colour of the box.
   */
   STDMETHOD_(void, AddBlock)(const RWSPosition3D &min_pt, const RWSPosition3D &max_pt, RWSColorAlpha &color) = 0;
   /**
   Adds a sphere to this object..
   \param center A reference to a RWSPosition3D object, which is the center of the sphere.
   \param radius The radius of the sphere.
   \param num_segments The number of segments to divide sphere into, typically a value of 12 produces a reasonable approximation to a sphere.
   \param col A reference to a RWSColorAlpha object, used to specify the colour of the sphere.
   */
   STDMETHOD_(void, AddSphere)(const RWSPosition3D &center, RWSReal radius, RWSUInt32 num_segments, RWSColorAlpha &color) = 0;

   /**
   Adds a (truncated) cone to this object.
   \param base_center Position of the centre of the base of the cone
   \param axis        Axis to align the cone along 0 for x, 1 for y, 2 for z
   \param base_radius Radius of base of cone
   \param top_radius  Radius of top of cone
   \param height      Height of cone
   \param cap_ends    Whether to generate capped-ends to the cone, if required.
   \param col         A reference to a RWSColorAlpha object, used to specify the colour of the cone.
   */
   STDMETHOD_(void, AddCone)(RWSPosition3D &base_center, AxisAlignment axis, 
                             RWSReal   base_radius,      RWSReal       top_radius, 
                             RWSUInt32 num_segments,     RWSReal       height, 
                             RWSBool   cap_ends,         RWSColorAlpha &color) = 0;


   /**
   Loads a RenderWarestream containing a clump, atomic, or world and adds it to this object.
   \param filename    File to load.
   \param image_path  Image path for location of textures.
   */
   STDMETHOD_(bool, LoadStream)(const RWSChar *filename,  const RWSChar *image_path) = 0;


   // Instancing

   /**
   Creates another IRWS3DExtObject which is an instance of this one. Instances will share the
   same geometry, but have their own transformation and properties.
   */
 
   STDMETHOD_(IRWS3DExtObject *, CreateInstance)(void) = 0;

   /**
   Makes this IRWS3DExtObject into an instance of pSource, so they share the same geometry.
   This clears any geometry currently being used by the object.
   \param pSource IRWS3DExtObject to share geometry with.
   */

   STDMETHOD_(void, MakeIntoInstance)(IRWS3DExtObject *pSource) = 0;

   // Properties - bitwise combination of RWS3DExt... flags above

   /**
   Inquire the object's properties ::RWS3DExtVisible, ::RWS3DExtWireframe, ::RWS3DExtPickable, ::RWS3DExtSelectable.
   \return A bitwise combination of the RWS3DExt flags.
   */
   STDMETHOD_(long, InqProperties)() = 0;
   /**
   Sets the properties of the object.
   \param new_properties A bitwise combination of the RWS3DExt flags ::RWS3DExtVisible, ::RWS3DExtWireframe, ::RWS3DExtPickable, ::RWS3DExtSelectable.
   */
   STDMETHOD_(long, SetProperties)(long new_properties) = 0;

   // Position
  /**
   Gets the current transform of the object.
   \param xform Reference to a RWSMatrix4 which is filled in with the transform of the object.
   */
   STDMETHOD_(void, GetTransform)(RWSMatrix4 &xform) = 0;
   /**
   Sets the current transform of the object.
   \param xform Reference to a RWSMatrix4.
   */
   STDMETHOD_(void, SetTransform)(const RWSMatrix4 &xform) = 0;

   // Tell the 3D view to find this object ...
   /**
   Moves the camera in the 3D view so that this object is centered in the view.
   */
   STDMETHOD_(void, Locate)(void) = 0;

   // Tell the 3D view to aim at this object ...
   /**
   Aims the camera in the 3D view so that this object is centered in the view.
   */
   STDMETHOD_(void, Aim)(void) = 0;

   // Select/deselect object in orthoviews (add/removes edit handles to/from object)
   /**
   Attaches or detaches edit handles to this IRWS3DExtObject, so the user can drag it about.
   \param bSelect Attach or detach the handles from the object.
   \param bClearCurrent Whether remove edit handles from any objects which are already
   selected, or leave them in place. (Only relevant if bSelect is TRUE.)
   */
   STDMETHOD_(void, Select)(BOOL bSelect, BOOL bClearCurrent) = 0;

   // User data
   /**
   Attaches user data to the object.
   \param data A pointer to the user data.
   */
   STDMETHOD_(void, SetUserData)(void *data) = 0;
   /**
   Removes any user data attached to the object.
   \return A pointer to the user data that has been removed otherwise 0.
   */
   STDMETHOD_(void *, RemoveUserData)(void) = 0;
   /**
   Inquire the pointer to any user data.
   \return A pointer to the user data attached to this object.
   */
   STDMETHOD_(void *, InqUserData)(void) = 0;

   // Callbacks
   /**
   Sets the event callback function for this object.
   \param cb An event callback function RWS3DExtObjectEventCB.
   */
   STDMETHOD_(void, SetEventCallback)(RWS3DExtObjectEventCB cb) = 0;
   /**
   Inquire the event callback function for this object.
   \return An event callback function RWS3DExtObjectEventCB.
   */
   STDMETHOD_(RWS3DExtObjectEventCB, InqEventCallback)(void) = 0; 

   /**
   Inquire run-time identifier for the object.
   \return A run-time identifier for the object.
   */
   STDMETHOD_(unsigned long, InqObjectID)(void) = 0; 

   
};

/*@}*/

/////////////////////////////////////////////////////////////////////////////////////////////
//
// RWS3DEventInfoObject -
//

/**
\defgroup RWS3DEventInfoObject Getting event information
*/

/*@{*/

/**
Definition of a pick event callback function.
\param object Pointer to an IRWS3DEventInfoObject object.
\param world_coord The location of the pick event, in world coordinates.
*/
typedef void (*RWS3DPickEventCB) (struct IRWS3DEventInfoObject *object, RWSPosition3D &world_coord, RWSVector3D &face_normal);

struct __declspec (uuid ("31A5E9F5-3657-4F6E-812D-59768445C5BC"))
RWS3DEventInfoObject;

/**
Allows you to attach your own "callback" function that is automatically
called when the Workspace user "picks" (left-clicks) an object in the
Design View window.
Here is an example of creating an instance of IRWS3DEventInfoObject:
\code
CLSID CLSID_RWS3DEventInfoObject = __uuidof(RWS3DEventInfoObject);
IID IID_IRWS3DEventInfoObject  = __uuidof(IRWS3DEventInfoObject);

IRWS3DEventInfoObject *pEventInfoObject = 0;

HRESULT hr = CoCreateInstance (CLSID_RWS3DEventInfoObject, 0, CLSCTX_SERVER, IID_IRWS3DEventInfoObject, 
										 reinterpret_cast<void **>(&pEventInfoObject));
if (SUCCEEDED(hr))
{
	// Use the IRWS3DEventInfoObject
	// pEventInfoObject->SetPickEventCallback(...)
}
\endcode
and to destroy the object:\n
\code
IRWS3DEventInfoObject->Release();
\endcode
Your callback function should be declared like this:
\code
void PickEventCallback (IRWS3DEventInfoObject *object, RWSPosition3D &pos)
{
	// User has picked an object in the 3D view
}
\endcode
To attach your callback function to the event, use the SetPickEventCallback method:
\code
pEventInfoObject->SetPickEventCallback (PickEventCallback);
\endcode
As with the IRWS3DExtObject, we can attach user data to this object.
*/
#ifdef DOXYGEN
// Allow doxygen to pickup class definitions.
//
class IRWS3DEventInfoObject : public IUnknown
{
   public:
#else
// Real definition of class.
//
interface __declspec (uuid ("F0FBA86D-13B7-46AF-8EDF-0D1A0C78998A"))
IRWS3DEventInfoObject : public IUnknown
{
#endif
   // User data
   /**
   Attaches user data to this object.
   \param data A pointer to the user data.
   */
   STDMETHOD_(void, SetUserData)(void *data) = 0;
   /**
   Removes user data attached to this object.
   \return A pointer to the user data that has been removed otherwise 0.
   */
   STDMETHOD_(void *, RemoveUserData)(void) = 0;
  /**
   Inquire user data attached to this object.
   \return A pointer to the attached user data if any, otherwise 0.
   */
   STDMETHOD_(void *, InqUserData)(void) = 0;

   // Callbacks
   /**
   Sets the pick event callback function for this object.
   \param cb Specifies the pick event callback function RWS3DPickEventCB.
   */
   STDMETHOD_(void, SetPickEventCallback)(RWS3DPickEventCB cb) = 0;
   /**
   Inquire the pick event callback function for this object.
   \return A pick event callback function RWS3DPickEventCB.
   */
   STDMETHOD_(RWS3DPickEventCB, InqPickEventCallback)(void) = 0;   

};

/*@}*/

/////////////////////////////////////////////////////////////////////////////////////////////
//
// RWS3DSceneControl -
//

/**
* \defgroup RWS3DSceneControl Controlling object and events in the scene
*/

/*@{*/

struct __declspec (uuid ("0F8D5A9C-BDF7-45B8-80D9-767C0A51EF6A"))
RWS3DSceneControl;

/**
This interface provides control over objects and events in the scene.

Shown below is an example of creating an instance of IRWS3DSceneControl:\n
\code
CLSID CLSID_RWS3DSceneControlt = __uuidof(RWS3DSceneControl);
IID IID_IRWS3DSceneControl  = __uuidof(IRWS3DSceneControl);

IRWS3DSceneControl *pSceneControl = 0;

HRESULT hr = CoCreateInstance (CLSID_RWS3DSceneControl, 0, CLSCTX_SERVER, IID_IRWS3DSceneControl, 
										 reinterpret_cast<void **>(&pSceneControl));
if (SUCCEEDED(hr))
{
	// Use the IRWS3DSceneControl
	// pSceneControl->SetObjectSelectEnabled(...)
}
\endcode
and to destroy the object:\n
\code
pSceneControl->Release();
\endcode
The object allows developers to stop entities in the 
view being selected by a pick (so the developer can have the pick events all to themselves).
You can enable, disable, and test object selection using the following methods:
\code
void SetObjectSelectEnabled (BOOL bEnable);
BOOL InqObjectSelectEnabled (void);
\endcode
By default, the ObjectSelect property is true, i.e. SetObjectSelectEnabled(true).

There is also an interface that allows you to enable or disable the redrawing of the 3D
views, plus a function to force them to be updated:
\code
BOOL  InqViewUpdates (void)
void  EnableViewUpdates (BOOL bEnable);
void  RefreshViews (void);
\endcode
The call to EnableViewUpdates() will not cause the views to be redrawn, so it is usually advisable
to follow it with a call to RefreshViews():
\code
 // Turn off view updates whilst we're doing a lot of processing
 pSceneControl->EnableViewUpdates (FALSE);
 
 // Do some work here
 ...

 // Turn view updates back on
 pSceneControl->EnableViewUpdates (TRUE);

 // and force a refresh
 pSceneControl->RefreshViews();
\endcode
*/
#ifdef DOXYGEN
// Allow doxygen to pickup class definitions.
//
class IRWS3DSceneControl : public IUnknown
{
   public:
#else
// Real definition of class.
//
interface __declspec (uuid ("02D92F47-81E9-4D09-9AE0-4AF0EDE5D862"))
IRWS3DSceneControl : public IUnknown
{
#endif

   // Control over the 3D view updating
   /**
   Inquire the global view update property. This property is used to control whether
   the 3D views are redrawn automatically.
   \return TRUE or FALSE depending on the current setting of the global view update property.
   */
   STDMETHOD_(BOOL, InqViewUpdates)(void) = 0;
   /**
   *
   Sets the 3D view update property. This property is used to control whether
   the 3D views are redrawn automatically. For example, temporarily disabling updates while adding
   many new objects to the scene can significantly increase performance.
   \param bEnabled Sets the global view update property.
   */
   STDMETHOD_(void, EnableViewUpdates)(BOOL bEnable) = 0;

   /**
   Refreshes the 3D views.
   */
   STDMETHOD_(void, RefreshViews)(void) = 0;

   // Control over the edit module (axis handles)
   /**
   Inquire the global object selection property. This property is used to control whether
   edit handles are attached to a picked object.
   \return TRUE or FALSE depending on the current setting of the global object selection property.
   */
   STDMETHOD_(BOOL, InqObjectSelectEnabled)(void) = 0;
   /**
   Sets the global object selection property. This property is used to control whether
   edit handles are attached to a picked object.
   \param bEnabled Sets the global object selection property.
   */
   STDMETHOD_(void, SetObjectSelectEnabled)(BOOL bEnabled) = 0;

};

/*@}*/

/////////////////////////////////////////////////////////////////////////////////////////////
//
// RWS3DSceneInquire -
//

/**
\defgroup RWS3DSceneInquire Getting object information
*/

/*@{*/

struct __declspec (uuid ("29D75F1C-2378-41C7-90B1-0B93689C508C"))
RWS3DSceneInquire;

/**
This interface provides information about objects in the scene.

Shown below is an example of creating an instance of IRWS3DSceneInquire:
\code
CLSID CLSID_RWS3DSceneInquire = __uuidof(RWS3DSceneInquire);
IID IID_IRWS3DSceneInquire  = __uuidof(IRWS3DSceneInquire);

IRWS3DSceneInquire *pSceneInquire = 0;

HRESULT hr = CoCreateInstance (CLSID_RWS3DSceneInquire, 0, CLSCTX_SERVER, IID_IRWS3DSceneInquire, 
										 reinterpret_cast<void **>(&pSceneInquire));
if (SUCCEEDED(hr))
{
	// Use the IRWS3DSceneControl
	// pSceneInquire->LineTest (...)
}
\endcode
and to destroy the object:
\code
pSceneInquire->Release();
\endcode
Once created, the object provides an interface which allows the user to get information
about the contents of the current scene:
\code
BOOL RayTest (RWSPosition3D *p, RWSVector3D *dirn, DWORD flags, RWSRayTestResult *res);
BOOL LineTest (RWSPosition3D *p, RWSPosition3D *dirn, DWORD flags, RWSRayTestResult *res);
\endcode
*/
#ifdef DOXYGEN
// Allow doxygen to pickup class definitions.
//
class IRWS3DSceneInquire : public IUnknown
{
   public:
#else
// Real definition of class.
//
interface __declspec (uuid ("55F524C9-254E-4064-8993-A3521DDFE077"))
IRWS3DSceneInquire : public IUnknown
{
#endif

   // 3D Scene query
   /**
   Fires a ray into the scene, starting at \a p in direction \a dirn, intersecting any objects in the scene
   \param p      Starting position for the ray.
   \param dirn   Direction of ray.
   \param flags  Flags to control ray test calculations (currently must be set to 0).
   \param res    Pointer to RWSRayTestResult structure, which will contain the results
   of the ray test.
   \return TRUE or FALSE depending on whether the ray hit an object.
   */
   STDMETHOD_(BOOL, RayTest)(RWSPosition3D *p, RWSVector3D *dirn, DWORD flags, RWSRayTestResult *res) = 0;

   /**
   Tests whether a 3D line segement (starting at \a p and finished at \a q) intersects any objects in the scene
   \param p      Start position for the line segment.
   \param q      End position for the line segment.
   \param flags  Flags to control line test calculations (currently must be set to 0).
   \param res    Pointer to RWSRayTestResult structure, which will contain the results
   of the ray test.
   \return TRUE or FALSE depending on whether the line segment intersects an object in the scene.
   */
   STDMETHOD_(BOOL, LineTest)(RWSPosition3D *p, RWSPosition3D *q, DWORD flags, RWSRayTestResult *res) = 0;

};

/*@}*/

#endif
