#include "stdafx.h"
#include "CommSNMP.h"

#include "SNMPManager.h"

#pragma comment(lib, "Wsnmp32.lib")

CCommSNMP::CCommSNMP(CString strIpAddr)
{
	m_sdUDP = INVALID_SOCKET;

	m_strRecvIP.Format(L"%s", strIpAddr);

	Start();
}


CCommSNMP::~CCommSNMP()
{
	ClearSocket();

	Terminate();

	ClearTrapMsgList();
}

int CCommSNMP::GetEncoderAudioSignal(wchar_t *strServerIP)
{
	char toid[256];
	char serverip[150];

	UnicodeToMByte(strServerIP, serverip, 150);

	sprintf(toid, ".1.3.6.1.4.1.27338.4.5.7.2.1.1.3.1.1");

	return SnmpGetINT(serverip, toid);	
}
int CCommSNMP::GetEncoderVideoInfo(wchar_t *strServerIP)
{
	char toid[256];
	char serverip[150];

	UnicodeToMByte(strServerIP, serverip, 150);
		
	sprintf(toid, ".1.3.6.1.4.1.27338.4.5.7.1.1.2.1");
	
	return SnmpGetINT(serverip, toid);	
}
int CCommSNMP::GetEncoderSystemEthenetLinkDown(wchar_t *strServerIP)
{
	char toid[256];
	char serverip[150];

	UnicodeToMByte(strServerIP, serverip, 150);

	int nResult = -1;
	for (int i = 0; i < 3; i++) {
		sprintf(toid, ".1.3.6.1.4.1.27338.4.5.6.1.1.2.%d", i+1);
		nResult = SnmpGetINT(serverip, toid);
		if (nResult == -1) {
			break;
		}
		else if (nResult == 2) {
			break;
		}
	}
	
	return nResult;
}
int CCommSNMP::GetEncoderFanFailure(wchar_t *strServerIP)
{
	char toid[256];
	char serverip[150];

	UnicodeToMByte(strServerIP, serverip, 150);

	int nResult = -1;
	for (int i = 0; i < 8; i++) {
		sprintf(toid, ".1.3.6.1.4.1.27338.4.5.2.1.1.2.%d", i + 1);
		nResult = SnmpGetINT(serverip, toid);
		if (nResult == -1) {
			break;
		}
		else if (nResult == 1) {
			break;
		}
	}

	return nResult;
}
int CCommSNMP::GetEncoderOverHeat(wchar_t *strServerIP)
{
	char toid[256];
	char serverip[150];

	UnicodeToMByte(strServerIP, serverip, 150);

	sprintf(toid, ".1.3.6.1.4.1.27338.4.5.1.0");

	return SnmpGetINT(serverip, toid);
}
int CCommSNMP::GetEncoderPSU(wchar_t *strServerIP) 
{
	char toid[256];
	char serverip[150];

	UnicodeToMByte(strServerIP, serverip, 150);

	int nResult = -1;
	for (int i = 0; i < 2; i++) {
		sprintf(toid, ".1.3.6.1.4.1.27338.4.5.3.1.1.2.%d", i + 1);
		nResult = SnmpGetINT(serverip, toid);
		if (nResult == -1) {
			break;
		}
		else if (nResult == 1) {
			break;
		}
	}

	return nResult;	
}

int CCommSNMP::GetDecoderInputState(wchar_t *strServerIP)
{
	char toid[256];
	char serverip[150];

	UnicodeToMByte(strServerIP, serverip, 150);
			
	sprintf(toid, ".1.3.6.1.4.1.27338.5.5.2.1.2.5.0");

	return SnmpGetINT(serverip, toid);	
}
int CCommSNMP::GetDecoderEthenetState(wchar_t *strServerIP)
{
	char toid[256];
	char serverip[150];

	UnicodeToMByte(strServerIP, serverip, 150);

	int nResult = -1;
	for (int i = 0; i < 3; i++) {
		sprintf(toid, ".1.3.6.1.4.1.27338.5.7.4.1.1.2.%d", i + 1);
		nResult = SnmpGetINT(serverip, toid);
		if (nResult == -1) {
			break;
		}
		else if (nResult == 2) {
			break;
		}
	}

	return nResult;
}
int CCommSNMP::GetDecoderFanState(wchar_t *strServerIP)
{
	char toid[256];
	char serverip[150];

	UnicodeToMByte(strServerIP, serverip, 150);

	int nResult = -1;
	for (int i = 0; i < 4; i++) {
		sprintf(toid, ".1.3.6.1.4.1.27338.5.7.2.1.1.2.%d", i + 1);
		nResult = SnmpGetINT(serverip, toid);
		if (nResult == -1) {
			break;
		}
		else if (nResult == 1) {
			break;
		}
	}

	return nResult;
}
int CCommSNMP::GetDecoderOverHeat(wchar_t *strServerIP)
{
	char toid[256];
	char serverip[150];

	UnicodeToMByte(strServerIP, serverip, 150);

	sprintf(toid, ".1.3.6.1.4.1.27338.5.7.1.0");

	return SnmpGetINT(serverip, toid);
}
int CCommSNMP::GetDecoderInputBitrate(wchar_t *strServerIP)
{
	int iResult = -1;

	char toid[256];
	char serverip[150];

	UnicodeToMByte(strServerIP, serverip, 150);

	sprintf(toid, ".1.3.6.1.4.1.27338.5.5.3.1.0");
	if (SnmpGet(serverip, "public", toid)) {
		//		_d(_T("[snmp] get server state: %I64d\n"), m_i64Result);
		if (m_iResultType == 0) {
			iResult = (int)m_i64Result;
		}
		else {
			iResult = -1;
		}
	}
	else {
		iResult = -1;
	}

	return iResult;
}

