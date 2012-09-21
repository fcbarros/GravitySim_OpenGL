
#ifndef _CUIGroup_h_
#define _CUIGroup_h_

#include "IUInterface.h"


class CUIGroup 
	: public IUInterface
{
public:

	CUIGroup( unsigned int _uId );

	virtual ~CUIGroup( void );

	virtual void Add( IUInterface * _pItem );

	virtual void Clear( void );

    virtual bool CheckBounds( float _fX, float _fY );

    virtual IUInterface * GetItemById( unsigned int _uId );

    virtual unsigned int GetLastSelected( void );

protected:

    virtual unsigned int ProcessMsgIntern( const CUIMessage & _msg );

	virtual void DrawIntern( void );

    unsigned int            m_uLastSelected;

	vector<IUInterface*>    m_lstItems;
};

#endif
