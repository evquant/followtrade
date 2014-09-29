#pragma once
#include "CTPAPI/ThostFtdcTraderApi.h"
#include "CTPAPI/ThostFtdcUserApiDataType.h"
#include "CTPAPI/ThostFtdcUserApiStruct.h"
#include <string>
#include <windows.h>
#include <iostream>
#include <map>
#include <vector>
#include "Entity/Entity.h"
#include "Common/Common.h"

using namespace std;

class CTrade;

class CTradeSPI: public CThostFtdcTraderSpi
{
public:
	CTradeSPI(CTrade *pTrade);
	~CTradeSPI();

public://Initialize
	bool Initialize(const char *pTradeServer, const char *pTradeLogin, const char *pTradePassword, const char *pTradeBroker);

public://Event
	///����������ɵ���Ӧ�¼�
	void OnFrontConnected();
	///�Ͽ����ӵ���Ӧ�¼�
	void OnFrontDisconnected(int nReason);
	///��¼������Ӧ
	void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	///�ǳ�������Ӧ
	void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	///Ͷ���߽�����ȷ����Ӧ
	void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
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
	///�����ѯ��Լ��Ӧ
	void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	///�����ѯ������Ӧ
	void OnRspQryDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	///�����ѯ������Ӧ
	void OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	///�����ѯͶ���ֲ߳���Ӧ
	void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	///�����ѯ�ʽ��˻���Ӧ
	void OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

private://Common
	bool IsErrorRspInfo(IN CThostFtdcRspInfoField *pRspInfo);					//�Ƿ������Ϣ
	int GetRequestID();															//��ȡ����ID
	void SetOrderRef(IN int nOrderRef);											//���ñ�������
	string GetOrderRef();														//��ȡ��������
	string GetCTPString(IN const char *pString, IN int nLength);				//��ΪCTP API�����ף�����ֱ�Ӹ�ֵ��string����Խ�硣

public://Request
	bool OrderInsert(IN COrder &Order);
	bool OrderDelete(IN const COrder &Order);

public://Get
	bool GetInstrument(IN vector<CThostFtdcInstrumentField> &vecInstrumentField);
	bool GetMarketData(IN vector<CThostFtdcDepthMarketDataField> &vecMarketDataField);
	bool GetSymbol(IN map<string, CSymbol> &mapSymbol);
	bool GetOrder(OUT vector<COrder> &vecOrder);
	bool GetPosition(OUT vector<CPosition> &vecPosition);
	bool GetAccount(OUT CAccount &Account);
	int GetOrderRef_Int();
	int GetFrontID();
	int GetSessionID();
	string GetInvestorID();
	string GetLoginTime();
	string GetLoginDate();

private://Fill
	bool FillSymbolByInstrument(OUT CSymbol &Symbol, IN const CThostFtdcInstrumentField &InstrumentField);
	bool FillSymbolByMarketData(OUT CSymbol &Symbol, IN const CThostFtdcDepthMarketDataField &MarketDataField);
	bool FillInputOrderFieldByOrder(OUT CThostFtdcInputOrderField &InputOrderField, IN const COrder &Order);
	bool FillInputOrderActionFieldByOrder(OUT CThostFtdcInputOrderActionField &InputOrderActionField, IN const COrder &Order);
	bool FillOrderByOrderField(OUT COrder &Order, IN const CThostFtdcOrderField &OrderField);
	bool FillPositionByPositionField(OUT CPosition &Position, IN const CThostFtdcInvestorPositionField &PositionField);
	bool FillAccountByTradingAccountField(OUT CAccount &Account, IN const CThostFtdcTradingAccountField &AccountField);


private://Temp
	map<int, vector<CThostFtdcInstrumentField>> m_mapInstrumentFieldTemp;		//��ѯ��Լ�Ļ���,KEY��nRequestID
	map<int, vector<CThostFtdcDepthMarketDataField>> m_mapMarketDataFieldTemp;	//��ѯ�г���ȵĻ���,KEY��nRequestID
	map<int, vector<CThostFtdcOrderField>> m_mapOrderTemp;						//��ѯ�����Ļ���,KEY��nRequestID
	map<int, vector<CThostFtdcInvestorPositionField>> m_mapPositionTemp;		//��ѯ��λ�Ļ���,KEY��nRequestID
	map<int, vector<CThostFtdcTradingAccountField>> m_mapAccountTemp;			//��ѯ�˻��Ļ���,KEY��nRequestID
private:
	map<HANDLE, SEventParameter> m_mapEventParameter;							//�¼�·��,KEY:�¼��ľ��
private:
	CThostFtdcTraderApi *m_pTradeAPI;
private:
	CTrade *m_pTrade;	
	CONFIG m_Config;
	CThostFtdcRspUserLoginField m_UserLoginField;	//CTP�û���Ϣ
private:
	HANDLE m_hLoginEvent;		//��¼�¼�
	bool m_bLoginSuccess;		//�жϵ�¼�Ƿ�ɹ�
private:
	int m_nRequestID;			//�����
	int m_nOrderRef;			//��CTP�ı�������

};