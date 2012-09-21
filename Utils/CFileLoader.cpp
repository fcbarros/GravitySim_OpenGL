
#include "CFileLoader.h"

CFileLoader::CFileLoader( void )
{
    xml = NULL;
}

CFileLoader::~CFileLoader( void )
{
    Close( );
}

void CFileLoader::Close( void )
{
    if ( xml )
    {
        delete xml;
        xml = NULL;
    }
}

bool CFileLoader::OpenFile( string _stFilename )
{
    xml = new XML( _stFilename.c_str( ) );

    // 0 OK , 1 Header warning (not fatal) , 2 Error in parse (fatal)
    int iPS = xml->ParseStatus( ); 
    // TRUE OK
    bool bIT = xml->IntegrityTest( ); 

    if ( (iPS == 2 ) || ( bIT == false ) )
    {
        Close( );
        return false;
    }

    XML_VERSION_INFO xI = { 0 };
    xml->Version( &xI );

    if ( xI.VersionHigh != 1 )
    {
        return false;
    }

    xCurrentElement = xml->GetRootElement( );

    return true;
}

void CFileLoader::SetRoot( void )
{
    xCurrentElement = xml->GetRootElement( );
}

void CFileLoader::SetCurrentElement( XMLElement * _xElement )
{
    xCurrentElement = _xElement;
}

bool CFileLoader::SetCurrentElement( string _stElemName )
{
    XMLElement * elem = xCurrentElement->FindElementZ( _stElemName.c_str( ) );

    if ( elem != NULL )
    {
        xCurrentElement =  elem;
    }

    return elem != NULL;
}

bool CFileLoader::SetCurrentElement( unsigned int _idx )
{
    xCurrentElement = xCurrentElement->GetChildren( )[ _idx ];

    return xCurrentElement != NULL;
}

XMLElement * CFileLoader::GetCurrentElement( void )
{
    return xCurrentElement;
}

string CFileLoader::GetVariableValue( string _stName )
{
    XMLElement * element = xCurrentElement->FindElementZ( _stName.c_str( ) );
    if ( !element )
    {
        return "";
    }
    char buffer[ 4096 ];
    XMLVariable * variable = element->FindVariableZ( "value" );
    variable->GetValue( buffer );

    string stResult = buffer;

    return stResult;
}

void CFileLoader::Back( void )
{
    xCurrentElement = xCurrentElement->GetParent( );
}

string CFileLoader::GetElementName( void )
{
    int nMaxElName = xCurrentElement->GetElementName( 0 );
    char * stElemName = new char[ nMaxElName + 1 ];
    xCurrentElement->GetElementName( stElemName );
    string stName = stElemName;
    delete[] stElemName;

    return stName;
}

unsigned int CFileLoader::GetNumChildren( void )
{
    return xCurrentElement->GetChildrenNum( );
}