int CCommSNMP::GetServerState(wchar_t *strServerIP)
{	
	// "Encoder status value 0-idle, 1-starting, 2-stopping, 3-started"

	int iResult = -1;

	char toid[256];	
	char serverip[150];

	UnicodeToMByte(strServerIP, serverip, 150);	

	sprintf(toid, ".1.3.6.1.4.1.51506.8.4.1.1.0"); 
	if (SnmpGet(serverip, "public", toid)) {
//		_d(_T("[snmp] get server state: %I64d\n"), m_i64Result);
		if (m_iResultType == 0) {
			iResult = (int)m_i64Result;
		}
		else {
			iResult = -1;
		}
	}
	else {
		iResult = -1;
	}

	return iResult;
}

void CCommSNMP::GetAppTime(wchar_t *strServerIP)
{
	int iResult = 0;

	char toid[256];
	char serverip[150];

	UnicodeToMByte(strServerIP, serverip, 150);

	sprintf(toid, ".1.3.6.1.4.1.51506.8.4.1.3.0");
//	sprintf(toid, ".1.3.6.1.2.1.25.1.2.0");	
	if (SnmpGet(serverip, "public", toid)) {
		//_d(_T("[snmp] get server Time: %d\n"), m_iResult);
		_d(_T("[snmp] get server Time: %I64d\n"), m_strResult);
		if (m_iResultType == 0) {
			iResult = m_i64Result;
		}
		else {
			iResult = -1;
		}
	}
	else {
		iResult = -1;
	}

//	return iResult;
}

int CCommSNMP::SnmpGetINT(char *strIP, char *strOID)
{
	int iResult = -1;

	if (SnmpGet(strIP, "public", strOID)) {
		//		_d(_T("[snmp] get server state: %I64d\n"), m_i64Result);
		if (m_iResultType == 0) {
			iResult = (int)m_i64Result;
		}
		else {
			iResult = -1;
		}
	}
	else {
		iResult = -1;
	}

	return iResult;
}

