// CICode.h: interface for the classes
//
//		CCICodeGroup, 
//      CCICodeGroupMgr
//      CCICodeGroupException
//      CCICodeBogie
//		CCICodeBogieMgr
//
//////////////////////////////////////////////////////////////////////
 
#if !defined(AFX_CICODE_H__F60F11BA_A60D_11D0_975C_0000C0C2484C__INCLUDED_)
#define AFX_CICODE_H__F60F11BA_A60D_11D0_975C_0000C0C2484C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CCICodeGroupDefSet;
class CCICodeGroupXrefSet;

class CCIBogieSet;   // added 5-4-10 k. hubbard

class CCICodeGroup 
{
	////////////////////////////////////////////////////////////////
	//	
	//	Represents a set of CI codes that are to be grouped together
	//		on the bogie report.
	//	
	////////////////////////////////////////////////////////////////

	//  This class is responsible for creating, etc.

	friend class CCICodeGroupMgr;



	////////////////////////////////////////////////////////////////
	//	
	//	Data members
	//	
	////////////////////////////////////////////////////////////////

public:         
//private:		commented out 5-4-10 k. hubbard			
	int			m_groupId;
	CString		m_name;
	vector<int>	m_codes;
	int			m_sortOrder;
	long m_Bogie;         // added 10 bogie fields here 5-4-10 k. hubbard  
	long m_Rolled;
//	struct BogieNew
//	{ 	long m_Bogie;         // added 10 bogie fields here 5-4-10 k. hubbard  
//		long m_Rolled;
		long m_OnLineup;
	    long m_SlabbedAvail;
		long m_Cms;
        long m_Shaves;
		long m_OtherUnavail;
		long m_Tons910;
		long m_Tons909;
		long m_TonsRemPlan;
//	} BogieData;
	////////////////////////////////////////////////////////////////
	//	
	//	Accessors
	//	
	////////////////////////////////////////////////////////////////

public:

	int GroupId() const
	{	return m_groupId;	}

	const vector<int>& Codes() const
	{	return m_codes; }

	int SortOrder() const
	{	return m_sortOrder; }

	const CString& Name() const
	{	return m_name; }

//	long Bogie() const			// added 5-4-10 k. hubbard
//	{	return m_Bogie;	}

//	long Rolled() const			// added 5-4-10 k. hubbard
//	{	return m_Rolled;	}


	////////////////////////////////////////////////////////////////
	//	
	//	C-tors, d-tor, etc
	//	
	////////////////////////////////////////////////////////////////

private:

	// c-tor is private to force using CCICoderoupMgr::CreateGroup.   This is group's definition point

	CCICodeGroup(int,CString,int,long,long,long,long,long,long,long,long,long,long); // added 10 long type changes 5-4-10 k. hubbard 


	// do not implement
	CCICodeGroup(const CCICodeGroup&);
	CCICodeGroup& operator=(const CCICodeGroup&);

	//	default d-tor okay


	////////////////////////////////////////////////////////////////
	//	
	//	Operators
	//	
	////////////////////////////////////////////////////////////////


public:

	bool operator<(const CCICodeGroup& x) const
	{
		return m_sortOrder < x.m_sortOrder;
	}

};





class CCICodeGroupMgr
{

	
	////////////////////////////////////////////////////////////////
	//	
	//	Manages CCICodeGroup objects
	//	
	////////////////////////////////////////////////////////////////



	////////////////////////////////////////////////////////////////
	//	
	//	Typedefs, enums, etc.
	//	
	////////////////////////////////////////////////////////////////

public:

	typedef vector<CCICodeGroup*> T_GroupVec;
	typedef map<int,CCICodeGroup*> T_GroupMap;

	enum { DefaultNumCodes=100 };


	
	////////////////////////////////////////////////////////////////
	//	
	//	Data members
	//	
	////////////////////////////////////////////////////////////////

private:

	int			m_size;
	T_GroupVec	m_ciCodeToGroup;
	T_GroupVec	m_groups;
	T_GroupMap	m_groupMap;


	////////////////////////////////////////////////////////////////
	//	
	//	Accessors
	//	
	////////////////////////////////////////////////////////////////

public:

	CCICodeGroup& GetGroup(int ciCode);
 	CCICodeGroup* GetGroupMaybe(int ciCode);
	CCICodeGroup& GetGroupById(int groupId);
	CCICodeGroup* GetGroupByIdMaybe(int groupId);
	
