#pragma once
#include "TradeSPI.h"
#include "Entity/Entity.h"
#include "Entity/Interface/ITrade.h"
#include <vector>

using namespace std;

class CTrade
{
public:
	CTrade(ITrade *pTradeInterface = NULL);
	virtual ~CTrade();

public://Initialize
	bool Initialize(const char *pTradeServer, const char *pTradeLogin, const char *pTradePassword, const char *pTradeBroker);

public://OnCallBack
	///����¼��������Ӧ
	void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	///��������������Ӧ
	void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	///����¼�����ر�
	void OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo);
	///������������ر�
	void OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo);
	///����֪ͨ
	void OnRtnOrder(CThostFtdcOrderField *pOrder);
	///�ɽ�֪ͨ
	void OnRtnTrade(CThostFtdcTradeField *pTrade);
	///����Ӧ��
	void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

public://API
	bool OrderInsert(IN COrder &Order);
	bool OrderDelete(IN const COrder &Order);

public://Get/Set
	bool GetOrder(OUT vector<COrder> &vecOrder);
	bool GetPosition(OUT vector<CPosition> &vecPosition);
	bool GetAccount(OUT CAccount &Account);
	bool GetSymbol(IN map<string, CSymbol> &mapSymbol);
	int GetOrderRef();
	int GetFrontID();
	int GetSessionID();
	string GetInvestorID();
	string GetLoginTime();
	string GetLoginDate();

private://private
	bool IsErrorRspInfo(IN CThostFtdcRspInfoField *pRspInfo);

private:
	ITrade *m_pTradeInterface;

private:
	CTradeSPI m_TradeSPI;
};