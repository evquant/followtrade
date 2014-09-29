#include "stdafx.h"
#include "TradeSPI.h"
#include "Trade.h"
#include "string.h"
#include <time.h>

CTradeSPI::CTradeSPI(CTrade *pTrade)
	:m_pTrade(pTrade),
	m_pTradeAPI(CThostFtdcTraderApi::CreateFtdcTraderApi()),	//��������API
	m_hLoginEvent(NULL),
	m_bLoginSuccess(false),
	m_nRequestID(0),
	m_nOrderRef(0)
{
	ZeroMemory(&m_Config, sizeof(m_Config));
}

CTradeSPI::~CTradeSPI()
{
	//���SPI��ַ
	m_pTradeAPI->RegisterSpi(NULL);
	//Release
	m_pTradeAPI->Release();
}

//=============Initialize=============//
bool CTradeSPI::Initialize(const char *pTradeServer, const char *pTradeLogin, const char *pTradePassword, const char *pTradeBroker)
{

	if (NULL != m_pTradeAPI)
	{
		//����������Ϣ
		strcpy_s(m_Config.TradeServer, pTradeServer);
		strcpy_s(m_Config.InvestorID, pTradeLogin);
		strcpy_s(m_Config.InvestorPassword, pTradePassword);
		strcpy_s(m_Config.BrokerID, pTradeBroker);

		try
		{
			//������¼�¼�
			m_hLoginEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

			if (NULL == m_hLoginEvent)
			{
				cout<<"��¼�¼�����ʧ��" <<endl;
				return false;
			}

			//��¼���
			m_bLoginSuccess = false;


			m_pTradeAPI->RegisterSpi(this);								//ע��ص���SPI
			m_pTradeAPI->SubscribePublicTopic(THOST_TERT_RESTART);		//���Ĺ�����Ϣ
			m_pTradeAPI->SubscribePrivateTopic(THOST_TERT_RESTART);		//����˽����Ϣ
			m_pTradeAPI->RegisterFront(m_Config.TradeServer);			//ע�ύ�׷�����
			m_pTradeAPI->Init();										//��ʼ��

			cout<< "\nCTradeSPI::Initialize::	this:"<<(int)this <<"	TradeAPI:"<<(int)m_pTradeAPI << endl;

			//�ȴ���¼��Ϣ
			WaitForSingleObject(m_hLoginEvent, 5000);

			//�رյ�¼�¼�
			CloseHandle(m_hLoginEvent);
			m_hLoginEvent = NULL;

			//���ص�¼���
			return m_bLoginSuccess;
		}
		catch (...)
		{
			cout<< "CTradeSPI::Initialize::	TradeSPI Initialize �쳣." << endl;
		}
	}

	return false;
}

//=============Event=============//
///����������ɵ���Ӧ�¼�
void CTradeSPI::OnFrontConnected()
{
	if (NULL != m_pTradeAPI)
	{
		///�û���¼����
		CThostFtdcReqUserLoginField req = {0};

		strcpy_s(req.BrokerID, m_Config.BrokerID);
		strcpy_s(req.UserID, m_Config.InvestorID);
		strcpy_s(req.Password, m_Config.InvestorPassword);

		//�û���¼
		m_pTradeAPI->ReqUserLogin(&req, 0);
	}
}

///�Ͽ����ӵ���Ӧ�¼�
void CTradeSPI::OnFrontDisconnected(int nReason)
{

}

///��¼������Ӧ
void CTradeSPI::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (NULL != pRspUserLogin && !IsErrorRspInfo(pRspInfo))
	{//��¼�ɹ�

		m_UserLoginField = *pRspUserLogin;
		SetOrderRef(atoi(pRspUserLogin->MaxOrderRef));

		//���õ�¼���
		m_bLoginSuccess = true;
		//���õ�¼�¼�
		SetEvent(m_hLoginEvent);

		///Ͷ���߽�����ȷ����Ϣ
		CThostFtdcSettlementInfoConfirmField req = {0};

		strcpy_s(req.BrokerID, m_Config.BrokerID);
		strcpy_s(req.InvestorID, m_Config.InvestorID);

		///Ͷ���߽�����ȷ��
		m_pTradeAPI->ReqSettlementInfoConfirm(&req, 0);

	}
	else if (NULL != pRspInfo)
	{
		cout<< "CTradeSPI::OnRspUserLogin::	��¼ʧ��: "<< pRspInfo->ErrorMsg << endl;
	}
}

