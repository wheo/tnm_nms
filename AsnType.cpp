#include "stdafx.h"
#include "AsnType.h"

CAsnType::CAsnType()
{
	m_strValue = new wchar_t[10000];
}


CAsnType::~CAsnType()
{
	delete[] m_strValue;
}

/**
* @ingroup SnmpParser
* @brief ��Ŷ���� ������ �Ľ��Ѵ�.
* @param pszPacket		��Ŷ
* @param iPacketLen	��Ŷ ����
* @param cLength			��Ŷ�� ����� ������ ����
* @param iValue			������ ���� ����
* @returns �����ϸ� �Ľ��� ��Ŷ ���̸� �����ϰ� �׷��� ������ -1 �� �����Ѵ�.
*/
int CAsnType::ParseInt(const char * pszPacket, int iPacketLen, uint8_t cLength, uint32_t & iValue)
{
	if (iPacketLen < cLength) return -1;

	if (cLength == 1) {
		iValue = (uint8_t)pszPacket[0];
		return 1;
	}

	if (cLength == 2) {
		int16_t sTemp;

		memcpy(&sTemp, pszPacket, cLength);
		iValue = ntohs(sTemp);
		return 2;
	}

	if (cLength == 3) {
		int32_t iTemp = 0;

		memcpy(((char *)&iTemp) + 1, pszPacket, cLength);
		iValue = ntohl(iTemp);
		return 3;
	}

	if (cLength == 4) {
		int32_t iTemp;

		memcpy(&iTemp, pszPacket, cLength);
		iValue = ntohl(iTemp);
		return 4;
	}

	if (cLength == 5) {
		int32_t iTemp;

		memcpy(&iTemp, pszPacket + 1, cLength - 1);
		iValue = ntohl(iTemp);
		return 5;
	}

	return -1;
}

/**
* @ingroup SnmpParser
* @brief ��Ŷ���� ������ �Ľ��Ѵ�.
* @param pszPacket		��Ŷ
* @param iPacketLen	��Ŷ ����
* @param cLength			��Ŷ�� ����� ������ ����
* @param iValue			������ ���� ����
* @returns �����ϸ� �Ľ��� ��Ŷ ���̸� �����ϰ� �׷��� ������ -1 �� �����Ѵ�.
*/
int CAsnType::ParseLong(const char * pszPacket, int iPacketLen, uint8_t cLength, uint64_t & iValue)
{
	if (iPacketLen < cLength) return -1;

	if (cLength == 1) {
		iValue = (uint8_t)pszPacket[0];
		return 1;
	}

	if (cLength == 2) {
		int16_t sTemp;

		memcpy(&sTemp, pszPacket, cLength);
		iValue = ntohs(sTemp);
		return 2;
	}

	if (cLength == 3) {
		int32_t iTemp = 0;

		memcpy(((char *)&iTemp) + 1, pszPacket, cLength);
		iValue = ntohl(iTemp);
		return 3;
	}

	if (cLength == 4) {
		int32_t iTemp;

		memcpy(&iTemp, pszPacket, cLength);
		iValue = ntohl(iTemp);
		return 4;
	}

	if (cLength == 5) {
		int64_t iTemp = 0;

		memcpy(((char *)&iTemp) + 3, pszPacket, cLength);
		iValue = ntohll(iTemp);
		return 5;
	}

	if (cLength == 6) {
		int64_t iTemp = 0;

		memcpy(((char *)&iTemp) + 2, pszPacket, cLength);
		iValue = ntohll(iTemp);
		return 6;
	}

	if (cLength == 7) {
		int64_t iTemp = 0;

		memcpy(((char *)&iTemp) + 1, pszPacket, cLength);
		iValue = ntohll(iTemp);
		return 7;
	}

	if (cLength == 8) {
		int64_t iTemp = 0;

		memcpy(&iTemp, pszPacket, cLength);
		iValue = ntohll(iTemp);
		return 8;
	}

	if (cLength == 9) {
		int64_t iTemp = 0;

		memcpy(&iTemp, pszPacket + 1, 8);
		iValue = ntohll(iTemp);
		return 8;
	}

	return -1;
}

