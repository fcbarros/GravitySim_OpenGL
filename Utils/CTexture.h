
#ifndef _CTEXTURE_H_
#define _CTEXTURE_H_

#include "GeneralIncludes.h"

class CTexture
{
public:
	CTexture( void );
	~CTexture( void );
    void Clear( void );
	bool LoadFromFile( string stFileName );
	void Select( void ) const;
    bool IsLoaded( void );
	unsigned int GetHeight( void );
	unsigned int GetWidth( void );

private:

    struct tImage
    {
	    int channels;
	    int sizeX;
	    int sizeY;
	    unsigned char *data;
    };
    enum
    {
        TGA_RGB = 2,
        TGA_A = 3,
        TGA_RLE = 10
    };


	tImage *LoadBMP( string stFileName  );
	tImage *LoadTGA( string stFileName  );
	tImage *LoadJPG( string stFileName  );

	unsigned int texture;

	unsigned int uWidth;
	unsigned int uHeight;
};

#endif