///�ǳ�������Ӧ
void CTradeSPI::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (NULL != pRspInfo)
	{
		cout<< "CTradeSPI::OnRspUserLogout::	��¼�˳�: "<< pRspInfo->ErrorMsg << endl;
	}
	else
	{
		cout<< "CTradeSPI::OnRspUserLogout::	��¼�˳�."<< endl;
	}
}

///Ͷ���߽�����ȷ����Ӧ
void CTradeSPI::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (NULL != pSettlementInfoConfirm)
	{
		cout<< "CTradeSPI::OnRspSettlementInfoConfirm::	Ͷ����: "<< pSettlementInfoConfirm->InvestorID << endl;
	}

	if (NULL != pRspInfo)
	{
		cout<< "CTradeSPI::OnRspSettlementInfoConfirm::	Ͷ����ȷ��: "<< pRspInfo->ErrorMsg << endl;
	}
	else
	{
		cout<< "CTradeSPI::OnRspSettlementInfoConfirm::	Ͷ����ȷ��."<< endl;
	}
}

///����¼��������Ӧ
void CTradeSPI::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (NULL != m_pTrade)
	{
		m_pTrade->OnRspOrderInsert(pInputOrder, pRspInfo, nRequestID, bIsLast);
	}
}

///��������������Ӧ
void CTradeSPI::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (NULL != m_pTrade)
	{
		m_pTrade->OnRspOrderAction(pInputOrderAction, pRspInfo, nRequestID, bIsLast);
	}
}

///����¼�����ر�
void CTradeSPI::OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo)
{
	if (NULL != m_pTrade)
	{
		m_pTrade->OnErrRtnOrderInsert(pInputOrder, pRspInfo);
	}
}

///������������ر�
void CTradeSPI::OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo)
{
	if (NULL != m_pTrade)
	{
		m_pTrade->OnErrRtnOrderAction(pOrderAction, pRspInfo);
	}
}

///����֪ͨ
void CTradeSPI::OnRtnOrder(CThostFtdcOrderField *pOrder)
{
	if (NULL != m_pTrade)
	{
		m_pTrade->OnRtnOrder(pOrder);
	}
}

///�ɽ�֪ͨ
void CTradeSPI::OnRtnTrade(CThostFtdcTradeField *pTrade)
{
	if (NULL != m_pTrade)
	{
		m_pTrade->OnRtnTrade(pTrade);
	}
}

///����Ӧ��
void CTradeSPI::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (NULL != m_pTrade)
	{
		m_pTrade->OnRspError(pRspInfo, nRequestID, bIsLast);
	}
}

///�����ѯ��Լ��Ӧ
void CTradeSPI::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	map<HANDLE, SEventParameter>::iterator itmapEventParameter = m_mapEventParameter.find((HANDLE)nRequestID);

	if (m_mapEventParameter.end() != itmapEventParameter)
	{
		if (NULL != pInstrument && !IsErrorRspInfo(pRspInfo))
		{
			m_mapInstrumentFieldTemp[nRequestID].push_back(*pInstrument);
		}

		if (true == bIsLast)
		{	
			itmapEventParameter->second.bSuccess = true;
			SetEvent(itmapEventParameter->first);
		}
	}
}

///�����ѯ������Ӧ
void CTradeSPI::OnRspQryDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	map<HANDLE, SEventParameter>::iterator itmapEventParameter = m_mapEventParameter.find((HANDLE)nRequestID);

	if (m_mapEventParameter.end() != itmapEventParameter)
	{
		if (NULL != pDepthMarketData && !IsErrorRspInfo(pRspInfo))
		{
			m_mapMarketDataFieldTemp[nRequestID].push_back(*pDepthMarketData);
		}

		if (true == bIsLast)
		{	
			itmapEventParameter->second.bSuccess = true;
			SetEvent(itmapEventParameter->first);
		}
	}
}

///�����ѯ������Ӧ
void CTradeSPI::OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	map<HANDLE, SEventParameter>::iterator itParameter = m_mapEventParameter.find((HANDLE)nRequestID);

	if (m_mapEventParameter.end() != itParameter)
	{
		if (NULL != pOrder && !IsErrorRspInfo(pRspInfo))
		{
			m_mapOrderTemp[nRequestID].push_back(*pOrder);
		}

		if (true == bIsLast)
		{
			itParameter->second.bSuccess = true;
			SetEvent(itParameter->first);
		}
	}
}

