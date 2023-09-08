/*****************************************************************************

    File:   RwXMLDOM.h

*****************************************************************************/

/*************************** Compilation defines ****************************/
#ifndef RWXMLDOM_H
#define RWXMLDOM_H

/****************************** Include files *******************************/

#pragma warning(disable : 4251)

#include "RwComm.h"
#include "RwCommPrimitives.h"

#include "RwXmlDll.h"
#include "RwXML.h"
#include "RwUtilManagedObj.h"
#include "RwUtilPtr.h"

class RwXMLNode;

// a smart pointer to RwXMLNode
typedef RwUtilRCAutoPtr<RwXMLNode> RwXMLNodePtr;
typedef RwList<RwXMLNodePtr> RwXMLNodeList;
typedef RwXMLNodeList::iterator RwXMLNodeListItor;


/************************ Constants and Definitions *************************/

typedef bool (*XMLEnumProc)(class RwXMLNode*, void*);

/********************************** Macros **********************************/

/************************** Typedefs and Classes ****************************/

/*****************************************************************************

RwXMLAttribute

Description:
XML keypair-style attribute class

Parent classes:
None

Child classes:
None

******************************************************************************/
class RWXML_API RwXMLAttribute
{
    friend class RwXMLNode;

public:
    //default constructor
    RwXMLAttribute();                        
    
    //data constructor
    RwXMLAttribute( const RwWString& lpcName, const RwWString& lpcValue );
    
    //sets the name of this attribute
    bool SetName( const RwWString& lpcName );

    //sets the value of this attribute
    bool SetValue( const RwWString& lpcValue );

    //gets the name of this attribute
    RwWString GetName() const;

    //gets the value of this attribute
    RwWString GetValue() const;

    // traversal operations
    inline const RwXMLAttribute* GetNextAttribute( void ) const { return mpNext; }
    inline const RwXMLAttribute* GetPreviousAttribute( void ) const { return mpPrev; }
    inline RwXMLAttribute* GetNextAttribute( void ) { return mpNext; }
    inline RwXMLAttribute* GetPreviousAttribute( void ) { return mpPrev; }

protected:       
    RwXMLAttribute* mpNext;
    RwXMLAttribute* mpPrev;

    RwWString nodeName;
    RwWString nodeValue;
};

/*****************************************************************************

RwXMLNode

Description:
XML data node. Contains a list of attributes and child nodes.

Parent classes:
None

Child classes:
None

******************************************************************************/
/**
*   \ingroup rf3helper 
*
*   This class represents an XML node. An XML node has a name, a set of attributes and
*   a text section. 
*/
class RWXML_API RwXMLNode : public RwUtilManagedObj
{
friend class RwXMLParser;

public:

    // constructors
    /** 
    *   Construct the node and attach it to its parent.
    *
    *   \param parentNode the parent node
    */
    RwXMLNode( RwXMLNode* parentNode = NULL );

    /** 
    *   Construct the node with the given name and attach it to its parent.
    *
    *   \param name the node's name
    *   \param parentNode the parent node
    */
    RwXMLNode( const RwWString& name, RwXMLNode* parentNode = NULL );

    // destructor
    virtual ~RwXMLNode();
    
    // clone this node
//    RwXMLNode* CloneNode( bool deep );

    /** 
    *   Set the node's name.
    *
    *   \param newName the node's name
    */
    bool SetName( const RwWString& newName );

    /** \returns the node's name */
    RwWString GetName( void ) const;

    /**
    *   Set the node's text.
    *
    *   \param newText the input text string
    */
    bool SetText( const RwWString& newText );

    /** \returns the node's text string.*/
    RwWString GetText( void ) const;

    // ----- Child operations

    /** 
    *   Append a child to this node
    *
    *   \param newChild a pointer to the child to append
    */
    RwXMLNode* AppendChild( RwXMLNode* newChild );

    // insert a child before a given node
//    RwXMLNode* InsertBefore( RwXMLNode* newChild, RwXMLNode* refChild );

    // replace a child
//    RwXMLNode* ReplaceChild( RwXMLNode* newChild, RwXMLNode* oldChild );

    /** 
    *   Remove a child.
    *
    *   \param oldChild pointer to the child to remove
    */
    RwXMLNode* RemoveChild( RwXMLNode* oldChild );

