/////////////////////////////////////////////////////////////////////////
///���ƣ�AutoTrader
///�汾��v1.0.x
///���ڣ�2014xxxx
///˵������������CTP����
/////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <fstream>
#include <cstring>
#include <windows.h>

using namespace std;

#include ".\ThostTraderApi\ThostFtdcTraderApi.h"
#include ".\ThostTraderApi\ThostFtdcMdApi.h"
#include "TraderSpi.h"
#include "MdSpi.h"

#pragma warning(disable : 4996)

// USER_API����
extern CThostFtdcTraderApi *pUserApi;

/**
 * һЩȫ�����ò���
 * �û������롢��������Ϣ��
 *
 * external  �ģ��ڡ�AutoTrader.cpp�����浼��ʱ    ���и�ֵ��
 */
// ���ò���
extern char BROKER_ID[];        // ���͹�˾����
extern char INVESTOR_ID[];        // Ͷ���ߴ���
extern char PASSWORD[];            // �û�����

extern TThostFtdcInstrumentIDType INSTRUMENT_ID;    // ��Լ����
extern int DIRECTION;            // ��������
extern int MARKETState;        // ��ƽ��
extern double LIMIT_PRICE;    // �۸�

// ������
extern int iRequestID;
extern char LogFilePaths[80];                //������־
extern TThostFtdcDateExprType TradingDay;

extern double Trade_LastDate[20];
extern char InstrumentID_n[20][10];
extern int InstrumentID_lots[20];
extern double InstrumentID_minmove[20];
// �Ự����
TThostFtdcFrontIDType FRONT_ID;        //ǰ�ñ��
TThostFtdcSessionIDType SESSION_ID;        //�Ự���
TThostFtdcOrderRefType ORDER_REF;        //��������
TThostFtdcOrderActionRefType ORDERACTION_REF[20];    //��������

/**
 * ��ǰ�û����ӳɹ���ĳ�ʼ��
 *
 */
/**
*
* �˴�����һ���ص㡪���������� ���������Ƶ�  spi �����  spi����  ��  api����  �Ĺ��ܡ�
*/
void CTraderSpi::OnFrontConnected() {
    Sleep(3000);
    cerr << "--->>> " << "���׵�¼��..." << endl;

    ///�û���¼����
    CTraderSpi::ReqUserLogin();
}

/**
 * ���͵�¼����
 *
 * ���ɵ�¼������,���͵�¼����
 *
 */
void CTraderSpi::ReqUserLogin() {
    CThostFtdcReqUserLoginField req;
    memset(&req, 0, sizeof(req));
    strcpy(req.BrokerID, BROKER_ID);
    strcpy(req.UserID, INVESTOR_ID);
    strcpy(req.Password, PASSWORD);
    int iResult = pUserApi->ReqUserLogin(&req, ++iRequestID);
    cerr << "--->>> ���ͽ����û���¼����: " << ((iResult == 0) ? "�ɹ�" : "ʧ��") << endl;
}

/**
 * �յ���¼��Ӧ
 *
    1.����Ự����
    2.ȷ�Ͻ�����Ϣ ReqSettlementInfoConfirm()
    3.������ʼ������
 *
 */
void CTraderSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
                                CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
    int GetLocalTimeMs2();
    cerr << "--->>> " << __FUNCTION__ << endl;

    char times[10];

    strcpy(times, pRspUserLogin->SHFETime);

    string str1 = times;
    string str2 = times;
    string str3 = times;

    str1 = str1.substr(0, 2);        //
    str2 = str2.substr(3, 2);        //
    str3 = str3.substr(6, 2);        //

    int hours = atoi(str1.c_str());
    int minutes = atoi(str2.c_str());
    int seconds = atoi(str3.c_str());
    int Millisecs = 0;

    //BATtime�ͱ���ʱ�����ڹ�����ӿ���ʱ�䣬�յ�¼ʱ������������ʱ�����г�ʼ����
    //Timeoffset_Tick=(((hours*60*60+minutes*60+seconds)*1000+Millisecs)-GetLocalTimeMs2());

    if (IsErrorRspInfo(pRspInfo)) {
        cerr << "--->>> ���׵�¼����: " << pRspInfo->ErrorID << pRspInfo->ErrorMsg << endl;
    }
    if (bIsLast && !IsErrorRspInfo(pRspInfo)) {
        // ����Ự����
        FRONT_ID = pRspUserLogin->FrontID;
        SESSION_ID = pRspUserLogin->SessionID;
        int iNextOrderRef = atoi(pRspUserLogin->MaxOrderRef);
        iNextOrderRef++;
        sprintf(ORDER_REF, "%d", iNextOrderRef);
        //cerr << "--->>> �������� = " << ORDER_REF << endl;
        ///��ȡ��ǰ������
        strcpy(TradingDay, pUserApi->GetTradingDay());
        cerr << "--->>> ��ȡ��ǰ������ = " << pUserApi->GetTradingDay() << endl;
        ///Ͷ���߽�����ȷ��
        ReqSettlementInfoConfirm();
    }
}

/**
 *  ȷ�Ͻ�����Ϣ
 *
 *  ���ɽ�����Ϣȷ������,����
 */
void CTraderSpi::ReqSettlementInfoConfirm() {
    CThostFtdcSettlementInfoConfirmField req;
    memset(&req, 0, sizeof(req));
    strcpy(req.BrokerID, BROKER_ID);
    strcpy(req.InvestorID, INVESTOR_ID);
    cerr << "--->>> ȷ�Ͻ���... " << endl;
    //�����ѯ������Ϣȷ��
    int iResult = pUserApi->ReqSettlementInfoConfirm(&req, ++iRequestID);
    cerr << "--->>> Ͷ���߽�����ȷ��: " << ((iResult == 0) ? "�ɹ�" : "ʧ��") << endl;
}

/**
 * �յ�������Ϣȷ����Ӧ
 *
    1.��ѯ��Լ��Ϣ ReqQryInstrument()
    2.��ѯ�ʽ���Ϣ ReqQryTradingAccount()
 */
void CTraderSpi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm,
                                            CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
    cerr << "--->>> " << __FUNCTION__ << endl;
    if (bIsLast && !IsErrorRspInfo(pRspInfo)) {
        ///�����ѯ��Լ
        //for (int i = 0; i < 20; i++)
        //{
        //strcpy(INSTRUMENT_ID,InstrumentID_n[1]);
        ReqQryInstrument();
        //Sleep(1000);
        //}
    }
}

/**
 * ��ѯ��Լ��Ϣ
 *              ���ɺ�Լ��ѯ����,����
 */
void CTraderSpi::ReqQryInstrument() {
    CThostFtdcQryInstrumentField req;
    memset(&req, 0, sizeof(req));
    strcpy(req.InstrumentID, INSTRUMENT_ID);
    int iResult = pUserApi->ReqQryInstrument(&req, ++iRequestID);
    //cerr << "--->>> �����ѯ��Լ: " << ((iResult == 0) ? "�ɹ�" : "ʧ��") << endl;
}

/**
 * �յ���Լ��Ϣ��Ӧ
 *              ��ѯ�ʽ��˻���Ϣ ReqQryTradingAccount();
 */
void CTraderSpi::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID,
                               bool bIsLast) {
    cerr << "--->>> " << __FUNCTION__ << endl;
    if (bIsLast && !IsErrorRspInfo(pRspInfo)) {
        ///�����ѯ��Լ

        //strcpy(INSTRUMENT_ID,pInstrument->InstrumentID);
        //for (int i = 0; i < 20; i++)
        //{
        //if (INSTRUMENT_ID==InstrumentID_n[i])
        //{
        //Trade_LastDate[i]=0;//pInstrument->ExpireDate;
        //cerr << "--->>> ��Լ�������: " <<pInstrument->InstrumentID<<"_"<< pInstrument->ExpireDate << endl;
        //InstrumentID_minmove[i]=pInstrument->PriceTick;
        //}
        //}

        ReqQryTradingAccount();
    }
}

