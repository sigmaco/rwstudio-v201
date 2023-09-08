// ScaleAnim.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

#include "RwExpAnmExporter.h"
#include "RwExpDffExporter.h"

#include "RwCommScene.h"
#include "RwCommTransform.h"
#include "RwCommMessage.h"

#include "RwExpUtils.h"
#include "RwExpRpGeometry.h"
#include "RwExpRpPatch.h"
#include "RwExpPositionOps.h"

// Must include this header for version compatability
#include "RwCommVersion.h"

#include "scalekey.h"
#include "ScaleAnim.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

bool InitRwPlugin()
{
    return true;
}

void RegisterCustomExporterObjects(RwCommExporter *pExporter)
{
    RwExpDffExporter *pDffExporter = dynamic_cast<RwExpDffExporter *>(pExporter);
    if (pDffExporter)
    {
        // Register the new Frame exporter in the Dff exporter
        pDffExporter->RegisterExportObject(RwCommTransformType, RwScaleAnimFrame);

        // Override the RwExpHAnimHierarchy action in the Dff exporter
        pDffExporter->ReplaceAction(RwExpHAnimHierarchyType, new RwExpScaleHAnimHierarchy);

        // Override the scale filter in geometries and patches
        RwExpRpGeometry::GetVertexPipeline().ReplaceFilter(RwExpScaleFilterType, new RwExpScaleAnimScaleFilter);
        RwExpRpPatch::GetVertexPipeline().ReplaceFilter(RwExpScaleFilterType, new RwExpScaleAnimScaleFilter);

        // Similarly for the offset filter
        RwExpRpGeometry::GetVertexPipeline().ReplaceFilter(RwExpOffsetFilterType, new RwExpScaleOffsetFilter);
        RwExpRpPatch::GetVertexPipeline().ReplaceFilter(RwExpOffsetFilterType, new RwExpScaleOffsetFilter);
    }

    RwExpAnmExporter *pAnmExporter = dynamic_cast<RwExpAnmExporter *>(pExporter);
    if (pAnmExporter)
    {
        // Register the new Matrix Animation exporter in the Anm exporter
        pAnmExporter->RegisterExportObject(RwCommTransformType, RwScaleMatrixAnimation);

        // Override the RwExpBuildHAnimAnimation action in the Anm exporter
        pAnmExporter->ReplaceAction(RwExpBuildHAnimAnimationType, new RwExpBuildScaleHAnimAnimation);
    }
}

// Register the new RpHAnim keyframe type
void AttachCustomRwPlugin()
{
    if (!HAnimFullKeyFrameRegister())
    {
        PRINT_MESSAGE("HAnimFullKeyFrameRegister failed!", "", 0);
    }
    return;
}

//////////////////////////////////////////////////////////////////////
// RwScaleAnimFrame
//////////////////////////////////////////////////////////////////////

// Override CalcTransformMatrix to leave scale in the matrix.
RwV3d        
RwScaleAnimFrame::CalcTransformMatrix(RwCommTransform *pTransform, RwMatrix *matrix,
                        RwCommReal scale, bool bSetRootIdent)
{
    RwCommMatrix        commMatrix;
    RwCommTransform     *pParentXForm = dynamic_cast<RwCommTransform*>(pTransform->GetParent());

    // Get the local matrix for the transform (including scale)
    pTransform->GetObjMatrix(commMatrix);

    // Convert it to a RW matrix without normalization
    RwExpCommMatrixToRwMatrix(matrix, commMatrix, false);

    // We apply some special cases to the root node
    if (pParentXForm == NULL)
    {
        RwV3d scaleVec = {1.0f, 1.0f, 1.0f};
        
        // May have been asked to clear the root
        if (bSetRootIdent)
        {
            scaleVec = RwExpMatrixGetScale(matrix);

            RwMatrixSetIdentity(matrix);
        }

        // Rather than burning the global scale into each node we simply apply it once
        // to the root
        scaleVec.x *= scale;
        scaleVec.y *= scale;
        scaleVec.z *= scale;
        RwMatrixScale(matrix, &scaleVec, rwCOMBINEPOSTCONCAT);
    }

    // Update the matrix
    RwMatrixUpdate(matrix);

    // No scale to burn into geometry
    RwV3d retVal = {1.0f, 1.0f, 1.0f};
    return retVal;
}

//////////////////////////////////////////////////////////////////////
// RwScaleMatrixAnimation
//////////////////////////////////////////////////////////////////////

