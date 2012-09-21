
#include "CMassObject.h"
#include "Utils/CRender.h"
#include <CFloat>
#include <time.h>

// Static object
int CMassObject::iTotalLightNum = 0;



CMassObject::CMassObject( void ) 
  : stName( "" ), 
    stTextureName( "" ),
    iHistorySize( 0 ), 
    SphereList( 0 ),
    dAngleRot( 0.0 ),
    iLightNum( -1 ),
    m_bShowVel( false ),
    m_bShowAccel( false )
{ 
}

CMassObject::~CMassObject( void ) 
{ 
    Clear( );
}

void CMassObject::Clear( void )
{
    if ( SphereList != 0 )
    {
        glDeleteLists( SphereList, 1 );
        SphereList = 0;
    }

    History.Clear( );

    Texture.Clear( );
}

CMassObject::CMassObject( const CMassObject &mObject )
  : SphereList( 0 ), 
    dAngleRot( 0.0 ),
    stName( mObject.stName ),
    vPosition( mObject.vPosition ),
    vVelocity( mObject.vVelocity ),
    dMass_Kg( mObject.dMass_Kg ),
    dRadius_m( mObject.dRadius_m ),
    sColor( mObject.sColor ),
    dMassG( mObject.dMassG ),
    iHistorySize( mObject.iHistorySize ),
    stTextureName( mObject.stTextureName ),
    iLightNum( mObject.iLightNum ),
    m_bShowVel( mObject.m_bShowVel ),
    m_bShowAccel( mObject.m_bShowAccel )
{
}

void CMassObject::GenerateSphere( void )
{
    SphereList = glGenLists( 1 );

    glNewList( SphereList, GL_COMPILE );
    g_Render.DrawSphere( dRadius_m, 40, 40, false );
    glEndList( );
}

void CMassObject::DrawLight( CCamera & _cam )
{
    if ( ! LightTexture.IsLoaded( ) )
    {
        LightTexture.LoadFromFile( "Images/flare.tga" );
        srand( (unsigned int)time( NULL ) );
    }

    double dRadius;
    if ( _cam.GetPercentOfScreen( vPosition, dRadius_m )  < 0.005 )
    {
        double dMult = ( 0.01 * ( (double)rand( ) / (double)RAND_MAX ) + 0.04 );
        dRadius = _cam.GetSizeForPercent( vPosition, dMult );
    }
    else
    {
        double dMult = ( 1.5 * ( (double)rand( ) / (double)RAND_MAX ) + 9.5 );
        dRadius = dRadius_m * dMult;
    }

    g_Render.TextureEnable( LightTexture );

    g_Render.PushMatrix( );

    g_Render.Translate( vPosition.x, vPosition.y, vPosition.z );
    PRECISIONVECTOR vOrbit = _cam.GetOrbit( );
    g_Render.Rotate( -vOrbit.x, 1.0, 0.0, 0.0 );
    g_Render.Rotate( -vOrbit.y, 0.0, 1.0, 0.0 );

    g_Render.Blend( true );

    g_Render.SetColor( sColor.r, sColor.g, sColor.b, 0.4f ); 

    glBegin( GL_QUADS );

		glTexCoord2f( 0.0f, 1.0f );
		glVertex2d( dRadius, -dRadius );

		glTexCoord2f( 1.0f, 1.0f );
		glVertex2d( dRadius, dRadius );

        glTexCoord2f( 1.0f, 0.0f );
		glVertex2d( -dRadius, dRadius );

		glTexCoord2f( 0.0f, 0.0f );
		glVertex2d( -dRadius, -dRadius );

    glEnd( );

    g_Render.PopMatrix( );

    g_Render.Blend( false );

    g_Render.TextureDisable( );
}

