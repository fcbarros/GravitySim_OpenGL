// Thanks to Ben Humphrey at Game Tutorials for his OpenGL image loading code. 
//Regards
//Ronny André Reierstad
//www.morrowland.com
//apron@morrowland.com

#include "CTexture.h"
#include "Utils/jpg/include/jpeglib.h"

typedef unsigned char byte;


CTexture::CTexture( void )
{
    texture = 0;
}

CTexture::~CTexture( void )
{
    Clear( );
}

void CTexture::Clear( void )
{
    if ( texture != 0 )
    {
        glDeleteTextures( 1, &texture );
        texture = 0;
    }
}

void CTexture::Select( void ) const
{
    if ( texture != 0 )
    {
    	glBindTexture( GL_TEXTURE_2D, texture );
    }
}

unsigned int CTexture::GetHeight( void )
{
    return ( texture != 0 ) ? uHeight : 0;
}

unsigned int CTexture::GetWidth( void )
{
    return ( texture != 0 ) ? uWidth : 0;
}

bool CTexture::IsLoaded( void )
{
    return texture != 0;
}

bool CTexture::LoadFromFile( string stFileName  )
{
    if ( stFileName.empty( ) )
	{
		return false;
	}
    Clear( );

	tImage *pImage = NULL;
	// Load TGA Image
    if ( strstr( stFileName.c_str( ), ".tga" ) )
	{
		pImage = LoadTGA( stFileName );
	}
	// Load JPG Image
	else if( strstr( stFileName.c_str( ), ".jpg" ) )
	{
		pImage = LoadJPG( stFileName );
	}
	// Load BMP Image
	else if( strstr( stFileName.c_str( ), ".bmp" ) )
	{
		pImage = LoadBMP( stFileName );
	}
	else
	{
		return false;
	}
	
	if ( pImage == NULL )
	{
		return false;
	}
	
	glGenTextures( 1, &texture );
	
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	
	glBindTexture( GL_TEXTURE_2D, texture );
	
	int textureType = GL_RGB;
	
	if ( pImage->channels == 4 )
	{
		textureType = GL_RGBA;
	}

	uWidth = pImage->sizeX;
	uHeight = pImage->sizeY;
	
	gluBuild2DMipmaps( GL_TEXTURE_2D, pImage->channels, pImage->sizeX, 
					   pImage->sizeY, textureType, GL_UNSIGNED_BYTE, pImage->data );
	
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );							
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
	
	if ( pImage )	
	{
		if ( pImage->data )
		{
            delete[] pImage->data;
			//free( pImage->data );
		}
		delete pImage;
	}
	
	return true;
}


