
#include "GeneralIncludes.h"
#include "CMatrix.h"


CMatrix4x4::CMatrix4x4( void )
{
    Identity( );
}

void CMatrix4x4::Identity( void )
{
    static const tMatrixData mIdentityMatrix = 
    {
        { 1.0, 0.0, 0.0, 0.0 },
        { 0.0, 1.0, 0.0, 0.0 },
        { 0.0, 0.0, 1.0, 0.0 },
        { 0.0, 0.0, 0.0, 1.0 },
    };

    memcpy( data, mIdentityMatrix, sizeof( tMatrixData ) );
}

CMatrix4x4 & CMatrix4x4::operator=( const CMatrix4x4 & _mSource )
{
    memcpy( data, _mSource.data, sizeof( tMatrixData ) );

    return *this;
}

CMatrix4x4 & CMatrix4x4::operator*( const CMatrix4x4 & _m1 )
{
    static CMatrix4x4 mResult;

    mResult.data[0][0] = data[0][0] * _m1.data[0][0] + data[0][1] * _m1.data[1][0] + data[0][2] * _m1.data[2][0];
    mResult.data[0][1] = data[0][0] * _m1.data[0][1] + data[0][1] * _m1.data[1][1] + data[0][2] * _m1.data[2][1];
    mResult.data[0][2] = data[0][0] * _m1.data[0][2] + data[0][1] * _m1.data[1][2] + data[0][2] * _m1.data[2][2];
    mResult.data[0][3] = 0.0;  // can probably get rid of this too, it's always 0

    mResult.data[1][0] = data[1][0] * _m1.data[0][0] + data[1][1] * _m1.data[1][0] + data[1][2] * _m1.data[2][0];
    mResult.data[1][1] = data[1][0] * _m1.data[0][1] + data[1][1] * _m1.data[1][1] + data[1][2] * _m1.data[2][1];
    mResult.data[1][2] = data[1][0] * _m1.data[0][2] + data[1][1] * _m1.data[1][2] + data[1][2] * _m1.data[2][2];
    mResult.data[1][3] = 1.0; // can probably get rid of this too, it's always 0

    mResult.data[2][0] = data[2][0] * _m1.data[0][0] + data[2][1] * _m1.data[1][0] + data[2][2] * _m1.data[2][0];
    mResult.data[2][1] = data[2][0] * _m1.data[0][1] + data[2][1] * _m1.data[1][1] + data[2][2] * _m1.data[2][1];
    mResult.data[2][2] = data[2][0] * _m1.data[0][2] + data[2][1] * _m1.data[1][2] + data[2][2] * _m1.data[2][2];
    mResult.data[2][3] = 0.0; // can probably get rid of this too, it's always 0

    mResult.data[3][0] = data[3][0] * _m1.data[0][0] + data[3][1] * _m1.data[1][0] + data[3][2] * _m1.data[2][0] + _m1.data[3][0];
    mResult.data[3][1] = data[3][0] * _m1.data[0][1] + data[3][1] * _m1.data[1][1] + data[3][2] * _m1.data[2][1] + _m1.data[3][1];
    mResult.data[3][2] = data[3][0] * _m1.data[0][2] + data[3][1] * _m1.data[1][2] + data[3][2] * _m1.data[2][2] + _m1.data[3][2];
    mResult.data[3][3] = 1.0; // can probably get rid of this too, it's always 0

    return mResult;
}
