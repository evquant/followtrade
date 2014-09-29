#pragma once
#include <string>
#include <vector>
#include "QuoteSPI.h"
#include "Entity/Entity.h"
#include "Entity/Interface/IQuote.h"

using namespace std;

class CQuote
{
public:
	CQuote(IQuote *pQuoteInterface = NULL);
	virtual ~CQuote();

public:
	///��ʼ��
	bool Initialize(string strQuoteServer);
	//���ú�Լ
	void SetSymbol(const vector<CSymbol> &vecSymbol);

public://CallBack
	//Tick�ص��¼�
	void OnTick(const CThostFtdcDepthMarketDataField *pDepthMarketData);

private:
	CQuoteSPI m_QuoteSPI;	//����SPI����
private:
	IQuote *m_pQuoteInterface;
};