
#include "CSimulator.h"
#include "Utils/CRender.h"
#include <limits>
#include <CFloat>
#include <time.h>


CSimulator::CSimulator( void )
{
    ColorMap.SetMap( CColorMap::JET, 64 );
    //ColorMap.SetMap( CColorMap::JET, 1024 );
    //ColorMap.SetMap( CColorMap::GRAY, 64 );

    m_FieldType = TYPE_2D_VECTOR;

    pD = NULL;
    pC = NULL;
    pB = NULL;
    pA = NULL;
    pState = NULL;

    Reset( );
}

CSimulator::~CSimulator( void )
{
    Reset( );
}

void CSimulator::Reset( void )
{
    Stop( );

    objMassObjects.clear( );

	dTimeReal = 0.0;
    dTimeSimulation = 0.0;
    dTimeHistory = 0.0;
    dMaxTimeConstant = 60.0 * 60.0 * 24.0 * 2.0;
    dTimeConstant = 60.0 * 60.0 * 24.0 * 2.0;
	dNumIterations = 10.0;
    dCTETimeHistory = 1.0;

    delete[] pD;
    delete[] pC;
    delete[] pB;
    delete[] pA;
    delete[] pState;

    pD = NULL;
    pC = NULL;
    pB = NULL;
    pA = NULL;
    pState = NULL;

    vFieldAccel.clear( );
    vFieldPos.clear( );
    iNumPoints = 0;
}

CMassObject & CSimulator::GetObject( const unsigned int _idx )
{
    return objMassObjects[ _idx ];
}

void CSimulator::AddObject( const CMassObject & _obj )
{
    objMassObjects.push_back( _obj );

    delete[] pD;
    delete[] pC;
    delete[] pB;
    delete[] pA;
    delete[] pState;
    pState = new State[ (int)objMassObjects.size( ) ];
    pA = new Derivative[ (int)objMassObjects.size( ) ];
    pB = new Derivative[ (int)objMassObjects.size( ) ];
    pC = new Derivative[ (int)objMassObjects.size( ) ];
    pD = new Derivative[ (int)objMassObjects.size( ) ];

}

void CSimulator::Reserve( int _iNum )
{
    objMassObjects.reserve( _iNum );
}

int CSimulator::GetNumObjects( void )
{
    return (int)objMassObjects.size( );
}

void CSimulator::SetFieldType( enFieldType _type )
{
    m_FieldType = _type;
}

CSimulator::enFieldType CSimulator::GetFieldType( void )
{
    return m_FieldType;
}

void CSimulator::SetHistorySaveInterval( double _dCTEHistory )
{
    dCTETimeHistory = _dCTEHistory;
}

void CSimulator::SetTimeConstant( double _dTimeCTE )
{
    dTimeConstant = _dTimeCTE;
    dMaxTimeConstant = _dTimeCTE;
}

double CSimulator::GetTimeConstant( void )
{
    return dTimeConstant;
}

void CSimulator::SetNumIterations( double _dNumIterations )
{
    dNumIterations = _dNumIterations;
}

double CSimulator::GetNumIterations( void )
{
    return dNumIterations;
}

void CSimulator::ProcessColision( void )
{
    //for ( int i = (int)objMassObjects.size( ) - 1; i >= 0 ; --i )
    //{
    //    CalcAccelaraion( i );
    //}

    for ( int i = (int)objMassObjects.size( ) - 1; i > 0 ; --i )
    {
        for ( int j = i - 1; j >= 0; --j )
        {
            PRECISIONVECTOR vTempVec( objMassObjects[ j ].vPosition );
            vTempVec -= objMassObjects[ i ].vPosition;
            
            double dMin = objMassObjects[ j ].dRadius_m + objMassObjects[ i ].dRadius_m;

            if ( vTempVec.LengthSquared( ) <= ( dMin * dMin ) )
            {
                objMassObjects[ i ].Join( objMassObjects[ j ] );
                objMassObjects.erase( objMassObjects.begin( ) + j );
                return;
            }
        }
    }
}

// Calculates the acceleration vector for one object
void CSimulator::CalcAcceleration( int _iIdx )
{
    // Clearing the acceleration vector
    objMassObjects[ _iIdx ].vAcceleration.Clear( );

    PRECISIONVECTOR vTempVec;
    double dOneOverLengthSq;
    // Calculating the Acceleration
    for ( int j = (int)objMassObjects.size( ) - 1; j >= 0; --j )
    {
        if ( _iIdx != j )
        {
            vTempVec = objMassObjects[ j ].vPosition  - objMassObjects[ _iIdx ].vPosition;
            dOneOverLengthSq = objMassObjects[ j ].dMassG / vTempVec.LengthSquared( );

            vTempVec.Normalize( );

            objMassObjects[ _iIdx ].vAcceleration += vTempVec * dOneOverLengthSq;
        }
    }
}