///�����ѯͶ���ֲ߳���Ӧ
void CTradeSPI::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	map<HANDLE, SEventParameter>::iterator itParameter = m_mapEventParameter.find((HANDLE)nRequestID);

	if (m_mapEventParameter.end() != itParameter)
	{
		if (NULL != pInvestorPosition && !IsErrorRspInfo(pRspInfo))
		{
			m_mapPositionTemp[nRequestID].push_back(*pInvestorPosition);
		}

		if (true == bIsLast)
		{
			itParameter->second.bSuccess = true;
			SetEvent(itParameter->first);
		}
	}
}

///�����ѯ�ʽ��˻���Ӧ
void CTradeSPI::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	map<HANDLE, SEventParameter>::iterator itParameter = m_mapEventParameter.find((HANDLE)nRequestID);

	if (m_mapEventParameter.end() != itParameter)
	{
		if (NULL != pTradingAccount && !IsErrorRspInfo(pRspInfo))
		{
			m_mapAccountTemp[nRequestID].push_back(*pTradingAccount);
		}

		if (true == bIsLast)
		{
			itParameter->second.bSuccess = true;
			SetEvent(itParameter->first);
		}
	}
}

//=============Common=============//
///�Ƿ��Ǵ�����Ϣ
bool CTradeSPI::IsErrorRspInfo(IN CThostFtdcRspInfoField *pRspInfo)
{
	return (NULL != pRspInfo && 0 != pRspInfo->ErrorID);
}

int CTradeSPI::GetRequestID()
{
	return ++m_nRequestID;
}

void CTradeSPI::SetOrderRef(IN int nOrderRef)
{
	m_nOrderRef = nOrderRef;
}

string CTradeSPI::GetOrderRef()
{
	char OrderRef[100] = {0};

	sprintf_s(OrderRef, "%d", ++m_nOrderRef);

	return OrderRef;
}

string CTradeSPI::GetCTPString(IN const char *pString, IN int nLength)
{
	string strString;

	if (0 < nLength)
	{
		char *pStringTemp = new char[nLength + 1];
		ZeroMemory(pStringTemp, nLength + 1);
		memcpy(pStringTemp, pString, nLength);
		strString = pStringTemp;
		F_SafeDeleteArray(pStringTemp);
	}

	return strString;
}

//=============Request=============//
bool CTradeSPI::OrderInsert(IN COrder &Order)
{
	if (NULL != m_pTradeAPI)
	{
		CThostFtdcInputOrderField Field = {0};

		if (true == FillInputOrderFieldByOrder(Field, Order))
		{
			return 0 == m_pTradeAPI->ReqOrderInsert(&Field, 0);
		}
	}

	return false;
}

bool CTradeSPI::OrderDelete(IN const COrder &Order)
{
	if (NULL != m_pTradeAPI)
	{
		CThostFtdcInputOrderActionField Feild = {0};

		if (true == FillInputOrderActionFieldByOrder(Feild, Order))
		{
			return 0 == m_pTradeAPI->ReqOrderAction(&Feild, 0);
		}
	}

	return false;
}
//=============Get=============//
bool CTradeSPI::GetInstrument(IN vector<CThostFtdcInstrumentField> &vecInstrumentField)
{
	vecInstrumentField.clear();

	SEventParameter EParameter = {0};
	EParameter.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	if (NULL != EParameter.hEvent)
	{
		CThostFtdcQryInstrumentField Field = {0};

		m_mapEventParameter[EParameter.hEvent] = EParameter;

		if (!m_pTradeAPI->ReqQryInstrument(&Field, (int)m_mapEventParameter[EParameter.hEvent].hEvent))
		{
			WaitForSingleObject(m_mapEventParameter[EParameter.hEvent].hEvent, T_WaitForSingleSpan);
		}


		if (true == m_mapEventParameter[EParameter.hEvent].bSuccess)
		{
			map<int, vector<CThostFtdcInstrumentField>>::iterator itmapInstrumentField = m_mapInstrumentFieldTemp.find((int)EParameter.hEvent);

			if (m_mapInstrumentFieldTemp.end() != itmapInstrumentField)
			{
				vecInstrumentField.assign(itmapInstrumentField->second.begin(), itmapInstrumentField->second.end());
			}
		}

		EParameter.bSuccess = m_mapEventParameter[EParameter.hEvent].bSuccess;
		m_mapEventParameter.erase(EParameter.hEvent);
		CloseHandle(EParameter.hEvent);
		EParameter.hEvent = NULL;
		m_mapInstrumentFieldTemp.clear();

	}

	return EParameter.bSuccess;
}

