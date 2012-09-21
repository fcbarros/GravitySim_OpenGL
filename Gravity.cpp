
#include "stdafx.h"
#include "MassObject.h"
#include "BitmapFont.h"

const double dConstG = 6.67428e-11; // N m2 kg-2

std::vector<CMassObject> objMassObjects;

CBitmapFont * fCourier = NULL;

double dHighPos;
double dLowPos;
//double dPosScaler;
double dTimeConstant;

double dWidth;
double dHeight;
double dHalfSize;
double dAspectRatio;

double dSenT;
double dCosT;

PRECISIONVECTOR vCamPos( 0.0, 0.0, 0.0 );
PRECISIONVECTOR vCamLookAt( 0.0, 0.0, 0.0 );


void InitOpenGLWindow( void );

void SetPos( double x, double y, double z )
{
	vCamPos.x += x * vCamPos.z;
	vCamPos.y += y * vCamPos.z;
	vCamPos.z += z * vCamPos.z;

	vCamLookAt.x += x * vCamPos.z;
	vCamLookAt.y += y * vCamPos.z;
	vCamLookAt.z += z * vCamPos.z;

	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}

void MoveCam( double scale )
{
	PRECISIONVECTOR temp = vCamPos - vCamLookAt;

	temp.Normalize( );

	temp *= scale * vCamPos.z;
	vCamPos += temp;

	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}

PRECISIONVECTOR rotate( PRECISIONVECTOR &vec, double alf, double bet, double gam )
{
	PRECISIONVECTOR temp;

	double calf = cos( alf * 3.141592654 / 180.0 );
	double salf = sin( alf * 3.141592654 / 180.0 );
	double cbet = cos( bet * 3.141592654 / 180.0 );
	double sbet = sin( bet * 3.141592654 / 180.0 );
	double cgam = cos( gam * 3.141592654 / 180.0 );
	double sgam = sin( gam * 3.141592654 / 180.0 );

	double v1 = calf * vec.x + salf * vec.z;
	double v2 = calf * vec.z - salf * vec.x;
	double v3 = cbet * vec.y + sbet * v2;

	temp.z = cbet * v2 - sbet * vec.y;
	temp.x = cgam * v1 + sgam * v3;
	temp.y = cgam * v3 - sgam * v1;

	return temp;
}

void SetAngleY( double angle1, double angle2 )
{
	dSenT = angle1;
	dCosT = angle2;

	PRECISIONVECTOR temp = vCamPos - vCamLookAt;

	temp = rotate( temp, angle1, angle2, 0.0 );

	temp += vCamLookAt;

	vCamPos = temp;

	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}

void FindHighLowPos( void )
{
	dHighPos = 0.0;
	dLowPos = 0.0;

	for ( unsigned int i = 0; i < objMassObjects.size( ); i++ )
	{
		if ( objMassObjects[ i ].vPosition.x > dHighPos )
		{
			dHighPos = objMassObjects[ i ].vPosition.x;
		}
		if ( objMassObjects[ i ].vPosition.y > dHighPos )
		{
			dHighPos = objMassObjects[ i ].vPosition.y;
		}
		if ( objMassObjects[ i ].vPosition.z > dHighPos )
		{
			dHighPos = objMassObjects[ i ].vPosition.z;
		}

		if ( objMassObjects[ i ].vPosition.x < dLowPos )
		{
			dLowPos = objMassObjects[ i ].vPosition.x;
		}
		if ( objMassObjects[ i ].vPosition.y < dLowPos )
		{
			dLowPos = objMassObjects[ i ].vPosition.y;
		}
		if ( objMassObjects[ i ].vPosition.z < dLowPos )
		{
			dLowPos = objMassObjects[ i ].vPosition.z;
		}
	}

	//dPosScaler = dHighPos - dLowPos;
}

bool RemoveComments( FILE * fRead, char * cBuffer, int iBufSize )
{
	while ( fgets( cBuffer, iBufSize, fRead ) != NULL )
	{
		if ( strncmp( cBuffer, "//", 2 ) != 0 )
		{
			return true;
		}
	}

	return false;
}

