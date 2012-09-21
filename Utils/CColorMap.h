
#ifndef _CColorMap_h_
#define _CColorMap_h_

#include "ColorTablesDefines.h"

class CColorMap
{
public:

    enum enMapType
    {
        GRAY = 0,
        JET
    };

public:
    CColorMap( void );

    ~CColorMap( void );

    void SetMap( enMapType _type, int _iTableSize );

    const stColor & operator[]( int _iIdx );

    int GetSize( void ) const;
    
private:

    stColor * Interpolate( const stColor * _pSrc, int _iSrcSize, int _iDstSize );

    stColor * pTable;

    int iTableSize;
};

#endif
