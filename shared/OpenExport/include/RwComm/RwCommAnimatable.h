// RwExpAnimatable.h: interface for the RwExpAnimatable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWEXPANIMATABLE_H__B831B91B_4FD5_4A4A_BEDA_5EC42D3D6410__INCLUDED_)
#define AFX_RWEXPANIMATABLE_H__B831B91B_4FD5_4A4A_BEDA_5EC42D3D6410__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwCommModifiable.h"

typedef RwString RwCommAnimType;

/**
*  \ingroup commlowlevelobjects
*
*   Object of this type define a channel RwCommAnimation is driving in the RwCommAnimatable. Channel 
*   is specified when adding animation object to animatable with RwCommAnimatable::AddAnimation()
*   To get the channel for an animation use RwCommAnimatable::GetAnimatedChannel(). Each derived
*   RwCommAnimatable class defines its own animated channels.
*/
struct RwCommAnimatedChannel
{
    RwCommAnimatedChannel() : m_index(0), m_type(0), m_pParent(NULL) {}
    RwCommAnimatedChannel(int type) : m_index(0), m_type(type), m_pParent(NULL) {}
    RwCommAnimatedChannel(int type, RwCommAnimatedChannel &parent) : m_index(0), m_type(type), m_pParent(&parent) {}

    /** Equality operator*/
    bool operator==(const RwCommAnimatedChannel &that) const
    {
        return(m_type == that.m_type);
    }

    bool operator<(const RwCommAnimatedChannel &that) const
    {
        return(m_type < that.m_type);
    }

    int GetIndex(void) const 
    {
        return m_index;
    }

    /** Checks if this RwCommAnimatedChannel derives from or is equal to the given animated channel.
    *   \param that the given animated channel.
    *   \return true if this RwCommAnimatedChannel derives from or is equal to that
    */
    bool IsA(const RwCommAnimatedChannel &that) const
    {
        bool bIsA = false;
        
        if (*this == that)
        {
            bIsA = true;
        }
        else if (m_pParent)
        {
            bIsA = m_pParent->IsA(that);
        }

        return bIsA;
    }

    /** the index of the animated channel (if applicable) */
    int                     m_index;

    /** enum value of the animated channel */
    int                     m_type;

    /** the parent animated channel that this channel derives from (if any) */
    RwCommAnimatedChannel   *m_pParent;
};

#define RwCommUndefinedAnimation "Undefined"

class RwCommAnimation;

const RwCommType RwCommAnimatableType("RwCommAnimatable");

/**
*
*  \ingroup commscenegraph
*
*   This is the base class for all classes related to animation. All animated objects in the scene graph
*   derive from this class. Animatable objects hold a list of attached RwCommAnimation objects that are
*   responsible for animating the hosting RwCommAnimatable object. 
*   
*   Each RwCommAnimatable maintains its own current frame. The current frame can be set using the
*   SetFrame() method. Once the current frame is set, all the methods of the RwCommAnimatable and its deriving
*   classes, relate to that current frame.
*/

class RWCOMM_API RwCommAnimatable : public RwCommModifiable
{
public:
    enum AttributeType
    {
        UNKNOWN = 0,
        INT,
        FLOAT,
        STRING,
    };

    /** Default animated channel */
    static RwCommAnimatedChannel UndefinedChannel;

    RwCommAnimatable();
    virtual ~RwCommAnimatable();

    /** \return RwCommAnimatableType*/
    virtual RwCommType      GetType() const { return RwCommAnimatableType; }
    virtual void            GetTypeHierarchy(RwVector<RwCommType> &vTypeVec) const;

