#include "stdafx.h"
#include "SnmpTrapParser.h"

CSnmpTrapParser::CSnmpTrapParser()
{
	m_strTrapTypeCM5000 = L"1.3.6.1.4.1.27338.4.9.4.";
	m_StrTrapTypeDR5000 = L"1.3.6.1.4.1.27338.5.11.4.";

	m_strCM5000Descr[1] = L"Trap emitted when a hardware error is detected";
	m_strCM5000Descr[2] = L"Trap emitted when a power supply unit is failing";
	m_strCM5000Descr[3] = L"Trap emitted when an extension hardware error is detected";
	m_strCM5000Descr[4] = L"Trap emitted when an ethernet link is down";
	m_strCM5000Descr[5] = L"Trap emitted when the video signal is missing";
	m_strCM5000Descr[6] = L"Trap emitted when the video signal is incompatible";
	m_strCM5000Descr[7] = L"Trap emitted when the video signal is inactive";
	m_strCM5000Descr[8] = L"Trap emitted when the audio signal is missing";
	m_strCM5000Descr[9] = L"Trap emitted when the audio signal is incompatible";
	m_strCM5000Descr[10] = L"Trap emitted when the audio signal is inactive";
	m_strCM5000Descr[11] = L"Trap emitted when the data signal is missing";
	m_strCM5000Descr[12] = L"Trap emitted when the TS input signal is missing";
	m_strCM5000Descr[13] = L"Trap emitted when an used as an output network link is down";
	m_strCM5000Descr[14] = L"Trap emitted when the modulator is muted";
	m_strCM5000Descr[15] = L"Trap emitted when a modulator hardware error is detected";
	m_strCM5000Descr[16] = L"Trap emitted when the modulator external clock is missing";
	m_strCM5000Descr[17] = L"Trap emitted when the system is over heating";
	m_strCM5000Descr[18] = L"Trap emitted when a fan is failing";
	m_strCM5000Descr[19] = L"Trap emitted when the audio signal decoding is failing";
	m_strCM5000Descr[20] = L"Trap emitted when a Zixi output cannot connect to the remote host";
	m_strCM5000Descr[21] = L"Trap emitted when the audio Dolby E signal is invalid";
	m_strCM5000Descr[22] = L"Trap emitted when the modulator TS input signal is missing";
	m_strCM5000Descr[23] = L"Trap emitted when the clock is incompatible";
	m_strCM5000Descr[24] = L"Trap emitted when the clock is missing";
	m_strCM5000Descr[25] = L"Trap emitted when a transport is muted";

	m_strDR5000Descr[1] = L"Trap emitted when an audio PID can not be found";
	m_strDR5000Descr[2] = L"Trap emitted when there is too much audio pid to decode";
	m_strDR5000Descr[3] = L"Trap emitted when there is too much audio pid to pass thru";
	m_strDR5000Descr[4] = L"Trap emitted when there is no signal on the output";
	m_strDR5000Descr[5] = L"Trap emitted when Failed to open input interface";
	m_strDR5000Descr[6] = L"Trap emitted when there is no signal on the input";
	m_strDR5000Descr[7] = L"Trap emitted when there is no video or pid in the selected service";
	m_strDR5000Descr[8] = L"Trap emitted when the engine starts";
	m_strDR5000Descr[9] = L"Trap emitted when there is a discontinuity on an audio pid";
	m_strDR5000Descr[10] = L"Trap emitted when there is a discontinuity on an video pid";
	m_strDR5000Descr[11] = L"Trap emitted when there is a discontinuity on an data pid";
	m_strDR5000Descr[12] = L"Trap emitted when there is a discontinuity on another pid";
	m_strDR5000Descr[13] = L"Trap emitted when the delected program start being scrambled";
	m_strDR5000Descr[14] = L"Trap emitted when a fan stops spining";
	m_strDR5000Descr[15] = L"Trap emitted when the temperature is very high";
	m_strDR5000Descr[16] = L"Trap emitted when we are unable to read from an ethernet interface";
	m_strDR5000Descr[17] = L"Trap emitted when we are unable to send on an ethernet interface";
	m_strDR5000Descr[18] = L"Trap emitted when Dolby E data can't be found in Dolby E Pass-through mod";
	m_strDR5000Descr[19] = L"Trap emitted when Dolby E is moving";
	m_strDR5000Descr[20] = L"Trap emitted when the selected pair doesn't exist in s302m";
	m_strDR5000Descr[21] = L"Trap emitted when a Power Supply is in failure";
	m_strDR5000Descr[22] = L"Trap emitted when there is too much AC3 audio pid to decode";
	m_strDR5000Descr[23] = L"Trap emitted when there is too much EAC3 audio pid to decode";
	m_strDR5000Descr[24] = L"Trap emitted when Jitter is excessive (in IP or Zixi input modes only)";
	m_strDR5000Descr[25] = L"Trap emitted when SMPTE 2022-7 network skew is higher than the configured one";
	m_strDR5000Descr[26] = L"Trap emitted when SMPTE 2022-7 Input is not protected";
	m_strDR5000Descr[27] = L"Trap emitted when error rate of pre-FEC RTP/IP input is excessive";
	m_strDR5000Descr[28] = L"Trap emitted when error rate of post-FEC RTP/IP input is excessive";
	m_strDR5000Descr[29] = L"Trap emitted when Sat input is not locked";
	m_strDR5000Descr[30] = L"Trap emitted when Sat input C/N margin is insufficient";
	m_strDR5000Descr[31] = L"Trap emitted when Sat input Power is insufficient";
	m_strDR5000Descr[32] = L"Trap emitted when Sat input Bit Error Rate is excessive";
	m_strDR5000Descr[33] = L"Trap emitted when Zixi input is not connected";
	m_strDR5000Descr[34] = L"Trap emitted when Zixi input error rate is excessive";
	m_strDR5000Descr[35] = L"Trap emitted when zixi cannot lock because of AES decryption";
}


