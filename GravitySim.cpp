
#include "GravitySim.h"
#include "CGravity.h"
#include <Windows.h>
#include <commdlg.h>

#define MAX_LOADSTRING 100

CGravity GravitySim;

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int, HWND *);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);


int APIENTRY _tWinMain( HINSTANCE hInstance,
                        HINSTANCE hPrevInstance,
                        LPTSTR    lpCmdLine,
                        int       nCmdShow )
{
	UNREFERENCED_PARAMETER( hPrevInstance );
	UNREFERENCED_PARAMETER( lpCmdLine );

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;
	HWND hWnd;
	bool quit = false;

	// Initialize global strings
	LoadString( hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING );
	LoadString( hInstance, IDC_GRAVITY_OPENGL, szWindowClass, MAX_LOADSTRING );
	MyRegisterClass( hInstance );

	// Perform application initialization:
	if ( !InitInstance( hInstance, nCmdShow, &hWnd ) )
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators( hInstance, MAKEINTRESOURCE( IDC_GRAVITY_OPENGL ) );

    GravitySim.Initialize( hWnd, hInstance );

	// Main message loop:
	while ( !quit )
	{
		// check for messages
		if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE )  )
		{
			// handle or dispatch messages
			if ( msg.message == WM_QUIT ) 
			{
				quit = TRUE;
			} 
			else if ( !TranslateAccelerator( msg.hwnd, hAccelTable, &msg ) )
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
		} 
		else 
		{
            GravitySim.DoLoop( );
		}
	}

    GravitySim.Release( );

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass( HINSTANCE hInstance )
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_OWNDC | CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE( IDI_GRAVITY_OPENGL ) );
	wcex.hCursor		= LoadCursor( NULL, IDC_ARROW );
	wcex.hbrBackground	= (HBRUSH)GetStockObject( BLACK_BRUSH );
	wcex.lpszMenuName	= MAKEINTRESOURCE( IDC_GRAVITY_OPENGL );
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon( wcex.hInstance, MAKEINTRESOURCE( IDI_SMALL ) );

	return RegisterClassEx( &wcex );
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance( HINSTANCE hInstance, int nCmdShow, HWND * hWnd )
{
   hInst = hInstance; // Store instance handle in our global variable

   *hWnd = CreateWindow( szWindowClass, 
	 					 szTitle, 
						 WS_OVERLAPPEDWINDOW,
						 CW_USEDEFAULT, 
						 0, 
						 CW_USEDEFAULT, //609
						 0, //653 
						 NULL, 
						 NULL, 
						 hInstance, 
						 NULL );

   if ( !*hWnd )
   {
      return FALSE;
   }

   ShowWindow( *hWnd, nCmdShow );
   UpdateWindow( *hWnd );

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	int wmId, wmEvent;
	//PAINTSTRUCT ps;
	//HDC hdc;

	switch ( message )
	{
	case WM_COMMAND:
		wmId    = LOWORD( wParam );
		wmEvent = HIWORD( wParam );
		// Parse the menu selections:
		switch ( wmId )
		{
		case IDM_ABOUT:
			DialogBox( hInst, MAKEINTRESOURCE( IDD_ABOUTBOX ), hWnd, About );
			break;

		case IDM_EXIT:
            //GravitySim.Stop( );
			DestroyWindow( hWnd );
			break;

        case IDC_OPEN_FILE:
            {
                OPENFILENAME ofn;       // common dialog box structure
                char szFile[260];       // buffer for file name

                // Initialize OPENFILENAME
                ZeroMemory( &ofn, sizeof( ofn ) );
                ofn.lStructSize = sizeof( ofn );
                ofn.hwndOwner = hWnd;
                ofn.lpstrFile = szFile;
                //
                // Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
                // use the contents of szFile to initialize itself.
                //
                ofn.lpstrFile[0] = '\0';
                ofn.nMaxFile = sizeof( szFile );
                ofn.lpstrFilter = "All\0*.*\0Gravity Simulator File\0*.GSI\0";
                ofn.nFilterIndex = 2;
                ofn.lpstrFileTitle = NULL;
                ofn.nMaxFileTitle = 0;
                ofn.lpstrInitialDir = NULL;
                ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

                if ( GetOpenFileName( &ofn ) )
                {
                    GravitySim.OpenFile( ofn.lpstrFile );
                }
            }
            break;

        case ID_INTEGRATOR_OLD:
            {
                HMENU hmenu = GetMenu( hWnd ); 
                CheckMenuItem( hmenu, ID_INTEGRATOR_NEW, MF_UNCHECKED | MF_BYCOMMAND );
                CheckMenuItem( hmenu, ID_INTEGRATOR_OLD, MF_CHECKED | MF_BYCOMMAND );
                GravitySim.ChangeIntegrator( 1 );
            }
            break;

        case ID_INTEGRATOR_NEW:
            {
                HMENU hmenu = GetMenu( hWnd ); 
                CheckMenuItem( hmenu, ID_INTEGRATOR_NEW, MF_CHECKED | MF_BYCOMMAND );
                CheckMenuItem( hmenu, ID_INTEGRATOR_OLD, MF_UNCHECKED | MF_BYCOMMAND );
                GravitySim.ChangeIntegrator( 2 );
            }
            break;

        case ID_GRAVITYFIELD_2D_COLORS:
            {
                HMENU hmenu = GetMenu( hWnd ); 
                CheckMenuItem( hmenu, ID_GRAVITYFIELD_2D_COLORS, MF_CHECKED | MF_BYCOMMAND );
                CheckMenuItem( hmenu, ID_GRAVITYFIELD_2D_DEPTH, MF_UNCHECKED | MF_BYCOMMAND );
                CheckMenuItem( hmenu, ID_GRAVITYFIELD_2D_VECTORS, MF_UNCHECKED | MF_BYCOMMAND );
                CheckMenuItem( hmenu, ID_GRAVITYFIELD_3D_COLORS, MF_UNCHECKED | MF_BYCOMMAND );
                GravitySim.ChangeGravityFieldType( CSimulator::TYPE_2D_COLORS );
            }
            break;

        case ID_GRAVITYFIELD_2D_DEPTH:
            {
                HMENU hmenu = GetMenu( hWnd ); 
                CheckMenuItem( hmenu, ID_GRAVITYFIELD_2D_COLORS, MF_UNCHECKED | MF_BYCOMMAND );
                CheckMenuItem( hmenu, ID_GRAVITYFIELD_2D_DEPTH, MF_CHECKED | MF_BYCOMMAND );
                CheckMenuItem( hmenu, ID_GRAVITYFIELD_2D_VECTORS, MF_UNCHECKED | MF_BYCOMMAND );
                CheckMenuItem( hmenu, ID_GRAVITYFIELD_3D_COLORS, MF_UNCHECKED | MF_BYCOMMAND );
                GravitySim.ChangeGravityFieldType( CSimulator::TYPE_2D_DEPTH );
            }
            break;

        case ID_GRAVITYFIELD_2D_VECTORS:
            {
                HMENU hmenu = GetMenu( hWnd ); 
                CheckMenuItem( hmenu, ID_GRAVITYFIELD_2D_COLORS, MF_UNCHECKED | MF_BYCOMMAND );
                CheckMenuItem( hmenu, ID_GRAVITYFIELD_2D_DEPTH, MF_UNCHECKED | MF_BYCOMMAND );
                CheckMenuItem( hmenu, ID_GRAVITYFIELD_2D_VECTORS, MF_CHECKED | MF_BYCOMMAND );
                CheckMenuItem( hmenu, ID_GRAVITYFIELD_3D_COLORS, MF_UNCHECKED | MF_BYCOMMAND );
                GravitySim.ChangeGravityFieldType( CSimulator::TYPE_2D_VECTOR );
            }
            break;

        case ID_GRAVITYFIELD_3D_COLORS:
            {
                HMENU hmenu = GetMenu( hWnd ); 
                CheckMenuItem( hmenu, ID_GRAVITYFIELD_2D_COLORS, MF_UNCHECKED | MF_BYCOMMAND );
                CheckMenuItem( hmenu, ID_GRAVITYFIELD_2D_DEPTH, MF_UNCHECKED | MF_BYCOMMAND );
                CheckMenuItem( hmenu, ID_GRAVITYFIELD_2D_VECTORS, MF_UNCHECKED | MF_BYCOMMAND );
                CheckMenuItem( hmenu, ID_GRAVITYFIELD_3D_COLORS, MF_CHECKED | MF_BYCOMMAND );
                GravitySim.ChangeGravityFieldType( CSimulator::TYPE_3D_COLORS );
            }
            break;

		default:
			return DefWindowProc( hWnd, message, wParam, lParam );
		}
		break;

	//case WM_PAINT:
		//hdc = BeginPaint( hWnd, &ps );
		// TODO: Add any drawing code here...
		//EndPaint( hWnd, &ps );
		//break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
        if ( ! GravitySim.ProcessWin32Msg( hWnd, message, wParam, lParam ) )
        {
            return DefWindowProc( hWnd, message, wParam, lParam );
        }
        break;
	}

	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
	UNREFERENCED_PARAMETER( lParam );
	switch ( message )
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

// Calculates the frequency of the processor clock
#pragma optimize( "", off )
double ClockFrequency( void )
{
    __int64 i64_perf_start, 
            i64_perf_freq, 
            i64_perf_end,
            i64_clock_start,
            i64_clock_end;
    double  d_loop_period, 
            d_clock_freq;

    HANDLE hThread = ::GetCurrentThread( );
    int nPriority = ::GetThreadPriority( hThread );
    ::SetThreadPriority( hThread, THREAD_PRIORITY_TIME_CRITICAL );

    QueryPerformanceFrequency( (LARGE_INTEGER*)&i64_perf_freq );

    QueryPerformanceCounter( (LARGE_INTEGER*)&i64_perf_start );
    i64_perf_end = 0;

    i64_clock_start = __rdtsc();
    while( i64_perf_end < ( i64_perf_start + 550000 ) )
    {
        QueryPerformanceCounter( (LARGE_INTEGER*)&i64_perf_end );
    }
    i64_clock_end = __rdtsc();

    ::SetThreadPriority( hThread, nPriority );

    i64_clock_end -= i64_clock_start;

    d_loop_period = ((double)i64_perf_freq) / 550000.0;
    d_clock_freq = ((double)( i64_clock_end & 0xffffffff )) * d_loop_period;

    return d_clock_freq;
}

#pragma optimize( "", on )
