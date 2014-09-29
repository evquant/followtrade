#pragma once
#include <windows.h>
#include <string>

using namespace std;

class CSymbol
{
public:
	CSymbol();
	virtual ~CSymbol();

public://Assign
	virtual void Assign(IN const CSymbol &Symbol);
	virtual void Clear();

public://Mumber
	virtual void SetSymbol(IN string strSymbol);						//���ý�������Լ
	virtual string GetSymbol() const;									//��ȡ��������Լ
	virtual void SetExchangeName(IN string strExchangeName);			//���ý���������
	virtual string GetExchangeName() const;								//��ȡ����������
	virtual void SetContractSize(IN double dContractSize);				//���ú�Լ��С
	virtual double GetContractSize() const;								//��ȡ��Լ��С
	virtual void SetTickSize(IN double dTickSize);						//����Tick��С(�۸���С����)
	virtual double GetTickSize() const;									//��ȡTick��С(�۸���С����)
	virtual void SetMaxVolume(IN double dMaxVolume);					//������������
	virtual double GetMaxVolume() const;								//��ȡ��������
	virtual void SetMinVolume(IN double dMinVolume);					//������С������
	virtual double GetMinVolume() const;								//��ȡ��С������
	virtual void SetVolumeStep(IN double dVolumeStep);					//����������С����
	virtual double GetVolumeStep() const;								//��ȡ������С����
	virtual void SetLongMarginRate(IN double dLongMarginRate);			//���ö�ֵı�֤����
	virtual double GetLongMarginRate() const;							//��ȡ��ֵı�֤����
	virtual void SetShortMarginRate(IN double dShortMarginRate);		//���ÿղֵı�֤����
	virtual double GetShortMarginRate() const;							//��ȡ�ղֵı�֤����
	virtual void SetUpperLimitPrice(IN double dUpperLimitPrice);		//������ͣ��
	virtual double GetUpperLimitPrice() const;							//��ȡ��ͣ��
	virtual void SetLowerLimitPrice(IN double dLowerLimitPrice);		//���õ�ͣ��
	virtual double GetLowerLimitPrice() const;							//��ȡ��ͣ��

private:
	string m_strSymbol;				//��������Լ
	string m_strExchangeName;		//����������
	double m_dContractSize;			//��Լ��С
	double m_dTickSize;				//Tick��С(�۸���С����)
	double m_dMaxVolume;			//��������
	double m_dMinVolume;			//��С������
	double m_dVolumeStep;			//������С����
	double m_dLongMarginRate;		//��ֵı�֤����
	double m_dShortMarginRate;		//�ղֵı�֤����
	double m_dUpperLimitPrice;		//��ͣ��
	double m_dLowerLimitPrice;		//��ͣ��
};