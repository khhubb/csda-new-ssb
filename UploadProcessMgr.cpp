#include "stdafx.h"
#include "UploadProcessMgr.h"
#include <Windows.h>

CUploadProcessMgr::CUploadProcessMgr()
{
}


CUploadProcessMgr::~CUploadProcessMgr()
{
}


CUploadProcessMgr::E_UploadResult CUploadProcessMgr::Invoke()
{
	CString cmd;
	cmd.Format(".\\RemoteDownload %d", m_caster);

	int result = system(LPCTSTR(cmd));

	if (result == -1)
	{
		AfxMessageBox("Failed to create process to show transport window\nHit OK to continue.");
		return FAIL;
	}

	if (result == 0)
		return PROCEED;

	return CANCEL;
}
