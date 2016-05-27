#ifndef TShapes_H
#define TShapes_H

#include "CVector3D.h"

struct TBounds
{
	float32			m_fRadius;
	CVector3D		m_vecCenter;
	CVector3D		m_vecMin;
	CVector3D		m_vecMax;
};

struct TSurface
{
	uint8			m_ucMaterial;
	uint8			m_ucFlag;
	uint8			m_ucBrightness;
	uint8			m_ucLight;
};

struct TSphere
{
	float32			m_fRadius;
	CVector3D		m_vecCenter;
	TSurface		m_surface;
};

struct TBox
{
	CVector3D		m_min;
	CVector3D		m_max;
	TSurface		m_surface;
};

struct TFaceGroup
{
	CVector3D		m_min;
	CVector3D		m_max;
	uint16			m_startFace;
	uint16			m_endFace;
};

struct TVertex : public CVector3D
{
};

struct TFace
{
	uint32			m_uiA;
	uint32			m_uiB;
	uint32			m_uiC;
	TSurface		m_surface;
};

#endif