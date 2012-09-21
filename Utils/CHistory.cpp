
#include "GeneralIncludes.h"

#include "CHistory.h"

CHistory::CHistory( void )
{
    pvHistory = NULL;

    iSize = 0;

    iInit = 0;

    iEnd = 0;
}

CHistory::~CHistory( void )
{
    Clear( );
}

void CHistory::Clear( void )
{
    if ( pvHistory )
    {
        delete[] pvHistory;

        pvHistory = NULL;

        iSize = 0;

        iInit = 0;

        iEnd = 0;
    }
}

void CHistory::SetSize( int _iSize )
{
    Clear( );

    iSize = _iSize;
    pvHistory = new PRECISIONVECTOR[ iSize ];
}

int CHistory::GetSize( void ) const
{
    return iSize;
}

const PRECISIONVECTOR & CHistory::operator[]( const int _idx ) const
{
    return pvHistory[ ( iInit + _idx ) % iSize ];
}

int CHistory::GetNumItems( void ) const
{
    return iEnd;
}

void CHistory::Add( const PRECISIONVECTOR & vVec )
{
    if ( iEnd < iSize )
    {
        pvHistory[ iEnd++ ] = vVec;
    }
    else
    {
        pvHistory[ iInit++ ] = vVec;
        if ( iInit >= iSize )
        {
            iInit = 0;
        }
    }
}

