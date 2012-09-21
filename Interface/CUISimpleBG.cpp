
#include "CUISimpleBG.h"


CUISimpleBG::CUISimpleBG( unsigned int _uId,
                          float _fPosX, float _fPosY,
                          float _fWidth, float _fHeight )
: IUInterface( _uId, _fPosX, _fPosY, _fWidth, _fHeight ),
  m_bMoving( false )
{
    // Text Color
    SetForegroundColor( 1.0f, 1.0f, 1.0f, 1.0f );
    // Background
    SetBackgroundColor( 0.2f, 0.7f, 0.2f, 0.4f );
    // Border
    SetBorderColor( 1.0f, 1.0f, 1.0f, 1.0f );
}

CUISimpleBG::~CUISimpleBG( void )
{
}

void CUISimpleBG::DrawIntern( void )
{
    DrawBackGround( );
    DrawBorders( );
}

unsigned int CUISimpleBG::ProcessMsgIntern( const CUIMessage & _msg )
{
    static float fPosXOld = 0.0f;
    static float fPosYOld = 0.0f;
    bool bProcessed = false;

    switch ( _msg.Type )
    {
    case CUIMessage::MOUSE_DOWN:
    case CUIMessage::MOUSE_UP:
    case CUIMessage::MOUSE_MOVE:
    case CUIMessage::MOUSE_DBLCLICK:
    case CUIMessage::LEAVE:
        bProcessed = true;
        break;

    default:
        return 0;
        break;
    }

    return bProcessed ? m_uId : 0;
}