    // Get attributes. 
    /**
     *  Get a custom int attribute value by name.
     *
     *  \param name of the attribute's name.
     *  \param value reference to the int value returned.
     *  \param index an optional index.
     *
     *  \return RWCOMM_SUCCESS if the attribute exists, RWCOMM_FAIL otherwise
     */
    virtual RwCommError     GetAttribute(const RwWString &name, int &value, int index = 0)
                                { return RWCOMM_NOT_IMPLEMENTED; }
    /**
     *  Get a custom float attribute value by name.
     *
     *  \param name of the attribute's name.
     *  \param value reference to the float value returned.
     *  \param index an optional index.
     *
     *  \return RWCOMM_SUCCESS if the attribute exists, RWCOMM_FAIL otherwise
     */
    virtual RwCommError     GetAttribute(const RwWString &name, float &value, int index = 0)
                                { return RWCOMM_NOT_IMPLEMENTED; }
    /**
     *  Get a custom string attribute value by name.
     *
     *  \param name the attribute's name.
     *  \param value reference to the string value returned.
     *  \param index an optional index.
     *
     *  \return RWCOMM_SUCCESS if the attribute exists, RWCOMM_FAIL otherwise
     */
    virtual RwCommError     GetAttribute(const RwWString &name, RwWString &value, int index = 0)
                                { return RWCOMM_NOT_IMPLEMENTED; }

    /**
     *  Get all atrribute names and types
     *
     *  \param vAttributeNames a vector of attribute names
     *  \param vAttributeTypes the corresponding attribute types
     *  \return RWCOMM_SUCCESS on succes, RWCOMM_FAIL otherwise
     */
    virtual RwCommError     GetAttributeNames(RwVector<RwWString> &vAttributeNames,
                                              RwVector<AttributeType> &vAttributeTypes)
                                { return RWCOMM_NOT_IMPLEMENTED; }
    /**
     *  Gets the number of animations attached to the RwCommAnimatable.
     *
     *  \return the number of animations.
     *
     *  \see RwCommAnimation class.
     */
    int                     GetNumAnimations() const;
    /**
     *  Attaches an RwCommAnimation object to the RwCommAnimatable. The method also specifies the animated channel
     *  that this animation drives. Each derived RwCommAnimatable class defines its own animated channels. 
     *
     *  \param pAnimation a pointer to an RwCommAnimation object to attach.
     *  \param channel of the specified RwCommAnimation. Default is UNDEFINED.
     *
     *  \see RwCommAnimation class.
     */
    void                    AddAnimation(RwCommAnimation *pAnimation,
                                RwCommAnimatedChannel channel = UndefinedChannel);
    /**
     *  Gets the i-th animation.
     *
     *  \param i index of the animation in the animation list.
     *
     *  \return a pointer to the RwCommAnimation object.
     *
     *  \see RwCommAnimation class.
     */
    RwCommAnimation         *GetAnimation(int i);
    virtual RwCommAnimType  GetAnimationType(int i);

    /**
     *  Gets the animation channel which is driven by the i-th animation.
     *
     *  \param i index of the animation in the animation list.
     *
     *  \return the animated channel.
     */
    RwCommAnimatedChannel   GetAnimatedChannel(int i);

    /**
     *  Sets the animatable's current frame value. RwCommAnimatables are constructed with an invalid frame. To start
     *  using an RwCommAnimatable object, its frame must be initialized to a certain value. Changing the frame value
     *  will modify the state of the animatable object, to correspond to the given frame.
     *
     *  \param frame the animation frame to set.
     *
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise
     *
     */
    RwCommError             SetFrame(RwCommReal frame);
    /**
     *  Gets the animatable's current animation frame value. 
     *
     *  \return the current frame value.
     *
     */
    RwCommReal              GetFrame() const { return m_nFrame; }

    /**
     *   Called every time the frame is changed.
     *  \param frame the animation frame being made current.
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise
     */
    virtual RwCommError     OnSetFrame(RwCommReal frame) { return RWCOMM_SUCCESS; }

protected:
    RwVector<RwCommAnimation*>      m_vAnimations;
    RwVector<RwCommAnimatedChannel> m_vChannels;
    RwCommReal                          m_nFrame;
};

#endif // !defined(AFX_RWEXPANIMATABLE_H__B831B91B_4FD5_4A4A_BEDA_5EC42D3D6410__INCLUDED_)