bool CTradeSPI::GetMarketData(IN vector<CThostFtdcDepthMarketDataField> &vecMarketDataField)
{
	vecMarketDataField.clear();

	SEventParameter EParameter = {0};
	EParameter.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	if (NULL != EParameter.hEvent)
	{
		CThostFtdcQryDepthMarketDataField Field = {0};

		m_mapEventParameter[EParameter.hEvent] = EParameter;

		if (!m_pTradeAPI->ReqQryDepthMarketData(&Field, (int)m_mapEventParameter[EParameter.hEvent].hEvent))
		{
			WaitForSingleObject(m_mapEventParameter[EParameter.hEvent].hEvent, T_WaitForSingleSpan);
		}

		if (true == m_mapEventParameter[EParameter.hEvent].bSuccess)
		{
			map<int, vector<CThostFtdcDepthMarketDataField>>::iterator itmapMarketDataField = m_mapMarketDataFieldTemp.find((int)EParameter.hEvent);

			if (m_mapMarketDataFieldTemp.end() != itmapMarketDataField)
			{
				vecMarketDataField.assign(itmapMarketDataField->second.begin(), itmapMarketDataField->second.end());
			}
		}

		EParameter.bSuccess = m_mapEventParameter[EParameter.hEvent].bSuccess;
		m_mapEventParameter.erase(EParameter.hEvent);
		CloseHandle(EParameter.hEvent);
		EParameter.hEvent = NULL;
		m_mapMarketDataFieldTemp.clear();
	}

	return EParameter.bSuccess;;
}

bool CTradeSPI::GetSymbol(IN map<string, CSymbol> &mapSymbol)
{
	map<string, CSymbol> mapSymbolTemp;
	vector<CThostFtdcInstrumentField> vecInstrumentField;
	vector<CThostFtdcDepthMarketDataField> vecMarketDataField;

	if (true == GetInstrument(vecInstrumentField))
	{
		for (vector<CThostFtdcInstrumentField>::iterator itInstrumentField = vecInstrumentField.begin(); vecInstrumentField.end() != itInstrumentField; ++itInstrumentField)
		{
			CSymbol Symbol;

			FillSymbolByInstrument(Symbol, *itInstrumentField);

			mapSymbolTemp[Symbol.GetSymbol().c_str()] = Symbol;
		}

		Sleep(1100);

		if (true == GetMarketData(vecMarketDataField))
		{
			for (vector<CThostFtdcDepthMarketDataField>::iterator itMarketDataField = vecMarketDataField.begin(); vecMarketDataField.end() != itMarketDataField; ++itMarketDataField)
			{
				map<string, CSymbol>::iterator itSymbol = mapSymbolTemp.find(itMarketDataField->InstrumentID);

				if (mapSymbolTemp.end() != itSymbol)
				{
					FillSymbolByMarketData(itSymbol->second, *itMarketDataField);

					mapSymbol[itSymbol->first] = itSymbol->second;
				}
			}

			return true;
		}
	}

	return false;
}

bool CTradeSPI::GetOrder(OUT vector<COrder> &vecOrder)
{
	vecOrder.clear();
	SEventParameter EventParameter = {0};
	EventParameter.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	if (NULL != EventParameter.hEvent)
	{
		m_mapEventParameter[EventParameter.hEvent] = EventParameter;

		CThostFtdcQryOrderField Field = {0};

		strncpy_s(Field.BrokerID, m_Config.BrokerID, sizeof(Field.BrokerID)/sizeof(Field.BrokerID[0]));
		strncpy_s(Field.InvestorID, m_Config.InvestorID, sizeof(Field.InvestorID)/sizeof(Field.InvestorID[0]));

		if (!m_pTradeAPI->ReqQryOrder(&Field, (int)m_mapEventParameter[EventParameter.hEvent].hEvent))
		{
			WaitForSingleObject(m_mapEventParameter[EventParameter.hEvent].hEvent, T_WaitForSingleSpan);
		}

		if (true == m_mapEventParameter[EventParameter.hEvent].bSuccess)
		{
			map<int, vector<CThostFtdcOrderField>>::iterator itmapOrder = m_mapOrderTemp.find((int)EventParameter.hEvent);

			if (m_mapOrderTemp.end() != itmapOrder)
			{
				for (vector<CThostFtdcOrderField>::iterator itOrderField = itmapOrder->second.begin(); itmapOrder->second.end() != itOrderField; ++itOrderField)
				{
					COrder Order;
					FillOrderByOrderField(Order, *itOrderField);
					vecOrder.push_back(Order);
				}
			}
		}

		EventParameter.bSuccess = m_mapEventParameter[EventParameter.hEvent].bSuccess;
		m_mapEventParameter.erase(EventParameter.hEvent);
		CloseHandle(EventParameter.hEvent);
		EventParameter.hEvent = NULL;
		m_mapOrderTemp.clear();
	}

	return EventParameter.bSuccess;
}

