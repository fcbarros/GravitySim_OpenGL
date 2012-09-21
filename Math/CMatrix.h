
#ifndef _CMATRIX_H_
#define _CMATRIX_H_

class CMatrix4x4
{
public:
    typedef double tMatrixData[ 4 ][ 4 ];

    CMatrix4x4( void );

    void Identity( void );

    CMatrix4x4 & operator=( const CMatrix4x4 & _mSource );

    CMatrix4x4 & operator*( const CMatrix4x4 & _m1 );

    tMatrixData data;
};

#endif
