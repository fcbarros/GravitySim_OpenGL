
#ifndef _CSpeedMath_h_
#define _CSpeedMath_h_

class CSpeedMath
{
public:

    // _dAngle = degree
    static double CosTable( double _dAngle );

    // Result in Degree
    static double ACosTableD( double _dVal );

    // Result in Rad
    static double ACosTableR( double _dVal );

    // _dAngle = degree
    static double SinTable( double _dAngle );

private:

    // This will be the number of divisions an angle will have in the look-up table
    static const int iTableIndexMultipler = 200;
    // Size of the tables below
    static const int iTableSize = 360 * iTableIndexMultipler;

};

#endif
