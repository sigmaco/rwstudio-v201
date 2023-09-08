// RwExpUVOps.h: interface for the RwExpUVOps class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWEXPUVOPS_H__E43BE86E_889D_446A_A9E1_83D2B6568B6A__INCLUDED_)
#define AFX_RWEXPUVOPS_H__E43BE86E_889D_446A_A9E1_83D2B6568B6A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwExp.h"
#include "RwExpVertexFilter.h"
#include "RwExpExporter.h"

class RwCommTexture;

struct RWEXP_API RwExpTexBlock
{
    RwCommMatrix        UVTransform;
    RwVector<int>   iFields;
    RwVector<float> fFields;
};

/**
*
*  \ingroup vertexpipeline
*
*   This class is an intermediate utility class, for caching texture information.  
*/
class RWEXP_API RwExpTextureCache : public RwExpVertexFilter 
{
public:
    RwExpTextureCache() {}
    virtual ~RwExpTextureCache() {}

protected:

    RwExpTexBlock* GetTexBlock(RwCommTexture *pTexture);
    RwExpTexBlock* AddTexBlock(RwCommTexture *pTexture);
    void ClearCache();

protected:

    RwMap<RwCommTexture *, RwExpTexBlock> m_mTexCache;

};

const RwCommType RwExpTransformUVFilterType("RwExpTransformUVFilter");

/**
*
*  \ingroup vertexpipeline
*
*   This vertex filter is responsible for transforming UV texture coordinates by their UV transformation.
*   In addition the filter flips the V values within the (Vmin, Vmax) range.  
*/

class RWEXP_API RwExpTransformUVFilter  : public RwExpTextureCache
{
public:
    RwExpTransformUVFilter() {}
    virtual ~RwExpTransformUVFilter() {}

    virtual RwCommType GetType() const { return RwExpTransformUVFilterType; }

    virtual unsigned GetVertexDataMask() const { return RwCommVertex::UVS; }

    // Initialise the filter. Called at the beginning of the pipeline.
    virtual bool Init(RwCommGeometry *pGeometry, const RwCommExportOptions *pGlobalData);

    // Apply the filter 
     virtual void Do(RwVector<RwCommVertex *> &vertices, RwVector<RwCommPrimitive> &primitives,
        RwCommGeometry *pGeometry, const RwCommExportOptions *pExportOptions);

};

const RwCommType RwExpWeldFilterType("RwExpWeldFilter");

/**
*
*  \ingroup vertexpipeline
*
*   This vertex filter is responsible for welding vertex positions and UV texture coordinates by a user
*   specified threshold (using the WELDTHRESHOLD and the WELDTHRESHOLDUV options).  
*/
class RWEXP_API RwExpWeldFilter : public RwExpVertexFilter  
{
public:
    RwExpWeldFilter() {}
    virtual ~RwExpWeldFilter() {}

    virtual RwCommType GetType() const { return RwExpWeldFilterType; }

    virtual unsigned GetVertexDataMask() const { return RwCommVertex::POSITION | RwCommVertex::UVS; }

    // Initialise the filter. Called at the beginning of the pipeline.
    virtual bool Init(RwCommGeometry *pGeometry, const RwCommExportOptions *pExportOptions);

    // Apply the filter 
     virtual void Do(RwVector<RwCommVertex *> &vertices, RwVector<RwCommPrimitive> &primitives,
        RwCommGeometry *pGeometry, const RwCommExportOptions *pExportOptions);

};


const RwCommType RwExpCropFilterType("RwExpCropFilter");

/**
*
*  \ingroup vertexpipeline
*
*   This vertex filter is responsible for cropping the geometry's UV texture coordinates, according to the
*   cropping values, specified in the geometry's textures.
*/
class RWEXP_API RwExpCropFilter : public RwExpTextureCache  
{
public:
    RwExpCropFilter() {}
    virtual ~RwExpCropFilter() {}

    virtual RwCommType GetType() const { return RwExpCropFilterType; }

    virtual unsigned GetVertexDataMask() const { return RwCommVertex::UVS; }

    // Initialise the filter. Called at the beginning of the pipeline.
    virtual bool Init(RwCommGeometry *pGeometry, const RwCommExportOptions *pGlobalData);

