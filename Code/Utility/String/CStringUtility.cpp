#include "CStringUtility.h"
#include "crc.h"
#include <algorithm>
#include <ctime>

using namespace std;




uint64_t pack754(float80 f, unsigned bits, unsigned expbits)
{
	float80 fnorm;
	int shift;
	long long sign, exp, significand;
	unsigned significandbits = bits - expbits - 1; // -1 for sign bit

	if (f == 0.0) return 0; // get this special case out of the way

	// check sign and begin normalization
	if (f < 0) { sign = 1; fnorm = -f; }
	else { sign = 0; fnorm = f; }

	// get the normalized form of f and track the exponent
	shift = 0;
	while (fnorm >= 2.0) { fnorm /= 2.0; shift++; }
	while (fnorm < 1.0) { fnorm *= 2.0; shift--; }
	fnorm = fnorm - 1.0;

	// calculate the binary form (non-float) of the significand data
	significand = (long long) (fnorm * ((1LL << significandbits) + 0.5f));

	// get the biased exponent
	exp = shift + ((1 << (expbits - 1)) - 1); // shift + bias

	// return the final answer
	return (sign << (bits - 1)) | (exp << (bits - expbits - 1)) | significand;
}
float80 unpack754(uint64_t i, unsigned bits, unsigned expbits)
{
	float80 result;
	long long shift;
	unsigned bias;
	unsigned significandbits = bits - expbits - 1; // -1 for sign bit

	if (i == 0) return 0.0;

	// pull the significand
	result = (float80) (i&((1LL << significandbits) - 1)); // mask
	result /= (1LL << significandbits); // convert back to float
	result += 1.0f; // add the one back on

	// deal with the exponent
	bias = (1 << (expbits - 1)) - 1;
	shift = ((i >> significandbits)&((1LL << expbits) - 1)) - bias;
	while (shift > 0) { result *= 2.0; shift--; }
	while (shift < 0) { result /= 2.0; shift++; }

	// sign it
	result *= (i >> (bits - 1)) & 1 ? -1.0 : 1.0;

	return result;
}




vector<string>		CStringUtility::split(string& strString, string strDelimiter)
{
	if (strString.length() > 0 && strString.substr(strString.length() - 1, 1) == strDelimiter)
	{
		strString = strString.substr(0, strString.length() - 1);
	}

	vector<string> vecTokens;
	string strToken;
	uint32
		uiSeek = 0,
		uiFoundPosition = strString.find(strDelimiter, uiSeek);
	while (uiFoundPosition != string::npos)
	{
		strToken = strString.substr(uiSeek, uiFoundPosition - uiSeek);
		vecTokens.push_back(strToken);
		uiSeek = uiFoundPosition + strDelimiter.length();
		uiFoundPosition = strString.find(strDelimiter, uiSeek);
	}
	if (uiSeek != strString.length())
	{
		strToken = strString.substr(uiSeek, string::npos);
		vecTokens.push_back(strToken);
	}
	return vecTokens;
}

string				CStringUtility::join(vector<string>& vecTokens, string strDelimiter)
{
	if (vecTokens.size() == 0)
	{
		return "";
	}

	string strText;
	for (uint32 i = 0; i < vecTokens.size() - 1; i++)
	{
		strText += vecTokens[i] + strDelimiter;
	}
	strText += vecTokens[vecTokens.size() - 1];
	return strText;
}

string				CStringUtility::join(deque<string>& deqTokens, string strDelimiter)
{
	if (deqTokens.size() == 0)
	{
		return "";
	}

	string strText;
	for (uint32 i = 0; i < deqTokens.size() - 1; i++)
	{
		strText += deqTokens[i] + strDelimiter;
	}
	strText += deqTokens[deqTokens.size() - 1];
	return strText;
}

string				CStringUtility::replace(string& strString, string strFind, string strReplace)
{
	size_t uiPos = strString.find(strFind);
	while (uiPos != string::npos)
	{
		strString.replace(uiPos, strFind.length(), strReplace);
		uiPos = strString.find(strFind, uiPos + strReplace.length());
	}
	return strString;
}

string				CStringUtility::packUint64(uint64 uiULongLong, bool bBigEndian)
{
	return packUint32((uiULongLong >> 32) & 0xFFFFFFFF, bBigEndian) + packUint32(uiULongLong & 0xFFFFFFFF, bBigEndian);
}

