// various user-defined resources

//  Our very own WM_ messages

#define WM_USER_STEP_OPN  (WM_USER+1)
	// Used in the CLongOpn dialog to communicate between threads

#define WM_USER_SHOW_OUTPUT_WINDOW (WM_USER+2)
	// Used by NOutputWnd to send a message from a worker thread
	// to the main frame to cause it to pop up the output window dialog bar

#define WM_USER_ACTIVATE_TAB (WM_USER+3)
	//  Used by NOutputWnd to activate a specific tab in the output window dialog bar

 