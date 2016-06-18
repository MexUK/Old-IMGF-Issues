#include "CImageControl.h"
#include "GUI/CGUIManager.h"
#include "GUI/GraphicsLibrary/CGraphicsLibrary.h"
#include "GUI/Styles/CGUIStyles.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

using namespace std;

CImageControl::CImageControl(void) :
	CGUIControl(GUI_CONTROL_IMAGE),
	m_pImage(nullptr)
{
}

// event binding
void					CImageControl::bindEvents(void)
{
}

// serialization
void					CImageControl::unserialize(bool bSkipControlId)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	CGUIControl::unserialize(bSkipControlId);
	setSize(pDataReader->readSize2D()); // image size
	setImageData(pDataReader->readStringWithLength()); // image data
}

void					CImageControl::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	CGUIControl::serialize();
	pDataWriter->writeSize2D(getSize()); // image size
	pDataWriter->writeStringWithLengthRef(getImageData()); // image data
}

// render
void					CImageControl::render(void)
{
	CGraphicsLibrary *pGFX = CGUIManager::getInstance()->getGraphicsLibrary();

	pGFX->drawImage(getPosition(), getImage(), getSize());

	getStyles()->restoreTemporaryStyleData();
}