// Override CalcTransformMatrix to leave scale in the matrix.
RwV3d        
RwScaleMatrixAnimation::CalcTransformMatrix(RwCommTransform *pTransform, RwMatrix *matrix,
                        RwCommReal scale, bool bSetRootIdent)
{
    RwCommMatrix        commMatrix;
    RwCommTransform     *pParentXForm = dynamic_cast<RwCommTransform*>(pTransform->GetParent());

    // Get the local matrix for the transform (including scale)
    pTransform->GetObjMatrix(commMatrix);

    // Convert it to a RW matrix without normalization
    RwExpCommMatrixToRwMatrix(matrix, commMatrix, false);

    // We apply some special cases to the root node
    if (pParentXForm == NULL)
    {
        RwV3d scaleVec = {1.0f, 1.0f, 1.0f};
        
        // May have been asked to clear the root
        if (bSetRootIdent)
        {
            scaleVec = RwExpMatrixGetScale(matrix);

            RwMatrixSetIdentity(matrix);
        }

        // Rather than burning the global scale into each node we simply apply it once
        // to the root
        scaleVec.x *= scale;
        scaleVec.y *= scale;
        scaleVec.z *= scale;
        RwMatrixScale(matrix, &scaleVec, rwCOMBINEPOSTCONCAT);
    }

    // Update the matrix
    RwMatrixUpdate(matrix);

    // No scale to burn into geometry
    RwV3d retVal = {1.0f, 1.0f, 1.0f};
    return retVal;
}

// Override SetHAnimFrame so that it writes scale values into the new keyframe type
void *
RwScaleMatrixAnimation::SetHAnimFrame(const RwExpAnimKey &animKey, RwCommReal startTime, 
                                    RpHAnimAnimation *pHAnim, int frame, void *pPrevFrame)
{
    void *pKeyFrame = NULL;
    int index = -1;
    if (m_pTransform && m_vAnimKeys.size() > 0)
    {
        if (FindKey(animKey, 0, m_vAnimKeys.size()-1, index) != NULL)
        {
            HAnimFullKeyFrame* hAnimFrames = (HAnimFullKeyFrame *)(pHAnim->pFrames);
            RwCommScene* pScene = m_pTransform->GetScene();

            hAnimFrames[frame].time        = pScene->FramesToSec(animKey.m_time - startTime);
            hAnimFrames[frame].q.imag      = m_vMatKeys[index].m_quat.imag;
            hAnimFrames[frame].q.real      = m_vMatKeys[index].m_quat.real;
            hAnimFrames[frame].pos         = m_vMatKeys[index].m_matrix.pos;

            // Save the scale from the keyframe matrix
            hAnimFrames[frame].scale       = RwExpMatrixGetScale(&m_vMatKeys[index].m_matrix);

            hAnimFrames[frame].prevFrame   = (HAnimFullKeyFrame*)pPrevFrame; 
            pKeyFrame = &(hAnimFrames[frame]);
        }
    }

    return pKeyFrame;
}


//////////////////////////////////////////////////////////////////////
// RwExpScaleHAnimHierarchy
//////////////////////////////////////////////////////////////////////

// Override CreateHAnimHierarchy to ensure that the RpHAnimHierarchy created has
// enough space for an array of the new keyframe type.
RpHAnimHierarchy *
RwExpScaleHAnimHierarchy::CreateHAnimHierarchy(RwInt32 numNodes, RwUInt32 *nodeFlags,
                       RwInt32 *nodeIDs, RpHAnimHierarchyFlag flags,
                       RwInt32 maxKeyFrameSize)
{
    return RpHAnimHierarchyCreate(numNodes, nodeFlags, nodeIDs, flags, sizeof(HAnimFullKeyFrame));
}

//////////////////////////////////////////////////////////////////////
// RwExpBuildScaleHAnimAnimation
//////////////////////////////////////////////////////////////////////
RwExpBuildScaleHAnimAnimation::RwExpBuildScaleHAnimAnimation() : RwExpBuildHAnimAnimation()
{
}

// Override CreateHAnimAnimation to create an animation using the new scale keyframe type.
RpHAnimAnimation *
RwExpBuildScaleHAnimAnimation::CreateHAnimAnimation(int nFrames, RwCommReal animTimeInterval)
{
    return RpHAnimAnimationCreate(HAnimFullKeyFrameID, nFrames, 0, animTimeInterval);
}

//////////////////////////////////////////////////////////////////////
// RwExpScaleAnimScaleFilter
//////////////////////////////////////////////////////////////////////

// Override SetScale so that it does nothing, leaving the scale to be
// burnt into geometry at its default value of 1,1,1
void
RwExpScaleAnimScaleFilter::SetScale(const RwCommVec3& scale)
{
}

//////////////////////////////////////////////////////////////////////
// RwExpScaleOffsetFilter
//////////////////////////////////////////////////////////////////////

// Override SetScale so that it does nothing, leaving the scale to be
// burnt into geometry at its default value of 1,1,1
void
RwExpScaleOffsetFilter::SetScale(RwCommReal scale)
{
}

// Override RemoveMatrixScale so that it does nothing, leaving scale in
// the offset matrices
void
RwExpScaleOffsetFilter::RemoveOffsetMatrixScale(RwCommMatrix &matrix)
{
}