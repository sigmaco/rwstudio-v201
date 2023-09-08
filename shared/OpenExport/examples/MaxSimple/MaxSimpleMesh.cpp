
#include "stdafx.h"

#include "RwUtilString.h"
#include "RwCommVertex.h"
#include "RwCommUtils.h"

#include "MaxSimpleMesh.h"
#include "MaxSimpleBuilder.h"
#include "MaxSimpleScene.h"
#include "MaxSimpleShader.h"


/** Construct an MaxSimpleMesh from an INode. The Mesh object will be created in the class's Evaluate() method.*/
MaxSimpleMesh::MaxSimpleMesh(INode* pNode) : m_pMesh(NULL), m_pObject(NULL), m_pTriObj(NULL), m_bDelObj(false), m_pINode(pNode),
m_iFaceIterator(0)
{

}

MaxSimpleMesh::~MaxSimpleMesh()
{
    if (m_bDelObj)
    {
        m_pTriObj->DeleteMe();
    } 
}

/**
*   Comparison operator. This method is used to determine if two meshes are identical.
*   For this example, we'll always return false.
*/
bool 
MaxSimpleMesh::operator==(const RwCommGeometry& geom) const
{
    return false;
} 


/** Checks if INode::Renderable() is true and that INode::IsNodeHidden() is false.*/
RwCommError     
MaxSimpleMesh::GetVisibility(bool& bIsVisible) const
{
    bIsVisible = (m_pINode->Renderable() && !m_pINode->IsNodeHidden() ? true : false);
    return RWCOMM_SUCCESS;
}

/** Calls ReEvaluate() each time the frame is changed.*/
RwCommError 
MaxSimpleMesh::OnSetFrame(RwCommReal frame)
{
    RwCommError hResult = RWCOMM_SUCCESS;

    if (frame != m_nFrame)
    {
        hResult = ReEvaluate(frame);
    }

    return hResult;
}

RwCommError 
MaxSimpleMesh::GetWorldSpaceBoundingBox(RwCommBoundingBox& bbox) const
{
    Box3            localBBox;
    Object          *obj = NULL;
    RwCommMatrix    axisSystem;
    RwCommError     hResult = RWCOMM_FAIL;

    obj = m_pINode->EvalWorldState(static_cast<int>(m_nFrame * GetTicksPerFrame())).obj;
    if (obj)
    {
        // Transform the bounding box by the global axis system
        GetScene()->GetAxisSystem(axisSystem);

        obj->GetDeformBBox(static_cast<int>(m_nFrame * GetTicksPerFrame()), localBBox);
        localBBox = localBBox * m_pINode->GetObjTMAfterWSM(static_cast<int>(m_nFrame * GetTicksPerFrame())) *
                        RwCommMatrixToMatrix3(axisSystem);

        bbox = Box3ToRwCommBoundingBox(localBBox);
        hResult = RWCOMM_SUCCESS;
    }

    return hResult;
}


/**
*   Creates the Mesh object.
*   This methods calls INode::EvalWorldState() with the given frame value and creates the Mesh object.
*   In addition, it builds the Mesh's normals (using Mesh::BuildNormals()).
*   
*   This method is called each time SetFrame() is called with a different frame value.
*/
RwCommError 
MaxSimpleMesh::ReEvaluate(RwCommReal frame)
{
    RwCommError hResult = RWCOMM_INVALID_FRAME;

    // If we've already cached the tri object, delete it
    if (m_bDelObj)
    {
        m_pTriObj->DeleteMe();
        m_pTriObj = NULL;
        m_bDelObj = false;
    }

    // Check that the object is in anim range
    Interval animRange = GetCOREInterface()->GetAnimRange();
    int frameInTicks = (TimeValue)(frame * GetTicksPerFrame());

    if (frameInTicks >= animRange.Start() && frameInTicks <= animRange.End())
    {
        m_pObject = m_pINode->EvalWorldState( frameInTicks ).obj;
        if (m_pObject->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0)))
        {
            // Cache the mesh & tri objects
            m_pTriObj = (TriObject *) m_pObject->ConvertToType(0, Class_ID(TRIOBJ_CLASS_ID, 0));
            m_pMesh = &m_pTriObj->mesh;

            m_bDelObj = (m_pObject != m_pTriObj);

            // Build the normals
            m_pMesh->buildNormals();

            // Calc world space matrix
            m_worldMat = m_pINode->GetObjTMAfterWSM((TimeValue)(m_nFrame * GetTicksPerFrame()));

            // Calc world space normal matrix
            m_worldNormalMat = m_worldMat;
            m_worldNormalMat.NoScale();
            m_worldNormalMat.NoTrans();

            // Calc offset matrix
            m_offsetMat = GetObjOffsetMatrix(m_pINode);

            // Clac offset normal matrix
            m_offsetNormalMat = m_offsetMat;
            m_offsetNormalMat.NoScale();
            m_offsetNormalMat.NoTrans();

            hResult = RWCOMM_SUCCESS;
        }
    }

    return hResult;
}

