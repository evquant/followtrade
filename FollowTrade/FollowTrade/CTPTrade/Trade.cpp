#include "stdafx.h"
#include "Trade.h"
#include <iostream>

using namespace std;

CTrade::CTrade(ITrade *pTradeInterface /* = NULL */)
	:m_TradeSPI(this),
	m_pTradeInterface(pTradeInterface)
{

}

CTrade::~CTrade()
{

}

//===================Initialize===================//
bool CTrade::Initialize(const char *pTradeServer, const char *pTradeLogin, const char *pTradePassword, const char *pTradeBroker)
{
	return m_TradeSPI.Initialize(pTradeServer, pTradeLogin, pTradePassword, pTradeBroker);
}

//===================OnCallBack===================//
///����¼��������Ӧ
void CTrade::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cout<< "=====================OnRspOrderInsert====================="<< endl;

	if (NULL != pInputOrder)
	{
		cout<< "CTraderHelper::OnRspOrderInsert::	InvestorID: "<< pInputOrder->InvestorID << endl;
		cout<< "CTraderHelper::OnRspOrderInsert::	InstrumentID: "<< pInputOrder->InstrumentID << endl;
		cout<< "CTraderHelper::OnRspOrderInsert::	OrderRef: "<< pInputOrder->OrderRef << endl;
	}

	if (NULL != pRspInfo)
	{
		cout<< "CTraderHelper::OnRspOrderInsert::	������Ӧ��Ϣ: "<< pRspInfo->ErrorMsg << endl;
	}
	else
	{
		cout<< "CTraderHelper::OnRspOrderInsert::	������Ӧ."<< endl;
	}
}

///��������������Ӧ
void CTrade::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cout<< "=====================OnRspOrderAction====================="<< endl;

	if (NULL != pInputOrderAction)
	{
		cout<< "CTraderHelper::OnRspOrderAction::	InvestorID: "<< pInputOrderAction->InvestorID << endl;
		cout<< "CTraderHelper::OnRspOrderAction::	InstrumentID: "<< pInputOrderAction->InstrumentID << endl;
		cout<< "CTraderHelper::OnRspOrderAction::	OrderRef: "<< pInputOrderAction->OrderRef << endl;
		cout<< "CTraderHelper::OnRspOrderAction::	SessionID: "<< pInputOrderAction->SessionID << endl;
	}

	if (NULL != pRspInfo)
	{
		cout<< "CTraderHelper::OnRspOrderAction::	������Ӧ��Ϣ: "<< pRspInfo->ErrorMsg << endl;
	}
	else
	{
		cout<< "CTraderHelper::OnRspOrderAction::	������Ӧ."<< endl;
	}
}

///����¼�����ر�
void CTrade::OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo)
{
	cout<< "=====================OnErrRtnOrderInsert====================="<< endl;

	if(NULL != pInputOrder)
	{
		cout<< "CTraderHelper::OnErrRtnOrderInsert::	InvestorID: "<< pInputOrder->InvestorID << endl;
		cout<< "CTraderHelper::OnErrRtnOrderInsert::	InstrumentID: "<< pInputOrder->InstrumentID << endl;
		cout<< "CTraderHelper::OnErrRtnOrderInsert::	OrderRef: "<< pInputOrder->OrderRef << endl;
	}

	if (NULL != pRspInfo)
	{
		cout<< "CTraderHelper::OnErrRtnOrderInsert::	����������Ӧ��Ϣ: "<< pRspInfo->ErrorMsg << endl;
	}
	else
	{
		cout<< "CTraderHelper::OnErrRtnOrderInsert::	����������Ӧ."<< endl;
	}
}

