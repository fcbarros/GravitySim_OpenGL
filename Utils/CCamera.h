
#ifndef _CCAMERA_H_
#define _CCAMERA_H_

#include "CFileLoader.h"
#include "Math/TVector3D.h"

class CCamera
{
public:

    enum enCamType
    {
        FREE,
        FIXED
    };

    CCamera( void );

    void Orbit( double _dAngleX, double _dAngleY, double _dAngleZ );

    void Pan( double _dX, double _dY, double _dZ );

    void Rotate( double _dAngleX, double _dAngleY, double _dAngleZ );

    void Reset( void );

    void Load( CFileLoader & _fl );

    void SetDistance( double _dDist );

    double GetDistance( void );

    double GetPercentOfScreen( const PRECISIONVECTOR & _v1, const double _dSize );

    double GetSizeForPercent( const PRECISIONVECTOR & _v1, const double _dPercent );

    double GetDistanceForSize( const PRECISIONVECTOR & _v1 );

    void DoClear( void );
    void DoPosition( void );
    void DoOrbit( void );
    void DoLookAT( void );
    void DoFreeRotation( void );

    void SetPosition( double _dX, double _dY, double _dZ );

    void SetPosition( const PRECISIONVECTOR & _v1 );

    void SetLookAt( double _dX, double _dY, double _dZ );

    void SetLookAt( const PRECISIONVECTOR & _v1 );

    void SetUp( double _dX, double _dY, double _dZ );

    void SetUp( const PRECISIONVECTOR & _v1 );

    const PRECISIONVECTOR & GetPosition( void );

    const PRECISIONVECTOR & GetLookAt( void );

    const PRECISIONVECTOR & GetUp( void );

    const PRECISIONVECTOR & GetOrbit( void );

private:

    enCamType m_eType;

    double m_dDist;

    // Camera position
    PRECISIONVECTOR m_vPos;
    // Camera Look At
    PRECISIONVECTOR m_vLookAt;
    // Tilt Orientation
    PRECISIONVECTOR m_vUp;

    PRECISIONVECTOR m_vOrbit;

    PRECISIONVECTOR m_vPan;

    PRECISIONVECTOR m_vAngle;
};

#endif