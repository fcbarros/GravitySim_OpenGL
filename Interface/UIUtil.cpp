
#include "UIUtil.h"


// Default constructor for the Interface Message Class
// Initializes the parameters for unknown message
CUIMessage::CUIMessage( void )
: Type( UNKNOWN ), uParam1( 0 ), uParam2( 0 ),
  fParam1( 0.0f ), fParam2( 0.0f )
{
}

CUIColor::CUIColor( void )
: r( 0.0f ), g( 0.0f ), b( 0.0f ), a( 1.0f )
{
}

CUIColor::CUIColor( float _fR, float _fG, float _fB, float _fA )
: r( _fR ), g( _fG ), b( _fB ), a( _fA )
{
}

void CUICornersColors::Set( tCorner _Corner, const CUIColor & _color )
{
    switch ( _Corner )
    {
    case CORNER_ALL:
        ul = ur = ll = lr = _color;
        break;
    case CORNER_UL:
        ul = _color;
        break;
    case CORNER_UR:
        ur = _color;
        break;
    case CORNER_LL:
        ll = _color;
        break;
    case CORNER_LR:
        lr = _color;
        break;
    default:
        break;
    }
}
