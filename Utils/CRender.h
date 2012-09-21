
#ifndef _CRender_h_
#define _CRender_h_

#include "Utils/TSingleton.h"
#include "Utils/CTexture.h"
#include "Math/TVector3D.h"

union stRenderVCoord
{
    struct
    {
        float x;
        float y;
        float z;
    };
    float pos[3];
};

class CRender
    : public Singleton<CRender>
{
public:

    CRender( void );

    ~CRender( void );

    void Enable( HWND _hWnd, HINSTANCE _hInst );

    void Disable( void );

    void Initialize( void );

    void Flush( void );

    void BeginScene( void );

    void EndScene( void );

    void SwapBuffers( void );

    void ClearBuffer( void );

    void TextureEnable( void );

    void TextureEnable( const CTexture & _tex );

    void TextureDisable( void );

    void SetOrthoMode( void );

    void SetPerspectiveSelectMode( float _x, float _y, float _width, float _height );

    void SetPerspectiveMode( double _dMinZ, double _dMaxZ );

    void SetColor( float _fR, float _fG, float _fB, float _fA );

    void Rotate( double _dAngle, double _dX, double _dY, double _dZ );

    void Translate( double _dX, double _dY, double _dZ );

    void LoadIdentity( void );

    void PushMatrix( void );

    void PopMatrix( void );

    void Blend( bool _bEnable );

    void DrawSphere( double _dRadius, int _hDiv, int _dDiv, bool _bShowInside );

    void DrawRectangle2D( double _dX, double _dY, double _dWidth, double _dHeight, bool _bFilled );

    void DrawArrow( PRECISIONVECTOR vStart, PRECISIONVECTOR vEnd );

    void DrawSolidCone( double _dRadius, double _dHeight );

    void DrawPoint( );

    void DrawLine( );

    void DrawTriangle( );

    void DrawPolygon( );

    void CreateSphere( );

    void DrawObject( );

    void SetLight( );

    HDC GetHDC( void );

    HWND GetHWND( void );

    HINSTANCE GetHINST( void );

    bool IsEnabled( void );

    float GetWidth( void );

    float GetHeight( void );

    void ResizeWindow( float _fWidth, float _fHeight );

private:

    float fWindowWidth;
    float fWindowHeight;
    float fAspectRatio;

    bool bIsEnabled;

    GLUquadricObj * pTempQuad;

    // Stores windows specific data
	HWND hWnd;
	HDC hDC;
	HGLRC hRC;
    HINSTANCE hInst;

};

#define g_Render CRender::GetSingleton( )

#endif