CSnmpTrapParser::~CSnmpTrapParser()
{
}

void CSnmpTrapParser::ParseCM5000(CString strOID)
{
	CString strRemain;

	//> 분석할 OID 만들기
	int nOIDLength = strOID.GetLength();
	int nDiff = m_strTrapTypeCM5000.GetLength();
	strRemain.Format(L"%s", strOID.Right(nOIDLength-nDiff));
//	_d(L"[parser] cm5000 remain oid: %s, oid: %s\n", strRemain, strOID);

	//> 1. 메시지 항목, 2. 데이터 타입
	CString token;
	int nMsgIndex = -1;
	int nDataIndex = -1;
	int nIndex = 0;
	int nPos = 0;
	while ((token = strRemain.Tokenize(_T("."), nPos)) != _T("")) {
		if (nIndex == 0) {
			nMsgIndex = _ttoi(token);
		}
		else {
			nDataIndex = _ttoi(token);
			if ((nMsgIndex >= 1) && (nDataIndex >= 1) ) {
				m_stTrapMsg.nData[nDataIndex] = m_iValue;
			}
		}
		nIndex++;
	}	

	m_stTrapMsg.nServerType = 0;
	m_stTrapMsg.nMsgIndex = nMsgIndex;

//	_d(L"[parser] cm 5000\n");
}
void CSnmpTrapParser::ParseDR5000(CString strOID)
{
	CString strRemain;
	int nOIDLength = strOID.GetLength();
	int nDiff = m_StrTrapTypeDR5000.GetLength();
	strRemain.Format(L"%s", strOID.Right(nOIDLength - nDiff));

	CString token;
	int nMsgIndex = -1;
	int nDataIndex = -1;
	int nIndex = 0;
	int nPos = 0;
	while ((token = strRemain.Tokenize(_T("."), nPos)) != _T("")) {
		if (nIndex == 0) {
			nMsgIndex = _ttoi(token);
		}
		else {
			nDataIndex = _ttoi(token);
			if ((nMsgIndex >= 1) && (nDataIndex >= 1)) {
				m_stTrapMsg.nData[nDataIndex] = m_iValue;
			}
		}
		nIndex++;
	}

	m_stTrapMsg.nServerType = 1;
	m_stTrapMsg.nMsgIndex = nMsgIndex;	
}

void CSnmpTrapParser::WriteTrapMsg(CString strOID, int iDataType)
{
	///> OID 분석하여 메시지 종류 체크
	int nFind = strOID.Find(m_strTrapTypeCM5000);
	if (nFind >= 0) {
		//_d(L"[Parser] type CM5000. oid: %s\n", strOID);
		ParseCM5000(strOID);
		return;
	} 
	
	nFind = strOID.Find(m_StrTrapTypeDR5000);
	if (nFind >= 0) {
		//_d(L"[Parser] type DR5000. oid: %s\n", strOID);
		ParseDR5000(strOID);
		return;
	}

#if ENABLE_TRAP_MSG
	_d(L"[Parser] Not Find type..oid: %s\n", strOID);
#endif
	return;		
}

