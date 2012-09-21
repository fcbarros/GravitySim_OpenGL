
#include "CUIWindow.h"


CUIWindow::CUIWindow( unsigned int _uId )
: IUInterface( _uId )
{
    m_uLastSelected = 0;
}

CUIWindow::~CUIWindow( void )
{
	Clear( );
}

void CUIWindow::Add( IUInterface * _pItem )
{
	m_lstItems.push_back( _pItem );

    _pItem->SetParent( this );
}

bool CUIWindow::CheckBounds( float /* _fX */, float /* _fY */ )
{
    return true;
}

void CUIWindow::DrawIntern( void )
{
	for ( unsigned int i = 0; i < m_lstItems.size( ); ++i )
	{
		m_lstItems[ i ]->Draw( );
	}
}

void CUIWindow::Clear( void )
{
	for ( unsigned int i = 0; i < m_lstItems.size( ); ++i )
	{
		if ( m_lstItems[ i ] != NULL )
		{
			delete m_lstItems[ i ];
			m_lstItems[ i ] = NULL;
		}
	}
	m_lstItems.clear( );

    m_uLastSelected = 0;
}

// Modified main GetItemById
// Loops through all items in the group calling GetItemById so it
// can process inner groups, if it did not find the id inside the
// items, try the group itself
IUInterface * CUIWindow::GetItemById( unsigned int _uId )
{
	for ( unsigned int i = 0; i < m_lstItems.size( ); ++i )
	{
        IUInterface * pTemp = m_lstItems[ i ]->GetItemById( _uId );
        if ( pTemp )
		{
            return pTemp;
		}
	}

    return IUInterface::GetItemById( _uId );
}

unsigned int CUIWindow::ProcessMsgIntern( const CUIMessage & _msg )
{
    CUIMessage msg = _msg;
    unsigned int uItemProcessed = 0;

    // Translates the coordinate system and loop the items
    switch ( msg.Type )
    {
    case CUIMessage::MOUSE_DOWN:
    case CUIMessage::MOUSE_UP:
    case CUIMessage::MOUSE_MOVE:
    case CUIMessage::MOUSE_DBLCLICK:
        {
            msg.fParam1 -= m_fPosX;
            msg.fParam2 -= m_fPosY;
	        for ( int i = (int)m_lstItems.size( ) - 1; i >= 0; --i )
	        {
		        if ( m_lstItems[ i ]->CheckBounds( msg.fParam1, msg.fParam2 ) )
		        {
                    uItemProcessed = m_lstItems[ i ]->ProcessMsg( msg );
                    if ( uItemProcessed > 0 )
                    {
                        break;
                    }
		        }
	        }
        }
        break;
    }

    // Comparing the last selected item with the current so we can send a leave message
    if ( uItemProcessed != m_uLastSelected )
    {
        if ( m_uLastSelected > 0 )
        {
            CUIMessage msgTemp;
            msgTemp.Type = CUIMessage::LEAVE;
            GetItemById( m_uLastSelected )->ProcessMsg( msgTemp );
        }
        m_uLastSelected = uItemProcessed;
    }

    return uItemProcessed;
}

unsigned int CUIWindow::GetLastSelected( void )
{
    return m_uLastSelected;
}

// returns the ID o the item processed
unsigned int CUIWindow::ProcessWin32Msg( HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam )
{
    CUIMessage Msg;

    switch ( _message )
    {
    case WM_LBUTTONDOWN:
        Msg.Type = CUIMessage::MOUSE_DOWN;
        Msg.uParam1 = 1;
        Msg.fParam1 = LOWORD( _lParam ); // x
        Msg.fParam2 = HIWORD( _lParam ); // y
        break;

    case WM_RBUTTONDOWN:
        Msg.Type = CUIMessage::MOUSE_DOWN;
        Msg.uParam1 = 2;
        Msg.fParam1 = LOWORD( _lParam ); // x
        Msg.fParam2 = HIWORD( _lParam ); // y
        break;

    case WM_LBUTTONUP:
        Msg.Type = CUIMessage::MOUSE_UP;
        Msg.uParam1 = 1;
        Msg.fParam1 = LOWORD( _lParam ); // x
        Msg.fParam2 = HIWORD( _lParam ); // y
        break;

    case WM_RBUTTONUP:
        Msg.Type = CUIMessage::MOUSE_UP;
        Msg.uParam1 = 2;
        Msg.fParam1 = LOWORD( _lParam ); // x
        Msg.fParam2 = HIWORD( _lParam ); // y
        break;

    case WM_MOUSEMOVE:
        Msg.Type = CUIMessage::MOUSE_MOVE;
        Msg.fParam1 = LOWORD( _lParam ); // x
        Msg.fParam2 = HIWORD( _lParam ); // y
        break;

    case WM_LBUTTONDBLCLK:
        Msg.Type = CUIMessage::MOUSE_DBLCLICK;
        Msg.fParam1 = LOWORD( _lParam ); // x
        Msg.fParam2 = HIWORD( _lParam ); // y
        break;

    case WM_SIZE:
        Msg.Type = CUIMessage::SIZE;
        Msg.fParam1 = LOWORD( _lParam ); // width
        Msg.fParam2 = HIWORD( _lParam ); // height
        break;

    default:
        return 0;
        break;
    }

    return ProcessMsg( Msg );
}