	T_GroupVec::const_iterator GroupBegin()
	{
		return m_groups.begin();
	}
	
	T_GroupVec::const_iterator GroupEnd()
	{
		return m_groups.end();
	}


	
	////////////////////////////////////////////////////////////////
	//	
	//	C-tors, d-tor, etc.
	//	
	////////////////////////////////////////////////////////////////

public:

	CCICodeGroupMgr(int size_ = DefaultNumCodes );
	~CCICodeGroupMgr();


	// Do not implement
	CCICodeGroupMgr(const CCICodeGroupMgr&);
	CCICodeGroupMgr& operator=(const CCICodeGroupMgr&);


	////////////////////////////////////////////////////////////////
	//	
	//	Operations
	//	
	////////////////////////////////////////////////////////////////

public:
	
	bool IsCICodeInRange(int ciCode) const
	{
		return 0 <= ciCode && ciCode < m_size ;
	}

	bool IsCICodeDefined(int ciCode) const
	{
		return IsCICodeInRange(ciCode) && m_ciCodeToGroup[ciCode] != 0;
	}

	bool CreateGroups(CDatabase* pDB);   // test comm out 5-3-10 k. hubbard

	
	////////////////////////////////////////////////////////////////
	//	
	//	Implementation
	//	
	////////////////////////////////////////////////////////////////


private:

	void AddCodeToGroup(int newCICode, int groupId);

	void SortGroups();

	void CreateGroups(CCICodeGroupDefSet& rSet);
	void PopulateGroups(CCICodeGroupXrefSet& rSet);

    void Initialize80HSMBogies(CCIBogieSet& rSet);       /// added k. hubbard 5-4-10

	void CreateGroup(int groupId, 
					 CString& name, 
					 int sortOrder,
					 long myBogie,     // added 10 bogie fields here 5-4-10 k. hubbard
					 long myRolled,
					 long myOnLineup,
					 long mySlabbedAvail,
					 long myCms,
					 long myShaves,
					 long myOtherUnavail,
					 long myTons910,
					 long myTons909,
					 long myTonsRemPlan,
					 bool doSort = TRUE);             

	void VerifyCICodeInRange(int ciCode);
	void VerifyCICodeDefined(int ciCode);
	void VerifyCICodeUndefined(int ciCode);

	void Cleanup();
	

};


class CCICodeGroupException 
{

	
	////////////////////////////////////////////////////////////////
	//	
	//	For throwing exceptions while reading CICode groups 
	//		from the database.
	//	
	////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////
	//	
	//	Typedefs, enums, etc
	//	
	////////////////////////////////////////////////////////////////

public:

	enum E_ErrorType { MISSING_GROUP,
					 BAD_INDEX,
					 ALREADY_ASSIGNED };


	////////////////////////////////////////////////////////////////
	//	
	//	Data members
	//	
	////////////////////////////////////////////////////////////////

private:	
	E_ErrorType	m_type;
	int			m_index;


	////////////////////////////////////////////////////////////////
	//	
	//	Accessors
	//	
	////////////////////////////////////////////////////////////////

public:

	E_ErrorType Type() { return m_type; };
	int Index() { return m_index; };



	////////////////////////////////////////////////////////////////
	//	
	//	C-tors, d-tor
	//	
	////////////////////////////////////////////////////////////////


public:

	CCICodeGroupException(E_ErrorType type_, int index_) 
		: m_type(type_), m_index(index_) 
	{};


	// supplied d-tor, copy c-tor okay
};



class CCICodeBogieMgr;
//class CCIBogieSet;   // added 5-4-10 k. hubbard


class CCICodeBogie
{
	
	////////////////////////////////////////////////////////////////
	//	
	//	Represents a row from the bogie spreadsheet
	//	
	////////////////////////////////////////////////////////////////

	//  This class manages creation, etc.
	friend class CCICodeBogieMgr;
//public:
//	CCIBogieSet*  m_pCIBogieSet;            // 5-4-10 k. hubbard

//friend class CCIBogieSet;   // added 5-4-10 k. hubbard

	////////////////////////////////////////////////////////////////
	//	
	//	Data members, private
	//	
	////////////////////////////////////////////////////////////////

private:

	const CCICodeGroup&		m_rCI;
	const CCICodeGroupMgr&	m_rCIMgr;
	const CCICodeBogieMgr&	m_rMgr;


