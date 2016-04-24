// Machine generated IDispatch wrapper class(es) created with ClassWizard

#include "stdafx.h"
#include "msoutl85.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// _Application properties

/////////////////////////////////////////////////////////////////////////////
// _Application operations

LPDISPATCH _Application::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Application::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Application::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Application::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Application::GetAssistant()
{
	LPDISPATCH result;
	InvokeHelper(0x114, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _Application::GetName()
{
	CString result;
	InvokeHelper(0x3001, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _Application::GetVersion()
{
	CString result;
	InvokeHelper(0x116, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Application::ActiveExplorer()
{
	LPDISPATCH result;
	InvokeHelper(0x111, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Application::ActiveInspector()
{
	LPDISPATCH result;
	InvokeHelper(0x112, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Application::CreateItem(long ItemType)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x10a, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		ItemType);
	return result;
}

LPDISPATCH _Application::CreateItemFromTemplate(LPCTSTR TemplatePath, const VARIANT& InFolder)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR VTS_VARIANT;
	InvokeHelper(0x10b, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		TemplatePath, &InFolder);
	return result;
}

LPDISPATCH _Application::CreateObject(LPCTSTR ObjectName)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x115, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		ObjectName);
	return result;
}

LPDISPATCH _Application::GetNamespace(LPCTSTR Type)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x110, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Type);
	return result;
}

void _Application::Quit()
{
	InvokeHelper(0x113, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// Recipient properties

/////////////////////////////////////////////////////////////////////////////
// Recipient operations

LPDISPATCH Recipient::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long Recipient::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH Recipient::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Recipient::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0x6d, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString Recipient::GetAddress()
{
	CString result;
	InvokeHelper(0x3003, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

LPDISPATCH Recipient::GetAddressEntry()
{
	LPDISPATCH result;
	InvokeHelper(0x79, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void Recipient::SetRefAddressEntry(LPDISPATCH newValue)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x79, DISPATCH_PROPERTYPUTREF, VT_EMPTY, NULL, parms,
		 newValue);
}

CString Recipient::GetAutoResponse()
{
	CString result;
	InvokeHelper(0x6a, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void Recipient::SetAutoResponse(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x6a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

long Recipient::GetDisplayType()
{
	long result;
	InvokeHelper(0x3900, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString Recipient::GetEntryID()
{
	CString result;
	InvokeHelper(0xf01e, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long Recipient::GetIndex()
{
	long result;
	InvokeHelper(0x5b, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long Recipient::GetMeetingResponseStatus()
{
	long result;
	InvokeHelper(0x66, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString Recipient::GetName()
{
	CString result;
	InvokeHelper(0x3001, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

BOOL Recipient::GetResolved()
{
	BOOL result;
	InvokeHelper(0x64, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

long Recipient::GetTrackingStatus()
{
	long result;
	InvokeHelper(0x76, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void Recipient::SetTrackingStatus(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x76, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

DATE Recipient::GetTrackingStatusTime()
{
	DATE result;
	InvokeHelper(0x77, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void Recipient::SetTrackingStatusTime(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x77, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

long Recipient::GetType()
{
	long result;
	InvokeHelper(0xc15, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void Recipient::SetType(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xc15, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

void Recipient::Delete()
{
	InvokeHelper(0x6e, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

CString Recipient::FreeBusy(DATE Start, long MinPerChar, const VARIANT& CompleteFormat)
{
	CString result;
	static BYTE parms[] =
		VTS_DATE VTS_I4 VTS_VARIANT;
	InvokeHelper(0x6f, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		Start, MinPerChar, &CompleteFormat);
	return result;
}

BOOL Recipient::Resolve()
{
	BOOL result;
	InvokeHelper(0x71, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// Attachments properties

/////////////////////////////////////////////////////////////////////////////
// Attachments operations

LPDISPATCH Attachments::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long Attachments::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH Attachments::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Attachments::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long Attachments::GetCount()
{
	long result;
	InvokeHelper(0x50, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH Attachments::Item(const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x51, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Index);
	return result;
}

LPDISPATCH Attachments::Add(const VARIANT& Source, const VARIANT& Type, const VARIANT& Position, const VARIANT& DisplayName)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x65, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Source, &Type, &Position, &DisplayName);
	return result;
}

void Attachments::Remove(long Index)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x54, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Index);
}


/////////////////////////////////////////////////////////////////////////////
// Attachment properties

/////////////////////////////////////////////////////////////////////////////
// Attachment operations

LPDISPATCH Attachment::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long Attachment::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH Attachment::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Attachment::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0x71, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString Attachment::GetDisplayName()
{
	CString result;
	InvokeHelper(0x3001, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void Attachment::SetDisplayName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3001, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString Attachment::GetFileName()
{
	CString result;
	InvokeHelper(0x3704, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long Attachment::GetIndex()
{
	long result;
	InvokeHelper(0x5b, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString Attachment::GetPathName()
{
	CString result;
	InvokeHelper(0x3708, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long Attachment::GetPosition()
{
	long result;
	InvokeHelper(0x72, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void Attachment::SetPosition(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x72, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long Attachment::GetType()
{
	long result;
	InvokeHelper(0x3705, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void Attachment::Delete()
{
	InvokeHelper(0x69, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void Attachment::SaveAsFile(LPCTSTR Path)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x68, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Path);
}


/////////////////////////////////////////////////////////////////////////////
// Recipients properties

/////////////////////////////////////////////////////////////////////////////
// Recipients operations

LPDISPATCH Recipients::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long Recipients::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH Recipients::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Recipients::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long Recipients::GetCount()
{
	long result;
	InvokeHelper(0x50, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH Recipients::Item(const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x51, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Index);
	return result;
}

LPDISPATCH Recipients::Add(LPCTSTR Name)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x6f, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Name);
	return result;
}

void Recipients::Remove(long Index)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x54, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Index);
}

BOOL Recipients::ResolveAll()
{
	BOOL result;
	InvokeHelper(0x7e, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// _MailItem properties

/////////////////////////////////////////////////////////////////////////////
// _MailItem operations

LPDISPATCH _MailItem::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _MailItem::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _MailItem::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _MailItem::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _MailItem::GetActions()
{
	LPDISPATCH result;
	InvokeHelper(0xf817, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _MailItem::GetAttachments()
{
	LPDISPATCH result;
	InvokeHelper(0xf815, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _MailItem::GetBillingInformation()
{
	CString result;
	InvokeHelper(0x8535, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _MailItem::SetBillingInformation(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8535, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _MailItem::GetBody()
{
	CString result;
	InvokeHelper(0x9100, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _MailItem::SetBody(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x9100, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _MailItem::GetCategories()
{
	CString result;
	InvokeHelper(0x9001, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _MailItem::SetCategories(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x9001, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _MailItem::GetCompanies()
{
	CString result;
	InvokeHelper(0x853b, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _MailItem::SetCompanies(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x853b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _MailItem::GetConversationIndex()
{
	CString result;
	InvokeHelper(0x71, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _MailItem::GetConversationTopic()
{
	CString result;
	InvokeHelper(0x70, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

DATE _MailItem::GetCreationTime()
{
	DATE result;
	InvokeHelper(0x3007, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

CString _MailItem::GetEntryID()
{
	CString result;
	InvokeHelper(0xf01e, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

LPDISPATCH _MailItem::GetFormDescription()
{
	LPDISPATCH result;
	InvokeHelper(0xf095, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _MailItem::GetGetInspector()
{
	LPDISPATCH result;
	InvokeHelper(0xf03e, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _MailItem::GetImportance()
{
	long result;
	InvokeHelper(0x17, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _MailItem::SetImportance(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x17, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

DATE _MailItem::GetLastModificationTime()
{
	DATE result;
	InvokeHelper(0x3008, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

CString _MailItem::GetMessageClass()
{
	CString result;
	InvokeHelper(0x1a, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _MailItem::SetMessageClass(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x1a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _MailItem::GetMileage()
{
	CString result;
	InvokeHelper(0x8534, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _MailItem::SetMileage(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8534, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL _MailItem::GetNoAging()
{
	BOOL result;
	InvokeHelper(0x850e, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _MailItem::SetNoAging(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x850e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _MailItem::GetOutlookInternalVersion()
{
	long result;
	InvokeHelper(0x8552, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString _MailItem::GetOutlookVersion()
{
	CString result;
	InvokeHelper(0x8554, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

BOOL _MailItem::GetSaved()
{
	BOOL result;
	InvokeHelper(0xf0a3, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

long _MailItem::GetSensitivity()
{
	long result;
	InvokeHelper(0x36, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _MailItem::SetSensitivity(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x36, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _MailItem::GetSize()
{
	long result;
	InvokeHelper(0xe08, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString _MailItem::GetSubject()
{
	CString result;
	InvokeHelper(0x37, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _MailItem::SetSubject(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x37, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL _MailItem::GetUnRead()
{
	BOOL result;
	InvokeHelper(0xf01c, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _MailItem::SetUnRead(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xf01c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

LPDISPATCH _MailItem::GetUserProperties()
{
	LPDISPATCH result;
	InvokeHelper(0xf816, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _MailItem::Close(long SaveMode)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xf023, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 SaveMode);
}

LPDISPATCH _MailItem::Copy()
{
	LPDISPATCH result;
	InvokeHelper(0xf032, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _MailItem::Delete()
{
	InvokeHelper(0xf04a, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _MailItem::Display(const VARIANT& Modal)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0xf0a6, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Modal);
}

LPDISPATCH _MailItem::Move(LPDISPATCH DestFldr)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xf034, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		DestFldr);
	return result;
}

void _MailItem::PrintOut()
{
	InvokeHelper(0xf033, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _MailItem::Save()
{
	InvokeHelper(0xf048, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _MailItem::SaveAs(LPCTSTR Path, const VARIANT& Type)
{
	static BYTE parms[] =
		VTS_BSTR VTS_VARIANT;
	InvokeHelper(0xf051, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Path, &Type);
}

BOOL _MailItem::GetAlternateRecipientAllowed()
{
	BOOL result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _MailItem::SetAlternateRecipientAllowed(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _MailItem::GetAutoForwarded()
{
	BOOL result;
	InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _MailItem::SetAutoForwarded(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

CString _MailItem::GetBcc()
{
	CString result;
	InvokeHelper(0xe02, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _MailItem::SetBcc(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xe02, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _MailItem::GetCc()
{
	CString result;
	InvokeHelper(0xe03, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _MailItem::SetCc(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xe03, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

DATE _MailItem::GetDeferredDeliveryTime()
{
	DATE result;
	InvokeHelper(0xf, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _MailItem::SetDeferredDeliveryTime(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0xf, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

BOOL _MailItem::GetDeleteAfterSubmit()
{
	BOOL result;
	InvokeHelper(0xe01, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _MailItem::SetDeleteAfterSubmit(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xe01, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

DATE _MailItem::GetExpiryTime()
{
	DATE result;
	InvokeHelper(0x15, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _MailItem::SetExpiryTime(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x15, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

DATE _MailItem::GetFlagDueBy()
{
	DATE result;
	InvokeHelper(0x30, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _MailItem::SetFlagDueBy(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x30, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

CString _MailItem::GetFlagRequest()
{
	CString result;
	InvokeHelper(0x8530, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _MailItem::SetFlagRequest(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8530, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

long _MailItem::GetFlagStatus()
{
	long result;
	InvokeHelper(0x1090, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _MailItem::SetFlagStatus(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1090, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString _MailItem::GetHTMLBody()
{
	CString result;
	InvokeHelper(0xf404, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _MailItem::SetHTMLBody(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xf404, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL _MailItem::GetOriginatorDeliveryReportRequested()
{
	BOOL result;
	InvokeHelper(0x23, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _MailItem::SetOriginatorDeliveryReportRequested(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x23, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _MailItem::GetReadReceiptRequested()
{
	BOOL result;
	InvokeHelper(0x29, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _MailItem::SetReadReceiptRequested(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x29, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

CString _MailItem::GetReceivedByEntryID()
{
	CString result;
	InvokeHelper(0x3f, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _MailItem::GetReceivedByName()
{
	CString result;
	InvokeHelper(0x40, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _MailItem::GetReceivedOnBehalfOfEntryID()
{
	CString result;
	InvokeHelper(0x43, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _MailItem::GetReceivedOnBehalfOfName()
{
	CString result;
	InvokeHelper(0x44, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

DATE _MailItem::GetReceivedTime()
{
	DATE result;
	InvokeHelper(0xe06, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

BOOL _MailItem::GetRecipientReassignmentProhibited()
{
	BOOL result;
	InvokeHelper(0x2b, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _MailItem::SetRecipientReassignmentProhibited(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x2b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

LPDISPATCH _MailItem::GetRecipients()
{
	LPDISPATCH result;
	InvokeHelper(0xf814, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL _MailItem::GetReminderOverrideDefault()
{
	BOOL result;
	InvokeHelper(0x851c, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _MailItem::SetReminderOverrideDefault(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x851c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _MailItem::GetReminderPlaySound()
{
	BOOL result;
	InvokeHelper(0x851e, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _MailItem::SetReminderPlaySound(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x851e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _MailItem::GetReminderSet()
{
	BOOL result;
	InvokeHelper(0x8503, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _MailItem::SetReminderSet(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x8503, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

CString _MailItem::GetReminderSoundFile()
{
	CString result;
	InvokeHelper(0x851f, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _MailItem::SetReminderSoundFile(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x851f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

DATE _MailItem::GetReminderTime()
{
	DATE result;
	InvokeHelper(0x8502, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _MailItem::SetReminderTime(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x8502, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

long _MailItem::GetRemoteStatus()
{
	long result;
	InvokeHelper(0x8511, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _MailItem::SetRemoteStatus(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x8511, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString _MailItem::GetReplyRecipientNames()
{
	CString result;
	InvokeHelper(0x50, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

LPDISPATCH _MailItem::GetReplyRecipients()
{
	LPDISPATCH result;
	InvokeHelper(0xf013, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _MailItem::GetSaveSentMessageFolder()
{
	LPDISPATCH result;
	InvokeHelper(0xf401, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _MailItem::SetRefSaveSentMessageFolder(LPDISPATCH newValue)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xf401, DISPATCH_PROPERTYPUTREF, VT_EMPTY, NULL, parms,
		 newValue);
}

CString _MailItem::GetSenderName()
{
	CString result;
	InvokeHelper(0xc1a, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

BOOL _MailItem::GetSent()
{
	BOOL result;
	InvokeHelper(0xf402, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

DATE _MailItem::GetSentOn()
{
	DATE result;
	InvokeHelper(0x39, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

CString _MailItem::GetSentOnBehalfOfName()
{
	CString result;
	InvokeHelper(0x42, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _MailItem::SetSentOnBehalfOfName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x42, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL _MailItem::GetSubmitted()
{
	BOOL result;
	InvokeHelper(0xf403, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

CString _MailItem::GetTo()
{
	CString result;
	InvokeHelper(0xe04, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _MailItem::SetTo(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xe04, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _MailItem::GetVotingOptions()
{
	CString result;
	InvokeHelper(0xf01b, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _MailItem::SetVotingOptions(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xf01b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _MailItem::GetVotingResponse()
{
	CString result;
	InvokeHelper(0x8524, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _MailItem::SetVotingResponse(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8524, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

void _MailItem::ClearConversationIndex()
{
	InvokeHelper(0xf822, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH _MailItem::Forward()
{
	LPDISPATCH result;
	InvokeHelper(0xf813, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _MailItem::Reply()
{
	LPDISPATCH result;
	InvokeHelper(0xf810, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _MailItem::ReplyAll()
{
	LPDISPATCH result;
	InvokeHelper(0xf811, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _MailItem::Send()
{
	InvokeHelper(0xf075, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}
