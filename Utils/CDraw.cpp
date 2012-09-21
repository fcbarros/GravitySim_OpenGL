//
//#include "CDraw.h"
//
//void CDraw::DrawField2DDepth( double hMin, double hMax, double dMult )
//{
//    hMax /= dMult;
//
//    glBegin( GL_LINES );
//
//    for ( int k = (int)vLinePair.size( ) - 1; k >= 0; --k )
//    {
//        double z = ( vFieldAccel[ vLinePair[ k ].i1 ].Length( ) - hMin ) / hMax;
//        double dNorm = z / dMult;
//        dNorm = dNorm <= 1.0 ? dNorm : 1.0;
//        dNorm = dNorm >= 0.0 ? dNorm : 0.0;
//        unsigned int uColor = (unsigned int)( dNorm * ( ColorMap.GetSize( ) - 1 ) );
//        glColor4fv( ColorMap[ uColor ].v );
//        PRECISIONVECTOR vTemp = vFieldPos[ vLinePair[ k ].i1 ];
//        vTemp.z -= z;
//        glVertex3dv( vTemp.v );
//
//        z = ( vFieldAccel[ vLinePair[ k ].i2 ].Length( ) - hMin ) / hMax;
//        dNorm = z / dMult;
//        dNorm = dNorm <= 1.0 ? dNorm : 1.0;
//        dNorm = dNorm >= 0.0 ? dNorm : 0.0;
//        uColor = (unsigned int)( dNorm * ( ColorMap.GetSize( ) - 1 ) );
//        glColor4fv( ColorMap[ uColor ].v );
//        vTemp = vFieldPos[ vLinePair[ k ].i2 ];
//        vTemp.z -= z;
//        glVertex3dv( vTemp.v );
//    }
//
//    glEnd( );
//}
//
//void CDraw::DrawField2DColors( double hMin, double hMax, double dMult )
//{
//    //hMax /= dMult;
//
//    glDisable( GL_CULL_FACE );
//
//    glBegin( GL_TRIANGLE_STRIP );
//
//    for ( int k = (int)vTriangleStrip.size( ) - 1; k >= 0; --k )
//    {
//        double z = ( vFieldLength[ vTriangleStrip[ k ] ] - hMin ) / hMax;
//        double dNorm = z * dMult;
//        dNorm = dNorm <= 1.0 ? dNorm : 1.0;
//        dNorm = dNorm >= 0.0 ? dNorm : 0.0;
//        unsigned int uColor = (unsigned int)( dNorm * ( ColorMap.GetSize( ) - 1 ) );
//        glColor4fv( ColorMap[ uColor ].v );
//        PRECISIONVECTOR vTemp = vFieldPos[ vTriangleStrip[ k ] ];
//        vTemp.z -= z;
//        glVertex3dv( vTemp.v );
//    }
//
//    glEnd( );
//
//    glEnable( GL_CULL_FACE );
//
//}
//
//void CDraw::DrawField2DVectors( double _dSize, double _dMult )
//{
//    double dX = 2.0 * _dSize * 0.333 / (double)iNumPoints;
//    for ( int i = ( iNumPoints * iNumPoints ) - 1; i >= 0; --i )
//    {
//        PRECISIONVECTOR vTemp = vFieldAccel[ i ];
//        unsigned int uColor = (unsigned int)( ( vFieldLength[ i ] * _dMult ) * ( ColorMap.GetSize( ) - 1 ) );
//        glColor4fv( ColorMap[ uColor ].v );
//
//        vTemp *= dX / vFieldLength[ i ];
//        vTemp += vFieldPos[ i ];
//        
//        g_Render.DrawArrow( vFieldPos[ i ], vTemp );
//    }
//}
//
//void CDraw::DrawGravityField( PRECISIONVECTOR _pCentral,
//                              double _dSize, double _dMult )
//{
//    glPushMatrix( );
//
//    glEnable( GL_LINE_SMOOTH );
//    glEnable( GL_BLEND );
//    glLineWidth( 1.5f );
//
//    //DrawGravitHistogram( dLen, 2.0 * _dInit, iNumPoints, _dMult );
//
//    switch ( m_FieldType )
//    {
//    case TYPE_2D_COLORS:
//        DrawField2DColors( 0.0, 1.0, _dMult );
//        break;
//
//    case TYPE_2D_DEPTH:
//        DrawField2DDepth( 0.0, 1.0, _dMult );
//        break;
//
//    case TYPE_2D_VECTOR:
//        DrawField2DVectors( _dSize, _dMult );
//        break;
//
//    case TYPE_3D_COLORS:
//        break;
//    }
//
//    glTranslated( _pCentral.x, _pCentral.y, _pCentral.z );
//    glColor4d( 1.0, 1.0, 1.0, 0.5 );
//    glBegin( GL_LINES );
//
//        glVertex3d( -_dSize, -_dSize, -_dSize );
//        glVertex3d( _dSize, -_dSize, -_dSize );
//
//        glVertex3d( _dSize, -_dSize, -_dSize );
//        glVertex3d( _dSize, _dSize, -_dSize );
//
//        glVertex3d( _dSize, _dSize, -_dSize );
//        glVertex3d( -_dSize, _dSize, -_dSize );
//
//        glVertex3d( -_dSize, _dSize, -_dSize );
//        glVertex3d( -_dSize, -_dSize, -_dSize );
//
//        glVertex3d( -_dSize, -_dSize, _dSize );
//        glVertex3d( _dSize, -_dSize, _dSize );
//
//        glVertex3d( _dSize, -_dSize, _dSize );
//        glVertex3d( _dSize, _dSize, _dSize );
//
//        glVertex3d( _dSize, _dSize, _dSize );
//        glVertex3d( -_dSize, _dSize, _dSize );
//
//        glVertex3d( -_dSize, _dSize, _dSize );
//        glVertex3d( -_dSize, -_dSize, _dSize );
//
//        glVertex3d( -_dSize, -_dSize, -_dSize );
//        glVertex3d( -_dSize, -_dSize, _dSize );
//
//        glVertex3d( _dSize, -_dSize, -_dSize );
//        glVertex3d( _dSize, -_dSize, _dSize );
//
//        glVertex3d( _dSize, _dSize, -_dSize );
//        glVertex3d( _dSize, _dSize, _dSize );
//
//        glVertex3d( -_dSize, _dSize, -_dSize );
//        glVertex3d( -_dSize, _dSize, _dSize );
//
//    glEnd( );
//
//    glLineWidth( 1.0f );
//    glDisable( GL_LINE_SMOOTH );
//    glDisable( GL_BLEND );
//
//    glPopMatrix( );
//}
//
