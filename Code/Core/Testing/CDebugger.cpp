#include "CDebugger.h"
#include "File/CFileUtility.h"

using namespace std;

void		CDebugger::log(string strData)
{
	//std::string strPath = "C:\\DEBUG FILE.txt";
	//std::string strPath = "C:\\Users\\Игорь\\Desktop\\DEBUG FILE.txt";
	//std::string strPath = "C:\\Users\\SOL\\Desktop\\DEBUG FILE.txt";

	std::string strPath = "C:\\Users\\James\\Desktop\\DEBUG FILE.txt";
	//std::string strPath = "C:\\Users\\Keith\\Desktop\\DEBUG FILE.txt";

	CFileUtility::storeFile(strPath, strData + "\r\n", true, true);
}