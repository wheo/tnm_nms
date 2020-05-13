#pragma once
#include "SnmpTrapParser.h"
class CCommSNMP : public CTnmThread, public CPtrList, public CSnmpTrapParser
{
public:
	CCommSNMP(CString strIpAddr);
	~CCommSNMP();

	///> SNMP 인터페이스를 통한 조회	
	//> encoder
	//> -1: not connect, 1: noSignal, 2: pcm
	int GetEncoderAudioSignal(wchar_t *strServerIP);
	//> -1: not connect, 1: unavailable, 2: none, etc...
	int GetEncoderVideoInfo(wchar_t *strServerIP);
	//> -1: not, 1: true, 2: error
	int GetEncoderSystemEthenetLinkDown(wchar_t *strServerIP);
	//> -1: not, 1: error, 2: true
	int GetEncoderFanFailure(wchar_t *strServerIP);
	//> -1: not, 55 < error
	int GetEncoderOverHeat(wchar_t *strServerIP);
	//> -1: not, 1: error, 2: true
	int GetEncoderPSU(wchar_t *strServerIP);

	///> decoder
	//> -1: not, 1: true, 2: error
	int GetDecoderInputState(wchar_t *strServerIP);
	//> -1: not, 1: true, 2: error
	int GetDecoderEthenetState(wchar_t *strServerIP);
	//> -1: not, 1: error, 2: true
	int GetDecoderFanState(wchar_t *strServerIP);
	//> -1: not, 65 < error 
	int GetDecoderOverHeat(wchar_t *strServerIP);
	int GetDecoderInputBitrate(wchar_t *strServerIP);



	int GetServerState(wchar_t *strServerIP);
	void GetAppTime(wchar_t *strServerIP);

	///> Trap Msg
	void *GetFirstTrapMsg();

private:
	int SnmpGetINT(char *strIP, char *strOID);

protected:
	///> Trap msg 수신
	void ClearSocket();
	bool CreateSocket();

	void ClearTrapMsgList();
	void Run();
	
	/*
	* tmo
	* 단위 마이크로 세컨트 100만분의1초
	* 여기서는 1초
	*/
//	bool SnmpGetForString(char *ip, char *comm, char *oids, int tmo = 1000);
	bool SnmpGet(char *ip, char *comm, char *oids, int tmo = 1000);
//	bool SnmpGet(char *ip, char *comm, char *oids, int tmo = 300);

protected:

	int m_iResultType; //> 0: int, 1: string
	__int64 m_i64Result;
	char m_strResult[1000];	

	CString m_strRecvIP;

	CCriticalSection m_csComm;

	SOCKET m_sdUDP;
	CCriticalSection m_csTrap;
};

