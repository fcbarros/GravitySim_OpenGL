
#include "resource.h"
#include "CGravity.h"
#include "Interface/CUIGroup.h"
#include "Interface/CUIImageButton.h"
#include "Interface/CUIImagePushButton.h"
#include "Interface/CUISimpleTextBox.h"
#include "Interface/CUISimpleBG.h"
#include "Interface/CUICheckBox.h"
#include <Windows.h>
#include <commdlg.h>


static const double dMiliDivision = 1.0 / 1000.0;
static const double dAUDivision = 1.0 / 1.5e11;

double dFreq  = 0;
double dFrameRate = 0;

INT_PTR CALLBACK AddEditDialogCallback( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam );
CMassObject * pDialogItem = NULL;

CGravity::CGravity( void )
{
    pMainInterface = NULL;
    iIdIntegrator = 2;
    bGravityField = false;
    dMult = 1.0;

    bIsInitialized = false;

    dFreq  = ClockFrequency( );
}

CGravity::~CGravity( void )
{
    Release( );
}

bool CGravity::Initialize( HWND _hWnd, HINSTANCE _hInst )
{
    g_Render.Enable( _hWnd, _hInst );
    g_Render.Initialize( );

    Reset( );

    InitializeInterface( );

    bIsInitialized = true;

    return true;
}

