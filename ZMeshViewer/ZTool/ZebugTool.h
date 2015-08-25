#ifndef __ZEBUGTOOL_H__
#define __ZEBUGTOOL_H__

/*
	Debug Tool:
	Developed by Zhao Yin, that's why it's called ZebugTool :)
	2010-12-12
*/

#define zassert(_Expression) (void)( (!!(_Expression)) || (printf( "\n<Zebug>\nFile: %s\nLine: %d\n%s\n", __FILE__, __LINE__, #_Expression), exit(-1), 0) )

#endif //__ZEBUGTOOL_H__