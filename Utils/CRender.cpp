
#include "Utils/CRender.h"
#include "Math/CSpeedMath.h"


CRender::CRender( void )
{
    bIsEnabled = false;

    pTempQuad = NULL;
}

CRender::~CRender( void )
{
    if ( pTempQuad )
    {
        gluDeleteQuadric( pTempQuad );
    }

    Disable( );
}

void CRender::Initialize( void )
{
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	glShadeModel( GL_SMOOTH );
    glClearDepth( 1.0f );
    glEnable( GL_CULL_FACE );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    pTempQuad = gluNewQuadric( );
    gluQuadricDrawStyle( pTempQuad, GLU_FILL );
    gluQuadricTexture( pTempQuad, TRUE );
}

void CRender::BeginScene( )
{
    ClearBuffer( );
}

void CRender::EndScene( )
{
    SwapBuffers( );
}

void CRender::ClearBuffer( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

void CRender::TextureEnable( void )
{
    glEnable( GL_TEXTURE_2D );
}

void CRender::TextureEnable( const CTexture & _tex )
{
    glEnable( GL_TEXTURE_2D );
    _tex.Select( );
}

void CRender::TextureDisable( void )
{
    glDisable( GL_TEXTURE_2D );
}

void CRender::Rotate( double _dAngle, double _dX, double _dY, double _dZ )
{
    glRotated( _dAngle, _dX, _dY, _dZ );
}

void CRender::Translate( double _dX, double _dY, double _dZ )
{
    glTranslated( _dX, _dY, _dZ );
}

void CRender::LoadIdentity( void )
{
    glLoadIdentity( );
}

void CRender::PushMatrix( void )
{
    glPushMatrix( );
}

void CRender::PopMatrix( void )
{
    glPopMatrix( );
}

void CRender::Blend( bool _bEnable )
{
    _bEnable ? glEnable( GL_BLEND ) : glDisable( GL_BLEND );
}

void CRender::SetColor( float _fR, float _fG, float _fB, float _fA )
{
    glColor4f( _fR, _fG, _fB, _fA );
}

void CRender::DrawSphere( double _dRadius, int _hDiv, int _dDiv, bool _bShowInside )
{
    if ( _bShowInside )
    {
        glDisable( GL_CULL_FACE );
    }

    gluSphere( pTempQuad, _dRadius, _hDiv, _dDiv );

    glEnable( GL_CULL_FACE );
}

void CRender::DrawRectangle2D( double _dX, double _dY, double _dWidth, double _dHeight, bool _bFilled )
{
    if ( _bFilled )
    {
	    glBegin( GL_QUADS );

		    glTexCoord2f( 0.0f, 0.0f );
		    glVertex2d( _dX, _dY + _dHeight );

            glTexCoord2f( 1.0f, 0.0f );
		    glVertex2d( _dX + _dWidth, _dY + _dHeight );

		    glTexCoord2f( 1.0f, 1.0f );
		    glVertex2d( _dX + _dWidth, _dY );

		    glTexCoord2f( 0.0f, 1.0f );
		    glVertex2d( _dX, _dY );

        glEnd( );
    }
    else
    {
	    glBegin( GL_LINE_LOOP );

		    glVertex2d( _dX, _dY + _dHeight );
		    glVertex2d( _dX + _dWidth, _dY + _dHeight );
		    glVertex2d( _dX + _dWidth, _dY );
		    glVertex2d( _dX, _dY );

        glEnd( );
    }
}

void CRender::Flush( void )
{
}

void CRender::SwapBuffers( void )
{
    ::SwapBuffers( hDC );
}

void CRender::SetOrthoMode( void )
{
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );

    if ( fWindowWidth <= fWindowHeight )
    {
        glOrtho( 0, fWindowHeight, 
                 fWindowHeight * fAspectRatio, 0,
                 -1000000.0, 1000000.0 );
    }
    else 
    {
        glOrtho( 0, fWindowWidth * fAspectRatio, 
                 fWindowWidth, 0, 
                 -1000000.0, 1000000.0 );
    }

    glScaled( fAspectRatio, fAspectRatio, 1.0 );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );
}

void CRender::SetPerspectiveSelectMode( float _x, float _y, float _width, float _height )
{
    GLint viewport[4];

    // Geting the viewport
    glGetIntegerv( GL_VIEWPORT, viewport );

    // Change Matrix Mode to Projection
    glMatrixMode( GL_PROJECTION );        
    glLoadIdentity( );

    gluPickMatrix( _x, viewport[3] - _y, _width, _height, viewport );
    //gluPickMatrix( _x, _y, _width, _height, viewport );

    // Do the perspective calculations. Last value = max clipping depth
    gluPerspective( 45.0, fWindowWidth / fWindowHeight, 0.0, 100.0 );  

    // Return to the modelview matrix
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );
}