PRECISIONVECTOR CSimulator::CalcAcceleration( const PRECISIONVECTOR & _pAny )
{
    // Clearing the acceleration vector
    PRECISIONVECTOR pAccel;

    PRECISIONVECTOR vTempVec;
    double dOneOverLengthSq;

    // Calculating the Acceleration
    for ( int j = (int)objMassObjects.size( ) - 1; j >= 0; --j )
    {
        vTempVec = objMassObjects[ j ].vPosition  - _pAny;
        double dLS = vTempVec.LengthSquared( );
        if ( dLS < ( objMassObjects[ j ].dRadius_m * objMassObjects[ j ].dRadius_m ) )
        {
            dLS = objMassObjects[ j ].dRadius_m * objMassObjects[ j ].dRadius_m;
        }
        dOneOverLengthSq = objMassObjects[ j ].dMassG / dLS;

        vTempVec.Normalize( );

        pAccel += vTempVec * dOneOverLengthSq;
    }

    return pAccel;
}

void CSimulator::DrawField2DDepth( double hMin, double hMax, double dMult )
{
    hMax /= dMult;

    glBegin( GL_LINES );

    for ( int k = (int)vLinePair.size( ) - 1; k >= 0; --k )
    {
        double z = ( vFieldAccel[ vLinePair[ k ].i1 ].Length( ) - hMin ) / hMax;
        double dNorm = z / dMult;
        dNorm = dNorm <= 1.0 ? dNorm : 1.0;
        dNorm = dNorm >= 0.0 ? dNorm : 0.0;
        unsigned int uColor = (unsigned int)( dNorm * ( ColorMap.GetSize( ) - 1 ) );
        glColor4fv( ColorMap[ uColor ].v );
        PRECISIONVECTOR vTemp = vFieldPos[ vLinePair[ k ].i1 ];
        vTemp.z -= z;
        glVertex3dv( vTemp.v );

        z = ( vFieldAccel[ vLinePair[ k ].i2 ].Length( ) - hMin ) / hMax;
        dNorm = z / dMult;
        dNorm = dNorm <= 1.0 ? dNorm : 1.0;
        dNorm = dNorm >= 0.0 ? dNorm : 0.0;
        uColor = (unsigned int)( dNorm * ( ColorMap.GetSize( ) - 1 ) );
        glColor4fv( ColorMap[ uColor ].v );
        vTemp = vFieldPos[ vLinePair[ k ].i2 ];
        vTemp.z -= z;
        glVertex3dv( vTemp.v );
    }

    glEnd( );
}

void CSimulator::DrawField2DColors( double hMin, double hMax, double dMult )
{
    //hMax /= dMult;
/*
    glBegin( GL_TRIANGLES );

    for ( int k = (int)vTriangleIdx.size( ) - 1; k >= 0; --k )
    {
        double z = ( vFieldLength[ vTriangleIdx[ k ].i1 ] - hMin ) / hMax;
        double dNorm = z * dMult;
        dNorm = dNorm <= 1.0 ? dNorm : 1.0;
        dNorm = dNorm >= 0.0 ? dNorm : 0.0;
        unsigned int uColor = (unsigned int)( dNorm * ( ColorMap.GetSize( ) - 1 ) );
        glColor4fv( ColorMap[ uColor ].v );
        PRECISIONVECTOR vTemp = vFieldPos[ vTriangleIdx[ k ].i1 ];
        vTemp.z -= z;
        glVertex3dv( vTemp.v );

        z = ( vFieldLength[ vTriangleIdx[ k ].i2 ] - hMin ) / hMax;
        dNorm = z * dMult;
        dNorm = dNorm <= 1.0 ? dNorm : 1.0;
        dNorm = dNorm >= 0.0 ? dNorm : 0.0;
        uColor = (unsigned int)( dNorm * ( ColorMap.GetSize( ) - 1 ) );
        glColor4fv( ColorMap[ uColor ].v );
        vTemp = vFieldPos[ vTriangleIdx[ k ].i2 ];
        vTemp.z -= z;
        glVertex3dv( vTemp.v );

        z = ( vFieldLength[ vTriangleIdx[ k ].i3 ] - hMin ) / hMax;
        dNorm = z * dMult;
        dNorm = dNorm <= 1.0 ? dNorm : 1.0;
        dNorm = dNorm >= 0.0 ? dNorm : 0.0;
        uColor = (unsigned int)( dNorm * ( ColorMap.GetSize( ) - 1 ) );
        glColor4fv( ColorMap[ uColor ].v );
        vTemp = vFieldPos[ vTriangleIdx[ k ].i3 ];
        vTemp.z -= z;
        glVertex3dv( vTemp.v );
    }

    glEnd( );
*/

    glDisable( GL_CULL_FACE );

    glBegin( GL_TRIANGLE_STRIP );

    for ( int k = (int)vTriangleStrip.size( ) - 1; k >= 0; --k )
    {
        double z = ( vFieldLength[ vTriangleStrip[ k ] ] - hMin ) / hMax;
        double dNorm = z * dMult;
        dNorm = dNorm <= 1.0 ? dNorm : 1.0;
        dNorm = dNorm >= 0.0 ? dNorm : 0.0;
        unsigned int uColor = (unsigned int)( dNorm * ( ColorMap.GetSize( ) - 1 ) );
        glColor4fv( ColorMap[ uColor ].v );
        PRECISIONVECTOR vTemp = vFieldPos[ vTriangleStrip[ k ] ];
        vTemp.z -= z;
        glVertex3dv( vTemp.v );
    }

    glEnd( );

    glEnable( GL_CULL_FACE );

}

