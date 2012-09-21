
#include "CUICheckBox.h"


CUICheckBox::CUICheckBox( unsigned int _uId,
                          string _stCaption,
				          float _fPosX, float _fPosY,
                          IUICallBack * _pInterfaceCallBack )
: IUInterface( _uId, _fPosX, _fPosY, 0.0f, 0.0f, _pInterfaceCallBack ),
  m_bChecked( false ),
  m_fHExtraSpace( 5.0f ), m_fVExtraSpace( 0.0f )
{
    // Text Color
    SetForegroundColor( 1.0f, 1.0f, 1.0f, 1.0f );

    try 
    {
		m_glFont.Create( "Images/Arial10" );
	}
	catch ( GLFontError::InvalidFile )
    {
		abort( );
	}

    SetCaption( _stCaption );
}

CUICheckBox::~CUICheckBox( void )
{
}

void CUICheckBox::SetCaption( string _stCaption )
{
    m_stCaption = _stCaption;

    float w, h;
    m_glFont.GetTextSize( _stCaption, w, h );
    m_fHeight = h + m_fVExtraSpace;
    m_fWidth = w + m_fHeight + m_fHExtraSpace;
}

string CUICheckBox::GetCaption( void )
{
    return m_stCaption;
}

void CUICheckBox::SetChecked( bool _bChecked )
{
    m_bChecked = _bChecked;
}

bool CUICheckBox::IsChecked( void )
{
    return m_bChecked;
}

void CUICheckBox::DrawIntern( void )
{
    g_Render.SetColor( m_ColorForeground.r, m_ColorForeground.g, m_ColorForeground.b, m_ColorForeground.a );

    g_Render.DrawRectangle2D( m_fPosX, m_fPosY, m_fHeight, m_fHeight, false );

    if ( IsChecked( ) )
    {
        glBegin( GL_LINES );

            glVertex2f( m_fPosX, m_fPosY );
            glVertex2f( m_fPosX + m_fHeight, m_fPosY + m_fHeight );

            glVertex2f( m_fPosX, m_fPosY + m_fHeight );
            glVertex2f( m_fPosX + m_fHeight, m_fPosY );

        glEnd( );
    }

    m_glFont.TextOut( m_stCaption, 
                      m_fPosX + m_fHeight + m_fHExtraSpace, 
                      m_fPosY + (m_fVExtraSpace / 2.0f), 
                      0.0f, 
                      m_fPosX + m_fWidth - (m_fHExtraSpace / 2.0f) );
}

unsigned int CUICheckBox::ProcessMsgIntern( const CUIMessage & _msg )
{
    static float fPosXOld = 0.0f;
    static float fPosYOld = 0.0f;
    bool bProcessed = false;

    switch ( _msg.Type )
    {
    case CUIMessage::MOUSE_DOWN:
        bProcessed = true;
        break;

    case CUIMessage::MOUSE_UP:
        if ( _msg.uParam1 == 1 )
        {
            SetChecked( ! IsChecked( ) );
        }
        bProcessed = true;
        break;

    case CUIMessage::MOUSE_MOVE:
        break;

    case CUIMessage::MOUSE_DBLCLICK:
    case CUIMessage::LEAVE:
        break;

    default:
        return 0;
        break;
    }

    return bProcessed ? m_uId : 0;
}
