#ifndef _XMESHLIB_COMMON_H_
#define _XMESHLIB_COMMON_H_

#pragma warning (disable : 4786)

//If you are a  beginner and just want to read this meshlib, please leave the following macro
//if you are using this meshlib, for efficiency issue, please comment it

//#define SIMPLE


#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <assert.h>
#include <math.h>

#ifndef SIMPLE
#include <complex>

#define ID_AdjHeTrait						0000
#define ID_UVTrait							0001
#define ID_TheBoolTrait						0002
#define ID_TheDoubleTrait					0003

#define ID_CFKMTrait						1000

#define ID_DiLenEdgeTrait					1001
#define ID_DiLenFaceTrait					1002

#define ID_CCSEdgeTrait						1003
#define ID_CCSFaceTrait						1004
#define ID_CCSVerTrait						1005
#define ID_CCSHalfEdgeTrait					1006
#define ID_CCSUVTrait						1007

#define ID_DistFieldEdgeTrait				1008
#define ID_DistFieldVerTrait				1009
#define	ID_DFTriEdgeTrait					1010
#define ID_DFTriVerTrait					1011
//#define ID_MeshDFEdgeTrait				1012
#define ID_MeshDFVerTrait				1013


#define ID_G0CMapVerTrait					1016
#define ID_G0CMapEdgeTrait				1017
#define ID_G0CMapHETrait					1018

#define ID_SegmentFaceTrait					1020
#define ID_SegmentEdgeTrait					1021
#define ID_SegmentVerTrait					1022
#define ID_SegmentHETrait					1023
	
#define ID_VolumeMapTrait					1030

#define ID_GreedyHomotopyEdgeTrait			1040
#define ID_GreedyHomotopyVerTrait			1041
#define ID_SMapVerTrait						1046
#define ID_SMapFaceTrait					1047
#define ID_SMapEdgeTrait					1048
#define ID_ChartVertexTrait					1051	
#define ID_ChartFaceTrait					1052
#define ID_ChartEdgeTrait					1053
#define ID_GeodesicVertexTrait				1061
#define ID_GeodesicEdgeTrait				1062
#define ID_GeodesicFaceTrait				1063
#define ID_FlatMetricTilerVertexTrait		1071
#define ID_FlatMetricTilerEdgeTrait			1072
#define ID_FlatMetricTilerFaceTrait			1073
#define ID_FlatMetricTilerHalfEdgeTrait		1074
#define ID_HMetricTilerVertexTrait			1081
#define ID_HMetricTilerFaceTrait			1082
#define ID_HMetricTilerEdgeTrait			1083
#define ID_HMetricTilerHalfEdgeTrait		1084

#define ID_PantVertexTrait					1090

#define ID_MChartSegVerTrait				1100
#define ID_MChartSegFaceTrait				1101

typedef std::complex<double> dcomp;
#endif

#define XREALDOUBLE
#ifdef XREALDOUBLE
#define xreal double
#endif
#ifndef XREALDOUBLE
#define xreal float
#endif


#define MAX2(a, b) ((a) > (b) ? (a) : (b))
#define MIN2(a, b) ((a) < (b) ? (a) : (b))

#define MAX_LINE 2048
#define PI 3.1415926535897932384626
#define EPS 1e-8

template <typename U>
struct delete_ptr
{
	void operator()(U* ptr){ 
		if (ptr) {
			delete ptr;
		}

	}
};


//extern std::ofstream SysLog;

//extern XTimer * ctimer;

#endif