string				CStringUtility::packUint32(uint32 uiULong, bool bBigEndian)
{
	char szULong[4];
	if (bBigEndian)
	{
		szULong[0] = (uint8)floor(uiULong / 16777216);
		szULong[1] = (uint8)floor(uiULong / 65536);
		szULong[2] = (uint8)floor(uiULong / 256);
		szULong[3] = (uint8)(uiULong % 256);
	}
	else
	{
		szULong[3] = (uint8)floor(uiULong / 16777216);
		szULong[2] = (uint8)floor(uiULong / 65536);
		szULong[1] = (uint8)floor(uiULong / 256);
		szULong[0] = (uint8)(uiULong % 256);
	}
	string strULong(szULong, 4);
	return strULong;
}

string				CStringUtility::packUint16(uint16 uiUShort, bool bBigEndian)
{
	char szUShort[2];
	if (bBigEndian)
	{
		szUShort[0] = (uint8)floor(uiUShort / 256);
		szUShort[1] = (uint8)(uiUShort % 256);
	}
	else
	{
		szUShort[1] = (uint8)floor(uiUShort / 256);
		szUShort[0] = (uint8)(uiUShort % 256);
	}
	string strUShort(szUShort, 2);
	return strUShort;
}

string				CStringUtility::packUint8(uint8 ucUChar)
{
	char szUChar[1];
	szUChar[0] = (uint8)ucUChar;
	string strUChar(szUChar, 1);
	return strUChar;
}

string				CStringUtility::packInt64(int64 iLongLong, bool bBigEndian)
{
	return packInt32((iLongLong >> 32) & 0xFFFFFFFF, bBigEndian) + packInt32(iLongLong & 0xFFFFFFFF, bBigEndian);
}

string				CStringUtility::packInt32(int32 iLong, bool bBigEndian)
{
	char szLong[4];
	if (bBigEndian)
	{
		szLong[0] = (iLong >> 24) & 0xFF;
		szLong[1] = (iLong >> 16) & 0xFF;
		szLong[2] = (iLong >> 8) & 0xFF;
		szLong[3] = iLong & 0xFF;
	}
	else
	{
		szLong[3] = (iLong >> 24) & 0xFF;
		szLong[2] = (iLong >> 16) & 0xFF;
		szLong[1] = (iLong >> 8) & 0xFF;
		szLong[0] = iLong & 0xFF;
	}
	string strLong(szLong, 4);
	return strLong;
}

string				CStringUtility::packInt16(int16 iShort, bool bBigEndian)
{
	char szLong[2];
	if (bBigEndian)
	{
		szLong[0] = (iShort >> 8) & 0xFF;
		szLong[1] = iShort & 0xFF;
	}
	else
	{
		szLong[1] = (iShort >> 8) & 0xFF;
		szLong[0] = iShort & 0xFF;
	}
	string strLong(szLong, 4);
	return strLong;
}

string				CStringUtility::packInt8(int8 iChar)
{
	char szChar[1];
	szChar[0] = (int8)iChar;
	string strChar(szChar, 1);
	return strChar;
}

string				CStringUtility::packFloat32(float32 fValue, bool bBigEndian)
{
	return packUint32((uint32)pack754_32(fValue), bBigEndian);
}

string				CStringUtility::packFloat64(float64 fValue, bool bBigEndian)
{
	return packUint64((uint64) pack754_64(fValue), bBigEndian);
}

string				CStringUtility::packVector2D(CVector2D& vecVector, bool bBigEndian)
{
	return CStringUtility::packFloat32(vecVector.m_x, bBigEndian)
		 + CStringUtility::packFloat32(vecVector.m_y, bBigEndian);
}

string				CStringUtility::packVector3D(CVector3D& vecVector, bool bBigEndian)
{
	return CStringUtility::packFloat32(vecVector.m_x, bBigEndian)
		 + CStringUtility::packFloat32(vecVector.m_y, bBigEndian)
		 + CStringUtility::packFloat32(vecVector.m_z, bBigEndian);
}

string				CStringUtility::packVector4D(CVector4D& vecVector, bool bBigEndian)
{
	return CStringUtility::packFloat32(vecVector.m_x, bBigEndian)
		 + CStringUtility::packFloat32(vecVector.m_y, bBigEndian)
		 + CStringUtility::packFloat32(vecVector.m_z, bBigEndian)
		 + CStringUtility::packFloat32(vecVector.m_w, bBigEndian);
}

