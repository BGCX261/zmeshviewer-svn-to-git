#ifndef __PROGRESSBAR_H__
#define __PROGRESSBAR_H__

#include <iostream>

class ProgressBar
{
public:
	static void PrintProgress( unsigned int curr, unsigned int tar, unsigned int unit = 10 )
	{
//		static unsigned long p = 0;
//		static const unsigned char w[] = "\\/-";
		unsigned long val = (curr * 100) / tar;
// 		if(++p == 3)
// 		{
// 			p = 0;
// 		}
 		if( val % unit == 0 )
 		{
			printf("\rprogress %4d%c", val, '%'/*, w[p]*/);
		}

		if( curr == tar )
		{
			printf("\n");
		}
	}
};

#endif //__PROGRESSBAR_H__