/**
 * ��ѯ�ʽ��˻�
 *              �����ʽ��˻���ѯ����,����
 */
void CTraderSpi::ReqQryTradingAccount() {
    CThostFtdcQryTradingAccountField req;
    memset(&req, 0, sizeof(req));
    strcpy(req.BrokerID, BROKER_ID);
    strcpy(req.InvestorID, INVESTOR_ID);
    int iResult = pUserApi->ReqQryTradingAccount(&req, ++iRequestID);
    cerr << "--->>> �����ѯ�ʽ��˻�: " << ((iResult == 0) ? "�ɹ�" : "ʧ��") << endl;
}

/**
 * �յ��ʽ��˻���Ӧ
 *              ��ѯ�ֲ� ReqQryInvestorPosition();
 */
void CTraderSpi::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo,
                                   int nRequestID, bool bIsLast) {
    cerr << "--->>> " << __FUNCTION__ << endl;
    if (bIsLast && !IsErrorRspInfo(pRspInfo)) {
        cerr << "--->>> ������: " << pTradingAccount->TradingDay << endl;
        cerr << "--->>> �����ʽ�: " << pTradingAccount->Available << endl;
        //cerr << "--->>> ��ȡ�ʽ�: " << pTradingAccount->WithdrawQuota  << endl;
        //��̬Ȩ��=���ս���-������+�����
        double preBalance = pTradingAccount->PreBalance - pTradingAccount->Withdraw + pTradingAccount->Deposit;
        //cerr << "--->>> ��̬Ȩ��: " << preBalance  << endl;
        //��̬Ȩ��=��̬Ȩ��+ ƽ��ӯ��+ �ֲ�ӯ��- ������
        double current = preBalance + pTradingAccount->CloseProfit + pTradingAccount->PositionProfit -
                         pTradingAccount->Commission;
        //cerr << "--->>> ��̬Ȩ��: " << current  << endl;

        //��齻����־�ļ��Ƿ���ڣ��Ƿ���Ҫ�½��ı��ļ�
        if (LogFilePaths[0] == '\0') {
            strcpy(LogFilePaths, "./AutoTrade/");
            strcat_s(LogFilePaths, pTradingAccount->TradingDay);
            strcat_s(LogFilePaths, ".txt");

            //����ļ��Ƿ���ڣ��Ƿ���Ҫ�½��ı��ļ�
            ifstream inf;
            ofstream ouf;
            inf.open(LogFilePaths, ios::out);
        }

        ///�����ѯͶ���ֲ߳�
        //for (int i = 0; i < 20; i++)
        //{
        strcpy(INSTRUMENT_ID, InstrumentID_n[3]);
        ReqQryInvestorPosition();
        //}
    }
}

/**
 * ��ѯ�ֲ�
 *              ���ɳֲֲ�ѯ����,����
 */
void CTraderSpi::ReqQryInvestorPosition() {
    CThostFtdcQryInvestorPositionField req;
    memset(&req, 0, sizeof(req));
    strcpy(req.BrokerID, BROKER_ID);
    strcpy(req.InvestorID, INVESTOR_ID);
    strcpy(req.InstrumentID, INSTRUMENT_ID);
    int iResult = pUserApi->ReqQryInvestorPosition(&req, ++iRequestID);
    cerr << "--->>> �����ѯͶ���ֲ߳�: " << ((iResult == 0) ? "�ɹ�" : "ʧ��") << endl;
}



///�����ѯͶ���ֲ߳���Ӧ

/**
 * �յ��ֲ���Ӧ
 *              ��Լ���ʲ�ѯ ReqQryInstrumentMarginRate();
 */
