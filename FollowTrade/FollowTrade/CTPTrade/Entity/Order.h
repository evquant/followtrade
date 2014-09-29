#pragma once
#include <string>
#include "Interface/IOrder.h"

using namespace std;

class COrder: public IOrder
{
public:
	COrder();
	virtual ~COrder();

public://Assign
	virtual void Assign( const IOrder &Order);
	virtual void Clear();

public://Mumber
	virtual void SetOrderID( int nOrderID);					//���õ���
	virtual int GetOrderID() const;								//��ȡ����
	virtual void SetSymbol( string strSymbol);					//����MT��Լ
	virtual string GetSymbol() const;								//��ȡMT��Լ
	virtual void SetOrderLocalID(string strOrderLocalID);			//����������ID
	virtual string GetOrderLocalID() const;							//����������DI
	virtual void SetVolume( double dVolume);						//���õ�ǰ��
	virtual double GetVolume() const;								//��ȡ��ǰ��
	virtual void SetInitVolume( double dInitVolume);				//���ó�ʼ��
	virtual double GetInitVolume() const;							//��ȡ��ʼ��
	virtual void SetRemaindVolume( double dRemaindVolume);		//����������
	virtual double GetRemaindVolume() const;						//��ȡ������
	virtual void SetPrice( double dPrice);						//���ñ���
	virtual double GetPrice() const;								//��ȡ����
	virtual void SetSLPrice( double dSLPrice);					//����ֹ���
	virtual double GetSLPrice() const;								//��ȡֹ���
	virtual void SetTPPrice( double dTPPrice);					//����ֹӯ��
	virtual double GetTPPrice() const;								//��ȡֹӯ��
	virtual void SetInsertTime( long long lInsertTime);			//���ñ���ʱ��
	virtual long long GetInsertTime() const;						//��ȡ����ʱ��
	virtual void SetIsCloseYesterday( bool bIsCloseYesterday);	//�����Ƿ�ƽ��
	virtual bool GetIsCloseYesterday() const;						//��ȡ�Ƿ�ƽ��
	virtual void SetFrontID( int nFrontID);						//����ǰ�÷�����ID
	virtual int GetFrontID() const;									//��ȡǰ�÷�����ID
	virtual void SetSession( int nSession);						//���ûỰID
	virtual int GetSession() const;									//��ȡ�ỰID
	virtual void SetComment( string strComment);					//���ñ�ע
	virtual string GetComment() const;								//��ȡ��ע
	virtual void SetType( EOrderType eType);						//��������
	virtual EOrderType GetType() const;								//��ȡ����
	virtual void SetAction( EOrderAction eAction);				//���ö���
	virtual EOrderAction GetAction() const;							//��ȡ����
	virtual void SetFlags( EOrderFlags eFlags);					//���ñ�־
	virtual EOrderFlags GetFlags() const;							//��ȡ��־
	virtual void SetStatus( EOrderStatus eStatus);				//����״̬
	virtual EOrderStatus GetStatus() const;							//��ȡ״̬
	virtual void SetResult( EOrderResult eResult);				//���ý��
	virtual EOrderResult GetResult() const;							//��ȡ���

private:
	int m_nOrderID;					//����
	string m_strExchangeID;				//����(������)ID
	string m_strSymbol;					//��Լ
	string m_strOrderLocalID;			//����������ID
	double m_dVolume;					//��ǰ��
	double m_dInitVolume;				//��ʼ��
	double m_dRemainVolume;				//������
	double m_dPrice;					//����
	double m_dSLPrice;					//ֹ���
	double m_dTPPrice;					//ֹӯ��
	long long m_lInsertTime;			//����ʱ��
	bool m_bIsCloseYesterday;			//�Ƿ�ƽ��
	int m_nFrontID;						//ǰ�÷�����ID
	int m_nSession;						//�ỰID
	string m_strComment;				//��ע
	EOrderType m_eType;					//����
	EOrderAction m_eAction;				//����
	EOrderFlags m_eFlags;				//��־
	EOrderStatus m_eStatus;				//״̬
	EOrderResult m_eResult;				//���
};

