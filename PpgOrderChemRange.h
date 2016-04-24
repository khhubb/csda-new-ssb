#if !defined(AFX_PPGORDERCHEMRANGE_H__D61C792E_FF8D_11D0_801F_006097B38214__INCLUDED_)
#define AFX_PPGORDERCHEMRANGE_H__D61C792E_FF8D_11D0_801F_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// PpgOrderChemRange.h : header file
//


class COrderSelection;

class CPpgOrderChemRange;
#include "SpecChemRangePreset.h"


//
//  SECDropEdit is an edit box with a button.
//  We subclass this to bring up a calculator to enter a chem range value.
//

class CChemBoundEdit : public SECDropEdit
{
public:
	
	CChemBoundEdit() : m_pMyParent(0) {}

	void SetMyParent(CPpgOrderChemRange* pParent)
	{	m_pMyParent = pParent;	}

protected:

	CPpgOrderChemRange* m_pMyParent;

	virtual void OnClicked();

	DECLARE_MESSAGE_MAP();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
};


/////////////////////////////////////////////////////////////////////////////
// CPpgOrderChemRange dialog

class CPpgOrderChemRange : public CPropertyPage
{
	DECLARE_DYNCREATE(CPpgOrderChemRange)

// Construction
public:
	CPpgOrderChemRange();
	~CPpgOrderChemRange();

// Dialog Data
	//{{AFX_DATA(CPpgOrderChemRange)
	enum { IDD = IDD_PPG_ORDER_CHEM_RANGE };
	CListBox	m_listPresetRanges;
	CString	m_chem3;
	CString	m_chem4;
	CString	m_chem5;
	double	m_dMaxChem1;
	double	m_dMaxChem2;
	double	m_dMaxChem3;
	double	m_dMaxChem4;
	double	m_dMaxChem5;
	double	m_dMinChem1;
	double	m_dMinChem2;
	double	m_dMinChem3;
	double	m_dMinChem4;
	double	m_dMinChem5;
	CString	m_presetRange;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPpgOrderChemRange)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPpgOrderChemRange)
	virtual BOOL OnInitDialog();
	afx_msg void OnEditchangeComboChemAll();
	afx_msg void OnChangeEditMaxChemAll();
	afx_msg void OnChangeEditMinChemAll();
	afx_msg void OnSelchangeComboChemAll();
	afx_msg void OnButtonResetAll();
	afx_msg void OnButtonResetCarbon();
	afx_msg void OnButtonResetManganese();
	afx_msg void OnButtonReset3();
	afx_msg void OnButtonReset4();
	afx_msg void OnButtonReset5();
	afx_msg void OnSelchangeListPresetRanges();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


	CChemBoundEdit m_minEdits[5];
	CChemBoundEdit m_maxEdits[5];
		// Buttons for each of the chem ranges

	vector<int> elts;
		// the element in each row.

	vector<CString> eltNames;
		// the names of the elements in each row

	vector<CSpecChemRangePreset> selectedSpecChemRangePresets;
		// selected preset ranges

	COrderSelection* m_pOrderSelection;
		// the object being edited

	bool m_hasBeenActivated;
		// has the page been activated since the last Apply

	void SetPresetSelections();
	void SetChemListEntries();



public:

	bool HasBeenActivated() { return m_hasBeenActivated; }
	void SetOrderSelection(COrderSelection* pSelect);
	bool LocalToOrderSelection(bool really = true);
	void OrderSelectionToLocal();


};





//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PPGORDERCHEMRANGE_H__D61C792E_FF8D_11D0_801F_006097B38214__INCLUDED_)
