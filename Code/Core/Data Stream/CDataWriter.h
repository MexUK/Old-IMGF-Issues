#ifndef CDataWriter_H
#define CDataWriter_H

#include "CDataStream.h"
#include "CSingleton.h"
#include "Types.h"
#include "eDataStreamType.h"
#include "eEndian.h"
#include "CVector2D.h"
#include "CVector3D.h"
#include "CVector4D.h"
#include "CVector4ui8.h"
#include "CVector4ui16.h"
#include "Pool/CVectorPool.h"
#include <string>
#include <vector>
#include <fstream>

class CDataWriter;

class CDataWriter : public CDataStream // todo - move to singleton like before and how CDataReader is, and remove static-instance methods and update it to how the CDataReader works by using something like getIndexedInstance().
{
public:
	CDataWriter(void);

	// static
	static CDataWriter*		getInstance(void);
	static CDataWriter*		addInstance(void);
	static void				removeLatestInstance(void);
	static void				moveLatestInstanceDown(void);

	// reset
	void					close(bool bHasError = false);
	void					reset(bool bHasError = false);
	void					resetData(void);

	// file processing
	void					openFile(std::string& strFilePath, uint32 uiFlags);
	bool					isFileOpen(void);
	void					resetFile(void);

	// write string
	void					write(std::string& strData);
	void					write(std::string& strData, uint32 uiByteCount);
	void					write(char *pData, uint32 uiByteCount);

	// write int
	void					write(uint8 uiInt);
	void					write(uint16 uiInt);
	void					write(uint32 uiInt);
	void					write(uint64 uiInt);
	void					write(int8 iInt);
	void					write(int16 iInt);
	void					write(int32 iInt);
	void					write(int64 iInt);
	void					write(CVector4ui8 vecUint);
	void					write(std::vector<uint32>& vecUints);
	void					write(std::vector<CVector2D>& vecVectors);
	void					write(std::vector<CVector3D>& vecVectors);
	void					write(std::vector<CVector4ui8>& vecVectors);
	void					write(std::vector<CVector4ui16>& vecVectors);

	// write float
	void					write(float32 fFloat);
	void					write(float64 fFloat);
	void					write(CVector2D& vecVector);
	void					write(CVector3D& vecVector);
	void					write(CVector4D& vecVector);
	
	// write token
	void					writeToken(std::string& strString);
	void					writeToken(int32 iInt);
	void					writeToken(uint32 uiInt);
	void					writeToken(float32 fFloat);
	void					writeToken(CVector2D& vecVector);
	void					writeToken(CVector3D& vecVector);
	void					writeToken(CVector4D& vecVector);

	template <class DerivedFormatClass>
	void					writeLineEntries(CVectorPool<DerivedFormatClass> *pPool);
	template <class SectionClass, class EntryClass>
	void					writeSectionLineEntries(CVectorPool<SectionClass> *pPool);

	// write raw struct
	template <class T>
	void					writeStruct(T& object)
	{
		write(&object, sizeof(T));
	}
	
	// seek
	void					setSeek(uint64 uiByteIndex);
	uint64					getSeek(void);

	// file mode only
	std::ofstream&			getFile(void) { return m_file; }

	void					setTemporaryFilePath(std::string& strTemporaryFilePath) { m_strTemporaryFilePath = strTemporaryFilePath; }
	std::string&			getTemporaryFilePath(void) { return m_strTemporaryFilePath; }

	std::string				generateTemporaryFilePath(std::string& strFilePath);

private:
	static std::vector<CDataWriter*>	m_vecDataWriters;
	std::ofstream			m_file;
	std::string				m_strTemporaryFilePath;
};

template<class DerivedFormatClass>
void						CDataWriter::writeLineEntries(CVectorPool<DerivedFormatClass> *pPool)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	for (DerivedFormatClass *pEntry : pPool->getEntries())
	{
		pEntry->serialize();
		pDataWriter->write("\n", 1);
	}
}

template <class SectionClass, class EntryClass>
void					CDataWriter::writeSectionLineEntries(CVectorPool<SectionClass> *pPool)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	for (SectionClass *pSection : pPool->getEntries())
	{
		pSection->serialize();
		pDataWriter->write("\n");

		for (EntryClass *pEntry : pSection->getEntries())
		{
			pEntry->serialize();
			pDataWriter->write("\n");
		}

		pDataWriter->write("end\n");
	}
}

#endif