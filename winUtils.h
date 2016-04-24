#ifndef _winUtils_included_
#define _winUtils_included_

// A miscellany of useful windows macros.
// See WinUtils.cpp for more documentation.

namespace WinUtils {


	void InitializeListBoxFromStrings(CListBox& lbox,
									  vector<CString>& strings,
									  set<CString>& selStrings);

	void SetSelStringsFromListBox(CListBox& lbox,
								  vector<CString>& strings,
								  set<CString>& selStrings);

	void DeleteHeaderItems(CListCtrl* pCtrl);

	void GetSelectedPosns(CListCtrl* pCtrl,vector<int>& posns);
	void GetSelectedPosns(CListCtrl& rCtrl,vector<int>& posns);

	// Should have been done with templates,
	// but I had trouble getting the compiler to go for it.
	void ExtractContentsForReport(ostream& ostr, SECTreeCtrl& rCtrl,bool selectedOnly);
	void ExtractContentsForReport(ostream& ostr, SECTreeCtrl* pCtrl,bool selectedOnly);
	void ExtractContentsForReport(ostream& ostr, CListCtrl& rCtrl,bool selectedOnly);
	void ExtractContentsForReport(ostream& ostr, CListCtrl* pCtrl,bool selectedOnly);

	int GetSelectedCount(CListCtrl* pCtrl);
	int GetSelectedCount(CListCtrl& rCtrl);


}  // namespace WinUtils



#endif // _winUtils_included_