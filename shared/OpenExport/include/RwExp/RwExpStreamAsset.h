// RwExpStreamAsset.h: interface for the RwExpStreamAsset class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWEXPSTREAMASSET_H__7823C909_44AD_4C82_88AA_7DF328D63532__INCLUDED_)
#define AFX_RWEXPSTREAMASSET_H__7823C909_44AD_4C82_88AA_7DF328D63532__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwCommPlatform.h"
#include "RwCommPrimitives.h"
#include "RwExp.h"

// =============================================================================
// ----------------------------  RwExpAsset CLASS  -----------------------------
// =============================================================================

/**
*  \ingroup structures
*   This class represents a standard stream asset in the StreamAssetList. The assets that are currently supported are:
*   \li rwID_WORLD
*   \li rwID_CLUMP
*   \li rwID_HANIMANIMATION
*   \li rwID_DMORPHANIMATION
*   \li rwID_SPLINE
*   \li rwID_MTEFFECTDICT
*   \li rwID_PITEXDICTIONARY
*   \li rwID_UVANIMDICT
*
*   Users can create their own custom stream assets by deriving from the RwExpStreamAsset class and implementing the
*   StreamWrite() and StreamToFile() virtual methods.
*
*   \see RwExpStreamAssetList
*/
class RWEXP_API RwExpStreamAsset
{
public:
    /**
    *   Construct an asset.
    *
    *   \param newType the type ID of the asset (RenderWare type).
    *   \param newData a pointer to the asset.
    *   \param newFileName file name used when calling the StreamToFile() method.
    *   \param guid an optional GUID string, that is streamed out in the TOC, for each asset.
    *   \param assetName an optional name for this asset. This will be streamed to the named group chunks.
    *   \param platform the target platform of the stream asset
    *   \param canBeInstanced if the stream asset can be instanced or not
    */
    RwExpStreamAsset(unsigned int newType, void *newData, RwWString &newFileName,
                     RwWString guid = L"", RwWString assetName = L"",
                     RwCommPlatform platform = RwCommPlatformDefault, bool canBeInstanced = true);

    virtual ~RwExpStreamAsset();

    /**
     *  Stream out the asset to a RenderWare stream. Users implementing this method should use the
     *  standard RenderWare streaming utilities for doing so.
     * 
     *  \param stream to be filled in with the user data.
     */
    virtual void    StreamWrite(RwStream *stream);

    /**
     *  Stream out an asset, including any group chunks and children. This function first streams
     *  a begin group chunk (if enabled), followed by the asset itself (by calling StreamWrite), then
     *  any children and finally an end group chunk.
     * 
     *  \param stream to be filled in with the user data.
     *  \param bGroupChunks whether group chunks should be streamed.
     */
    virtual void    StreamWriteGroup(RwStream *stream, bool bGroupChunks = false);

    /**
     *  Stream out the asset to a file. Use the m_fileName member to obtain the file name.
     */
    virtual void    StreamToFile();

    /**
     *  Get the plugin ID for this stream asset.
     * 
     *  \return the plugin id of the data contained in this stream asset.
     */
    unsigned int GetPluginID() const { return m_type; }

    /**
     *  Get a pointer to the asset's data.
     * 
     *  \return pointer to the data contained by this stream asset.
     */
    void    *GetData() const { return m_pData; }

    /**
     *  Get the asset name.
     * 
     *  \return the asset name.
     */
    const RwWString &GetName() const { return m_fileName; }

    /**
     *  Get the asset file name.
     * 
     *  \return the asset file name.
     */
    const RwWString &GetFileName() const { return m_fileName; }

    /**
     *  Get the asset GUID.
     * 
     *  \return the GUID of this stream asset.
     */
    const RwWString &GetGuid() const { return m_guid; }

    /**
     *  Fill in RtTOC GUID for this asset and any children. The index of the entry to be
     *  filled is passed in by pEntry. After the entry has been filled in the int pointed
     *  to by pEntry should be incremented.
     * 
     *  \param pToc pointer to the toc to be filled in.
     *  \param pEntry the entry to be filled in.
     */
    void                FillTocEntry(RtTOC *pToc, int *pEntry);

    /**
     *  Write a chunk group start chunk to a stream. The name an GUID used should be those
     *  of this stream asset.
     * 
     *  \param stream the stream to add the chunk to.
     */
    /**  */
    void                StreamChunkGroupStart(RwStream *stream);

    /**
     *  Write a chunk group end chunk to a stream.
     * 
     *  \param stream the stream to add the chunk to.
     */
    void                StreamChunkGroupEnd(RwStream *stream);

    /**
     *  Add another asset as a child of this one.
     * 
     *  \param pChild the asset to be added.
     */
    void                AddChild(RwExpStreamAsset *pChild);

    /**
     *  Remove an existing child asset.
     * 
     *  \param i the index of the child to be removed.
     */
    void                RemoveChild(int i);

