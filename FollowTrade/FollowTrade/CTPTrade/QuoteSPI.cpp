#include "stdafx.h"
#include "QuoteSPI.h"
#include "Quote.h"

CQuoteSPI::CQuoteSPI(CQuote *pQuote)
	:m_pQuote(pQuote),
	m_pQuoteAPI(CThostFtdcMdApi::CreateFtdcMdApi())	//��������API
{

}

CQuoteSPI::~CQuoteSPI()
{
	//���SPI��ַ
	m_pQuoteAPI->RegisterSpi(NULL);
	//Release
	m_pQuoteAPI->Release();
}

///��ʼ��
bool CQuoteSPI::Initialize(string strQuoteServer)
{
	char QuoteServer[1000] = {0};
	strcpy_s(QuoteServer, strQuoteServer.c_str());

	m_pQuoteAPI->RegisterSpi(this);				//ע��ص���SPI
	m_pQuoteAPI->RegisterFront(QuoteServer);	//ע�������������ַ
	m_pQuoteAPI->Init();						//��ʼ��
	m_Thread.Start(ProcessThreadWrapper, this, 262144);

	return true;
}

void CQuoteSPI::SetSymbol(const vector<CSymbol> &vecSymbol)
{
	m_vecSymbol.clear();
	m_vecSymbol.assign(vecSymbol.begin(), vecSymbol.end());
}

bool CQuoteSPI::Subscribe()
{
	int nResult(0);

	int size = m_vecSymbol.size();
	char **pInstrumentIds = new char*[size];

	for(int i=0;i<size;++i)
	{
		int num = m_vecSymbol.at(i).GetSymbol().length();
		pInstrumentIds[i] = new char[num+1];

		memset(pInstrumentIds[i], 0, num + 1);
		memcpy(pInstrumentIds[i], m_vecSymbol.at(i).GetSymbol().c_str(), num);
	}

	nResult = m_pQuoteAPI->SubscribeMarketData(pInstrumentIds,size);

	for(int i=0;i<size;++i)
	{
		delete[] pInstrumentIds[i];
		pInstrumentIds[i] = NULL;
	}

	delete[] pInstrumentIds;
	pInstrumentIds = NULL;

	return 0 == nResult;
}

///����������ɵ���Ӧ�¼�
void CQuoteSPI::OnFrontConnected()
{
	CThostFtdcReqUserLoginField req = {0};
	m_pQuoteAPI->ReqUserLogin(&req,0);
}

///��¼������Ӧ
void CQuoteSPI::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	Subscribe();
}

///�������֪ͨ
void CQuoteSPI::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	if (NULL != m_pQuote && NULL != pDepthMarketData)
	{
		m_pQuote->OnTick(pDepthMarketData);
	}
}

//=========================thread=========================//
UINT __stdcall CQuoteSPI::ProcessThreadWrapper(LPVOID param)
{
	CQuoteSPI *pThis=reinterpret_cast<CQuoteSPI*>(param);
	if(pThis!=NULL) pThis->ProcessThread();

	return 0;
}

void CQuoteSPI::ProcessThread()
{
	m_pQuoteAPI->Join();
}