// RwRf3XMLParser.h: interface for the RwRf3XMLParser class.
//
//////////////////////////////////////////////////////////////////////

#ifndef RWRF3XMLPARSER_H
#define RWRF3XMLPARSER_H

#include "RwRf3Core.h"
#include "RwXMLDOM.h"

/**
*   \ingroup rf3helper 
*
*   Classes, deriving from the RwXMLNode, should be registered with the RwRf3XMLParser by deriving
*   from the RwRf3XMLNodeCreator and implementing its RwRf3XMLNodeCreator::Create() method.
*
*   \see RwRf3XMLParser
*/
class RWRF3CORE_API RwRf3XMLNodeCreator
{
public:
    RwRf3XMLNodeCreator();
    virtual ~RwRf3XMLNodeCreator();

    /**
    *   This method should create the custom RwXMLNode and return a pointer to it.
    *
    *   \param parentNode the XML parent node. Classes, implementing this method, should attach the created
    *           node to its parent node
    *
    *   \returns the created custom RwXMLNode
    */
    virtual RwXMLNode* Create( RwXMLNode* parentNode = NULL ) const = 0;
};


/**
*   \ingroup rf3helper 
*
*   This is the XML parser class. The class reads in an XML file and returns a tree of RwXMLNode objects.
*   Users can derive their own classes from the RwXMLNode and register them with the parser using the
*   RwRf3XMLNodeCreator class. The parser will then create their custom RwXMLNode objects from the XML file
*   once these objects are registered.
*
*   \see RwRf3XMLNodeCreator
*/
class RWRF3CORE_API RwRf3XMLParser
{
public:
    
    //default constructor
    RwRf3XMLParser();

    //destructor
    ~RwRf3XMLParser();

    // load an xml file into a xml node
    bool Load( RwXMLNode* node, const RwWString& filename , const RwWString &filter = L"");

    // load an xml file
    RwXMLNode* Load( const RwWString &filename , const RwWString &filter = L"");

    //parses the given buffer into the given node
//    bool ParseFrom( RwXMLNode& lNode, const char* lpcBuffer, unsigned int length );

    //creates a new node from a given buffer
//    RwXMLNode* ParseFrom( const char* lpcBuffer, unsigned int length );

    //gets the parser error message
    const wchar_t* GetErrorMessage() const;

    unsigned int GetLineCount() const { return lineCount; }

    //expands tokens into their xml escape codes
//    static bool ExpandSubstitutes( char* lpcDst, const char* lpcSrc, bool lbInText );

    void MapNode(RwXMLNode* pNode);
    RwXMLNode *GetNodeByName(const RwWString &name);

    bool RegisterCreator(const RwWString &tag, RwRf3XMLNodeCreator *pCreator);
    void UnRegisterCreators();

    const RwVector<RwXMLNodePtr>    &GetAssetNodes() const { return m_vAssets; }

private:        

    static void openTagCB(void *userdata, const char *el, const char **attr);
    static void closeTagCB(void *userdata, const char *el);
    static void tagTextCB(void *userdata, const char *t);

    void ClearChache();

    RwWString errorMessage;
    unsigned int lineCount;

    RwMap<RwWString, RwXMLNode*>             m_mNameTable;
    RwMap<RwWString, RwRf3XMLNodeCreator*>   m_mCreators;
    RwVector<RwXMLNodePtr>                  m_vAssets;
};


#endif // RWRF3XMLPARSER_H