/**
* @ingroup SnmpParser
* @brief ��Ŷ�� �Ľ��Ͽ��� ASN.1 Ÿ�� �� body ���̸� �����Ѵ�.
* @param pszPacket		��Ŷ
* @param nPacketLen	��Ŷ ����
* @returns �����ϸ� �Ľ��� ��Ŷ ���̸� �����ϰ� �׷��� ������ -1 �� �����Ѵ�.
*/
int CAsnType::ParseHeader(const char *pszPacket, int nPacketLen, int &nMsgSize)
{
	int nPos = 0;
	
	m_cType = pszPacket[nPos++];
	uint32_t iDataSize = 0;
	nMsgSize = 0;

	if (pszPacket[nPos] & 0x80) {
		uint8_t cLength = pszPacket[nPos] & 0x7F;
		++nPos;

		int n = ParseInt(pszPacket + nPos, nPacketLen - nPos, cLength, iDataSize);
		nMsgSize = iDataSize;
		if (n == -1) return -1;

		nPos += n;
	}
	else {
		iDataSize = pszPacket[nPos++];
		nMsgSize = iDataSize;
	}

	return nPos;
}

/**
* @ingroup SnmpParser
* @brief ��Ŷ�� �Ľ��Ͽ��� ���� ������ ��Ŷ �����͸� �����Ѵ�.
* @param pszPacket		��Ŷ
* @param iPacketLen	��Ŷ ����
* @returns �����ϸ� �Ľ��� ��Ŷ ���̸� �����ϰ� �����ϸ� -1 �� �����Ѵ�.
*/
int CAsnType::ParseToInt(const char * pszPacket, int iPacketLen)
{
	int		iPos = 0;
	char	cLength;

	m_cType = pszPacket[iPos++];
	cLength = pszPacket[iPos++];

	int n = ParseInt(pszPacket + iPos, iPacketLen - iPos, cLength, m_iValue);
	if (n == -1) return -1;
#if ENABLE_TRAP_MSG
	_d(_T("[Asnint] parse Int : %d\n"), m_iValue);
#endif
	iPos += n;

	return iPos;
}

/**
* @ingroup SnmpParser
* @brief ��Ŷ�� �Ľ��Ͽ��� ���� ������ ��Ŷ �����͸� �����Ѵ�.
* @param pszPacket		��Ŷ
* @param iPacketLen	��Ŷ ����
* @returns �����ϸ� �Ľ��� ��Ŷ ���̸� �����ϰ� �����ϸ� -1 �� �����Ѵ�.
*/
int CAsnType::ParseToLong(const char * pszPacket, int iPacketLen)
{
	int		iPos = 0;
	char	cLength;

	m_cType = pszPacket[iPos++];
	cLength = pszPacket[iPos++];

	int n = ParseLong(pszPacket + iPos, iPacketLen - iPos, cLength, m_i64Value);
	if (n == -1) return -1;
#if ENABLE_TRAP_MSG
	_d(_T("[Asnlong] parse Long : %I64d\n"), m_i64Value);
#endif
	iPos += n;

	return iPos;
}

/**
* @ingroup SnmpParser
* @brief ��Ŷ�� �Ľ��Ͽ��� ���� ������ ��Ŷ �����͸� �����Ѵ�.
* @param pszPacket		��Ŷ
* @param iPacketLen	��Ŷ ����
* @returns �����ϸ� �Ľ��� ��Ŷ ���̸� �����ϰ� �����ϸ� -1 �� �����Ѵ�.
*/
int CAsnType::ParseToString(const char * pszPacket, int iPacketLen)
{
	int	iPos = 0;
	int nMsgSize = 0;
	iPos = ParseHeader(pszPacket, iPacketLen, nMsgSize);
	if (iPos == -1) return -1;

	std::string strData;
	strData.append(pszPacket + iPos, nMsgSize);
	/*wsprintf(m_strValue, L"");
	const char *pData = &pszPacket[iPos];*/
	//m_strValue.append(pszPacket + iPos, m_iLen);

	iPos += nMsgSize;
		
	if (nMsgSize > 0) {
		TCHAR strTemp[5000];
		memset(strTemp, 0, sizeof(strTemp));
		UTF8TOWide((char*)strData.c_str(), strTemp);
	//	UTF8TOWide((char*)strData.c_str(), m_strValue);
		wsprintf(m_strValue, L"%s", strTemp);
	}
#if ENABLE_TRAP_MSG	
	_d(_T("[AsnString] parse String : %s\n"), m_strValue);
#endif

	return iPos;
}

