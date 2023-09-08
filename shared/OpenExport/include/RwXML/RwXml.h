
/***************************************************************************
 *                                                                         *
 * Module  : RwXML.h                                                       *
 *                                                                         *
 * Purpose : Load XML format files                                         *
 *                                                                         *
 **************************************************************************/

#ifndef RTXML_H
#define RTXML_H

#include "RwXmlDll.h"

/**
 * \defgroup rtxml RtXML
 * \ingroup xmltools
 *
 * XML Format Toolkit for RenderWare.
 *
 */

/****************************************************************************
 Includes
 */

/*--- Include files ---*/
//#include "rwcore.h"

//#include "rtxml.rpe"   /* automatically generated header file */


typedef void (*RtXMLStartElementCallBack)(void* userdata, const char* elementName,
                                          const char** attrList);
typedef void (*RtXMLEndElementCallBack)(void* userdata, const char* elementName);

typedef void (*RtXMLTextCallBack)(void* userdata, const char* elementText);


enum RtXMLParserErrorCode
{
    rwPERROR_SUCCESS = 0,
    rwPERROR_UNKNOWN,
    rwPERROR_EXPECTEDSTRING,
    rwPERROR_EXPECTEDTAGOPEN,
    rwPERROR_EXPECTEDDASH,
    rwPERROR_EXPECTEDTAGCLOSE,
    rwPERROR_EXPECTEDEQUAL,
    rwPERROR_ILLEGALCHAR,    
    rwPERROR_UNKNOWNESCAPE,
    rwPERROR_INVALIDESCAPE,
    rwPERROR_OUTOFMEMORY
};

typedef enum RtXMLParserErrorCode RtXMLParserErrorCode;

enum RtXMLParserState
{
    rwPSTATE_ERROR = 0,
    rwPSTATE_TAGSTART,
    rwPSTATE_READOPENTAG,
    rwPSTATE_READTAG,
    rwPSTATE_READCOMMENTTAG,
    rwPSTATE_READCOMMENTMAIN,
    rwPSTATE_COMMENTCLOSEDASH,       
    rwPSTATE_COMMENTOPENDASH,
    rwPSTATE_COMMENTCLOSE,
    rwPSTATE_READVERSIONTAG,
    rwPSTATE_READTAGNAME,
    rwPSTATE_READATTR,
    rwPSTATE_FINCLOSETAG,
    rwPSTATE_FINMULTITAG,
    rwPSTATE_FINPROCESSTAG,
    rwPSTATE_TEXTTAG,
    rwPSTATE_READATTRNAME,
    rwPSTATE_READTEXT,
    rwPSTATE_READATTREQUAL,
    rwPSTATE_FINDATTRVALUE,
    rwPSTATE_READATTRVALUE,
    rwPSTATE_NORMAL,
    rwPSTATE_ESCAPE,
    rwPSTATE_AUTODETECTENCODING
};

typedef enum RtXMLParserState RtXMLParserState;

enum RtXMLParserTagType
{
    rwPTAGTYPE_CLOSING = 0,
    rwPTAGTYPE_OPENING,
    rwPTAGTYPE_PROCESS
};

typedef enum RtXMLParserTagType RtXMLParserTagType;

/*enum RtXMLParserEncoding
{
    ASCII = 0,
    UTF-8,
    UTF-16
};

typedef enum RtXMLParserEncoding RtXMLParserEncoding;*/

struct RtXMLParser
{
    RtXMLStartElementCallBack startElementCB;   /* callback for opening tags */
    RtXMLEndElementCallBack   endElementCB;     /* callback for closing tags */
    RtXMLTextCallBack         textCB;           /* callback for tagged text */

    const char*               currBuffer;       /* current parse location in 
                                                   the buffer */
    const char*               endBuffer;        /* the end of the parse 
                                                   buffer */

    RtXMLParserTagType        curretTag;        /* type of the current tag */

    RtXMLParserState          state;            /* the current state of the
                                                   parser */
    RtXMLParserErrorCode      errorCode;        /* the error code of the 
                                                   parser */
    int                       lineCount;        /* current line number */

    RtXMLParserState          subState;         /* the state inside the main
                                                   state */
    char*                     tokenBuffer;      /* the current token buffer */
    char*                     tokenBufferPtr;   /* current location in the
                                                   token buffer */

    char**                    attrList;         /* list of attributes */
    int                       attrCount;        /* number of attributes */

    char*                     tagNameBuffer;    /* current tag name */
    char*                     tagNameBufferPtr; /* current location in the tag
                                                   name */

    char*                     stringCache;      /* string cache (used by key
                                                   pair strings and tagged
                                                   text) */
    int                       stringCacheIndex; /* current index into the text
                                                   cache */
    int                       stringCacheSize;  /* size of the text cache */

    char                      escapeCache[16];
                                                /* escape characters are
                                                   buffered here while they are
                                                   collapsed i.e (&amp) */
    int                       escapeCacheIndex; /* index in the the escape
                                                   character buffer */
    void*                     userdata;         /* userdata passed to the 
                                                   callbacks */
};

typedef struct RtXMLParser RtXMLParser;

/****************************************************************************
 Function prototypes
 */

#ifdef    __cplusplus
extern "C"
{
#endif                          /* __cplusplus */

extern RWXML_API RtXMLParser* RtXMLParserCreate(void);
extern RWXML_API void RtXMLParserDestroy(RtXMLParser* parser);

extern RWXML_API void RtXMLSetElementCallBacks(RtXMLParser* parser, RtXMLStartElementCallBack start,
                                     RtXMLEndElementCallBack end);

extern RWXML_API void RtXMLSetTextCallBack(RtXMLParser* parser, RtXMLTextCallBack text);

extern RWXML_API RtXMLParserErrorCode RtXMLParse(RtXMLParser* parser, const char* buffer, int length,
                       int final);

extern RWXML_API RtXMLParserErrorCode RtXMLGetErrorCode( RtXMLParser* parser );

extern RWXML_API unsigned int RtXMLGetLineCount( RtXMLParser* parser );

extern RWXML_API void RtXMLSetUserData(RtXMLParser* parser, void* userdata);

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

/* RWPUBLICEND */

#endif /* RTXML_H */