void CGravity::InitializeInterface( void )
{
    pMainInterface = new CUIWindow( INTERFACE_MAIN );
    pMainInterface->Resize( g_Render.GetWidth( ), g_Render.GetHeight( ) );

    pMainInterface->Add( new CUISimpleTextBox( INTERFACE_TEXT_BOX_ID,
                                                5.0f, g_Render.GetHeight( ) - 95.0f,
                                                450.0f, 90.0f ) );

    pMainInterface->Add( new CUISimpleTextBox( INTERFACE_MOUSE_OVER_INFO,
                                                0.0f, 0.0f,
                                                -1.0f, -1.0f ) );

    CUIGroup * pInterfaceObjectInfo = new CUIGroup( INTERFACE_OBJECT_INFO_GROUP );
    pInterfaceObjectInfo->Move( 5.0f, 5.0f );

    pInterfaceObjectInfo->Add( new CUISimpleTextBox( INTERFACE_OBJECT_INFO_TEXTBOX,
                                                     0.0f, 0.0f,
                                                     400.0f, 150.0f ) );

    pInterfaceObjectInfo->Add( new CUIImageButton( INTERFACE_OBJECT_INFO_BUTTON, 
                                                   400.0 - 28.0, 2.0,
                                                   -1, 0,
                                                   this,
                                                   "images/ButtonAddNormal.tga",
                                                   "images/ButtonAddPressed.tga",
                                                   "images/ButtonAddOver.tga" ) );

    pInterfaceObjectInfo->Add( new CUICheckBox( INTERFACE_OBJECT_INFO_CHECK_SPEED,
                                                "Show Speed Vector",
                                                10.0f, 150.0f - 45.0f,
                                                this ) );

    pInterfaceObjectInfo->Add( new CUICheckBox( INTERFACE_OBJECT_INFO_CHECK_ACCELERATION,
                                                "Show Acceleration Vector",
                                                10.0f, 150.0f - 25.0f,
                                                this ) );

    pMainInterface->Add( pInterfaceObjectInfo );

    CUIGroup * pInterfaceButtons = new CUIGroup( INTERFACE_BUTTONS_GROUP );
    pInterfaceButtons->Move( g_Render.GetWidth( ) - 95.0f, 0.0f );

    pInterfaceButtons->Add( new CUIImageButton( INTERFACE_BUTTON_PLAY, 
                                                10.0, 10.0,
                                                -1, 0,
                                                this,
                                                "images/ButtonPlayNormal.tga",
                                                "images/ButtonPlayPressed.tga",
                                                "images/ButtonPlayOver.tga" ) );

    pInterfaceButtons->Add( new CUIImageButton( INTERFACE_BUTTON_PAUSE, 
                                                52.0, 10.0,
                                                -1, 0,
                                                this,
                                                "images/ButtonPauseNormal.tga",
                                                "images/ButtonPausePressed.tga",
                                                "images/ButtonPauseOver.tga" ) );
                                           
    pInterfaceButtons->Add( new CUIImageButton( INTERFACE_BUTTON_ZOOM_UP, 
                                                28.0, 222.0,
                                                -1, 0,
                                                this,
                                                "images/ZoomUpNormal.tga",
                                                "images/ZoomUpPressed.tga",
                                                "images/ZoomUpOver.tga" ) );

    pInterfaceButtons->Add( new CUIImageButton( INTERFACE_BUTTON_ZOOM_DOWN, 
                                                48.0, 222.0,
                                                -1, 0,
                                                this,
                                                "images/ZoomDownNormal.tga",
                                                "images/ZoomDownPressed.tga",
                                                "images/ZoomDownOver.tga" ) );

    pInterfaceButtons->Add( new CUIImagePushButton( INTERFACE_BUTTON_GF, 
                                                    12.0, 264.0,
                                                    -1, 0,
                                                    this,
                                                    "images/ButtonGFNormal.tga",
                                                    "images/ButtonGFPressed.tga" ) );

    pInterfaceButtons->Add( new CUIImageButton( INTERFACE_BUTTON_ADD, 
                                                20.0, 294.0,
                                                -1, 0,
                                                this,
                                                "images/ButtonAddNormal.tga",
                                                "images/ButtonAddPressed.tga",
                                                "images/ButtonAddOver.tga" ) );

    pInterfaceButtons->Add( new CUIImageButton( INTERFACE_BUTTON_SUB, 
                                                52.0, 294.0,
                                                -1, 0,
                                                this,
                                                "images/ButtonSubNormal.tga",
                                                "images/ButtonSubPressed.tga",
                                                "images/ButtonSubOver.tga" ) );

    pInterfaceButtons->Add( new CUIImageButton( INTERFACE_BUTTON_ROT_RIGHT, 
                                                58.0, 154.0,
                                                -1, 0,
                                                this,
                                                "images/ButtonRotateRightNormal.tga",
                                                "images/ButtonRotateRightPressed.tga",
                                                "images/ButtonRotateRightOver.tga" ) );

    pInterfaceButtons->Add( new CUIImageButton( INTERFACE_BUTTON_ROT_LEFT, 
                                                16.0, 154.0,
                                                -1, 0,
                                                this,
                                                "images/ButtonRotateLeftNormal.tga",
                                                "images/ButtonRotateLeftPressed.tga",
                                                "images/ButtonRotateLeftOver.tga" ) );

    pInterfaceButtons->Add( new CUIImageButton( INTERFACE_BUTTON_ROT_UP, 
                                                35.0, 134.0,
                                                -1, 0,
                                                this,
                                                "images/ButtonRotateUpNormal.tga",
                                                "images/ButtonRotateUpPressed.tga",
                                                "images/ButtonRotateUpOver.tga" ) );

    pInterfaceButtons->Add( new CUIImageButton( INTERFACE_BUTTON_ROT_DOWN, 
                                                35.0, 178.0,
                                                -1, 0,
                                                this,
                                                "images/ButtonRotateDownNormal.tga",
                                                "images/ButtonRotateDownPressed.tga",
                                                "images/ButtonRotateDownOver.tga" ) );

    pInterfaceButtons->Add( new CUIImageButton( INTERFACE_BUTTON_PAN_RIGHT, 
                                                55.0, 81.0,
                                                -1, 0,
                                                this,
                                                "images/ButtonPanRightNormal.tga",
                                                "images/ButtonPanRightPressed.tga",
                                                "images/ButtonPanRightOver.tga" ) );

    pInterfaceButtons->Add( new CUIImageButton( INTERFACE_BUTTON_PAN_LEFT, 
                                                17.0, 81.0,
                                                -1, 0,
                                                this,
                                                "images/ButtonPanLeftNormal.tga",
                                                "images/ButtonPanLeftPressed.tga",
                                                "images/ButtonPanLeftOver.tga" ) );

    pInterfaceButtons->Add( new CUIImageButton( INTERFACE_BUTTON_PAN_UP, 
                                                38.0, 59.0,
                                                -1, 0,
                                                this,
                                                "images/ButtonPanUpNormal.tga",
                                                "images/ButtonPanUpPressed.tga",
                                                "images/ButtonPanUpOver.tga" ) );

    pInterfaceButtons->Add( new CUIImageButton( INTERFACE_BUTTON_PAN_DOWN, 
                                                  38.0, 99.0,
                                                  -1, 0,
                                                  this,
                                                  "images/ButtonPanDownNormal.tga",
                                                  "images/ButtonPanDownPressed.tga",
                                                  "images/ButtonPanDownOver.tga" ) );

    pMainInterface->Add( pInterfaceButtons );
}

void CGravity::Release( void )
{
    delete pMainInterface;
    pMainInterface = NULL;

    g_Render.Disable( );
}

void CGravity::OpenFile( string _stFileName  )
{
    Reset( );

    if ( FileLoader.OpenFile( _stFileName ) )
    {
        LoadFromFileLoader( );
        FileLoader.Close( );
    }

    Draw( );
    //Simulator.Run( );
}

void CGravity::ResizeWindow( float _fWidth, float _fHeight )
{
    g_Render.ResizeWindow( _fWidth, _fHeight );

    if ( bIsInitialized )
    {
        pMainInterface->Resize( _fWidth, _fHeight );
        CUIGroup * pInterfaceButtons = (CUIGroup *)pMainInterface->GetItemById( INTERFACE_BUTTONS_GROUP );
        pInterfaceButtons->Move( g_Render.GetWidth( ) - 95.0f, 0.0f );
    }
}

