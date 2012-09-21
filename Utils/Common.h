
#ifndef _Common_h_
#define _Common_h_

union stColor
{
    struct
    {
	    float r;
	    float g;
	    float b;
	    float a;
    };
    float v[4];
};

#endif