string				CStringUtility::packVector4ui8(CVector4ui8& vecVector)
{
	return CStringUtility::packUint8(vecVector.m_x)
		+ CStringUtility::packUint8(vecVector.m_y)
		+ CStringUtility::packUint8(vecVector.m_z)
		+ CStringUtility::packUint8(vecVector.m_w);
}

string				CStringUtility::packVector4ui16(CVector4ui16& vecVector, bool bBigEndian)
{
	return CStringUtility::packUint16(vecVector.m_x, bBigEndian)
		+ CStringUtility::packUint16(vecVector.m_y, bBigEndian)
		+ CStringUtility::packUint16(vecVector.m_z, bBigEndian)
		+ CStringUtility::packUint16(vecVector.m_w, bBigEndian);
}

uint32		CStringUtility::unpackUint32(string& strData, bool bBigEndian)
{
	uint32 uiInt;
	if (bBigEndian)
	{
		uiInt =
			(((uint8)strData.c_str()[0]) * 16777216) +
			(((uint8)strData.c_str()[1]) * 65536) +
			(((uint8)strData.c_str()[2]) * 256) +
			((uint8)strData.c_str()[3]);
	}
	else
	{
		uiInt =
			(((uint8)strData.c_str()[3]) * 16777216) +
			(((uint8)strData.c_str()[2]) * 65536) +
			(((uint8)strData.c_str()[1]) * 256) +
			((uint8)strData.c_str()[0]);
	}
	return uiInt;
}

uint16		CStringUtility::unpackUint16(string& strData, bool bBigEndian)
{
	uint16 uiInt;
	if (bBigEndian)
	{
		uiInt =
			(((uint8)strData.c_str()[0]) * 256) +
			((uint8)strData.c_str()[1]);
	}
	else
	{
		uiInt =
			(((uint8)strData.c_str()[1]) * 256) +
			((uint8)strData.c_str()[0]);
	}
	return uiInt;
}

uint8		CStringUtility::unpackUint8(string& strData)
{
	return (uint8)strData.c_str()[0];
}

int32			CStringUtility::unpackInt32(string& strData, bool bBigEndian)
{
	int32 iInt;
	if (bBigEndian)
	{
		iInt =
			(((int8)strData.c_str()[0]) << 24) |
			(((int8)strData.c_str()[1]) << 16) |
			(((int8)strData.c_str()[2]) << 8) |
			((int8)strData.c_str()[3]);
	}
	else
	{
		iInt =
			(((int8)strData.c_str()[3]) << 24) |
			(((int8)strData.c_str()[2]) << 16) |
			(((int8)strData.c_str()[1]) << 8) |
			((int8)strData.c_str()[0]);
	}
	return iInt;
}

int16		CStringUtility::unpackInt16(string& strData, bool bBigEndian)
{
	int16 iInt;
	if (bBigEndian)
	{
		iInt =
			(((int8)strData.c_str()[0]) << 8) |
			((int8)strData.c_str()[1]);
	}
	else
	{
		iInt =
			(((int8)strData.c_str()[1]) << 8) |
			((int8)strData.c_str()[0]);
	}
	return iInt;
}

int8			CStringUtility::unpackInt8(string& strData)
{
	return (int8)strData.c_str()[0];
}

float32				CStringUtility::unpackFloat32(string& strData, bool bBigEndian)
{
	return (float32)unpack754_32(unpackUint32(strData, bBigEndian));
}

CVector2D			CStringUtility::unpackVector2D(std::string& strData, bool bBigEndian)
{
	return CVector2D(
		unpackFloat32(strData.substr(0, 4), bBigEndian),
		unpackFloat32(strData.substr(4, 4), bBigEndian)
	);
}

CVector3D			CStringUtility::unpackVector3D(std::string& strData, bool bBigEndian)
{
	return CVector3D(
		unpackFloat32(strData.substr(0, 4), bBigEndian),
		unpackFloat32(strData.substr(4, 4), bBigEndian),
		unpackFloat32(strData.substr(8, 4), bBigEndian)
	);
}

CVector4D			CStringUtility::unpackVector4D(std::string& strData, bool bBigEndian)
{
	return CVector4D(
		unpackFloat32(strData.substr(0, 4), bBigEndian),
		unpackFloat32(strData.substr(4, 4), bBigEndian),
		unpackFloat32(strData.substr(8, 4), bBigEndian),
		unpackFloat32(strData.substr(12, 4), bBigEndian)
	);
}

string				CStringUtility::toString(int iNumber)
{
	char szString[100];
	_itoa_s(iNumber, szString, 10);
	return string(szString);
}