void CGravity::Reset( void )
{
    // Clearing the array of objects
    Simulator.Reset( );

    // Times
    dTimeOld = 0;

    dTimeInterface = 0.0;
    dTimeDraw = 0.0;
    dTimeFRDraw = 0.0;

    dCTETimeInterface = 0.01;
    dCTETimeDraw = 0.01;
    dCTETimeFRDraw = 1.0;

    bRotateUp = false;
    bRotateDown = false;
    bRotateLeft = false;
    bRotateRight = false;

    bZoomUp = false;
    bZoomDown = false;

    bAdd = false;
    bSub = false;

    iSelectedObject = -1;
    iMouseOverObject = -1;

    // Setting default Camera postion and direction
    Camera.Reset( );
}

void CGravity::Stop( void )
{
    Simulator.Stop( );
}

void CGravity::DoLoop( void )
{
    double dTime = (double)GetTickCount( ) * dMiliDivision;
    double dTimeDiff = dTime - dTimeOld;
    if ( dTimeOld == 0 )
    {
        dTimeDiff = 0;
    }
    dTimeOld = dTime;

    if ( iIdIntegrator == 1 )
    {
        Simulator.MoveBodies( dTimeDiff );
    }
    else
    {
        Simulator.MoveBodiesTest( dTimeDiff );
    }

    dTimeDraw += dTimeDiff;
    dTimeInterface += dTimeDiff;
    dTimeFRDraw += dTimeDiff;

    if ( dTimeDraw > dCTETimeDraw )
    {
        Draw( );

        g_Render.SwapBuffers( );
        dTimeDraw = 0.0;
    }

    static double dNumTimesFR = 0.0;
    ++dNumTimesFR;
    if ( dTimeFRDraw > dCTETimeFRDraw )
    {
        //dFrameRate = 1.0 / dTimeDiff;
        dFrameRate = dNumTimesFR / dTimeFRDraw;

        dTimeFRDraw = 0.0;
        dNumTimesFR = 0.0;
    }

    ProcessInterfaceEvents( );
}

