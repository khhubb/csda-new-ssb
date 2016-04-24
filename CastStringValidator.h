// CastStringValidator.h: interface for the CCastStringValidator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CASTSTRINGVALIDATOR_H__5D1F3423_B04E_11D1_8068_006097B38214__INCLUDED_)
#define AFX_CASTSTRINGVALIDATOR_H__5D1F3423_B04E_11D1_8068_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "CastStringHeatValidnError.h"
#include "CastStringHeat.h"

class CCastString;
class CCasterScen;
class CSuperScen;
class CCSOrder;


class CCastStringValidator  
{
	////////////////////////////////////////////////////////////////
	//	
	//	Performs validation checks on a cast string.
	//	
	////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////
	//	
	//	Typedefs, enums, etc.
	//	
	////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////
	//	
	//	Data
	//	
	////////////////////////////////////////////////////////////////

private:

	vector<CCastStringHeat>* m_pHeats;
		// the heats in the string begin validated

	CCastString* m_pCastString;
		// the string being validated

	CCasterScen* m_pCasterScen;
		// the caster scen the string belongs to

	CSuperScen* m_pSuperScen;
		// the superscenario providing context


	////////////////////////////////////////////////////////////////
	//	
	//	Accessors
	//	
	////////////////////////////////////////////////////////////////

public:

	const vector<CCastStringHeatValidnError>& ValidnErrors() const;
		// we decided to cach the errors on the string


	////////////////////////////////////////////////////////////////
	//	
	//	C-tors, d-tor
	//	
	////////////////////////////////////////////////////////////////

public:

	CCastStringValidator();
	virtual ~CCastStringValidator() {};


	////////////////////////////////////////////////////////////////
	//	
	//	Operators
	//	
	////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////
	//	
	//	Operations
	//	
	////////////////////////////////////////////////////////////////

public:

	bool Validate(vector<CCastStringHeat>& heats,
				  CCastString* pCastString,
				  CCasterScen* pCasterScen,
				  CSuperScen* pSuperScen);

	static bool FixHeatSpec(CString& heatSpec,
			 				const CString& orderedSpec,
							int caster);

	////////////////////////////////////////////////////////////////
	//	
	//	Implementation
	//	
	////////////////////////////////////////////////////////////////

private:

	void AddValidnError(int heatIndex,
					    int strandNum,
						int lotIndex,
						const char* str,
						CCastStringHeatValidnError::E_Severity severity);
	void AddValidnError(int heatIndex,
					    int strandNum,
						int lotIndex,
						ostrstream& ostr,
						CCastStringHeatValidnError::E_Severity severity);


	void UpdateLotSlabThicknesses();
	void UpdateLotSlabThicknesses(int strandNum);


	void UpdateCondAndDispCodes();
	void UpdateCondAndDispCodes(int strandNum);
	void UpdateLotDispCode(CCSOrder* pCSOrder, CCastStringHeat& rHeat );
	void UpdateCondnCode( CCSOrder* pOrder, CCastStringHeat& rHeat, int& outCondn, int& outDisp  );
	void UpdateHeatDispCode( CCastStringHeat& rHeat );
	
	bool CheckCondAndDispCodeRelationships() ;
	bool CheckCondAndDispCodeRelationships(int strandNum);


	bool ValidateTabNums();
	bool ValidateWeights();
	bool ValidateDemandDue();
	bool ValidateDemandDue(int strandNum);
	bool ValidateSpecs();
	bool ValidateChems();
	bool ValidateHeatChems( int heatIndex, CCastStringHeat& rHeat );
	bool ValidateLotSpecs( CCastStringHeat& rHeat, int strandNum );
	bool ValidateLotChems( int heatIndex, CCastStringHeat& rHeat, int strandNum );
	

	bool Validate340080() ;
	bool Validate340080(int strandNum) ;
	bool Validate340080Weights();
	bool AuditSlabLengthData(CCSOrder* pOrder);
	void ComputeSlabLength(int caster,
						   CCSOrder* pOrder, 
						   bool isLenOk, 
						   Length& minL, 
						   Length& maxL);

	bool FixHeatSpec( CCastStringHeat& rHeat, int caster );



};

#endif // !defined(AFX_CASTSTRINGVALIDATOR_H__5D1F3423_B04E_11D1_8068_006097B38214__INCLUDED_)
