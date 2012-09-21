
#include "CUIImagePushButton.h"


CUIImagePushButton::CUIImagePushButton( unsigned int _uId,
							            float _fPosX, float _fPosY,
							            float _fWidth, float _fHeight,
                                        IUICallBack * _pInterfaceCallBack,
	    						        string stTexNormal,
		    					        string stTexPressed )
: IUInterface( _uId, _fPosX, _fPosY, 0, 0, _pInterfaceCallBack )
{
    m_enState = NORMAL;

    m_TexNormal.LoadFromFile( stTexNormal );
	if ( _fWidth == -1.0f )
	{
		m_fWidth = (float)m_TexNormal.GetWidth( );
        m_fHeight = (float)m_TexNormal.GetHeight( );
	}
	else
	{
		m_fWidth = _fWidth;
		m_fHeight = _fHeight;
	}

    if ( ! stTexPressed.empty( ) )
	{
		m_TexPressed.LoadFromFile( stTexPressed );
	}
}

CUIImagePushButton::~CUIImagePushButton( void )
{
}

void CUIImagePushButton::DrawIntern( void )
{
    if ( ( m_enState == PRESSED ) && m_TexPressed.IsLoaded( ) )
	{
        g_Render.TextureEnable( m_TexPressed );
	}
    else
	{
        g_Render.TextureEnable( m_TexNormal );
	}

    g_Render.Blend( true );

    g_Render.SetColor( 1.0f, 1.0f, 1.0f, 1.0f );

    g_Render.DrawRectangle2D( m_fPosX, m_fPosY, m_fWidth, m_fHeight, true );

    g_Render.TextureDisable( );

    g_Render.Blend( false );
}

void CUIImagePushButton::SetState( enButtonStates _enState )
{
    m_enState = _enState;
}

CUIImagePushButton::enButtonStates CUIImagePushButton::GetState( void )
{
    return m_enState;
}

unsigned int CUIImagePushButton::ProcessMsgIntern( const CUIMessage & _msg )
{
    switch ( _msg.Type )
    {
    case CUIMessage::MOUSE_DOWN:
        if ( _msg.uParam1 == 1 )
        {
            if ( GetState( ) == PRESSED )
            {
                SetState( NORMAL );
            }
            else
            {
                SetState( PRESSED );
            }
        }
        break;

    case CUIMessage::MOUSE_UP:
        //if ( ( _msg.uParam1 == 1 ) && ( GetState( ) == PRESSED ) )
        //{
        //    SetState( OVER );
        //}
        break;

    case CUIMessage::MOUSE_MOVE:
        //if ( GetState( ) != PRESSED )
        //{
        //    SetState( OVER );
        //}
        break;

    case CUIMessage::MOUSE_DBLCLICK:
        break;

    case CUIMessage::LEAVE:
        //SetState( NORMAL );
        break;

    default:
        return 0;
        break;
    }

    return m_uId;
}