void CGravity::DrawInterface( void )
{
    static string FormatedDate = Simulator.SimulatedTimeFormat( );
    char myText[ 4096 ];

    g_Render.SetOrthoMode( );

    CUISimpleTextBox * pInterfaceTextBox = (CUISimpleTextBox *)pMainInterface->GetItemById( INTERFACE_TEXT_BOX_ID );

    pInterfaceTextBox->Clear( );

    //sprintf_s( myText, "mX: %3.5f   mY: %3.5f   Selected: %d", dMouseX, dMouseY, iSelectedObject );
    sprintf_s( myText, "FrameRate: %3.5f", dFrameRate / 1000.0 );
    pInterfaceTextBox->AddLines( myText );

    FormatedDate = Simulator.SimulatedTimeFormat( );
    sprintf_s( myText, "Tempo Real: %3.2fs  CT: %3.2fs   Sim: %s", 
        Simulator.GetTimeReal( ), 
        Simulator.GetTimeConstant( ),
        FormatedDate.c_str( ), FormatedDate.length( ) );
    pInterfaceTextBox->AddLines( myText );
    sprintf_s( myText, "Distance: %3.2f", Camera.GetDistance( ) );
    pInterfaceTextBox->AddLines( myText );

    if ( bGravityField )
    {
        sprintf_s( myText, "Gravity Field: ON    Multiplier: %f", dMult );
        pInterfaceTextBox->AddLines( myText );
    }

    CUISimpleTextBox * pInterfaceTextBoxInfoObject = (CUISimpleTextBox *)pMainInterface->GetItemById( INTERFACE_OBJECT_INFO_TEXTBOX );
    pInterfaceTextBoxInfoObject->SetEnabled( false );
    if ( iSelectedObject >= 0 )
    {
        pMainInterface->GetItemById( INTERFACE_OBJECT_INFO_GROUP )->SetVisible( true );
        pInterfaceTextBoxInfoObject->Clear( );

        CMassObject & mObject = Simulator.GetObject( iSelectedObject );

        sprintf_s( myText, "Name: %s", mObject.GetName( ).c_str( ),
            mObject.GetName( ).length( ) );
        pInterfaceTextBoxInfoObject->AddLines( myText );

        sprintf_s( myText, "Mass: %3.3f Ton", mObject.dMass_Kg / 1000.0 );
        pInterfaceTextBoxInfoObject->AddLines( myText );

        sprintf_s( myText, "Radius: %3.3f km", mObject.dRadius_m / 1000.0 );
        pInterfaceTextBoxInfoObject->AddLines( myText );

        sprintf_s( myText, "Position: ( %3.3f, %3.3f, %3.3f ) A.U.", 
            mObject.vPosition.x * dAUDivision, mObject.vPosition.y * dAUDivision, mObject.vPosition.z * dAUDivision );
        pInterfaceTextBoxInfoObject->AddLines( myText );

        sprintf_s( myText, "Vel. Vector: ( %3.3f, %3.3f, %3.3f ) km/s", 
            mObject.vVelocity.x / 1000.0, mObject.vVelocity.y / 1000.0, mObject.vVelocity.z / 1000.0 );
        pInterfaceTextBoxInfoObject->AddLines( myText );

        sprintf_s( myText, "Absolute Velocity: %3.3f km/s", mObject.vVelocity.Length( ) / 1000.0 );
        pInterfaceTextBoxInfoObject->AddLines( myText );

        CUICheckBox * pCheck = (CUICheckBox *)pMainInterface->GetItemById( INTERFACE_OBJECT_INFO_CHECK_SPEED );
        pCheck->SetChecked( mObject.IsShowVelocityVector( ) );

        pCheck = (CUICheckBox *)pMainInterface->GetItemById( INTERFACE_OBJECT_INFO_CHECK_ACCELERATION );
        pCheck->SetChecked( mObject.IsShowAccelerationVector( ) );

        /*
        if ( ( Simulator.GetNumObjects( ) > 6 ) && ( mObject.GetName( ) == "New Horizons" ) )
        {
            static double dNew, dOld = DBL_MAX;
            PRECISIONVECTOR vTemp = mObject.vPosition - Simulator.GetObject( 6 ).vPosition;
            dNew = vTemp.Length( );
            sprintf_s( myText, "Distance from Jupiter: %3.5f km", dNew / 1000.0 );
            pInterfaceTextBoxInfoObject->AddLines( myText );
            if ( dNew > dOld )
            {
                //Simulator.Pause( true );
            }
            else if ( dNew > 0 )
            {
                dOld = dNew;
            }
        }
        */
    }
    else
    {
        pMainInterface->GetItemById( INTERFACE_OBJECT_INFO_GROUP )->SetVisible( false );
    }

    CUISimpleTextBox * pInterfaceTextBoxMouseOver = (CUISimpleTextBox *)pMainInterface->GetItemById( INTERFACE_MOUSE_OVER_INFO );
    pInterfaceTextBoxMouseOver->SetEnabled( false );
    if ( iMouseOverObject >= 0 )
    {
        pInterfaceTextBoxMouseOver->SetVisible( true );

        pInterfaceTextBoxMouseOver->Clear( );

        CMassObject & mObject = Simulator.GetObject( iMouseOverObject );

        pInterfaceTextBoxMouseOver->AddLines( mObject.GetName( ) );
    }
    else
    {
        pInterfaceTextBoxMouseOver->SetVisible( false );
    }

    pMainInterface->Draw( );
}

int CGravity::Pick( float _x, float _y )
{
    static GLuint selectBuf[ 10000 ]; // up to 2500 objects

    glSelectBuffer( sizeof( selectBuf ), selectBuf );
    glRenderMode( GL_SELECT );

    glInitNames( );
    glPushName( 0 );

    g_Render.SetPerspectiveSelectMode( _x, _y, 5.0f, 5.0f );

    if ( iSelectedObject >= 0 )
    {
        Camera.SetLookAt( Simulator.GetObject( iSelectedObject ).vPosition );
    }

    Camera.DoPosition( );
    Camera.DoOrbit( );
    Camera.DoLookAT( );

    for ( int i = Simulator.GetNumObjects( ) - 1; i >= 0 ; --i )
    {
        glLoadName( i );
        Simulator.GetObject( i ).Draw( Camera, false );
    }

    int hits = glRenderMode( GL_RENDER );

    int iSelect = -1;

    double dMaxMass = 0.0;
    for ( int i = 0; i < hits; ++i )
    {
        int iObj = selectBuf[ i * 4 + 3 ];
        if ( Simulator.GetObject( iObj ).dMass_Kg > dMaxMass )
        {
            dMaxMass = Simulator.GetObject( iObj ).dMass_Kg;
            iSelect = iObj;
        }
    }

    return iSelect;
}

void CGravity::DrawSkyBox( double _dRadius )
{
    if ( SkyBox.IsLoaded( ) )
    {
        g_Render.TextureEnable( SkyBox );

        g_Render.SetColor( 1.0f, 1.0f, 1.0f, 1.0f );

        g_Render.DrawSphere( _dRadius, 100, 100, true );

        g_Render.TextureDisable( );
    }
}

void CGravity::DrawGravityField( void )
{
    double dSize = Camera.GetSizeForPercent( Camera.GetLookAt( ), 1.0 );

    int iNumDivisions = Simulator.GetFieldType( ) == CSimulator::TYPE_2D_VECTOR ? 30 : 160;
    Simulator.CalcGravityField( Camera.GetLookAt( ), dSize, iNumDivisions, dMult );
}