void CMassObject::SetLight( bool _bEnable )
{
    if ( IsLight( ) )
    {
        GLfloat light_position[4] = { (float)vPosition.x, (float)vPosition.y, (float)vPosition.z, 1.0 };
        static GLfloat ambient[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
        //GLfloat diffuse[4] = { sColor.r, sColor.g, sColor.b, 1.0f };
        static GLfloat specular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
        
        glLightfv( GL_LIGHT0 + iLightNum, GL_AMBIENT, ambient );
        glLightfv( GL_LIGHT0 + iLightNum, GL_SPECULAR, specular );
        glLightfv( GL_LIGHT0 + iLightNum, GL_DIFFUSE, sColor.v );
        glLightfv( GL_LIGHT0 + iLightNum, GL_POSITION, light_position );
        //glLightfv( GL_LIGHT0 + iLightNum, GL_DIFFUSE, diffuse );
        //glLightf( GL_LIGHT0 + iLightNum, GL_CONSTANT_ATTENUATION, 1.0 );
        //glLightf( GL_LIGHT0 + iLightNum, GL_LINEAR_ATTENUATION, 0.0 );
        //glLightf( GL_LIGHT0 + iLightNum, GL_QUADRATIC_ATTENUATION, 0.0 );

        glEnable( GL_LIGHT0 + iLightNum );
    }
    else if ( _bEnable && ( iTotalLightNum > 0 ) )
    {
        glEnable( GL_LIGHTING );
    }
}

void CMassObject::Draw( CCamera & _cam, bool _bDrawExtras )
{
    // If the object is larger than 0.5% of the screen draw the sphere
    if ( _cam.GetPercentOfScreen( vPosition, dRadius_m )  > 0.005 )
    {
        static GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
        static GLfloat mat_shininess = 100.0f;

        if ( SphereList == 0 )
        {
            GenerateSphere( );
        }

        glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular );
        glMaterialfv( GL_FRONT, GL_SHININESS, &mat_shininess );

        SetLight( _bDrawExtras );

        g_Render.PushMatrix( );

        g_Render.Translate( vPosition.x, vPosition.y, vPosition.z );

        if ( ! Texture.IsLoaded( ) && ! stTextureName.empty( ) )
        {
            if ( ! Texture.LoadFromFile( stTextureName ) )
            {
                stTextureName = "";
            }
        }

        if ( _bDrawExtras && Texture.IsLoaded( ) )
        {
            g_Render.TextureEnable( Texture );
            g_Render.SetColor( 1.0, 1.0, 1.0, 1.0f ); 
            g_Render.Rotate( dAngleRot, 0.0, 0.0, 1.0 );
            dAngleRot += 0.1;
        }
        else
        {
            g_Render.SetColor( sColor.r, sColor.g, sColor.b, 1.0f );
        }

        glEnable( GL_DEPTH_TEST );

        glCallList( SphereList );

        glDisable( GL_LIGHTING );

        g_Render.TextureDisable( );

        g_Render.PopMatrix( );

        if ( IsLight( ) && _bDrawExtras )
        {
            DrawLight( _cam );
        }

        glDisable( GL_DEPTH_TEST );
    }
    else
    {
        // Enabling AntiAliasing
        g_Render.Blend( true );

        glEnable( GL_POINT_SMOOTH );

        //glEnable( GL_DEPTH_TEST );

        glPointSize( 4.5f );
        
        SetLight( false );

        glBegin( GL_POINTS );

            glColor4fv( sColor.v ); 
		    glVertex3dv( vPosition.v );

	    glEnd();

        if ( IsLight( ) && _bDrawExtras )
        {
            DrawLight( _cam );
        }

        glDisable( GL_POINT_SMOOTH );

        g_Render.Blend( false );
        //glDisable( GL_DEPTH_TEST );
    }
    double dSize = _cam.GetSizeForPercent( vPosition, 0.07 );
    if ( m_bShowVel )
    {
        PRECISIONVECTOR vTemp = vVelocity;
        vTemp.Normalize( );
        vTemp *= dSize;
        vTemp += vPosition;
        g_Render.DrawArrow( vPosition, vTemp );
    }

    if ( m_bShowAccel )
    {
        PRECISIONVECTOR vTemp = vAcceleration;
        vTemp.Normalize( );
        vTemp *= dSize;
        vTemp += vPosition;
        g_Render.DrawArrow( vPosition, vTemp );
    }


    if ( _bDrawExtras && ( iHistorySize > 0 ) )
    {
        g_Render.Blend( true );

        glEnable( GL_LINE_SMOOTH );
        glLineWidth( 1.5f );

        //glEnable( GL_DEPTH_TEST );

        glBegin( GL_LINE_STRIP );

            glColor4fv( sColor.v ); 

            // Going from the current position till the fisrt history
	        glVertex3d( vPosition.x, vPosition.y, vPosition.z );
            for ( int i = History.GetNumItems( ) - 1; i >= 0 ; --i )
            {
		        glVertex3dv( History[ i ].v );
            }

	    glEnd( );

        glLineWidth( 1.0f );
        glDisable( GL_LINE_SMOOTH );

        g_Render.Blend( false );
        //glDisable( GL_DEPTH_TEST );
    }
}

void CMassObject::SaveHistory( void )
{
    if ( iHistorySize > 0 )
    {
        if ( History.GetSize( ) != iHistorySize )
        {
            History.SetSize( iHistorySize );
        }
        History.Add( vPosition );
    }
}

bool CMassObject::IsLight( void ) const
{
    return iLightNum >= 0;
}

// Not precise
double CubicRoot( double _dNum )
{
    double dTemp1 = sqrt( sqrt( _dNum ) );
    dTemp1 *= sqrt( sqrt( dTemp1 ) );
    dTemp1 *= sqrt( sqrt( sqrt( sqrt( dTemp1 ) ) ) );
    dTemp1 *= sqrt( sqrt( sqrt( sqrt( sqrt( sqrt( sqrt( sqrt( dTemp1 ) ) ) ) ) ) ) );

    return dTemp1 ;
}

