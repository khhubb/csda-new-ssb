#pragma once
class CUploadProcessMgr
{
public:

	enum E_UploadResult { PROCEED, CANCEL, FAIL };

	CUploadProcessMgr();
	~CUploadProcessMgr();

	int m_caster;
	E_UploadResult Invoke();

};