void DrawGravitHistogram( double * pdHeightMap, double width, double dNumPoints, double dMult )
{
    double dX = -width / 2.0;
    double dDeltaX = width / dNumPoints;

    glBegin( GL_LINE_STRIP );

    glColor3f( 1.0, 1.0, 1.0 );
    for ( int i = 0; i < dNumPoints; ++i )
    {
        glVertex3d( dX, pdHeightMap[ i ] * dMult, 0.0 );
        dX += dDeltaX;
    }

    glEnd( );
}

void CSimulator::DrawField2DVectors( double _dSize, double _dMult )
{
    double dX = 2.0 * _dSize * 0.333 / (double)iNumPoints;
    for ( int i = ( iNumPoints * iNumPoints ) - 1; i >= 0; --i )
    {
        PRECISIONVECTOR vTemp = vFieldAccel[ i ];
        unsigned int uColor = (unsigned int)( ( vFieldLength[ i ] * _dMult ) * ( ColorMap.GetSize( ) - 1 ) );
        glColor4fv( ColorMap[ uColor ].v );

        vTemp *= dX / vFieldLength[ i ];
        vTemp += vFieldPos[ i ];
        
        g_Render.DrawArrow( vFieldPos[ i ], vTemp );
    }
}

void CSimulator::DrawGravityField( PRECISIONVECTOR _pCentral,
                                   double _dSize, double _dMult )
{
    glPushMatrix( );

    glEnable( GL_LINE_SMOOTH );
    glEnable( GL_BLEND );
    glLineWidth( 1.5f );

    //DrawGravitHistogram( dLen, 2.0 * _dInit, iNumPoints, _dMult );

    switch ( m_FieldType )
    {
    case TYPE_2D_COLORS:
        DrawField2DColors( 0.0, 1.0, _dMult );
        break;

    case TYPE_2D_DEPTH:
        DrawField2DDepth( 0.0, 1.0, _dMult );
        break;

    case TYPE_2D_VECTOR:
        DrawField2DVectors( _dSize, _dMult );
        break;

    case TYPE_3D_COLORS:
        break;
    }

    glTranslated( _pCentral.x, _pCentral.y, _pCentral.z );
    glColor4d( 1.0, 1.0, 1.0, 0.5 );
    glBegin( GL_LINES );

        glVertex3d( -_dSize, -_dSize, -_dSize );
        glVertex3d( _dSize, -_dSize, -_dSize );

        glVertex3d( _dSize, -_dSize, -_dSize );
        glVertex3d( _dSize, _dSize, -_dSize );

        glVertex3d( _dSize, _dSize, -_dSize );
        glVertex3d( -_dSize, _dSize, -_dSize );

        glVertex3d( -_dSize, _dSize, -_dSize );
        glVertex3d( -_dSize, -_dSize, -_dSize );

        glVertex3d( -_dSize, -_dSize, _dSize );
        glVertex3d( _dSize, -_dSize, _dSize );

        glVertex3d( _dSize, -_dSize, _dSize );
        glVertex3d( _dSize, _dSize, _dSize );

        glVertex3d( _dSize, _dSize, _dSize );
        glVertex3d( -_dSize, _dSize, _dSize );

        glVertex3d( -_dSize, _dSize, _dSize );
        glVertex3d( -_dSize, -_dSize, _dSize );

        glVertex3d( -_dSize, -_dSize, -_dSize );
        glVertex3d( -_dSize, -_dSize, _dSize );

        glVertex3d( _dSize, -_dSize, -_dSize );
        glVertex3d( _dSize, -_dSize, _dSize );

        glVertex3d( _dSize, _dSize, -_dSize );
        glVertex3d( _dSize, _dSize, _dSize );

        glVertex3d( -_dSize, _dSize, -_dSize );
        glVertex3d( -_dSize, _dSize, _dSize );

    glEnd( );

    glLineWidth( 1.0f );
    glDisable( GL_LINE_SMOOTH );
    glDisable( GL_BLEND );

    glPopMatrix( );
}