bool CTradeSPI::GetPosition(OUT vector<CPosition> &vecPosition)
{
	vecPosition.clear();
	SEventParameter EventParameter = {0};
	EventParameter.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	if (NULL != EventParameter.hEvent)
	{
		m_mapEventParameter[EventParameter.hEvent] = EventParameter;

		CThostFtdcQryInvestorPositionField Field = {0};

		strncpy_s(Field.BrokerID, m_Config.BrokerID, sizeof(Field.BrokerID)/sizeof(Field.BrokerID[0]));
		strncpy_s(Field.InvestorID, m_Config.InvestorID, sizeof(Field.InvestorID)/sizeof(Field.InvestorID[0]));

		if (!m_pTradeAPI->ReqQryInvestorPosition(&Field, (int)(m_mapEventParameter[EventParameter.hEvent].hEvent)))
		{
			WaitForSingleObject(m_mapEventParameter[EventParameter.hEvent].hEvent, T_WaitForSingleSpan);
		}

		if (true == m_mapEventParameter[EventParameter.hEvent].bSuccess)
		{
			map<int, vector<CThostFtdcInvestorPositionField>>::iterator itmapPosition = m_mapPositionTemp.find((int)EventParameter.hEvent);

			if (m_mapPositionTemp.end() != itmapPosition)
			{
// 				map<string, CPosition> mapPositionTemp;
// 
// 				for (vector<CThostFtdcInvestorPositionField>::iterator itPositionField = itmapPosition->second.begin(); itmapPosition->second.end() != itPositionField; ++itPositionField)
// 				{
// 					map<string, CPosition>::iterator itPosition = mapPositionTemp.find(itPositionField->InstrumentID);
// 
// 					CPosition Position;
// 
// 					FillPositionByPositionField(Position, *itPositionField);
// 
// 					if (mapPositionTemp.end() != itPosition)
// 					{
// 						mapPositionTemp[Position.InstrumentID] = Position;
// 					}
// 					else
// 					{
// 						if (itPosition->second.GetPositionType() == Position.GetPositionType())
// 						{//������ͬ,��λ���
// 							itPosition->second.SetVolume(itPosition->second.GetVolume() + Position.GetVolume());
// 							itPosition->second.SetTodayVolume(itPosition->second.GetTodayVolume() + Position.GetTodayVolume());
// 							itPosition->second.SetYesterdayVolume(itPosition->second.GetYesterdayVolume() + Position.GetYesterdayVolume());
// 							itPosition->second.SetPositionCost(itPosition->second.GetPositionCost() + Position.GetPositionCost());
// 							itPosition->second.SetOpenCost(itPosition->second.GetOpenCost() + Position.GetOpenCost());
// 						}
// 
// 					}
// 
// 				}
// 
// 				for (map<string, CPosition>::iterator itPosition = mapPositionTemp.begin(); mapPositionTemp.end() != itPosition; ++itPosition)
// 				{
// 					vecPosition.push_back(itPosition->second);
// 				}

				for (vector<CThostFtdcInvestorPositionField>::iterator itPositionField = itmapPosition->second.begin(); itmapPosition->second.end() != itPositionField; ++itPositionField)
				{
					vecPosition.push_back(*itPositionField);
				}
			}
		}

		EventParameter.bSuccess = m_mapEventParameter[EventParameter.hEvent].bSuccess;
		m_mapEventParameter.erase(EventParameter.hEvent);
		CloseHandle(EventParameter.hEvent);
		EventParameter.hEvent = NULL;
		m_mapPositionTemp.clear();		
	}

	return EventParameter.bSuccess;
}

