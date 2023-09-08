#ifndef RWUTIL_GUI_H
#define RWUTIL_GUI_H

#include "RwUtil.h"
#include "RwUtilTypes.h"

#include <windows.h>
#include <Shlwapi.h>
#include <winsock.h>
#include <nspapi.h>
#include <winnetwk.h>
#include <shlobj.h>
#include <winnls.h>
#include <winuser.h>
#include "imagehlp.h"

/**
*   This function gets the state of a check box control.
*
*   \ingroup rwutilgui
*
*   \param hWnd handle to the window containing the check box
*   \param ResourceID resource id of the check box
*
*   \return the state of the check box
*/
RWUTIL_API bool            RwUtilGetCheckBoxState(HWND hWnd, int ResourceID);

/**
*   This function gets the text from an edit box control and converts it to
*   a float representation.
*
*   \ingroup rwutilgui
*
*   \param hWnd handle to the window containing the edit box
*   \param ResourceID resource id of the edit box
*
*   \return the float version of the string in the edit box
*/
RWUTIL_API float           RwUtilGetEditBoxAsFloat(HWND hWnd, int ResourceID);

/**
*   This function gets the text from an edit box control and converts it to an
*   integer representation.
*
*   \ingroup rwutilgui
*
*   \param hWnd handle to the window containing the edit box
*   \param ResourceID resource id of the edit box
*
*   \return the int version of the string in the edit box
*/
RWUTIL_API int             RwUtilGetEditBoxAsInt(HWND hWnd, int ResourceID);

/**
*   This function gets the text from an edit box control.
*
*   \ingroup rwutilgui
*
*   \param hWnd handle to the window containing the edit box
*   \param ResourceID resource id of the edit box
*
*   \return the string contained in the edit box
*/
RWUTIL_API RwWString        RwUtilGetEditBoxAsString(HWND hWnd, int ResourceID);

/**
*   This function gets the text of a control, then determines the width and height.
*
*   \ingroup rwutilgui
*
*   \param hWnd handle to the window containing the control
*   \param ResourceID resource id of the control
*
*   \return the dimensions of the text in logical units 
*/
RWUTIL_API SIZE             RwUtilGetControlTextSize(HWND hWnd, int ResourceID);
 
/**
*   This function gets the current selection of a combo box.
*
*   \ingroup rwutilgui
*
*   \param hWnd handle to the window containing the combo box
*   \param ResourceID resource id of the combo box
*
*   \return 
*/
RWUTIL_API int             RwUtilGetComboBox(HWND hWnd, int ResourceID);

/**
*   This function sets the state of a check box.
*
*   \ingroup rwutilgui
*
*   \param hWnd handle to the window containing the check box
*   \param ResourceID resource id of the check box
*   \param Value new state of the check box
*
*/
RWUTIL_API void            RwUtilSetCheckBoxState(HWND hWnd, int ResourceID, bool Value);

/**
*   This function sets the text of an edit box from a float.
*
*   \ingroup rwutilgui
*
*   \param hWnd handle to the window containing the edit box
*   \param ResourceID resource id of the edit box
*   \param Value value to enter in the edit box
*
*/
RWUTIL_API void            RwUtilSetEditBoxAsFloat(HWND hWnd, int ResourceID, float Value);

/**
*   This function sets the text of an edit box from an integer.
*
*   \ingroup rwutilgui
*
*   \param hWnd handle to the window containing the edit box
*   \param ResourceID resource id of the check box
*   \param Value value to enter in the edit box
*
*/
RWUTIL_API void            RwUtilSetEditBoxAsInt(HWND hWnd, int ResourceID, int Value);

/**
*   This function sets the text of an edit box.
*
*   \ingroup rwutilgui
*
*   \param hWnd handle to the window containing the edit box
*   \param ResourceID resource id of the check box
*   \param Value value to enter in the edit box
*
*/
RWUTIL_API void            RwUtilSetEditBoxAsString(HWND hWnd, int ResourceID, RwWString Value);

/**
*   This function sets the selection of a combo box.
*
*   \ingroup rwutilgui
*
*   \param hWnd handle to the window containing the combo box
*   \param ResourceID resource id of the combo box
*   \param Value selection to select in the combo box
*
*/
RWUTIL_API void            RwUtilSetComboBox(HWND hWnd, int ResourceID, int Value);

/**
*   This function adjusts the position of a dialog so that it is centred within (or about)
*   it's parent window.
*
*   \ingroup rwutilgui
*
*   \param hWnd handle to the window to be centred
*
*/
RWUTIL_API void            RwUtilCenterDialogWithinOwner(HWND hWnd);

/**
*   This function extracts a string resource from a resource module.
*
*   \ingroup rwutilgui
*
*   \param resourceInstance handle to the resource module
*   \param id resource id
*
*   \return the resource string returned if successful, empty string otherwise
*/
RWUTIL_API RwWString        RwUtilGetResourceString(HINSTANCE resourceInstance, int id);

/**
*   This function displays a directory pick dialog.
*
*   \ingroup rwutilgui
*
*   \param parent the parent of the directory pick dialog created
*   \param title the title of the directory pick dialog created
*   \param initialDir the initial directory of the directory pick dialog created
*   \param outputDir the directory selected in the directory pick dialog
*
*   \return true if the user selected OK, false otherwise
*/
RWUTIL_API bool            RwUtilDirectoryPick(const HWND parent, const RwWString &title,
                                    const RwWString &initialDir, RwWString &outputDir);

/**
*   This function returns the topmost parent of the window passed. It does this by
*   calling GetParent until it returns NULL.
*
*   \ingroup rwutilgui
*
*   \param hWnd the window to find the topmost parent from
*
*   \return the topmost parent of hWnd
*/
RWUTIL_API HWND            RwUtilGetTopParent(HWND hWnd);


/**
*   This function creates a string representation of a GUID.
*
*   \ingroup rwutilgui
*
*
*   \return string version of the GUID created
*/
RWUTIL_API RwWString        RwUtilCreateGUID();

/**
*   This function converts a static text control to a hyperlink (which is coloured blue
*   and is underlined). When the mouse cursor is moved over the hyperlink it changes to
*   a hand icon.
*
*   \ingroup rwutilgui
*
*   \param hdlg the window containing the text to be made a link
*   \param id the id of the text to be made a link
*   \param url the URL to link to
*
*/
RWUTIL_API void            RwUtilSetDlgItemUrl(HWND hdlg, int id, const wchar_t *url);

/**
*   Class RwUtilWarningDialog
*   On creation generates a modal warning dialog box
*
*   \ingroup rwutilgui
*
*   \param strDialogTitle the title for the dialog box
*   \param strMessageLine1 line 1 of the warning message
*   \param strMessageLine1 line 2 of the warning message
*   \param displayRegKey if registry key name is true the dialog is displayed (leave blank to disable "don't show this again")
*/

class RWUTIL_API RwUtilWarningDialog
{
public:
    RwUtilWarningDialog(const RwWString& strDialogTitle,
                        const RwWString& strMessageLine1,
                        const RwWString& strMessageLine2,
                        const RwWString& displayRegKey = L"");
    ~RwUtilWarningDialog() {}

public:
    LRESULT DialogFunc(HWND hwndCtl, UINT TheMsg, WPARAM wParam, LPARAM lParam);

private:
    RwWString m_strDialogTitle;
    RwWString m_strMessageLine1;
    RwWString m_strMessageLine2;
    RwWString m_displayRegKey;
};


#endif // RWUTIL_GUI_H
