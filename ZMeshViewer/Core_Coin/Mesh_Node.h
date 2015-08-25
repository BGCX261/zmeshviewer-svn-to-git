#pragma warning(once:4819)

#ifndef __MESH_NODE_H__
#define __MESH_NODE_H__

#include "../Core_Mesh/Mesh.h"
#include "../Core_Mesh/MeshUtility.h"
#include "../Core_Mesh/Iterators.h"
#include "../Core_Mesh/TraitParser.h"
#include "../Core_Mesh/GetNormals.h"
#include "Common_Coin.h"

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/Win/viewers/SoWinExaminerViewer.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoIndexedLineSet.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoMaterialBinding.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoTransform.h>

#include <map>

BEGIN_DEFINE_COIN_NAMESPACE

class Mesh_Node
{
public:
	Mesh_Node( const char *mesh_name );
	~Mesh_Node();

	SoSeparator* GetSeperator();
	void ShowCone();

	XMeshLib::Mesh* GetMesh();
	std::string GetMeshName();

	int NumVertex();
	int NumFace();
	int NumEdge();
	int EulerCharacteristic();
	int NumBoundary();
	int NumComponent();
	float Genus();

	void SetTransparency( float trans_value );	// 0.0: no transpanrency; 1.0: total transparency
	float GetTransparency();

	void EnableColor();

private:
	SoSeparator*					m_separator;
	XMeshLib::Mesh*                 m_mesh;

	SoMaterial*						m_ver_face_mat;
	float							m_transparency;

	std::map<VertexID, VertexLinkID> m_verid2linkid_map;
	std::map<FaceID, FaceLinkID>     m_faceid2linked_map;

	SbVec3f * m_coin_vert;
	int32_t * m_coin_face;
	int32_t * m_coin_edge;
	int32_t * m_coin_boudary_edge;
	int32_t * m_coin_sharp_edge;
	float   * m_ver_rgb;
	float   * m_default_ver_rgb;
	bool      m_enalbe_color;

	int m_boundary_num;

	void ConstruceMesh();
	void InitializeMesh();
	void ConstructVertex();
	void ConstructFace();
	void ConstructEdge();
	void ConstructBoudaryEdge();
	void ConstructSharpEdge();

	void ShowMeshInfo();

	int VertexIdx( XMeshLib::Vertex* vertex );
	int VertexIdx( VertexID verid );
	int FaceIdx( XMeshLib::Face* face );

	int NumBoundaryEdge();
	int NumSharpEdge();
};

END_DEFINE_COIN_NAMESPACE

#endif //__MESH_NODE_H__