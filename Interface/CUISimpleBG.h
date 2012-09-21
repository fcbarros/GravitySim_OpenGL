
#ifndef _CUISimpleBG_H_
#define _CUISimpleBG_H_

#include "IUInterface.h"

class CUISimpleBG 
	: public IUInterface
{
public:

    CUISimpleBG( unsigned int _uId,
                 float _dPosX, float _dPosY,
                 float _dWidth, float _dHeight );

    virtual ~CUISimpleBG( void );


protected:

    virtual void DrawIntern( void );

    virtual unsigned int ProcessMsgIntern( const CUIMessage & _msg );

    bool m_bMoving;
};

#endif