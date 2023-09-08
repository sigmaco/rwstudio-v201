#ifndef RWCOMM_PRIMITIVES
#define RWCOMM_PRIMITIVES
#pragma warning(disable : 4786)
#pragma warning(disable : 4251)

#include "RwComm.h"
#include "RwUtilTypes.h"

#include <float.h>
#include <math.h>

/**
*  \ingroup commlowlevelobjects
*   This is the RwComm specific real type. It is recommended that you use it to
*   avoid float to double conversion errors. Currently it is defined as float.
*/
typedef float RwCommReal;

class RwCommShader;
class RwCommMatrix;

// =============================================================================
// ----------------------------  RwCommVec2 CLASS  -----------------------------
// =============================================================================

/**
*  \ingroup commlowlevelobjects
*   This class implements two dimensional vector. Common convenience functions
*   are available.
*/
class RWCOMM_API RwCommVec2
{
public:
    /** Default constructor.*/
    RwCommVec2();

    /**
     *  Copy constructor.
     *  \param v the vector to be copied.
     */
    RwCommVec2(const RwCommVec2& v);
    
    /**
     *  Create a new instance and initialize it to the given position.
     *  \param x the x value for the new RwCommVec2
     *  \param y the y value for the new RwCommVec2
     */
    RwCommVec2(RwCommReal x, RwCommReal y);
    
    /**
     *  Return the i-th element in the vector by reference.
     *  \return a reference to the RwCommReal at the i-th position.
     */
    RwCommReal  &operator[](int i);
    
    /**
     *  Return the i-th element in the vector.
     *  \return the RwCommReal at the i-th position.
     */
    RwCommReal  operator[](int i) const;
    
    /** Return vector data as an RwCommReal pointer.*/
                operator RwCommReal*();
    /**
    *   Multiply (v[0], v[1], 1) with ((m[0][0], m[0][1]), (m[1][0],
    *   m[1][1]), (m[3][0], m[3][1])).
    */
    RwCommVec2  operator*(const RwCommMatrix &M) const;

    /**
     *  The less then operator.
     *  \param the RwCommVec2 to be compared against
     *  \return a bool indicating the result of the comparison.
     */
    bool        operator<(const RwCommVec2 &v) const;
    
    /**
     *  The equality operator.
     *  \param the RwCommVec2 to be compared against
     *  \return a bool indicating the result of the comparison.
     */
    bool        operator==(const RwCommVec2 &v) const;
    
    /**
     *  The inequality operator.
     *  \param the RwCommVec2 to be compared against
     *  \return a bool indicating the result of the comparison.
     */
    bool        operator!=(const RwCommVec2 &v) const;
    
    /**
     *  Return the dimension of the vector.
     *  \return number of elements.
     */
    unsigned int size() const { return 2; }
    
    RwCommReal data[2];
};

// =============================================================================
// ----------------------------  RwCommVec3 CLASS  -----------------------------
// =============================================================================

/**
*  \ingroup commlowlevelobjects
*   This class implements three dimensional vector. Common convenience functions
*   are available.
*/
class RWCOMM_API RwCommVec3
{
public:
    /** Default constructor.*/
    RwCommVec3();
    
    /**
     *  Copy constructor.
     *  \param v the vector to be copied.
     */
    RwCommVec3(const RwCommVec3 &v);
    
    /**
     *  Create a new instance and initialize it to the given position.
     *  \param x the x value for the new RwCommVec3
     *  \param y the y value for the new RwCommVec3
     *  \param z the z value for the new RwCommVec3
     */
    RwCommVec3(RwCommReal x, RwCommReal y, RwCommReal z);

    /**
     *  Return the i-th element in the vector by reference.
     *  \return a reference to the RwCommReal at the i-th position.
     */
    RwCommReal &operator[](int i);
    
    /**
     *  Return the i-th element in the vector.
     *  \return the RwCommReal at the i-th position.
     */
    RwCommReal  operator[](int i) const;
    
    /** Return vector data as an RwCommReal pointer.*/
                operator RwCommReal*();
    
    /**
      *  The scalar multiplication operator.
      *  \param r the multiplication factor
      *  \return the resut of the multiplication.
      */
    RwCommVec3  operator*(RwCommReal r) const;
    