    // has this node got child nodes
    bool HasChildNodes() const { return( NULL != mpFirstChild ); }

    // Create a new child and append - NON W3C!
    RwXMLNode* AddChild( const RwWString& name );

    /** \returns the number of children nodes.*/
    unsigned int GetNumChildren(void) const;


    // ----- Attribute operations

    /** 
    * Get a string attribute by name.
    *
    *   \param name the name of the attribute
    *   \param value the attribute's returned value
    *
    *   \returns true if the attribute exists, false otherwise
    */       
    bool GetAttribute( const RwWString& name, RwWString& value ) const;

    /** 
    * Get an int attribute by name.
    *
    *   \param name the name of the attribute
    *   \param value the attribute's returned value
    *
    *   \returns true if the attribute exists, false otherwise
    */     
    bool GetAttribute( const RwWString& name, int& value ) const;

    /** 
    * Get a float attribute by name.
    *
    *   \param name the name of the attribute
    *   \param value the attribute's returned value
    *
    *   \returns true if the attribute exists, false otherwise
    */     
    bool GetAttribute( const RwWString& name, float& value ) const;
    
    /**
    * Set a string attribute.
    *
    *   \param name the attribute's name
    *   \param value the value to set
    */
    void SetAttribute( const RwWString& name, const RwWString& value );

    /**
    * Set an int attribute.
    *
    *   \param name the attribute's name
    *   \param value the value to set
    */
    void SetAttribute( const RwWString& name, int value );

    /**
    * Set a float attribute.
    *
    *   \param name the attribute's name
    *   \param value the value to set
    */
    void SetAttribute( const RwWString& name, float value );


    // remove the attribute with the matching name
//    void RemoveAttribute( RwWString& name );

    // Retrieves an attribute by name. Returns the attribute with the 
    // specified attribute name or NULL if there is no such attribute.
//    RwXMLAttribute* GetAttributeNode( RwWString& name );

    // Retrieves an attribute by index. Returns the attribute with the 
    // specified index or NULL if there is no such attribute.
    RwXMLAttribute* GetAttributeNode( unsigned int index ) const;

    // Adds a new attribute. If an attribute with that name is
    // already present in this node, it is replaced by the new one.
    // If the newAttr attribute replaces an existing attribute with the
    // same name, the previously existing attribute is returned, otherwise
    // NULL is returned.
//    RwXMLAttribute* SetAttributeNode( RwXMLAttribute* newAttr );

    // Removes the specified attribute.
    // The attribute that was removed. If the attribute is not found 
    // NULL is returned. 
//    RwXMLAttribute* RemoveAttributeNode( RwXMLAttribute* oldAttr );

    /** \returns the number of attributes.*/
    unsigned int GetNumAttributes( void ) const;

    inline const RwXMLAttribute* GetFirstAttribute( void ) const { return mpFirstAttribute; }
    inline const RwXMLAttribute* GetListAttribute( void ) const { return mpLastAttribute; }

    inline RwXMLAttribute* GetFirstAttribute( void ) { return mpFirstAttribute; }
    inline RwXMLAttribute* GetListAttribute( void ) { return mpLastAttribute; }

    // ----- Traversal operations
    
    /** \returns the node's parent.*/
    inline const RwXMLNode* GetParent( void ) const { return mpParent; }

    /** \returns the first child in the child list.*/
    inline const RwXMLNode* GetFirstChild( void ) const { return mpFirstChild; }

    /** \returns the last child in the child list.*/
    inline const RwXMLNode* GetLastChild( void ) const { return mpLastChild; }

    /** 
    * Sibling iterator.
    *   \returns the next sibling. */
    inline const RwXMLNode* GetNextNode( void ) const { return mpNext; }

    /** 
    * Sibling iterator.
    * \returns the previous sibling. */
    inline const RwXMLNode* GetPreviousNode( void ) const { return mpPrev; }

    /** \returns the node's parent.*/
    inline RwXMLNode* GetParent( void ) { return mpParent; }

    /** \returns the first child in the child list.*/
    inline RwXMLNode* GetFirstChild( void ) { return mpFirstChild; }

    /** \returns the last child in the child list.*/
    inline RwXMLNode* GetLastChild( void )  { return mpLastChild; }

