#ifndef RWCOMMERROR_H
#define RWCOMMERROR_H

/**
*  \ingroup commlowlevelobjects
*   This enumeration is used as functions result values. It encapsulates API level
*   error handling and status code.
*/
enum RwCommError
{
    /** The operation was successful. */
    RWCOMM_SUCCESS = 0,
    /** The operation failed. */
    RWCOMM_FAIL = -1,
    /** The operation is not implemented. */
    RWCOMM_NOT_IMPLEMENTED = 2,
    /** The system is out of memory. */
    RWCOMM_OUTOFMEMORY,
    /** An invalid operand. */
    RWCOMM_INVALID_FIELD ,
    /** The operation aborted by the user. */
    RWCOMM_ABORT,
    /** Invalid index. */
    RWCOMM_INVALID_INDEX,
    /** The specified object is of wrong type. */
    RWCOMM_INVALID_OBJECT,
    /** The specified type of object not found. */
    RWCOMM_OBJECT_NOT_FOUND,
    /** External selection invalid. */
    RWCOMM_INVALID_SELECTION,
    /** Called action is disabled by an option. */
    RWCOMM_ACTION_DISABLED,

    // Animation 
    /** An approximation to an unsupported animation type has been generated. */
    RWCOMM_ANIMATION_APPROXIMATED,
    /** Invalid current frame for the operation. */
    RWCOMM_INVALID_FRAME,

    // Geometry
    /** The iterator reached the end. */
    RWCOMM_ITERATOR_END,
    /** Specified vertex doesn't exist. */
    RWCOMM_INVALID_VERTEX,

    // Morphs/BlendShapes
    /** Invalid number of morph/blendshape targets. */
    RWCOMM_INVALID_NUMTARGETS,

    // Textures
    /** Invalid image. */
    RWCOMM_INVALID_IMAGE,
    /** Failed to process alpha image. */
    RWCOMM_ALPHA_LOAD_FAIL,

    // Skin
    /** Invalid skin bone. */
    RWCOMM_INVALID_BONE,

	// RF3
	/** The param was not found */
	RWCOMM_PARAM_NOT_FOUND,
	/** The param is invalid */
	RWCOMM_INVALID_PARAM,
};

#endif