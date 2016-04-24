// ArchiveQueue.cpp : implementation file
//

#include "stdafx.h"
#include "csda.h"
#include "ArchiveQueue.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "CasterScen.h"
#include "CasterScenArchiver.h"
#include "ScenMgr.h"




/////////////////////////////////////////////////////////////////////////////
// CArchiveQueue
//
//   A background thread that process requests to archive scenarios.
//
//   Maintains queues of requests, and processes them.
//
/////////////////////////////////////////////////////////////////////////////


IMPLEMENT_DYNCREATE(CArchiveQueue, CWinThread)

CArchiveQueue::CArchiveQueue()
{
	m_bAutoDelete = false;
	m_pfnThreadProc = MyThreadProc;
	m_pThreadParams = this;
}

CArchiveQueue::~CArchiveQueue()
{
}

BOOL CArchiveQueue::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CArchiveQueue::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CArchiveQueue, CWinThread)
	//{{AFX_MSG_MAP(CArchiveQueue)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CArchiveQueue message handlers



//
//  Make sure a thread exists
//

void CArchiveQueue::MaybeCreateThread()
{
	if ( m_hThread == 0 )
		CreateThread(CREATE_SUSPENDED);
}



//
//  Add a request to the background queue
//

void CArchiveQueue::AddBackground(CCasterScen* pOrig, CCasterScen* pCopy)
{	
	CSingleLock lock(&m_qCritSec,true);
	
	m_bgQCopy.push_back(pCopy);
	m_bgQOrig.push_back(pOrig);

#ifdef _DEBUG
	afxDump << "CArchiveQueue:: Adding background job for " << pOrig->Caster()
			<< "/" << pOrig->Id() << "\n";
#endif
	
	MaybeCreateThread();
	ResumeThread();
}


//
//  Add a request to the foreground queue
//

void CArchiveQueue::AddForeground(CCasterScen* pOrig, CCasterScen* pCopy)
{
	CSingleLock lock(&m_qCritSec,true);
	
	m_fgQCopy.push_back(pCopy);
	m_fgQOrig.push_back(pOrig);

#ifdef _DEBUG
	afxDump << "CArchiveQueue:: Adding foreground job for " << pOrig->Caster()
			<< "/" << pOrig->Id() << "\n";
#endif

	MaybeCreateThread();
	ResumeThread();
}


//
//  Remove a request from the queues
//

void CArchiveQueue::RemoveOriginal(CCasterScen* pOrig)
{
	CSingleLock lock(&m_qCritSec,true);
	RemoveOriginal(pOrig,m_fgQOrig,m_fgQCopy);
	RemoveOriginal(pOrig,m_bgQOrig,m_bgQCopy);
	
#ifdef _DEBUG
	afxDump << "CArchiveQueue:: Removing any jobs for " << pOrig->Caster()
			<< "/" << pOrig->Id() << "\n";
#endif

}


//
// remove a request from the designated queue
//

void CArchiveQueue::RemoveOriginal(CCasterScen* pOrig,
								   deque<CCasterScen*>& vOrig,
								   deque<CCasterScen*>& vCopy)
{
	deque<CCasterScen*>::iterator io = find(vOrig.begin(),
											 vOrig.end(),
											 pOrig);

	if ( io == vOrig.end() )
		return;

	int index = io - vOrig.begin();
	deque<CCasterScen*>::iterator ic = vCopy.begin() + index;

	vOrig.erase(io);

	CScenMgr::DeleteArchivalCasterScen( (*ic) );

	vCopy.erase( ic );
}

										

//static 
UINT CArchiveQueue::MyThreadProc(LPVOID self)
{
	((CArchiveQueue*)self)->MyThreadMethod();
	return 0;
}


//
//  To run the thread, process all requests on the queue.
//  Foreground jobs have precedence.
//

void CArchiveQueue::MyThreadMethod()
{
#ifdef _DEBUG
	afxDump << "CArchiveQueue:: Entering MyThreadMethod()\n";
#endif

	while ( 1 ) {
		
		CCasterScen* pOrig;
		CCasterScen* pCopy;

#ifdef _DEBUG		
		afxDump << "CArchiveQueue:: Examining queues ...";
#endif


		bool isFG = GetNextScen(pOrig,pCopy);

		if ( pCopy == 0 ) {
#ifdef _DEBUG
			afxDump << " no entries, suspending.\n";
#endif
			SuspendThread();
		}
		else  {
#ifdef _DEBUG
			afxDump << " found an entry, archiving.\n";
#endif
			Archive(isFG, pOrig, pCopy);
		}
	}
}


//
//  Gives precedence to the foreground queue.
//  Return a flag indicating which queue.
//  Return the scen and its copy indirectly.
//

bool CArchiveQueue::GetNextScen(CCasterScen*& rpOrig,
								CCasterScen*& rpCopy)
{
	rpOrig = 0;
	rpCopy = 0;

	CSingleLock lock(&m_qCritSec,true);
	
	if ( ! m_fgQCopy.empty() ) {
		rpCopy = m_fgQCopy.front();
		rpOrig = m_fgQOrig.front();
		m_fgQCopy.pop_front();
		m_fgQOrig.pop_front();
		return true;
	}
	else if ( ! m_bgQCopy.empty() ) {
		rpCopy = m_bgQCopy.front();
		rpOrig = m_bgQOrig.front();
		m_bgQCopy.pop_front();
		m_bgQOrig.pop_front();
		return false;
	}

	return false;
}



//
//  Actually perform the archive
//

void CArchiveQueue::Archive(bool isFG, 
							CCasterScen* pOrig, 
							CCasterScen* pCopy)
{
#ifdef _DEBUG
	afxDump << " archiving " << pOrig->Caster()
			<< "/" << pOrig->Id() << "\n";
#endif

	CCasterScenArchiver::Archive(pCopy,false);
	CScenMgr::DeleteArchivalCasterScen(pCopy);
}