///������������ر�
void CTrade::OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo)
{
	cout<< "=====================OnErrRtnOrderAction====================="<< endl;
	if(NULL != pOrderAction)
	{
		cout<< "CTraderHelper::OnErrRtnOrderAction::	InvestorID: "<< pOrderAction->InvestorID << endl;
		cout<< "CTraderHelper::OnErrRtnOrderAction::	InstrumentID: "<< pOrderAction->InstrumentID << endl;
		cout<< "CTraderHelper::OnErrRtnOrderAction::	OrderRef: "<< pOrderAction->OrderRef << endl;
	}

	if (NULL != pRspInfo)
	{
		cout<< "CTraderHelper::OnErrRtnOrderAction::	����������Ӧ��Ϣ: "<< pRspInfo->ErrorMsg << endl;
	}
	else
	{
		cout<< "CTraderHelper::OnErrRtnOrderAction::	����������Ӧ."<< endl;
	}
}

///����֪ͨ
void CTrade::OnRtnOrder(CThostFtdcOrderField *pOrder)
{
	cout<< "=====================OnRtnOrder====================="<< endl;

	if(NULL != pOrder)
	{
		if (NULL != m_pTradeInterface)
		{

			m_pTradeInterface->OnRtnOrder(pOrder);
		}

		cout<< "CTraderHelper::OnRtnOrder::	InvestorID: "<< pOrder->InvestorID << endl;
		cout<< "CTraderHelper::OnRtnOrder::	InstrumentID: "<< pOrder->InstrumentID << endl;
		cout<< "CTraderHelper::OnRtnOrder::	OrderRef: "<< pOrder->OrderRef << endl;
		cout<< "CTraderHelper::OnRtnOrder::	StatusMsg: "<< pOrder->StatusMsg << endl;
	}
}

///�ɽ�֪ͨ
void CTrade::OnRtnTrade(CThostFtdcTradeField *pTrade)
{
	cout<< "=====================OnRtnTrade====================="<< endl;

	if (NULL != pTrade)
	{
		if (NULL != m_pTradeInterface)
		{
			m_pTradeInterface->OnRtnTrade(pTrade);
		}

		cout<< "CTraderHelper::OnRtnTrade::	InvestorID: "<< pTrade->InvestorID << endl;
		cout<< "CTraderHelper::OnRtnTrade::	InstrumentID: "<< pTrade->InstrumentID << endl;
		cout<< "CTraderHelper::OnRtnTrade::	OrderRef: "<< pTrade->OrderRef << endl;
	}
}

///����Ӧ��
void CTrade::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (NULL != pRspInfo)
	{
		cout<< "CTraderHelper::OnRspError::	������Ӧ��Ϣ: "<< pRspInfo->ErrorMsg << endl;
	}
	else
	{
		cout<< "CTraderHelper::OnRspError::	������Ӧ."<< endl;
	}
}

//===================API===================//
bool CTrade::OrderInsert(IN COrder &Order)
{
	return m_TradeSPI.OrderInsert(Order);
}

bool CTrade::OrderDelete(IN const COrder &Order)
{
	return m_TradeSPI.OrderDelete(Order);
}

//===================Get/Set===================//
bool CTrade::GetOrder(OUT vector<COrder> &vecOrder)
{
	return m_TradeSPI.GetOrder(vecOrder);
}

bool CTrade::GetPosition(OUT vector<CPosition> &vecPosition)
{
	return m_TradeSPI.GetPosition(vecPosition);
}

bool CTrade::GetAccount(OUT CAccount &Account)
{
	return m_TradeSPI.GetAccount(Account);
}

bool CTrade::GetSymbol(IN map<string, CSymbol> &mapSymbol)
{
	return m_TradeSPI.GetSymbol(mapSymbol);
}

int CTrade::GetOrderRef()
{
	return m_TradeSPI.GetOrderRef_Int();
}

int CTrade::GetFrontID()
{
	return m_TradeSPI.GetFrontID();
}

int CTrade::GetSessionID()
{
	return m_TradeSPI.GetSessionID();
}

string CTrade::GetInvestorID()
{
	return m_TradeSPI.GetInvestorID();
}

string CTrade::GetLoginTime()
{
	return m_TradeSPI.GetLoginTime();
}

string CTrade::GetLoginDate()
{
	return m_TradeSPI.GetLoginDate();
}

//===================private===================//
bool CTrade::IsErrorRspInfo(IN CThostFtdcRspInfoField *pRspInfo)
{
	return (NULL != pRspInfo && 0 != pRspInfo->ErrorID);
}