RwCommError 
MaxSimpleMesh::GetVertex(const RwCommVertexId& id, RwCommVertex& vertex, bool bWorldSpace)
{
    RwCommError eResult = GetVertex(id.face, id.vertex, vertex, bWorldSpace);  

    // Flip normal if this is a back face vertex
    if (id.backFace)
    {
        RwCommVec3 normal;
        if (vertex.GetNormal(normal) == RWCOMM_SUCCESS)
        {
            vertex.SetNormal( RwCommVec3( -normal[0], -normal[1], -normal[2] ) );
        }
    }
    return eResult;
}

RwCommError 
MaxSimpleMesh::GetNumPrimitives(int& nPrims) const
{
    nPrims = m_pMesh->getNumFaces();
    return RWCOMM_SUCCESS;
}

RwCommError
MaxSimpleMesh::ResetPrimIterator()
{
    m_iFaceIterator = 0;
    return (m_pMesh->getNumFaces() > 0 ? RWCOMM_SUCCESS : RWCOMM_FAIL);
}

RwCommError 
MaxSimpleMesh::AdvancePrimIterator()
{
    m_iFaceIterator++;
    return (m_iFaceIterator < m_pMesh->getNumFaces() ? RWCOMM_SUCCESS : RWCOMM_ITERATOR_END);   
}

RwCommError 
MaxSimpleMesh::GetPrimVertexCount(int& nVertices)
{
    nVertices = 3;
    return RWCOMM_SUCCESS;
}

RwCommError 
MaxSimpleMesh::GetPrimIndex(int& nIndex)
{
    nIndex = m_iFaceIterator;
    return RWCOMM_SUCCESS;
}

RwCommError 
MaxSimpleMesh::GetPrimShader(RwCommShader*& pShader)
{
    pShader = GetFaceShader(m_iFaceIterator) ;
    return (pShader ? RWCOMM_SUCCESS : RWCOMM_FAIL);
}

RwCommError 
MaxSimpleMesh::GetPrimNormal(RwCommVec3& normal, bool bWorldSpace)
{
    Point3 faceNormal;
    faceNormal = m_pMesh->FaceNormal(m_iFaceIterator, TRUE);

    if (bWorldSpace)
    {
        normal = Point3ToRwCommVec3(faceNormal * m_worldNormalMat);
    }
    else
    {
        normal = Point3ToRwCommVec3(faceNormal * m_offsetNormalMat);
    }
    return RWCOMM_SUCCESS;
}

RwCommError
MaxSimpleMesh::GetPrimTriangleCount(int& nTriangles)
{
    nTriangles = 1;
    return RWCOMM_SUCCESS;
}

RwCommError
MaxSimpleMesh::GetPrimTriangulation(RwVector< int >& triangleIndices)
{
    triangleIndices.push_back(0);
    triangleIndices.push_back(1);
    triangleIndices.push_back(2);
    return RWCOMM_SUCCESS;
}


RwCommError 
MaxSimpleMesh::GetDefaultColor(RwCommVec3 &color) const
{
    COLORREF    wireColor = m_pINode->GetWireColor();

    color[0] = ((int) GetRValue(wireColor))/255.0f;
    color[1] = ((int) GetGValue(wireColor))/255.0f;
    color[2] = ((int) GetBValue(wireColor))/255.0f;

    return RWCOMM_SUCCESS;
}



