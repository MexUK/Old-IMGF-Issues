#include "CVectorUtility.h"
#include "../String/CStringUtility.h"
#include <algorithm>

using namespace std;

vector<string>		CVectorUtility::combineVectors(vector<string>& vecVector1, vector<string>& vecVector2)
{
	vector<string> vecValues;
	for (auto strValue : vecVector1)
	{
		vecValues.push_back(strValue);
	}
	for (auto strValue : vecVector2)
	{
		vecValues.push_back(strValue);
	}
	return vecValues;
}

void				CVectorUtility::addToVector(vector<string>& vecVector1, vector<string>& vecVector2)
{
	uint32 uiKey = vecVector1.size();
	vecVector1.resize(uiKey + vecVector2.size());
	for (string& strValue : vecVector2)
	{
		vecVector1[uiKey] = strValue;
		uiKey++;
	}
}

deque<string>		CVectorUtility::convertVectorToDeque(vector<string>& vecVector)
{
	deque<string> deqValues;
	for (auto strValue : vecVector)
	{
		deqValues.push_back(strValue);
	}
	return deqValues;
}

unordered_map<string, bool>		CVectorUtility::convertVectorToUnorderedMap(vector<string>& vecVector)
{
	unordered_map<string, bool> umapValues;
	for (string& strValue : vecVector)
	{
		umapValues[strValue] = true;
	}
	return umapValues;
}

vector<string>		CVectorUtility::toUpperCase(vector<string> vecVector)
{
	vector<string> vecValues;
	for (auto strValue : vecVector)
	{
		vecValues.push_back(CStringUtility::toUpperCase(strValue));
	}
	return vecValues;
}

vector<string>		CVectorUtility::removeDuplicates(vector<string>& vecVector)
{
	vector<string> vecNewVector;
	for (auto strValue : vecVector)
	{
		if (std::find(vecNewVector.begin(), vecNewVector.end(), strValue) == vecNewVector.end())
		{
			vecNewVector.push_back(strValue);
		}
	}
	return vecNewVector;
}

uint32		CVectorUtility::findKey(vector<string>& vecVector, string& strValue)
{
	for (uint32 i = 0; i < vecVector.size(); i++)
	{
		if (vecVector[i] == strValue)
		{
			return i;
		}
	}
	return -1;
}

void				CVectorUtility::addUniqueEntry(vector<string>& vecVector, string& strValue)
{
	auto it = std::find(vecVector.begin(), vecVector.end(), strValue);
	if (it == vecVector.end())
	{
		vecVector.push_back(strValue);
	}
}

vector<string>		CVectorUtility::getUniqueEntries(vector<string>& vecVector1, vector<string>& vecVector2)
{
	vector<string>
		vecVector3 = toUpperCase(vecVector1),
		vecVector4 = toUpperCase(vecVector2),
		vecUniqueEntries;

	for (uint32 i = 0, j = vecVector3.size(); i < j; i++)
	{
		string strEntryUpper = vecVector3[i];

		if (std::find(vecVector4.begin(), vecVector4.end(), strEntryUpper) == vecVector4.end())
		{
			string strEntry = vecVector1[i];
			vecUniqueEntries.push_back(strEntry);
		}
	}
	return vecUniqueEntries;
}

bool				CVectorUtility::isIn(vector<string>& vecVector, string& strLookFor)
{
	return std::find(vecVector.begin(), vecVector.end(), strLookFor) != vecVector.end();
}

bool				sortStdVectorAzCaseInsensitive(std::string& strString1, std::string& strString2)
{
	return strcmp(strString1.c_str(), strString2.c_str()) < 0;
}

void				CVectorUtility::sortAZCaseInsensitive(vector<string>& vecVector)
{
	std::sort(vecVector.begin(), vecVector.end(), sortStdVectorAzCaseInsensitive);
}