void CGravity::Draw( void )
{
    g_Render.ClearBuffer( );

    double dXMin, dYMin, dZMin;
    double dXMax, dYMax, dZMax;

    Simulator.GetBoundingBox( dXMin, dYMin, dZMin, dXMax, dYMax, dZMax );
    g_Render.SetPerspectiveMode( 100000.0, dZMax );

    if ( iSelectedObject >= 0 )
    {
        Camera.SetLookAt( Simulator.GetObject( iSelectedObject ).vPosition );
    }

    Camera.DoPosition( );
    Camera.DoOrbit( );

    if ( ! bGravityField )
    {
        DrawSkyBox( Camera.GetDistance( ) * 2.0 );
    }

    Camera.DoLookAT( );

    if ( bGravityField )
    {
        DrawGravityField( );

        for ( int i = (int)Simulator.GetNumObjects( ) - 1; i >= 0 ; --i )
        {
            Simulator.GetObject( i ).Draw( Camera, false );
        }
    }
    else
    {
        for ( int i = (int)Simulator.GetNumObjects( ) - 1; i >= 0 ; --i )
        {
            if ( Simulator.GetObject( i ).IsLight( ) )
            {
                Simulator.GetObject( i ).Draw( Camera, true );
            }
        }

        for ( int i = (int)Simulator.GetNumObjects( ) - 1; i >= 0 ; --i )
        {
            if ( ! Simulator.GetObject( i ).IsLight( ) )
            {
                Simulator.GetObject( i ).Draw( Camera, true );
            }
        }

        for ( int i = 0; i < CMassObject::GetTotalLights( ); ++i )
        {
            glDisable( GL_LIGHT0 + i );
        }
    }
    if ( dTimeInterface > dCTETimeInterface )
    {
        DrawInterface( );

        dTimeInterface = 0.0;
    }
}

bool CGravity::IsInitialized( void )
{
    return bIsInitialized;
}

void CGravity::ProcessInterfaceEvents( bool )
{
    static double dTimeEvent = 0.0;
    static double dTimeEventOld = 0.0;
    double dTime = (double)GetTickCount( ) * dMiliDivision;

    if ( dTimeEvent > 0.1 )
    {
        if ( bRotateUp )
        {
            Camera.Orbit( -0.2, 0.0, 0.0 );
        }
        if ( bRotateDown )
        {
            Camera.Orbit( 0.2, 0.0, 0.0 );
        }
        if ( bRotateRight )
        {
            Camera.Orbit( 0.0, 0.2, 0.0 );
        }
        if ( bRotateLeft )
        {
            Camera.Orbit( 0.0, -0.2, 0.0 );
        }
        if ( bZoomUp )
        {
            double dDist = Camera.GetDistance( );
            Camera.SetDistance( dDist - dDist / 5.0 );
        }
        if ( bZoomDown )
        {
            double dDist = Camera.GetDistance( );
            Camera.SetDistance( dDist + dDist / 5.0 );
        }
        if ( bAdd )
        {
            dMult *= 2.0;
        }
        if ( bSub )
        {
            dMult /= 2.0;
        }

        dTimeEvent = 0.0;
    }
    dTimeEvent += dTime - dTimeEventOld;
    dTimeEventOld = dTime;
}


bool CGravity::InterfaceProcessMsg( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    if ( ! IsInitialized( ) )
    {
        return false;
    }

    // returns the ID o the item processed
    return pMainInterface->ProcessWin32Msg( hWnd, message, wParam, lParam ) != 0;
}

