#include "Control_Node.h"
USE_NAMESPACE_ZCoinLib;

Control_Node::Control_Node( char* win_name ):
m_win_name(win_name)
{
	InitialCoin();
}
Control_Node::~Control_Node()
{
	delete m_viewer;
	m_root->unref();
}
void Control_Node::InitialCoin()
{
	m_window = SoWin::init( m_win_name );
 	if( m_window == NULL )
 	{
 		 exit(1);
 	}

	m_root = new SoSeparator;
	m_root->ref();

	m_viewer = new SoWinExaminerViewer( m_window );
	m_coin_node = Coin_Node::GetInstance( m_root, m_viewer );
}
void Control_Node::AddMesh( char* mesh_name )
{
	m_mesh_name_vec.push_back( std::string(mesh_name) );
}
void Control_Node::Show()
{
	//Add Face Mesh
 	for( unsigned int i = 0; i < m_mesh_name_vec.size(); i ++ )
 	{
 		m_coin_node->AddMesh( m_mesh_name_vec[i].c_str() );
 	}
// 	m_coin_node->AddFaceMesh( m_mesh_name_vec[0].c_str() );
// 	m_coin_node->AddMesh( m_mesh_name_vec[1].c_str() );

	m_viewer->setSceneGraph( m_root );
	m_viewer->setTitle("ZMeshViewer");
	m_viewer->setBackgroundColor( SbColor( COLOR[0] )  );
	SbVec2s view_size(800,800);
	m_viewer->setSize(view_size);
	//m_viewer->setDrawStyle( SoWinViewer::STILL, SoWinViewer::VIEW_WIREFRAME_OVERLAY );
	//m_viewer->setWireframeOverlayColor( SbColor( 0.2f, 0.2f, 0.5f ) );
	m_viewer->setFeedbackVisibility(TRUE);

	m_viewer->show();

	SoWin::show( m_window );
	SoWin::mainLoop();
}