/**
* @ingroup SnmpParser
* @brief 패킷을 파싱하여서 내부 변수에 패킷 데이터를 저장한다.
* @param pszPacket		패킷
* @param iPacketLen	패킷 길이
* @returns 성공하면 파싱한 패킷 길이를 리턴하고 실패하면 -1 을 리턴한다.
*/
int CSnmpTrapParser::ParsePacket(const char * pszPacket, int iPacketLen)
{
	int iPos = 0, n;
	uint8_t	cType;
	CString strOID = L"";

	int nMsgSize = 0;
	
	iPos = ParseHeader(pszPacket, iPacketLen, nMsgSize);
	if (iPos == -1) return -1;

	for (int i = 0; i < nMsgSize; ) {
		cType = pszPacket[iPos];
#if ENABLE_TRAP_MSG
		_d(L"type: %02x\n", cType);
#endif
		switch (cType)
		{
		case ASN_TYPE_INT:
		case ASN_TYPE_COUNTER_32:
		case ASN_TYPE_GAUGE_32:
		case ASN_TYPE_TIMETICKS:
			n = ParseToInt(pszPacket + iPos, iPacketLen - iPos);
			if (n != -1) {
				if (strOID.IsEmpty() == false) {
					WriteTrapMsg(strOID, TRAP_DATA_TYPE_INT);
				}				
			}
			break;
		case ASN_TYPE_COUNTER_64:
			n = ParseToLong(pszPacket + iPos, iPacketLen - iPos);
			if (n != -1) {
				CString strData;
				strData.Format(L"%I64d", m_i64Value);				
			}
			break;
		case ASN_TYPE_OCTET_STR:
		case ASN_TYPE_IP_ADDRESS:
			n = ParseToString(pszPacket + iPos, iPacketLen - iPos);
			if (n != -1) {
				if (strOID.IsEmpty() == false) {
					WriteTrapMsg(strOID, TRAP_DATA_TYPE_STRING);
				}
			}
			break;
		case ASN_TYPE_OID:
			n = ParseToOid(pszPacket + iPos, iPacketLen - iPos);
			if (n != -1) {				
				strOID.Format(L"%s", m_strValue);				
			}
			break;
		case ASN_TYPE_NULL:
		case ASN_TYPE_NO_SUCH_OBJECT:
		case ASN_TYPE_NO_SUCH_INSTANCE:
			n = ParseToNull(pszPacket + iPos, iPacketLen - iPos);			
			break;
		case ASN_TYPE_COMPLEX:
		case SNMP_CMD_GET:
		case SNMP_CMD_GET_NEXT:
		case SNMP_CMD_TRAP:
		case SNMP_CMD_RESPONSE:
		case SNMP_CMD_REPORT:
			n = ParsePacket(pszPacket + iPos, iPacketLen - iPos);
			break;
		default:
			_d(_T("%s type(%02x) is not defined\n"), cType);
			return -1;
		}
				
		if (n == -1) {			
			return -1;
		}

		iPos += n;
		i += n;
	}
	
	return iPos;
}

