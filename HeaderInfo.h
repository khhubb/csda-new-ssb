// HeaderInfo.h: interface for the CHeaderInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HEADERINFO_H__BC5C2137_308A_11D3_85E5_00104B2D39AC__INCLUDED_)
#define AFX_HEADERINFO_H__BC5C2137_308A_11D3_85E5_00104B2D39AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "WinUtils.h"

//////////////////////////////////////////////////////////////////////////
//
// THeaderInfo
//
//  This templated class encapsulates some standard practices
//   for handling headers in CListCtrls and SECTreeCtrls.
//
//  The templated class is a typename of an enumerated type.
//  This enumeration is used to label instances of this class
//  being used together in a single control.
//
//  An instance of this class represents one column.
//  Typically several instances are put together into a vector
//    to represent the set of columns of a control.
//  There are several auxiliary functions which work with such a vector.
//
//	Usage:
//
//
//		enum E_LIST_HEADER { LH_1, LH_2, LH_3, ... };
//		enum { NUM_LHS = LH_N+1	};		// set to number of enum values
//
//		// often, create a class derived from CHeaderInfo.
//		class CMyHeaderInfo : public CHeaderInfo<E_LIST_HEADER>
//		{	
//			...
//		};
//	
//		vector<CMyHeaderInfo> m_allHeaders;		// this might typically be a data member
//
//
//		// somewhere, such as OnInitDialog or OnInitialUpdate:
//
//		// We can preallocate, or build incrementally with push_back
//
//		allHeaders.resize(NUM_LHS);	
//
//		// assume in allHeaders, each is in order by index.
//		//  that is index is same as type, as identified by the enumerated type value
//		// this is typically the case, but not necessarily
//
//		{
//			int index = 0;
//			for ( vector<CHeaderInfo>::iterator ih = m_allHeaders.begin();
//				  ih != m_allHeaders.end();	
//				  ++ih, ++index )
//				(*ih).m_type = E_LIST_HEADER(index);
//		}
//
//		// Each entry has at least the m_string set.
//		// Options are:
//		//   m_string -- the string to display in the header for the column
//		//	 m_type   -- the enumerated value identifying this column.
//		//	 m_item   -- another string which can be used to set the width.
//		//   m_format -- one of LVCFMT_RIGHT, LVCFMTLEFT, LVCFMTCENTER
//		//   m_width  -- width of column in pixels.
//		//				 you can set it yourself, but more typically have it
//		//				 computed automatically from m_string and m_item
//				
//		allHeaders[ LH_1	].m_string = "Coil#";
//		allHeaders[ LH_1	].m_item   = "999999-999";
//		allHeaders[ LH_2	].m_string = "Order#";
//		allHeaders[ LH_3	].m_string = "Gauge";
//		allHeaders[ LH_3	].m_format = LVCFMT_RIGHT;
//
//	
//		// Typically, compute the width from the m_string and m_item strings
//		// We must pass a pointer to a CListCtrl or SECTreeCtrl
//		// in order to pick up the font to compute the width.
//		THeaderInfoAux::SetWidths(m_pListCtrl,allHeaders);
//
//		// Typically, use this auxiliary to install the headers on the control
//		THeaderInfoAux::SetHeader(m_pListCtrl,allHeaders);
//
//
////////////////////////////////////////////////////////////////////////////////


template<typename E_Id>
class THeaderInfo  
{
public:
	typedef E_Id T_Type;
		// don't know why, never used it

	E_Id m_type;
		// an identifying value for this column

	int m_index;
		// not really used.
		// presumably, the column number of the column

	CString m_string;
		// the string to display at the head of the column

	CString m_item;
		// another string to help determine width of the column
		// larger (in pixels, using current font) of m_string and m_item 
		//    will determine width

	int m_width;
		// the width of the column, in pixels

	int m_format;
		// the format, one of LVCFMT_LEFT ,LVCFMT_CENTER ,LVCFMT_RIGHT

	// c-tor, nominal values
	THeaderInfo() {
		m_type = E_Id(0);
		m_index = 0;
		m_string = "";
		m_item = "";
		m_width = 0;
		m_format = LVCFMT_LEFT; 
	}
	
	// Compute width from m_string and m_item.
	// Typically you need to add some padding to get the value
	//    strings to display completely.

	virtual void SetWidth(CListCtrl& rCtrl,int padding=15)
	{	
		m_width = padding + max(rCtrl.GetStringWidth(m_string),
								rCtrl.GetStringWidth(m_item));
	}
	virtual void SetWidth(CListCtrl* pCtrl,int padding=15)
	{
		SetWidth(*pCtrl,padding);
	}

	virtual void SetWidth(CDC& dc,int padding=15)
	{
		m_width = padding + max(dc.GetTextExtent(m_string).cx,
								dc.GetTextExtent(m_item).cx);
	}



	virtual ~THeaderInfo() {}

};


//  A namespace holding auxiliary functions for manipulating THeaderInfos

namespace THeaderInfoAux
{


// insert one column per header

template<class T_HeaderInfo>
void SetHeader(CListCtrl* pCtrl, const vector<T_HeaderInfo>& headers)
{
	WinUtils::DeleteHeaderItems(pCtrl);

	int i = 0;
	for ( vector<T_HeaderInfo>::const_iterator ih = headers.begin();
		  ih != headers.end();
		  ++ih, ++i )

		pCtrl->InsertColumn(i,(*ih).m_string,(*ih).m_format,(*ih).m_width,i);
}


// insert one column per header

template<class T_HeaderInfo>
void SetHeader(SECTreeCtrl* pCtrl, const vector<T_HeaderInfo>& headers)
{
	pCtrl->DeleteAllColumns();

	int i = 0;
	for ( vector<T_HeaderInfo>::const_iterator ih = headers.begin();
		  ih != headers.end();
		  ++ih, ++i )
		pCtrl->AddColumn((*ih).m_string,(*ih).m_format,(*ih).m_width,i);
}


// set widths on an entire array of CHeaderInfo instances

template<class T_HeaderInfo>
void SetWidths(CListCtrl* pCtrl, vector<T_HeaderInfo>& headers, int padding=15)
{
	for ( vector<T_HeaderInfo>::iterator ih = headers.begin();
		  ih != headers.end();
		  ++ih )
		(*ih).SetWidth(pCtrl,padding);
}


// set widths on an entire array of CHeaderInfo instances

template<class T_HeaderInfo>
void SetWidths(CDC& dc,  vector<T_HeaderInfo>& headers, int padding=15)
{
	for ( vector<T_HeaderInfo>::iterator ih = headers.begin();
		  ih != headers.end();
		  ++ih )
		(*ih).SetWidth(dc,padding);
}

}	// namespace THeaderInfoAux



		


#endif // !defined(AFX_HEADERINFO_H__BC5C2137_308A_11D3_85E5_00104B2D39AC__INCLUDED_)
