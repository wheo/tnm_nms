#pragma once
#include "AsnType.h"

struct stTrapMsg
{
	wchar_t strServerIP[100];
	wchar_t strCummnity[100];
	wchar_t strMsgTime[100];
		
	wchar_t strDescription[500];	

	int nServerType; //> 0: cm500, 1: dr5000
	int nMsgIndex;

	int nData[10];
	//int nLevel;		//> 수신된 트랩의 레벨
	//int nType;		//> 트랩의 타입
	//int nChannel;	//> 해당 채널
	//int nMain;		 
	//int nIndex;		//> ES in channel

	/*int iMsgType;
	int iLevel;
	int iData;
	int iTime;*/
};

struct stTrapMsgList
{
	wchar_t strOID[500];
	int iMsgType;	//> 트랩메시지 종류. 인코딩 상태정보냐, 성능에 관한거냐, 입력 단절에 관한거냐 등등
	int iDataType;  //> 0: string, 1, int
	int iIndex;
};

class CSnmpTrapParser : public CAsnType
{
public:
	CSnmpTrapParser();
	~CSnmpTrapParser();

	bool ParseTrapMsg(byte *pBuff, int nLen, char *pAgent);

	virtual int ParsePacket(const char * pszPacket, int iPacketLen);

protected:	
	void WriteTrapMsg(CString strOID, int iDataType);
	
	void ParseCM5000(CString strOID);
	void ParseDR5000(CString strOID);

	//> 사용 안함
	bool ParseTrapv1(byte *pBuff, int nLen);
protected:
	CString m_strTrapTypeCM5000;
	CString m_StrTrapTypeDR5000;

	stTrapMsg	m_stTrapMsg;
	
	CString m_strCM5000Descr[MAX_TRAP_MSG_COUNT];
	CString m_strDR5000Descr[MAX_TRAP_MSG_COUNT];	
};