    /**
     *  Get the number of child assets.
     * 
     *  \return the number of children.
     */
    int                 GetNumChildren() const;

    /**
     *  Get a child asset.
     * 
     *  \param i the index of the child to be retrieved.
     *  \return pointer to the child if i is in range, NULL otherwise.
     */
    RwExpStreamAsset    *GetChild(int i) const;

    /**
     *  Determines if the asset stream can be instanced
     *  \return if the asset can be instanced or not
     */
    bool CanBeInstanced(void) const { return m_bCanBeInstanced; }

protected:

    bool                         m_bCanBeInstanced;
    unsigned int                 m_type;
    void                         *m_pData;
    RwWString                    m_name;
    RwWString                    m_fileName;
    RwWString                    m_guid;
    RwCommPlatform               m_platform;
    RwVector<RwExpStreamAsset *> m_vChildren;
};

typedef RwList<RwExpStreamAsset *>::iterator rwExpAssetListIt;

// =============================================================================
// --------------------------  RwExpAssetList CLASS  ---------------------------
// =============================================================================

/**
*  \ingroup structures
*   The exporters build the appropriate RenderWare Graphics structures and add them to the stream asset list.
*   The stream asset list is a collection of RenderWare Graphics primitives, such as RpClumps, RpWorlds, 
*   RpHAnimHierarchy etc. These primitives are created in the exporters, appended to the asset list and streamed out
*   to a file (usually an .RWS file) at the end of the export process.
*
*   \see RwExpStreamAsset
*/
class RWEXP_API RwExpStreamAssetList
{
public:
    RwExpStreamAssetList();
    ~RwExpStreamAssetList();

    /**
    *  Remove all stream assets from the list.
    */
    void    ClearAssets();

    /**
    *  Add an asset to the list.
    */
    void    AddEntry(RwExpStreamAsset *asset);

    /**
    *  Get the list of assets.
    */
    const RwList<RwExpStreamAsset *> &GetAssets() const { return m_assets; }

    /**
    *  Find an asset from a GUID. Returns NULL if the GUID was not found.
    */
    RwExpStreamAsset *FindAssetByGuid(RwWString sGuid) const;

    // Streaming methods
    void    StreamToc(RwStream *stream, bool bGroupChunks = false);

    /**
    *   Write the asset list to a RenderWare stream. 
    *
    *   \param stream a valid open RenderWare stream
    *   \param bToc if this flag is set to true then a TOC chunk will be inserted at the beginning of the stream.
    *   \param bGroupChunks if this flag is set to true then a group chunks will be added to the stream.
    *
    *   \see RwExpStreamAssetList::StreamToFile()
    */
    void    StreamWrite(RwStream *stream, bool bToc, bool bGroupChunks = false);

    /**
    *   Stream out the asset list.
    *
    *   \param fileName the output file name.
    *   \param bToc if this flag is set to true then a TOC chunk will be inserted at the beginning of the stream.
    *   \param bGroupChunks if this flag is set to true then a group chunks will be added to the stream.
    */
    void    StreamToFile(RwWString fileName, bool bToc, bool bGroupChunks = false);

    /**
    *   Stream out the asset list to multiple files. Uses the file name member in each RwExpStreamAsset object.
    *
    */
    void    StreamToFiles();

    // Texture methods
    int     GetNumTextures() const { return m_textures.size(); }
    void    AddTexture(const RwWString &name);
    void    AddUnresolvedTexture(const RwWString &name);
    void    AddLightmapTexture(RwTexture *pTexture);

    const RwVector<RwWString>  &GetTextures() { return m_textures; }
    const RwVector<RwWString>  &GetUnresolvedTextures() { return m_unresolvedTextures; }
    const RwVector<RwTexture *>   &GetLightmapTextures() { return m_lightmapTextures; }

    int                         GetRwLightmapCount() { return m_rwLightmapCount; }
    void                        SetRwLightmapCount(int count) { m_rwLightmapCount = count; }
    
    // Effect files
    RwVector<RwWString>  &GetEffectFiles() { return m_effectFiles; }
    void                        AddEffectFile(const RwWString &file);
    
    // Tetxure dictionary
    RwTexDictionary         *GetTextureDictionary() { return m_pTextureDictionary; };
    void                    SetTextureDictionary(RwTexDictionary *pTextureDictionary)
                                { m_pTextureDictionary = pTextureDictionary; };

protected:

private:
    RwList<RwExpStreamAsset *>  m_assets;
    RwTexDictionary                 *m_pTextureDictionary;

    RwVector<RwWString>      m_textures;
    RwVector<RwWString>      m_unresolvedTextures;
    RwVector<RwTexture *>       m_lightmapTextures;
    RwVector<RwWString>      m_effectFiles;
    int                             m_rwLightmapCount;
};

#endif // !defined(AFX_RWEXPSTREAMASSET_H__7823C909_44AD_4C82_88AA_7DF328D63532__INCLUDED_)
