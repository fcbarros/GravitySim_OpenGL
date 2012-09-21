
#ifndef _UInterfaceUtil_h_
#define _UInterfaceUtil_h_

#include "GeneralIncludes.h"
#include "Utils/CRender.h"

// Just for reference
class IUInterface;

//////////////////////////////////////////////////////////////////
// CInterfaceMessage Class
// Holds message events
//////////////////////////////////////////////////////////////////
class CUIMessage
{
public:
    enum enMsgType
    {
        UNKNOWN = 0,
        MOUSE_DOWN,
        MOUSE_UP,
        MOUSE_MOVE,
        MOUSE_DBLCLICK,
        KEY_DOWN,
        KEY_UP,
        ENTER,
        LEAVE,
        SIZE
    };

    CUIMessage( void );

    enMsgType Type;

    float  fParam1;
    float  fParam2;

    unsigned int uParam1;
    unsigned int uParam2;
};

//////////////////////////////////////////////////////////////////
// CinterfaceColor Class
// Holds color information
//////////////////////////////////////////////////////////////////
class CUIColor
{
public:
    CUIColor( void );
    CUIColor( float _fR, float _fG, float _fB, float _fA );

    float r;
    float g;
    float b;
    float a;
};

//////////////////////////////////////////////////////////////////
// CInterfaceCornersColors Class
// Holds color information
//////////////////////////////////////////////////////////////////
class CUICornersColors
{
public:
    enum tCorner
    {
        CORNER_ALL = 0,
        CORNER_UL,
        CORNER_UR,
        CORNER_LL,
        CORNER_LR
    };

    void Set( tCorner _Corner, const CUIColor & _color );

    // Up Left
    CUIColor ul;
    // Up Right
    CUIColor ur;
    // Lower Left
    CUIColor ll;
    // Lower Right
    CUIColor lr;
};

//////////////////////////////////////////////////////////////////
// CInterfaceCallBack Class
// Any class that wants to receive Event CallBack 
//////////////////////////////////////////////////////////////////
class IUICallBack
{
public:
    virtual unsigned int CallBackFunction( const IUInterface & _InterfaceObj, const CUIMessage & _msg ) = 0;
};


#endif