bool CGravity::ProcessWin32Msg( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	static bool bLButtonDown = false;
	static bool bRButtonDown = false;
    static float fXMouse = 0.0, 
                 fYMouse = 0.0;
	static float fXPosL, fYPosL;
	static float fXPosR, fYPosR;

    if ( ! InterfaceProcessMsg( hWnd, message, wParam, lParam ) )
    {
	    switch ( message )
	    {
	    case WM_LBUTTONDBLCLK:
            {
                if ( iSelectedObject >= 0 )
                {
                    iSelectedObject = -1;
                }
                else
                {
                    Camera = CameraDefault;
                }
            }
		    break;

	    case WM_LBUTTONDOWN:
            {
		        bLButtonDown = true;
		        fXPosL = LOWORD( lParam );
		        fYPosL = HIWORD( lParam );
            }
		    break;

	    case WM_RBUTTONDOWN:
            {
		        bRButtonDown = true;
		        fXPosR = LOWORD( lParam );
		        fYPosR = HIWORD( lParam );
            }
		    break;

	    case WM_LBUTTONUP:
            {
		        bLButtonDown = false;
                int iSel = Pick( LOWORD( lParam ), HIWORD( lParam ) );
                if ( iSel >= 0 )
                {
                    iSelectedObject = iSel;
                }
            }
		    break;

	    case WM_RBUTTONUP:
            {
		        bRButtonDown = false;
            }
		    break;

        case WM_MOUSEMOVE:
            { 
                fXMouse = LOWORD( lParam );
                fYMouse = HIWORD( lParam );
                dMouseX = fXMouse;
                dMouseY = fYMouse;

		        if ( bLButtonDown && ! bRButtonDown )
		        {
                    double XDif = ( fXPosL - fXMouse ) / g_Render.GetWidth( );
                    double YDif = ( fYPosL - fYMouse ) / g_Render.GetHeight( );

                    Camera.Orbit( -YDif * 180.0, -XDif * 180.0, 0.0 );

			        fXPosL = fXMouse;
			        fYPosL = fYMouse;
		        }
		        else if ( bLButtonDown && bRButtonDown )
		        {
			        double ZDif = ( fYPosL - fYMouse ) / g_Render.GetHeight( );

                    double dDist = Camera.GetDistance( );
                    Camera.SetDistance( dDist - ( dDist * ZDif * 2.0 ) );

                    fYPosL = fYMouse;
		        } 
                else
                {
                    iMouseOverObject = Pick( fXMouse, fYMouse );
                    if ( iMouseOverObject >= 0 )
                    {
                        CUISimpleTextBox * pInterfaceTextBoxMouseOver = (CUISimpleTextBox*)pMainInterface->GetItemById( INTERFACE_MOUSE_OVER_INFO );
                        pInterfaceTextBoxMouseOver->Move( fXMouse, fYMouse );
                    }
                }
            }
		    break;

        case WM_MOUSEWHEEL:
            {
                double zDelta = (double)GET_WHEEL_DELTA_WPARAM( wParam ) / 120;
                //LONG fwKeys = GET_KEYSTATE_WPARAM( wParam );

                double dDist = Camera.GetDistance( );
                Camera.SetDistance( dDist - ( dDist * ( zDelta / 5.0 ) ) );
            }
            break;

        case WM_SIZE:
            if ( ( wParam != SIZE_MAXHIDE ) && ( wParam != SIZE_MAXSHOW ) &&
                 ( wParam != SIZE_MINIMIZED ) )
            {
                float fNewWidth = LOWORD( lParam );
                float fNewHeight = HIWORD( lParam );
                ResizeWindow( fNewWidth, fNewHeight );
            }
            break;

        default:
            return false;
            break;
        }
    }

    return true;
}

unsigned int CGravity::CallBackFunction( const IUInterface & _InterfaceObj, const CUIMessage & _msg )
{
    unsigned int uItemId = _InterfaceObj.GetId( );

    CUIImageButton* pInterfaceButton = NULL;

    switch ( uItemId )
    {
    case INTERFACE_BUTTON_ROT_UP:
        pInterfaceButton = (CUIImageButton*)&_InterfaceObj;
        bRotateUp = ( pInterfaceButton->GetState( ) == CUIImageButton::PRESSED );
        break;
    case INTERFACE_BUTTON_ROT_DOWN:
        pInterfaceButton = (CUIImageButton*)&_InterfaceObj;
        bRotateDown = ( pInterfaceButton->GetState( ) == CUIImageButton::PRESSED );
        break;
    case INTERFACE_BUTTON_ROT_RIGHT:  
        pInterfaceButton = (CUIImageButton*)&_InterfaceObj;
        bRotateRight = ( pInterfaceButton->GetState( ) == CUIImageButton::PRESSED );
        break;
    case INTERFACE_BUTTON_ROT_LEFT:
        pInterfaceButton = (CUIImageButton*)&_InterfaceObj;
        bRotateLeft = ( pInterfaceButton->GetState( ) == CUIImageButton::PRESSED );
        break;

    case INTERFACE_BUTTON_PAUSE:
        pInterfaceButton = (CUIImageButton*)&_InterfaceObj;
        if ( pInterfaceButton->GetState( ) == CUIImageButton::PRESSED )
        {
            dTimeOld = 0;
            Simulator.Pause( true );
        }
        break;

    case INTERFACE_BUTTON_PLAY:
        pInterfaceButton = (CUIImageButton*)&_InterfaceObj;
        if ( pInterfaceButton->GetState( ) == CUIImageButton::PRESSED )
        {
            dTimeOld = 0;
            Simulator.Pause( false );
        }
        break;

    case INTERFACE_BUTTON_ZOOM_UP:    
        pInterfaceButton = (CUIImageButton*)&_InterfaceObj;
        bZoomUp = ( pInterfaceButton->GetState( ) == CUIImageButton::PRESSED );
        break;
    case INTERFACE_BUTTON_ZOOM_DOWN:
        pInterfaceButton = (CUIImageButton*)&_InterfaceObj;
        bZoomDown = ( pInterfaceButton->GetState( ) == CUIImageButton::PRESSED );
        break;

    case INTERFACE_BUTTON_GF:
        {
            CUIImagePushButton * pInterfacePushButton = (CUIImagePushButton*)&_InterfaceObj;
            bGravityField = ( pInterfacePushButton->GetState( ) == CUIImagePushButton::PRESSED );
        }
        break;

    case INTERFACE_BUTTON_SUB:
        pInterfaceButton = (CUIImageButton*)&_InterfaceObj;
        bSub = ( pInterfaceButton->GetState( ) == CUIImageButton::PRESSED );
        break;

    case INTERFACE_BUTTON_ADD:
        pInterfaceButton = (CUIImageButton*)&_InterfaceObj;
        bAdd = ( pInterfaceButton->GetState( ) == CUIImageButton::PRESSED );
        break;

    case INTERFACE_OBJECT_INFO_BUTTON:
        pInterfaceButton = (CUIImageButton*)&_InterfaceObj;
        if ( pInterfaceButton->GetState( ) == CUIImageButton::PRESSED )
        {
            pDialogItem = &Simulator.GetObject( iSelectedObject );
            DialogBox( g_Render.GetHINST( ), 
                       MAKEINTRESOURCE( IDD_DIALOG_ADDEDIT_OBJECT ), 
                       g_Render.GetHWND( ), 
                       AddEditDialogCallback );
        }
        break;

    case INTERFACE_OBJECT_INFO_CHECK_SPEED:
        {
            if ( iSelectedObject >= 0 )
            {
                CUICheckBox * pInterfaceCheck = (CUICheckBox*)&_InterfaceObj;
                Simulator.GetObject( iSelectedObject ).ShowVelocityVector( pInterfaceCheck->IsChecked( ) );
            }
        }
        break;

    case INTERFACE_OBJECT_INFO_CHECK_ACCELERATION:
        {
            if ( iSelectedObject >= 0 )
            {
                CUICheckBox * pInterfaceCheck = (CUICheckBox*)&_InterfaceObj;
                Simulator.GetObject( iSelectedObject ).ShowAccelerationVector( pInterfaceCheck->IsChecked( ) );
            }
        }
        break;
    }

    return 0;
}

