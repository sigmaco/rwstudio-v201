#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif // _DEBUG

#include "Utility.h"

/* -------------------------------------------------------------------------- */
namespace
{
   const RWS::DataType g_DataTypes[] =
   {
      RWS::DataType (_T("RwBool"),       RWSTypeBool,   sizeof (RWSUInt32)),
      RWS::DataType (_T("BOOLEAN"),      RWSTypeBool,   sizeof (RWSUInt32)),
      RWS::DataType (_T("BOOL"),         RWSTypeBool,   sizeof (RWSUInt32)),
      RWS::DataType (_T("RwInt32"),      RWSTypeInt32,  sizeof (RWSInt32)),
      RWS::DataType (_T("int"),          RWSTypeInt32,  sizeof (RWSInt32)),
      RWS::DataType (_T("unsigned int"), RWSTypeUInt32, sizeof (RWSUInt32)),
      RWS::DataType (_T("uint"),         RWSTypeUInt32, sizeof (RWSUInt32)),
      RWS::DataType (_T("RwUInt32"),     RWSTypeUInt32, sizeof (RWSUInt32)),
      RWS::DataType (_T("RwReal"),       RWSTypeReal,   sizeof (RWSReal)),
      RWS::DataType (_T("float"),        RWSTypeReal,   sizeof (RWSReal)),
      RWS::DataType (_T("double"),       RWSTypeDouble, sizeof (RWSDouble)),
      RWS::DataType (_T("RwUInt8"),      RWSTypeByte,   sizeof (RWSByte)),
      RWS::DataType (_T("char"),         RWSTypeByte,   sizeof (RWSByte)),
      RWS::DataType (_T("STRING"),       RWSTypeChar,   sizeof (RWSChar)),
      RWS::DataType (_T("RwChar"),       RWSTypeChar,   sizeof (RWSChar)),
      RWS::DataType (_T("_TCHAR"),       RWSTypeChar,   sizeof (RWSChar)),
      RWS::DataType (_T("TCHAR"),        RWSTypeChar,   sizeof (RWSChar)),
      RWS::DataType (0, 0, 0) // null terminator
   };
}

/* -------------------------------------------------------------------------- */
namespace RWS
{
   /* ----------------------------------------------------------------------- */
   const DataType *FindDataType (const RWSChar * const szTypeName)
   {
      const DataType *pDataType = g_DataTypes;
      while (pDataType && pDataType->szDataType) // null type string terminates
      {
         if (0 == _tcsicmp (pDataType->szDataType, szTypeName))
            return pDataType;

         // next type
         pDataType++;
      }
      return 0;
   }

   /* ----------------------------------------------------------------------- */
   const RWSChar *GetNthParam (const RWSChar * const szParamList,
                               RWSUInt32 ParamLength, RWSUInt32 nParam)
   {
      const RWSChar *szParamString = 0;
      RWSUInt32 ParamCount = 0;
      RWSUInt32 StrPos = 0;

      while (StrPos < ParamLength)
      {
         if (ParamCount == nParam)
         {
            szParamString = &szParamList[StrPos];
            break;
         }
         else
         {
            // Find next '0' character
            while ((StrPos < ParamLength) && (szParamList[StrPos] != _T('\0')))
               StrPos++;

            // Move on to the next character
            StrPos++;

            ParamCount++;
         }
      }
      return szParamString;
   }

   /* ----------------------------------------------------------------------- */
   const RWSChar *FindParamString (const RWSChar * const szParamList,
                                   RWSUInt32 ParamLength,
                                   const RWSChar * const szSearch)
   {
      // Search param list for param beginning with 'szSearch'
      const RWSChar *szParamString = 0;
      RWSUInt32 StrPos = 0;
      while (StrPos < ParamLength)
      {
         if (0 == _tcsncmp (&szParamList[StrPos], szSearch, _tcslen (szSearch)))
         {
            szParamString = &szParamList[StrPos];
            break;
         }
         else
         {
            // Find next '0' character
            while ((StrPos < ParamLength) && (szParamList[StrPos] != _T('\0')))
               StrPos++;

            // Move on to the next character
            StrPos++;
         }
      }
      return szParamString;
   }

   /* ----------------------------------------------------------------------- */
   bool UnquoteParamString (const RWSChar * const szParam,
                            RWSChar * const szBuffer)
   {
      ATLASSERT (szParam);
      ATLASSERT (szBuffer);
      ATLASSERT (szParam != szBuffer); // dodgy, source and dest are same!

      int nParamLen = _tcslen (szParam);

      if (!nParamLen)
         return false;
      if (szParam[0] != _T('\"') || szParam[nParamLen-1] != _T ('\"'))
         return false;

      // We need to remove the quotes from the string.
      memcpy (szBuffer, szParam + 1, sizeof (RWSChar ) * (nParamLen - 2));
      szBuffer[nParamLen-2] = _T('\0');
      return true;
   }

   /* ----------------------------------------------------------------------- */
}