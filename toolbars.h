#ifndef _toolbars_included_
#define _toolbars_included_


//
// Customizable toolbar configuration information
//


// Establish default toolbars
// All buttons available for use in the customizable toolbar must
// be represented in 1 toolbar bitmap, which is loaded by the toolbar
// manager at initialization (actually there can be 2 bitmaps, a small
// and large view of the same data). These buttons can be dragged
// and dropped all over the desktop in completely unpredictable ways.
// The following arrays represent DEFAULT toolbars available to the
// application (and administered by the toolbar manager). These
// are primarily used as working categories of buttons in the 
// "customize" dialogs, but are also used for the default toolbars
// to be displayed when no workspace info is available.
// Since these toolbars are HIGHLY customizable, it is strongly
// advised that this implementation is used in conjunction with
// the workspace manager (SECWorkspaceManager) to save settings
// across multiple executions.
//
// Note: Although this information is almost always known at compile
// time, these defaults could be generated dynamically as well.


// The comments above come from Stingray documentation.
// We don't currently use either of these, but keep them here for documentation.


// "File" default toolbar
static UINT BASED_CODE fileButtons[] =
{
	ID_FILE_NEW,
	ID_FILE_OPEN,
	ID_FILE_SAVE,
	ID_FILE_SAVEALL,
	ID_SEPARATOR,
	ID_EDIT_CUT,
	ID_EDIT_COPY,
	ID_EDIT_PASTE,
	ID_EDIT_DELETE,
	ID_SEPARATOR,
	ID_FILE_PRINT,
	ID_APP_ABOUT,
};

// "Edit" default toolbar
static UINT BASED_CODE editButtons[] =
{
	ID_EDIT_BM_TOGGLE,
	ID_EDIT_BM_NEXT,
	ID_EDIT_BM_PREV,
	ID_EDIT_BM_CLEARALL,
	ID_EDIT_FIND,
	ID_EDIT_FIND_INFILES
};



//
// Button map
// Use this to define mappings of command ids from the toolbar manager's 
// bitmap resource to non-standard toolbar buttons such as
// "2-part" buttons (undo-redo, for example) and combo boxes

BEGIN_BUTTON_MAP(btnMap)
//	COMBO_BUTTON(ID_EDIT_FIND, IDC_COMBO_FIND, SEC_TBBS_VCENTER, CBS_DROPDOWN, 150, 70, 150)
END_BUTTON_MAP()





#endif