	////////////////////////////////////////////////////////////////
	//	
	//	Data members, public
	//	
	////////////////////////////////////////////////////////////////

public:

	long m_bogie;
	long m_rolled;
	long m_onLu;
	long m_slabbed;
	long m_cms;
	long m_shaves;
	long m_otherUnavail;
	long m_on910;
	long m_on909;

	// computed

	long m_remainingToSchedule;
	long m_amountOverOnLu;
	long m_remainingAfter910;
	long m_remainingAfter909;
	long m_amountOverBogie;


	////////////////////////////////////////////////////////////////
	//	
	//	Accessors
	//	
	////////////////////////////////////////////////////////////////

	const CCICodeGroup& GetCICodeGroup()
	{
		return m_rCI;
	}

	
	////////////////////////////////////////////////////////////////
	//	
	//	C-tors, d-tor
	//	
	////////////////////////////////////////////////////////////////

private:

	// c-tor is private so only CCICodeBogieMgr can create.
	CCICodeBogie(const CCICodeBogieMgr& rMgr_,
				 const CCICodeGroup& rCI_,
				 const CCICodeGroupMgr& rCIMgr_)
				 : m_rMgr(rMgr_),
				   m_rCI(rCI_),
				   m_rCIMgr(rCIMgr_)
	{
		Initialize();
	}


	// not implemented
	CCICodeBogie(const CCICodeBogie&);
	CCICodeBogie& operator=(const CCICodeBogie&) const;

	// default d-tor okay


	////////////////////////////////////////////////////////////////
	//	
	//	Operations
	//	
	////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////
	//	
	//	Operators
	//	
	////////////////////////////////////////////////////////////////

public:

	bool operator<(const CCICodeBogie& x) const
	{
		return m_rCI < x.m_rCI;
	}



	////////////////////////////////////////////////////////////////
	//	
	//	Implementation
	//	
	////////////////////////////////////////////////////////////////
//private:
public:

	void Initialize();  

	void ComputeDependentVars();
};

 

class CCICodeBogieMgr
{


	////////////////////////////////////////////////////////////////
	//	
	//	Manages a collection of bogies
	//	
	////////////////////////////////////////////////////////////////

//friend class CCIBogieSet;   // added 5-4-10 k. hubbard
	
	////////////////////////////////////////////////////////////////
	//	
	//	Typedefs, enums, etc.
	//	
	////////////////////////////////////////////////////////////////

public:

	typedef vector<CCICodeBogie*> T_BogiesVec;


	
	////////////////////////////////////////////////////////////////
	//	
	//	Data members
	//	
	////////////////////////////////////////////////////////////////

private:

	T_BogiesVec m_bogies;

	CCICodeGroupMgr& m_rGrpMgr;	

	
	////////////////////////////////////////////////////////////////
	//	
	//	Accessors
	//	
	////////////////////////////////////////////////////////////////

public:
	
	T_BogiesVec::const_iterator BogiesBegin()
	{
		return m_bogies.begin();
	}
	
	T_BogiesVec::const_iterator BogiesEnd()
	{
		return m_bogies.end();
	}

	CCICodeBogie* BogiesAt(int index);
	
	

	////////////////////////////////////////////////////////////////
	//	
	//	C-tors, d-tor
	//	
	////////////////////////////////////////////////////////////////
	
public:

	CCICodeBogieMgr(CCICodeGroupMgr& rGrpMgr_);
	~CCICodeBogieMgr();

	// Do not implement
	CCICodeBogieMgr(const CCICodeBogieMgr&);
	CCICodeBogieMgr& operator=(const CCICodeBogieMgr&);


	////////////////////////////////////////////////////////////////
	//	
	//	Operations
	//	
	////////////////////////////////////////////////////////////////

public:
//	bool Initialize80HSMBogies(CDatabase* pDB);  // added 5-4-10 k. hubbard

	bool InitializeBogies(CDatabase* pDB);

private:
	void InitializeBogies(CCIBogieSet& rSet);   // added 5-4-10 k. hubbard


	////////////////////////////////////////////////////////////////
	//	
	//	Implementation
	//	
	////////////////////////////////////////////////////////////////

private:

	void SortBogies();

	void Cleanup();
};
	






#endif // !defined(AFX_CICODE_H__F60F11BA_A60D_11D0_975C_0000C0C2484C__INCLUDED_)