    // Apply the filter 
    virtual void Do(RwVector<RwCommVertex *> &vertices, RwVector<RwCommPrimitive> &primitives,
        RwCommGeometry *pGeometry, const RwCommExportOptions *pExportOptions);

};

const RwCommType RwExpLimitUVFilterType("RwExpLimitUVFilter");

/**
*
*  \ingroup vertexpipeline
*
*   This vertex filter is responsible for applying UV limiting within the user specified range. The filter
*   is enabled only if the LIMITUVS option is on. The range is specified by the (LIMITUVMIN, LIMITUVMAX) options.
*/

class RWEXP_API RwExpLimitUVFilter : public RwExpTextureCache  
{
public:
    RwExpLimitUVFilter() {}
    virtual ~RwExpLimitUVFilter() {}

    virtual RwCommType GetType() const { return RwExpLimitUVFilterType; }

    virtual unsigned GetVertexDataMask() const { return RwCommVertex::UVS; }

    // Initialise the filter. Called at the beginning of the pipeline.
    virtual bool Init(RwCommGeometry *pGeometry, const RwCommExportOptions *pGlobalData);

     // Apply the filter 
    virtual void Do(RwVector<RwCommVertex *> &vertices, RwVector<RwCommPrimitive> &primitives,
        RwCommGeometry *pGeometry, const RwCommExportOptions *pExportOptions);

    void SetExporter(RwExpExporter *exporter) { m_exporter = exporter; }

protected:

    // Error stuff for displaying missing textures
    // The following classes exist solely to output the error information collected
    // by this filter
    friend class RwExpCheckUVFilterErrors;

    // map of missing textures to the objects they affect
    RwMap< RwWString, RwWString >  m_MissingTextureData;

    RwExpExporter *m_exporter;
};

const RwCommType RwExpLightMapFilterType("RwExpLightMapFilter");

/**
*
*  \ingroup vertexpipeline
*
*   This vertex filter is responsible for proccessing UV texture coordinates for light map support. The filter
*   is enabled only if the PROCESSFORLIGHTMAPS option is turned on.
*/
class RWEXP_API RwExpLightMapFilter : public RwExpVertexFilter  
{
public:
    RwExpLightMapFilter() {}
    virtual ~RwExpLightMapFilter() {}

    virtual RwCommType GetType() const { return RwExpLightMapFilterType; }

    virtual unsigned GetVertexDataMask() const { return RwCommVertex::UVS; }

    // Initialise the filter. Called at the beginning of the pipeline.
    virtual bool Init(RwCommGeometry *pGeometry, const RwCommExportOptions *pExportOptions);

    // Apply the filter 
     virtual void Do(RwVector<RwCommVertex *> &vertices, RwVector<RwCommPrimitive> &primitives,
        RwCommGeometry *pGeometry, const RwCommExportOptions *pExportOptions);

     // Set the axis matrix transformation to apply to generated primitive normals
     virtual void SetAxisMatrix(const RwCommMatrix &axisMatrix) { m_axisMatrix = axisMatrix; }

     void SetExporter(RwExpExporter *exporter) { m_exporter = exporter; }

protected:
    RwCommMatrix m_axisMatrix;

    RwExpExporter *m_exporter;
};


const RwCommType RwExpMarkPatchFilterType("RwExpMarkPatchFilter");

/**
*
*  \ingroup vertexpipeline
*
*   This vertex filter is responsible for tagging patches according to RenderWare requirements.
*   For quad-patches the fifth vertex is given a zero U texture coordinate. For tri-patches the
*   fifth vertex is given a non-zero U texture coordinate.
*/
class RWEXP_API RwExpMarkPatchFilter : public RwExpVertexFilter  
{
public:
    RwExpMarkPatchFilter() {}
    virtual ~RwExpMarkPatchFilter() {}

    virtual RwCommType GetType() const { return RwExpMarkPatchFilterType; }

    virtual unsigned GetVertexDataMask() const { return RwCommVertex::UVS; }

     // Apply the filter 
     virtual void Do(RwVector<RwCommVertex *> &vertices, RwVector<RwCommPrimitive> &primitives,
        RwCommGeometry *pGeometry, const RwCommExportOptions *pExportOptions);

};

#endif // !defined(AFX_RWEXPUVOPS_H__E43BE86E_889D_446A_A9E1_83D2B6568B6A__INCLUDED_)
