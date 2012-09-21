
#include "GeneralIncludes.h"
#include "CSpeedMath.h"

double CSpeedMath::CosTable( double _dAngle )
{
    // static variable to only build the table once
    static double cos_look[ iTableSize + 1 ];
    static bool bCosTableInitialized = false;

	if ( _dAngle < 0.0 )
	{
		_dAngle += 360.0;
	}

	int iAngle = (int)(_dAngle * (double)iTableIndexMultipler);

    if ( ! bCosTableInitialized )
    {
        double rad;  // used in conversion from degrees to radians

        double angInc = 360.0 / (double)iTableSize;
        int idx = 0;
        for ( double angle = 0.0; angle <= 360.0; angle += angInc )
        {
            rad = 3.141592654 * angle / 180.0;

            cos_look[ idx ] = cos( rad );

            ++idx;
        }
        bCosTableInitialized = true;
    }

    return cos_look[ iAngle ];
}

double CSpeedMath::ACosTableD( double _dVal )
{
    // static variable to only build the table once
    static double acos_look[ iTableSize + 1 ];
    static bool bACosTableInitialized = false;

	int iVal = (int)((_dVal + 1.0) * ((double)iTableSize * 0.5));

    if ( ! bACosTableInitialized )
    {
        double angInc = 2.0 / (double)iTableSize;
        int idx = 0;
        for ( double val = 1.0; val >= -1.0; val -= angInc )
        {
            acos_look[ idx ] = acos( val ) * 57.295779513082320876798154814105;
            ++idx;
        }
        bACosTableInitialized = true;
    }

    return acos_look[ iVal ];
}

double CSpeedMath::ACosTableR( double _dVal )
{
    // static variable to only build the table once
    static double acos_look[ iTableSize + 1 ];
    static bool bACosTableInitialized = false;

	int iVal = (int)((_dVal + 1.0) * ((double)iTableSize * 0.5));

    if ( ! bACosTableInitialized )
    {
        double angInc = 2.0 / (double)iTableSize;
        int idx = 0;
        for ( double val = 1.0; val >= -1.0; val -= angInc )
        {
            acos_look[ idx ] = acos( val );
            ++idx;
        }
        bACosTableInitialized = true;
    }

    return acos_look[ iVal ];
}

double CSpeedMath::SinTable( double _dAngle )
{
    // static variable to only build the table once
    static double sin_look[ iTableSize + 1 ];
    static bool bSinTableInitialized = false;

	if ( _dAngle < 0.0 )
	{
		_dAngle += 360.0;
	}

	int iAngle = (int)(_dAngle * (double)iTableIndexMultipler);

    if ( ! bSinTableInitialized )
    {
        double rad;  // used in conversion from degrees to radians

        double angInc = 360.0 / (double)iTableSize;
        int idx = 0;
        for ( double angle = 0.0; angle <= 360.0; angle += angInc )
        {
            rad = 3.141592654 * angle / 180.0;

            sin_look[ idx ] = sin( rad );

            ++idx;
        }
        bSinTableInitialized = true;
    }

    return sin_look[ iAngle ];
}
