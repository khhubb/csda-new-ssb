// DocMvcMixin.h: interface for the CDocMvcMixin class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOCMVCMIXIN_H__AEF38696_E7EC_11D3_862C_00104B2D39AC__INCLUDED_)
#define AFX_DOCMVCMIXIN_H__AEF38696_E7EC_11D3_862C_00104B2D39AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMessageBus;

class CDocMvcMixin  
{
	////////////////////////////////////////////////////////////////
	//	
	//	Mixin in to all CDocument-derived classes
	//  Provides some basic add-in functionality that is standard
	//    for interfacing CDocument to the MVC mechanism.
	//	
	////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////
	//	
	//	Typedefs, enums, etc.
	//	
	////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////
	//	
	//	Data
	//	
	////////////////////////////////////////////////////////////////

protected:

	CMessageBus* m_pBus;


	////////////////////////////////////////////////////////////////
	//	
	//	Accessors
	//	
	////////////////////////////////////////////////////////////////

public:

	CMessageBus* GetModel() { return m_pBus; }


	////////////////////////////////////////////////////////////////
	//	
	//	C-tors, d-tor
	//	
	////////////////////////////////////////////////////////////////

public:

	CDocMvcMixin();
	virtual ~CDocMvcMixin() {}


	////////////////////////////////////////////////////////////////
	//	
	//	Operators
	//	
	////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////
	//	
	//	Operations
	//	
	////////////////////////////////////////////////////////////////

public:
	
	virtual void ActivationChange(BOOL activated) {}


	////////////////////////////////////////////////////////////////
	//	
	//	Implementation
	//	
	////////////////////////////////////////////////////////////////

};

#endif // !defined(AFX_DOCMVCMIXIN_H__AEF38696_E7EC_11D3_862C_00104B2D39AC__INCLUDED_)