void CRender::SetPerspectiveMode( double _dMinZ, double _dMaxZ )
{
    // Change Matrix Mode to Projection
    glMatrixMode( GL_PROJECTION );        
    glLoadIdentity( );                   

    // Do the perspective calculations. Last value = max clipping depth
    gluPerspective( 45.0, fWindowWidth / fWindowHeight, _dMinZ, _dMaxZ );  
    //gluPerspective( 45.0, fWindowWidth / fWindowHeight, 0.1f, 100.0f );  

    // Return to the modelview matrix
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );
}

// Enable OpenGL for the specified Window Handle hWnd
void CRender::Enable( HWND _hWnd, HINSTANCE _hInst )
{
    PIXELFORMATDESCRIPTOR pfd;
    int format;

    hWnd = _hWnd;
    hInst = _hInst;
    
    // get the device context (DC)
    hDC = GetDC( hWnd );
    
    // set the pixel format for the DC
    ZeroMemory( &pfd, sizeof( pfd ) );
    pfd.nSize = sizeof( pfd );
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;
    format = ChoosePixelFormat( hDC, &pfd );
    SetPixelFormat( hDC, format, &pfd );
    
    // create and enable the render context (RC)
    hRC = wglCreateContext( hDC );
    wglMakeCurrent( hDC, hRC );

    bIsEnabled = true;
}

// Disable OpenGL
void CRender::Disable( void )
{
    if ( bIsEnabled )
    {
        wglMakeCurrent( NULL, NULL );
        wglDeleteContext( hRC );
        ReleaseDC( hWnd, hDC );

        hRC = NULL;
        hDC = NULL;

        bIsEnabled = false;
    }
}

HDC CRender::GetHDC( void )
{
    return hDC;
}

HWND CRender::GetHWND( void )
{
    return hWnd;
}

HINSTANCE CRender::GetHINST( void )
{
    return hInst;
}

bool CRender::IsEnabled( void )
{
    return bIsEnabled;
}

void CRender::ResizeWindow( float _fWidth, float _fHeight )
{
    fWindowWidth = _fWidth;
    fWindowHeight = _fHeight;
    fAspectRatio = ( fWindowWidth <= fWindowHeight ) ? 
        ( fWindowHeight / fWindowWidth ) : ( fWindowWidth / fWindowHeight );

    if ( IsEnabled( ) )
    {
        // Set the viewport for the OpenGL window
        glViewport( 0, 0, (GLint)fWindowWidth, (GLint)fWindowHeight );
    }
}

float CRender::GetWidth( void )
{
    return fWindowWidth;
}

float CRender::GetHeight( void )
{
    return fWindowHeight;
}

void CRender::DrawArrow( PRECISIONVECTOR vStart, PRECISIONVECTOR vEnd )
{
    static PRECISIONVECTOR vZ( 0.0, 0.0, 1.0 );
    PRECISIONVECTOR vDirection = vEnd - vStart;
    double Length = vDirection.Length( );
    PRECISIONVECTOR  vV = vZ.CrossP( vDirection );
    double Cos_Theta = vZ.DotP( vDirection ) / Length;
    double Theta = CSpeedMath::ACosTableD( Cos_Theta );
    //double Theta = acos( Cos_Theta ) * 57.295779513082320876798154814105; //(180 / PI)

    glLineWidth( 2.5 );
    glBegin( GL_LINES );
        glVertex3dv( vStart.v );
        glVertex3dv( vEnd.v );
    glEnd( );
    glLineWidth( 1.0 );

    glDisable( GL_CULL_FACE );

    glPushMatrix( );
    glTranslated( vStart.x, vStart.y, vStart.z );
    glRotated( Theta, vV.x, vV.y, vV.z );
    glTranslated( 0.0, 0.0, Length - 0.3 );
    DrawSolidCone( Length * 0.5, Length * 0.5 );
    glPopMatrix( );

    glEnable( GL_CULL_FACE );
}

void CRender::DrawSolidCone( double _dRadius, double _dHeight )
{
    static double dSin45 = 0.70710678118654752440084436210485;

    glBegin( GL_TRIANGLE_FAN );

        glVertex3d( 0.0, 0.0, _dHeight );
        glVertex3d( _dRadius, 0.0, 0.0 );
        glVertex3d( _dRadius * dSin45, _dRadius * dSin45, 0.0 );
        glVertex3d( 0.0, _dRadius, 0.0 );
        glVertex3d( -_dRadius * dSin45, _dRadius * dSin45, 0.0 );
        glVertex3d( -_dRadius, 0.0, 0.0 );
        glVertex3d( -_dRadius * dSin45, -_dRadius * dSin45, 0.0 );
        glVertex3d( 0.0, -_dRadius, 0.0 );
        glVertex3d( _dRadius * dSin45, -_dRadius * dSin45, 0.0 );
        glVertex3d( _dRadius, 0.0, 0.0 );

    glEnd( );
}
