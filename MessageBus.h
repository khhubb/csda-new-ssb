// MessageBus.h: interface for the CMessageBus class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MESSAGEBUS_H__CDD277B3_DD17_11D3_862A_00104B2D39AC__INCLUDED_)
#define AFX_MESSAGEBUS_H__CDD277B3_DD17_11D3_862A_00104B2D39AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000




class CMessageBus : public MvcModel  
{
	////////////////////////////////////////////////////////////////
	//	
	//	An instance of this class is listened to by all editors.
	//  All editing operations, loading operations -- anything that
	//    might cause an editor display to be updated or some other
	//    ancillary operation to be performed -- must create a message
	//    and send it out on the message bus.
	//  This class follows the Singleton pattern (one instance created).
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

	static CMessageBus* m_pInstance;
		// holds the single instance


	////////////////////////////////////////////////////////////////
	//	
	//	Accessors
	//	
	////////////////////////////////////////////////////////////////

public:

	static CMessageBus* CMessageBus::Instance();


	////////////////////////////////////////////////////////////////
	//	
	//	C-tors, d-tor
	//	
	////////////////////////////////////////////////////////////////

private:

	// we want the c-tor to be private to enforce the singleton semantics

	CMessageBus() {}							
	~CMessageBus() {}							
	CMessageBus(const CMessageBus& x);			// do not implement
	CMessageBus& operator=(const CMessageBus& x);	// do not implement


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

	////////////////////////////////////////////////////////////////
	//	
	//	Implementation
	//	
	////////////////////////////////////////////////////////////////

private:
	
	// required to provide implementation of IQueryGuid::QueryGuid (stingray)
	BEGIN_GUID_MAP(CMessageBus)
	END_GUID_MAP
};


// the global accessor to the singleton
CMessageBus& TheBus();


// And here come 10,000 different message classes

enum E_MsgId { 

		// from CScenMgr
		MSG_ID_SMGR_CREATE_CASTER_SCEN,
		MSG_ID_SMGR_DELETE_CASTER_SCEN,
		MSG_ID_SMGR_SAVE_CASTER_SCEN,
		MSG_ID_SMGR_CASTER_SCEN_INFO_CHANGED,
		MSG_ID_SMGR_CREATE_PRODN_SCEN,
		MSG_ID_SMGR_DELETE_PRODN_SCEN,
		MSG_ID_SMGR_SAVE_PRODN_SCEN,
		MSG_ID_SMGR_PRODN_SCEN_INFO_CHANGED,
		MSG_ID_SMGR_CREATE_SUPER_SCEN,
		MSG_ID_SMGR_DELETE_SUPER_SCEN,
		MSG_ID_SMGR_SUPER_SCEN_INFO_CHANGED,
		MSG_ID_SMGR_CREATE_MASTER_SCEN,
		MSG_ID_SMGR_SNAPSHOT_LOADED,

		// from CScenMru
		MSG_ID_MRU_PROMOTE_CASTER_SCEN,
		MSG_ID_MRU_PROMOTE_PRODN_SCEN,
		MSG_ID_MRU_SELECT_CASTER,
		MSG_ID_MRU_SELECT_CAST_STRING,

		// from CScenMaster*
		MSG_ID_SMASTER_CHANGE_SEL_SCENS,
		
		// from HMLoadGraph
		MSG_ID_HM_SCEN_UPDATE,

		// from CCastStringEditor

		MSG_ID_CSE_ACTIVATED,
		MSG_ID_CSE_STRING_INFO_CHANGED,
		MSG_ID_CSE_ORDERS_DELETED,
		MSG_ID_CSE_ORDERS_INSERTED,
		MSG_ID_CSE_SCEN_SAVED,
		MSG_ID_CSE_LOCKED_HEATS_STATUS_CHANGED,
		MSG_ID_CSE_CAN_EDIT_STATUS_CHANGED,
		MSG_ID_CSE_ORDERS_MOVED,
		MSG_ID_CSE_ORDER_FOR_INSERT_CHANGED,
		MSG_ID_CSE_ORDER_CHANGED,
		MSG_ID_CSE_STRING_VALIDATED,

		// from CCasterScenInfoDlg
		MSG_ID_CSI_CASTER_SCEN_INFO_CHANGED

};



enum E_MsgSubId {

	MSG_SUBID_DUMMY
};

 


//  mixin template for all of our messages
//  instantiated with one of the values of E_MsgId
//  If you want to find all messages that travel through the bus,
//     just do a global search over the source for CCsdaMsg_Impl.

template<int MSG_ID>
class CCsdaMsg_Impl : public IUnknownImpl_T<IMvcMessage>
{
public:

	virtual unsigned int GetTypeID() const { return MSG_ID;	}

	// required to provide implementation of IQueryGuid::QueryGuid (stingray)
	BEGIN_GUID_MAP(CKiipMsg_Impl)
	END_GUID_MAP

};


 


#endif // !defined(AFX_MESSAGEBUS_H__CDD277B3_DD17_11D3_862A_00104B2D39AC__INCLUDED_)