bool CTradeSPI::GetAccount(OUT CAccount &Account)
{
	SEventParameter EventParameter = {0};
	EventParameter.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	if (NULL != EventParameter.hEvent)
	{
		m_mapEventParameter[EventParameter.hEvent] = EventParameter;

		CThostFtdcQryTradingAccountField Field = {0};

		strncpy_s(Field.BrokerID, m_Config.BrokerID, sizeof(Field.BrokerID)/sizeof(Field.BrokerID[0]));
		strncpy_s(Field.InvestorID, m_Config.InvestorID, sizeof(Field.InvestorID)/sizeof(Field.InvestorID[0]));

		if (!m_pTradeAPI->ReqQryTradingAccount(&Field, (int)m_mapEventParameter[EventParameter.hEvent].hEvent))
		{
			WaitForSingleObject(m_mapEventParameter[EventParameter.hEvent].hEvent, T_WaitForSingleSpan);
		}

		if (true == m_mapEventParameter[EventParameter.hEvent].bSuccess)
		{
			map<int, vector<CThostFtdcTradingAccountField>>::iterator itmapAccount = m_mapAccountTemp.find((int)EventParameter.hEvent);

			if (m_mapAccountTemp.end() != itmapAccount)
			{
				for (vector<CThostFtdcTradingAccountField>::iterator itAccountField = itmapAccount->second.begin(); itmapAccount->second.end() != itAccountField; ++itAccountField)
				{
					//CAccount Account;

					FillAccountByTradingAccountField(Account, *itAccountField);

					break;
				}
			}
		}

		EventParameter.bSuccess = m_mapEventParameter[EventParameter.hEvent].bSuccess;
		m_mapEventParameter.erase(EventParameter.hEvent);
		CloseHandle(EventParameter.hEvent);
		EventParameter.hEvent = NULL;
		m_mapAccountTemp.clear();

	}

	return EventParameter.bSuccess;
}

int CTradeSPI::GetOrderRef_Int()
{
	return ++m_nOrderRef;
}

int CTradeSPI::GetFrontID()
{
	return m_UserLoginField.FrontID;
}

int CTradeSPI::GetSessionID()
{
	return m_UserLoginField.SessionID;
}

string CTradeSPI::GetInvestorID()
{
	return m_UserLoginField.UserID;
}

string CTradeSPI::GetLoginTime()
{
	char Time[50] = {0};
	strncpy_s(Time, m_UserLoginField.LoginTime, sizeof(m_UserLoginField.LoginTime));

	return Time;
}

string CTradeSPI::GetLoginDate()
{
	char Date[50] = {0};
	strncpy_s(Date, m_UserLoginField.TradingDay, sizeof(m_UserLoginField.TradingDay));

	return Date;
}

//=============Fill=============//
bool CTradeSPI::FillSymbolByInstrument(OUT CSymbol &Symbol, IN const CThostFtdcInstrumentField &InstrumentField)
{
	Symbol.SetSymbol(GetCTPString(InstrumentField.InstrumentID, F_GetLength(InstrumentField.InstrumentID)));
	Symbol.SetExchangeName(GetCTPString(InstrumentField.ExchangeID, F_GetLength(InstrumentField.ExchangeID)));
	Symbol.SetLongMarginRate(MIN(InstrumentField.LongMarginRatio, 1000000));
	Symbol.SetShortMarginRate(MIN(InstrumentField.ShortMarginRatio, 1000000));
	Symbol.SetContractSize(InstrumentField.VolumeMultiple);
	Symbol.SetTickSize(InstrumentField.PriceTick);
	Symbol.SetMaxVolume(InstrumentField.MaxLimitOrderVolume);
	Symbol.SetMinVolume(InstrumentField.MinLimitOrderVolume);
	Symbol.SetVolumeStep(1);

	return true;
}

bool CTradeSPI::FillSymbolByMarketData(OUT CSymbol &Symbol, IN const CThostFtdcDepthMarketDataField &MarketDataField)
{
	Symbol.SetSymbol(GetCTPString(MarketDataField.InstrumentID, F_GetLength(MarketDataField.InstrumentID)));
	Symbol.SetUpperLimitPrice(MarketDataField.UpperLimitPrice);
	Symbol.SetLowerLimitPrice(MarketDataField.LowerLimitPrice);

	return true;
}

