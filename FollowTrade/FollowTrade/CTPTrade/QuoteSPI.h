#pragma once
#include "CTPAPI/ThostFtdcMdApi.h"
#include "CTPAPI/ThostFtdcUserApiDataType.h"
#include "CTPAPI/ThostFtdcUserApiStruct.h"
#include "Entity/Entity.h"
#include "Thread/Thread.h"
#include <vector>

using namespace std;

class CQuote;

class CQuoteSPI: public CThostFtdcMdSpi
{
public:
	CQuoteSPI(CQuote *pQuote);
	virtual ~CQuoteSPI();

public:
	///��ʼ��
	bool Initialize(string strQuoteServer);

public:
	void SetSymbol(const vector<CSymbol> &vecSymbol);
	bool Subscribe();

private:
	///����������ɵ���Ӧ�¼�
	void OnFrontConnected();
	///��¼������Ӧ
	void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	///�������֪ͨ
	void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);

private://thread of messages processing
	static UINT __stdcall ProcessThreadWrapper(IN LPVOID param);
	void ProcessThread(void);
private://thread of messages processing
	CThread m_Thread;	
	volatile long m_Thread_workflag;	//flag of processing thread

private:
	vector<CSymbol> m_vecSymbol;
private:
	CQuote *m_pQuote;				//������
	CThostFtdcMdApi *m_pQuoteAPI;	//����API
};
