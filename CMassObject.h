
#ifndef _CMassObject_h_
#define _CMassObject_h_

#include "Utils/Common.h"
#include "Utils/CFileLoader.h"
#include "Utils/CHistory.h"
#include "Utils/CCamera.h"
#include "Utils/CTexture.h"
#include "Math/TVector3D.h"

static const double dConstG = 6.67428e-11; // N m2 kg-2

class CMassObject;
typedef vector<CMassObject> ObjectArray;

struct State
{
    PRECISIONVECTOR x;
    PRECISIONVECTOR v;
};

struct Derivative
{
    PRECISIONVECTOR dx;
    PRECISIONVECTOR dv;
};



class CMassObject
{
public:

	CMassObject( void );

    ~CMassObject( void );

	CMassObject( const CMassObject &mObject );

	void Draw( CCamera & _cam, bool _bDrawExtras );

    void SaveHistory( void );

    bool Load( CFileLoader & _fl );

    bool IsLight( void ) const;

    void Join( const CMassObject & _obj );

    void Clear( void );

    string GetName( void );

    static int GetTotalLights( void );

    void DrawLight( CCamera & _cam );

    double CheckCollisionSimple( CMassObject & _obj );

    void ShowVelocityVector( bool _bShow );

    void ShowAccelerationVector( bool _bShow );

    bool IsShowVelocityVector( void );

    bool IsShowAccelerationVector( void );

public:

    PRECISIONVECTOR	vPosTemp;
    PRECISIONVECTOR	vVelTemp;
    PRECISIONVECTOR	vPosition;
    PRECISIONVECTOR	vVelocity;
    PRECISIONVECTOR vAcceleration;
	double			dMass_Kg;
	double			dRadius_m;
	stColor			sColor;
    string          stTextureName;

    // Pre computation of mass * G constant
	double			dMassG; 

private:

    void GenerateSphere( void );
    void SetLight( bool _bEnable );

    CHistory        History;
    unsigned int    SphereList;
    string          stName;
    int             iLightNum;
    int             iHistorySize;
    static int      iTotalLightNum;
    CTexture        Texture;
    double          dAngleRot;
    CTexture        LightTexture;

    bool            m_bShowVel;
    bool            m_bShowAccel;
};

#endif