    /**
     *  The matrix multiplication operator.
     *  \param b the multiplication factor
     *  \return the resut of the multiplication.
     */
    RwCommVec3  operator*(const RwCommMatrix &b) const;
    
    /**
     *  The in-place matrix multiplication operator.
     *  \param b the multiplication factor
     *  \return a reference to the modified RwCommVec3.
     */
    RwCommVec3 &operator*=(const RwCommMatrix &b);
    
    /**
     *  The addition  operator.
     *  \param v the vector to be added with
     *  \return the result of the addition.
     */
    RwCommVec3  operator+(const RwCommVec3 &v) const;
    
    /**
     *  The in-place addition operator.
     *  \param v the vector to be added with
     *  \return a reference to the modified RwCommVec3.
     */
    RwCommVec3 &operator+=(const RwCommVec3 &v);
    
    /**
     *  The subtraction operator.
     *  \param v the vector to be subtracted
     *  \return the result of the subtraction.
     */
    RwCommVec3  operator-(const RwCommVec3 &v) const;
    
    /**
     *  The in-place subtraction operator.
     *  \param v the vector to be subtracted
     *  \return a reference to the modified RwCommVec3.
     */
    RwCommVec3 &operator-=(const RwCommVec3 &v);
    
    /**
     *  The less then operator.
     *  \param the RwCommVec3 to be compared against
     *  \return a bool indicating the result of the comparison.
     */
    bool        operator<(const RwCommVec3 &v) const;
    
    /**
     *  The equality operator.
     *  \param the RwCommVec3 to be compared against
     *  \return a bool indicating the result of the comparison.
     */
    bool        operator==(const RwCommVec3 &v) const;
    
    /**
     *  The inequality operator.
     *  \param the RwCommVec3 to be compared against
     *  \return a bool indicating the result of the comparison.
     */
    bool        operator!=(const RwCommVec3 &v) const;
    
    /**
     *  Performs the cross product.
     *  \param the RwCommVec3 to perform a cross product with
     *  \return a reference to the modified RwCommVec3.
     */
    RwCommVec3  cross(const RwCommVec3 &v) const;
    
    /** 
     *  Performs an in-place normalization of the vector.
     *  \return true if successful, false otherwise
     */
    bool        Normalize();

    /**
     *  Return the dimension of the vector.
     *  \return number of elements.
     */
    unsigned int size() const { return 3; }

    RwCommReal data[3];
};

// =============================================================================
// ----------------------------  RwCommVec4 CLASS  -----------------------------
// =============================================================================

/**
*  \ingroup commlowlevelobjects
*   This class implements four dimensional vector. Common convenience functions
*   are available.
*/
class RWCOMM_API RwCommVec4
{
public:
    /** Default constructor.*/
    RwCommVec4();
    
    /**
     *  Copy constructor.
     *  \param v the vector to be copied.
     */
    RwCommVec4(const RwCommVec4 &v);
    
    /**
     *  Create a new instance and initialize it to the given position.
     *  \param x the x value for the new RwCommVec4
     *  \param y the y value for the new RwCommVec4
     *  \param z the z value for the new RwCommVec4
     *  \param w the w value for the new RwCommVec4
     */
    RwCommVec4(RwCommReal x, RwCommReal y, RwCommReal z, RwCommReal w);

    /**
     *  Return the i-th element in the vector by reference.
     *  \return a reference to the RwCommReal at the i-th position.
     */
    RwCommReal &operator[](int i);
    
    /**
     *  Return the i-th element in the vector.
     *  \return the RwCommReal at the i-th position.
     */
    RwCommReal  operator[](int i) const;
    
    /** Return vector data as an RwCommReal pointer.*/
                operator RwCommReal*();

    /**
     *  Return the dimension of the vector.
     *  \return number of elements.
     */
    unsigned int size() const { return 4; }

    /**
     *  The equality operator.
     *  \param the RwCommVec3 to be compared against
     *  \return a bool indicating the result of the comparison.
     */
    bool        operator==(const RwCommVec4 &v) const;
    
