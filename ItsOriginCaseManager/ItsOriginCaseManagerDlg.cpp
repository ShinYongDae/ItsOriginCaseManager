
// ItsOriginCaseManagerDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "ItsOriginCaseManager.h"
#include "ItsOriginCaseManagerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CItsOriginCaseManagerDlg ��ȭ ����



CItsOriginCaseManagerDlg::CItsOriginCaseManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_ITSORIGINCASEMANAGER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_sPathCamSpecDir = _T("");

	m_sModel = _T("");
	m_nCase = 0;
}

void CItsOriginCaseManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
}

BEGIN_MESSAGE_MAP(CItsOriginCaseManagerDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CItsOriginCaseManagerDlg::OnClickList1)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_ITS_CASE, &CItsOriginCaseManagerDlg::OnBnClickedButtonSaveItsCase)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH_MODEL, &CItsOriginCaseManagerDlg::OnBnClickedButtonRefreshModel)
	ON_CBN_SELCHANGE(IDC_COMBO_ITS_CASE, &CItsOriginCaseManagerDlg::OnSelchangeComboItsCase)
	ON_BN_CLICKED(IDC_BUTTON_FIND_MODEL, &CItsOriginCaseManagerDlg::OnBnClickedButtonFindModel)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_MODEL, &CItsOriginCaseManagerDlg::OnBnClickedButtonDeleteModel)
	ON_CBN_SELCHANGE(IDC_COMBO_MODEL, &CItsOriginCaseManagerDlg::OnSelchangeComboModel)
END_MESSAGE_MAP()


// CItsOriginCaseManagerDlg �޽��� ó����

BOOL CItsOriginCaseManagerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	LoadIni();
	InitList();
	DispList();

	ModifyModelData();
	ModifyItsOriginCaseData();

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CItsOriginCaseManagerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CItsOriginCaseManagerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CItsOriginCaseManagerDlg::LoadIni()
{
	TCHAR szData[MAX_PATH];

	if (0 < ::GetPrivateProfileString(_T("Info"), _T("CamSpecDir"), NULL, szData, sizeof(szData), LIST_PATH))
		m_sPathCamSpecDir = CString(szData);
	else
	{
		AfxMessageBox(_T("CamMaster�� Spec���丮 Path�� �����Ǿ� ���� �ʽ��ϴ�."), MB_ICONWARNING | MB_OK);
		m_sPathCamSpecDir = CString(_T(""));
	}
}

void CItsOriginCaseManagerDlg::InitList()
{
	// Addition ListControl
	int nIdxUnit;
	CString strItem = _T("");

	m_List.DeleteAllItems();
	Sleep(30);
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT); // ���� �߰�
	m_List.InsertColumn(0, _T("Index"), LVCFMT_CENTER, 50);//LVCFMT_LEFT
	m_List.InsertColumn(1, _T("��"), LVCFMT_CENTER, 180);//LVCFMT_LEFT
	m_List.InsertColumn(2, _T("ITS���� CASE"), LVCFMT_CENTER, 100);
}

void CItsOriginCaseManagerDlg::UpdateList()
{
	m_stList.ReloadList();
	DispList();
}

void CItsOriginCaseManagerDlg::DispList()
{
	int nIdxModel, nIdxUnit;
	TCHAR tChar[200];
	CString sVal;

	m_List.DeleteAllItems();
	Sleep(30);

	for (nIdxModel = 0; nIdxModel < m_stList.nTotalModels; nIdxModel++)
	{
		LVITEM item;
		::ZeroMemory(&item, sizeof(item));
		item.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
		// Index
		item.iItem = m_List.GetItemCount();
		item.iSubItem = 0;
		item.iImage = 1;
		sVal.Format(_T("%10d"), nIdxModel);
		_stprintf(tChar, _T("%s"), sVal);
		item.pszText = tChar;
		m_List.InsertItem(&item);
		// ��
		item.iSubItem = 1;
		_stprintf(tChar, _T("%s"), m_stList.pModel[nIdxModel].sModel);
		item.pszText = tChar;
		m_List.SetItem(&item);
		// ITS���� case
		item.iSubItem = 2;
		_stprintf(tChar, _T("%s"), GetItsCaseLabel(m_stList.pModel[nIdxModel].nItsCase));
		item.pszText = tChar;
		m_List.SetItem(&item);
	}

	m_List.ModifyStyle(LVS_TYPEMASK, LVS_REPORT);
}

