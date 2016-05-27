#ifndef CTextureEntry_H
#define CTextureEntry_H

#include <string>

class CRWSection_Texture;

class CTextureEntry
{
public:
	CTextureEntry(void) :
		m_bHasDiffuse(false),
		m_bHasAlpha(false),
		m_pRWTextureSection(nullptr)
	{};

	void				setHasDiffuse(bool bHasDiffuse) { m_bHasDiffuse = bHasDiffuse; }
	bool				doesHaveDiffuse(void) { return m_bHasDiffuse; }

	void				setDiffuseName(std::string strDiffuseName) { m_strDiffuseName = strDiffuseName; }
	std::string			getDiffuseName(void) { return m_strDiffuseName; }

	void				setDiffusePath(std::string strDiffusePath) { m_strDiffusePath = strDiffusePath; }
	std::string			getDiffusePath(void) { return m_strDiffusePath; }

	void				setHasAlpha(bool bHasAlpha) { m_bHasAlpha = bHasAlpha; }
	bool				doesHaveAlpha(void) { return m_bHasAlpha; }

	void				setAlphaName(std::string strAlphaName) { m_strAlphaName = strAlphaName; }
	std::string			getAlphaName(void) { return m_strAlphaName; }

	void				setAlphaPath(std::string strAlphaPath) { m_strAlphaPath = strAlphaPath; }
	std::string			getAlphaPath(void) { return m_strAlphaPath; }

	void				setRWTextureSection(CRWSection_Texture *pRWSection_Texture) { m_pRWTextureSection = pRWSection_Texture; }
	CRWSection_Texture*	getRWTextureSection(void) { return m_pRWTextureSection; }

private:
	bool				m_bHasDiffuse;
	std::string			m_strDiffuseName;
	std::string			m_strDiffusePath;
	bool				m_bHasAlpha;
	std::string			m_strAlphaName;
	std::string			m_strAlphaPath;
	CRWSection_Texture*	m_pRWTextureSection;
};

#endif