    /**
     *  The inequality operator.
     *  \param the RwCommVec3 to be compared against
     *  \return a bool indicating the result of the comparison.
     */
    bool        operator!=(const RwCommVec4 &v) const;

    RwCommReal data[4];
};

// =============================================================================
// ----------------------------  RwCommMatrix CLASS  ---------------------------
// =============================================================================

/**
*  \ingroup commlowlevelobjects
*   This class implements 4x4 matrix. Common convenience functions are available.
*
*  \note Some functions assume this is a 4x3 transformation matrix like:
*   RwCommMatrix::Invert, RwCommMatrix::RemoveScale,
*   RwCommMatrix::RemoveTranslation.
*/
class RWCOMM_API RwCommMatrix
{
public:
    /** Default constructor.*/
    RwCommMatrix();
    /** Copy constructor.*/
    RwCommMatrix(const RwCommMatrix& m);
    
    /** Copy constructor.*/
    /** Return the i-th row.*/
    RwCommReal         *operator[](int i);
    /** Return the i-th row as a const.*/
    const RwCommReal   *operator[](int i) const;
    /** Return matrix data as an RwCommReal double pointer.*/
                        operator RwCommReal**();
    /**
    *   The matrix multiplication operator. Multiplies the matrix with vector
    *   RwCommVec4(v[0], v[1], 0.0, 1.0)
    */
    RwCommVec2          operator*(const RwCommVec2 &v) const;
    /**
    *   The matrix multiplication operator. Multiplies the matrix with vector
    *   RwCommVec4(v[0], v[1], v[2], 1.0)
    */
    RwCommVec3          operator*(const RwCommVec3 &v) const;
    /** The vector multiplication operator.*/
    RwCommVec4          operator*(const RwCommVec4 &v) const;
    /** The matrix multiplication operator.*/
    RwCommMatrix        operator*(const RwCommMatrix &b) const;
    /** The in-place multiplication operator.*/
    RwCommMatrix       &operator*=(const RwCommMatrix &b);
    /** The equality operator.*/
    bool                operator==(const RwCommMatrix &M) const;

    /** Set the i-th row of the matrix to the given vector.*/
    void                SetRow(int i, const RwCommVec4 &v);
    /** Invert matrix. This function assumes this is a 4x3 transformation matrix.*/
    void                Invert();
    /** Remove scale. This function assumes this is a 4x3 transformation matrix.*/
    RwCommVec3          RemoveScale();
    /**
    *   Remove transformation. This function assumes this is a 4x3
    *   transformation matrix.
    */
    void                RemoveTranslation();

    /**
    *   Returns the transpose of the matrix
    */
    RwCommMatrix        Transpose() const;

    RwCommReal data[4][4];
};

// =============================================================================
// -------------------------  RwCommBoundingBox CLASS  -------------------------
// =============================================================================

/**
*  \ingroup commlowlevelobjects
*   This class implements a 3D bounding box. Common convenience functions are
*   available.
*/
class RWCOMM_API RwCommBoundingBox
{
public:
    /** Default constructor. Creates an empty bounding box.*/
    RwCommBoundingBox();
    /** Copy constructor.*/
    RwCommBoundingBox(const RwCommVec3 &min, const RwCommVec3 &max);

    /** Reset the value to an empty bounding box.*/
    void                Init();
    /** \return bounding box diagonal.*/
    float               GetDiagonalSize() const;

    /** Expand the bounding box to include a second bounding box.*/
    RwCommBoundingBox  &operator+=(const RwCommBoundingBox &b);
    /** Expand the bounding box to include the given point.*/
    RwCommBoundingBox  &operator+=(const RwCommVec3 &point);
    
    /** Transform to this bounding box.*/
    RwCommBoundingBox   operator*(const RwCommMatrix &M) const;
    /** Apply the given transformation to this bounding box.*/
    RwCommBoundingBox  &operator*=(const RwCommMatrix &M);

    RwCommVec3 min;
    RwCommVec3 max;
};


#if (!defined(DOXYGEN))

template<class T>
struct RwCommLerp
{
    T operator() (RwCommReal factor, const T &v1, const T &v2) const
    {
        return v1*(RwCommReal(1) - factor) + v2*factor;
    }

};
#endif // (!defined(DOXYGEN))

#endif