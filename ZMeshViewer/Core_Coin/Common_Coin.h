#ifndef __COMMON_COIN_H__
#define __COMMON_COIN_H__

#include <Inventor/Win/SoWin.h>
#include<iostream>

#define USE_NAMESPACE_ZCoinLib using namespace ZCoinLib;
#define BEGIN_DEFINE_COIN_NAMESPACE namespace ZCoinLib {
#define END_DEFINE_COIN_NAMESPACE }


const SbVec3f COLOR[]={	
					SbVec3f(0.35f, 0.53f, 0.70f),		// Background blue
					SbVec3f(0.2f, 0.2f, 0.5f),			// Default edge blue 
					SbVec3f(0.83f, 0.55f, 0.45f),		// Default Vertex brown
					SbVec3f(0.8f, 0.2f, 0.2f),			// Picked vertex red
					SbVec3f(0.2f, 0.8f, 0.2f),			// Marked vertex green
					SbVec3f(1.0, 1.0, 1.0),				// Boundary edge white
					SbVec3f(1.0f, 1.0f, 0.4f)			// Sharp edge yellow
				};

typedef int							MeshID;			// Start from 1
typedef int							VertexID;		// 
typedef int							FaceID;
typedef int							VertexLinkID;	// start from 1
typedef int							FaceLinkID;	// start from 1
typedef std::pair<MeshID,VertexID>	MeshVertexID;

#endif //__COMMON_COIN_H__