void CSimulator::CalcGravityField( PRECISIONVECTOR _pCentral,
                                   double _dSize, double _dNumSteps, double _dMult )
{
    if ( iNumPoints != (int)_dNumSteps )
    {
        iNumPoints = (int)_dNumSteps;
        vFieldAccel.resize( iNumPoints * iNumPoints + 1 );
        vFieldLength.resize( iNumPoints * iNumPoints + 1 );
        vFieldPos.resize( iNumPoints * iNumPoints + 1 );
        vLinePair.resize( iNumPoints * 2 * ( iNumPoints + 1 ) );
        //vTriangleIdx.resize( ( iNumPoints + 1 ) * ( iNumPoints + 1 ) * 2 );
        vTriangleStrip.resize( iNumPoints * ( ( iNumPoints - 1 ) * 2 + 1 ) );

        int k = 0;
        for ( int j = 0; j < iNumPoints; ++j )
        {
            for ( int i = 0; i < iNumPoints; ++i )
            {
                if ( i < ( iNumPoints - 1 ) )
                {
                    vLinePair[ k ].i1 = i * iNumPoints + j;
                    vLinePair[ k ].i2 = ( i + 1 ) * iNumPoints + j;
                    ++k;
                }
                if ( j < ( iNumPoints - 1 ) )
                {
                    vLinePair[ k ].i1 = i * iNumPoints + j;
                    vLinePair[ k ].i2 = i * iNumPoints + ( j + 1 );
                    ++k;
                }
            }
        }

        /*
        k = 0;
        for ( int j = 0; j < iNumPoints - 1; ++j )
        {
            for ( int i = 0; i < iNumPoints - 1; ++i )
            {
                vTriangleIdx[ k ].i1 = i * iNumPoints + j;
                vTriangleIdx[ k ].i2 = ( i + 1 ) * iNumPoints + ( j + 1 );
                vTriangleIdx[ k ].i3 = i * iNumPoints + ( j + 1 );
                ++k;

                vTriangleIdx[ k ].i1 = i * iNumPoints + j;
                vTriangleIdx[ k ].i2 = ( i + 1 ) * iNumPoints + j;
                vTriangleIdx[ k ].i3 = ( i + 1 ) * iNumPoints + ( j + 1 );
                ++k;
            }
        }
        */
        k = 0;
        vTriangleStrip[ k++ ] = 0;
        for ( int j = 0; j < iNumPoints - 2; j += 2 )
        {
            for ( int i = 0; i < iNumPoints; ++i )
            {
                vTriangleStrip[ k++ ] = i * iNumPoints + ( j + 1 );
                if ( i < iNumPoints - 1 )
                {
                    vTriangleStrip[ k++ ] = ( i + 1 ) * iNumPoints + j;
                }
            }
            for ( int i = iNumPoints - 1; i >= 0; --i )
            {
                vTriangleStrip[ k++ ] = i * iNumPoints + ( j + 2 );
                if ( i > 0 )
                {
                    vTriangleStrip[ k++ ] = ( i - 1 ) * iNumPoints + ( j + 1 );
                }
            }
        }
    }

    //double dMin = DBL_MAX;
    //double dMax = DBL_MIN;
    //double dMean = 0.0;
    double dStep = 2.0 * _dSize / _dNumSteps;

    int i = 0;
    for ( int j = 0; j < iNumPoints; ++j )
    {
        for ( int k = 0; k < iNumPoints; ++k )
        {
            //for ( int l = 0; l < iNumPoints; ++l )
            //{
                vFieldPos[ i ].x = ( -_dSize + _pCentral.x ) + ( j * dStep );
                vFieldPos[ i ].y = ( -_dSize + _pCentral.y ) + ( k * dStep );
                vFieldPos[ i ].z = _pCentral.z;
                vFieldAccel[ i ] = CalcAcceleration( vFieldPos[ i ] );
                vFieldLength[ i ] = vFieldAccel[ i ].Length( );
                //if ( dLen[ i ] == std::numeric_limits<double>::infinity( ) )
                //{
                //    dLen[ i ] = DBL_MAX;
                //}
                //if ( dLen[ i ] > dMax )
                //{
                //    dMax = dLen[ i ];
                //}
                //if ( dLen[ i ] < dMin )
                //{
                //    dMin = dLen[ i ];
                //}
                //dMean = ( dMean * i + dLen[ i ] ) / ( i + 1 );
                ++i;
            //}
        }
    }

    //double dNumPoints = i;
    //double dVar = 0.0;

    //for ( --i; i >= 0; --i )
    //{
    //    dVar += ( dLen[ i ] - dMean ) * ( dLen[ i ] - dMean ) / dNumPoints;
    //}
    //double dStdDev = sqrt( dVar );

    //dMin = dMean - dStdDev;
    //dMax = dMean + dStdDev;

    //if ( dMin < 0.0 )
    //{
        //dMin = dMin;
    //}

    //dMin = dMin >= 0 ? dMin : 0.0;

    DrawGravityField( _pCentral, _dSize, _dMult );
}

double CSimulator::CollisionTime( void )
{
    double dMinTime = DBL_MAX;
    for ( int i = (int)objMassObjects.size( ) - 1; i > 0 ; --i )
    {
        for ( int j = i - 1; j >= 0; --j )
        {
            double t1 = objMassObjects[ i ].CheckCollisionSimple( objMassObjects[ j ] );
            if ( ( t1 != 0.0 ) && ( t1 < dMinTime ) )
            {
                dMinTime = t1;
            }
        }
    }

    return dMinTime != DBL_MAX ? dMinTime : 0.0;
}

