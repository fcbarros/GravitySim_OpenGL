
#ifndef _IUInterface_h_
#define _IUInterface_h_

// Including Utils classes for interface
#include "UIUtil.h"

// Virtual Class Interface
class IUInterface
{
public:

    //////////////////////////////////////////////////////////////////
    // Constructors and Destructor
    //////////////////////////////////////////////////////////////////
    IUInterface( unsigned int _uId, 
                 float _fX = 0.0f, float _fY = 0.0f,
                 float _fWidth = 0.0f, float _fHeight = 0.0f,
                 IUICallBack * _pInterfaceCallBack = NULL );

    IUInterface( const IUInterface & _interface ); 

    virtual ~IUInterface( void );

    //////////////////////////////////////////////////////////////////
    // Move/Size related functions
    //////////////////////////////////////////////////////////////////
	inline virtual float GetX( void ) const;

	inline virtual float GetY( void ) const;

    inline virtual float GetAbsX( void ) const;

    inline virtual float GetAbsY( void ) const;

	inline virtual float GetWidth( void ) const;

	inline virtual float GetHeight( void ) const;

	inline virtual void Move( float _fPosX, float _fPosY );

	inline virtual void Resize( float _fWidth, float _fHeight );

    //////////////////////////////////////////////////////////////////
    // State related functions
    //////////////////////////////////////////////////////////////////
    inline virtual void SetVisible( bool _bVisible );

    inline virtual bool IsVisible( void ) const;

    inline virtual void SetEnabled( bool _bEnabled );

    inline virtual bool IsEnabled( void ) const;

    virtual void SetForegroundColor( float _fR, float _fG, float _fB, float _fA );

    virtual void SetBackgroundColor( float _fR, float _fG, float _fB, float _fA );

    virtual void SetBorderColor( float _fR, float _fG, float _fB, float _fA );

    //////////////////////////////////////////////////////////////////
    // Message processing functions
    //////////////////////////////////////////////////////////////////
    virtual bool CheckBounds( float _fX, float _fY );

    virtual unsigned int ProcessMsg( const CUIMessage & _msg );

    //////////////////////////////////////////////////////////////////
    // General functions
    //////////////////////////////////////////////////////////////////
    virtual IUInterface * GetItemById( unsigned int _uId );

	virtual unsigned int GetId( void ) const;
    
    virtual void Draw( void );

    virtual void SetCallBack( IUICallBack * _pInterfaceCallBack );

    virtual void SetParent( IUInterface * _pInterfaceParent );

    virtual const IUInterface * GetParent( void ) const;


protected:

    //////////////////////////////////////////////////////////////////
    // General drawing functions
    //////////////////////////////////////////////////////////////////
    virtual void DrawBackGround( void );

    virtual void DrawBorders( void );

    //////////////////////////////////////////////////////////////////
    // Pure virtual functions to implement
    //////////////////////////////////////////////////////////////////
    virtual void DrawIntern( void ) = 0;

    virtual unsigned int ProcessMsgIntern( const CUIMessage & _msg ) = 0;

    //////////////////////////////////////////////////////////////////
    // Variables
    //////////////////////////////////////////////////////////////////
    const unsigned int  m_uId;

    float               m_fPosX;
    float               m_fPosY;

    float               m_fWidth;
    float               m_fHeight;

    bool                m_bVisible;

    bool                m_bEnabled;

    CUIColor            m_ColorForeground;
    CUIColor            m_ColorBackGround;
    CUIColor            m_ColorBorder;

    IUInterface *       m_pInterfaceParent;

    IUICallBack *       m_CallBackInterface;
};


#endif