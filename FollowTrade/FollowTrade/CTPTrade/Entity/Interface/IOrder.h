#pragma once
#include <string>

using namespace std;

class IOrder
{
public:
	IOrder() {};
	virtual ~IOrder() {};

public:
	enum EOrderType
	{
		OrderTypeInvalid = 0,	//��Ч
		OrderTypeBuy,			//��
		OrderTypeSell			//��
	};
	enum EOrderAction
	{
		OrderActionInvalid = 0,	//��Ч
		OrderActionInstant,		//����ִ��
		OrderActionPending,		//�ҵ�
		OrderActionMarket,		//�м�
		OrderActionRemove		//����
	};
	enum EOrderFlags
	{
		OrderFlagsInvalid = 0,	//��Ч
		OrderFlagsOpen,			//����
		OrderFlagsClose			//ƽ��
	};
	enum EOrderStatus
	{
		OrderStatusInvalid = 0,	//��Ч
		OrderStatusSubmitted,	//�ύ
		OrderStatusAccepted,	//����
		OrderStatusRejected,	//�ܾ�
		OrderStatusCancel,		//ȡ��
		OrderStatusDonePartial,	//���ֳɽ�
		OrderStatusDone			//ȫ���ɽ�
	};
	enum EOrderResult
	{
		OrderResultInvalid = 0,	//��Ч
		OrderResultAllTraded,	//��ȫ���ɽ�
		OrderResultTrading,		//���ڽ�����
		OrderResultNotQueueing,	//���˳�����
		OrderResultCanceled		//��ȡ��
	};

public://Assign
	virtual void Assign( const IOrder &Order) = 0;
	virtual void Clear() = 0;

public://Mumber
	virtual void SetOrderID( int uOrderID) = 0;					//���õ���
	virtual int GetOrderID() const = 0;								//��ȡ����
	virtual void SetSymbol( string strSymbol) = 0;					//����MT��Լ
	virtual string GetSymbol() const = 0;								//��ȡMT��Լ
	virtual void SetOrderLocalID(string strOrderLocalID) = 0;			//����������ID
	virtual string GetOrderLocalID() const = 0;								//����������DI
	virtual void SetVolume( double dVolume) = 0;						//���õ�ǰ��
	virtual double GetVolume() const = 0;								//��ȡ��ǰ��
	virtual void SetInitVolume( double dInitVolume) = 0;				//���ó�ʼ��
	virtual double GetInitVolume() const = 0;							//��ȡ��ʼ��
	virtual void SetRemaindVolume( double dRemaindVolume) = 0;		//����������
	virtual double GetRemaindVolume() const = 0;						//��ȡ������
	virtual void SetPrice( double dPrice) = 0;						//���ñ���
	virtual double GetPrice() const = 0;								//��ȡ����
	virtual void SetSLPrice( double dSLPrice) = 0;					//����ֹ���
	virtual double GetSLPrice() const = 0;								//��ȡֹ���
	virtual void SetTPPrice( double dTPPrice) = 0;					//����ֹӯ��
	virtual double GetTPPrice() const = 0;								//��ȡֹӯ��
	virtual void SetInsertTime( long long lInsertTime) = 0;			//���ñ���ʱ��
	virtual long long GetInsertTime() const = 0;						//��ȡ����ʱ��
	virtual void SetIsCloseYesterday( bool bIsCloseYesterday) = 0;	//�����Ƿ�ƽ��
	virtual bool GetIsCloseYesterday() const = 0;						//��ȡ�Ƿ�ƽ��
	virtual void SetFrontID( int nFrontID) = 0;						//����ǰ�÷�����ID
	virtual int GetFrontID() const = 0;									//��ȡǰ�÷�����ID
	virtual void SetSession( int nSession) = 0;						//���ûỰID
	virtual int GetSession() const = 0;									//��ȡ�ỰID
	virtual void SetComment( string strComment) = 0;					//���ñ�ע
	virtual string GetComment() const = 0;								//��ȡ��ע
	virtual void SetType( EOrderType eType) = 0;						//��������
	virtual EOrderType GetType() const = 0;								//��ȡ����
	virtual void SetAction( EOrderAction eAction) = 0;				//���ö���
	virtual EOrderAction GetAction() const = 0;							//��ȡ����
	virtual void SetFlags( EOrderFlags eFlags) = 0;					//���ñ�־
	virtual EOrderFlags GetFlags() const = 0;							//��ȡ��־
	virtual void SetStatus( EOrderStatus eStatus) = 0;				//����״̬
	virtual EOrderStatus GetStatus() const = 0;							//��ȡ״̬
	virtual void SetResult( EOrderResult eResult) = 0;				//���ý��
	virtual EOrderResult GetResult() const = 0;							//��ȡ���
};