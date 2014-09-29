// DialogStatus.cpp : implementation file
//

#include "stdafx.h"
#include "FollowTrade.h"
#include "DialogStatus.h"
#include "afxdialogex.h"
#include "ListHeade.h"

SListHeade StatusListHeade[] =
{
	{"ʱ��",80},
	{"�˻�",80},
	{"��Ϣ",400},
};

// CDialogStatus dialog

IMPLEMENT_DYNAMIC(CDialogStatus, CDialogEx)

CDialogStatus::CDialogStatus(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogStatus::IDD, pParent)
{

}

CDialogStatus::~CDialogStatus()
{
}

void CDialogStatus::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogStatus, CDialogEx)
	//MyMessage
	ON_MESSAGE(CMessageAPI::WM_LoginError, LoginError)
	ON_MESSAGE(CMessageAPI::WM_Logining, Logining)
	ON_MESSAGE(CMessageAPI::WM_LogingSucess, LoginSuccess)
	ON_MESSAGE(CMessageAPI::WM_BrokerEmpty, BrokerEmpty)
	ON_MESSAGE(CMessageAPI::WM_TradeServerEmpty, TradeServerEmpty)
	ON_MESSAGE(CMessageAPI::WM_TraderInvestorIDEmpty, TraderInvestorIDEmpty)
	ON_MESSAGE(CMessageAPI::WM_TraderPasswordEmpty, TraderPasswordEmpty)
	ON_MESSAGE(CMessageAPI::WM_FollowerInvestorIDEmpty, FollowerInvestorIDEmpty)
	ON_MESSAGE(CMessageAPI::WM_FollowerPasswordEmpty, FollowerPasswordEmpty)
	ON_MESSAGE(CMessageAPI::WM_NewTradeStateError, NewTradeStateError)
	ON_MESSAGE(CMessageAPI::WM_TraderLoginError, TraderLoginError)
	ON_MESSAGE(CMessageAPI::WM_FollowerLoginError, FollowerLoginError)
	ON_MESSAGE(CMessageAPI::WM_TradeObjectError, TradeObjectError)
	ON_MESSAGE(CMessageAPI::WM_InitingUsedOrder, InitingUsedOrder)
	ON_MESSAGE(CMessageAPI::WM_GetTraderOrderError, GetTraderOrderError)
	ON_MESSAGE(CMessageAPI::WM_InitUsedOrderSuccess, InitUsedOrderSuccess)
	ON_MESSAGE(CMessageAPI::WM_InitingFollowerOrder, InitingFollowerOrder)
	ON_MESSAGE(CMessageAPI::WM_GetFollowerOrderError, GetFollowerOrderError)
	ON_MESSAGE(CMessageAPI::WM_InitFollowerOrderSuccess, InitFollowerOrderSuccess)
	ON_MESSAGE(CMessageAPI::WM_FollowingInsertOrder, FollowingInsertOrder)
	ON_MESSAGE(CMessageAPI::WM_FollowingDeleteOrder, FollowingDeleteOrder)
	ON_MESSAGE(CMessageAPI::WM_SendInsertOrder, SendInsertOrder)
	ON_MESSAGE(CMessageAPI::WM_SendDeleteOrder, SendDeleteOrder)
	ON_MESSAGE(CMessageAPI::WM_FollowInsertOrderInfo, FollowInsertOrderInfo)
	ON_MESSAGE(CMessageAPI::WM_FollowDeleteOrderInfo, FollowDeleteOrderInfo)
	//MyMessage
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDialogStatus message handlers


void CDialogStatus::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	CWnd *pStatusList = GetDlgItem(IDC_LIST_STATUS);
	if (NULL != pStatusList)
	{
		pStatusList->MoveWindow(0, 0, cx, cy);
	}
}


BOOL CDialogStatus::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	InitStatusList();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

bool CDialogStatus::InitStatusList()
{
	CListCtrl *pStatusList = (CListCtrl*)GetDlgItem(IDC_LIST_STATUS);
	if (NULL != pStatusList)
	{
		RECT ret = {0};
		GetClientRect(&ret);
		pStatusList->MoveWindow(&ret);

		pStatusList->ModifyStyle(0,LVS_REPORT|LVS_SINGLESEL|LVS_SHOWSELALWAYS);
		pStatusList->SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP);

		for (int nIndex(0); sizeof(StatusListHeade)/sizeof(StatusListHeade[0]) > nIndex; ++nIndex)
		{
			pStatusList->InsertColumn(nIndex, StatusListHeade[nIndex].pString, LVCFMT_LEFT, StatusListHeade[nIndex].nWidth);
		}
	}

	return true;
}

//===============MessageFunction===============
LRESULT CDialogStatus::LoginError(WPARAM wParam,LPARAM lParam)
{
	char Message[1000] = {0};
	sprintf_s(Message, "%08d ��¼���󡭡�", (int)wParam);
	InsertMessageList(Message);
	return 0;
}

LRESULT CDialogStatus::Logining(WPARAM wParam,LPARAM lParam)
{
	char Message[1000] = {0};
	sprintf_s(Message, "%08d ���ڵ�¼����", (int)wParam);
	InsertMessageList(Message);

	return 0;
}