void CSimulator::MoveBodies( double _dTime )
{
    if ( _dTime == 0.0 )
    {
        return;
    }
    else if ( _dTime > 1.0 )
    {
        _dTime = 1.0;
    }

    if ( !bPause )
    {
        dTimeReal += _dTime;
        dTimeHistory += _dTime;

        if ( objMassObjects.size( ) > 0 )
        {
            double dTimeMul = dTimeConstant * _dTime;

            double dCollisionTime = CollisionTime( );
            if ( dCollisionTime > 0 )
            {
                if ( dTimeMul > ( dCollisionTime * 0.1 ) )
                {
                    dTimeMul = dCollisionTime * 0.1;
                }
                if ( dTimeMul < 0.5 )
                {
                    dTimeMul = 0.5;
                }
            }
            else if ( dCollisionTime < 0 )
            {
                //dTimeMul = dCollisionTime;
            }
            dTimeSimulation += dTimeMul;
            //dNumIterations = 1;

            double dKError = dTimeMul / dNumIterations;
            int uIterations = (int)( dNumIterations );
            double dOneOverLengthSq;

            PRECISIONVECTOR vTempVec;
            for ( ; uIterations > 0; --uIterations )
            {
                ProcessColision( );
                for ( int i = (int)objMassObjects.size( ) - 1; i > 0 ; --i )
                {
                    for ( int j = i - 1; j >= 0; --j )
                    {
                        vTempVec = objMassObjects[ j ].vPosition;
                        vTempVec -= objMassObjects[ i ].vPosition;
                        dOneOverLengthSq = dKError / vTempVec.LengthSquared( );

                        vTempVec.Normalize( );

                        objMassObjects[ i ].vVelocity += vTempVec * ( objMassObjects[ j ].dMassG * dOneOverLengthSq );
                        objMassObjects[ j ].vVelocity -= vTempVec * ( objMassObjects[ i ].dMassG * dOneOverLengthSq );
                    }
                }
                for ( int i = (int)objMassObjects.size( ) - 1; i >= 0 ; --i )
                {
                    vTempVec = objMassObjects[ i ].vPosition;
                    objMassObjects[ i ].vPosition += objMassObjects[ i ].vVelocity * dKError;
                    if ( ( dTimeHistory > dCTETimeHistory ) &&
                         ( objMassObjects[ i ].vPosition != vTempVec ) )
                    {
                        objMassObjects[ i ].SaveHistory( );
                    }
                }
                if ( dTimeHistory > dCTETimeHistory )
                {
                    dTimeHistory = 0.0;
                }
            }
        }
    }
}

void CSimulator::MoveBodiesTest( double _dTime )
{
    if ( _dTime == 0.0 )
    {
        return;
    }
    else if ( _dTime > 1.0 )
    {
        _dTime = 1.0;
    }

    if ( !bPause )
    {
        dTimeReal += _dTime;
        dTimeHistory += _dTime;

        if ( objMassObjects.size( ) > 0 )
        {
            double dTimeMul = dTimeConstant * _dTime;

            Move( dTimeMul );
        }
    }
}

void acceleration( ObjectArray & objArray )
{
    PRECISIONVECTOR vTempVec;

    double dOneOverLengthSq;

    for ( int i = (int)objArray.size( ) - 1; i >= 0; --i )
    {
        objArray[ i ].vAcceleration.Clear( );
    }

    for ( int i = (int)objArray.size( ) - 1; i > 0; --i )
    {
        for ( int j = i - 1; j >= 0; --j )
        {
            vTempVec = objArray[ j ].vPosTemp - objArray[ i ].vPosTemp;
            dOneOverLengthSq = 1.0 / vTempVec.LengthSquared( );

            vTempVec.Normalize( );

            objArray[ i ].vAcceleration += vTempVec * objArray[ j ].dMassG * dOneOverLengthSq;
            objArray[ j ].vAcceleration -= vTempVec * objArray[ i ].dMassG * dOneOverLengthSq;
        }
    }
}

Derivative evaluate( const State &initial, double /* t */, double dt, const Derivative &d, PRECISIONVECTOR & vAccel )
{
    State state;
    state.x = initial.x + d.dx * dt;
    state.v = initial.v + d.dv * dt;

    Derivative output;
    output.dx = state.v;
    output.dv = vAccel;//acceleration( state, t + dt, objArray );

    return output;
}

Derivative evaluate( const State &initial, double /* t */, PRECISIONVECTOR & vAccel )
{
    Derivative output;
    output.dx = initial.v;
    output.dv = vAccel;//acceleration( initial, t, objArray );

    return output;
}