RwCommError
MaxSimpleMesh::GetPrimVertex(int iVertexIndex, RwCommVertex& vertex, RwCommVertexId& id, bool bWorldSpace)
{
    int posIndex = -1;
    RwCommError eResult = GetVertex(m_iFaceIterator, iVertexIndex, vertex, bWorldSpace);

    vertex.GetPositionIndex(posIndex);
    id = RwCommVertexId(this, m_iFaceIterator, iVertexIndex, posIndex);
    return eResult;
}

RwCommError 
MaxSimpleMesh::GetVertex(int iFace, int iVertex, RwCommVertex& vertex, bool bWorldSpace) const
{
    RwCommVec3 v;
    RwCommReal r;
    int iIndex;

    // Get vertex position
    if (GetVertexPosition(iFace, iVertex, v, bWorldSpace) == RWCOMM_SUCCESS)
    {
        vertex.SetPosition(v);
    }
    
    // Get the position index
    if (GetVertexPositionIndex(iFace, iVertex, iIndex) == RWCOMM_SUCCESS)
    {
        vertex.SetPositionIndex(iIndex);
        vertex.SetUniqueID(iIndex);
    }

    // Get the vertex normal
    if (GetVertexNormal(iFace, iVertex, v, bWorldSpace) == RWCOMM_SUCCESS)
    {
        vertex.SetNormal(v);
    }

    // Get the vertex color
    if (GetVertexColor(iFace, iVertex, v) == RWCOMM_SUCCESS)
    {
        vertex.SetColor(v);
    }

    // Get the vertex alpha value
    if (GetVertexAlpha(iFace, iVertex, r) == RWCOMM_SUCCESS)
    {
        vertex.SetAlpha(r);
    }

    // Get the vertex illumination
    if (GetVertexIllumination(iFace, iVertex, v) == RWCOMM_SUCCESS)
    {
        vertex.SetIllumination(v);
    }

    // Get vertex texture coords
    RwVector< RwCommVec2 > uvs;
    if (GetVertexUVs(iFace, iVertex, uvs) == RWCOMM_SUCCESS)
    {
        for (unsigned int j = 0; j < uvs.size(); j++)
            vertex.AddUV(uvs[j]);
    }

    // Set the shader, assigned to the vertex's primitive
    vertex.SetShader(GetFaceShader(iFace)) ;

    return RWCOMM_SUCCESS;
}

RwCommError 
MaxSimpleMesh::GetVertexPosition(int iFace, int iVertex, RwCommVec3& vertex, bool bWorldSpace) const
{
    if (bWorldSpace)
    {
        vertex = Point3ToRwCommVec3(m_pMesh->verts[m_pMesh->faces[iFace].v[iVertex]] * m_worldMat);
    }
    else
    {
        vertex = Point3ToRwCommVec3(m_pMesh->verts[m_pMesh->faces[iFace].v[iVertex]] * m_offsetMat);
    }
    return RWCOMM_SUCCESS;
}

RwCommError 
MaxSimpleMesh::GetVertexPositionIndex(int iFace, int iVertex, int& iIndex) const
{
    iIndex = m_pMesh->faces[iFace].v[iVertex];
    return RWCOMM_SUCCESS;
}

RwCommError 
MaxSimpleMesh::GetVertexNormal(int iFace, int iVertex, RwCommVec3& normal, bool bWorldSpace) const
{
    RwCommError eResult = RWCOMM_FAIL;

    Point3 n;
    RVertex *rvert = m_pMesh->getRVertPtr(m_pMesh->faces[iFace].v[iVertex]);

    if ((rvert->rFlags & NORCT_MASK) > 1)
    {
            for (unsigned int i = 0; i < (rvert->rFlags & NORCT_MASK); i++)
            {
                if (rvert->ern[i].getSmGroup() == m_pMesh->faces[iFace].smGroup)
                {
                    n = rvert->ern[i].getNormal();
                    n.Normalize();
                    eResult = RWCOMM_SUCCESS;
                    break;
                }
            }
    }

    if (eResult != RWCOMM_SUCCESS)
    {
        n = rvert->rn.getNormal();
        n.Normalize();
        eResult = RWCOMM_SUCCESS;
    }

    if (bWorldSpace)
    {
        normal = Point3ToRwCommVec3(n * m_worldNormalMat);
    }
    else
    {
        normal = Point3ToRwCommVec3(n * m_offsetNormalMat);
    }
   

    return eResult;
}

