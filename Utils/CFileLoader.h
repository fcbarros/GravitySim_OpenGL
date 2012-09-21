
#ifndef _CFileLoader_h_
#define _CFileLoader_h_

#include "xml/xml.h"

class CFileLoader
{
public:
    CFileLoader( void );

    ~CFileLoader( void );

    void Close( void );

    bool OpenFile( string pStzFilename );

    void SetRoot( void );

    void SetCurrentElement( XMLElement * _xElement );

    bool SetCurrentElement( string _stElemName );

    bool SetCurrentElement( unsigned int _idx );

    XMLElement * GetCurrentElement( void );

    void Back( void );

    unsigned int GetNumChildren( void );

    string GetVariableValue( string _stName );

    string GetElementName( void );

private:

    XML * xml;
    XMLElement * xCurrentElement;
};

#endif