// RwMaxScene.cpp: implementation of the RwMaxScene class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MaxSimpleScene.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MaxSimpleScene::MaxSimpleScene()
{

}

MaxSimpleScene::~MaxSimpleScene()
{

}

RwCommReal 
MaxSimpleScene::FramesToSec(RwCommReal frame) const
{
    return TicksToSec(frame * GetTicksPerFrame());
}

/**
*   Returns the max global axis system.
*   The method returns the following matrix:
*
*   1  0   0
*   0  0  -1
*   0  1   0
*   0  0   0
*
*/
RwCommError
MaxSimpleScene::GetAxisSystem(RwCommMatrix &matrix) const
{
    matrix.SetRow(0, RwCommVec4( 1, 0, 0, 0 ));
    matrix.SetRow(1, RwCommVec4( 0, 0,-1, 0 ));
    matrix.SetRow(2, RwCommVec4( 0, 1, 0, 0 ));
    matrix.SetRow(3, RwCommVec4( 0, 0, 0, 1 ));

    return RWCOMM_SUCCESS;
}

RwCommReal
MaxSimpleScene::GetStartFrame() const
{
    return (RwCommReal) (GetCOREInterface()->GetAnimRange().Start() / GetTicksPerFrame());
}

RwCommReal
MaxSimpleScene::GetEndFrame() const
{
    return (RwCommReal) (GetCOREInterface()->GetAnimRange().End() / GetTicksPerFrame());
}
