
#ifndef _CUIImageButton_H_
#define _CUIImageButton_H_

#include "IUInterface.h"
#include "Utils/CTexture.h"

class CUIImageButton 
	: public IUInterface
{
public:

    enum enButtonStates
    {
        NORMAL = 0,
        PRESSED,
        OVER
    };

    CUIImageButton( unsigned int _uId,
                    float _fPosX, float _fPosY,
                    float _fWidth, float _fHeight,
                    IUICallBack * _pFunctionCallBack,
				    string _stTexNormal,
				    string _stTexPressed = "",
				    string _stTexOver = "" );

    virtual ~CUIImageButton( void );

    virtual void SetState( enButtonStates _enState );

    virtual enButtonStates GetState( void );

protected:

    virtual void DrawIntern( void );

    virtual unsigned int ProcessMsgIntern( const CUIMessage & _msg );


    enButtonStates  m_enState;

	CTexture        m_TexNormal;
	CTexture        m_TexOver;
	CTexture        m_TexPressed;
};

#endif