bool CCommSNMP::SnmpGet(char *ip, char *comm, char *oids, int tmo)
{
	m_csComm.Lock();

	SNMPManager::getManager()->startup();
	SNMPRemoteAgent ra(ip, comm, 161);

	SNMPRemoteAgentGetRequest getReq(oids);

	getReq.execute(&ra);
	getReq.wait(tmo);

	if (!getReq.succeeded()) {
		int iState = getReq.GetState();
		int iError = getReq.getError();

		switch (iState) {
		case SNMP_RAREQ_STATE_NONE:
//			_d(L"[snmp] get request failed. SNMP_RAREQ_STATE_NONE\n");
			break;

		case SNMP_RAREQ_STATE_ONGOING:
//			_d(L"[snmp] get request failed. SNMP_RAREQ_STATE_ONGOING\n");
			break;

		case SNMP_RAREQ_STATE_TIMEDOUT:
//			_d(L"[snmp] get request failed. time out\n");
			break;

		case SNMP_RAREQ_STATE_SUCCEEDED:
//			_d(L"[snmp] get request failed. SNMP_RAREQ_STATE_SUCCEEDED\n");
			break;

		case SNMP_RAREQ_STATE_ERROR:
			// getReq.getErrorAsString()
//			_d(L"[snmp] get request failed. SNMP_RAREQ_STATE_ERROR\n");
			if (iError == SNMP_ERROR_TOOBIG) {
//				_d(L"[snmp] error SNMP_ERROR_TOOBIG\n");
			}
			else if (iError == SNMP_ERROR_NOSUCHNAME) {
//				_d(L"[snmp] error SNMP_ERROR_NOSUCHNAME\n");
			}
			else if (iError == SNMP_ERROR_BADVALUE) {
//				_d(L"[snmp] error SNMP_ERROR_BADVALUE\n");
			}
			else if (iError == SNMP_ERROR_READONLY) {
//				_d(L"[snmp] error SNMP_ERROR_READONLY\n");
			}
			else if (iError == SNMP_ERROR_GENERR) {
//				_d(L"[snmp] error SNMP_ERROR_GENERR\n");
			}
			break;

		case SNMP_RAREQ_STATE_CANCELLED:
//			_d(L"[snmp] get request failed. SNMP_RAREQ_STATE_CANCELLED\n");
			break;

		default:
//			_d(L"[snmp] get request failed. etc..\n");
			break;
		}

		m_csComm.Unlock();
		return false;
	}
	else {
		int iCount = getReq.getResultCount();
		for (int i = 0; i<getReq.getResultCount(); i++) {
			SNMPObject *pRes = getReq.getResult(i);
			int iRecvType = pRes->getSyntax();
			switch (iRecvType) {
			case SNMP_INT:
			case SNMP_CNTR64:
			case SNMP_CNTR32:
			case SNMP_GAUGE32:
			case SNMP_UINT32:
			case SNMP_TIMETICKS:			
				m_i64Result = pRes->getValueAsInteger();
				m_iResultType = 0;
				break;
											
			case SNMP_OCTETS:
			case SNMP_OPAQUE:
			case SNMP_BITS:
			case SNMP_OID:
			case SNMP_IPADDR:
			case SNMP_NULL:
				//sprintf(m_strResult, "%s", pRes->getValueAsOctets(&nSize));
				sprintf(m_strResult, "%s", pRes->getValueAsString());
				m_iResultType = 1;
				break;

			default:
				m_iResultType = -1;
				break;
			}			
			m_csComm.Unlock();
			return true;
		}
	}
		
	m_csComm.Unlock();
	return true;
}

void *CCommSNMP::GetFirstTrapMsg()
{
	void *pMsg = NULL;

	int nCount = GetCount();
	if (nCount > 0) {		
		m_csTrap.Lock();
		POSITION pos1, pos2;
		pos1 = GetHeadPosition();
		if (pos1) {
			pos2 = pos1;
			pMsg = GetNext(pos1);
			RemoveAt(pos2);
		}
		m_csTrap.Unlock();
	}

	return pMsg;
}

void CCommSNMP::ClearTrapMsgList()
{
	m_csTrap.Lock();
	POSITION pos = GetHeadPosition();
	while (pos) {
		stWarningMsg *pMsg = (stWarningMsg*)GetNext(pos);
		SAFE_DELETE(pMsg);
	}

	RemoveAll();
	m_csTrap.Unlock();
}

void CCommSNMP::ClearSocket()
{
	if (m_sdUDP != INVALID_SOCKET) {
		shutdown(m_sdUDP, 2);
		closesocket(m_sdUDP);
	}
}

bool CCommSNMP::CreateSocket()
{
	struct sockaddr_in sin;
	int nTemp = 1;

	struct timeval tv;

	ClearSocket();

	m_sdUDP = socket(AF_INET, SOCK_DGRAM, 0);
	if (m_sdUDP == INVALID_SOCKET) {
		_d(_T("[stcServer] Invalid socket\n"));
		return false;
	}

	memset(&sin, 0, sizeof(struct sockaddr_in));

	sin.sin_family = AF_INET;
	if (m_strRecvIP.IsEmpty()) {
		sin.sin_addr.s_addr = htonl(INADDR_ANY);//htonl(INADDR_ANY);
	}
	else {
		char strIpAddr[100];
		UnicodeToMByte(m_strRecvIP, strIpAddr, 100);
		sin.sin_addr.s_addr = inet_addr(strIpAddr);				
	}
	sin.sin_port = htons(162);

	if (setsockopt(m_sdUDP, SOL_SOCKET, SO_REUSEADDR, (const char*)&nTemp, sizeof(nTemp)) < 0) {
		_d(_T("[stcServer] Failed to setsockopt\n"));
		return false;
	}

	if (bind(m_sdUDP, (struct sockaddr *)&sin, sizeof(struct sockaddr)) < 0) {
		int nErr = WSAGetLastError();
		_d(_T("[stcServer] Failed to bind socket (%d)\n"), nErr);
		return false;
	}

	tv.tv_sec = 5;
	tv.tv_usec = 0;
	if (setsockopt(m_sdUDP, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv))< 0) {
		int nErr = WSAGetLastError();
		_d(_T("[stcServer] Failed to socket opt (%d)\n"), nErr);
		return false;
	}

	return true;
}

