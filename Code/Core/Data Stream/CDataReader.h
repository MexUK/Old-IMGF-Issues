#ifndef CDataReader_H
#define CDataReader_H

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
#include <string>
#include <vector>
#include <fstream>

class CDataReader : public CDataStream, public CSingleton<CDataReader>
{
public:
	CDataReader(void);
	
	// reset
	void					close(void) { reset(); }
	void					reset(void);
	void					resetLines(void);		// text mode only

	// file processing
	void					openFile(std::string& strFilePath, uint32 uiFlags);
	bool					isFileOpen(void);
	void					resetFile(void);

	// read string (binary mode)
	std::string				readAll(void);
	std::string				readString(uint32 uiByteCount = 0);
	std::string				readStringUntilZero(void);
	std::string				readRemaining(void);
	void					readCString(uint8* pDestination, uint32 uiByteCount);
	uint8*					readCString(uint32 uiByteCount);

	// read int (binary mode)
	uint32					readUint32(void);
	uint16					readUint16(void);
	uint8					readUint8(void);
	int32					readInt32(void);
	int16					readInt16(void);
	int8					readInt8(void);
	CVector4ui8				readVector4ui8(void);
	std::vector<uint32>			readUint32ArrayAsStdVector(uint32 uiValueCount);
	std::vector<CVector2D>		readVector2DArrayAsStdVector(uint32 uiValueCount);
	std::vector<CVector3D>		readVector3DArrayAsStdVector(uint32 uiValueCount);
	std::vector<CVector4ui8>	readVector4ui8ArrayAsStdVector(uint32 uiValueCount);
	std::vector<CVector4ui16>	readVector4ui16ArrayAsStdVector(uint32 uiValueCount);

	// read float (binary mode)
	float32					readFloat32(void);
	CVector2D				readVector2D(void);
	CVector3D				readVector3D(void);
	CVector4D				readVector4D(void);
	
	// read token (text mode)
	std::string				readTokenString(void);
	int32					readTokenInt32(void);
	uint32					readTokenUint32(void);
	float32					readTokenFloat32(void);
	CVector2D				readTokenVector2D(void);
	CVector3D				readTokenVector3D(void);
	CVector4D				readTokenVector4D(void);

	// read struct
	template<class T>
	T*						readStruct(void)
	{
		return (T*) readCString(sizeof(T));
	};
	
	template<class T>
	T*						readStructMultiple(uint32 uiObjectCount)
	{
		T *pObjects = new T[uiObjectCount];
		uint32 uiByteCount = uiObjectCount * sizeof(T);
		memcpy(pObjects, readCString(uiByteCount), uiByteCount);
		return pObjects;
	}

	// user data
	void					setUserData(uint32 uiUserData) { m_uiUserData = uiUserData; }
	uint32					getUserData(void) { return m_uiUserData; }

	// peek
	void					setPeek(bool bPeekEnabled) { m_bPeek = bPeekEnabled; }
	bool					getPeek(void) { return m_bPeek; }

	// line processing
	std::string				readLineToken(void);
	bool					iterateLines(void);
	
	// seek
	void					setSeek(uint64 uiByteIndex);
	uint64					getSeek(void);
	
	bool					canSeekTo(uint64 uiByteIndex);
	bool					isEOF(void);

	// data length
	uint64					getDataLength(void);
	uint64					getProcessedDataLength(void); // returns getCachedDataLength() or if 0 then getDataLength().
	uint64					getRemainingDataLength(void);

	void					setCachedDataLength(uint32 uiCachedDataLength) { m_uiCachedDataLength = uiCachedDataLength; }
	uint32					getCachedDataLength(void) { return m_uiCachedDataLength; }
	
	bool					canRead(uint32 uiByteCount);

	// file mode only
	std::ifstream&			getFile(void) { return m_file; }

	// text mode only - lines
	void					readAndStoreLines(void);
	
	void					setActiveLine(std::string* pstrActiveLine) { m_pstrActiveLine = pstrActiveLine; }
	std::string*			getActiveLine(void) { return m_pstrActiveLine; }

	uint32					getLineCount(void);

	void					setActiveLineIndex(uint32 uiActiveLineIndex) { m_uiActiveLineTokenIndex = uiActiveLineIndex; }
	uint32					getActiveLineIndex(void) { return m_uiActiveLineIndex; }
	
	// text mode only - line tokens
	void					prepareLineTokens(void);
	
	void					setTokenModeEnabled(bool bTokenModeEnabledState) { m_bTokenModeEnabledState = bTokenModeEnabledState; }
	bool					isTokenModeEnabled(void) { return m_bTokenModeEnabledState; }
	
	void					setActiveLineTokenIndex(uint32 uiActiveLineTokenIndex) { m_uiActiveLineTokenIndex = uiActiveLineTokenIndex; }
	uint32					getActiveLineTokenIndex(void) { return m_uiActiveLineTokenIndex; }

	void					setLineTokenCount(uint32 uiLineTokenCount) { m_uiLineTokenCount = uiLineTokenCount; }
	uint32					getLineTokenCount(void) { return m_uiLineTokenCount; }

private:
	// data stream type: memory
	void						restoreSeekForPeek(uint32 uiByteCount);

private:
	std::ifstream				m_file;
	uint32						m_uiUserData;
	uint32						m_uiCachedDataLength;
	std::string*				m_pstrActiveLine;	// only used for byte interpretation type: text
	std::vector<std::string>	m_vecLines;			// only used for byte interpretation type: text
	uint32						m_uiActiveLineIndex;// only used for byte interpretation type: text
	uint32						m_uiMaxLineIndex;	// only used for byte interpretation type: text
	uint8						m_bTokenModeEnabledState	: 1;	// only used for byte interpretation type: text
	uint8						m_bPeek						: 1;
	uint32						m_uiLineTokenCount;	// only used for byte interpretation type: text
	uint32						m_uiActiveLineTokenIndex;	// only used for byte interpretation type: text
};

#endif