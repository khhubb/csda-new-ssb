// Machine generated IDispatch wrapper class(es) created with ClassWizard
/////////////////////////////////////////////////////////////////////////////
// _Application wrapper class

class _Application : public COleDispatchDriver
{
public:
	_Application() {}		// Calls COleDispatchDriver default constructor
	_Application(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_Application(const _Application& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	LPDISPATCH GetAssistant();
	CString GetName();
	CString GetVersion();
	LPDISPATCH ActiveExplorer();
	LPDISPATCH ActiveInspector();
	LPDISPATCH CreateItem(long ItemType);
	LPDISPATCH CreateItemFromTemplate(LPCTSTR TemplatePath, const VARIANT& InFolder);
	LPDISPATCH CreateObject(LPCTSTR ObjectName);
	LPDISPATCH GetNamespace(LPCTSTR Type);
	void Quit();
};
/////////////////////////////////////////////////////////////////////////////
// Recipient wrapper class

class Recipient : public COleDispatchDriver
{
public:
	Recipient() {}		// Calls COleDispatchDriver default constructor
	Recipient(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Recipient(const Recipient& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	CString GetAddress();
	LPDISPATCH GetAddressEntry();
	void SetRefAddressEntry(LPDISPATCH newValue);
	CString GetAutoResponse();
	void SetAutoResponse(LPCTSTR lpszNewValue);
	long GetDisplayType();
	CString GetEntryID();
	long GetIndex();
	long GetMeetingResponseStatus();
	CString GetName();
	BOOL GetResolved();
	long GetTrackingStatus();
	void SetTrackingStatus(long nNewValue);
	DATE GetTrackingStatusTime();
	void SetTrackingStatusTime(DATE newValue);
	long GetType();
	void SetType(long nNewValue);
	void Delete();
	CString FreeBusy(DATE Start, long MinPerChar, const VARIANT& CompleteFormat);
	BOOL Resolve();
};
/////////////////////////////////////////////////////////////////////////////
// Attachments wrapper class

class Attachments : public COleDispatchDriver
{
public:
	Attachments() {}		// Calls COleDispatchDriver default constructor
	Attachments(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Attachments(const Attachments& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	long GetCount();
	LPDISPATCH Item(const VARIANT& Index);
	LPDISPATCH Add(const VARIANT& Source, const VARIANT& Type, const VARIANT& Position, const VARIANT& DisplayName);
	void Remove(long Index);
};
/////////////////////////////////////////////////////////////////////////////
// Attachment wrapper class

class Attachment : public COleDispatchDriver
{
public:
	Attachment() {}		// Calls COleDispatchDriver default constructor
	Attachment(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Attachment(const Attachment& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	CString GetDisplayName();
	void SetDisplayName(LPCTSTR lpszNewValue);
	CString GetFileName();
	long GetIndex();
	CString GetPathName();
	long GetPosition();
	void SetPosition(long nNewValue);
	long GetType();
	void Delete();
	void SaveAsFile(LPCTSTR Path);
};
/////////////////////////////////////////////////////////////////////////////
// Recipients wrapper class

class Recipients : public COleDispatchDriver
{
public:
	Recipients() {}		// Calls COleDispatchDriver default constructor
	Recipients(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Recipients(const Recipients& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	long GetCount();
	LPDISPATCH Item(const VARIANT& Index);
	LPDISPATCH Add(LPCTSTR Name);
	void Remove(long Index);
	BOOL ResolveAll();
};
/////////////////////////////////////////////////////////////////////////////
// _MailItem wrapper class

class _MailItem : public COleDispatchDriver
{
public:
	_MailItem() {}		// Calls COleDispatchDriver default constructor
	_MailItem(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_MailItem(const _MailItem& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	LPDISPATCH GetActions();
	LPDISPATCH GetAttachments();
	CString GetBillingInformation();
	void SetBillingInformation(LPCTSTR lpszNewValue);
	CString GetBody();
	void SetBody(LPCTSTR lpszNewValue);
	CString GetCategories();
	void SetCategories(LPCTSTR lpszNewValue);
	CString GetCompanies();
	void SetCompanies(LPCTSTR lpszNewValue);
	CString GetConversationIndex();
	CString GetConversationTopic();
	DATE GetCreationTime();
	CString GetEntryID();
	LPDISPATCH GetFormDescription();
	LPDISPATCH GetGetInspector();
	long GetImportance();
	void SetImportance(long nNewValue);
	DATE GetLastModificationTime();
	CString GetMessageClass();
	void SetMessageClass(LPCTSTR lpszNewValue);
	CString GetMileage();
	void SetMileage(LPCTSTR lpszNewValue);
	BOOL GetNoAging();
	void SetNoAging(BOOL bNewValue);
	long GetOutlookInternalVersion();
	CString GetOutlookVersion();
	BOOL GetSaved();
	long GetSensitivity();
	void SetSensitivity(long nNewValue);
	long GetSize();
	CString GetSubject();
	void SetSubject(LPCTSTR lpszNewValue);
	BOOL GetUnRead();
	void SetUnRead(BOOL bNewValue);
	LPDISPATCH GetUserProperties();
	void Close(long SaveMode);
	LPDISPATCH Copy();
	void Delete();
	void Display(const VARIANT& Modal);
	LPDISPATCH Move(LPDISPATCH DestFldr);
	void PrintOut();
	void Save();
	void SaveAs(LPCTSTR Path, const VARIANT& Type);
	BOOL GetAlternateRecipientAllowed();
	void SetAlternateRecipientAllowed(BOOL bNewValue);
	BOOL GetAutoForwarded();
	void SetAutoForwarded(BOOL bNewValue);
	CString GetBcc();
	void SetBcc(LPCTSTR lpszNewValue);
	CString GetCc();
	void SetCc(LPCTSTR lpszNewValue);
	DATE GetDeferredDeliveryTime();
	void SetDeferredDeliveryTime(DATE newValue);
	BOOL GetDeleteAfterSubmit();
	void SetDeleteAfterSubmit(BOOL bNewValue);
	DATE GetExpiryTime();
	void SetExpiryTime(DATE newValue);
	DATE GetFlagDueBy();
	void SetFlagDueBy(DATE newValue);
	CString GetFlagRequest();
	void SetFlagRequest(LPCTSTR lpszNewValue);
	long GetFlagStatus();
	void SetFlagStatus(long nNewValue);
	CString GetHTMLBody();
	void SetHTMLBody(LPCTSTR lpszNewValue);
	BOOL GetOriginatorDeliveryReportRequested();
	void SetOriginatorDeliveryReportRequested(BOOL bNewValue);
	BOOL GetReadReceiptRequested();
	void SetReadReceiptRequested(BOOL bNewValue);
	CString GetReceivedByEntryID();
	CString GetReceivedByName();
	CString GetReceivedOnBehalfOfEntryID();
	CString GetReceivedOnBehalfOfName();
	DATE GetReceivedTime();
	BOOL GetRecipientReassignmentProhibited();
	void SetRecipientReassignmentProhibited(BOOL bNewValue);
	LPDISPATCH GetRecipients();
	BOOL GetReminderOverrideDefault();
	void SetReminderOverrideDefault(BOOL bNewValue);
	BOOL GetReminderPlaySound();
	void SetReminderPlaySound(BOOL bNewValue);
	BOOL GetReminderSet();
	void SetReminderSet(BOOL bNewValue);
	CString GetReminderSoundFile();
	void SetReminderSoundFile(LPCTSTR lpszNewValue);
	DATE GetReminderTime();
	void SetReminderTime(DATE newValue);
	long GetRemoteStatus();
	void SetRemoteStatus(long nNewValue);
	CString GetReplyRecipientNames();
	LPDISPATCH GetReplyRecipients();
	LPDISPATCH GetSaveSentMessageFolder();
	void SetRefSaveSentMessageFolder(LPDISPATCH newValue);
	CString GetSenderName();
	BOOL GetSent();
	DATE GetSentOn();
	CString GetSentOnBehalfOfName();
	void SetSentOnBehalfOfName(LPCTSTR lpszNewValue);
	BOOL GetSubmitted();
	CString GetTo();
	void SetTo(LPCTSTR lpszNewValue);
	CString GetVotingOptions();
	void SetVotingOptions(LPCTSTR lpszNewValue);
	CString GetVotingResponse();
	void SetVotingResponse(LPCTSTR lpszNewValue);
	void ClearConversationIndex();
	LPDISPATCH Forward();
	LPDISPATCH Reply();
	LPDISPATCH ReplyAll();
	void Send();
};
