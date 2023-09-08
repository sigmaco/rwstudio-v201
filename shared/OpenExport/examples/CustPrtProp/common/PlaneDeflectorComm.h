#ifndef _PLANEDEFLECTORCOMM__H
#define _PLANEDEFLECTORCOMM__H

#include "CustPrtPropComm.h"
#include "RwComm.h"
#include "RwCommError.h"
#include "RwCommPrimitives.h"

// simple class for a particle system plane deflector
class CUSTPRTPROP_COMM_API PlaneDeflectorComm
{
public:
    PlaneDeflectorComm();
	virtual ~PlaneDeflectorComm();

    // The interface to the common layer
    virtual RwCommError GetPlane(RwCommVec3 &normal, RwCommVec3 &position) const = 0;
};

#endif