string				CStringUtility::toString(uint32 uiNumber)
{
	char szString[100];
	_itoa_s(uiNumber, szString, 10);
	return string(szString);
}

string				CStringUtility::toString(size_t iNumber)
{
	char szString[100];
	_itoa_s(iNumber, szString, 10);
	return string(szString);
}

string				CStringUtility::toString(float32 fNumber)
{
	char szString[100];
	sprintf_s(szString, "%g", fNumber);
	return string(szString);
}

string				CStringUtility::toStringExtendedFloat(float32 fNumber)
{
	char szString[100];
	sprintf_s(szString, "%f", fNumber);
	return string(szString);
}

string				CStringUtility::toStringHex(uint32 uiNumber)
{
	char szString[100];
	sprintf_s(szString, "%X", uiNumber);
	return string(szString);
}

int					CStringUtility::toNumber(string& strText)
{
	return atoi(strText.c_str());
}

uint32				CStringUtility::toUint32(string& strString)
{
	return atoi(strString.c_str());
}

uint16				CStringUtility::toUint16(string& strString)
{
	return (uint16) atoi(strString.c_str());
}

uint8				CStringUtility::toUint8(string& strString)
{
	return (uint8) atoi(strString.c_str());
}

int32				CStringUtility::toInt32(string& strString)
{
	return atoi(strString.c_str());
}

int16				CStringUtility::toInt16(string& strString)
{
	return (int16) atoi(strString.c_str());
}

int8				CStringUtility::toInt8(string& strString)
{
	return (int8) atoi(strString.c_str());
}

float32				CStringUtility::toFloat32(string& strString)
{
	return (float32) atof(strString.c_str());
}

string				CStringUtility::trim(string& strString, string strTrimCharacters)
{
	if (strTrimCharacters == "")
	{
		return rtrim(ltrim(strString));
	}
	else
	{
		return rtrim(ltrim(strString, strTrimCharacters), strTrimCharacters);
	}
}

string				CStringUtility::ltrim(string& strString, string strTrimCharacters)
{
	if (strTrimCharacters == "")
	{
		uint32 iStart = 0;
		for (uint32 i = 0; i < strString.length(); i++)
		{
			if (strString.at(i) <= 32 || strString.at(i) >= 127)
			{
				iStart++;
			}
			else
			{
				break;
			}
		}
		return strString.substr(iStart);
	}
	else
	{
		uint32 iStart = 0;
		for (uint32 i = 0; i < strString.length(); i++)
		{
			if (strString.substr(i, strTrimCharacters.length()) == strTrimCharacters)
			{
				iStart += strTrimCharacters.length();
				i += strTrimCharacters.length() - 1;
			}
			else
			{
				break;
			}
		}
		return strString.substr(iStart);
	}
}

string				CStringUtility::rtrim(string& strString, string strTrimCharacters)
{
	if (strTrimCharacters == "")
	{
		uint32 iEnd = strString.length() - 1;
		for (int32 i = iEnd; i >= 0; i--)
		{
			if (strString.at(i) <= 32 || strString.at(i) >= 127)
			{
				iEnd--;
			}
			else
			{
				break;
			}
		}
		return strString.substr(0, iEnd + 1);
	}
	else
	{
		uint32 iEnd = strString.length() - strTrimCharacters.length();
		for (int32 i = iEnd; i >= 0; i--)
		{
			if (strString.substr(i, strTrimCharacters.length()) == strTrimCharacters)
			{
				iEnd -= strTrimCharacters.length();
				i -= strTrimCharacters.length() - 1;
			}
			else
			{
				break;
			}
		}
		return strString.substr(0, iEnd + 1);
	}
}

string				CStringUtility::rtrimFromLeft(string& strString)
{
	for (uint32 i = 0; i < strString.length(); i++)
	{
		if (strString.at(i) < 32 || strString.at(i) >= 127)
		{
			return strString.substr(0, i);
		}
	}
	return strString;
}

string				CStringUtility::zeroPad(string& strData, uint32 uiPadLength)
{
	uint32 uiOldLength = strData.size();
	strData.resize(uiPadLength);
	for (uint32 i = uiOldLength; i < uiPadLength; i++)
	{
		strData[i] = '\0';
	}
	return strData;
}

string				CStringUtility::zeroPad(uint32 uiPadLength)
{
	string strData = "";
	strData.resize(uiPadLength);
	for (uint32 i = 0; i < uiPadLength; i++)
	{
		strData[i] = '\0';
	}
	return strData;
}

