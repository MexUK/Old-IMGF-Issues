#ifndef CHTTPManager_H
#define CHTTPManager_H

#include "Types.h"
#include "CSingleton.h"
#include "CManager.h"
#include "HappyHTTP.h"
#include <string>

void		OnBegin(const happyhttp::Response* r, void* userdata);
void		OnData(const happyhttp::Response* r, void* userdata, const uint8* data, int n);
void		OnComplete(const happyhttp::Response* r, void* userdata);

class CHTTPManager : public CManager, public CSingleton<CHTTPManager>
{
public:
	void						init(void);
	void						uninit(void);

	std::string					getFileContent(std::string strURL);
	
	bool						m_bConnectionComplete;
	std::string					m_strReceivedData;
};

#endif