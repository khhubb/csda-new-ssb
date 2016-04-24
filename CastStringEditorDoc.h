#if !defined(AFX_CASTSTRINGEDITORDOC_H__157132FF_13EB_11D1_8027_006097B38214__INCLUDED_)
#define AFX_CASTSTRINGEDITORDOC_H__157132FF_13EB_11D1_8027_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// CastStringEditorDoc.h : header file
//

class CSuperScen;
class CProdnScen;
class CCasterScen;
class CCastString;
class COrderSelection;

#include "SuperScen.h"
#include "ScenMru.h"
#include "DocBase.h"
#include "MessageBus.h"



/////////////////////////////////////////////////////////////////////////////
// CCastStringEditorDoc document

class CCastStringEditorDoc : public CDocBase
{
protected:
	CCastStringEditorDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CCastStringEditorDoc)

// Attributes
public:


public:
	//void ActivationChange(BOOL bActivate);  // called by frame



// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCastStringEditorDoc)
	public:
	virtual void SetTitle(LPCTSTR lpszTitle);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCastStringEditorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CCastStringEditorDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CASTSTRINGEDITORDOC_H__157132FF_13EB_11D1_8027_006097B38214__INCLUDED_)


#if 0

	// move to individual viewport/controllers

// from the message map
	afx_msg void OnEditOrderInquiry();
	afx_msg void OnEditHeatDetails();
	afx_msg void OnUpdateEditHeatDetails(CCmdUI* pCmdUI);


private:



public:


	void ActivateFrame();


	void StandardUpdate(int casterNum,bool doBackup, UpdateReason e);

private:
	// implementation

	void ValidateCastString();
	void UpdateTitle();
	void Init();
	void ValidateOrderSelection();

#endif