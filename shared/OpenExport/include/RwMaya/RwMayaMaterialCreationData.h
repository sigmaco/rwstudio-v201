#if !defined(RW_MAYA_MATERIAL_CREATION_DATA_H)
#define RW_MAYA_MATERIAL_CREATION_DATA_H

#include "stdafx.h"

#include "RwMaya.h"
#include "RwCommCreationData.h"
#include "RwMayaCreationData.h"

/**
*   \ingroup mayabuilder 
*
*   This class holds the Maya shader creation data. In addition to the MObject and MDagPath
*   RwMayaMaterialCreationData holds the Maya set this shader is connected to. Information
*   about the set comes from RwMayaTraverseMaterials::Traverse(), which is being called in
*   RwMayaMeshCreator::Traverse() and RwMayaNurbsSurface::Traverse()
*/
class RWMAYA_API RwMayaMaterialCreationData : public RwMayaCreationData
{
public:
    RwMayaMaterialCreationData(MObject object, MDagPath dagPath, MObject set);
	virtual ~RwMayaMaterialCreationData();

    /** \return The Maya set this shader is connected to */
    MObject     GetSet() const { return m_Set; }

protected:
    MObject  m_Set;
};


#endif // !defined(RW_MAYA_MATERIAL_CREATION_DATA_H)