bool CSnmpTrapParser::ParseTrapMsg(byte *pBuff, int nLen, char *pAgent)
{
	byte *ps = pBuff;	
	bool bResult = false;
	int nTotalLen = 0;
	int nElapsed = 0;	//> 다음 항목의 인덱스를 위해 처음부터 버퍼 증가량
	int nVer = 0;

	char community[32];
	memset(community, 0, 32);

	memset(&m_stTrapMsg, 0, sizeof(stTrapMsg));
		
	//> 1. Message Header	
	if (ps[1] & 0x80) {
		nTotalLen = ((ps[1] & 0x7f) << 7) | (ps[2] & 0x7f);
		nElapsed += 3;
	}
	else {
		nTotalLen = ps[1];
		nElapsed += 2;
	}
	ps = pBuff + nElapsed;	

	//> 1.1 Version Number
	if (ps[2] == 0x1) {
		nVer = 2;
#if ENABLE_TRAP_MSG
		_d(_T("[MRT] SNMP v2 detected\n"));
#endif
	}
	else if (ps[2] == 0x0) {
		nVer = 1;
#if ENABLE_TRAP_MSG
		_d(_T("[MRT] SNMP v1 detected\n"));
#endif
	}
	else {
#if ENABLE_TRAP_MSG
		_d(_T("[MRT] SNMP v3 detected... not implemented yet, so discard it\n"));
#endif
		return false;
	}
	nElapsed += 3;
	ps = pBuff + nElapsed;

	//> 1.2 community name		
	for (int i = 0; i<ps[1]; i++) {
		community[i] = ps[2 + i];
	}
	nElapsed += (2 + ps[1]);

	MByteToUnicode(pAgent, m_stTrapMsg.strServerIP, 100);
	MByteToUnicode(community, m_stTrapMsg.strCummnity, 100);
#if ENABLE_TRAP_MSG
	_d(_T("[Trap] ip: %s, community: %s\n"), m_stTrapMsg.strServerIP, m_stTrapMsg.strCummnity);
#endif

	//> 메시지 시간정보
	CTime curtime = CTime::GetCurrentTime();
	//> 시간정보
	_stprintf(m_stTrapMsg.strMsgTime, _T("%04d-%02d-%02d_%02d:%02d:%02d"),
		curtime.GetYear(), curtime.GetMonth(), curtime.GetDay(), curtime.GetHour(), curtime.GetMinute(), curtime.GetSecond());
	
	if (nVer == 1) {
		bResult = ParseTrapv1(pBuff + nElapsed, nLen - nElapsed);
	}
	else if (nVer == 2) {			
		bResult = ParsePacket((const char*)(pBuff + nElapsed), nLen - nElapsed);

		//> 메시지 정보 입력
		int nMsgIndex = m_stTrapMsg.nMsgIndex;
		if (nMsgIndex > 0) {
			if (m_stTrapMsg.nServerType == 0) {
				//> 180615 - 문제발생 장비 표시
				//> 인코더 파워, output link down
				if ( (nMsgIndex == 2) || (nMsgIndex == 13) ) {
					_stprintf(m_stTrapMsg.strDescription, L"%s (%d)", m_strCM5000Descr[nMsgIndex], m_stTrapMsg.nData[4]);
				} 
				else {
					_stprintf(m_stTrapMsg.strDescription, L"%s", m_strCM5000Descr[nMsgIndex]);
				}
			}
			else if (m_stTrapMsg.nServerType == 1) {
				//> output link down 
				if (nMsgIndex == 17) {
					_stprintf(m_stTrapMsg.strDescription, L"%s (%d)", m_strDR5000Descr[nMsgIndex], m_stTrapMsg.nData[4]);
				}
				else {
					_stprintf(m_stTrapMsg.strDescription, L"%s", m_strDR5000Descr[nMsgIndex]);
				}
			}
		}
	}

	return bResult;
}

bool CSnmpTrapParser::ParseTrapv1(byte *pBuff, int nLen)
{
	byte *ps = pBuff;
	int nElapsed = 0;
	int nTotalLen = 0;

	//> 2. protocol data unit
	//> 2.1 pdu type - pdu 종류
	if ((ps[0] & 0xf) != 0x4) {
#if ENABLE_TRAP_MSG
		_d(_T("[MRT] This is not a TRAP...(%02x)\n"), ps[0]);
#endif
		return false;
	}
	if (ps[1] & 0x80) {
		nTotalLen = ((ps[1] & 0x7f) << 7) | (ps[2] & 0x7f);
		nElapsed += 3;
	}
	else {
		nTotalLen = ps[1];
		nElapsed += 2;
	}
	ps = pBuff + nElapsed;

	//> 2.2 enterprise - 회사명
	
	//> 2.3 agent addr - 에이전트 주소
	//> 2.4 generic trap - 트랩 일반 코드
	//> 2.5 specific trap = 세부 트랩 코드
	//> 2.6 time stamp - 타입스템프 (시간값 확인)
	//> 2.7. variable bindings = pdu 변수 바인딩

	return true;
}


#if 0
int nIndex = 0;
///> Encoder (cm5000) Trap
//> system hardware
wsprintf(m_stTrapMagList[nIndex].strOID, L"1.3.6.1.4.1.27338.4.9.4.1");
nIndex++;

//> system power supply unit
wsprintf(m_stTrapMagList[nIndex].strOID, L"1.3.6.1.4.1.27338.4.9.4.2");
nIndex++;

//> system hardware extension
wsprintf(m_stTrapMagList[nIndex].strOID, L"1.3.6.1.4.1.27338.4.9.4.3");
nIndex++;

