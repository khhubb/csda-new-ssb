#if !defined(AFX_ARCHIVEQUEUE_H__4CF06D25_6DA6_11D2_85C3_00104B2D39AC__INCLUDED_)
#define AFX_ARCHIVEQUEUE_H__4CF06D25_6DA6_11D2_85C3_00104B2D39AC__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ArchiveQueue.h : header file
//

class CCasterScen;

/////////////////////////////////////////////////////////////////////////////
// CArchiveQueue thread
//
// Queues requests for archiving scenarios, 
//   runs a background thread to do the archiving.
//
////////////////////////////////////////////////////////////////////////////


class CArchiveQueue : public CWinThread
{
	DECLARE_DYNCREATE(CArchiveQueue)

public:
	CArchiveQueue();           // protected constructor used by dynamic creation
	virtual ~CArchiveQueue();

// Attributes
public:

// Operations
public:

	void AddBackground(CCasterScen* pOrig, CCasterScen* pCopy);
	void AddForeground(CCasterScen* pOrig, CCasterScen* pCopy);
	void RemoveOriginal(CCasterScen* pOrig);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CArchiveQueue)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:

	void MaybeCreateThread();


	CCriticalSection m_qCritSec;
		// The main thread will add items to the deques.
		// we need to control access to make this thread-safe.

	deque<CCasterScen*> m_bgQOrig;
	deque<CCasterScen*> m_bgQCopy;
	deque<CCasterScen*> m_fgQOrig;
	deque<CCasterScen*> m_fgQCopy;
		// Queues for scenarios that need saving.

	static UINT MyThreadProc(LPVOID ignore);
	void MyThreadMethod();

	void RemoveOriginal(CCasterScen* pOrig,
						deque<CCasterScen*>& vOrig,
						deque<CCasterScen*>& vCopy);

	bool GetNextScen(CCasterScen*& rpOrig,
					 CCasterScen*& rpCopy);

	void Archive(bool isFG,
				 CCasterScen* pOrig,
				 CCasterScen* pCopy);


	// Generated message map functions
	//{{AFX_MSG(CArchiveQueue)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ARCHIVEQUEUE_H__4CF06D25_6DA6_11D2_85C3_00104B2D39AC__INCLUDED_)
