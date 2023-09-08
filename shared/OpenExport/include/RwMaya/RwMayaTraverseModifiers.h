#if !defined(RW_MAYA_TRAVERSE_MODIFIERS_H)
#define RW_MAYA_TRAVERSE_MODIFIERS_H

#pragma warning(disable : 4786)
#pragma warning(disable : 4251)

class RwMayaBuilder;
class RwCommCreationData;

/**
*  \ingroup mayabuilder 
*
*  This class traverses all upstream geometry filter deformers connected to
*  the given dag node.
*
*/
class RWMAYA_API RwMayaTraverseModifiers  
{
public:
	RwMayaTraverseModifiers();
	virtual ~RwMayaTraverseModifiers();

	virtual void Traverse(RwCommBuilder &rBuilder, RwCommCreationData *pCreationData, RwCommObject *pParent);

};

#endif // !defined(RW_MAYA_TRAVERSE_MODIFIERS_H)