void CGravity::ParseConfig( void )
{
    double dTemp = atof( FileLoader.GetVariableValue( "time_constant" ).c_str( ) );
    if ( dTemp > 0 )
    {
        Simulator.SetTimeConstant( dTemp );
    }
    dTemp = atof( FileLoader.GetVariableValue( "num_iterations" ).c_str( ) );
    if ( dTemp > 0 )
    {
        Simulator.SetNumIterations( dTemp );
    }
    dTemp = atof( FileLoader.GetVariableValue( "history_save_interval" ).c_str( ) );
    if ( dTemp > 0 )
    {
        Simulator.SetHistorySaveInterval( dTemp );
    }
}

void CGravity::LoadFromFileLoader( void )
{
    bool bCameraSet = false;

    CUISimpleTextBox * pInterfaceTextBoxMouseOver = (CUISimpleTextBox *)pMainInterface->GetItemById( INTERFACE_MOUSE_OVER_INFO );
    pInterfaceTextBoxMouseOver->SetEnabled( false );
    pInterfaceTextBoxMouseOver->SetVisible( true );
    pInterfaceTextBoxMouseOver->Clear( );
    pInterfaceTextBoxMouseOver->AddLines( "         " );
    pInterfaceTextBoxMouseOver->AddLines( " Loading " );
    pInterfaceTextBoxMouseOver->AddLines( "         " );
    pInterfaceTextBoxMouseOver->Move( g_Render.GetWidth( ) / 2.0f, g_Render.GetHeight( ) / 2.0f );
    g_Render.SetOrthoMode( );
    pInterfaceTextBoxMouseOver->Draw( );
    g_Render.SwapBuffers( );

    // Points the XML to the root
    FileLoader.SetRoot( );

    // Get the num of children of the root
    unsigned int nC = FileLoader.GetNumChildren( );

    Simulator.Reserve( nC );

    // Loops through the children
    for ( unsigned int i = 0; i < nC; ++i )
    {
        // Set the current element as the i children
        FileLoader.SetCurrentElement( i );

        // Get the Element type
        string stTipo = FileLoader.GetElementName( );

        // If time is body load it and push into the vector
        if ( stTipo == "body" )
        {
            CMassObject mObject;

            if ( mObject.Load( FileLoader ) )
            {
                Simulator.AddObject( mObject );
            }
        }
        else if ( stTipo == "config" )
        {
            ParseConfig( );
        }
        else if ( stTipo == "camera" )
        {
            Camera.Load( FileLoader );
            bCameraSet = true;
        }
        else if ( stTipo == "time" )
        {
            Simulator.TimeLoad( FileLoader );
        }
        else if ( stTipo == "skybox" )
        {
            SkyBox.LoadFromFile( FileLoader.GetVariableValue( "file" ) );
        }

        // Gets the current element back at root
        FileLoader.Back( );
    }

    if ( ! bCameraSet )
    {
        double dXMin, dYMin, dZMin;
        double dXMax, dYMax, dZMax;

        Simulator.GetBoundingBox( dXMin, dYMin, dZMin, dXMax, dYMax, dZMax );

        PRECISIONVECTOR vTemp( dXMax - dXMin, dYMax - dYMin, dZMax - dZMin );

        double d = Camera.GetDistanceForSize( vTemp );
        Camera.SetDistance( d );
    }
    CameraDefault = Camera;

    pInterfaceTextBoxMouseOver->SetVisible( false );
}