RwCommError 
MaxSimpleMesh::GetVertexColor(int iFace, int iVertex, RwCommVec3& color) const
{
    RwCommError r = RWCOMM_FAIL;

    if (m_pMesh->vcFace) {
        r = RWCOMM_SUCCESS;

        int iColorIndex = m_pMesh->vcFace[iFace].t[iVertex];

        color = Point3ToRwCommVec3(m_pMesh->vertCol[iColorIndex]);

        color[0] = static_cast<RwCommReal>(floor(color[0]*255.0f)/255.0f);
        color[1] = static_cast<RwCommReal>(floor(color[1]*255.0f)/255.0f);
        color[2] = static_cast<RwCommReal>(floor(color[2]*255.0f)/255.0f);
    }
    return r;
}   

RwCommError 
MaxSimpleMesh::GetVertexAlpha(int iFace, int iVertex, RwCommReal& alpha) const
{
    RwCommError hResult = RWCOMM_FAIL;

    if (m_pMesh->mapSupport(MAP_ALPHA)) {
        int nVerts = m_pMesh->getNumMapVerts(MAP_ALPHA);
        const UVVert *tVerts = m_pMesh->mapVerts(MAP_ALPHA);
        int vertIndex = m_pMesh->mapFaces(MAP_ALPHA)[iFace].t[iVertex];

        if (vertIndex >= 0 && vertIndex < nVerts)
        {
            alpha = tVerts[vertIndex].x;
            hResult = RWCOMM_SUCCESS;
        }
    }

    return hResult;
}

RwCommError 
MaxSimpleMesh::GetVertexIllumination(int iFace, int iVertex, RwCommVec3& vIllum) const
{
    RwCommError hResult = RWCOMM_FAIL;

    if (m_pMesh->mapSupport(MAP_SHADING)) {
        int nVerts = m_pMesh->getNumMapVerts(MAP_SHADING);
        const UVVert *tVerts = m_pMesh->mapVerts(MAP_SHADING);
        int vertIndex = m_pMesh->mapFaces(MAP_SHADING)[iFace].t[iVertex];

        if (vertIndex >= 0 && vertIndex < nVerts)
        {
            vIllum = Point3ToRwCommVec3(tVerts[vertIndex]);
            hResult = RWCOMM_SUCCESS;
        }
    }

    return hResult;
}

RwCommError 
MaxSimpleMesh::GetVertexUVs(int iFace, int iVertex, RwVector< RwCommVec2 >& uvs) const
{
    RwCommShader* pShader = GetFaceShader(iFace);
    if (pShader)
    {
        for (int i = 0; i < pShader->GetNumTextures(); i++)
        {
            int channel = 0;
            RwCommTexture* pTexture = pShader->GetTexture(i);
            if (pShader->GetTextureUVSet(pTexture, channel) == RWCOMM_SUCCESS)
            {
                if (m_pMesh->mapSupport(channel)) {
                    const UVVert *tVerts = m_pMesh->mapVerts(channel);
                    const TVFace *tvFaces = m_pMesh->mapFaces(channel);

                    UVVert uv = tVerts[tvFaces[iFace].t[iVertex]];

                    if (uvs.size() < static_cast<unsigned int>(channel))
                    {
                        uvs.resize(channel);
                    }
                    uvs[channel-1] = RwCommVec2( uv.x, uv.y );
                }
            }
        }
    }

    return RWCOMM_SUCCESS;
}