void CTraderSpi::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition,
                                          CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
    cerr << "--->>> " << "Ͷ���ֲ߳�����:" << "0" << endl;

    if (bIsLast && !IsErrorRspInfo(pRspInfo)) {
        ///��Լ��֤���Լ������Ѳ�ѯ����
        //for (int i = 0; i < 20; i++)
        //{
        strcpy(INSTRUMENT_ID, InstrumentID_n[3]);
        ReqQryInstrumentMarginRate();
        ReqQryInstrumentCommissionRate();
        Sleep(1000);
        //}

    }

    //END

}

///�����ѯ��Լ��֤����
/**
 * ��Լ����/��֤����  ��ѯ
 *              ���ɺ�Լ���ʲ�ѯ����,����
 */
void CTraderSpi::ReqQryInstrumentMarginRate() {
    //cerr << "--->>> ��ѯ��Լ��֤����"<< endl;
    CThostFtdcQryInstrumentMarginRateField req;
    memset(&req, 0, sizeof(req));

    ///���͹�˾����
    strcpy(req.BrokerID, BROKER_ID);
    ///Ͷ���ߴ���
    strcpy(req.InvestorID, INVESTOR_ID);
    ///��Լ����
    strcpy(req.InstrumentID, INSTRUMENT_ID);
    //////Ͷ���ױ���־
    req.HedgeFlag = THOST_FTDC_HF_Speculation;    //Ͷ��

    int iResult = pUserApi->ReqQryInstrumentMarginRate(&req, ++iRequestID);
    //cerr << "--->>> ��ѯ��Լ��֤����: " << ((iResult == 0) ? " �ɹ�" : " ʧ��") << endl;
}


///�����ѯ��Լ��֤������Ӧ
/**
 * �յ���Լ����/��֤����  ��Ӧ
 *              ��¼��Լ����
 */
void CTraderSpi::OnRspQryInstrumentMarginRate(CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate,
                                              CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
    double bzj_b = 100 * (pInstrumentMarginRate->LongMarginRatioByMoney);
    cerr << "--->>> " << "��Լ��֤����:" << pInstrumentMarginRate->InstrumentID << "_" << bzj_b << "%" << endl;
}

///�����ѯ��Լ��������
/**
 * ���ɲ�ѯ��Լ�������ʵ�����,�����͸�ǰ�û���
 *
    ����������Ҫ����:

                    �����̴���
                    Ͷ���ߴ���
                    ��Լ����
                    Ͷ����־(Ͷ��/�ױ�)
                    ���ڻ�ȡ��Լ���������ʡ�

 */
void CTraderSpi::ReqQryInstrumentCommissionRate() {
    //cerr << "--->>> ��ѯ��Լ��������"<< endl;
    CThostFtdcQryInstrumentCommissionRateField req;
    memset(&req, 0, sizeof(req));

    ///���͹�˾����
    strcpy(req.BrokerID, BROKER_ID);
    ///Ͷ���ߴ���
    strcpy(req.InvestorID, INVESTOR_ID);
    ///��Լ����
    strcpy(req.InstrumentID, INSTRUMENT_ID);

    int iResult = pUserApi->ReqQryInstrumentCommissionRate(&req, ++iRequestID);
    //cerr << "--->>> ��ѯ��Լ��������: " << ((iResult == 0) ? " �ɹ�" : " ʧ��") << endl;
}


///�����ѯ��Լ����������Ӧ
/**
 * ��Լ�������ʲ�ѯ�ر�
 *
 *
 * ���Բ��õ����̣�

            ��Ҫ��������:

                            ͨ��pRspInfo�жϲ�ѯ�Ƿ�ɹ�
                            ����ɹ�,�������з��صĺ�Լ�������ʼ�¼
                            �������º�Լ�Ŀ��֡�ƽ���������ʵ����ؼ�¼
                            ��ӡ��ѯ��������������Ϣ
                            ����������һ����¼,�����ȴ�����ķ���
                            ��������һ��,�����ѯ����,�ɴ�������ҵ��

 */