/**
* @ingroup SnmpParser
* @brief ��Ŷ�� �Ľ��Ͽ��� ���� ������ ��Ŷ �����͸� �����Ѵ�.
* @param pszPacket		��Ŷ
* @param iPacketLen	��Ŷ ����
* @returns �����ϸ� �Ľ��� ��Ŷ ���̸� �����ϰ� �����ϸ� -1 �� �����Ѵ�.
*/
int CAsnType::ParseToOid(const char * pszPacket, int iPacketLen)
{
	int			iPos = 0;
	uint8_t	cLength;
	char	szValue[512];
	int		iValueLen = 0;
	
	m_cType = pszPacket[iPos++];
	cLength = pszPacket[iPos++];

	memset(szValue, 0, sizeof(szValue));

	iValueLen = snprintf(szValue, sizeof(szValue), "%d.%d", pszPacket[iPos] / 40, pszPacket[iPos] % 40);

	char oids[256];
	memset(oids, 0, 256);

	const char *pt = &pszPacket[iPos];
	bool overrun = false;
	int temp = 0;
	int noid = 0;
	for (int i = 1; i < cLength; ++i) {
		if (pt[i] == 0x2b) {
			oids[0] = '.';
			noid = 1;
		}
		else if (pt[i] & 0x80) {
			overrun = true;
			temp = ((temp << 7) | (pt[i] & 0x7f));
		}
		else {
			if (overrun) {
				char stemp[256];
				temp = ((temp << 7) | (pt[i] & 0x7f));
				sprintf(stemp, "%d.", temp);
				sprintf(&oids[noid], "%s", stemp);
				temp = 0;
				noid += strlen(stemp);
			}
			else {
				char stemp[256];
				temp = (pt[i] & 0x7f);
				sprintf(stemp, "%d.", temp);
				sprintf(&oids[noid], "%s", stemp);
				noid += strlen(stemp);
				temp = 0;
			}
		}
	}

	//	m_strValue = szValue;
	//	m_strValue.assign(szValue);

#if 0
	char *strTemp = new char[5000];
	TCHAR strData[5000];
	CString strTemp2;
	sprintf(strTemp, "%s", m_strValue);
	_stprintf(strData, _T("%s"), m_strValue);
	strTemp2.Format(_T("%s"), m_strValue);
	MByteToUnicode(strTemp, strData, 5000);
	delete[] strTemp;
#endif
#if 0
	TCHAR strData[5000];
	UTF8TOWide((char*)m_strValue.c_str(), strData, 5000);
	_d(_T("[Asnoid] parse Oid : %s\n"), strData);
#endif
	TCHAR strData[5000];
	char strTemp[5000];
	memset(strData, 0, sizeof(strData));
	sprintf(strTemp, "%s.%s", szValue, oids);
	UTF8TOWide(strTemp, strData);
	wsprintf(m_strValue, L"%s", strData);
	//	m_strValue.Format(_T("%s"), strData);
#if ENABLE_TRAP_MSG
	_d(_T("[Asnoid] parse Oid : %s\n"), m_strValue);
#endif

	return 2 + cLength;
}

/**
* @ingroup SnmpParser
* @brief ��Ŷ�� �Ľ��Ͽ��� ���� ������ ��Ŷ �����͸� �����Ѵ�.
* @param pszPacket		��Ŷ
* @param iPacketLen	��Ŷ ����
* @returns �����ϸ� �Ľ��� ��Ŷ ���̸� �����ϰ� �����ϸ� -1 �� �����Ѵ�.
*/
int CAsnType::ParseToNull(const char * pszPacket, int iPacketLen)
{
	m_cType = pszPacket[0];

	return 2;
}