
#include "IUInterface.h"


IUInterface::IUInterface( unsigned int _uId, 
                          float _fX, float _fY,
                          float _fWidth, float _fHeight,
                          IUICallBack * _pInterfaceCallBack )
: m_pInterfaceParent( NULL ), 
  m_uId( _uId ), 
  m_bVisible( true ), 
  m_bEnabled( true ),
  m_CallBackInterface( _pInterfaceCallBack )
{
    Move( _fX, _fY );

    Resize( _fWidth, _fHeight );
}

IUInterface::IUInterface( const IUInterface & _interface )
: m_pInterfaceParent( _interface.m_pInterfaceParent ), 
  m_uId( _interface.m_uId ), 
  m_bVisible( _interface.m_bVisible ), 
  m_bEnabled( _interface.m_bEnabled ),
  m_CallBackInterface( _interface.m_CallBackInterface )
{
    Move( _interface.m_fPosX, _interface.m_fPosY );

    Resize( _interface.m_fWidth, _interface.m_fHeight );
}

IUInterface::~IUInterface( void )
{
}

unsigned int IUInterface::GetId( void ) const
{
	return m_uId;
}

IUInterface * IUInterface::GetItemById( unsigned int _uId )
{
    if ( GetId( ) == _uId )
    {
        return this;
    }

    return NULL;
}

bool IUInterface::CheckBounds( float _fX, float _fY )
{
    if ( ( _fX >= m_fPosX ) && ( _fY >= m_fPosY ) &&
         ( _fX <= ( m_fPosX + m_fWidth ) ) && ( _fY <= ( m_fPosY + m_fHeight ) ) )
    {
        return true;
    }
    else
    {
        return false;
    }
}

void IUInterface::Move( float _fPosX, float _fPosY )
{
	m_fPosX = _fPosX;
	m_fPosY = _fPosY;
}

float IUInterface::GetX( void ) const
{
	return m_fPosX;
}

float IUInterface::GetY( void ) const
{
	return m_fPosY;
}

float IUInterface::GetAbsX( void ) const
{
    float fX = GetX( );

    if ( m_pInterfaceParent )
    {
        fX += m_pInterfaceParent->GetAbsX( );
    }

    return fX;
}

float IUInterface::GetAbsY( void ) const
{
    float fY = GetY( );

    if ( m_pInterfaceParent )
    {
        fY += m_pInterfaceParent->GetAbsY( );
    }

    return fY;
}

void IUInterface::Resize( float _fWidth, float _fHeight )
{
	m_fWidth = _fWidth;
	m_fHeight = _fHeight;
}

float IUInterface::GetWidth( void ) const
{
	return m_fWidth;
}

float IUInterface::GetHeight( void ) const
{
	return m_fHeight;
}

void IUInterface::SetForegroundColor( float _fR, float _fG, float _fB, float _fA )
{
    m_ColorForeground.r = _fR;
    m_ColorForeground.g = _fG;
    m_ColorForeground.b = _fB;
    m_ColorForeground.a = _fA;
}

void IUInterface::SetBackgroundColor( float _fR, float _fG, float _fB, float _fA )
{
    m_ColorBackGround.r = _fR;
    m_ColorBackGround.g = _fG;
    m_ColorBackGround.b = _fB;
    m_ColorBackGround.a = _fA;
}

void IUInterface::SetBorderColor( float _fR, float _fG, float _fB, float _fA )
{
    m_ColorBorder.r = _fR;
    m_ColorBorder.g = _fG;
    m_ColorBorder.b = _fB;
    m_ColorBorder.a = _fA;
}

void IUInterface::SetCallBack( IUICallBack * _pInterfaceCallBack )
{
    m_CallBackInterface = _pInterfaceCallBack;
}

void IUInterface::SetParent( IUInterface * _pInterfaceParent )
{
    m_pInterfaceParent = _pInterfaceParent;
}

const IUInterface * IUInterface::GetParent( void ) const
{
    return m_pInterfaceParent;
}

void IUInterface::SetVisible( bool _bVisible )
{
    m_bVisible = _bVisible;
}

bool IUInterface::IsVisible( void ) const
{
    return m_bVisible;
}

void IUInterface::Draw( void )
{
    if ( m_bVisible )
    {
        DrawIntern( );
    }
}

void IUInterface::SetEnabled( bool _bEnabled )
{
    m_bEnabled = _bEnabled;
}

bool IUInterface::IsEnabled( void ) const
{
    return m_bEnabled;
}

unsigned int IUInterface::ProcessMsg( const CUIMessage & _msg )
{
    unsigned int uResult = 0;

    if ( IsEnabled( ) )
    {
        uResult = ProcessMsgIntern( _msg );

        if ( m_CallBackInterface )
        {
            m_CallBackInterface->CallBackFunction( *this, _msg );
        }
    }

    return uResult;
}

void IUInterface::DrawBackGround( void )
{
    g_Render.Blend( true );

    g_Render.SetColor( m_ColorBackGround.r, m_ColorBackGround.g, m_ColorBackGround.b, m_ColorBackGround.a );

    g_Render.DrawRectangle2D( m_fPosX, m_fPosY, m_fWidth, m_fHeight, true );

    g_Render.Blend( false );
}

void IUInterface::DrawBorders( void )
{
    g_Render.Blend( true );

    g_Render.SetColor( m_ColorBorder.r, m_ColorBorder.g, m_ColorBorder.b, m_ColorBorder.a );

    g_Render.DrawRectangle2D( m_fPosX, m_fPosY, m_fWidth, m_fHeight, false );

    g_Render.Blend( false );
}
