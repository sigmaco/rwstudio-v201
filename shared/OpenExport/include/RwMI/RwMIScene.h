// RwMIScene.h: interface for the RwMIScene class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWMISCENE_H__DA5DB0A2_38F3_4909_A804_32887CB17C1D__INCLUDED_)
#define AFX_RWMISCENE_H__DA5DB0A2_38F3_4909_A804_32887CB17C1D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwMI.h"
#include "RwCommScene.h"

/**
*   \ingroup miscenegraph
*
*   This is the RwCommScene implementation. The object caches frame data
*   and axis system data.
*
*   \see RwCommScene
*/
class RWMI_API RwMIScene : public RwCommScene
{
public:
    RwMIScene();
    virtual ~RwMIScene();

    // RwCommScene methods
    virtual RwCommReal  FramesToSec(RwCommReal frame) const;
    virtual RwCommError GetAxisSystem(RwCommMatrix &matrix) const;
    virtual RwCommError GetAmbientColor(RwCommVec3 &color) const;
    virtual RwCommError GetBackgroundColor(RwCommVec3 &color) const;

    virtual RwCommReal GetStartFrame() const { return m_startFrame; }
    virtual RwCommReal GetEndFrame() const   { return m_endFrame; }
  
    virtual RwCommError GetDescription(RwWString &description) const 
        { description = m_description; return RWCOMM_SUCCESS; }
    virtual RwCommError GetGUID(RwWString &guid) const 
        { guid = m_guid; return RWCOMM_SUCCESS; }

    virtual RwCommError SetDescription(const RwWString &description)
        { m_description = description; return RWCOMM_SUCCESS; }
    virtual RwCommError SetGUID(const RwWString &guid)
        { m_guid = guid; return RWCOMM_SUCCESS; }

    // RwMIScene methods
    RwCommError     SetFPS(RwCommReal fps);
    RwCommError     SetAxisSystem(const RwCommMatrix &matrix);
    RwCommError     SetAmbientColor(const RwCommVec3 &color);
    RwCommError     SetBackgroundColor(const RwCommVec3 &color);

    void            SetStartFrame(RwCommReal frame) { m_startFrame = frame; }
    void            SetEndFrame(RwCommReal frame)   { m_endFrame = frame; }


protected:
    RwCommReal      m_fps;
    RwCommMatrix    m_axisSystem;
    RwCommVec3      m_ambientColor;
    RwCommVec3      m_backgroundColor;

    RwCommReal      m_startFrame;
    RwCommReal      m_endFrame;

    RwWString        m_description;
    RwWString        m_guid;
};

#endif // !defined(AFX_RWMISCENE_H__DA5DB0A2_38F3_4909_A804_32887CB17C1D__INCLUDED_)
