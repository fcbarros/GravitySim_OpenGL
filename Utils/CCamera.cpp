
#include "CCamera.h"
#include "Utils/CRender.h"

// Tangent of half 45 degrees
static const double tgT2 = 0.4142135623730950488016887242097;

CCamera::CCamera( void )
{
    Reset( );
}

void CCamera::Orbit( double _dAngleX, double _dAngleY, double _dAngleZ )
{
    m_vOrbit.x += _dAngleX;
    m_vOrbit.y += _dAngleY;
    m_vOrbit.z += _dAngleZ;
}

void CCamera::Pan( double _dX, double _dY, double _dZ )
{
    m_vPan.x += _dX;
    m_vPan.y += _dY;
    m_vPan.z += _dZ;
}

void CCamera::Rotate( double _dAngleX, double _dAngleY, double _dAngleZ )
{
    m_vAngle.x += _dAngleX;
    m_vAngle.y += _dAngleY;
    m_vAngle.z += _dAngleZ;
}

void CCamera::Reset( void )
{
    m_eType = CCamera::FREE;

    m_vPos = PRECISIONVECTOR( 0.0, 0.0, 1.0 );
    m_vLookAt = PRECISIONVECTOR( 0.0, 0.0, 0.0 );
    m_vUp = PRECISIONVECTOR( 0.0, 1.0, 0.0 );

    m_dDist = 1.0;

    m_vOrbit = PRECISIONVECTOR( 0.0, 0.0, 0.0 );

    m_vPan = PRECISIONVECTOR( 0.0, 0.0, 0.0 );

    m_vAngle = PRECISIONVECTOR( 0.0, 0.0, 0.0 );
}

void CCamera::Load( CFileLoader & _fl )
{
    _fl.SetCurrentElement( "position" );
    m_vPos.x = atof( _fl.GetVariableValue( "x" ).c_str( ) );
    m_vPos.y = atof( _fl.GetVariableValue( "y" ).c_str( ) );
    m_vPos.z = atof( _fl.GetVariableValue( "z" ).c_str( ) );
    _fl.Back( );

    _fl.SetCurrentElement( "lookat" );
    m_vLookAt.x = atof( _fl.GetVariableValue( "x" ).c_str( ) );
    m_vLookAt.y = atof( _fl.GetVariableValue( "y" ).c_str( ) );
    m_vLookAt.z = atof( _fl.GetVariableValue( "z" ).c_str( ) );
    _fl.Back( );

    _fl.SetCurrentElement( "up" );
    m_vUp.x = atof( _fl.GetVariableValue( "x" ).c_str( ) );
    m_vUp.y = atof( _fl.GetVariableValue( "y" ).c_str( ) );
    m_vUp.z = atof( _fl.GetVariableValue( "z" ).c_str( ) );
    _fl.Back( );

    PRECISIONVECTOR vTemp = m_vPos - m_vLookAt;
    double dDist = vTemp.Length( );
    SetDistance( dDist );
}

void CCamera::SetDistance( double _dDist )
{
    m_dDist = _dDist;
    m_vPos = m_vLookAt;
    m_vPos.z += m_dDist;
}

double CCamera::GetDistance( void )
{
    return m_dDist;
}

void CCamera::DoClear( void )
{
    g_Render.LoadIdentity( );
}

void CCamera::DoPosition( void )
{
    g_Render.Translate( 0.0, 0.0, -m_dDist );
}

void CCamera::DoOrbit( void )
{
    g_Render.Rotate( m_vOrbit.y, 0.0, 1.0, 0.0 );
    g_Render.Rotate( m_vOrbit.x, 1.0, 0.0, 0.0 );
}

void CCamera::DoLookAT( void )
{
    g_Render.Translate( -m_vLookAt.x, -m_vLookAt.y, -m_vLookAt.z );
}

void CCamera::DoFreeRotation( void )
{
    g_Render.Rotate( m_vAngle.y, 0.0, 1.0, 0.0 );
    g_Render.Rotate( m_vAngle.x, 1.0, 0.0, 0.0 );
}

double CCamera::GetPercentOfScreen( const PRECISIONVECTOR & _v1, const double _dSize )
{
    return _dSize / ( PRECISIONVECTOR( m_vPos - _v1 ).Length( ) * tgT2 ) ;
}

double CCamera::GetSizeForPercent( const PRECISIONVECTOR & _v1, const double _dPercent )
{
    return _dPercent * ( PRECISIONVECTOR( m_vPos - _v1 ).Length( ) * tgT2 );
}

double CCamera::GetDistanceForSize( const PRECISIONVECTOR & _v1 )
{
    return _v1.Length( ) / tgT2;
}

void CCamera::SetPosition( double _dX, double _dY, double _dZ )
{
    m_vPos.x = _dX;
    m_vPos.y = _dY;
    m_vPos.z = _dZ;
}

void CCamera::SetPosition( const PRECISIONVECTOR & _v1 )
{
    m_vPos = _v1;
}

void CCamera::SetLookAt( double _dX, double _dY, double _dZ )
{
    m_vLookAt.x = _dX;
    m_vLookAt.y = _dY;
    m_vLookAt.z = _dZ;

    if ( m_eType == CCamera::FREE )
    {
        m_vPos = m_vLookAt;
        m_vPos.z += m_dDist;
    }
}

void CCamera::SetLookAt( const PRECISIONVECTOR & _v1 )
{
    m_vLookAt = _v1;

    if ( m_eType == CCamera::FREE )
    {
        m_vPos = m_vLookAt;
        m_vPos.z += m_dDist;
    }
}

void CCamera::SetUp( double _dX, double _dY, double _dZ )
{
    m_vUp.x = _dX;
    m_vUp.y = _dY;
    m_vUp.z = _dZ;
}

void CCamera::SetUp( const PRECISIONVECTOR & _v1 )
{
    m_vUp = _v1;
}

const PRECISIONVECTOR & CCamera::GetPosition( void )
{
    return m_vPos;
}

const PRECISIONVECTOR & CCamera::GetLookAt( void )
{
    return m_vLookAt;
}

const PRECISIONVECTOR & CCamera::GetUp( void )
{
    return m_vUp;
}

const PRECISIONVECTOR & CCamera::GetOrbit( void )
{
    return m_vOrbit;
}
