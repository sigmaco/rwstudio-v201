// RwExpVertexFilter.h: interface for the RwExpVertexFilter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWEXPVERTEXFILTER_H__88A9C80E_A37B_457C_8CA4_580E849CC992__INCLUDED_)
#define AFX_RWEXPVERTEXFILTER_H__88A9C80E_A37B_457C_8CA4_580E849CC992__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwExp.h"
#include "RwExpExportOptions.h"
#include "RwCommVertex.h"
#include "RwCommObject.h"
#include "RwCommGeometry.h"

class RwExpVertexPipeline;

#define REMAPPER_MASK 0x200

/**
*
*  \ingroup vertexpipeline
*
As explained in the previous section the vertex filters are the basic atomic modular components for applying
vertex and primitive optimization tasks. The RwExpVertexFilter class is the base class for all vertex filters.
To implement your own vertex filter you need to implement the following methods:

\code
virtual RwCommType GetType() const = 0;
\endcode
Each vertex filter has a unique type, identifying it in the pipeline. The implementing class should return
that type within this method.

\code
virtual unsigned GetVertexDataMask() const = 0;
\endcode
Each filter specifies the type of data that it is modifying. This data need to be packed in a mask
containing all the appropriate RwCommVertex data types that filter is modifying. See the RwCommVertex class
for these data types. Filters implementing a re-map operation should also return the REMAPPER_MASK flag.

\code
virtual bool Init(RwCommGeometry* pGeometry, const   RwCommExportOptions* pExportOptions);
\endcode
Each filter can optionally implement this method to perform its initialization code. 
Filters that fail to initialize and return a FALSE value are excluded from the upcoming vertex pipeline
operation. This method is called once for each export process, before applying the filter. The vertex
pipeline iterates through all of its filters checking if they succeeded initializing. Only those that were
initialized participate in the pipeline operation.

\code
virtual void Do(RwVector<RwCommVertex*>& vertices, RwVector<RwCommPrimitive>& primitives, RwCommGeometry* pGeometry, const RwCommExportOptions* pExportOptions);
\endcode
This method should implement the actual filter operation. It receives a list of vertices and primitives.
Typically, filters will traverse the vertices and primitives list and perform their required operation.
In addition the RwCommGeometry object to which these vertices and primitives belong are passed to this method.
Filters can perform different queries on the RwCommGeometry to assist them with the filter operation.

\code
virtual void CleanUp(RwCommGeometry* pGeometry, const RwCommExportOptions* pExportOptions);
\endcode
Filters can optionally clean up all the data they have created in this method. This method is called at the
end of the pipeline operation, after all the filters finished performing their operation.

Here's an example of a simple filter that scales all of the vertices by a simple value:
\code
const RwCommType RwExpScaleFilterType("RwExpScaleFilter");

class RwExpScaleFilter : public RwExpVertexFilter  
{
public:
 RwExpScaleFilter();
 virtual ~RwExpScaleFilter() {}

 RwCommType GetType() const { return RwExpScaleFilterType; }

 unsigned GetVertexDataMask() const
                               { return RwCommVertex::POSITION; }

void Do(RwVector<RwCommVertex*>& vertices, RwVector<RwCommPrimitive>& primitives
        RwCommGeometry* pGeometry, RwCommExportOptions* pOptions) 
{
    for (int i = 0; i < vertices.size(); i++)
    {
        RwCommVec3 v;
        RwCommVertex& vertex = *vertices[i];
        
        if (vertex.GetPosition(v) == RWCOMM_SUCCESS)
        {
            v[0] *= m_vScale;
            v[1] *= m_vScale;
            v[2] *= m_vScale;

            vertex.SetPosition(v);
        }
    }
}

void SetScale(const RwCommVec3& scale) { m_vScale = scale; }

private:

    RwCommVec3 m_vScale;
};
\endcode
 
*/

class RWEXP_API RwExpVertexFilter  : public RwCommObject
{
public:
    RwExpVertexFilter();
    virtual ~RwExpVertexFilter();

    /**  Get the vertex data type mask. Users deriving from RwExpVertexFilter should
     *   return a mask containing all vertex data fields that their filter modifies.
     *  \see RwCommVertex
     *  \return the current vertex data mask
     */
    virtual unsigned GetVertexDataMask() const = 0;

    /** Initialize the filter. Called at the beginning of the pipeline.
     *  Filters that fail to initialize (return false) do not participate in the vertex pipeline.
     *
     *  \param pGeometry a pointer to the proccessed geometry.
     *  \param pExportOptions a pointer to the export options object
     *  \return a boolean indicating whether this filter should be used in the pipeline
     */
    virtual bool Init(RwCommGeometry *pGeometry, const RwCommExportOptions *pExportOptions) { return true; }

    /** Apply the filter. All classes deriving from RwExpVertexFilter should implement their
     *  filter code here.
     *
     *  \param vertices a vector of RwCommVertex pointers.
     *  \param primitives a vector of primitives.
     *  \param pGeometry a pointer to the proccessed geometry.
     *  \param pExportOptions a pointer to the export options object
     */
    virtual void Do(RwVector<RwCommVertex *> &vertices, RwVector<RwCommPrimitive> &primitives,
        RwCommGeometry *pGeometry, const RwCommExportOptions *pExportOptions) {}

    /**
     *  Clean up the filter. Called at the end of the pipeline. All classes deriving from RwExpVertexFilter should
     *  implement their filter clean up code here.
     *
     *  \param pGeometry a pointer to the proccessed geometry.
     *  \param pGlobalData the current export options
     */
    virtual void CleanUp(RwCommGeometry *pGeometry, const RwCommExportOptions *pGlobalData) {}

    void SetVertexPipeline(RwExpVertexPipeline *pVertexPipeline) { m_pVertexPipeline = pVertexPipeline; }
    RwExpVertexPipeline *GetVertexPipeline() const { return m_pVertexPipeline; }

    /**
     *  Enable/Disable the filter.
     *
     *  \param b whether this filter is to be enabled or not
     */
    void Enable(bool b) { m_bEnabled = b; }

    /**
     *  Is the filter enabled/disabled.
     *  \return a bool indicating if this filter is enabled or not
     */
    bool IsEnabled() const { return m_bEnabled; }


private:    

    RwExpVertexPipeline         *m_pVertexPipeline;
    bool                        m_bEnabled;
};

#endif // !defined(AFX_RWEXPVERTEXFILTER_H__88A9C80E_A37B_457C_8CA4_580E849CC992__INCLUDED_)