bool ReadObjectsFile( const char* fileName )
{
	FILE * fRead;
	char cBuff[ 256 ];
	int numObjects;

	errno_t error = fopen_s( &fRead, fileName, "r" );

	if ( RemoveComments( fRead, cBuff, sizeof( cBuff ) ) )
	{
		numObjects = atoi( cBuff );
	}
	else
	{
		return false;
	}

	for ( int i = 0; i < numObjects; i++ )
	{
		char stX[256], stY[256], stZ[256];
		CMassObject mObject;

		if ( ! RemoveComments( fRead, cBuff, sizeof( cBuff ) ) )
		{
			fclose( fRead );
			return false;
		}
		mObject.dMass_Kg = atof( cBuff );

		if ( ! RemoveComments( fRead, cBuff, sizeof( cBuff ) ) )
		{
			fclose( fRead );
			return false;
		}
		mObject.dRadius_m = atof( cBuff );

		if ( ! RemoveComments( fRead, cBuff, sizeof( cBuff ) ) )
		{
			fclose( fRead );
			return false;
		}
		sscanf_s( cBuff, "%s %s %s", stX, sizeof(stX), stY, sizeof(stY), stZ, sizeof(stZ) );
		mObject.vPosition.x = atof( stX );
		mObject.vPosition.y = atof( stY );
		mObject.vPosition.z = atof( stZ );

		if ( ! RemoveComments( fRead, cBuff, sizeof( cBuff ) ) )
		{
			fclose( fRead );
			return false;
		}
		sscanf_s( cBuff, "%s %s %s", stX, sizeof(stX), stY, sizeof(stY), stZ, sizeof(stZ) );
		mObject.vVelocity.x = atof( stX );
		mObject.vVelocity.y = atof( stY );
		mObject.vVelocity.z = atof( stZ );

		if ( ! RemoveComments( fRead, cBuff, sizeof( cBuff ) ) )
		{
			fclose( fRead );
			return false;
		}
		sscanf_s( cBuff, "%s %s %s", stX, sizeof(stX), stY, sizeof(stY), stZ, sizeof(stZ) );
		mObject.sColor.r = (float)atof( stX );
		mObject.sColor.g = (float)atof( stY );
		mObject.sColor.b = (float)atof( stZ );
		mObject.sColor.a = 1.0f;

		mObject.dMassG = mObject.dMass_Kg * dConstG;

		objMassObjects.push_back( mObject );
	}

	FindHighLowPos( );

	vCamPos.x = 0.0;
	vCamPos.y = 0.0;
	vCamPos.z = dHighPos - dLowPos;

	fclose( fRead );

	return true;
}

void Reset( void )
{
    objMassObjects.clear( );

	dTimeConstant = 1000.0;
	dWidth = 600.0;
	dHeight = 600.0;

    vCamPos = PRECISIONVECTOR( 0.0, 0.0, -1.0 );
    vCamLookAt = PRECISIONVECTOR( 0.0, 0.0, 0.0 );

	InitOpenGLWindow( );
}

void Init( HDC hDC )
{
    Reset( );

	if ( fCourier )
	{
		delete fCourier;
        fCourier = NULL;
	}
    fCourier = new CBitmapFont( CDC::FromHandle( hDC ), "Courier" );
    if ( !fCourier )
	{
        // Mostra Erro
        return;
    }
}

void LoadFile( const char * stzFileName )
{
    Reset( );

    ReadObjectsFile( stzFileName );
}

void DrawInterface( void )
{
	char myText[ 4096 ];

    glMatrixMode( GL_PROJECTION );

	glPushMatrix( );

    glLoadIdentity( );
    if ( dWidth <= dHeight )
	{
        glOrtho( -dHalfSize, dHalfSize, 
                 -dHalfSize * dAspectRatio, dHalfSize * dAspectRatio, 
                 -1000000.0, 1000000.0 );
    }
    else 
	{
        glOrtho( -dHalfSize * dAspectRatio, dHalfSize * dAspectRatio, 
                 -dHalfSize, dHalfSize, 
                 -1000000.0, 1000000.0 );
    }

    /* Make the world and window coordinates coincide so that 1.0 in */
    /* model space equals one pixel in window space.                 */
    glScaled( dAspectRatio, dAspectRatio, 1.0 );

    glMatrixMode( GL_MODELVIEW );

	glPushMatrix( );

	glLoadIdentity( );
    glColor3f( 1.0f, 1.0f, 1.0f );

	sprintf_s( myText, "SenT: %3.3f   CosT: %3.3f   Angle: %3.3f", dSenT, dCosT, acos( dSenT ) );
	fCourier->DrawStringAt( -( (float)dWidth / 2.0f ) + 10.0f, -( (float)dHeight / 2.0f ) + 50.0f, 0.0f, myText );	

	sprintf_s( myText, "X: %3.3f   Y: %3.3f   Z: %3.3f", vCamPos.x, vCamPos.y, vCamPos.z );
	fCourier->DrawStringAt( -( (float)dWidth / 2.0f ) + 10.0f, -( (float)dHeight / 2.0f ) + 30.0f, 0.0f, myText );	

	sprintf_s( myText, "X: %3.3f   Y: %3.3f   Z: %3.3f", vCamLookAt.x, vCamLookAt.y, vCamLookAt.z );
	fCourier->DrawStringAt( -( (float)dWidth / 2.0f ) + 10.0f, -( (float)dHeight / 2.0f ) + 10.0f, 0.0f, myText );	

	glPopMatrix();

    glMatrixMode( GL_PROJECTION );

	glPopMatrix();

    glMatrixMode( GL_MODELVIEW );
}

