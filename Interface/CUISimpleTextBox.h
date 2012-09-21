
#ifndef _CInterfaceTextBox_H_
#define _CInterfaceTextBox_H_

#include "IUInterface.h"
#include "Utils/CGLFont.h"


class CUISimpleTextBox 
	: public IUInterface
{
public:

    CUISimpleTextBox( unsigned int _uId,
                      float _dPosX, float _dPosY,
                      float _dWidth, float _dHeight );

    virtual ~CUISimpleTextBox( void );

    virtual void AddLines( string _stLine );

    virtual void Clear( void );


protected:

    virtual void DrawIntern( void );

    virtual unsigned int ProcessMsgIntern( const CUIMessage & _msg );

    bool                m_bMoving;

    bool                m_bVariableSize;

    const float         m_fHExtraSpace;
    const float         m_fVExtraSpace;

    vector<string>      m_TextLines;

    // Class for drawing text in the window
    PixelPerfectGLFont  m_glFont;
};

#endif