string				CStringUtility::toUpperCase(string& strString)
{
	transform(strString.begin(), strString.end(), strString.begin(), ::toupper);
	return strString;
}

string				CStringUtility::toLowerCase(string& strString)
{
	transform(strString.begin(), strString.end(), strString.begin(), ::tolower);
	return strString;
}

string				CStringUtility::toTitleCase(string& strString)
{
	string strSpace = " ";
	vector<string> vecTokens = split(strString, strSpace);
	for (uint32 i = 0; i < vecTokens.size(); i++)
	{
		vecTokens[i] = toUpperCase(strString.substr(0, 1)) + toLowerCase(strString.substr(1));
	}
	return join(vecTokens, strSpace);
}

string				CStringUtility::getTimestampText(uint32 uiTimestamp)
{
	time_t uiTime;
	if (uiTimestamp == 0)
	{
		uiTime = time(0);
	}
	else
	{
		uiTime = uiTimestamp;
	}
	tm *pTime = localtime(&uiTime);

	char *pStr = new char[1024];
	strftime(pStr, 1024, "%d %B %Y %H:%M:%S", pTime);
	string strTimeString(pStr);
	delete[] pStr;
	return strTimeString;
}

string				CStringUtility::getDateTextForFolder(void)
{
	time_t t = time(0);
	tm *curTime = localtime(&t);

	char *pStr = new char[1024];
	strftime(pStr, 1024, "%d %B %Y", curTime);
	string strTimeString(pStr);
	delete[] pStr;
	return strTimeString;
}

string				CStringUtility::escapeMenuText(string& strText)
{
	string
		strAmp1 = "&",
		strAmp2 = "&&";
	return replace(strText, strAmp1, strAmp2);
}

bool				CStringUtility::isIn(string& strText, string strFind)
{
	return toUpperCase(strText).find(toUpperCase(strFind)) != string::npos;
}

string				CStringUtility::readStringUntilZero(string& strData)
{
	uint32 uiLength = strlen(strData.c_str());
	if (uiLength == 0)
	{
		return "";
	}
	return strData.substr(0, uiLength);
}

string				CStringUtility::readStringUntilSpace(string& strString)
{
	size_t iSeek = strString.find(' ', 0);
	if (iSeek == string::npos)
	{
		return strString;
	}
	else
	{
		return strString.substr(0, iSeek);
	}
}

string				CStringUtility::stripNumbersFromRight(string& strText)
{
	uint32 iEnd = strText.length() - 1;
	for (int32 i = strText.length() - 1; i >= 0; i--)
	{
		if (strText.c_str()[i] >= 48 && strText.c_str()[i] <= 57)
		{
			iEnd--;
		}
		else
		{
			break;
		}
	}
	return strText.substr(0, iEnd + 1);
}

bool				CStringUtility::isLengthInRange(string& strString, uint32 uiMinLength, uint32 uiMaxLength)
{
	return strString.length() >= uiMinLength && strString.length() <= uiMaxLength;
}

uint32		CStringUtility::getCRC(string& strData)
{
	return crc32buf((char*) strData.c_str(), (size_t)strData.length());
}

bool				CStringUtility::isAlphaNumericUnderscore(string& strData)
{
	for (uint32 i = 0, j = strData.length(); i < j; i++)
	{
		uint8 ucChar = strData.c_str()[i] & 0xFF;
		if ((ucChar >= 48 && ucChar <= 57) || (ucChar >= 65 && ucChar <= 90) || (ucChar >= 97 && ucChar <= 122) || ucChar == 95)
		{
		}
		else
		{
			return false;
		}
	}
	return true;
}

void				CStringUtility::setClipboardText(string& strText)
{
	uint16 iTextLength = strText.length() + 1;
	HGLOBAL glob = GlobalAlloc(GMEM_FIXED, iTextLength);
	memcpy(glob, strText.c_str(), iTextLength);

	if (OpenClipboard(NULL) != 0)
	{
		if (EmptyClipboard() != FALSE)
		{
			SetClipboardData(CF_TEXT, glob);
		}
		CloseClipboard();
	}
}

string				CStringUtility::capLength(string& strData, uint32 uiMaxLength)
{
	if (strData.length() > uiMaxLength)
	{
		return strData.substr(0, uiMaxLength);
	}
	return strData;
}

uint32		CStringUtility::getCurrentEpochTime(void)
{
	string strEpochTime = (char*)std::time(NULL);
	return CStringUtility::toNumber(strEpochTime);
}

