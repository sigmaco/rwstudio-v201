// RwCommUserMessage.h: interface for the RwCommUserMessage class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "RwComm.h"
#include "RwUtilTypes.h"
#include "RwUtilString.h"



/** 
* 
* \ingroup commlowlevelobjects
*
* This class represents a message that is sent to the user and all of it's associated data.
* 
*/

class RWCOMM_API RwCommUserMessage  
{
    //map of all FAQ related data
    static RwMap<int, RwCommUserMessage> s_MessageData;

    // The data that this message has in relation to itself
    RwWString m_sMessage;
    RwWString m_sObjectName;
    int m_nMessageID;
    int m_nFAQItem;
    int m_nSeverity;
    int m_nVerbosity;

    // const for items with no FAQ entry
    static const int NOFAQITEM;
    static const int NOMESSAGE;

public:
    /** 
    *  Default constructor for an empty message
    */
	inline RwCommUserMessage()
        : m_sMessage(L"")
        , m_sObjectName(L"")
        , m_nMessageID(NOMESSAGE)
        , m_nFAQItem(NOFAQITEM)
        , m_nSeverity(INT_MAX)
        , m_nVerbosity(INT_MAX)
    {}

    /** 
    *  Legacy constructor for use within the PRINT_MESSAGE macro
    */
	inline RwCommUserMessage(RwWString msg, RwWString node, int level)
        : m_sMessage(msg)
        , m_sObjectName(node)
        , m_nMessageID(NOMESSAGE)
        , m_nFAQItem(NOFAQITEM)
        , m_nSeverity(level)
        , m_nVerbosity(level)
    {}

	inline RwCommUserMessage(RwString msg, RwString node, int level)
        : m_nMessageID(NOMESSAGE)
        , m_nFAQItem(NOFAQITEM)
        , m_nSeverity(level)
        , m_nVerbosity(level)
    {
        m_sMessage = RwUtilA2W(msg);
        m_sObjectName = RwUtilA2W(node);
    }

#ifndef DOXYGEN

    /**
    *  New style constructor - we haven't implemented this yet so leave it out of the docs
    */
    inline RwCommUserMessage(int messageID, RwWString msg, RwWString node)
    {
        //initialise with a copy constructor
        *this = GetMessage(messageID);
        //override per instance info
        m_sMessage = msg;
        m_sObjectName = node;
    }

#endif

    // TODO RwCommUserMessage(int messageID, ...);
    
    virtual ~RwCommUserMessage(){};

    
    /**
    *  Access the formatted message string
    */
    RwWString GetFullMessage() const;

    /**
    *  Access the raw message string
    */
    inline RwWString GetMessage() const
    {
        return m_sMessage;
    }

    /**
    *  Access the FAQ item for the message
    */
    inline int GetFAQItem() const
    {
        return m_nFAQItem;
    }

    /**
    *  Access the object associated with the message
    */
    inline RwWString GetObjectName() const
    {
        return m_sObjectName;
    }

    /**
    *  Access the severity of the message (e.g. warning, error, informational)
    */
    inline int GetSeverity() const
    {
        return m_nSeverity;
    }

    /**
    *  Access the verbosity of the message (e.g. low, high, medium)
    */
    inline int GetVerbosity() const
    {
        return m_nVerbosity;
    }

    /**
    *  Combine two messages to form a single user message.
    */
    inline RwCommUserMessage &operator += (const RwCommUserMessage &toAdd)
    {
        // concatenate the messages
        m_sMessage += toAdd.m_sMessage;

        //if we now have an FAQ item to use - use it
        if(m_nFAQItem == NOFAQITEM && toAdd.m_nFAQItem != NOFAQITEM)
            m_nFAQItem = toAdd.m_nFAQItem;

        // added messages can push verbosity levels down but not up
        if(m_nVerbosity > toAdd.m_nVerbosity)
            m_nVerbosity = toAdd.m_nVerbosity;

        // added messages can push severity levels down but not up
        if(m_nSeverity > toAdd.m_nSeverity)
            m_nSeverity = toAdd.m_nSeverity;
        
        // only one object name per message (legacy stuff - will go when we take parameterised messages)
        if(m_sObjectName.empty())
            m_sObjectName = toAdd.m_sObjectName;

        return *this;
    }


#ifndef DOXYGEN

    // Add items to the messageID list use like this
    // RwCommUserMessage::InsertItem(ID,FAQ,Sev,Verb);
    // RenderwareIDs are masked with 0x80000000 so as not to conflict with user values
    // haven't put this in the docs yet as usage is undecided at this moment
    static void InsertItem(int MessageID, int FAQItem, int Severity, int Verbosity) 
    { 
        s_MessageData.insert(RwPair<int, RwCommUserMessage>(MessageID, RwCommUserMessage(MessageID, FAQItem, Severity, Verbosity))); 
    };
#endif

private:

    // constructor for making data entries
    inline RwCommUserMessage(int MessageID, int FAQItem, int Severity, int Verbosity)
        : m_nMessageID(MessageID)
        , m_nFAQItem(FAQItem)
        , m_nSeverity(Severity)
        , m_nVerbosity(Verbosity)
    {}

    // static function to get a message based on an ID
    static RwCommUserMessage GetMessage(int MessageID)
    {
        if(s_MessageData.empty())
            InitialiseData();

        return s_MessageData[MessageID];
    }

    static void InitialiseData();

};
