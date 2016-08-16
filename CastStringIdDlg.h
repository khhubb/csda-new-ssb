#if !defined(AFX_CASTSTRINGIDDLG_H__314DAEA4_DAFD_11D0_8017_006097B38214__INCLUDED_)
#define AFX_CASTSTRINGIDDLG_H__314DAEA4_DAFD_11D0_8017_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// CastStringIdDlg.h : header file
//


#include "CastString.h"
#include "SpecChemRangePreset.h"
#include "Caster.h"


/////////////////////////////////////////////////////////////////////////////
// CCastStringIdDlg dialog

class CCastStringIdDlg : public CDialog
{
// Construction
public:
	CCastStringIdDlg(CWnd* pParent = NULL);   // standard constructor


	CCastStringId* m_pId;
		// the id being edited

	CCastStringMiscProps* m_pProps;
		// the miscProps being edited

	vector<CCastStringId> m_usedIds[Caster::CasterArrayLen];
		// caststring ids that are in use, per caster = 1,2,3,4,5

	vector<CSpecChemRangePreset> m_selectedSpecChemRangePresets;
		// holds the selected presets.

	bool m_isEditing;
		// two modes: editing and crearing.

	bool m_setInitialId;
		// set by the user to indicate if the id fields should be initialized

	bool m_idChanged;
		// flags whether changes have been made.

	SECDateTimeCtrl m_dateTimeCtrl;
		// control

	void InitSpecEntries();
	void SetChemPresets();

	void SetHeatSizeExtremes(int casterNum, bool updateString);
	void SetTons(int numHeats);
	void SetStrand2Enabled(int casterNum);
	void SetWidthSpinLimits(int casterNum);
	void SetTurnaroundTime(int casterNum);

	void SetInitialId(int casterNum);



// Dialog Data
	//{{AFX_DATA(CCastStringIdDlg)
	enum { IDD = IDD_CAST_STRING_ID };
	CEdit	m_editTurnaround;
	CComboBox	m_comboPresets;
	CComboBox	m_comboSpec;
	CEdit	m_editStrand1Start;
	CEdit	m_editStrand1End;
	CSpinButtonCtrl	m_spinStrand2End;
	CSpinButtonCtrl	m_spinStrand1End;
	CSpinButtonCtrl	m_spinStrand2Start;
	CSpinButtonCtrl	m_spinStrand1Start;
	CEdit	m_editStrand2Start;
	CEdit	m_editStrand2End;
	CSpinButtonCtrl	m_spinNumHeats;
	CEdit	m_editTons;
	CEdit	m_editNumHeats;
	CSpinButtonCtrl	m_spinYear;
	CSpinButtonCtrl	m_spinWeek;
	CSpinButtonCtrl	m_spinNum;
	CSpinButtonCtrl	m_spinCaster;
	CEdit	m_editYear;
	CEdit	m_editWeek;
	CEdit	m_editNum;
	CEdit	m_editCaster;
	int		m_caster;
	int		m_num;
	int		m_week;
	int		m_year;
	int		m_numHeats;
	long	m_tons;
	int		m_heatSizeAim;
	int		m_heatSizeMax;
	int		m_heatSizeMin;
	int		m_heatSizeMaxLimit;
	int		m_heatSizeMinLimit;
	int		m_strand1End;
	int		m_strand2End;
	int		m_strand1Start;
	int		m_strand2Start;
	float	m_maxMn;
	float	m_maxCarbon;
	float	m_minCarbon;
	float	m_minMn;
	CString	m_spec;
	int		m_turnaround;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCastStringIdDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCastStringIdDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnKillfocusEditNumHeats();
	afx_msg void OnDeltaposSpinNumHeats(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinCaster(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillfocusEditCaster();
	afx_msg void OnKillfocusEditHeatSizeAim();
	afx_msg void OnChangeEditNum();
	afx_msg void OnChangeEditWeek();
	afx_msg void OnChangeEditYear();
	afx_msg void OnDeltaposSpinNum(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinWeek(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinYear(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeComboPresets();
	afx_msg void OnSelchangeComboSpec();
	afx_msg void OnEditchangeComboSpec();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CASTSTRINGIDDLG_H__314DAEA4_DAFD_11D0_8017_006097B38214__INCLUDED_)