void CGravity::ChangeIntegrator( int _iIntegrator )
{
    iIdIntegrator = _iIntegrator;
}


void CGravity::ChangeGravityFieldType( CSimulator::enFieldType _Type )
{
    Simulator.SetFieldType( _Type );
}

// Message handler for about box.
INT_PTR CALLBACK AddEditDialogCallback( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
	UNREFERENCED_PARAMETER( lParam );
	switch ( message )
	{
	case WM_INITDIALOG:
        {
            char buffer[ 1024 ];
            SetDlgItemText( hDlg, IDC_EDIT_AEO_NAME, pDialogItem->GetName( ).c_str( ) );
            _gcvt( pDialogItem->dMass_Kg, 20, buffer );
            SetDlgItemText( hDlg, IDC_EDIT_AEO_MASS, buffer );
            _gcvt( pDialogItem->dRadius_m, 20, buffer );
            SetDlgItemText( hDlg, IDC_EDIT_AEO_RADIUS, buffer );

            _gcvt( pDialogItem->vPosition.x, 20, buffer );
            SetDlgItemText( hDlg, IDC_EDIT_AEO_POSX, buffer );
            _gcvt( pDialogItem->vPosition.y, 20, buffer );
            SetDlgItemText( hDlg, IDC_EDIT_AEO_POSY, buffer );
            _gcvt( pDialogItem->vPosition.z, 20, buffer );
            SetDlgItemText( hDlg, IDC_EDIT_AEO_POSZ, buffer );

            _gcvt( pDialogItem->vVelocity.x, 20, buffer );
            SetDlgItemText( hDlg, IDC_EDIT_AEO_VELX, buffer );
            _gcvt( pDialogItem->vVelocity.y, 20, buffer );
            SetDlgItemText( hDlg, IDC_EDIT_AEO_VELY, buffer );
            _gcvt( pDialogItem->vVelocity.z, 20, buffer );
            SetDlgItemText( hDlg, IDC_EDIT_AEO_VELZ, buffer );

            _gcvt( pDialogItem->sColor.r, 20, buffer );
            SetDlgItemText( hDlg, IDC_EDIT_AEO_COLORR, buffer );
            _gcvt( pDialogItem->sColor.g, 20, buffer );
            SetDlgItemText( hDlg, IDC_EDIT_AEO_COLORG, buffer );
            _gcvt( pDialogItem->sColor.b, 20, buffer );
            SetDlgItemText( hDlg, IDC_EDIT_AEO_COLORB, buffer );

            SetDlgItemText( hDlg, IDC_EDIT_AEO_TEXTURE, pDialogItem->stTextureName.c_str( ) );

            CheckDlgButton( hDlg, IDC_CHECK_AOE_LIGHT, pDialogItem->IsLight( ) );
        }
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if ( ( LOWORD( wParam ) == IDOK ) || 
             ( LOWORD( wParam ) == IDCANCEL ) )
		{
			EndDialog( hDlg, LOWORD( wParam ) );
			return (INT_PTR)TRUE;
		}
        else if ( LOWORD( wParam ) == IDC_BUTTON_AEO_TEXTURE_BROWSE )
        {
            OPENFILENAME ofn;
            char szFile[260];

            ZeroMemory( &ofn, sizeof( ofn ) );
            ofn.lStructSize = sizeof( ofn );
            ofn.hwndOwner = hDlg;
            ofn.lpstrFile = szFile;
            ofn.lpstrFile[0] = '\0';
            ofn.nMaxFile = sizeof( szFile );
            ofn.lpstrFilter = "All\0*.*\0JPEG (*.jpg)\0*.JPG\0TGA (*.tga)\0*.TGA\0BMP (*.bmp)\0*.BMP\0";
            ofn.nFilterIndex = 2;
            ofn.lpstrFileTitle = NULL;
            ofn.nMaxFileTitle = 0;
            ofn.lpstrInitialDir = NULL;
            ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

            if ( GetOpenFileName( &ofn ) )
            {
                SetDlgItemText( hDlg, IDC_EDIT_AEO_TEXTURE, ofn.lpstrFile );
            }
        }

		break;
	}
	return (INT_PTR)FALSE;
}