void CSimulator::Move( double _dTime )
{
    PRECISIONVECTOR dxdt;
    PRECISIONVECTOR dvdt;

    double t = dTimeSimulation;
    double dt = _dTime / dNumIterations;
    int uIterations = (int)( dNumIterations );

    for ( ; uIterations > 0; --uIterations )
    {
        for ( int i = (int)objMassObjects.size( ) - 1; i >= 0; --i )
        {
            pState[ i ].x = objMassObjects[ i ].vPosition;
            pState[ i ].v = objMassObjects[ i ].vVelocity;

            objMassObjects[ i ].vPosTemp = objMassObjects[ i ].vPosition;
            objMassObjects[ i ].vVelTemp = objMassObjects[ i ].vVelocity;
        }

        acceleration( objMassObjects );
        for ( int i = (int)objMassObjects.size( ) - 1; i >= 0; --i )
        {
            pA[ i ] = evaluate( pState[ i ], t, objMassObjects[ i ].vAcceleration );

            objMassObjects[ i ].vPosTemp += pA[ i ].dx * dt / 6.0;
            objMassObjects[ i ].vVelTemp += pA[ i ].dv * dt / 6.0;
        }

        acceleration( objMassObjects );
        for ( int i = (int)objMassObjects.size( ) - 1; i >= 0; --i )
        {
            pB[ i ] = evaluate( pState[ i ], t, dt * 0.5, pA[ i ], objMassObjects[ i ].vAcceleration );

            objMassObjects[ i ].vPosTemp += pB[ i ].dx * dt / 3.0;
            objMassObjects[ i ].vVelTemp += pB[ i ].dv * dt / 3.0;
        }

        acceleration( objMassObjects );
        for ( int i = (int)objMassObjects.size( ) - 1; i >= 0; --i )
        {
            pC[ i ] = evaluate( pState[ i ], t, dt * 0.5, pB[ i ], objMassObjects[ i ].vAcceleration );

            objMassObjects[ i ].vPosTemp += pC[ i ].dx * dt / 3.0;
            objMassObjects[ i ].vVelTemp += pC[ i ].dv * dt / 3.0;
        }

        acceleration( objMassObjects );
        for ( int i = (int)objMassObjects.size( ) - 1; i >= 0; --i )
        {
            pD[ i ] = evaluate( pState[ i ], t, dt, pC[ i ], objMassObjects[ i ].vAcceleration );

            objMassObjects[ i ].vPosTemp += pD[ i ].dx * dt / 6.0;
            objMassObjects[ i ].vVelTemp += pD[ i ].dv * dt / 6.0;
        }

        for ( int i = (int)objMassObjects.size( ) - 1; i >= 0; --i )
        {
            dxdt = ( pA[ i ].dx + ( pB[ i ].dx + pC[ i ].dx ) * 2.0 + pD[ i ].dx ) * ( 1.0 / 6.0 );
            dvdt = ( pA[ i ].dv + ( pB[ i ].dv + pC[ i ].dv ) * 2.0 + pD[ i ].dv ) * ( 1.0 / 6.0 );

            pState[ i ].x = pState[ i ].x + dxdt * dt;
            pState[ i ].v = pState[ i ].v + dvdt * dt;

            PRECISIONVECTOR vTempVec = objMassObjects[ i ].vPosition;

            objMassObjects[ i ].vPosition = pState[ i ].x;
            objMassObjects[ i ].vVelocity = pState[ i ].v;

            if ( ( dTimeHistory > dCTETimeHistory ) &&
                 ( objMassObjects[ i ].vPosition != vTempVec ) )
            {
                objMassObjects[ i ].SaveHistory( );
            }
        }
        if ( dTimeHistory > dCTETimeHistory )
        {
            dTimeHistory = 0.0;
        }
    }
    dTimeSimulation += _dTime;
}

double CSimulator::GetTimeReal( void )
{
    return dTimeReal;
}

void CSimulator::TimeLoad( CFileLoader & _fl )
{
    int day = atoi( _fl.GetVariableValue( "day" ).c_str( ) );
    int month = atoi( _fl.GetVariableValue( "month" ).c_str( ) );
    int year = atoi( _fl.GetVariableValue( "year" ).c_str( ) );
    int hour = atoi( _fl.GetVariableValue( "hour" ).c_str( ) );
    int min = atoi( _fl.GetVariableValue( "minute" ).c_str( ) );
    int sec = atoi( _fl.GetVariableValue( "second" ).c_str( ) );

    TimeSet( day, month, year,
             hour, min, sec );
}

void CSimulator::TimeSet( int _day, int _month, int _year,
                          int _hour, int _min, int _sec )
{
    //CTime myTime( _year, _month, _day, _hour, _min, _sec );
    
    struct tm myTime;
    myTime.tm_mday = _day;
    myTime.tm_mon = _month - 1;
    myTime.tm_year = _year - 1900;
    myTime.tm_hour = _hour;
    myTime.tm_min = _min;
    myTime.tm_sec = _sec;
    myTime.tm_wday = 0;
    myTime.tm_yday = 0;
    myTime.tm_isdst = 0;

    dTimeSimulation = (double)_mktime64( &myTime );
    //dTimeSimulation = (double)myTime.GetTime( );
}

string CSimulator::SimulatedTimeFormat( void )
{
    double dTime = dTimeSimulation;    
    
    __time64_t time64 = (__time64_t)dTime;
    struct tm * myTime = _gmtime64( &time64 );

    char buffer[ 256 ];
    strftime( buffer, sizeof( buffer ) - 1, "%d-%b-%Y %H:%M:%S", myTime );

    return string( buffer );
    //return (string)CTime( (__time64_t) dTime ).Format( "%d-%b-%Y %H:%M:%S" );
}

