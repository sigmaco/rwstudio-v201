// RwCommExportOptions.h: interface for the RwCommExportOptions class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWCOMMEXPORTOPTIONS_H__0979F10C_AA62_4D54_9540_D200F234C065__INCLUDED_)
#define AFX_RWCOMMEXPORTOPTIONS_H__0979F10C_AA62_4D54_9540_D200F234C065__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwComm.h"
#include "RwCommPrimitives.h"

class RwCommExportOptions;

/**
*  \ingroup commlowlevelobjects
*
*   This class represents an option as a generic parameter.
*/
class RWCOMM_API RwCommOption
{
public:
    /** Type of the option. */
    enum Type
    {
        /** integer option */
        INT = 0,
        /** float option */
        FLOAT,
        /** boolean option */
        BOOL,
        /** string option */
        STRING};

    /** This is an "Invalid Option" used by RwCommExportOptions::GetOption(). */
    static RwCommOption nullOption;

    /** The copy constructor. */
    RwCommOption (const RwCommOption &that);
    /** Construct an option with an int type and 0 value. */
    RwCommOption (const RwWString &name);
    /** Construct an option with an int type and specified value. */
    RwCommOption (const RwWString &name, const int val);
    /** Construct an option with an float type and specified value. */
    RwCommOption (const RwWString &name, const float val);
    /** Construct an option with an bool type and specified value. */
    RwCommOption (const RwWString &name, const bool val);
    /** Construct an option with an string type and specified value. */
    RwCommOption (const RwWString &name, const RwWString &val);
    virtual ~RwCommOption();

    /**
     *  The equality operator.
     *  \param the RwCommOption to be compared against.
     *  \return a bool indicating equality.
     */
    bool operator==(const RwCommOption &other) const;
    
    /**
     *  The inequality operator.
     *  \param the RwCommOption to be compared against.
     *  \return a bool indicating inequality.
     */
    bool operator!=(const RwCommOption &other) const;

    /**
     *  Get an options name.
     *  \return the option name.
     */
    RwWString  GetName() const { return m_name; }
    
    /**
     *  Set an options name.
     *  \param name to set.
     */
    void    SetName(const RwWString &name) { m_name = name; }

    /** 
     *  Get an options type
     *  \return the option type.
     */
    Type    GetType() const { return m_type; }
    /**
     *  Set an options type.
     *  \param type to set.
     */
    void    SetType(const Type type) { m_type = type; }

    /**
    *   \return integer value of the option.
    *
    *   \note If the option is not of type int, the default value will be returned.
    */
    int     GetInt() const { return m_intVal; }
    /**
    *   \return float value of the option.
    *
    *   \note If the option is not of type float, the default value will be returned.
    */
    float   GetFloat() const { return m_floatVal; }
    /**
    *   \return boolean value of the option.
    *
    *   \note If the option is not of type boolean, the default value will be returned.
    */
    bool    GetBool() const { return m_boolVal; }
    /**
    *   \return string value of the option.
    *
    *   \note If the option is not of type string, the default value will be returned.
    */
    const RwWString &GetString() const { return m_stringVal; }

    /**
    *   \param val integer value to set.
    *
    *   \note If the option is not of type int, integer value will not be updated.
    */
    void    SetValue (const int val) { m_intVal = val; }
    /**
    *   \param val float value to set.
    *
    *   \note If the option is not of type float, float value will not be updated.
    */
    void    SetValue (const float val) { m_floatVal = val; }
    /**
    *   \param val bool value to set.
    *
    *   \note If the option is not of type bool, bool value will not be updated.
    */
    void    SetValue (const bool val) { m_boolVal = val; }
    /**
    *   \param val string value to set.
    *
    *   \note If the option is not of type string, string value will not be updated.
    */
    void    SetValue (const RwWString val) { m_stringVal = val; }

private:
    RwWString       m_name;
    Type            m_type;
    int             m_intVal;
    float           m_floatVal;
    bool            m_boolVal;
    RwWString       m_stringVal;
};

#if (!defined(DOXYGEN))

struct RwCommOptionLess
{
    bool operator() (const RwCommOption &o1, const RwCommOption &o2) const
    {
        return ( o1.GetName() < o2.GetName() );
    }
};

#endif // (!defined(DOXYGEN))

/**
*  \ingroup commlowlevelobjects
*
*   This class is a dynamic container of all of the export options. This object
*   is a container for the RwCommOptions and is passed throughout the export
*   process and can be used to specify how to export different elements within
*   the scene. 
 To query an option's value use the GetOption() method specifying the name of the option.
The options are usually set in some UI component, and are queried throughout the export process
*/
class RWCOMM_API RwCommExportOptions  
{
public:
    /** The default constructor. */
    RwCommExportOptions();
    /** The copy constructor. */
    RwCommExportOptions(const RwCommExportOptions &other);
    virtual ~RwCommExportOptions();

    // Operators
    /** The assignment operator. */
    virtual RwCommExportOptions &operator=(const RwCommExportOptions &other);
    /** The addition operator. Adds all options together */
    virtual RwCommExportOptions  operator+(const RwCommExportOptions &other) const;
    /** The in-place addition operator. Adds all options together */
    virtual RwCommExportOptions &operator+=(const RwCommExportOptions &other);

    /** \param option to set. If option doesn't exist it is added. */
    void    SetOption(const RwCommOption &option);

    /** \param optionName name of the option to remove. */
    void    RemoveOption(const RwWString &optionName);

    /**
    *   \param optionName name of the option to get
    *
    *   \return RwCommOption with the specified name if it exists.
    *           RwCommOption::nullOption otherwise.
    */
    const RwCommOption &GetOption(const RwWString &optionName) const;

    /** \param vOptions returns a vector of all options. */
    void    GetAllOptions(RwVector<RwCommOption> &vOptions) const;

    /** Clear all the options. */
    void    ClearAllOptions();

    /**
    *   Get all common options between this and other.
    *
    *   \param other options to compare with
    *   \param vCommonOptions returns the common ones
    *
    *   \return vector of commen options.
    */
    void    GetCommonOptions(const RwCommExportOptions &other, RwVector<RwCommOption> &vCommonOptions) const;

    /**
    *   Deserialize the options from the vector of strings and set/add the options.
    *   Strings in the vector should be formated like this: 
    *   
    *   \verbatim "type:name=value" \endverbatim
    * 
    *   Type can be: INT, FLOAT, BOOL, STRING. Value can be any string
    *   including spaces and value should correspond to the type. For example:
    *   
    *   \verbatim "BOOL:Process Geometry for Lightmapping=0" \endverbatim
    *
    *   \param strTab vector of strings to deserialize
    */
    void    Deserialize(RwVector<RwWString> &strTab);
    /**
    *   Serialize the options into a vector of string.
    *
    *   \param strTab returns all serialized options
    *
    *   \see Deserialize() for string format.
    */
    void    Serialize(RwVector<RwWString> &strTab) const;

    /**
    *   Dump all the options in the debugger output window using
    *   OutputDebugString().
    */
    void    DumpOptions();

private:
    RwSet<RwCommOption, RwCommOptionLess> m_options;
};

#endif // !defined(AFX_RWCOMMEXPORTOPTIONS_H__0979F10C_AA62_4D54_9540_D200F234C065__INCLUDED_)