//> system ethernet link down
wsprintf(m_stTrapMagList[nIndex].strOID, L"1.3.6.1.4.1.27338.4.9.4.4");
nIndex++;

//> video signal missing
wsprintf(m_stTrapMagList[nIndex].strOID, L"1.3.6.1.4.1.27338.4.9.4.5");
nIndex++;

//> video signal incompatible
wsprintf(m_stTrapMagList[nIndex].strOID, L"1.3.6.1.4.1.27338.4.9.4.6");
nIndex++;

//> video signal inactive 
wsprintf(m_stTrapMagList[nIndex].strOID, L"1.3.6.1.4.1.27338.4.9.4.7");
nIndex++;

//> audio signal missing
wsprintf(m_stTrapMagList[nIndex].strOID, L"1.3.6.1.4.1.27338.4.9.4.8");
nIndex++;

//> audio signal incompatible
wsprintf(m_stTrapMagList[nIndex].strOID, L"1.3.6.1.4.1.27338.4.9.4.9");
nIndex++;

//> audio signal inactive
wsprintf(m_stTrapMagList[nIndex].strOID, L"1.3.6.1.4.1.27338.4.9.4.10");
nIndex++;

//> data signal missing
wsprintf(m_stTrapMagList[nIndex].strOID, L"1.3.6.1.4.1.27338.4.9.4.11");
nIndex++;

//> ts input signal missing
wsprintf(m_stTrapMagList[nIndex].strOID, L"1.3.6.1.4.1.27338.4.9.4.12");
nIndex++;

//> output network Link down
wsprintf(m_stTrapMagList[nIndex].strOID, L"1.3.6.1.4.1.27338.4.9.4.13");
nIndex++;

//> system overheat
wsprintf(m_stTrapMagList[nIndex].strOID, L"1.3.6.1.4.1.27338.4.9.4.17");
nIndex++;

//> system fan
wsprintf(m_stTrapMagList[nIndex].strOID, L"1.3.6.1.4.1.27338.4.9.4.18");
nIndex++;

//> output zixi connection failure
wsprintf(m_stTrapMagList[nIndex].strOID, L"1.3.6.1.4.1.27338.4.9.4.20");
nIndex++;

//> click signal incompatible
wsprintf(m_stTrapMagList[nIndex].strOID, L"1.3.6.1.4.1.27338.4.9.4.23");
nIndex++;

//> clock signal missing
wsprintf(m_stTrapMagList[nIndex].strOID, L"1.3.6.1.4.1.27338.4.9.4.24");
nIndex++;

//> output muted 
wsprintf(m_stTrapMagList[nIndex].strOID, L"1.3.6.1.4.1.27338.4.9.4.25");
nIndex++;

///> decoder (dr5000)
//> missing audio pid
wsprintf(m_stTrapMagList[nIndex].strOID, L"1.3.6.1.4.1.27338.5.11.4.1");
nIndex++;

//> audio decoders limit exceeded
wsprintf(m_stTrapMagList[nIndex].strOID, L"1.3.6.1.4.1.27338.5.11.4.2");
nIndex++;

//> audio pass thru limit excceeded
wsprintf(m_stTrapMagList[nIndex].strOID, L"1.3.6.1.4.1.27338.5.11.4.3");
nIndex++;

//> output activity missing
wsprintf(m_stTrapMagList[nIndex].strOID, L"1.3.6.1.4.1.27338.5.11.4.4");
nIndex++;

//> ip input failure
wsprintf(m_stTrapMagList[nIndex].strOID, L"1.3.6.1.4.1.27338.5.11.4.5");
nIndex++;

//> no input activity
wsprintf(m_stTrapMagList[nIndex].strOID, L"1.3.6.1.4.1.27338.5.11.4.6");
nIndex++;

//> configuration error
wsprintf(m_stTrapMagList[nIndex].strOID, L"1.3.6.1.4.1.27338.5.11.4.7");
nIndex++;

//> engine service started
wsprintf(m_stTrapMagList[nIndex].strOID, L"1.3.6.1.4.1.27338.5.11.4.8");
nIndex++;

//> ts discontinuity on audio pid
wsprintf(m_stTrapMagList[nIndex].strOID, L"1.3.6.1.4.1.27338.5.11.4.9");
nIndex++;

//> ts discontinuity on video pid
wsprintf(m_stTrapMagList[nIndex].strOID, L"1.3.6.1.4.1.27338.5.11.4.10");
nIndex++;

