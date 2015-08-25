#pragma warning(once:4819)

#ifndef __COIN_NODE_H__
#define __COIN_NODE_H__

#include "Mesh_Node.h"
#include "Common_Coin.h"
#include "../Ztool/ZebugTool.h"

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/Win/viewers/SoWinExaminerViewer.h>
#include <Inventor/nodes/SoEventCallback.h>
#include <Inventor/events/SoKeyboardEvent.h>
#include <Inventor/events/SoMouseButtonEvent.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/SoPickedPoint.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoCylinder.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoBaseColor.h>
#include <Inventor/nodes/SoText2.h>
#include <Inventor/nodes/SoText3.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoRotation.h>
#include <Inventor/draggers/SoDragPointDragger.h>
#include <Inventor/nodes/SoScale.h>

#include <vector>
#include <list>
#include <map>
#include <iostream>
#include <string>
#include <sstream>

BEGIN_DEFINE_COIN_NAMESPACE

class Coin_Node
{
public:
	static Coin_Node* GetInstance( SoSeparator * soSeparator, SoWinExaminerViewer * viewer )
	{
		if( m_pInstance == NULL )
		{
			m_pInstance = new Coin_Node( soSeparator, viewer );
		}

		return m_pInstance;
	}

	static Coin_Node* GetInstance()
	{
		zassert( m_pInstance != NULL );
		return m_pInstance;
	}
	~Coin_Node();

	bool MeshExist( MeshID mesh_id );
	SoSeparator* GetRootSoSeparator();
	SoSeparator* GetMeshSoSeparator();
	SoSeparator* GetMarkSoSeparator();
	SoSeparator* GetSoSeparator( MeshID mesh_id );

	SoWinExaminerViewer* GetSoWinViewer();

	void AddMesh( const char* mesh_name );
	//Add Face Mesh
	void AddFaceMesh( const char* face_mesh_name );		
	int GetMeshNum();
	Mesh_Node* GetMeshNode( MeshID mesh_id );

	void SetMeshMaxDiagonalLength();
	void SetMeshInitialMarkders();
	double GetMeshMaxDiagonalLength();

	std::list<VertexID>& MarkVerList();

	bool IsVertexMarked( MeshID mesh_id, VertexID ver_id );
	void AddMarkedVertex( MeshID mesh_id, VertexID ver_id );
	SoCylinder* GetMarkedVertexCylinder( MeshID mesh_id, VertexID ver_id );
	MeshID& GetCurrentMarkedVerMeshId();
	VertexID& GetCurrentMarkedVerId();

	XMeshLib::Vertex* GetVertex( MeshID mesh_id, VertexID ver_id );

	void SetMarkedVertexCylinderLength( MeshID mesh_id, VertexID ver_id, double length );
	double GetMarkedVertexCylinderLength( MeshID mesh_id, VertexID ver_id );

private:
	Coin_Node( SoSeparator * soSeparator, SoWinExaminerViewer * viewer );

	class Garbo
	{
	public:
		~Garbo()
		{
			if( Coin_Node::m_pInstance )
			{
				delete Coin_Node::m_pInstance;
			}
		}

		static Garbo garbo;
	};

	static Coin_Node*				m_pInstance;

	SoWinExaminerViewer *			m_viewer;
	SoSeparator*					m_root_sep;
	SoSeparator*					m_mesh_sep;
	SoSeparator*					m_mark_sep;
	//Add Face Mesh
	//SoSeparator*					m_face_sep;

	std::map<MeshID,Mesh_Node*>     m_mesh_node_map;
	std::map<MeshID,SoSeparator*>   m_mesh_separator;
	//Add Face Mesh
	//Mesh_Node*						m_face_mesh_node;


	std::list<VertexID>             m_mark_ver_list;
	MeshID							m_curr_marked_ver_mesh_id;
	VertexID						m_curr_marked_ver_id;
	
	static bool bMark_vertex_begin;
	double m_mesh_maxdiagonallength;
	std::set< std::pair<MeshID,VertexID> >	m_marked_ver_set;

	void Construct_Callback();
	static void Key_Press_Callback( void * userdata, SoEventCallback * keyboardCallback );
	static void Mouse_Pick_Callback( void * userdata, SoEventCallback * mouseCallback );
	
	static void HelpInfo();
	static int TransKey2Num( const SoEvent * ke );

	static void BeginMarkVertex();
	static void EndMarkVertex();
	static void InsertVertexIdx( const SoEvent * ke );
	static void ShowEdge();

	static void Get_Nearest_Vertex_On_Model( SbVec3f ver_coor );
	static void AddPickedVertexMarker_Sphere( SbVec3f ver_coor, int rgb_type = 3 );
	static void AddPickedVertexMarker_Cylinder( MeshID mesh_id, VertexID vertex_id, int rgb_type = 3 );
	static double AddPickedVertexMarker_Cylinder( SbVec3f ver_coor, SbVec3f normal, std::string cy_name, double length, int rgb_type = 3 );
	static void ChangePickedVertexMarker_Length( bool elongate );

	static std::string GetCylinderSepName( MeshID mesh_id, VertexID vertex_id );

	static void PrintText( char* msg, SbVec3f ver_coor, int rgb_type );
	static void PrintText( int num, SbVec3f ver_coor, int rgb_type );
	static void Hide_ShowMesh( const SoEvent * ke );
	static void Hide_ShowMesh( MeshID mesh_id );
	static void Enable_DisableMeshColor();
	static void SaveMesh();
};

END_DEFINE_COIN_NAMESPACE

#endif //__COIN_NODE_H__