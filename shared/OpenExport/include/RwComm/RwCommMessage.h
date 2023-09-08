// RwCommMessage.h: interface for the RwCommMessage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWCOMMMESSAGE_H__F65839FE_6BF2_4610_857F_D4C5E6BCDD66__INCLUDED_)
#define AFX_RWCOMMMESSAGE_H__F65839FE_6BF2_4610_857F_D4C5E6BCDD66__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#pragma warning(disable : 4251)

#include "RwComm.h"
#include "RwCommError.h"
#include "RwCommPrimitives.h"
#include "RwCommUserMessage.h"

/** 
*  \ingroup commlowlevelobjects
*   \def PRINT_MESSAGE This macro is used to print output messages.
*/
#define PRINT_MESSAGE(x, y, z) RwCommMessage::TheInstance()->AddMessage( RwCommUserMessage( (x) , (y) , (z) ) )

/**
*  \ingroup commlowlevelobjects
*
*   This class displays and manages messages. It is a singleton class. Each message has
*   verbosity level for filtering. There is a separate array of summary messages
*   that should be display at the end.
*
*   \note For printing out the messages you should use PRINT_MESSAGE macro.
*/
class RWCOMM_API RwCommMessage
{
public:
	virtual ~RwCommMessage();

    /**
    *   Register a static instance of the class. RwCommMessage is a singleton.
    */
    static void RegisterInstance(RwCommMessage *pInstance);

    /**
    *   \return static instance of the class. RwCommMessage is a singleton.
    */
    static RwCommMessage *TheInstance();

    /**
    *   Display a message.
    *
    *   \param userMessage the message to display
    *
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    virtual RwCommError AddMessage(const RwCommUserMessage &userMessage) { return RWCOMM_NOT_IMPLEMENTED; }
    /**
    *   Set verbosity level. Messages with a higher level will be stored but not displayed.
    *   On changing the verbosity level all currently stored messages will be re-tested.
    *
    *   \param verbosity the new verbosity level
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    virtual RwCommError SetVerbosity(int verbosity) { return RWCOMM_NOT_IMPLEMENTED; }

    /**
    *   Set prefilter level. All messages with a higher level will not be stored or displayed.
    *
    *   \param preFilter the new prefilter level
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    virtual RwCommError SetPreFilter(int preFilter) { return RWCOMM_NOT_IMPLEMENTED; }

    /**
    *   Get preFilter level.
    *
    *   \param preFilter storage for the prefilter level
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    virtual RwCommError GetPreFilter(int &preFilter) { return RWCOMM_NOT_IMPLEMENTED; }

    /**
    *   Check if there is a message with the specified entry name in the summary.
    */
    bool    HasEntry(const RwWString &entryName) const;
    /**
    *   Add a message to the summary. This is a list of separate messages 
    *   that should be displayed at the end.
    *   
    *   \param userMessage the message to display
    *   \param entryName the entry that this message will be associated with
    */
    void AddToSummary(const RwCommUserMessage &userMessage, const RwWString &entryName = L"");
    /**
    *   Display all the messages from the summary by calling the AddMessage(),
    *   then clear all of them.
    */
    void    FlushSummary();

protected:
    RwCommMessage();

private:
    RwVector<RwCommUserMessage>  m_vSummary;
    RwSet<RwWString>     m_sEntries;
    static RwCommMessage        *m_pTheInstance;
};

#endif // !defined(AFX_RWCOMMMESSAGE_H__F65839FE_6BF2_4610_857F_D4C5E6BCDD66__INCLUDED_)
