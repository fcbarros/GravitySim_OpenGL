
#include "CUISimpleTextBox.h"


CUISimpleTextBox::CUISimpleTextBox( unsigned int _uId,
							          float _fPosX, float _fPosY,
							          float _fWidth, float _fHeight )
: IUInterface( _uId, _fPosX, _fPosY, _fWidth, _fHeight ),
  m_bMoving( false ),
  m_fHExtraSpace( 20.0f ), m_fVExtraSpace( 10.0f )
{
    // Text Color
    SetForegroundColor( 1.0f, 1.0f, 1.0f, 1.0f );
    // Background
    SetBackgroundColor( 0.2f, 0.7f, 0.2f, 0.4f );
    // Border
    SetBorderColor( 1.0f, 1.0f, 1.0f, 1.0f );

    if ( m_fWidth == -1 )
    {
        m_bVariableSize = true;
    }
    else
    {
        m_bVariableSize = false;
    }

    Clear( );

    try 
    {
		m_glFont.Create( "Images/Arial10" );
	}
	catch ( GLFontError::InvalidFile )
    {
		abort( );
	}
}

CUISimpleTextBox::~CUISimpleTextBox( void )
{
    Clear( );
}

void CUISimpleTextBox::DrawIntern( void )
{
    DrawBackGround( );
    DrawBorders( );

    g_Render.SetColor( m_ColorForeground.r, m_ColorForeground.g, m_ColorForeground.b, m_ColorForeground.a );

    float h = 0.0;
    for ( unsigned int i = 0; i < m_TextLines.size( ); i++ )
    {
        h += m_glFont.TextOut( m_TextLines[ i ], 
                               m_fPosX + 10.0f, m_fPosY + 4.0f + h, 0.0f, 
                               m_fPosX + m_fWidth - (m_fHExtraSpace / 2.0f) );
    }
}

void CUISimpleTextBox::AddLines( string _stLine )
{
    m_TextLines.push_back( _stLine );

    if ( m_bVariableSize )
    {
        float w, h;
        m_glFont.GetTextSize( _stLine, w, h );
        if ( ( w + m_fHExtraSpace ) > m_fWidth )
        {
            m_fWidth = w + m_fHExtraSpace;
        }
        m_fHeight += h;
    }
}

void CUISimpleTextBox::Clear( void )
{
    m_TextLines.clear( );

    if ( m_bVariableSize )
    {
        // To include a space before and after the drawing
        m_fWidth = m_fHExtraSpace;
        m_fHeight = m_fVExtraSpace;
    }
}

unsigned int CUISimpleTextBox::ProcessMsgIntern( const CUIMessage & _msg )
{
    static float fPosXOld = 0.0f;
    static float fPosYOld = 0.0f;
    bool bProcessed = false;

    switch ( _msg.Type )
    {
    case CUIMessage::MOUSE_DOWN:
        if ( _msg.uParam1 == 1 )
        {
            fPosXOld = _msg.fParam1;
            fPosYOld = _msg.fParam2;

            m_bMoving = true;
            bProcessed = true;
        }
        break;

    case CUIMessage::MOUSE_UP:
        if ( _msg.uParam1 == 1 )
        {
            m_bMoving = false;
            bProcessed = true;
        }
        break;

    case CUIMessage::MOUSE_MOVE:
        if ( m_bMoving )
        {
            float fPosXDiff = fPosXOld - _msg.fParam1;
            float fPosYDiff = fPosYOld - _msg.fParam2;
            Move( m_fPosX - fPosXDiff, m_fPosY - fPosYDiff );
            fPosXOld = _msg.fParam1;
            fPosYOld = _msg.fParam2;
            bProcessed = true;
        }
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
