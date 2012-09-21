
#include "GeneralIncludes.h"
#include "CColorMap.h"


CColorMap::CColorMap( void )
{
    pTable = NULL;

    iTableSize = 0;

    SetMap( JET, 64 );
}

CColorMap::~CColorMap( void )
{
    delete[] pTable;
}

void CColorMap::SetMap( enMapType _type, int _iTableSize )
{
    delete[] pTable;

    switch ( _type )
    {
    case GRAY:
        iTableSize = _iTableSize;
        pTable = Interpolate( GrayTable, 64, iTableSize );
        break;

    case JET:
        iTableSize = _iTableSize;
        pTable = Interpolate( JetTable, 64, iTableSize );
        break;

    default:
        pTable = NULL;
        break;
    }
}

const stColor & CColorMap::operator[]( int _iIdx )
{
    _iIdx = _iIdx < iTableSize ? _iIdx : iTableSize - 1;
    return pTable[ _iIdx ];
}

stColor * CColorMap::Interpolate( const stColor * _pSrc, int _iSrcSize, int _iDstSize )
{
    stColor * pRetTable = new stColor[ _iDstSize ];

    int iDiv = _iDstSize / _iSrcSize;

    for ( int i = 0; i < _iDstSize; i += iDiv )
    {
        float dDifR = ( _pSrc[ ( i / iDiv ) + 1 ].r - _pSrc[ i / iDiv ].r ) / (float)iDiv;
        float dDifG = ( _pSrc[ ( i / iDiv ) + 1 ].g - _pSrc[ i / iDiv ].g ) / (float)iDiv;
        float dDifB = ( _pSrc[ ( i / iDiv ) + 1 ].b - _pSrc[ i / iDiv ].b ) / (float)iDiv;
        float dDifA = ( _pSrc[ ( i / iDiv ) + 1 ].a - _pSrc[ i / iDiv ].a ) / (float)iDiv;
        for ( int j = 0; j < iDiv; j++ )
        {
            pRetTable[ i + j ].r = _pSrc[ i / iDiv ].r + dDifR * j;
            pRetTable[ i + j ].g = _pSrc[ i / iDiv ].g + dDifG * j;
            pRetTable[ i + j ].b = _pSrc[ i / iDiv ].b + dDifB * j;
            pRetTable[ i + j ].a = _pSrc[ i / iDiv ].a + dDifA * j;
        }
    }

    return pRetTable;
}

int CColorMap::GetSize( void ) const
{
    return iTableSize;
}
