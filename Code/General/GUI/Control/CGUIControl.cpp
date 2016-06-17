#include "CGUIControl.h"
#include "GUI/Window/CWindow.h"
#include "GUI/Styles/CGUIStyles.h"
#include "CGUIScrollPool.h"
#include "Math/CMathUtility.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

CGUIControl::CGUIControl(eGUIControl eControlType) :
	m_eControlType(eControlType),
	m_uiControlId(0),
	m_bPointMarkedAsInControl(false),
	m_pScrolls(nullptr)
{
	m_vecPosition.m_x = 0;
	m_vecPosition.m_y = 0;
	m_vecSize.m_x = 0;
	m_vecSize.m_y = 0;
	m_pScrolls = new CGUIScrollPool;
}

CGUIControl::~CGUIControl(void)
{
	delete m_pScrolls;
}

// serialization
void									CGUIControl::unserialize(bool bSkipControlId)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	if (!bSkipControlId)
	{
		setControlType((eGUIControl)pDataReader->readUint32()); // control type
	}
	setPosition(pDataReader->readPoint2D()); // control position
	setSize(pDataReader->readSize2D()); // control size
}

void									CGUIControl::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	pDataWriter->writeUint32(getControlType()); // control type
	pDataWriter->writePoint2D(getPosition()); // control position
	pDataWriter->writeSize2D(getSize()); // control size
}

// point testing
bool									CGUIControl::isPointInItem(CPoint2D& vecPoint)
{
	return CMathUtility::isPointInRectangle(vecPoint, getPosition(), getSize());
}

// focus
bool									CGUIControl::doesControlHaveFocus(void)
{
	return getWindow()->getFocusedControl() == this;
}

// scroll
void									CGUIControl::addScroll(CScrollControl *pScroll)
{
	getScrolls()->addEntry(pScroll);
}