LRESULT CDialogStatus::LoginSuccess(WPARAM wParam,LPARAM lParam)
{
	char Message[1000] = {0};
	sprintf_s(Message, "%08d ��¼�ɹ�����", (int)wParam);
	InsertMessageList(Message);
	return 0;
}

LRESULT CDialogStatus::BrokerEmpty(WPARAM wParam,LPARAM lParam)
{
	InsertMessageList("�ڻ���ID����Ϊ�ա���");
	return 0;
}

LRESULT CDialogStatus::TradeServerEmpty(WPARAM wParam,LPARAM lParam)
{
	InsertMessageList("���׷�������ַ����Ϊ�ա���");
	return 0;
}

LRESULT CDialogStatus::TraderInvestorIDEmpty(WPARAM wParam,LPARAM lParam)
{
	InsertMessageList("�����˻�����Ϊ�ա���");
	return 0;
}

LRESULT CDialogStatus::TraderPasswordEmpty(WPARAM wParam,LPARAM lParam)
{
	InsertMessageList("�����˻����벻��Ϊ�ա���");
	return 0;
}

LRESULT CDialogStatus::FollowerInvestorIDEmpty(WPARAM wParam,LPARAM lParam)
{
	InsertMessageList("�����˻�����Ϊ�ա���");
	return 0;
}

LRESULT CDialogStatus::FollowerPasswordEmpty(WPARAM wParam,LPARAM lParam)
{
	InsertMessageList("�����˻����벻��Ϊ�ա���");
	return 0;
}

LRESULT CDialogStatus::NewTradeStateError(WPARAM wParam,LPARAM lParam)
{
	InsertMessageList("������������󡭡�");
	return 0;
}

LRESULT CDialogStatus::TraderLoginError(WPARAM wParam,LPARAM lParam)
{
	InsertMessageList("�����˻���¼���󡭡�");
	return 0;
}

LRESULT CDialogStatus::FollowerLoginError(WPARAM wParam,LPARAM lParam)
{
	InsertMessageList("�����˻���¼���󡭡�");
	return 0;
}

LRESULT CDialogStatus::TradeObjectError(WPARAM wParam,LPARAM lParam)
{
	InsertMessageList("������������󡭡�");
	return 0;
}

LRESULT CDialogStatus::InitingUsedOrder(WPARAM wParam,LPARAM lParam)
{
	InsertMessageList("���ڳ�ʼ���Ѹ��汨������");
	return 0;
}

LRESULT CDialogStatus::GetTraderOrderError(WPARAM wParam,LPARAM lParam)
{
	InsertMessageList("��ȡ�����˻��������󡭡�");
	return 0;
}

LRESULT CDialogStatus::InitUsedOrderSuccess(WPARAM wParam,LPARAM lParam)
{
	InsertMessageList("��ʼ���Ѹ��汨���ɹ�����");
	return 0;
}

LRESULT CDialogStatus::InitingFollowerOrder(WPARAM wParam,LPARAM lParam)
{
	InsertMessageList("���ڳ�ʼ�������˻���������");
	return 0;
}

LRESULT CDialogStatus::GetFollowerOrderError(WPARAM wParam,LPARAM lParam)
{
	InsertMessageList("��ȡ�����˻���������");
	return 0;
}

LRESULT CDialogStatus::InitFollowerOrderSuccess(WPARAM wParam,LPARAM lParam)
{
	InsertMessageList("��ʼ�������˻������ɹ�����");
	return 0;
}

LRESULT CDialogStatus::FollowingInsertOrder(WPARAM wParam,LPARAM lParam)
{
	InsertMessageList("���ڸ��汨������");
	return 0;
}

LRESULT CDialogStatus::FollowingDeleteOrder(WPARAM wParam,LPARAM lParam)
{
	InsertMessageList("���ڸ��泷������");
	return 0;
}

LRESULT CDialogStatus::SendInsertOrder(WPARAM wParam,LPARAM lParam)
{
	char Message[1000] = {0};
	sprintf_s(Message, "���ͱ���%s����", (0 == (int)wParam) ?"ʧ��":"�ɹ�");
	InsertMessageList(Message);
	return 0;
}

LRESULT CDialogStatus::SendDeleteOrder(WPARAM wParam,LPARAM lParam)
{
	char Message[1000] = {0};
	sprintf_s(Message, "���ͳ���%s����", (0 == (int)wParam) ?"ʧ��":"�ɹ�");
	InsertMessageList(Message);
	return 0;
}

LRESULT CDialogStatus::FollowInsertOrderInfo(WPARAM wParam,LPARAM lParam)
{
	InsertMessageList((const char*)wParam);
	return 0;
}

LRESULT CDialogStatus::FollowDeleteOrderInfo(WPARAM wParam,LPARAM lParam)
{
	InsertMessageList((const char*)wParam);
	return 0;
}

//===============Function===============
void CDialogStatus::InsertMessageList(IN string strMessage)
{
	CTime CurrentTime = CTime::GetCurrentTime();

	CListCtrl *pStatusList = (CListCtrl*)GetDlgItem(IDC_LIST_STATUS);

	if (NULL != pStatusList)
	{
		int nIndex = pStatusList->GetItemCount();
		pStatusList->InsertItem(nIndex, CurrentTime.Format("%H:%M:%S"));
		pStatusList->SetItemText(nIndex, 2, strMessage.c_str());
		pStatusList->EnsureVisible(nIndex, FALSE); 
	}
}