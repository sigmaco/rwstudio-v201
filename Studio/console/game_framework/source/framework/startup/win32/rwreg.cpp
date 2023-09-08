#include "precomp.h"
#include <windows.h>
#include "rwreg.h"

#include <stdexcept>

using std::string;
using std::vector;

namespace CSL {
   
   // Initial revision shipped with reg keys in HKEY_LOCAL_MACHINE.
   // This needed to be changed to HKEY_CURRENT_USER so that users
   // without admin priviledges can write keys/preferences.
   // To be backwards compatible, when reading a key we check if
   // it exists under HKEY_CURRENT_USER.  If not we check HKEY_LOCAL_MACHINE.
   // If we still couldn't find it, create it under HKEY_CURRENT_USER with
   // default value.
   
   const HKEY legacyhTopKey = HKEY_LOCAL_MACHINE,
      hTopKey = HKEY_CURRENT_USER;
   
   
   static REGSAM PrimarySecAccMask = KEY_QUERY_VALUE |
      KEY_SET_VALUE |
      KEY_CREATE_SUB_KEY |
      KEY_ENUMERATE_SUB_KEYS |
      KEY_NOTIFY ;
   
   const size_t BufferSize = 255;
   
   void
      RegSetString (const string& application,
      const string& section, 
      const string& entry,
      const string& value)
   {
      HKEY hMasterKey = 0 ;
      DWORD dwDisposition = 0 ;
      LONG lReturn = 0 ;
      
      // Thou shalt not pass cack.
      
      if (entry.empty() )
      {
         throw std::invalid_argument(
            string("Bad data to write to registry - section \'")
            + section
            + string("\'; entry \'")
            + entry
            + "\' value \'"
            + value
            + "\'"
            );
      }
      
      // Select the appropriate tree.
      
      string treeBranch("Software\\Criterion\\RenderWare\\");
      treeBranch += application;
      if (!section.empty())
      {
         treeBranch += string("\\") + section;
      }
      
      lReturn = RegCreateKeyEx (hTopKey,
         treeBranch.c_str(),
         0,
         "", //szPNC_CLASS, 
         REG_OPTION_NON_VOLATILE,
         PrimarySecAccMask,
         0,
         &hMasterKey,
         &dwDisposition);
      
      if (lReturn == ERROR_SUCCESS)
      {
         // section exists : set the value.
         lReturn = RegSetValueEx (hMasterKey,
            entry.c_str(),
            0,
            REG_SZ,
            (CONST BYTE *) value.c_str(),
            value.length() + 1
            );
      }
      else
      {
         // Something went wrong; handle if appropriate
      }
      
      if (hMasterKey)
      {
         RegCloseKey(hMasterKey);
      }
   }
   
   
   bool QueryKey(string & retString,
      const HKEY hMasterKey,
      const char *entrycstr
      )
   {
      // section exists : get the value.
      vector<char> buffer(BufferSize);
      DWORD sizeTransfer = BufferSize;
      DWORD typeTransfer(REG_SZ);
      LONG lReturn = RegQueryValueEx(
         hMasterKey,
         entrycstr,
         0,
         &typeTransfer,
         (BYTE*)&buffer[0],
         &sizeTransfer);
      if (lReturn == ERROR_SUCCESS)
      {
         retString = string(&buffer[0]);
         return true;
      }
      // Query failed. If the buffer was too small resize and try again.
      else if ((lReturn == ERROR_MORE_DATA) && (sizeTransfer > buffer.size()))
      {
         buffer.resize(sizeTransfer);
         
         if (ERROR_SUCCESS == RegQueryValueEx(
            hMasterKey,
            entrycstr,
            0,
            &typeTransfer,
            (BYTE*)&buffer[0],
            &sizeTransfer) )
         {
            retString = string(&buffer[0]);
            return true;
         }
      }
      
      return false;
   }
   
   
   const string RegGetString (const string& application,
      const string& section, 
      const string& entry,
      const string& defaultVal)
   {
      // Thou shalt not pass cack.
      
      if (entry.empty())
      {
         throw std::invalid_argument(
            string("Bad registry location - section \'")
            + section
            + string("\'; entry \'"
            + entry
            + "\'")
            );  
      }
      
      // Select the appropriate tree.
      string treeBranch("Software\\Criterion\\RenderWare\\");
      treeBranch += application;
      if (!section.empty())
      {
         treeBranch += string("\\") + section;
      }
      
      string retString = defaultVal;
      
      //try the new place first
      HKEY hMasterKey = 0 ;
      DWORD ReservedPassZero = 0;
      if (ERROR_SUCCESS == RegOpenKeyEx(
         hTopKey,         // handle to open key
         treeBranch.c_str(),  // subkey name
         ReservedPassZero,   // reserved
         KEY_READ, //PrimarySecAccMask, // security access mask
         &hMasterKey   // handle to open key
         ))
      {
         //great read the value
         QueryKey(retString, hMasterKey, entry.c_str() );
      }
      else
      {
         //try the old place
         if (ERROR_SUCCESS == RegOpenKeyEx(
            legacyhTopKey,         // handle to open key
            treeBranch.c_str(),  // subkey name
            ReservedPassZero,   // reserved
            KEY_READ, //PrimarySecAccMask, // security access mask
            &hMasterKey   // handle to open key
            ))
         {
            //great read the value
            QueryKey(retString, hMasterKey, entry.c_str() );
            
            //and set it in the new place so we can find it next time
            //(rather than get the blank entry created by a failed RegOpenKeyEx!)
            RegSetString (application,
               section, 
               entry,
               retString);
         }
         else
         {
            //create a new key with default value
            //this is exceedingly useless for finding out where the visualizer's
            //installed.
            DWORD dwDisposition = 0;
            RegCreateKeyEx(
               hTopKey,
               treeBranch.c_str(),
               0,
               "", //szPNC_CLASS, 
               REG_OPTION_NON_VOLATILE,
               PrimarySecAccMask,
               0,
               &hMasterKey,
               &dwDisposition);
         }
      }
      
      if (hMasterKey)
      {
         RegCloseKey(hMasterKey);
      }
      
      return retString;
   }
   
}