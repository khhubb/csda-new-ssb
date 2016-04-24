// Scenario.h: interface for the CScenario class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCENARIO_H__F60F11C4_A60D_11D0_975C_0000C0C2484C__INCLUDED_)
#define AFX_SCENARIO_H__F60F11C4_A60D_11D0_975C_0000C0C2484C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

 
class CScenario 
{
	////////////////////////////////////////////////////////////////
	//	
	//	Base class for CCasterScen, CProdnScen
	//	
	////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////
	//	
	//	Data members
	//	
	////////////////////////////////////////////////////////////////

protected:

	long m_id;
		//  A numeric identifier, unique among all scenarios during invocation
		//  Assigned by a trusted source.
		//  When first created, a temporary id is assigned.
		//  When appropriate later, we call the trusted source to generate
		//    an official, public id.
		//  We only allow that id to be set once.

	bool m_idAssigned;
		// indicates if the official public id has been assigned

	CString m_owner;
		// the owner, from the login name

	CString m_comment;
		// the user can assign a comment

	bool m_canEdit;
		//  I believe only the scenarios attached to the snapshot cannot be edited

	bool m_isModified;
		//  Don't think we really use this flag anymore -- TODO -- check it out.

	static int m_counter;
		//  Used to generate ids



	////////////////////////////////////////////////////////////////
	//	
	//	Accessors/modifiers
	//	
	////////////////////////////////////////////////////////////////

public:

	long Id() const
	{ return m_id; }

	void SetId(long newId)
	{	
		assert( ! m_idAssigned );
		m_idAssigned = true;
		m_id = newId;
	}

	void SetIdBoldly(long newId)
	{	m_id = newId;	}

	const CString& Comment() const
	{ return m_comment; }

	void SetComment(const CString& newComment)
	{	m_comment = newComment; }

	const CString& Owner() const
	{	return m_owner;	}

	void SetOwner(const CString& newOwner)
	{	m_owner = newOwner;	}

	bool CanEdit() const
	{ return m_canEdit; }

	bool IsModified() const
	{	return m_isModified;	}

	void SetModified(bool newVal)
	{	m_isModified = newVal;	}


protected:

	//  We want ability to change this to rest with CScenMgr only.
	//  Derived classes will grant friend status to CScenMgr.

	void SetCanEdit(bool newVal)
	{ m_canEdit = newVal; }


	////////////////////////////////////////////////////////////////
	//	
	//	C-tors, d-tor
	//	
	////////////////////////////////////////////////////////////////

public:

	CScenario();
	virtual ~CScenario() {};
		// This is a base class, so d-tor needs to be virtual

private:

	//  do not implement
	CScenario(const CScenario&);
	CScenario& operator=(const CScenario&);
	//  do not implement



};



#endif // !defined(AFX_SCENARIO_H__F60F11C4_A60D_11D0_975C_0000C0C2484C__INCLUDED_)
