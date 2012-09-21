
#ifndef _TSingleton_h_
#define _TSingleton_h_

#include <cassert>

#pragma warning ( push )

#pragma warning ( disable : 4311 4312 )

template <typename T> class Singleton
{
    static T* ms_Singleton;

public:

    Singleton( void )
    {
        assert( !ms_Singleton );
        int offset = (int)(T*)1 - (int)(Singleton <T>*)(T*)1;
        ms_Singleton = (T*)((int)this + offset);
    }
    ~Singleton( void )
    {
        assert( ms_Singleton );
        ms_Singleton = 0;
    }

    static T& GetSingleton( void )
    {
        assert( ms_Singleton );
        return ( *ms_Singleton );
    }
    static T* GetSingletonPtr( void )
    {
        return ( ms_Singleton );
    }
};

template <typename T> T* Singleton<T>::ms_Singleton = 0;

#pragma warning ( pop )


#endif