void CSimulator::GetBoundingBox( double & _dXMin, double & _dYMin, double & _dZMin,
                                 double & _dXMax, double & _dYMax, double & _dZMax )
{
    _dXMin = _dYMin = _dZMin = 0.0;
    _dXMax = _dYMax = _dZMax = 0.0;

    for ( int i = 0; i < GetNumObjects( ); i++ )
    {
        if ( GetObject( i ).vPosition.x > _dXMax )
        {
            _dXMax = GetObject( i ).vPosition.x;
        }
        if ( GetObject( i ).vPosition.y > _dYMax )
        {
            _dYMax = GetObject( i ).vPosition.y;
        }
        if ( GetObject( i ).vPosition.z > _dZMax )
        {
            _dZMax = GetObject( i ).vPosition.z;
        }

        if ( GetObject( i ).vPosition.x < _dXMin )
        {
            _dXMin = GetObject( i ).vPosition.x;
        }
        if ( GetObject( i ).vPosition.y < _dYMin )
        {
            _dYMin = GetObject( i ).vPosition.y;
        }
        if ( GetObject( i ).vPosition.z < _dZMin )
        {
            _dZMin = GetObject( i ).vPosition.z;
        }
    }
}

bool CSimulator::IsPaused( void )
{
    return bPause;
}

//void * __cdecl ProcessThread( void * parameter );
//void * __cdecl ProcessThreadNoDelay( void * parameter );

void CSimulator::Run( void )
{
/*
    if ( thread.p != NULL )
    {
        return;
    }

    int rc;

    data.dCTETimeHistory = &dCTETimeHistory;
    data.dNumIterations = &dNumIterations;
    data.dTimeConstant = &dTimeConstant ;
    data.dTimeHistory = &dTimeHistory;
    data.dTimeReal = &dTimeReal;
    data.dTimeSimulation = &dTimeSimulation;
    data.objMassObjects = &objMassObjects;
    data.quit = false;
    data.pause = &bPause;

    rc = pthread_create( &thread, NULL, ProcessThread, (void *)&data );
    if ( rc )
    {
//        printf( "ERROR; return code from pthread_create() is %d\n", rc );
        exit( -1 );
    }
*/
}

void CSimulator::Stop( void )
{
/*
    if ( thread.p != NULL )
    {
        data.quit = true;
        void * retValue;
        pthread_join( thread, &retValue );
        thread.p = NULL;

        bPause = false;
    }
*/
}

void CSimulator::Pause( bool _bPause )
{
    bPause = _bPause;
}

