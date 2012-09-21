
#ifndef _CInterfaceWindow_h_
#define _CInterfaceWindow_h_

#include "IUInterface.h"


class CUIWindow 
	: public IUInterface
{
public:

	CUIWindow( unsigned int _uId );

	virtual ~CUIWindow( void );

	virtual void Add( IUInterface * pItem );

	virtual void Clear( void );

    virtual bool CheckBounds( float _fX, float _fY );

    virtual IUInterface * GetItemById( unsigned int _uId );

    virtual unsigned int GetLastSelected( void );

    virtual unsigned int ProcessWin32Msg( HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam );

protected:

    virtual unsigned int ProcessMsgIntern( const CUIMessage & _msg );

	virtual void DrawIntern( void );

    unsigned int            m_uLastSelected;

	vector<IUInterface*>    m_lstItems;
};

#endif
