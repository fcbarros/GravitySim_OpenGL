
#ifndef _CSimulator_h_
#define _CSimulator_h_

//#include <pthread/include/pthread.h>

#include "CMassObject.h"
#include "Utils/CColorMap.h"

struct stLinePair
{
    int i1;
    int i2;
};

struct stTriangleIdx
{
    int i1;
    int i2;
    int i3;
};

class CSimulator
{
public:

    enum enFieldType
    {
        TYPE_2D_COLORS = 0,
        TYPE_2D_DEPTH,
        TYPE_2D_VECTOR,
        TYPE_3D_COLORS
    };

    CSimulator( void );

    ~CSimulator( void );

    void Reset( void );

    void Run( void );

    void Stop( void );

    void Pause( bool _bPause );

    bool IsPaused( void );

    void GetBoundingBox( double & _dXMin, double & _dYMin, double & _dZMin,
                         double & _dXMax, double & _dYMax, double & _dZMax );

    void CalcGravityField( PRECISIONVECTOR _pCentral, 
                           double _dSize, double _dStep, double _dMult );

    PRECISIONVECTOR CalcAcceleration( const PRECISIONVECTOR & _pAny );
    void CalcAcceleration( int iIdx );
    void MoveBodies( double _dTime );
    double CollisionTime( void );
    void MoveBodiesTest( double _dTime );

    CMassObject & GetObject( const unsigned int _idx );

    void AddObject( const CMassObject & _obj );

    void Reserve( int _iNum );

    int GetNumObjects( void );

    void SetHistorySaveInterval( double _dCTEHistory );

    void SetTimeConstant( double _dTimeCTE );

    double GetTimeConstant( void );

    void SetNumIterations( double _dNumIterations );

    double GetNumIterations( void );

    double GetTimeReal( void );

    void TimeLoad( CFileLoader & _fl );

    void TimeSet( int _day, int _month, int _year,
                  int _hour, int _min, int _sec );

    string SimulatedTimeFormat( void );

    void SetFieldType( enFieldType _type );

    enFieldType GetFieldType( void );

private:

    void Move( double _dtime );

    void ProcessColision( void );

    void DrawField2DDepth( double hMin, double hMax, double dMult );
    void DrawField2DVectors( double _dSize, double _dMult );
    void DrawField2DColors( double hMin, double hMax, double dMult );
    void DrawGravityField( PRECISIONVECTOR _pCentral,
                           double _dSize, double _dMult );

    // Stores the Mass Objects in a vector class
    ObjectArray objMassObjects;

	double dTimeReal;
    double dTimeSimulation;
    double dTimeHistory;
    double dTimeConstant;
    double dMaxTimeConstant;
	double dNumIterations;
    double dCTETimeHistory;

    bool bPause;

    CColorMap ColorMap;

    enFieldType m_FieldType;

    //pthread_t thread;

    //stSimulatorData data;

    vector<stLinePair> vLinePair;
    //vector<stTriangleIdx> vTriangleIdx;
    vector<int> vTriangleStrip;
    vector<double> vFieldLength;
    vector<PRECISIONVECTOR> vFieldAccel;
    vector<PRECISIONVECTOR> vFieldPos;
    int iNumPoints;

    State * pState;
    Derivative * pA;
    Derivative * pB;
    Derivative * pC;
    Derivative * pD;

};

#endif