RwCommShader* 
MaxSimpleMesh::GetFaceShader(int iFaceIndex) const
{
    RwCommShader* pShader = NULL;

    if (m_vShaders.size() > 0)
    {
        // We only have one shader
        if (m_vShaders.size() == 1)
        {
            int matID = m_pMesh->getFaceMtlIndex(iFaceIndex) % 1;
            if (matID == 0)
            {
                pShader = RwCommGeometry::GetShader(0);
            }
        }
        else
        {
            Mtl* pMtl = m_pINode->GetMtl();
            int nSubs = pMtl->NumSubMtls();
            if (nSubs > 0)
            {
                int matID = m_pMesh->getFaceMtlIndex(iFaceIndex) % nSubs;
                Mtl *pSubMtl = pMtl->GetSubMtl(matID);

                // Search for the corresponding Mtl in the shader list
                for (int i = 0; i < GetNumShaders() && !pShader; i++)
                {
                    MaxSimpleShader *pSimpleShader = dynamic_cast<MaxSimpleShader*>(GetShader(i));
                    if (pSimpleShader)
                    {
                        if (pSimpleShader->GetMaterial() == pSubMtl)
                        {
                            // Found our Mtl object
                            pShader = pSimpleShader;
                        }
                    }
                }

            }
        }
    }

    return pShader;
}

RwCommBoundingBox 
MaxSimpleMesh::Box3ToRwCommBoundingBox(const Box3& b) const
{
    RwCommBoundingBox box;
    box.min = Point3ToRwCommVec3(b.pmin);
    box.max = Point3ToRwCommVec3(b.pmax);

    return box;
}

RwCommVec3 
MaxSimpleMesh::Point3ToRwCommVec3(const Point3& p) const
{
    return RwCommVec3(p.x, p.y, p.z);
}

Matrix3 
MaxSimpleMesh::RwCommMatrixToMatrix3(const RwCommMatrix& m) const
{
    Matrix3 ret(TRUE);

    for (int i = 0; i < 4; i++)
    {
        Point3 point(m[i][0], m[i][1], m[i][2]);
        ret.SetRow(i, point);
    }
    
    return ret;
}

Matrix3
MaxSimpleMesh::GetObjOffsetMatrix(INode *iNode)
{
    Quat        quat;
    Point3      pos;
    Matrix3     offsetMatrix(TRUE);
    ScaleValue  scaleValue;
    
    pos = iNode->GetObjOffsetPos();
    offsetMatrix.PreTranslate(pos);
    quat = iNode->GetObjOffsetRot();
    PreRotateMatrix(offsetMatrix, quat);
    scaleValue = iNode->GetObjOffsetScale();
    ApplyScaling(offsetMatrix, scaleValue);

    return offsetMatrix;
}


//Class RwMaxMeshCreator

MaxSimpleMeshCreator::MaxSimpleMeshCreator() : RwCommObjCreator()
{

}

MaxSimpleMeshCreator::~MaxSimpleMeshCreator()
{

}

/** Creates an RwMaxMesh from an INode.*/
RwCommObject* 
MaxSimpleMeshCreator::Create(const RwCommCreationData* pCreationData, const RwCommExportOptions* pExportOptions) const
{
    RwCommObject* pObject = NULL;

    const MaxSimpleCreationData* pData = dynamic_cast<const MaxSimpleCreationData*>(pCreationData);
    if (pData)
    {
        pObject = new MaxSimpleMesh(pData->m_pNode);
    }

    return pObject;
}

/** Sets the geometry's name.*/
void
MaxSimpleMeshCreator::Init(RwCommObject* pObject,
                           const RwCommCreationData* pCreationData,
                           const RwCommExportOptions* pExportOptions) const
{
    const MaxSimpleCreationData* pData = dynamic_cast<const MaxSimpleCreationData*>(pCreationData);
    if (pData)
    {
        INode* pNode = pData->m_pNode;
        RwWString name = RwUtilA2W(pNode->GetName());
        name += L"Shape";
        pObject->SetName(name);
    }
}

/** 
*   Attaches the RwCommGeometry to its transform using the RwCommTransform::SetShape() method.
*   In addition it adds all the RwCommShader objects to the geometry using the RwCommGeometry::AddShader()
*   method.*/
void
MaxSimpleMeshCreator::Attach(RwCommObject* pObject,
                             RwCommObject* pParent, RwCommScene* pScene, const RwCommCreationData* pCreationData) const
{
    RwCommGeometry* pGeometry = dynamic_cast<RwCommGeometry*>(pObject);

    if (pGeometry)
    {
        RwCommTransform* pTransform = dynamic_cast<RwCommTransform*>(pParent);
        if (pTransform)
        {
            pTransform->SetShape(pGeometry);
        }
    }
}


