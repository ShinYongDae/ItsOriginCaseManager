
// ItsOriginCaseManagerDlg.h : ��� ����
//

#pragma once
#include "afxcmn.h"

#define LIST_PATH _T("C:\\R2RSet\\ItsOriginCase.ini")
#define LOG_PATH _T("C:\\R2RSet\\Log\\")


#define CASE_0 _T("1 [�»�]")
#define CASE_1 _T("2 [����]")
#define CASE_2 _T("3 [���]")
#define CASE_3 _T("4 [����]")

struct stModel
{
	CString sModel;
	int nItsCase;			// 0 ~ 3 :  �¿�[0], ����[1], ���[2], ����[3]

	stModel()
	{
		sModel = _T("");
		nItsCase = 0;
	}
};

struct stList
{
	int nTotalModels;
	stModel *pModel;

	TCHAR szData[MAX_PATH];
	TCHAR sep[6]; // _T(",/;\r\n\t");
	TCHAR *token;
	CString sModelIdx;
	int i;

	stList()
	{
		nTotalModels = 0;
		pModel = NULL;
		sep[0] = _T(',');
		sep[1] = _T('/');
		sep[2] = _T(';');
		sep[3] = _T('\r');
		sep[4] = _T('\n');
		sep[5] = _T('\t');
		token = NULL;

		LoadList();
	}

	~stList()
	{
		if (pModel)
		{
			delete[] pModel;
			pModel = NULL;
		}
	}


	void LoadList()
	{
		if (0 < ::GetPrivateProfileString(_T("Info"), _T("Total_Models"), NULL, szData, sizeof(szData), LIST_PATH))
			nTotalModels = _ttoi(szData);
		else
			nTotalModels = 0;

		AllocMem();

		if (nTotalModels > 0)
		{
			for (i = 0; i < nTotalModels; i++)
			{
				sModelIdx.Format(_T("%d"), i);
				if (0 < ::GetPrivateProfileString(_T("Model"), sModelIdx, NULL, szData, sizeof(szData), LIST_PATH))
				{
					token = _tcstok(szData, sep);
					pModel[i].sModel = CString(token);
					token = _tcstok(NULL, sep);
					pModel[i].nItsCase = _ttoi(token);
				}
			}
		}
	}

	void ReloadList()
	{
		if (pModel)
		{
			delete[] pModel;
			pModel = NULL;
		}

		LoadList();
	}

	void AllocMem()
	{
		if (nTotalModels > 0)
		{
			if (pModel)
				delete[] pModel;

			pModel = new stModel[nTotalModels];
		}
	}
};

// CItsOriginCaseManagerDlg ��ȭ ����
class CItsOriginCaseManagerDlg : public CDialog
{
	CString m_sPathCamSpecDir, m_sModel;
	stList m_stList;
	int m_nCase;

	void LoadIni();
	void InitList();
	void UpdateList();
	void DispList();

	int SearchModelInCombo(CString sModel);
	int SearchModel(CString sModel);
	int SearchModel(CString sModel, int &nCase);
	BOOL UpdateModel(CString sModel, int nCase);
	void InsertModel(CString sModel, int nCase);
	void ModifyModel(CString sModel, int nCase);
	void ModifyModelData();
	void SelectList(CString sModel);
	BOOL DeleteModel(CString sModel);

	int GetItsOriginCase(CString sModel);
	void ResetComboItsCase();
	CString GetItsCaseLabel(int nIdx);
	void ModifyItsOriginCaseData();
	void ModifyItsOriginCaseData(int nCase);

	void Log(CString strMsg, int nType = 0);
	BOOL DirectoryExists(LPCTSTR szPath);
	void StringToChar(CString str, char* pCh); // char* returned must be deleted... 

// �����Դϴ�.
public:
	CItsOriginCaseManagerDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ITSORIGINCASEMANAGER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_List;
	afx_msg void OnClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonSaveItsCase();
	afx_msg void OnBnClickedButtonRefreshModel();
	afx_msg void OnSelchangeComboItsCase();
	afx_msg void OnBnClickedButtonFindModel();
	afx_msg void OnBnClickedButtonDeleteModel();
	afx_msg void OnSelchangeComboModel();
};