//> ts discontinuity on data pid
wsprintf(m_stTrapMagList[nIndex].strOID, L"1.3.6.1.4.1.27338.5.11.4.11");
nIndex++;

//> ts discontinuity on other pid
wsprintf(m_stTrapMagList[nIndex].strOID, L"1.3.6.1.4.1.27338.5.11.4.12");
nIndex++;

//> program is scrambled
wsprintf(m_stTrapMagList[nIndex].strOID, L"1.3.6.1.4.1.27338.5.11.4.13");
nIndex++;

//> fan failure
wsprintf(m_stTrapMagList[nIndex].strOID, L"1.3.6.1.4.1.27338.5.11.4.14");
nIndex++;

//> critical temperature
wsprintf(m_stTrapMagList[nIndex].strOID, L"1.3.6.1.4.1.27338.5.11.4.15");
nIndex++;

//> input network link down
wsprintf(m_stTrapMagList[nIndex].strOID, L"1.3.6.1.4.1.27338.5.11.4.16");
nIndex++;

//> output network link down
wsprintf(m_stTrapMagList[nIndex].strOID, L"1.3.6.1.4.1.27338.5.11.4.17");
nIndex++;

//> dolbye missing
wsprintf(m_stTrapMagList[nIndex].strOID, L"1.3.6.1.4.1.27338.5.11.4.18");
nIndex++;

//> dolbye moving
wsprintf(m_stTrapMagList[nIndex].strOID, L"1.3.6.1.4.1.27338.5.11.4.19");
nIndex++;

//> dolbye inexisting pair
wsprintf(m_stTrapMagList[nIndex].strOID, L"1.3.6.1.4.1.27338.5.11.4.20");
nIndex++;

//> psu failure
wsprintf(m_stTrapMagList[nIndex].strOID, L"1.3.6.1.4.1.27338.5.11.4.21");
nIndex++;

//> Audio AC3 Decoders Limit excceeded
wsprintf(m_stTrapMagList[nIndex].strOID, L"1.3.6.1.4.1.27338.5.11.4.22");
nIndex++;

//> audio eac3 decoders limit excceeded
wsprintf(m_stTrapMagList[nIndex].strOID, L"1.3.6.1.4.1.27338.5.11.4.23");
nIndex++;

//> input ip zixi jitter
wsprintf(m_stTrapMagList[nIndex].strOID, L"1.3.6.1.4.1.27338.5.11.4.24");
nIndex++;

//> input ip 20227 skew
wsprintf(m_stTrapMagList[nIndex].strOID, L"1.3.6.1.4.1.27338.5.11.4.25");
nIndex++;

//> input ip 20227 unprotected
wsprintf(m_stTrapMagList[nIndex].strOID, L"1.3.6.1.4.1.27338.5.11.4.26");
nIndex++;

//> input ip rtp pre fec error rate
wsprintf(m_stTrapMagList[nIndex].strOID, L"1.3.6.1.4.1.27338.5.11.4.27");
nIndex++;

//> input ip rtp post fec error rate
wsprintf(m_stTrapMagList[nIndex].strOID, L"1.3.6.1.4.1.27338.5.11.4.28");
nIndex++;

//> input sat unlocked
wsprintf(m_stTrapMagList[nIndex].strOID, L"1.3.6.1.4.1.27338.5.11.4.29");
nIndex++;

//> input sat cn margin
wsprintf(m_stTrapMagList[nIndex].strOID, L"1.3.6.1.4.1.27338.5.11.4.30");
nIndex++;

//> input sat Power
wsprintf(m_stTrapMagList[nIndex].strOID, L"1.3.6.1.4.1.27338.5.11.4.31");
nIndex++;

//> input sat ber
wsprintf(m_stTrapMagList[nIndex].strOID, L"1.3.6.1.4.1.27338.5.11.4.32");
nIndex++;

//> input zixi unconnected
wsprintf(m_stTrapMagList[nIndex].strOID, L"1.3.6.1.4.1.27338.5.11.4.33");
nIndex++;

//> input zixi error rate
wsprintf(m_stTrapMagList[nIndex].strOID, L"1.3.6.1.4.1.27338.5.11.4.34");
nIndex++;

//> input zixi encryption
wsprintf(m_stTrapMagList[nIndex].strOID, L"1.3.6.1.4.1.27338.5.11.4.35");
nIndex++;
#endif