bool CTradeSPI::FillInputOrderFieldByOrder(OUT CThostFtdcInputOrderField &InputOrderField, IN const COrder &Order)
{
	ZeroMemory(&InputOrderField, sizeof(InputOrderField));

	///��Լ����
	strncpy_s(InputOrderField.InstrumentID, Order.GetSymbol().c_str(), F_GetLength(InputOrderField.InstrumentID));
	///���͹�˾����
	strncpy_s(InputOrderField.BrokerID, m_Config.BrokerID, F_GetLength(InputOrderField.BrokerID));
	///Ͷ���ߴ���
	strncpy_s(InputOrderField.InvestorID, m_Config.InvestorID, F_GetLength(InputOrderField.InvestorID));
	///�û�����
	strncpy_s(InputOrderField.UserID, m_UserLoginField.UserID, F_GetLength(InputOrderField.UserID));
	///��������
	strncpy_s(InputOrderField.OrderRef, GetOrderRef().c_str(), F_GetLength(InputOrderField.OrderRef));
	if (0 != Order.GetOrderID())
	{
		sprintf_s(InputOrderField.OrderRef, "%d", Order.GetOrderID());
	}

	///��/������
	switch (Order.GetType())
	{
	case IOrder::EOrderType::OrderTypeBuy:
		{
			InputOrderField.Direction = THOST_FTDC_D_Buy;
		}
		break;
	case IOrder::EOrderType::OrderTypeSell:
		{
			InputOrderField.Direction = THOST_FTDC_D_Sell;
		}
		break;
	default:
		break;
	}

	///��ƽ��־
	switch (Order.GetFlags())
	{
	case IOrder::EOrderFlags::OrderFlagsOpen:
		{
			InputOrderField.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
		}
		break;
	case IOrder::EOrderFlags::OrderFlagsClose:
		{
			if (true == Order.GetIsCloseYesterday())
			{
				InputOrderField.CombOffsetFlag[0] = THOST_FTDC_OF_Close;
			}
			else
			{
				InputOrderField.CombOffsetFlag[0] = THOST_FTDC_OF_CloseToday;
			}
		}
		break;
	default:
		break;
	}
	///�����۸�����: �޼�
	InputOrderField.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
	///���Ͷ���ױ���־
	InputOrderField.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
	///�۸�
	InputOrderField.LimitPrice = Order.GetPrice();
	///ֹ���
	InputOrderField.StopPrice = Order.GetSLPrice();
	///��/������
	InputOrderField.VolumeTotalOriginal = (int)Order.GetVolume();
	///��Ч������: ������Ч
	InputOrderField.TimeCondition = THOST_FTDC_TC_GFD;
	///�ɽ�������: �κ�����
	InputOrderField.VolumeCondition = THOST_FTDC_VC_AV;
	///��С�ɽ���: 1(�����ǵ�ǰ����)
	InputOrderField.MinVolume = 1;
	///��������: ����
	InputOrderField.ContingentCondition = THOST_FTDC_CC_Immediately;		
	///ǿƽԭ��: ��ǿƽ
	InputOrderField.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	///�Զ������־: ��
	InputOrderField.IsAutoSuspend = 0;
	///�û�ǿ����־: ��
	InputOrderField.UserForceClose = 0;

	return true;
}

bool CTradeSPI::FillInputOrderActionFieldByOrder(OUT CThostFtdcInputOrderActionField &InputOrderActionField, IN const COrder &Order)
{
	//��Լ
	strncpy_s(InputOrderActionField.InstrumentID, Order.GetSymbol().c_str(), F_GetLength(InputOrderActionField.InstrumentID));
	//���͹�˾����
	strncpy_s(InputOrderActionField.BrokerID, m_UserLoginField.BrokerID, F_GetLength(InputOrderActionField.BrokerID));
	//Ͷ���ߴ���
	strncpy_s(InputOrderActionField.InvestorID, m_Config.InvestorID, F_GetLength(InputOrderActionField.InvestorID));
	//��������
	sprintf_s(InputOrderActionField.OrderRef, "%d", Order.GetOrderID());
	//�û�����
	strncpy_s(InputOrderActionField.UserID, m_Config.InvestorID, F_GetLength(InputOrderActionField.UserID));
	//ǰ�÷��������
	InputOrderActionField.FrontID = 0 == Order.GetFrontID()?m_UserLoginField.FrontID:Order.GetFrontID();
	//�Ự���
	InputOrderActionField.SessionID = 0 == Order.GetSession()?m_UserLoginField.SessionID:Order.GetSession();
	//������־
	InputOrderActionField.ActionFlag = THOST_FTDC_AF_Delete;	

	return true;
}