CString CItsOriginCaseManagerDlg::GetItsCaseLabel(int nIdx) // �¿�[0], ����[1], ���[2], ����[3]
{
	CString sName = _T("");
	switch (nIdx)
	{
	case 0:
		sName = CASE_0;
		break;
	case 1:
		sName = CASE_1;
		break;
	case 2:
		sName = CASE_2;
		break;
	case 3:
		sName = CASE_3;
		break;
	default:
		sName = CASE_0;
		break;
	}
	return sName;
}



void CItsOriginCaseManagerDlg::OnClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;

	// �� Ŭ���� �� �ѹ��� �޾ƿ���
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int idx = pNMListView->iItem;

	// ���õ� �����۰�(idx)�� ������ [1]��° col�� �������� �����´�.
	CString sModel = m_List.GetItemText(idx, 1);

	// Model ComboBox���� ������ ���� ǥ���Ѵ�.
	int nSel = SearchModelInCombo(sModel);
	((CComboBox*)GetDlgItem(IDC_COMBO_MODEL))->SetCurSel(nSel);
	int nCase = GetItsOriginCase(sModel);
	if (nCase < 0)
	{
		return;
	}
	m_sModel = sModel;
	m_nCase = nCase;
}

int CItsOriginCaseManagerDlg::SearchModelInCombo(CString sModel)
{
	int i, idx = -1, nTotalModel = 0;
	CString sModelInCombo;

	nTotalModel = ((CComboBox*)GetDlgItem(IDC_COMBO_MODEL))->GetCount();

	for (i = 0; i < nTotalModel; i++)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_MODEL))->GetLBText(i, sModelInCombo);

		if (sModelInCombo == sModel)
			return i;
	}

	return idx;
}

int CItsOriginCaseManagerDlg::GetItsOriginCase(CString sModel)
{
	int nModelIdx, nCase;
	CString sModelIdx, sListModel;
	TCHAR szData[MAX_PATH];
	TCHAR *token;
	TCHAR sep[] = { _T(",;\r\n\t") };

	nModelIdx = SearchModel(sModel, nCase);
	if (nModelIdx < 0)
		return -1; // Not Found

	//sModelIdx.Format(_T("%d"), nModelIdx);
	//if (0 < ::GetPrivateProfileString(_T("Model"), sModelIdx, NULL, szData, sizeof(szData), LIST_PATH))
	//{
	//	token = _tcstok(szData, sep);
	//	sListModel = CString(token);
	//	token = _tcstok(NULL, sep);
	//	nThickness = _ttoi(token);
	//}
	//else
	//	return -1; // Not found.

	return nCase;
}

int CItsOriginCaseManagerDlg::SearchModel(CString sModel)
{
	int nModelIdx, nCase;
	CString sModelIdx, sListModel;
	TCHAR szData[MAX_PATH];
	TCHAR *token;
	TCHAR sep[] = { _T(",;\r\n\t") };

	for (nModelIdx = 0; nModelIdx < m_stList.nTotalModels; nModelIdx++)
	{
		sModelIdx.Format(_T("%d"), nModelIdx);
		if (0 < ::GetPrivateProfileString(_T("Model"), sModelIdx, NULL, szData, sizeof(szData), LIST_PATH))
		{
			token = _tcstok(szData, sep);
			sListModel = CString(token);
			token = _tcstok(NULL, sep);
			nCase = _ttoi(token);
		}
		else
			sListModel = _T("");

		if (sListModel == sModel)
			return nModelIdx;
	}

	return -1; // Not Found
}

int CItsOriginCaseManagerDlg::SearchModel(CString sModel, int &nCase)
{
	int nModelIdx;
	CString sModelIdx, sListModel;
	TCHAR szData[MAX_PATH];
	TCHAR *token;
	TCHAR sep[] = { _T(",;\r\n\t") };

	for (nModelIdx = 0; nModelIdx < m_stList.nTotalModels; nModelIdx++)
	{
		sModelIdx.Format(_T("%d"), nModelIdx);
		if (0 < ::GetPrivateProfileString(_T("Model"), sModelIdx, NULL, szData, sizeof(szData), LIST_PATH))
		{
			token = _tcstok(szData, sep);
			sListModel = CString(token);
			token = _tcstok(NULL, sep);
			nCase = _ttoi(token);
		}
		else
			sListModel = _T("");

		if (sListModel == sModel)
			return nModelIdx;
	}

	return -1; // Not Found
}



