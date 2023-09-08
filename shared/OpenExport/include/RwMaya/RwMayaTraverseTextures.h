#if !defined(RW_MAYA_TRAVERSE_TEXTURES_H)
#define RW_MAYA_TRAVERSE_TEXTURES_H

#pragma warning(disable : 4786)
#pragma warning(disable : 4251)

class RwMayaBuilder;
class RwCommCreationData;

/**
*  \ingroup mayabuilder 
*
*  This class traverses all upstream nodes of type file.
*/
class RWMAYA_API RwMayaTraverseTextures
{
public:
	RwMayaTraverseTextures();
	virtual ~RwMayaTraverseTextures();

	virtual void Traverse(RwCommBuilder &rBuilder, RwCommCreationData *pCreationData, RwCommObject *pParent);

};

#endif // !defined(RW_MAYA_TRAVERSE_TEXTURES_H)