void CTraderSpi::OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate,
                                                  CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
    double sxf_w = 100 * 100 * (pInstrumentCommissionRate->OpenRatioByMoney);
    cerr << "--->>> " << "��Լ��������:" << pInstrumentCommissionRate->InstrumentID << "_" << sxf_w << "%%_"
         << pInstrumentCommissionRate->CloseTodayRatioByVolume << "RMB" << endl;
}


//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//


//����-�޼�,��ʱδʹ��
/**
 * �������ɱ�������,���ͱ���¼������,���������۸���������ƽ�ַ������Ϣ��
 */
void CTraderSpi::ReqOrderInsert() {
    //const char bsstat;
    //char bsopenstat;

    CThostFtdcInputOrderField req;
    memset(&req, 0, sizeof(req));
    ///���͹�˾����
    strcpy(req.BrokerID, BROKER_ID);
    ///Ͷ���ߴ���
    strcpy(req.InvestorID, INVESTOR_ID);
    ///��Լ����
    strcpy(req.InstrumentID, INSTRUMENT_ID);
    ///��������
    strcpy(req.OrderRef, ORDER_REF);
    ///�û�����
//	TThostFtdcUserIDType	UserID;
    ///�����۸�����: �޼�
    req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
    ///��������:
    req.Direction = DIRECTION;
    ///��Ͽ�ƽ��־: ����
    req.CombOffsetFlag[0] = MARKETState;
    ///���Ͷ���ױ���־
    req.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
    ///�۸�
    req.LimitPrice = LIMIT_PRICE;
    ///����: 1
    req.VolumeTotalOriginal = 1;
    ///��Ч������: ������Ч
    req.TimeCondition = THOST_FTDC_TC_GFD;
    ///GTD����
//	TThostFtdcDateType	GTDDate;
    ///�ɽ�������: �κ�����
    req.VolumeCondition = THOST_FTDC_VC_AV;
    ///��С�ɽ���: 1
    req.MinVolume = 1;
    ///��������: ����
    req.ContingentCondition = THOST_FTDC_CC_Immediately;
    ///ֹ���
//	TThostFtdcPriceType	StopPrice;
    ///ǿƽԭ��: ��ǿƽ
    req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
    ///�Զ������־: ��
    req.IsAutoSuspend = 0;
    ///ҵ��Ԫ
//	TThostFtdcBusinessUnitType	BusinessUnit;
    ///������
//	TThostFtdcRequestIDType	RequestID;
    ///�û�ǿ����־: ��
    req.UserForceClose = 0;

    //int iResult = pUserApi->ReqOrderInsert(&req, ++iRequestID);
    //cerr << "--->>> ����¼������: " << ((iResult == 0) ? " �ɹ�" : " ʧ��") << endl;
}



// �������粻��ͨ��THOSTУ�飬THOST�ܾ�����������OnRspOrderInsert����������룩�����У��ͨ����THOST���ձ�����ת����������
// �������յ�������ͨ��У�顣�û����յ�OnRtnOrder��OnRtnTrade��
// �����������Ϊ���������û��ͻ��յ�OnErrRtnOrder
/**
 * ���յ�����¼���������Ӧʱ����,���ڴ����������ܻ򱨵�����Ļر���Ϣ��

    �������´���:

            ��ӡ�����ر�����
            ͨ��IsErrorRspInfo�ж��Ƿ񱨵�ʧ��
            ������ܱ���,��¼�������
            �������ʧ��,���д�����

 *
 */
void CTraderSpi::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID,
                             bool bIsLast) {
    double GetLocalTimeSec1();

    cerr << "--->>> " << __FUNCTION__ << endl;
    double system_times = GetLocalTimeSec1();//��ʽʱ��0.145100
    //cerr << "--->>> ����¼����Ӧ: "<<pInputOrder->InstrumentID<< endl;
    cerr << "--->>> ������Ӧ��Ϣ: " << pInputOrder->InstrumentID << "_" << ((pInputOrder->Direction == '0') ? "��" : "��")
         << "_" << ((pInputOrder->CombOffsetFlag[0] == '0') ? "����" : "ƽ��") << "_" << pInputOrder->LimitPrice << endl;
    IsErrorRspInfo(pRspInfo);
}

