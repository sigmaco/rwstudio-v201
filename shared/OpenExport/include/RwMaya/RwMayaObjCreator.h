#if !defined(RW_MAYA_OBJ_CREATOR_H)
#define RW_MAYA_OBJ_CREATOR_H

/**
*  \ingroup mayabuilder  
*   This is an abstract base class which adds and extra function
*   to the RwCommObjCreator interface. All Maya object creators
*   derive from this class rather than RwCommObjCreator. The
*   RwMayaBuilder::BuildObject function calls Traverse() after
*   calling the Create, Init and Attach members of RwCommObjCreator.
*
*   \see RwCommObjCreator
*/
class RWMAYA_API RwMayaObjCreator : public RwCommObjCreator
{
public:
	RwMayaObjCreator();
	virtual ~RwMayaObjCreator();

    /**
     *  This method is called by the RwMayaBuilder::BuildObject function. It gives
     *  the creator class control on how and what objects to traverse to after building
     *  and RwComm scene graph object. Most implementations will simply create
     *  RwMayaTraverseAnimations and/or RwMayaTraverseMaterials objects and call their
     *  Traverse member functions.
     * 
     *  \param rBuilder the instance of RwMayaBuilder calling this function.
     *  \param pCreationData a pointer to a RwMayaCreationData class containing the MObject being traverse from.
     *  \param pParent a pointer to the parent of the object being traversed from
     *
     */
    virtual void Traverse(  RwCommBuilder       &rBuilder,
                            RwCommCreationData  *pCreationData,
                            RwCommObject        *pParent) const = 0;
};

#endif // !defined(RW_MAYA_OBJ_CREATOR_H)
