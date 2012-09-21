
#ifndef _CGravity_h_
#define _CGravity_h_

#include "Utils/CFileLoader.h"
#include "Utils/CCamera.h"
#include "Utils/CRender.h"
#include "Interface/CUIWindow.h"
#include "CSimulator.h"

// Simple Enums for the Interface Items

enum eButtonIDs
{
INTERFACE_MAIN = 1,
INTERFACE_TEXT_BOX_ID,
INTERFACE_OBJECT_INFO_GROUP,
INTERFACE_OBJECT_INFO_TEXTBOX,
INTERFACE_OBJECT_INFO_BUTTON,
INTERFACE_OBJECT_INFO_CHECK_SPEED,
INTERFACE_OBJECT_INFO_CHECK_ACCELERATION,
INTERFACE_MOUSE_OVER_INFO,
INTERFACE_BUTTONS_GROUP,
INTERFACE_BUTTON_PLAY,
INTERFACE_BUTTON_PAUSE,
INTERFACE_BUTTON_ZOOM_UP,
INTERFACE_BUTTON_ZOOM_DOWN,
INTERFACE_BUTTON_ROT_RIGHT,
INTERFACE_BUTTON_ROT_LEFT,
INTERFACE_BUTTON_ROT_UP,
INTERFACE_BUTTON_ROT_DOWN,
INTERFACE_BUTTON_PAN_RIGHT,
INTERFACE_BUTTON_PAN_LEFT,
INTERFACE_BUTTON_PAN_UP,
INTERFACE_BUTTON_PAN_DOWN,
INTERFACE_BUTTON_GF,
INTERFACE_BUTTON_ADD,
INTERFACE_BUTTON_SUB
};


// CGravity Class Interface
class CGravity
    : public IUICallBack
{
public:

    // Default constructor
    CGravity( void );
    
    // Default destructor
    ~CGravity( void );

    // Initializes the system
    // Return: true if success, false otherwise
    bool Initialize( HWND _hWnd, HINSTANCE _hInst );

    // Relases window and memory Data
    void Release( void );

    // Resizes the window
    // Sets and calculate all necessary data
    void ResizeWindow( float _fWidth, float _fHeight );

    // Resets the system to the default values
    void Reset( void );

    // Process the loop
    void DoLoop( void );

    void Stop( void );

    // Open a file
    void OpenFile( string _stFileName );

    // Draws the scene
    void Draw( void );

    bool ProcessWin32Msg( HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam );

    bool IsInitialized( void );

    void ChangeIntegrator( int _iIntegrator );

    void ChangeGravityFieldType( CSimulator::enFieldType _Type );

protected:

    unsigned int CallBackFunction( const IUInterface & _InterfaceObj, const CUIMessage & _msg );

    void DrawGravityField( void );
    void DrawInterface( void );
    void InitializeInterface( void );
    bool InterfaceProcessMsg( HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam );
    void ProcessInterfaceEvents( bool _bProcess = false );
    void LoadFromFileLoader( void );
    void ParseConfig( void );
    int Pick( float _x, float _y );
    void DrawSkyBox( double _dRadius );

    CTexture SkyBox;

    CFileLoader FileLoader;

	CUIWindow * pMainInterface;

    CSimulator Simulator;

    CRender Render;

    CCamera Camera;
    CCamera CameraDefault;

    double dMouseX;
    double dMouseY;

    bool bIsInitialized;

	double dTimeOld;

    double dTimeInterface;
    double dTimeDraw;
    double dTimeFRDraw;

    double dCTETimeInterface;
    double dCTETimeDraw;
    double dCTETimeFRDraw;

    bool bRotateUp;
    bool bRotateDown;
    bool bRotateLeft;
    bool bRotateRight;

    bool bZoomUp;
    bool bZoomDown;

    bool bAdd;
    bool bSub;

    int iSelectedObject;

    int iMouseOverObject;

    int iIdIntegrator;

    bool bGravityField;

    double dMult;
};

#endif