string				CStringUtility::mergeStrings(string& strString1, string& strString2, uint32 uiCapLength)
{
	if ((strString1.length() + strString2.length()) <= uiCapLength)
	{
		return strString1 + strString2;
	}

	if (strString1.length() > strString2.length())
	{
		if (uiCapLength > strString2.length())
		{
			strString1 = strString1.substr(0, uiCapLength - strString2.length());
		}
	}
	else if (strString1.length() < strString2.length())
	{
		if (uiCapLength > strString1.length())
		{
			strString2 = strString2.substr(0, uiCapLength - strString1.length());
		}
	}

	uint32 uiLength = uiCapLength / 2;
	return strString1.substr(0, uiLength) + strString2.substr(0, uiLength + ((uiCapLength % 2) == 1 ? 1 : 0));
}

bool				CStringUtility::doesStartWith(string& strText, string& strStartWith, bool bMatchCase)
{
	if (strText.length() < strStartWith.length())
	{
		return false;
	}

	string strStartWith2 = strText.substr(0, strStartWith.length());
	if (!bMatchCase)
	{
		strStartWith = CStringUtility::toUpperCase(strStartWith);
		strStartWith2 = CStringUtility::toUpperCase(strStartWith2);
	}
	if (strStartWith2 == strStartWith)
	{
		return true;
	}
	
	return false;
}

bool				CStringUtility::isPositiveInteger(string& strText)
{
	for (uint32 i = 0, j = strText.length(); i < j; i++)
	{
		uint8 ucChar = strText.c_str()[i] & 0xFF;
		if (ucChar >= 48 && ucChar <= 57)
		{
		}
		else
		{
			return false;
		}
	}
	return true;
}

string				CStringUtility::addNumberGrouping(string& strNumber)
{
	if (strNumber.length() < 1)
	{
		return "";
	}

	int iStart = strNumber.c_str()[0] == '-' || strNumber.c_str()[0] == '+' ? 1 : 0;
	int iDotPos = strNumber.find('.');

	string strOldDigits;
	vector<string> vecNewDigits;
	if (iDotPos == string::npos)
	{
		strOldDigits = strNumber.substr(iStart);
	}
	else
	{
		strOldDigits = strNumber.substr(iStart, iDotPos - iStart);
	}

	uint32 uiDigitOffset = strOldDigits.length() % 3;
	if (uiDigitOffset != 0)
	{
		vecNewDigits.push_back(strOldDigits.substr(0, uiDigitOffset));
	}
	for (uint32 i = 0, j = floor((float32)strOldDigits.length() / 3.0f); i < j; i++)
	{
		vecNewDigits.push_back(strOldDigits.substr(uiDigitOffset + (i * 3), 3));
	}

	string strComma = ",";
	return join(vecNewDigits, strComma);
}

string				CStringUtility::fixEOLs(string& strData, string strEOLCharsToUse)
{
	string
		strEOL_Windows = "\r\n",
		strEOL_Linux = "\n",
		strEOL_OldMac = "\r";
	if (strEOLCharsToUse == "\n")
	{
		return CStringUtility::replace(CStringUtility::replace(strData, strEOL_Windows, strEOL_Linux), strEOL_OldMac, strEOL_Linux);
	}
	else if (strEOLCharsToUse == "\r")
	{
		return CStringUtility::replace(CStringUtility::replace(strData, strEOL_Windows, strEOL_OldMac), strEOL_Linux, strEOL_OldMac);
	}
	else if (strEOLCharsToUse == "\r\n")
	{
		return CStringUtility::replace(CStringUtility::replace(CStringUtility::replace(strData, strEOL_Windows, strEOL_Linux), strEOL_OldMac, strEOL_Linux), strEOL_Linux, strEOL_Windows);
	}
	return strData;
}

uint32				CStringUtility::swapEndian(uint32 uiValue)
{
	return
		(uiValue >> 24) & 0xFF |
		(((uiValue >> 16) & 0xFF) << 8) |
		(((uiValue >> 8) & 0xFF) << 16) |
		((uiValue & 0xFF) << 24)
	;
}

bool				CStringUtility::isAsciiCharacterDisplayable(uint8 uiCharCode, bool bIncludeSpace, bool bIncludeTab)
{
	return (uiCharCode > 32 && uiCharCode < 127)
		|| (bIncludeSpace && uiCharCode == 32)
		|| (bIncludeTab && uiCharCode == 9);
}

string				CStringUtility::createCharString(uint8 uiCharCode)
{
	return string(1, uiCharCode);
}