
#include "CUIGroup.h"


CUIGroup::CUIGroup( unsigned int _uId )
: IUInterface( _uId )
{
    m_uLastSelected = 0;
}

CUIGroup::~CUIGroup( void )
{
	Clear( );
}

void CUIGroup::Add( IUInterface * _pItem )
{
	m_lstItems.push_back( _pItem );

    _pItem->SetParent( this );
}

bool CUIGroup::CheckBounds( float _fX, float _fY )
{
    _fX -= GetX( );
    _fY -= GetY( );
	for ( unsigned int i = 0; i < m_lstItems.size( ); ++i )
	{
        if ( m_lstItems[ i ]->CheckBounds( _fX, _fY ) )
        {
            return true;
        }
    }

    return false;
}

void CUIGroup::DrawIntern( void )
{
	g_Render.PushMatrix( );

	// Translating coordinates to the group system
	g_Render.Translate( m_fPosX, m_fPosY, 0.0f );

	for ( unsigned int i = 0; i < m_lstItems.size( ); ++i )
	{
		m_lstItems[ i ]->Draw( );
	}

	g_Render.PopMatrix( );
}

void CUIGroup::Clear( void )
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
IUInterface * CUIGroup::GetItemById( unsigned int _uId )
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

unsigned int CUIGroup::ProcessMsgIntern( const CUIMessage & _msg )
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
        // Check the Group bounds first to optimize
        // commented because of item moving behavior error
        //if ( CheckBounds( msg.fParam1, msg.fParam2 ) )
        {
            msg.fParam1 -= m_fPosX;
            msg.fParam2 -= m_fPosY;
	        for ( int i = (int)m_lstItems.size( ) - 1; i >= 0; --i )
	        {
		        if ( m_lstItems[ i ]->CheckBounds( msg.fParam1, msg.fParam2 ) )
		        {
                    uItemProcessed = m_lstItems[ i ]->ProcessMsg( msg );
                    if ( uItemProcessed )
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

unsigned int CUIGroup::GetLastSelected( void )
{
    return m_uLastSelected;
}
