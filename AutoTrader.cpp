/////////////////////////////////////////////////////////////////////////
///���ƣ�AutoTrader
///�汾��v1.0.2
///���ڣ�20140701
///˵������������CTP����  6.2.0_20131205_tradeapi_windows
///�汾��ctp 6.2.0_20131205   
///���ߣ�Сƽ����
///  QQ��1396144618
///�������ң�����Դ�����ȫ�����Լ����Զ�������ϵͳ��
/////////////////////////////////////////////////////////////////////////
// AutoTrader.cpp : �������̨Ӧ�ó������ڵ㡣
//

/**
 * ������Ҫ��ͷ�ļ��������ռ�
 */

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream> 
#include <string>
#include <windows.h>
#include <time.h>
#include <algorithm>

#include <stdlib.h>
using namespace std;

#include ".\ThostTraderApi\ThostFtdcTraderApi.h"
#include ".\ThostTraderApi\ThostFtdcMdApi.h"
#include "TraderSpi.h"
#include "MdSpi.h"
#include "Common.h"
#include "DataSniffer.h"
#include "MyTrader.h"

#pragma warning(disable : 4996)

/**
 * ������ȫ�ֵ�API����ָ��pUserApi��pMdApi
 */

// UserApi����
CThostFtdcTraderApi *pUserApi;
// MdApi����
CThostFtdcMdApi *pMdApi;

/**
 * �����˳�����ȫ�ֱ���:��������ģʽ��CTP��ǰ�õ�ַ���û�������ȡ�
 *
 * ������ʵ�̺ͷ�����������ģʽ���л�����RunMode  ��
 */

int		RunMode = 0;		//����ģʽѡ��0=���ز��ԣ�1=ʵ������ ����ʾCommon.h������InstrumentID_En=0��ʵ�����߷���ģʽ��InstrumentID_En=1��ʵ�����߽���ģʽ��


/**
 * �����˽��׺������ǰ�÷�������ַ,ʵ�̺ͷ���ʹ�ò�ͬ��ַ
 */

// ���ò���
char  FRONT_ADDR_1A[] = "tcp://180.168.212.51:41205";		// ǰ�õ�ַ1����:ʵ��
char  FRONT_ADDR_1B[] = "tcp://180.168.212.51:41213";		// ǰ�õ�ַ1����:ʵ��
char  FRONT_ADDR_2A[] = "tcp://180.168.212.52:41205";		// ǰ�õ�ַ2����:ʵ��
char  FRONT_ADDR_2B[] = "tcp://180.168.212.53:41213";		// ǰ�õ�ַ2����:ʵ��
char  FRONT_ADDR_3A[] = "tcp://180.168.212.53:41205";		// ǰ�õ�ַ3����:ʵ�� �̺�
char  FRONT_ADDR_3B[] = "tcp://180.168.212.53:41213";		// ǰ�õ�ַ3����:ʵ�� �̺�

char  FRONT_ADDR_6A[] = "tcp://asp-sim2-front1.financial-trading-platform.com:26205";	// ǰ�õ�ַ6����:���� 17:00��ʼ
char  FRONT_ADDR_6B[] = "tcp://asp-sim2-md1.financial-trading-platform.com:26213";		// ǰ�õ�ַ6����:���� 17:00��ʼ

/**
 * �������˺�����;��͹�˾������˻���Ϣ
 */

TThostFtdcBrokerIDType	BROKER_ID = "2030";								// ���͹�˾����
TThostFtdcInvestorIDType INVESTOR_ID = "88888888";						// Ͷ���ߴ���
TThostFtdcPasswordType  PASSWORD = "123456";							// �û�����
//TThostFtdcBrokerIDType	BROKER_ID = "2030";							// ���͹�˾����:����
//TThostFtdcInvestorIDType INVESTOR_ID = "00069";						// Ͷ���ߴ���:����"00069","00092"
//TThostFtdcPasswordType  PASSWORD = "888888";							// �û�����:����


/**
 * �����˶��ĵĺ�Լ�����б�ppInstrumentID
 */
//char *ppInstrumentID[] = {"rb1410"};									// ���鶩���б�
//int iInstrumentID = 1;												// ���鶩������
//							0		1			2		  3			4		 5			6		   7		8			9		10		11		12		13		14		15		  16		17	   18		 19
char *ppInstrumentID[] = {  "i1409", "jm1409", "j1409", "rb1410","rb1501", "TA409", "l1409","ru1409", "ru1501", "jd1409", "RM409", "m1409","y1501", "p1501","ag1506","ag1412","cu1408","cu1409","IF1409","IF1407"};//���鶩���б�
//char *ppInstrumentID[] = {  "ag1412"};//���鶩���б�
int iInstrumentID = 1;													// ���鶩������

