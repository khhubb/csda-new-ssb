// UserInfo.h: interface for the CUserInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USERINFO_H__D1505336_4B0F_11D1_804B_006097B38214__INCLUDED_)
#define AFX_USERINFO_H__D1505336_4B0F_11D1_804B_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
 
class CUserInfo  
{
	////////////////////////////////////////////////////////////////
	//	
	//	Hold information about the user who is logged in 
	//    to this process.
	//	
	////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////
	//	
	//	Data
	//	
	////////////////////////////////////////////////////////////////

protected:

	CString m_loginName;
	CString m_lastName;
	CString m_firstName;
	CString m_sbId;


	////////////////////////////////////////////////////////////////
	//	
	//	Accessors
	//	
	////////////////////////////////////////////////////////////////

public:

	const CString& LoginName() const { return m_loginName; }
	const CString& LastName()  const { return m_lastName;  }
	const CString& FirstName() const { return m_firstName; }
	const CString& SBId()      const { return m_sbId;      }


	////////////////////////////////////////////////////////////////
	//	
	//	C-tors, d-tor
	//	
	////////////////////////////////////////////////////////////////


public:

	CUserInfo(const CString& loginName,
			  const CString& lastName,
			  const CString& firstName,
			  const CString& sbId)
			  :
		m_loginName(loginName),
		m_lastName(lastName),
		m_firstName(firstName),
		m_sbId(sbId)
	{
	}

	CUserInfo() {}

	virtual ~CUserInfo() {}


	// we need exactly one of these

	static CUserInfo TheUser;

};

#endif // !defined(AFX_USERINFO_H__D1505336_4B0F_11D1_804B_006097B38214__INCLUDED_)
