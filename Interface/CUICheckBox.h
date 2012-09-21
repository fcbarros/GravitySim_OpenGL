
#ifndef _CUICheckBox_H_
#define _CUICheckBox_H_

#include "IUInterface.h"
#include "Utils/CGLFont.h"


class CUICheckBox 
	: public IUInterface
{
public:

    CUICheckBox( unsigned int _uId,
                 string _stCaption,
                 float _dPosX, float _dPosY,
                 IUICallBack * _pFunctionCallBack = NULL );

    virtual ~CUICheckBox( void );

    virtual void SetCaption( string _stCaption );

    virtual string GetCaption( void );

    virtual void SetChecked( bool _bChecked );

    virtual bool IsChecked( void );


protected:

    virtual void DrawIntern( void );

    virtual unsigned int ProcessMsgIntern( const CUIMessage & _msg );

    bool                m_bChecked;

    const float         m_fHExtraSpace;
    const float         m_fVExtraSpace;

    string              m_stCaption;

    // Class for drawing text in the window
    PixelPerfectGLFont  m_glFont;
};

#endif