bool CTradeSPI::FillOrderByOrderField(OUT COrder &Order, IN const CThostFtdcOrderField &OrderField)
{
	Order.SetOrderID(atoi(OrderField.OrderRef));
	Order.SetSymbol(GetCTPString(OrderField.InstrumentID, sizeof(OrderField.InstrumentID)/sizeof(OrderField.InstrumentID[0])));
	Order.SetVolume(OrderField.VolumeTotal);
	Order.SetInitVolume(OrderField.VolumeTotalOriginal);
	Order.SetRemaindVolume(OrderField.VolumeTotal);
	Order.SetPrice(OrderField.LimitPrice);
	Order.SetSLPrice(OrderField.StopPrice);
	Order.SetFrontID(OrderField.FrontID);
	Order.SetSession(OrderField.SessionID);
	Order.SetComment(GetCTPString(OrderField.StatusMsg, F_GetLength(OrderField.StatusMsg)));
	Order.SetOrderLocalID(OrderField.OrderLocalID);


	//��/��
	switch (OrderField.Direction)
	{
	case THOST_FTDC_D_Buy:
		{
			Order.SetType(IOrder::EOrderType::OrderTypeBuy);
		}
		break;
	case THOST_FTDC_D_Sell:
		{
			Order.SetType(IOrder::EOrderType::OrderTypeSell);
		}
		break;
	default:
		break;
	}

	//��/ƽ
	switch (OrderField.CombOffsetFlag[0])
	{
	case THOST_FTDC_OF_Open:
		{
			Order.SetFlags(IOrder::EOrderFlags::OrderFlagsOpen);
		}
		break;
	case THOST_FTDC_OF_Close:
	case THOST_FTDC_OF_CloseYesterday:
		{
			//�����Ƿ���ƽ�򱨵�
			Order.SetIsCloseYesterday(true);
		}
	case THOST_FTDC_OF_ForceClose:
	case THOST_FTDC_OF_CloseToday:	
	case THOST_FTDC_OF_ForceOff:
	case THOST_FTDC_OF_LocalForceClose:
		{
			Order.SetFlags(IOrder::EOrderFlags::OrderFlagsClose);
		}
		break;
	default:
		break;
	}

	//״̬
	switch (OrderField.OrderSubmitStatus)
	{
	case THOST_FTDC_OSS_InsertSubmitted:
	case THOST_FTDC_OSS_CancelSubmitted:
	case THOST_FTDC_OSS_ModifySubmitted:
		{
			Order.SetStatus(IOrder::EOrderStatus::OrderStatusSubmitted);
		}
		break;
	case THOST_FTDC_OSS_Accepted:
		{
			Order.SetStatus(IOrder::EOrderStatus::OrderStatusAccepted);
		}
		break;
	case THOST_FTDC_OSS_InsertRejected:
	case THOST_FTDC_OSS_CancelRejected:
	case THOST_FTDC_OSS_ModifyRejected:
		{
			Order.SetStatus(IOrder::EOrderStatus::OrderStatusRejected);
		}
		break;
	default:
		break;
	}


	//���
	switch (OrderField.OrderStatus)
	{
	case THOST_FTDC_OST_AllTraded:
		{
			Order.SetResult(IOrder::EOrderResult::OrderResultAllTraded);
		}
		break;
	case THOST_FTDC_OST_PartTradedQueueing:
	case THOST_FTDC_OST_NoTradeQueueing:
		{
			Order.SetResult(IOrder::EOrderResult::OrderResultTrading);
		}
		break;
	case THOST_FTDC_OST_PartTradedNotQueueing:	
	case THOST_FTDC_OST_NoTradeNotQueueing:
		{
			Order.SetResult(IOrder::EOrderResult::OrderResultNotQueueing);
		}
		break;
	case THOST_FTDC_OST_Canceled:
		{
			Order.SetResult(IOrder::EOrderResult::OrderResultCanceled);
		}
		break;
	case THOST_FTDC_OST_Unknown:
	default:
		break;
	}


	return true;
}

bool CTradeSPI::FillPositionByPositionField(OUT CPosition &Position, IN const CThostFtdcInvestorPositionField &PositionField)
{
	Position = PositionField;

	return true;
}

bool CTradeSPI::FillAccountByTradingAccountField(OUT CAccount &Account, IN const CThostFtdcTradingAccountField &AccountField)
{
	Account = AccountField;

	return true;
}