    /** Sibling iterator. 
    * \returns the next sibling. */
    inline RwXMLNode* GetNextNode( void )  { return mpNext; }

    /** Sibling iterator. 
    * \returns the previous sibling. */
    inline RwXMLNode* GetPreviousNode( void ) { return mpPrev; }

    // generate a list of node with tag matching name, "*" matches all
    RwXMLNodeList* GetChildrenByTagName( const RwWString& name, bool deep = false ) const;
    
    bool GetChildrenByTagName( RwXMLNodeList& nodeList, const RwWString& namw, bool deep = false ) const;

    bool EnumerateByTagName( const RwWString& name, XMLEnumProc lpfXMLEnumProc,
                        bool deep = false, void* param = NULL ) const;

    // ---- saving operation
   
    /** 
    * Save the node and its descendants to the given xml file.
    *
    *   \param filename the XML file name
    *   \param lbCompressed flag indicating whether to remove all redundant spacing
    */
    bool Save( const RwWString& filename, bool lbCompressed = true );

protected:

    RwWString     nodeName;             //node name
    RwWString     nodeText;             //node text
    RwXMLNode*       mpParent;             //node parent pointer
    RwXMLNode*       mpFirstChild;         //first child node
    RwXMLNode*       mpLastChild;          //last child node
    RwXMLNode*       mpNext;               //next child node
    RwXMLNode*       mpPrev;               //previous child node

    RwXMLAttribute*  mpFirstAttribute;     //first attribute    
    RwXMLAttribute*  mpLastAttribute;      //last attribute 
};


/*****************************************************************************

RwXMLParser

Description:
XML parser class

Parent classes:
None

Child classes:
None

******************************************************************************/
class RWXML_API RwXMLParser
{
public:
    
    //default constructor
    RwXMLParser();

    //destructor
    ~RwXMLParser();

    // load an xml file into a xml node
    bool Load( RwXMLNode* node, const RwWString& filename );

    // load an xml file
    RwXMLNode* Load( const RwWString& filename );

    //parses the given buffer into the given node
//    bool ParseFrom( RwXMLNode& lNode, const char* lpcBuffer, unsigned int length );

    //creates a new node from a given buffer
//    RwXMLNode* ParseFrom( const char* lpcBuffer, unsigned int length );

    //gets the parser error message
    const wchar_t* GetErrorMessage() const;

    unsigned int GetLineCount() const { return lineCount; }

    //expands tokens into their xml escape codes
//    static bool ExpandSubstitutes( char* lpcDst, const char* lpcSrc, bool lbInText );

private:        

    static void openTagCB(void *userdata, const char *el, const char **attr);
    static void closeTagCB(void *userdata, const char *el);
    static void tagTextCB(void *userdata, const char *t);

    RwWString errorMessage;
    unsigned int lineCount;
};

enum XMLCompressionType
{
    NORMAL = 0,
    NOSPACE,
    BINARY
};

// xml saving class
class RWXML_API RwXMLSaver
{
public:
    //default constructor
    RwXMLSaver();

    //destructor
    ~RwXMLSaver();

    // save node to xml file
    bool Save( RwXMLNode* node, const RwWString& filename );

//    bool SetEncoding(XMLEncoding encoding);

    void SetCompression( XMLCompressionType comp ) { compressionType = comp; }

private:

    XMLCompressionType compressionType;

    //expands tokens into their xml escape codes
    static bool ExpandSubstitutes( RwWString& dst, const RwWString& src, bool lbInText );

    typedef struct XMLWriteInfoTag XMLWriteInfo;

    struct XMLWriteInfoTag
    {
        FILE*   mpFile;                   //file pointer to write to
        int     mnIndent;                 //current indent level
        bool  mbCompressed;               //save compressed?
        RwWString workBuffer;             //string munging work buffer
        RwXMLSaver* mpSaver;
    };

    //writes this node to the given file
    bool WriteNode( RwXMLNode* lpNode, XMLWriteInfoTag* lpWriteInfo );

    //enumation callback function
    friend static bool _XMLEnumWriteNode( RwXMLNode* lpNode, void* lpParam );
};

/*************************** Function Prototypes ****************************/

/******************************* Global Data ********************************/

/******************************** Functions *********************************/

#endif //RWXMLDOM_H