/////////////////////////////////////////////////////////////////////////////////////////////////
//										BMP LOADER
/////////////////////////////////////////////////////////////////////////////////////////////////
CTexture::tImage * CTexture::LoadBMP( string stFileName )
{
	AUX_RGBImageRec *pBitmap = NULL;
	FILE *pFile = NULL;
	
    if ( fopen_s( &pFile, stFileName.c_str( ), "rb" ) != 0 ) 
	{
        string stMsg = "Can't load BMP image: " + stFileName;
        MessageBox( NULL, stMsg.c_str( ), "ERROR", MB_OK | MB_ICONINFORMATION );
		return NULL;
	}
	
    pBitmap = auxDIBImageLoad( stFileName.c_str( ) );
	
	tImage *pImage = new tImage( );
	
	pImage->channels = 3;
	pImage->sizeX = pBitmap->sizeX;
	pImage->sizeY = pBitmap->sizeY;
    // Maybe compying the data instead of pointer assignment because of new/malloc delete[]/free
	pImage->data  = pBitmap->data;
	
	free( pBitmap );
	
	return pImage;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//										TGA LOADER
/////////////////////////////////////////////////////////////////////////////////////////////////
CTexture::tImage * CTexture::LoadTGA( string stFileName )
{
	tImage *pImageData = NULL;
	WORD width = 0, 
		 height = 0;	
	byte length = 0;				
	byte imageType = 0;					
	byte bits = 0;						
	FILE *pFile = NULL;					
	int channels = 0;					
	int stride = 0;					
	int i = 0;						
	
    if ( fopen_s( &pFile, stFileName.c_str( ), "rb" ) != 0 ) 
	{
        string stMsg = "Can't load TGA image: " + stFileName;
        MessageBox( NULL, stMsg.c_str( ), "ERROR", MB_OK | MB_ICONINFORMATION );
		return NULL;
	}
	
	pImageData = new tImage( );
	
	fread( &length, sizeof( byte ), 1, pFile );
	
	fseek( pFile, 1, SEEK_CUR ); 
	
	fread( &imageType, sizeof( byte ), 1, pFile );
	
	fseek( pFile, 9, SEEK_CUR ); 
	
	fread( &width,  sizeof( WORD ), 1, pFile );
	fread( &height, sizeof( WORD ), 1, pFile );
	fread( &bits,   sizeof( byte ), 1, pFile );
	
	fseek( pFile, length + 1, SEEK_CUR ); 
	
	if( imageType != TGA_RLE )
	{
		if ( ( bits == 24 ) || ( bits == 32 ) )
		{
			channels = bits / 8;
			stride = channels * width;
			pImageData->data = new unsigned char[ stride * height ];
			
			for ( int y = 0; y < height; y++ )
			{
				unsigned char *pLine = &( pImageData->data[ stride * y ] );
				
				fread( pLine, stride, 1, pFile );
				
				for ( i = 0; i < stride; i += channels )
				{
					unsigned char temp  = pLine[ i ];
					pLine[ i ]          = pLine[ i + 2 ];
					pLine[ i + 2 ]      = temp;
				}
			}
		}
		else if( bits == 16 )
		{
			unsigned short pixels = 0;
			int r = 0, 
				g = 0, 
				b = 0;
			
			channels = 3;
			stride = channels * width;
			pImageData->data = new unsigned char[ stride * height ];
			
			for ( int i = 0; i < width*height; i++ )
			{
				fread( &pixels, sizeof( unsigned short ), 1, pFile );
				
				b = ( pixels & 0x1f ) << 3;
				g = ( ( pixels >>  5 ) & 0x1f ) << 3;
				r = ( ( pixels >> 10 ) & 0x1f ) << 3;
				
				pImageData->data[ i * 3 + 0 ] = (unsigned char)r;
				pImageData->data[ i * 3 + 1 ] = (unsigned char)g;
				pImageData->data[ i * 3 + 2 ] = (unsigned char)b;
			}
		}	
		else
		{
			return NULL;
		}
	}
	else
	{
		byte rleID = 0;
		int colorsRead = 0;
		channels = bits / 8;
		stride = channels * width;
		
		pImageData->data = new unsigned char[ stride * height ];

        byte * pColors = new byte[ channels ];
		
		while ( i < ( width * height ) )
		{
			fread( &rleID, sizeof( byte ), 1, pFile );
			
			if ( rleID < 128 )
			{
				rleID++;
				
				while( rleID )
				{
					fread( pColors, sizeof( byte ) * channels, 1, pFile );
					
					pImageData->data[ colorsRead + 0 ] = pColors[ 2 ];
					pImageData->data[ colorsRead + 1 ] = pColors[ 1 ];
					pImageData->data[ colorsRead + 2 ] = pColors[ 0 ];
					
					if ( bits == 32 )
					{
						pImageData->data[ colorsRead + 3 ] = pColors[ 3 ];
					}
					
					i++;
					rleID--;
					colorsRead += channels;
				}
			}
			else
			{
				rleID -= 127;
				
				fread( pColors, sizeof( byte ) * channels, 1, pFile );
				
				while ( rleID )
				{
					pImageData->data[ colorsRead + 0 ] = pColors[ 2 ];
					pImageData->data[ colorsRead + 1 ] = pColors[ 1 ];
					pImageData->data[ colorsRead + 2 ] = pColors[ 0 ];
					
					if ( bits == 32 )
					{
						pImageData->data[ colorsRead + 3 ] = pColors[ 3 ];
					}
					
					i++;
					rleID--;
					colorsRead += channels;
				}
			}
		}
        delete[] pColors;
	}

	fclose( pFile );
	
	pImageData->channels = channels;
	pImageData->sizeX    = width;
	pImageData->sizeY    = height;
	
	return pImageData;
}

CTexture::tImage * CTexture::LoadJPG( string stFileName  )
{
	struct jpeg_decompress_struct cinfo;
	tImage *pImageData = NULL;
	FILE *pFile;
	
    if ( fopen_s( &pFile, stFileName.c_str( ), "rb" ) != 0 ) 
    //if ( ( pFile = fopen( stFileName.c_str( ), "rb" ) ) == NULL ) 
	{
        string stMsg = "Can't load JPG image: " + stFileName;
        MessageBox( NULL, stMsg.c_str( ), "ERROR", MB_OK | MB_ICONINFORMATION );
		return NULL;
	}
	
	jpeg_error_mgr jerr;
	
	cinfo.err = jpeg_std_error( &jerr );
	
	jpeg_create_decompress( &cinfo );
	
	jpeg_stdio_src( &cinfo, pFile );
	
	pImageData = (tImage*)malloc( sizeof( tImage ) );
	
	jpeg_read_header( &cinfo, TRUE );
	
	jpeg_start_decompress( &cinfo );
	
	pImageData->channels = cinfo.num_components;
	pImageData->sizeX    = cinfo.image_width;
	pImageData->sizeY    = cinfo.image_height;
	
	int rowSpan = cinfo.image_width * cinfo.num_components;
	
	pImageData->data = ((unsigned char*)malloc(sizeof(unsigned char)*rowSpan*pImageData->sizeY));
	
	unsigned char** rowPtr = new unsigned char*[pImageData->sizeY];
	
	for ( int i = 0; i < pImageData->sizeY; i++ )
    {
		rowPtr[i] = &( pImageData->data[ i * rowSpan ] );
    }
	
	int rowsRead = cinfo.output_height - 1;
	while ( cinfo.output_scanline < cinfo.output_height ) 
	{
		rowsRead -= jpeg_read_scanlines( &cinfo, &rowPtr[rowsRead], cinfo.output_height - rowsRead );
	}

	delete[] rowPtr;
	
	jpeg_finish_decompress( &cinfo );
	
	jpeg_destroy_decompress( &cinfo );
	
	fclose( pFile );
	
	return pImageData;
}
