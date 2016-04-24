// NStateBitmapButton.cpp : implementation file
//

#include "stdafx.h"



#include "csda.h"
#include "NStateBitmapButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//	CNStateBitmapButton
//
//	Used to subclass a CButton.
//  Button has n states, each with its own image.
//  The current state is set by user command.
//  States are numbered 0-index.
//  The number of states will be the number of images.
//
//  Usage:
//
//		// subclassing
//		// somewhere, assume the declaration  CNStateBitmapButton myButton;
//		// Let  pWnd be a pointer to the window in which a button with id IDC_BUTTON resides.
//		myButton.SubclassDlgItem(IDC_BUTTON,pWnd);
//
//      // setting the images
//		{
//			vector<int> ids;
//			ids.push_back(IDB_IMG1);
//			ids.push_back(IDB_IMG2);
//
//			myButton.SetBitmaps(ids);
//		}
//
//		// later on, to change the displayed image
//		myButton.SetCurrentState( newState );
//
////////////////////////////////////////////////////////////////////////////////////////////



CNStateBitmapButton::CNStateBitmapButton()
{
	// initialize to nominal values

	m_currentState = 0;
	m_isInitialized = false;

}



CNStateBitmapButton::~CNStateBitmapButton()
{
	// destroy all the bitmaps that have been loaded

	for ( vector<CBitmap*>::iterator ib = m_bitmaps.begin();
		  ib != m_bitmaps.end();
		  ++ib )

		delete (*ib);

}



BEGIN_MESSAGE_MAP(CNStateBitmapButton, CButton)
	//{{AFX_MSG_MAP(CNStateBitmapButton)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNStateBitmapButton message handlers


void CNStateBitmapButton::SetBitmaps(vector<int>& ids)
{
	// allowing setting of bitmaps only once, for simplicity.
	assert( ! m_isInitialized );

	// we have to have at least one state
	assert( ids.size() > 0 );

	//  prevent multiple calls
	m_isInitialized = true;
	

	// load the bitmaps into the vector

	m_bitmaps.resize(ids.size());

	int index = 0;
	for ( vector<int>::iterator ii = ids.begin();
		  ii != ids.end();
		  ++ii, ++index ) {

		m_bitmaps[index] = new CBitmap();

		BOOL loadOk = m_bitmaps[index]->LoadBitmap( (*ii ) ) ;
		assert(loadOk);
	}


	SetCurrentState(0);
}



void CNStateBitmapButton::SetCurrentState(int newState)
{
	// we need to have images already
	assert( m_isInitialized );

	// state must be valid.
	assert( 0 <= newState  && newState < m_bitmaps.size() );


	m_currentState = newState;

	SetBitmap( HBITMAP(*m_bitmaps[m_currentState]) );
}
