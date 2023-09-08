#if !defined(RW_MAYA_TRAVERSE_MATERIALS_H)
#define RW_MAYA_TRAVERSE_MATERIALS_H

#pragma warning(disable : 4786)
#pragma warning(disable : 4251)

class RwMayaBuilder;
class RwCommCreationData;

/**
*  \ingroup mayabuilder 
*
*  This class traverses all materials connected to the given dag node.
*
*/
class RWMAYA_API RwMayaTraverseMaterials  
{
public:
	RwMayaTraverseMaterials();
	virtual ~RwMayaTraverseMaterials();

	virtual void Traverse(RwCommBuilder &rBuilder, RwCommCreationData *pCreationData, RwCommObject *pParent);

};

#endif // !defined(RW_MAYA_TRAVERSE_MATERIALS_H)
