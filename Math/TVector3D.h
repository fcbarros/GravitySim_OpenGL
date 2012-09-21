
#ifndef _TVector3D_H_
#define _TVector3D_H_

template <typename T> 
    class TVector3D
{
public:
	TVector3D( void )
        : x( (T)0.0 ), y( (T)0.0 ), z( (T)0.0 )
    {}

	TVector3D( const TVector3D & _v )
        : x( _v.x ), y( _v.y ), z( _v.z )
    {}

	TVector3D( const T _x, const T _y, const T _z )
        : x( _x ), y( _y ), z( _z ) 
    {}

    TVector3D & operator=( const TVector3D & _v1 )
    {
        x = _v1.x;
        y = _v1.y;
        z = _v1.z;

        return *this;
    }

    bool operator!=( const TVector3D & _v1 ) const
    {
        return ( x != _v1.x ) || ( y != _v1.y ) || ( z != _v1.z );
    }

    bool operator==( const TVector3D & _v1 ) const
    {
        return ( x == _v1.x ) && ( y == _v1.y ) && ( z == _v1.z );
    }

    inline TVector3D & operator+=( const TVector3D & _v1 )
    {
	    x += _v1.x;
	    y += _v1.y;
	    z += _v1.z;

	    return *this;
    }

	inline TVector3D & operator-=( const TVector3D & _v1 )
    {
	    x -= _v1.x;
	    y -= _v1.y;
	    z -= _v1.z;

	    return *this;
    }

    inline TVector3D & operator*=( const T _tScale )
    {
	    x *= _tScale;
	    y *= _tScale;
	    z *= _tScale;

	    return *this;
    }

    inline TVector3D & operator/=( const T _tScale )
    {
	    x /= _tScale;
	    y /= _tScale;
	    z /= _tScale;

	    return *this;
    }

    const TVector3D operator-( const TVector3D & _v1 ) const
    {
        return TVector3D( *this ) -= _v1;
    }

	const TVector3D operator+( const TVector3D & _v1 ) const
    {
        return TVector3D( *this ) += _v1;
    }

	const TVector3D operator*( const T _tScale ) const
    {
        return TVector3D( *this ) *= _tScale;
    }

	const TVector3D operator/( const T _tScale ) const
    {
        return TVector3D( *this ) /= _tScale;
    }

    inline T DotP( const TVector3D & _v1 ) const
    {
        return ( x * _v1.x ) + ( y * _v1.y ) + ( z * _v1.z );
    }

    inline TVector3D CrossP( const TVector3D & _v1 ) const
    {
       return TVector3D( y * _v1.z - z * _v1.y, 
                         z * _v1.x - x * _v1.z,
                         x * _v1.y - y * _v1.x );
    }

	inline T LengthSquared( void ) const
    {
	    return x * x + y * y + z * z;
    }

    inline T Length( void ) const
    {
        return sqrt( LengthSquared( ) );
    }

	void Normalize( void )
    {
	    T tOneOverSize = 1.0 / Length( );

	    x *= tOneOverSize;
	    y *= tOneOverSize;
	    z *= tOneOverSize;
    }

    void Clear( void )
    {
        x = y = z = (T)0.0;
    }

    static T DotProd( const TVector3D & _v1, const TVector3D & _v2 )
    {
        return TVector3D( _v1 ).DotP( _v2 );
    }

public:

    union
    {
        struct
        {
            T x;
            T y;
            T z;
        };
        T v[3];
    };
};

typedef TVector3D<double> PRECISIONVECTOR;

#endif