void CItsOriginCaseManagerDlg::OnBnClickedButtonSaveItsCase()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (m_sModel.IsEmpty())
	{
		AfxMessageBox(_T("Model�� ���õ��� �ʾҽ��ϴ�."));
		return;
	}

	if (m_nCase < 0)
	{
		AfxMessageBox(_T("ITS���� CASE�� ���õ��� �ʾҽ��ϴ�."));
		return;
	}

	CString sMsg;
	sMsg.Format(_T("%s ���� ITS���� CASE�� case %d�� �����Ͻðڽ��ϱ�?"), m_sModel, m_nCase);
	if (IDNO == MessageBox(sMsg, _T("����"), MB_YESNO | MB_ICONQUESTION))
	{
		return;
	}

	UpdateModel(m_sModel, m_nCase);
	UpdateList();
	sMsg.Format(_T("%s ���� ITS���� CASE�� case %d�� ����"), m_sModel, m_nCase);
	Log(sMsg);
}

BOOL CItsOriginCaseManagerDlg::UpdateModel(CString sModel, int nCase)
{
	int nIdx = SearchModel(sModel);

	if (nIdx < 0) // Not Found
	{
		InsertModel(sModel, nCase);
	}
	else
	{
		ModifyModel(sModel, nCase);
	}

	return TRUE;
}

void CItsOriginCaseManagerDlg::InsertModel(CString sModel, int nCase)
{
	CString sModelIdx, strData;
	int nModelIdx = m_stList.nTotalModels;
	sModelIdx.Format(_T("%d"), nModelIdx);
	strData.Format(_T("%s,%d"), sModel, nCase);
	::WritePrivateProfileString(_T("Model"), sModelIdx, strData, LIST_PATH);
	m_stList.nTotalModels++;

	strData.Format(_T("%d"), m_stList.nTotalModels);
	::WritePrivateProfileString(_T("Info"), _T("Total_Models"), strData, LIST_PATH);
}

void CItsOriginCaseManagerDlg::ModifyModel(CString sModel, int nCase)
{
	CString sModelIdx, strData;
	int nModelIdx = SearchModel(sModel);
	sModelIdx.Format(_T("%d"), nModelIdx);
	strData.Format(_T("%s,%d"), sModel, nCase);
	::WritePrivateProfileString(_T("Model"), sModelIdx, strData, LIST_PATH);
}

void CItsOriginCaseManagerDlg::Log(CString strMsg, int nType)
{
	TCHAR szFile[MAX_PATH] = { 0, };
	TCHAR szPath[MAX_PATH] = { 0, };
	TCHAR* pszPos = NULL;

	_stprintf(szPath, LOG_PATH);
	if (!DirectoryExists(szPath))
		CreateDirectory(szPath, NULL);

	COleDateTime time = COleDateTime::GetCurrentTime();

	switch (nType)
	{
	case 0:
		_stprintf(szFile, _T("%s\\%s.txt"), szPath, COleDateTime::GetCurrentTime().Format(_T("%Y%m%d")));
		break;
	}

	CString strDate;
	CString strContents;
	CTime now;

	strDate.Format(_T("%s - "), COleDateTime::GetCurrentTime().Format(_T("%Y/%m/%d %H:%M:%S")));
	strContents = strDate;
	strContents += strMsg;
	strContents += _T("\r");

	CFile file;
	if (file.Open(szFile, CFile::modeWrite) == 0)
	{
		if (file.Open(szFile, CFile::modeCreate | CFile::modeWrite) == 0)
			return;
	}

	char cameraKey[1024];
	StringToChar(strContents, cameraKey);

	file.SeekToEnd();
	int nLenth = strContents.GetLength();
	int nLenth2 = strlen(cameraKey);
	file.Write(cameraKey, nLenth2);
	file.Flush();
	file.Close();
}

void CItsOriginCaseManagerDlg::StringToChar(CString str, char* pCh) // char* returned must be deleted... 
{
	wchar_t* wszStr;
	int	nLenth;

	USES_CONVERSION;
	//1. CString to wchar_t* conversion
	wszStr = T2W(str.GetBuffer(str.GetLength()));

	//2. wchar_t* to char* conversion
	nLenth = WideCharToMultiByte(CP_ACP, 0, wszStr, -1, NULL, 0, NULL, NULL); //char* ���� ���ѱ��̸� ���� 

																			  //3. wchar_t* to char* conversion
	WideCharToMultiByte(CP_ACP, 0, wszStr, -1, pCh, nLenth, 0, 0);
	return;
}

