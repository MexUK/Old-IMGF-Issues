#ifndef CThemeDesigner_H
#define CThemeDesigner_H

class CWindow;

class CThemeDesigner
{
public:
	CThemeDesigner(void);

	void					init(void);

	void					initWindow(void);
	void					initWindowDesign(void);

	void					setWindow(CWindow* pWindow) { m_pWindow = pWindow; }
	CWindow*				getWindow(void) { return m_pWindow; }

private:
	void					initTab_AddItem(void);

private:
	CWindow*				m_pWindow;
};

#endif