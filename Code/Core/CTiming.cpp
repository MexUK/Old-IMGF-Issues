#include "CTiming.h"
#include "String/CStringUtility.h"
#include <chrono>

using namespace std;
using namespace std::chrono;

void							CTiming::start(string strTimingName)
{
	m_uiTimeStart = pollTime();
	m_strTimingName = strTimingName;
}

void							CTiming::stop(void)
{
	uint32 uiTimeEnd = pollTime();
	uint32 uiTimeLength = uiTimeEnd - m_uiTimeStart;
	
	m_umapTimings[m_strTimingName].push_back(uiTimeLength);

	m_uiTimeStart = 0;
	m_strTimingName = "";
}

uint32							CTiming::pollTime(void)
{
	return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

string							CTiming::joinTimings(vector<uint32>& vecTimings)
{
	string strTimings = "";
	for (uint32 i = 0, j = vecTimings.size(); i < j; i++)
	{
		strTimings += CStringUtility::toString(vecTimings[i]);
		if (i != (j - 1))
		{
			strTimings += " ";
		}
	}
	return strTimings;
}