BOOL CItsOriginCaseManagerDlg::DirectoryExists(LPCTSTR szPath)
{
	DWORD dwAttrib = GetFileAttributes(szPath);

	return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
		(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}


void CItsOriginCaseManagerDlg::OnBnClickedButtonRefreshModel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	ModifyModelData();
}

void CItsOriginCaseManagerDlg::ModifyModelData()
{
	((CComboBox*)GetDlgItem(IDC_COMBO_MODEL))->ResetContent();

	TCHAR FN[100];
	_stprintf(FN, _T("%s*.*"), m_sPathCamSpecDir);

	((CComboBox*)GetDlgItem(IDC_COMBO_MODEL))->Dir(0x8010, FN);
	int t = 0;

	//	"[..]"�� ���� 
	//((CComboBox*)GetDlgItem(IDC_COMBO_MODEL))->DeleteString(0);
	int nIndex = ((CComboBox*)GetDlgItem(IDC_COMBO_MODEL))->FindStringExact(-1, _T("[..]"));
	if (nIndex > -1)
		((CComboBox*)GetDlgItem(IDC_COMBO_MODEL))->DeleteString(nIndex);

	int nCount = ((CComboBox*)GetDlgItem(IDC_COMBO_MODEL))->GetCount();

	CString strBuf, strBuf2;
	for (int i = 0; i < nCount; i++)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_MODEL))->GetLBText(i, strBuf);

		if (strBuf.GetLength() < 3)
			continue;
		// �����̸����� "["�� ���� 
		CString strBuf2 = strBuf.Mid(1, strBuf.GetLength() - 2);
		((CComboBox*)GetDlgItem(IDC_COMBO_MODEL))->InsertString(t, strBuf2);
		((CComboBox*)GetDlgItem(IDC_COMBO_MODEL))->DeleteString(t + 1);
		t++;
	}
}

void CItsOriginCaseManagerDlg::ModifyItsOriginCaseData()
{
	// Model Thickness
	((CComboBox*)GetDlgItem(IDC_COMBO_ITS_CASE))->ResetContent();
	((CComboBox*)GetDlgItem(IDC_COMBO_ITS_CASE))->InsertString(0, GetItsCaseLabel(0));
	((CComboBox*)GetDlgItem(IDC_COMBO_ITS_CASE))->InsertString(1, GetItsCaseLabel(1));
	((CComboBox*)GetDlgItem(IDC_COMBO_ITS_CASE))->InsertString(2, GetItsCaseLabel(2));
	((CComboBox*)GetDlgItem(IDC_COMBO_ITS_CASE))->InsertString(3, GetItsCaseLabel(3));
}

void CItsOriginCaseManagerDlg::ModifyItsOriginCaseData(int nCase)
{
	if (nCase < 0)
		return;

	int nIndex = nCase;
	((CComboBox*)GetDlgItem(IDC_COMBO_ITS_CASE))->SetCurSel(nIndex);
}

void CItsOriginCaseManagerDlg::OnSelchangeComboItsCase()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString sCase = _T("");

	int nIndex = ((CComboBox*)GetDlgItem(IDC_COMBO_ITS_CASE))->GetCurSel();
	if (nIndex != LB_ERR)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_ITS_CASE))->GetLBText(nIndex, sCase);
	}

	if (sCase.IsEmpty())
		return;

	if (sCase == CASE_0)
		m_nCase = 0;

	if (sCase == CASE_1)
		m_nCase = 1;

	if (sCase == CASE_2)
		m_nCase = 2;

	if (sCase == CASE_3)
		m_nCase = 3;
}


void CItsOriginCaseManagerDlg::OnBnClickedButtonFindModel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString sModel, sMsg;
	GetDlgItem(IDC_EDIT_MODEL)->GetWindowText(sModel);
	sModel.MakeUpper();

	int nSel = SearchModelInCombo(sModel);
	if (nSel < 0)
	{
		sMsg.Format(_T("%s ���� ��ũ���� �������� �ʾҽ��ϴ�."), sModel);
		AfxMessageBox(sMsg);
		return;
	}

	((CComboBox*)GetDlgItem(IDC_COMBO_MODEL))->SetCurSel(nSel);
	m_sModel = sModel;

	int nCase = GetItsOriginCase(sModel);
	if (nCase < 0)
	{
		ResetComboItsCase();
		return;
	}
	ModifyItsOriginCaseData(nCase);

	m_sModel = sModel;
	m_nCase = nCase;

	SelectList(sModel);
}