bool CMassObject::Load( CFileLoader & _fl )
{
    stName = _fl.GetVariableValue( "name" );
    dMass_Kg = atof( _fl.GetVariableValue( "mass" ).c_str( ) );
    dRadius_m = atof( _fl.GetVariableValue( "radius" ).c_str( ) );
    _fl.SetCurrentElement( "position" );
    vPosition.x = atof( _fl.GetVariableValue( "x" ).c_str( ) );
    vPosition.y = atof( _fl.GetVariableValue( "y" ).c_str( ) );
    vPosition.z = atof( _fl.GetVariableValue( "z" ).c_str( ) );
    _fl.Back( );
    _fl.SetCurrentElement( "velocity" );
    vVelocity.x = atof( _fl.GetVariableValue( "x" ).c_str( ) );
    vVelocity.y = atof( _fl.GetVariableValue( "y" ).c_str( ) );
    vVelocity.z = atof( _fl.GetVariableValue( "z" ).c_str( ) );
    _fl.Back( );
    _fl.SetCurrentElement( "color" );
    sColor.r = (float)atof( _fl.GetVariableValue( "r" ).c_str( ) );
    sColor.g = (float)atof( _fl.GetVariableValue( "g" ).c_str( ) );
    sColor.b = (float)atof( _fl.GetVariableValue( "b" ).c_str( ) );
    sColor.a = 1.0;
    _fl.Back( );
    iHistorySize = atoi( _fl.GetVariableValue( "history_size" ).c_str( ) );
    stTextureName = _fl.GetVariableValue( "texture" );
    dMassG = dConstG * dMass_Kg;
    if ( _fl.GetVariableValue( "light" ) != "" )
    {
        iLightNum = iTotalLightNum++;
    }
    return true;
}

void CMassObject::Join( const CMassObject & _obj )
{
    // Position/Velocity = Mean by mass
    vPosition = ( ( vPosition * dMassG ) + ( _obj.vPosition * _obj.dMassG ) ) / ( dMassG + _obj.dMassG );
    vVelocity = ( ( vVelocity * dMassG ) + ( _obj.vVelocity * _obj.dMassG ) ) / ( dMassG + _obj.dMassG );

    // Radius = ( R1^3 + R2^3 )^1/3
    dRadius_m = CubicRoot( ( dRadius_m * dRadius_m * dRadius_m ) + 
                           ( _obj.dRadius_m * _obj.dRadius_m * _obj.dRadius_m ) );

    //dMaxDist = ( dMaxDist + _obj.dMaxDist ) / 2.0;

    stName += " + " + _obj.stName;
    sColor = ( dMass_Kg > _obj.dMass_Kg ) ? sColor : _obj.sColor;
    iHistorySize = iHistorySize > _obj.iHistorySize ? iHistorySize : _obj.iHistorySize;
    stTextureName = ( dMass_Kg > _obj.dMass_Kg ) ? stTextureName : _obj.stTextureName;

    // Mass must be updated last because we use it above
    // Mass = Mass1 + Mass2
    dMass_Kg += _obj.dMass_Kg;

    dMassG = dMass_Kg * dConstG;

    if ( !IsLight( ) && _obj.IsLight( ) )
    {
        iLightNum = _obj.iLightNum;
    }

    Clear( );
}

string CMassObject::GetName( void )
{
    return stName;
}

int CMassObject::GetTotalLights( void )
{
    return iTotalLightNum;
}

double CMassObject::CheckCollisionSimple( CMassObject & _obj )
{
    PRECISIONVECTOR p = vPosition - _obj.vPosition;
    PRECISIONVECTOR v = vVelocity - _obj.vVelocity;
    double d = dRadius_m + _obj.dRadius_m;

    double a = v.LengthSquared( );
    double b = 2.0 * PRECISIONVECTOR::DotProd( p, v );
    double c = p.LengthSquared( ) - d * d;
    double delta = b * b - 4 * a * c;

    if ( delta >= 0 )
    {
        double t2 = ( -b - sqrt( delta ) ) / ( 2.0 * a );

        return t2 > 0 ? t2 : 0.0;
    }

    return 0.0;
}

void CMassObject::ShowVelocityVector( bool _bShow )
{
    m_bShowVel = _bShow;
}

void CMassObject::ShowAccelerationVector( bool _bShow )
{
    m_bShowAccel = _bShow;
}

bool CMassObject::IsShowVelocityVector( void )
{
    return m_bShowVel;
}

bool CMassObject::IsShowAccelerationVector( void )
{
    return m_bShowAccel;
}