// User��������

// �����Ự����

// �����ⲿ����


void main(void)
{
	void Erasefiles();
	bool ReadMdConfiguration();	
	bool ReadTradeConfiguration();
    // _record0���ڼ�¼��־��
	void _record0(char *txt);
	void Sniffer();
	void Trading();
	void test();
    // SendOrder()���ڷ��Ͷ�����
    // ͨ��SendOrder()������װ�˱�������
    // ��Լ���롢�������򡢿�ƽ�ֱ�־��Ϊ���������Ĳ���
	void SendOrder(TThostFtdcInstrumentIDType FuturesId,int BuySell,int OpenClose,int i);


    /**
     * �ȳ�ʼ����־,��ȡ������Ϣ
     */
	JustRun = true;													//����������־
    /**
     * Erasefiles()����ļ���
     */
	Erasefiles();
	Sleep(2000);

	cerr << "--->>> " << "Welcom MyAutoTrader System!" << endl;
	cerr << "--->>> " << "Version 1.0.3!" << endl;

    /**
     * ReadMdConfiguration()��ReadTradeConfiguration()���ڶ�ȡ���á�
     */
	ReadMdConfiguration();
	Sleep(2000);

    /**
     * ����CTP�Ľ��׺�����API����,��ע����Ӧ�Ļص��ӿ�
     *
     * ��ʼ������ǰ�÷�����
     */
    /**
     * ���ú�����ʼ��UserApi��MdApi����,��������ͽ���ǰ�á�
     */

	// ��ʼ��UserApi
	pUserApi = CThostFtdcTraderApi::CreateFtdcTraderApi("./thosttraderapi.dll");			// ����UserApi//"./thosttraderapi.dll"
	CTraderSpi* pUserSpi = new CTraderSpi();
	pUserApi->RegisterSpi((CThostFtdcTraderSpi*)pUserSpi);			// ע���¼���
	pUserApi->SubscribePublicTopic(THOST_TERT_RESTART);				// ע�ṫ����
	pUserApi->SubscribePrivateTopic(THOST_TERT_RESTART);			// ע��˽����
	pUserApi->RegisterFront(FRONT_ADDR_1A);							// connect
	//pUserApi->RegisterFront(FRONT_ADDR_2A);						// connect
	
	if (RunMode)
	{
		pUserApi->Init();
		cerr << "--->>> " << "Initialing UserApi" << endl;
	}
	

	// ��ʼ��MdApi
	pMdApi = CThostFtdcMdApi::CreateFtdcMdApi("./thostmduserapi.dll");					// ����MdApi//"./thostmduserapi.dll"
	CThostFtdcMdSpi* pMdSpi = new CMdSpi();
	pMdApi->RegisterSpi(pMdSpi);									// ע���¼���
	pMdApi->RegisterFront(FRONT_ADDR_1B);							// connect
	//pMdApi->RegisterFront(FRONT_ADDR_2B);							// connect

	if (RunMode)    // ʵ��
	{
		pMdApi->Init();
		cerr << "--->>> " << "Initialing MdApi" << endl;
	}
	else
	{
		cerr << "--->>> " << "Test Mode!" << endl;
	}
	
	//pMdApi->Join();
	//pMdApi->Release();
	
	Sleep(7000);	
	ReadTradeConfiguration();
	Sleep(1000);
	cerr << "--->>> " << "��ʼ�����!" << endl;


    /**
     * �ṩ�˷����ʵ��ģʽ�л�
     */

    /**
     * ��ѭ���е���Sniffer()��Trading()����
     *
     * ѭ������Sniffer()��Trading()������
     */
	while(RunMode)	//ʵ��
	{
        /**
         * Sniffer()����������ɼ��ʹ�������
         */
		Sniffer();

        /**
         * Trading()���������ɺͷ��ͽ���ָ��ĺ�����
         *
         * Trading()����������ʵ�ֽ��ײ��Ժ��µ����߼�
         */
		Trading();
		Sleep(050);
	}

    /**
     * ��һ�����ز���ģʽ,�ڸ�ģʽ�µ���test()�������в��ԡ�
     */
	while(!RunMode)	//���ز���
	{
		if (JustRun==true)
		{
			test();	//���ػز�
			JustRun=false;
		}
	}



}

