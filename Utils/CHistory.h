
#ifndef _CHistory_h_
#define _CHistory_h_

#include "Math/TVector3D.h"

class CHistory
{
public:
    CHistory( void );
    ~CHistory( void );

    void SetSize( int _iSize );

    int GetSize( void ) const;

    void Clear( void );

    int GetNumItems( void ) const;

    const PRECISIONVECTOR & operator[]( const int _idx ) const;

    void Add( const PRECISIONVECTOR & vVec );

private:

    PRECISIONVECTOR * pvHistory;

    int iSize;

    int iInit;

    int iEnd;
};

#endif