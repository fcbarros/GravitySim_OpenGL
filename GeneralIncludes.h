
#ifndef _GeneralIncludes_h_
#define _GeneralIncludes_h_


#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
// Windows Header Files:

#include <Windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// OPENGL
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glaux.h>		// Header File For The Glaux Library

#include <math.h>
#include <CFloat>
#include <vector>
#include <string>

using namespace std;

double ClockFrequency( void );

#endif