void CItsOriginCaseManagerDlg::ResetComboItsCase()
{
	((CComboBox*)GetDlgItem(IDC_COMBO_ITS_CASE))->SetWindowText(_T(""));
}

void CItsOriginCaseManagerDlg::SelectList(CString sModel)
{
	int i = SearchModel(sModel);

	// ���� ���� ���û��¸� �����մϴ�
	m_List.SetItemState(-1, 0, LVIS_SELECTED | LVIS_FOCUSED);
	// ���ϴ� �������� �����մϴ�
	m_List.SetItemState(i, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
	// ���õ� �������� ǥ���մϴ�
	m_List.EnsureVisible(i, false);
	// ����Ʈ ��Ʈ�ѿ� ��Ŀ���� ����ϴ�
	m_List.SetFocus();

	//Ư����ġ ���̶���Ʈ
	m_List.SetSelectionMark(i);
	m_List.EnsureVisible(i, TRUE); //��ũ��
	m_List.SetItemState(m_List.GetSelectionMark(), LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	m_List.SetFocus();
}


void CItsOriginCaseManagerDlg::OnBnClickedButtonDeleteModel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString sModel = m_sModel;
	int nCase = m_nCase;
	CString sMsg;
	sMsg.Format(_T("�� : %s �� ���� ��ũ���� ������ �����Ͻðڽ��ϱ�?"), sModel);
	if (IDNO == MessageBox(sMsg, _T("����"), MB_YESNO | MB_ICONQUESTION))
	{
		return;
	}

	BOOL bRtn = DeleteModel(sModel);

	if (bRtn)
		UpdateList();
}

BOOL CItsOriginCaseManagerDlg::DeleteModel(CString sModel)
{
	int nDelIdx = SearchModel(sModel);
	if (nDelIdx < 0)
		return FALSE;

	int nModelTot = m_stList.nTotalModels;

	TCHAR szData[MAX_PATH];
	TCHAR *token;
	TCHAR sep[] = { _T(",;\r\n\t") };

	CString sModelIdx, sData;
	int nThickness;
	CString sListModel;

	for (int i = nDelIdx + 1; i < nModelTot; i++)
	{
		sModelIdx.Format(_T("%d"), i);
		if (0 < ::GetPrivateProfileString(_T("Model"), sModelIdx, NULL, szData, sizeof(szData), LIST_PATH))
		{
			token = _tcstok(szData, sep);
			sListModel = CString(token);
			token = _tcstok(NULL, sep);
			nThickness = _ttoi(token);

			sModelIdx.Format(_T("%d"), i - 1);
			sData.Format(_T("%s,%d"), sListModel, nThickness);
			::WritePrivateProfileString(_T("Model"), sModelIdx, sData, LIST_PATH);
		}
	}

	m_stList.nTotalModels--;

	sModelIdx.Format(_T("%d"), m_stList.nTotalModels);
	sData.Format(_T(""));
	::WritePrivateProfileString(_T("Model"), sModelIdx, sData, LIST_PATH);

	sData.Format(_T("%d"), m_stList.nTotalModels);
	::WritePrivateProfileString(_T("Info"), _T("Total_Models"), sData, LIST_PATH);

	return TRUE;
}


void CItsOriginCaseManagerDlg::OnSelchangeComboModel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString sModel, sMsg;
	int nCase;
	int nIndex = ((CComboBox*)GetDlgItem(IDC_COMBO_MODEL))->GetCurSel();
	if (nIndex != LB_ERR)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_MODEL))->GetLBText(nIndex, sModel);
		m_sModel = sModel;

		nCase = GetItsOriginCase(sModel);
		if (nCase < 0)
		{
			sMsg.Format(_T("%s ���� ITS���� CASE�� �������� �ʾҽ��ϴ�."), sModel);
			AfxMessageBox(sMsg);
			ResetComboItsCase();
			return;
		}
		ModifyItsOriginCaseData(nCase);
		m_sModel = sModel;
		m_nCase = nCase;
		SelectList(sModel);
	}
}
