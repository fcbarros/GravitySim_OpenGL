
#include "GeneralIncludes.h"
#include "TVector3D.h"
#include "CMatrix.h"


/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/*
/*
class PRECISIONVECTOR
{
public:
	PRECISIONVECTOR( void );
	PRECISIONVECTOR( const PRECISIONVECTOR & _v );
	PRECISIONVECTOR( const double _x, const double _y, const double _z );

    PRECISIONVECTOR & operator=( const PRECISIONVECTOR & _v1 );

    bool operator!=( const PRECISIONVECTOR & _v1 ) const;

    bool operator==( const PRECISIONVECTOR & _v1 ) const;

    inline PRECISIONVECTOR & operator+=( const PRECISIONVECTOR & _v1 );

	inline PRECISIONVECTOR & operator-=( const PRECISIONVECTOR & _v1 );

    inline PRECISIONVECTOR & operator*=( const double _dScale );

    inline PRECISIONVECTOR & operator/=( const double _dScale );

    const PRECISIONVECTOR operator-( const PRECISIONVECTOR & _v1 ) const;

	const PRECISIONVECTOR operator+( const PRECISIONVECTOR & _v1 ) const;

	const PRECISIONVECTOR operator*( const double _dScale ) const;

	const PRECISIONVECTOR operator/( const double _dScale ) const;

    inline double DotP( const PRECISIONVECTOR & _v1 ) const;

	inline double LengthSquared( void ) const;

    inline double Length( void ) const;

	void Normalize( void );

    static double DotProd( const PRECISIONVECTOR & _v1, const PRECISIONVECTOR & _v2 );

    void Rotate( double _dAngleX, double _dAngleY, double _dAngleZ );

    void Clear( void );

public:

	double x;
	double y;
	double z;

private:

    // This will be the number of divisions an angle will have in the look-up table
    static const int iTableIndexMultipler = 100;
    // Size of the tables below
    static const int iSinCosTableSize = 360 * iTableIndexMultipler;

    inline double CosTable( int _idx );
    inline double SinTable( int _idx );
};


PRECISIONVECTOR::PRECISIONVECTOR( void ) 
: x( 0.0 ), y( 0.0 ), z( 0.0 ) 
{
}

PRECISIONVECTOR::PRECISIONVECTOR( const PRECISIONVECTOR & _v ) 
: x( _v.x ), y( _v.y ), z( _v.z )
{ 
}

PRECISIONVECTOR::PRECISIONVECTOR( const double _x, const double _y, const double _z ) 
: x( _x ), y( _y ), z( _z ) 
{ 
}

PRECISIONVECTOR & PRECISIONVECTOR::operator=( const PRECISIONVECTOR & _v1 )
{
    x = _v1.x;
    y = _v1.y;
    z = _v1.z;

    return *this;
}

PRECISIONVECTOR & PRECISIONVECTOR::operator+=( const PRECISIONVECTOR & _v1 )
{
	x += _v1.x;
	y += _v1.y;
	z += _v1.z;

	return *this;
}

PRECISIONVECTOR & PRECISIONVECTOR::operator-=( const PRECISIONVECTOR & _v1 )
{
	x -= _v1.x;
	y -= _v1.y;
	z -= _v1.z;

	return *this;
}

PRECISIONVECTOR & PRECISIONVECTOR::operator*=( const double _dScale )
{
	x *= _dScale;
	y *= _dScale;
	z *= _dScale;

	return *this;
}

PRECISIONVECTOR & PRECISIONVECTOR::operator/=( const double _dScale )
{
	x /= _dScale;
	y /= _dScale;
	z /= _dScale;

	return *this;
}

const PRECISIONVECTOR PRECISIONVECTOR::operator-( const PRECISIONVECTOR & _v1 ) const
{
    return PRECISIONVECTOR( *this ) -= _v1;
}

const PRECISIONVECTOR PRECISIONVECTOR::operator+( const PRECISIONVECTOR & _v1 ) const
{
    return PRECISIONVECTOR( *this ) += _v1;
}

const PRECISIONVECTOR PRECISIONVECTOR::operator*( const double _dScale ) const
{
    return PRECISIONVECTOR( *this ) *= _dScale;
}

const PRECISIONVECTOR PRECISIONVECTOR::operator/( const double _dScale ) const
{
    return PRECISIONVECTOR( *this ) /= _dScale;
}

bool PRECISIONVECTOR::operator==( const PRECISIONVECTOR & _v1 ) const
{
    return ( x == _v1.x ) && ( y == _v1.y ) && ( z == _v1.z );
}

bool PRECISIONVECTOR::operator!=( const PRECISIONVECTOR & _v1 ) const
{
    return ( x != _v1.x ) || ( y != _v1.y ) || ( z != _v1.z );
}

double PRECISIONVECTOR::LengthSquared( void ) const
{
	return x * x + y * y + z * z;
}

double PRECISIONVECTOR::Length( void ) const
{
    return sqrt( LengthSquared( ) );
}

void PRECISIONVECTOR::Normalize( void )
{
	double dOneOverSize = 1.0 / Length( );

	x *= dOneOverSize;
	y *= dOneOverSize;
	z *= dOneOverSize;
}

void PRECISIONVECTOR::Clear( void )
{
    x = y = z = 0.0;
}

double PRECISIONVECTOR::DotP( const PRECISIONVECTOR & _v1 ) const
{
    return ( x * _v1.x ) + ( y * _v1.y ) + ( z * _v1.z );
}

// this function rotates an vector relative to it's own local coordinate system
// and allows simultaneous rotations
void PRECISIONVECTOR::Rotate( double _dAngleX, double _dAngleY, double _dAngleZ )
{
    int product = 0;   // used to determine which matrices need multiplying

    double temp_x, // used to hold intermediate results during rotation
           temp_y,
           temp_z;

	int iAngleX;
	int iAngleY;
	int iAngleZ;

    // figure out which axes are active
    if ( _dAngleX != 0.0 )
    {
		product += 4;

		if ( _dAngleX < 0.0 )
		{
			_dAngleX += 360.0;
		}

		iAngleX = (int)(_dAngleX * (double)iTableIndexMultipler);
    }

    if ( _dAngleY != 0.0 )
    {
		product += 2;

		if ( _dAngleY < 0.0 )
		{
			_dAngleY += 360.0;
		}

		iAngleY = (int)(_dAngleY * (double)iTableIndexMultipler);
    }

    if ( _dAngleZ != 0.0 )
    {
		product += 1;

		if ( _dAngleZ < 0.0 )
		{
			_dAngleZ += 360.0;
		}

		iAngleZ = (int)(_dAngleZ * (double)iTableIndexMultipler);
    }

    // compute final rotation matrix and perform rotation all in one!
    switch ( product )
    {
    case 1: // final matrix = z
        temp_x = x * CosTable( iAngleZ ) + y * (-SinTable( iAngleZ ));
        temp_y = x * SinTable( iAngleZ ) + y *   CosTable( iAngleZ );

        x = temp_x;
        y = temp_y;
        break;

    case 2: // final matrix = y
        temp_x = x *   CosTable( iAngleY )  + z * SinTable( iAngleY );
        temp_z = x * (-SinTable( iAngleY )) + z * CosTable( iAngleY );

        x = temp_x;
        z = temp_z;
        break;

    case 3: // final matrix = y * z
        temp_x = x * CosTable( iAngleY ) * CosTable( iAngleZ ) +
                 y * (-SinTable( iAngleZ )) +
                 z * SinTable( iAngleY ) * CosTable( iAngleZ );
        temp_y = x * CosTable( iAngleY ) * SinTable( iAngleZ ) +
                 y * CosTable( iAngleZ ) +
                 z * SinTable( iAngleY ) * SinTable( iAngleZ );
        temp_z = x * (-SinTable( iAngleY )) +
                 z * CosTable( iAngleY );

        x = temp_x;
        y = temp_y;
        z = temp_z;
        break;

    case 4: // final matrix = x
        temp_y = y * CosTable( iAngleX ) + z * (-SinTable( iAngleX ));
        temp_z = y * SinTable( iAngleX ) + z *   CosTable( iAngleX );

        y = temp_y;
        z = temp_z;
        break;

    case 5: // final matrix = x * z
        temp_x = x * CosTable( iAngleZ ) +
                 y * (-CosTable( iAngleX ) * SinTable( iAngleZ )) +
                 z * SinTable( iAngleX ) * SinTable( iAngleZ );
        temp_y = x * SinTable( iAngleZ ) +
                 y * CosTable( iAngleX ) * CosTable( iAngleZ ) +
                 z * (-SinTable( iAngleX ) *CosTable( iAngleZ ));
        temp_z = y * SinTable( iAngleX ) +
                 z * CosTable( iAngleX );

        x = temp_x;
        y = temp_y;
        z = temp_z;
        break;

    case 6: // final matrix = x * y
        temp_x = x * CosTable( iAngleY ) +
                 y * SinTable( iAngleX ) * SinTable( iAngleY ) +
                 z * CosTable( iAngleX ) * SinTable( iAngleY );
        temp_y = y * CosTable( iAngleX ) +
                 z * (-SinTable( iAngleX ));
        temp_z = x * (-SinTable( iAngleY )) +
                 y * SinTable( iAngleX ) * CosTable( iAngleY ) +
                 z * CosTable( iAngleX ) * CosTable( iAngleY );

        x = temp_x;
        y = temp_y;
        z = temp_z;
        break;

    case 7: // final matrix = x*y*z, do it the hard way
        {
            CMatrix4x4 rotate,      // the final rotation matrix
                       rotate_x,    // the x,y and z rotation matrices
                       rotate_y,
                       rotate_z,
                       temp;        // temporary working matrix

            rotate_x.data[1][1] = ( CosTable( iAngleX ) );
            rotate_x.data[1][2] = ( SinTable( iAngleX ) );
            rotate_x.data[2][1] = (-SinTable( iAngleX ) );
            rotate_x.data[2][2] = ( CosTable( iAngleX ) );

            rotate_y.data[0][0] = ( CosTable( iAngleY ) );
            rotate_y.data[0][2] = (-SinTable( iAngleY ) );
            rotate_y.data[2][0] = ( SinTable( iAngleY ) );
            rotate_y.data[2][2] = ( CosTable( iAngleY ) );

            rotate_z.data[0][0] = ( CosTable( iAngleZ ) );
            rotate_z.data[0][1] = ( SinTable( iAngleZ ) );
            rotate_z.data[1][0] = (-SinTable( iAngleZ ) );
            rotate_z.data[1][1] = ( CosTable( iAngleZ ) );

            //temp = rotate_x * rotate_y;
            //rotate = temp * rotate_z;
            rotate = rotate_x * rotate_y * rotate_z;

            temp_x = x * rotate.data[0][0] +
                     y * rotate.data[1][0] +
                     z * rotate.data[2][0];
            temp_y = x * rotate.data[0][1] +
                     y * rotate.data[1][1] +
                     z * rotate.data[2][1];
            temp_z = x * rotate.data[0][2] +
                     y * rotate.data[1][2] +
                     z * rotate.data[2][2];

            x = temp_x;
            y = temp_y;
            z = temp_z;
        }
        break;

    default:
        break;
    }
}

double PRECISIONVECTOR::CosTable( int _idx )
{
    // static variable to only build the table once
    static double cos_look[ iSinCosTableSize + 1 ];
    static bool bCosTableInitialized = false;

    if ( ! bCosTableInitialized )
    {
        double rad;  // used in conversion from degrees to radians

        double angInc = 360.0 / (double)iSinCosTableSize;
        int idx = 0;
        for ( double angle = 0.0; angle <= 360.0; angle += angInc )
        {
            rad = 3.141592654 * angle / 180.0;

            cos_look[ idx ] = cos( rad );

            ++idx;
        }
        bCosTableInitialized = true;
    }

    return cos_look[ _idx ];
}

double PRECISIONVECTOR::SinTable( int _idx )
{
    // static variable to only build the table once
    static double sin_look[ iSinCosTableSize + 1 ];
    static bool bSinTableInitialized = false;

    if ( ! bSinTableInitialized )
    {
        double rad;  // used in conversion from degrees to radians

        double angInc = 360.0 / (double)iSinCosTableSize;
        int idx = 0;
        for ( double angle = 0.0; angle <= 360.0; angle += angInc )
        {
            rad = 3.141592654 * angle / 180.0;

            sin_look[ idx ] = sin( rad );

            ++idx;
        }
        bSinTableInitialized = true;
    }

    return sin_look[ _idx ];
}

double PRECISIONVECTOR::DotProd( const PRECISIONVECTOR & _v1, const PRECISIONVECTOR & _v2 )
{
    return PRECISIONVECTOR( _v1 ).DotP( _v2 );
}
*/