//����-�޸ģ�����
/**
 * �������ɱ������������޸�����,����ԭʼ������Ϣ�Ͳ�����־�ȡ�

    ��Ҫ����:

                    pOrder:ԭʼ������Ϣ,�ṩ��������
                    ActionFlag:������־,�������޸�
                    ��������������Ҫ����Ϣ

    ���ô˺������Է��ͱ����������޸�����

 */
void CTraderSpi::ReqOrderAction(CThostFtdcOrderField *pOrder) {
    static bool ORDER_ACTION_SENT = false;        //�Ƿ����˱���
    if (ORDER_ACTION_SENT)
        return;

    CThostFtdcInputOrderActionField req;
    memset(&req, 0, sizeof(req));
    ///���͹�˾����
    strcpy(req.BrokerID, pOrder->BrokerID);
    ///Ͷ���ߴ���
    strcpy(req.InvestorID, pOrder->InvestorID);
    ///������������
//	TThostFtdcOrderActionRefType	OrderActionRef;
    ///��������
    strcpy(req.OrderRef, pOrder->OrderRef);
    ///������
//	TThostFtdcRequestIDType	RequestID;
    ///ǰ�ñ��
    req.FrontID = FRONT_ID;
    ///�Ự���
    req.SessionID = SESSION_ID;
    ///����������
//	TThostFtdcExchangeIDType	ExchangeID;
    ///�������
//	TThostFtdcOrderSysIDType	OrderSysID;
    ///������־
    req.ActionFlag = THOST_FTDC_AF_Delete;
    ///�۸�
//	TThostFtdcPriceType	LimitPrice;
    ///�����仯
//	TThostFtdcVolumeType	VolumeChange;
    ///�û�����
//	TThostFtdcUserIDType	UserID;
    ///��Լ����
    strcpy(req.InstrumentID, pOrder->InstrumentID);
    //cerr << "--->>> ������������: "<< endl;
    int iResult = pUserApi->ReqOrderAction(&req, ++iRequestID);
    cerr << "--->>> ������������: " << pOrder->InstrumentID << "_" << ((pOrder->Direction == '0') ? "��" : "��") << "_"
         << ((pOrder->CombOffsetFlag[0] == '0') ? "����" : "ƽ��") << ((iResult == 0) ? " �ɹ�" : " ʧ��") << endl;
    ORDER_ACTION_SENT = true;
}

/**
 * ���յ��������������޸��������Ӧʱ����,���ڴ�������Ļر���

    ��Ҫ����:

            ��ӡ�����ر�����
            ͨ��IsErrorRspInfo�ж��Ƿ񳷵��ɹ�
            ����ɹ�,��¼�����Ϣ
            ���ʧ��,���д�����

 */
void CTraderSpi::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo,
                                  int nRequestID, bool bIsLast) {
    cerr << "--->>> " << __FUNCTION__ << endl;
    IsErrorRspInfo(pRspInfo);

}

///����֪ͨ
/**
 * ���ڽ��ձ����ر�,���Ը��ݻر����г���������������
 *

OnRtnOrder()�Ǳ����ر��Ļص�����,���ڽ��ղ�������״̬�����͸��¡�

��Ҫ����:

        pOrder: ������Ϣ,��������״̬��������š���Լ���롢�۸���������Ϣ��

��Ҫ��������:

        �жϱ����Ƿ����Լ�������(ͨ���Ự��š��������õ��ж�)
        ������Լ��ı���,��һ���жϱ���״̬ 2.1 ����������ڽ���,�����ReqOrderAction()���ͳ������� 2.2 ��������ѳ���,��ӡ��ʾ���������ɹ�
        ���ݱ���״̬������ؼ�¼,��ί�гɹ������ֳɽ�����ȫ�ɽ���
        ���ݱ�����Ϣ��������ҵ����
        ��ӡ������Ϣ����ȷ�Ͻ��յ������ر�

����OnRtnOrder��Ҫ�����ǽ��ձ���״̬��ʵʱ����,Ȼ����ݱ�����Ϣ���г�������¼���¡�ҵ����Ȳ�����

 */