/*
struct stSimulatorData
{
    vector<CMassObject> * objMassObjects;

	double * dTimeReal;
    double * dTimeSimulation;
    double * dTimeHistory;
    double * dTimeConstant;
	double * dNumIterations;
    double * dCTETimeHistory;

    bool quit;
    bool * pause;
};

void * __cdecl ProcessThread( void * parameter )
{
    static const double dMiliDivision = 1.0 / 1000.0;

    stSimulatorData * data = (stSimulatorData *)parameter;

    double dTime = 0.0;
    double dTimeOld = GetTickCount( ) * dMiliDivision;
    PRECISIONVECTOR vTempVec;

    int iNumObjects = (int)data->objMassObjects->size( );

    while ( ! data->quit )
    {
        if ( !*data->pause )
        {
            dTime = ( GetTickCount( ) * dMiliDivision ) - dTimeOld;
            dTimeOld = GetTickCount( ) * dMiliDivision;

            *data->dTimeReal += dTime;
            *data->dTimeHistory += dTime;

            double dKError = *data->dTimeConstant * dTime / *data->dNumIterations;

            int uIterations = (int)( *data->dNumIterations );
            for ( ; uIterations > 0; --uIterations )
            {
                for ( int i = iNumObjects - 1; i > 0 ; --i )
                {
                    for ( int j = i - 1; j >= 0; --j )
                    {
                        vTempVec = (*data->objMassObjects)[ j ].vPosition - (*data->objMassObjects)[ i ].vPosition;
                        double dOneOverLengthSq = dKError / vTempVec.LengthSquared( );

                        vTempVec.Normalize( );

                        (*data->objMassObjects)[ i ].vVelocity += vTempVec * (*data->objMassObjects)[ j ].dMassG * dOneOverLengthSq;
                        (*data->objMassObjects)[ j ].vVelocity -= vTempVec * (*data->objMassObjects)[ i ].dMassG * dOneOverLengthSq;
                    }
                    
                    vTempVec = (*data->objMassObjects)[ i ].vPosition;
                    (*data->objMassObjects)[ i ].vPosition += (*data->objMassObjects)[ i ].vVelocity * dKError;
                    if ( ( *data->dTimeHistory > *data->dCTETimeHistory ) &&
                         ( (*data->objMassObjects)[ i ].vPosition != vTempVec ) )
                    {
                        (*data->objMassObjects)[ i ].SaveHistory( );
                    }
                    
                }
                vTempVec = (*data->objMassObjects)[ 0 ].vPosition;
                (*data->objMassObjects)[ 0 ].vPosition += (*data->objMassObjects)[ 0 ].vVelocity * dKError;
                if ( ( *data->dTimeHistory > *data->dCTETimeHistory ) &&
                     ( (*data->objMassObjects)[ 0 ].vPosition != vTempVec ) )
                {
                    (*data->objMassObjects)[ 0 ].SaveHistory( );
                }
            }

            if ( *data->dTimeHistory > *data->dCTETimeHistory )
            {
                *data->dTimeHistory = 0.0;
            }
        }
        else
        {
            Sleep( 200 );
        }
    }

    pthread_exit( NULL );

    return NULL;
}


void * __cdecl ProcessThreadNoDelay( void * parameter )
{
    static const double dMiliDivision = 1.0 / 1000.0;
    double dTimeOld = GetTickCount( ) * dMiliDivision;

    stSimulatorData * data = (stSimulatorData *)parameter;

    PRECISIONVECTOR vTempVec;

    *data->dTimeHistory = 0;
    int iNumObjects = (int)data->objMassObjects->size( );

    int uIterations = (int)( *data->dNumIterations );
    double dKError = *data->dTimeConstant / *data->dNumIterations;
    double dOneOverLengthSq;

    while ( ! data->quit )
    {
        ++*data->dTimeReal;

        for ( ; uIterations > 0; --uIterations )
        {
            for ( int i = iNumObjects - 1; i > 0 ; --i )
            {
                for ( int j = i - 1; j >= 0; --j )
                {
                    vTempVec = (*data->objMassObjects)[ j ].vPosition - (*data->objMassObjects)[ i ].vPosition;
                    dOneOverLengthSq = dKError / vTempVec.LengthSquared( );

                    vTempVec.Normalize( );

                    (*data->objMassObjects)[ i ].vVelocity += vTempVec * (*data->objMassObjects)[ j ].dMassG * dOneOverLengthSq;
                    (*data->objMassObjects)[ j ].vVelocity -= vTempVec * (*data->objMassObjects)[ i ].dMassG * dOneOverLengthSq;
                }
                
                (*data->objMassObjects)[ i ].vPosition += (*data->objMassObjects)[ i ].vVelocity * dKError;
                if ( *data->dTimeHistory > *data->dCTETimeHistory )
                {
                    (*data->objMassObjects)[ i ].SaveHistory( );
                    dTimeOld = GetTickCount( ) * dMiliDivision;
                }
                
            }
            (*data->objMassObjects)[ 0 ].vPosition += (*data->objMassObjects)[ 0 ].vVelocity * dKError;
            if ( *data->dTimeHistory > *data->dCTETimeHistory )
            {
                (*data->objMassObjects)[ 0 ].SaveHistory( );
                *data->dTimeHistory = 0;
            }
            *data->dTimeHistory += ( GetTickCount( ) * dMiliDivision ) - dTimeOld;
            dTimeOld = GetTickCount( ) * dMiliDivision;
        }

    }

    pthread_exit( NULL );

    return NULL;
}
*/
/*
Old Functions for reference

PRECISIONVECTOR CSimulator::ForceFunction( CMassObject &obj1, CMassObject &obj2 )
{
    PRECISIONVECTOR vDir = obj2.vPosition - obj1.vPosition;

    double F = obj2.dMassG / vDir.LengthSquared( ); // Kg m / s-2

    vDir.Normalize( );
    vDir *= F;

    return vDir;
}

void CSimulator::MoveBodies( double _dTime )
{
    if ( _dTime == 0.0 )
    {
        return;
    }

    dTimeReal += _dTime;
    dTimeHistory += _dTime;

    if ( objMassObjects.size( ) > 0 )
    {
        double dTimeMul = dTimeConstant * _dTime;

        double dKError = dTimeMul / dNumIterations;
        int uIterations = (int)( dNumIterations );

        PRECISIONVECTOR vTempVec;
        for ( ; uIterations > 0; --uIterations )
        {
            for ( unsigned int i = 0; i < objMassObjects.size( ); ++i )
            {
                for ( unsigned int j = 0; j < objMassObjects.size( ); ++j )
                {
                    if ( i != j )
                    {
                        objMassObjects[ i ].vVelocity += ForceFunction( objMassObjects[ i ], objMassObjects[ j ] ) * dKError;
                    }
                }
            }
            for ( unsigned int i = 0; i < objMassObjects.size( ); ++i )
            {
                vTempVec = objMassObjects[ i ].vPosition;
                objMassObjects[ i ].vPosition += objMassObjects[ i ].vVelocity * dKError;//dTimeMul;
                if ( ( dTimeHistory > dCTETimeHistory ) &&
                     ( objMassObjects[ i ].vPosition != vTempVec ) )
                {
                    objMassObjects[ i ].SaveHistory( );
                }
            }
        }
    }

    if ( dTimeHistory > dCTETimeHistory )
    {
        dTimeHistory = 0.0;
    }
}

*/