void DrawBodies( void )
{
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
	//glClear( GL_COLOR_BUFFER_BIT );

	glLoadIdentity( );
	gluLookAt( vCamPos.x, vCamPos.y, vCamPos.z,
			   vCamLookAt.x, vCamLookAt.y, vCamLookAt.z,
			   0.0f, 1.0f, 0.0f );

	for ( unsigned int i = 0; i < objMassObjects.size( ); i++ )
	{
		objMassObjects[ i ].Draw( );
	}

	DrawInterface( );
}

PRECISIONVECTOR ForceFunctionDebug( CMassObject * obj1, CMassObject * obj2 )
{
	double m1 = obj1->dMass_Kg;
	double m2 = obj2->dMass_Kg;

	PRECISIONVECTOR vDir = obj2->vPosition - obj1->vPosition;
	double r2 = vDir.LengthSquared( );

	double F = dConstG * m1 * m2 / r2; // Kg m / s-2

	vDir.Normalize( );
	vDir *= F / m1;

	return vDir;
}

PRECISIONVECTOR ForceFunction( CMassObject &obj1, CMassObject &obj2 )
{
	PRECISIONVECTOR vDir = obj2.vPosition - obj1.vPosition;

	double F = obj2.dMassG / vDir.LengthSquared( ); // Kg m / s-2

	vDir.Normalize( );
	vDir *= F;

	return vDir;
}

void MoveBodies( void )
{
	for ( unsigned int i = 0; i < objMassObjects.size( ); i++ )
	{
		PRECISIONVECTOR vAcceleration;
		for ( unsigned int j = 0; j < objMassObjects.size( ); j++ )
		{
			if ( i != j )
			{
				vAcceleration += ForceFunction( objMassObjects[ i ], objMassObjects[ j ] );
			}
		}
		objMassObjects[ i ].vVelocity += vAcceleration * dTimeConstant;
		objMassObjects[ i ].vPosition += objMassObjects[ i ].vVelocity * dTimeConstant;
	}
}

void Process( HDC hDC )
{
	MoveBodies( );
	DrawBodies( );

	SwapBuffers( hDC );
}

void InitOpenGLWindow( )
{
    dHalfSize = ( ( dWidth >= dHeight ) ? dWidth : dHeight ) / 2.0;
    dAspectRatio = ( dWidth <= dHeight ) ? ( dHeight / dWidth ) : ( dWidth / dHeight );

	// Set the viewport for the OpenGL window
	glViewport( 0, 0, (GLint)dWidth, (GLint)dHeight );

	// Change Matrix Mode to Projection
	glMatrixMode( GL_PROJECTION );        

	// Reset View
	glLoadIdentity( );                   

	// Do the perspective calculations. Last value = max clipping depth
	gluPerspective( 45.0, dWidth / dHeight, 0.0, 100.0 );  

	// Return to the modelview matrix
	glMatrixMode( GL_MODELVIEW );

	// Reset View
	glLoadIdentity( );

	// Enable Depth testing
	//glEnable( GL_DEPTH_TEST );
	//glDepthFunc( GL_LEQUAL );

	// Use smooth shading
	glShadeModel( GL_SMOOTH );
	// Set the smooth shading to the best we can have
	glHint( GL_SHADE_MODEL, GL_NICEST );   

	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}