void CTraderSpi::OnRtnOrder(CThostFtdcOrderField *pOrder) {

    char str[10];
    int i = 0;
    sprintf(str, "%d", pOrder->OrderSubmitStatus);
    int OrderState = atoi(str) - 48;    //����״̬0=�Ѿ��ύ��3=�Ѿ�����
    //cerr << "--->>> " << __FUNCTION__ <<"_"<< OrderState << endl;

    if (IsMyOrder(pOrder)) {
        if (IsTradingOrder(pOrder)) {
            //ReqOrderAction(pOrder);	//����
            cout << "--->>> ����ִ�У�" << endl;
            i = 1;
        } else if (pOrder->OrderStatus == THOST_FTDC_OST_Canceled)
            cout << "--->>> �����ɹ���" << endl;
    }
}

///�ɽ�֪ͨ
/**
 * ���ڽ��ճɽ��ر�,���Լ�¼�ɽ���Ϣ

 */
void CTraderSpi::OnRtnTrade(CThostFtdcTradeField *pTrade) {
    cerr << "--->>> " << __FUNCTION__ << endl;
    cerr << "--->>> �ɽ�֪ͨ:" << pTrade->TradingDay << "_" << pTrade->TradeTime << "_" << pTrade->InstrumentID << "_"
         << ((pTrade->Direction == '0') ? "��" : "��") << "_" << ((pTrade->OffsetFlag == '0') ? "����" : "ƽ��") << "_"
         << pTrade->Volume << "_" << pTrade->Price << endl;
}

/**
 * �Ͽ����ӻر�
 */
void CTraderSpi::OnFrontDisconnected(int nReason) {
    cerr << "--->>> " << "OnFrontDisconnected!" << endl;
    //cerr << "--->>> Reason = " << nReason << endl;
    //cerr << "--->>> " << "���׶Ͽ�! Reason ="+nReason << endl;
}

/**
 * ������ʱ�ر�
 */
void CTraderSpi::OnHeartBeatWarning(int nTimeLapse) {
    cerr << "--->>> " << "OnHeartBeatWarning" << endl;
    cerr << "--->>> nTimerLapse = " << nTimeLapse << endl;
}

/**
 * ���ڽ��մ���ر�,���Ի�ȡ������Ϣ
 *
 */
void CTraderSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
    cerr << "--->>> " << "OnRspError" << endl;
    IsErrorRspInfo(pRspInfo);
}

/**
 * �ж�һ���ر��Ƿ����������Ϣ
 */
bool CTraderSpi::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo) {
    // ���ErrorID != 0, ˵���յ��˴������Ӧ
    bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
    if (bResult)
        cerr << "--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << endl;
    return bResult;
}

/**
 * �ж�һ�������ر��Ƿ����Լ�������
 */
bool CTraderSpi::IsMyOrder(CThostFtdcOrderField *pOrder) {
    //cerr << "--->>> " << "IsMyOrder" << endl;
    return ((pOrder->FrontID == FRONT_ID) &&
            (pOrder->SessionID == SESSION_ID) &&
            (strcmp(pOrder->OrderRef, ORDER_REF) == 0));
}

/**
 * �ж�һ�������Ƿ����ڽ�����
 */
bool CTraderSpi::IsTradingOrder(CThostFtdcOrderField *pOrder) {
    //cerr << "--->>> " << "IsTradingOrder" << endl;
    return ((pOrder->OrderStatus != THOST_FTDC_OST_PartTradedNotQueueing) &&
            (pOrder->OrderStatus != THOST_FTDC_OST_Canceled) &&
            (pOrder->OrderStatus != THOST_FTDC_OST_AllTraded));
}

