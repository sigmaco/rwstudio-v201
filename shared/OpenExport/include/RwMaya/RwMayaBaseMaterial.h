#if !defined(RW_MAYA_BASE_MATERIAL_H)
#define RW_MAYA_BASE_MATERIAL_H

/**
*  \ingroup mayascenegraph 
*
*  This is base RwMaya material class. It is contains functionality common to
*  RwMayaMaterial and RwMayaRwMaterial.
*
*   \see RwMayaMaterial
*   \see RwMayaRwMaterial
*/
class RWMAYA_API RwMayaBaseMaterial
{
public:
	RwMayaBaseMaterial(MObject shader, MObject set);
	virtual ~RwMayaBaseMaterial();

    /**
     *  This function is used to generate a mapping from the textures used by the
     *  shader to the UV sets of the Maya shape and the UV sets of the output
     *  RenderWare object.
     *  
     *  Before entry the m_vUvTextures member is expected to have been filled in
     *  with all the texture which need mapping.
     *  
     *  On exit m_vMayaUvSets will be filled in with the index of Maya UV set used
     *  by each member of m_vUvTextures. m_vOutputUvSets will be filled in with the output
     *  UV set index to be used for each member of m_vUvTextures. m_vUniqueMayaUvSets will be
     *  filled in with an entry for each output UV set recording the index of the Maya
     *  UV set it came from. The size of m_vUniqueMayaUvSets can thus be used as the number
     *  of UV sets that will be required in the ouput geometry.
     * 
     *  \param object the Maya mesh or nurbs surface for which UV sets are to be mapped
     *
     */
    void            MapUVSets(MObject object);

    /**
     *  This function retrieves the Maya shader object this class encapulates.
     * 
     *  \returns object the Maya shader object
     */
    MObject         GetShaderObject() const { return m_ShaderObject; }

    /**
     *  This function retrieves the Maya set object this class encapulates.
     * 
     *  \returns object the Maya shader set
     */
    MObject         GetSetObject() const {return m_Set; }
    
    // List of indices of Maya UV sets with duplicates removed. This
    // is a map from the RW UV set index to the Maya UV set index.
    RwVector<int>       m_vUniqueMayaUvSets;

protected:

	MObject                 m_ShaderObject;

    MObject                 m_Set;
    
    // Textures in this material that require UVs
    RwVector<MObject>   m_vUvTextures;

    // Per-texture list of the index of the Maya mesh UV set used
    RwVector<int>       m_vMayaUvSets;

    // Per-texture list of the index of the output UV set used.
    RwVector<int>       m_vOutputUvSets;
};

#endif // !defined(RW_MAYA_BASE_MATERIAL_H)
