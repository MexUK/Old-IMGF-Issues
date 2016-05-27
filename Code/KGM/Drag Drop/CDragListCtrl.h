#ifndef CDragListCtrl_H
#define CDragListCtrl_H

#include <afxcmn.h>
#include <afxole.h>
#include "CDropSource.h"
//#include <Windows.h>
#include <string>

struct CDraggableFile
{
	CDraggableFile(void) :
		m_bLogged(false) {};

	std::string						m_strFileName;
	std::string						m_strFileData;
	bool							m_bLogged;
};

struct CDraggableFile;

class CDragListCtrl : public CListCtrl
{
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBeginDrag(void);
	DECLARE_MESSAGE_MAP()

private:
	CDropSource						m_dropSource;
	std::vector<CDraggableFile*>	m_vecDraggableFiles;
};

#endif