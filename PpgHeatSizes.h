#if !defined(AFX_PPGHEATSIZES_H__642EE488_E2B3_11D0_8019_006097B38214__INCLUDED_)
#define AFX_PPGHEATSIZES_H__642EE488_E2B3_11D0_8019_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// PpgHeatSizes.h : header file
//


class CMiscConstants;


/////////////////////////////////////////////////////////////////////////////
// CPpgHeatSizes dialog

class CPpgHeatSizes : public CPropertyPage
{
	DECLARE_DYNCREATE(CPpgHeatSizes)

// Construction
public:
	CPpgHeatSizes();
	~CPpgHeatSizes();

// Dialog Data
	//{{AFX_DATA(CPpgHeatSizes)
	enum { IDD = IDD_PPG_HEAT_SIZES };
	int		m_caster1HeatSizeAim;
	int		m_caster1HeatSizeMax;
	int		m_caster1HeatSizeMin;
	int		m_caster23HeatSizeAim;
	int		m_caster23HeatSizeMax;
	int		m_caster23HeatSizeMin;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPpgHeatSizes)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


	public:

		void SetMiscConstantsObject(CMiscConstants* pConsts) { m_pConsts = pConsts; }



	private:

		CMiscConstants* m_pConsts;
			// the object being edited


// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPpgHeatSizes)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PPGHEATSIZES_H__642EE488_E2B3_11D0_8019_006097B38214__INCLUDED_)
