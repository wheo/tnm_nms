#pragma once
#include <stdint.h>
#include <string>

#define ASN_TYPE_BOOL				0x01
#define ASN_TYPE_INT				0x02
#define ASN_TYPE_BIT_STR			0x03
#define ASN_TYPE_OCTET_STR			0x04
#define ASN_TYPE_NULL				0x05
#define ASN_TYPE_OID				0x06
#define ASN_TYPE_SEQUENCE			0x10
#define ASN_TYPE_CONSTRUCTOR		0x20

#define ASN_TYPE_COMPLEX			0x30
#define ASN_TYPE_IP_ADDRESS			0x40
#define ASN_TYPE_COUNTER_32			0x41
#define ASN_TYPE_GAUGE_32			0x42
#define ASN_TYPE_TIMETICKS			0x43
#define ASN_TYPE_COUNTER_64			0x46
#define ASN_TYPE_NO_SUCH_OBJECT		0x80
#define ASN_TYPE_NO_SUCH_INSTANCE	0x81

#define SNMP_CMD_GET		0xA0
#define SNMP_CMD_GET_NEXT	0xA1
#define SNMP_CMD_RESPONSE	0xA2
#define SNMP_CMD_TRAP		0xA7
#define SNMP_CMD_REPORT		0xA8

class CAsnType
{
public:
	CAsnType();
	~CAsnType();

	virtual int ParsePacket(const char * pszPacket, int iPacketLen) = 0;

	int ParseHeader(const char *pszPacket, int nPacketLen, int &nMsgSize);
	
protected:
	int ParseInt(const char * pszPacket, int iPacketLen, uint8_t cLength, uint32_t & iValue);
	int ParseLong(const char * pszPacket, int iPacketLen, uint8_t cLength, uint64_t & iValue);

	int ParseToInt(const char * pszPacket, int iPacketLen);
	int ParseToLong(const char * pszPacket, int iPacketLen);
	int ParseToString(const char * pszPacket, int iPacketLen);
	int ParseToOid(const char * pszPacket, int iPacketLen);
	int ParseToNull(const char * pszPacket, int iPacketLen);

	/** 타입 */
	uint8_t		m_cType;
	
	/** 길이 */
	uint32_t	m_iLen;

	uint32_t	m_iValue;
	uint64_t	m_i64Value;	
	wchar_t		*m_strValue;
};