void CCommSNMP::Run()
{
	///> 트랩메시지 수신
	struct sockaddr_in sin;
	char buf[64 * 1024];

#if ENABLE_TRAP_MSG
	_d(_T("[StcServer] start Recv Trap\n"));
#endif
	while (!m_bExit) {
		if (m_sdUDP == INVALID_SOCKET) {
			if (CreateSocket() == false) {
				_d(_T("[error][StcServer] 트랩용 소켓 생성 실패\n"));
				return;
			}
		}

		int nAddrLen = sizeof(struct sockaddr_in);
		int nRecv = recvfrom(m_sdUDP, buf, 64 * 1024, 0, (sockaddr *)&sin, &nAddrLen);
		if (nRecv > 0) {
#if ENABLE_TRAP_MSG
			_d(_T("[MRT] 트랩메시지 수신 size : %d\n"), nRecv);
#endif
			if (ParseTrapMsg((byte *)buf, nRecv, inet_ntoa(sin.sin_addr))) {
				CString strData;

				///> 180305 - 예외처리 추가. 쓰레기 트랩이 날라오는 경우가 있네
				/*strData.Format(L"%s", m_stTrapMsg.strDescription);
				if (strData.IsEmpty()) {
				continue;
				}*/

				///> 메인 리스트에 추가
				stTrapMsg *pMsg = new stTrapMsg;
				
				memcpy(pMsg, &m_stTrapMsg, sizeof(stTrapMsg));
				
				m_csTrap.Lock();
				AddTail(pMsg);
				m_csTrap.Unlock();						
			}
		}
	}
}


#if 0
bool CCommSNMP::SnmpGetForString(char *ip, char *comm, char *oids, int tmo)
{
	m_csComm.Lock();

	SNMPManager::getManager()->startup();
	SNMPRemoteAgent ra(ip, comm, 161);

	SNMPRemoteAgentGetRequest getReq(oids);

	getReq.execute(&ra);
	getReq.wait(tmo);

	if (!getReq.succeeded()) {
		int iState = getReq.GetState();
		int iError = getReq.getError();

		switch (iState) {
		case SNMP_RAREQ_STATE_NONE:
			//			_d(L"[snmp] get request failed. SNMP_RAREQ_STATE_NONE\n");
			break;

		case SNMP_RAREQ_STATE_ONGOING:
			//			_d(L"[snmp] get request failed. SNMP_RAREQ_STATE_ONGOING\n");
			break;

		case SNMP_RAREQ_STATE_TIMEDOUT:
			//			_d(L"[snmp] get request failed. time out\n");
			break;

		case SNMP_RAREQ_STATE_SUCCEEDED:
			//			_d(L"[snmp] get request failed. SNMP_RAREQ_STATE_SUCCEEDED\n");
			break;

		case SNMP_RAREQ_STATE_ERROR:
			// getReq.getErrorAsString()
			//			_d(L"[snmp] get request failed. SNMP_RAREQ_STATE_ERROR\n");
			if (iError == SNMP_ERROR_TOOBIG) {
				//				_d(L"[snmp] error SNMP_ERROR_TOOBIG\n");
			}
			else if (iError == SNMP_ERROR_NOSUCHNAME) {
				//				_d(L"[snmp] error SNMP_ERROR_NOSUCHNAME\n");
			}
			else if (iError == SNMP_ERROR_BADVALUE) {
				//				_d(L"[snmp] error SNMP_ERROR_BADVALUE\n");
			}
			else if (iError == SNMP_ERROR_READONLY) {
				//				_d(L"[snmp] error SNMP_ERROR_READONLY\n");
			}
			else if (iError == SNMP_ERROR_GENERR) {
				//				_d(L"[snmp] error SNMP_ERROR_GENERR\n");
			}
			break;

		case SNMP_RAREQ_STATE_CANCELLED:
			//			_d(L"[snmp] get request failed. SNMP_RAREQ_STATE_CANCELLED\n");
			break;

		default:
			//			_d(L"[snmp] get request failed. etc..\n");
			break;
		}

		m_csComm.Unlock();
		return false;
	}
	else {
		for (int i = 0; i<getReq.getResultCount(); i++) {
			SNMPObject *pRes = getReq.getResult(i);
			int nSize = 0;
			int nTest = 0;
			//sprintf(result, "%s", pRes->kmkgetValueAsOctets(&nSize, &nTest));
			//	sprintf(m_strResult, "%s", pRes->getValueAsOctets(&nSize));
			sprintf(m_strResult, "%s", pRes->getValueAsString());
			m_strResult[nSize] = 0;
			//_d("[test] %s, size: %d... test: %d\n", result, nSize, nTest);
			m_csComm.Unlock();
			return true;
		}
	}

	m_strResult[0] = 0;
	m_csComm.Unlock();
	return true;
}
#endif