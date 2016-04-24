//
//  clipFormats.h
//
//
//		Defines a statics-only class that registers the clipboard formats
//		used for various drag-drop operations.
//
//		One instance of this should be created to get initialization to occur.
 

#ifndef _clipFormats_included_
#define _clipFormats_included_

class CClipFormat {

	static CLIPFORMAT sm_availOrderGroupFormat;
	static CLIPFORMAT sm_availOrderOrderFormat;
	static CLIPFORMAT sm_casterScenOrderFormat;
	static CLIPFORMAT sm_insertStockOrderFormat;
	static CLIPFORMAT sm_insertOrderFormat;
	
	static bool sm_staticsInitialized;
	static void InitializeStatics();

public:

	CClipFormat();

	static CLIPFORMAT& AvailOrderGroupFormat() 
	{ 
		assert( sm_staticsInitialized ); 
		return sm_availOrderGroupFormat;
	}

	static CLIPFORMAT& AvailOrderOrderFormat()
	{ 
		assert( sm_staticsInitialized ); 
		return sm_availOrderOrderFormat;
	}

	static CLIPFORMAT& CasterScenOrderFormat()
	{ 
		assert( sm_staticsInitialized ); 
		return sm_casterScenOrderFormat;
	}

	static CLIPFORMAT& InsertStockOrderFormat()
	{	
		assert( sm_staticsInitialized );
		return sm_insertStockOrderFormat;
	}
	
	static CLIPFORMAT& InsertOrderFormat()
	{	
		assert( sm_staticsInitialized );
		return sm_insertOrderFormat;
	}
	
};




#endif