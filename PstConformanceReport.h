// PstConformanceReport.h: interface for the CPstConformanceReport class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PSTCONFORMANCEREPORT_H__2C913C03_96E2_11D4_863E_00104B2D39AC__INCLUDED_)
#define AFX_PSTCONFORMANCEREPORT_H__2C913C03_96E2_11D4_863E_00104B2D39AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCasterScen;
class CCastString;
class CCSOrder;
class CSuperScen;

#include "modelTypes.h"

class CPstConformanceReport  
{
	////////////////////////////////////////////////////////////////
	//	
	//	Creates a PST Conformance report as an HTML document
	//    and displays for printing.
	//	
	////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////
	//	
	//	Typedefs, enums, etc.
	//	
	////////////////////////////////////////////////////////////////

public:

	class CItem 
	{
	public:
		enum E_Category {
			CAT_PST_LT_8,
			CAT_PST_8_14,
			CAT_PST_GT_14,
			CAT_PST_15_21,
			CAT_PST_22_28,
			CAT_PST_29_35,
			CAT_PST_GT_35,
			CAT_PST_TOTAL
		};

		enum { NumCategories = CAT_PST_TOTAL+1 };

		int m_ciCode;
		Weight m_tonsPrioLe10[NumCategories];
		Weight m_tonsPrioGt10[NumCategories];
		Weight m_tonsTotal[NumCategories];
		CItem() : m_ciCode(-2) { Zero(); }
		CItem(int ciCode) : m_ciCode(ciCode) { Zero(); }
		void AddCSOrder(CCSOrder* pOrder);
		void ComputeCategories(vector<E_Category>& categories,CCSOrder* pCSOrder);
		void Zero();
		void AddItem(CItem* pItem);
		void AddItem(CItem& rItem) { AddItem(&rItem); }
		void WriteTotReport(ostream&                     ostr,
							const char* header,
							const char* tagLine);
		void WriteTotDetailLine(ostream& ostr,
								const char* tagLine,
								const char* descr,
								double value);
		void WriteDetailLine(ostream& ostr);
		static void WriteDashes(ostream& ostr);
		static void WriteHeader(ostream& ostr);
	};


	typedef map<int,CItem*> T_MapIntItem;


	class CStringHolder 
	{
	private:
		T_MapIntItem m_items;
		CCastString* m_pString;
		CItem m_totItem;

	public:
		T_MapIntItem& Items() 
		{ return m_items;	}

		CCastString* CastString()
		{ return m_pString;	}

		CItem& TotItem() 
		{ return m_totItem;	}

	public:
		CStringHolder(CCastString* pString) : m_pString(pString) {}
		~CStringHolder();

	public:
		void Compute();
		void WriteReport(ostream& ostr);

	private:
		void AddCSOrder(CCSOrder* m_pOrder);
		void ComputeTotals();
		void WriteHeader(ostream& ostr);
	};

	class CScenHolder
	{
	private:
		vector<CStringHolder*> m_stringHolders;
		CCasterScen* m_pScen;
		CItem m_totItem;
	public:
		CCasterScen* Scen() { return m_pScen; }
		CItem& TotItem() { return m_totItem; }
		vector<CStringHolder*>& StringHolders()
		{ return m_stringHolders;	}

	public:
		CScenHolder(CCasterScen* pScen) : m_pScen(pScen) {}
		~CScenHolder();

	public:
		void Compute();
		void Compute(vector<CCastString*>& castStrings);
		void WriteReport(ostream& ostr);

	private:
		void AddString(CCastString* pString);
		void ComputeTotals();
	};


	////////////////////////////////////////////////////////////////
	//	
	//	Data
	//	
	////////////////////////////////////////////////////////////////

private:

	CSuperScen* m_pSS;
	vector<CScenHolder*> m_scenHolders;
	CItem m_totItem;


	////////////////////////////////////////////////////////////////
	//	
	//	Accessors
	//	
	////////////////////////////////////////////////////////////////

public:


	CSuperScen* SuperScen()
	{ return m_pSS;	}

	vector<CScenHolder*>& ScenHolders()
	{ return m_scenHolders;	}

	CItem& TotItem()
	{ return m_totItem;	}


	////////////////////////////////////////////////////////////////
	//	
	//	C-tors, d-tor
	//	
	////////////////////////////////////////////////////////////////

public:
	CPstConformanceReport(CSuperScen* pSS) : m_pSS(pSS) {}
	~CPstConformanceReport();

	////////////////////////////////////////////////////////////////
	//	
	//	Operations
	//	
	////////////////////////////////////////////////////////////////

public:

	void Create();


	////////////////////////////////////////////////////////////////
	//	
	//	Implementation
	//	
	////////////////////////////////////////////////////////////////

private:

	void ComputeTotals();
	void CreateReport();
	void WriteReport(CString& filename);
	void DisplayReport(CString& filename);
	void DeleteFile(CString& filename);
};

#endif // !defined(AFX_PSTCONFORMANCEREPORT_H__2C913C03_96E2_11D4_863E_00104B2D39AC__INCLUDED_)
