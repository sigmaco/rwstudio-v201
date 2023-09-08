
' Messages.vbs

' This file is auto-generated from RWSHints.h and contains the
' list of RenderWare Studio reserved messages and associated
' helper functions.

Option Explicit ' All variables must be explicitly declared

Const conEventsSuspended = "RWSMSG_EVENTSSUSPENDED" ' Events have been suspended if nData is RWSTrue, enabled if nData is RWSFalse
Const conReloadAsset = "RWSMSG_RELOADASSET" ' Force a reload of the specified asset. nData contains an RWSID of the asset
Const conAssetRefreshed = "RWSMSG_ASSETREFRESHED" ' An asset was loaded/reloaded, so update any view of it
Const conPreImportAsset = "RWSMSG_PREIMPORTASSET" ' An asset is about to be imported.
Const conPostImportAsset = "RWSMSG_POSTIMPORTASSET" ' An asset has just been imported.
Const conPostImportAssets = "RWSMSG_POSTIMPORTASSETS" ' All assets have just been imported.
Const conDisable3DRefresh = "RWSMSG_DISABLE3DREFRESH" ' Temporarily suspend/re-enable calls to refresh the 3D views
Const conInvalidateAssets = "RWSMSG_INVALIDATEASSETS" ' Invalidates all assets, so that the asset previews will be reloaded.
Const conClipboardChanged = "RWSMSG_CLIPBOARDCHANGED" ' Informs that a cut/copy operation has taken place
