#include "Coin_Node.h"

USE_NAMESPACE_ZCoinLib

Coin_Node* Coin_Node::m_pInstance = NULL;
bool Coin_Node::bMark_vertex_begin = false;

Coin_Node::Coin_Node( SoSeparator * soSeparator, SoWinExaminerViewer * viewer ):
m_root_sep(soSeparator),
m_viewer(viewer)
{
	m_mesh_sep = new SoSeparator;
	m_mark_sep = new SoSeparator;
	//Add Face Mesh
	//m_face_sep = new SoSeparator;

	m_root_sep->addChild( m_mesh_sep );
	//Add Face Mesh
	//m_root_sep->addChild( m_face_sep );
	m_root_sep->addChild( m_mark_sep );

	m_mesh_node_map.clear();
	m_marked_ver_set.clear();

	m_curr_marked_ver_mesh_id = -1;
	m_curr_marked_ver_id = -1;

	m_mesh_maxdiagonallength = 1.0;

	Construct_Callback();
}
Coin_Node::~Coin_Node()
{
	for( std::map<MeshID,Mesh_Node*>::iterator mit = m_mesh_node_map.begin(); mit != m_mesh_node_map.end(); mit++ )
	{
		Mesh_Node* mn = (*mit).second;
		delete mn;
		mn = NULL;
	}
}
bool Coin_Node::MeshExist( MeshID mesh_id )
{
	return m_mesh_separator.find(mesh_id) != m_mesh_separator.end();
}
SoSeparator* Coin_Node::GetRootSoSeparator()
{
	return m_root_sep;

}
SoSeparator* Coin_Node::GetMeshSoSeparator()
{
	return m_mesh_sep;
}
SoSeparator* Coin_Node::GetMarkSoSeparator()
{
	return m_mark_sep;
}
SoSeparator* Coin_Node::GetSoSeparator( MeshID mesh_id )
{
	if( m_mesh_separator.find(mesh_id) != m_mesh_separator.end() )
	{
		return m_mesh_separator[mesh_id];
	}
	else
	{
		return NULL;
	}
}
SoWinExaminerViewer* Coin_Node::GetSoWinViewer()
{
	return m_viewer;
}
void Coin_Node::AddMesh( const char* mesh_name )
{
	MeshID mesh_id = m_mesh_node_map.size() + 1;
	std::cout<< "New mesh added: " << mesh_name << " (id: " << mesh_id << ").\n";
	Mesh_Node* mn = new Mesh_Node( mesh_name );
	SoSeparator * soSeparator = mn->GetSeperator();
	soSeparator->setName( mesh_name );

	m_mesh_sep->addChild( soSeparator );

	m_mesh_separator[mesh_id] = soSeparator;
	m_mesh_node_map[mesh_id] = mn;

	XMeshLib::GetNormals::ComputeNormals( mn->GetMesh() );
	SetMeshMaxDiagonalLength();
	SetMeshInitialMarkders();
}
//Add Face Mesh
//void Coin_Node::AddFaceMesh( const char* face_mesh_name )
//{
//	std::cout<< "Face mesh added: " << face_mesh_name << "\n";
//	m_face_mesh_node = new Mesh_Node( face_mesh_name );
//	SoSeparator * soSeparator = m_face_mesh_node->GetSeperator();
//	soSeparator->setName( face_mesh_name );
//	
//	m_face_sep->addChild( soSeparator );
//}
int Coin_Node::GetMeshNum()
{
	return m_mesh_node_map.size();
}
Mesh_Node* Coin_Node::GetMeshNode( MeshID mesh_id )
{
	return  m_mesh_node_map[mesh_id];
}
void Coin_Node::SetMeshMaxDiagonalLength()
{
	XMeshLib::Point max_point( -1e8, -1e8, -1e8 );
	XMeshLib::Point min_point(  1e8,  1e8,  1e8 );
	for( MeshID i = 1; i <= Coin_Node::GetInstance()->GetMeshNum(); i ++ )
	{
		Mesh_Node* mn = Coin_Node::GetInstance()->GetMeshNode( i );
		XMeshLib::Mesh* mesh = mn->GetMesh();
		for( XMeshLib::MeshVertexIterator vit(mesh); !vit.end(); ++vit )
		{
			XMeshLib::Point pt = (*vit)->point();
			for( unsigned int k = 0; k < 3; k ++ )
			{
				if( pt[k] > max_point[k] )
				{
					max_point[k] = pt[k];
				}
				if( pt[k] < min_point[k] )
				{
					min_point[k] = pt[k];
				}
			}
		}
	}

	m_mesh_maxdiagonallength = (max_point - min_point).norm();
}
void Coin_Node::SetMeshInitialMarkders()
{
	for( MeshID i = 1; i <= Coin_Node::GetInstance()->GetMeshNum(); i ++ )
	{
		Mesh_Node* mn = Coin_Node::GetInstance()->GetMeshNode( i );
		XMeshLib::Mesh* mesh = mn->GetMesh();
		for( XMeshLib::MeshVertexIterator vit(mesh); !vit.end(); ++vit )
		{
			XMeshLib::Vertex* ver = *vit;
			double markder_length = GetMarkedVertexCylinderLength( i, ver->id() );
			if( markder_length > 0.0 )
			{
				XMeshLib::Point ver_normal;
				if( XMeshLib::TraitParser::ReadNormalTrait( ver, ver_normal ) )
				{
					SbVec3f point_coor( (float)ver->point()[0], (float)ver->point()[1], (float)ver->point()[2] );
					SbVec3f point_normal( (float)ver_normal[0], (float)ver_normal[1], (float)ver_normal[2] );
					std::string cy_name = GetCylinderSepName( i, ver->id() );
					std::cout<< "\nInitial makder\n";
					AddPickedVertexMarker_Cylinder( point_coor, point_normal, std::string(cy_name), markder_length );
					AddMarkedVertex( i, ver->id() );
					PrintText( ver->id(), point_coor, 3 );
				}
			}
		}
	}
	std::cout<< "\n";
}
double Coin_Node::GetMeshMaxDiagonalLength()
{
	return m_mesh_maxdiagonallength;
}
std::list<VertexID>& Coin_Node::MarkVerList()
{
	return m_mark_ver_list;
}
bool Coin_Node::IsVertexMarked( MeshID mesh_id, VertexID ver_id )
{
	return m_marked_ver_set.find( std::make_pair(mesh_id,ver_id) ) != m_marked_ver_set.end();
}
void Coin_Node::AddMarkedVertex( MeshID mesh_id, VertexID ver_id )
{
	m_marked_ver_set.insert( std::make_pair(mesh_id,ver_id) );
}
MeshID& Coin_Node::GetCurrentMarkedVerMeshId()
{
	return m_curr_marked_ver_mesh_id;
}
VertexID& Coin_Node::GetCurrentMarkedVerId()
{
	return m_curr_marked_ver_id;
}
XMeshLib::Vertex* Coin_Node::GetVertex( MeshID mesh_id, VertexID ver_id )
{
	XMeshLib::Mesh* mesh = GetMeshNode( mesh_id )->GetMesh();
	return mesh->idVertex( ver_id );
}
void Coin_Node::SetMarkedVertexCylinderLength( MeshID mesh_id, VertexID ver_id, double length )
{
	std::string key_markder = "marker";
	double vlength = length;
	XMeshLib::Vertex* ver = GetVertex( mesh_id, ver_id );
	XMeshLib::TraitParser::UpdateTrait( ver->string(), key_markder, 1, &vlength );
}
double Coin_Node::GetMarkedVertexCylinderLength( MeshID mesh_id, VertexID ver_id )
{
	std::string key_markder = "marker";
	XMeshLib::Vertex* ver = GetVertex( mesh_id, ver_id );
	double vlength;
	if( XMeshLib::TraitParser::ReadTrait( ver->string(), key_markder, 1, &vlength ) )
	{
		return vlength;
	}
	else
	{
		return -1.0;
	}
}
void Coin_Node::Construct_Callback()
{
	SoEventCallback * myEventCallback = new SoEventCallback;
	myEventCallback->addEventCallback( SoKeyboardEvent::getClassTypeId(), Key_Press_Callback, m_viewer );
	myEventCallback->addEventCallback( SoMouseButtonEvent::getClassTypeId(), Mouse_Pick_Callback, m_viewer );

	m_root_sep->insertChild( myEventCallback, 0 );
}
void Coin_Node::Key_Press_Callback( void * userdata, SoEventCallback * keyboardCallback )
{
	const SoEvent * ke = keyboardCallback->getEvent();

	if( SO_KEY_PRESS_EVENT(ke, Q) )
	{
		std::cout<< "Want to Quit? Press ESC, then Q\n";
	}
	else if( SO_KEY_PRESS_EVENT(ke, H) )
	{
		HelpInfo();
	}
	else if( SO_KEY_PRESS_EVENT(ke, S) )
	{
		BeginMarkVertex();
	}
	else if( SO_KEY_PRESS_EVENT(ke, NUMBER_0) ||SO_KEY_PRESS_EVENT(ke, NUMBER_1) || SO_KEY_PRESS_EVENT(ke, NUMBER_2)
		|| SO_KEY_PRESS_EVENT(ke, NUMBER_3) || SO_KEY_PRESS_EVENT(ke, NUMBER_4) || SO_KEY_PRESS_EVENT(ke, NUMBER_5)
		|| SO_KEY_PRESS_EVENT(ke, NUMBER_6) || SO_KEY_PRESS_EVENT(ke, NUMBER_6) || SO_KEY_PRESS_EVENT(ke, NUMBER_7)
		|| SO_KEY_PRESS_EVENT(ke, NUMBER_8) || SO_KEY_PRESS_EVENT(ke, NUMBER_9))
	{
		if( bMark_vertex_begin )
		{
			InsertVertexIdx( ke );
		}
 		else
 		{
			int mesh_id = TransKey2Num(ke);
			if( mesh_id != -1 )
			{
				Hide_ShowMesh( mesh_id );
			}
		}
	}
	else if( SO_KEY_PRESS_EVENT(ke, C) )
	{
		Enable_DisableMeshColor();
	}
	else if( SO_KEY_PRESS_EVENT(ke, D) )
	{
		EndMarkVertex();
	}
	else if( SO_KEY_PRESS_EVENT(ke, E) )
	{
		ShowEdge();
	}
	else if( SO_KEY_PRESS_EVENT(ke, W) )
	{
		SaveMesh();
	}
	else if( SO_KEY_PRESS_EVENT(ke, UP_ARROW) )
	{
		ChangePickedVertexMarker_Length( true );
	}
	else if( SO_KEY_PRESS_EVENT(ke, DOWN_ARROW) )
	{
		ChangePickedVertexMarker_Length( false );
	}

	keyboardCallback->setHandled();
}
void Coin_Node::Mouse_Pick_Callback( void * userdata, SoEventCallback * mouseCallback )
{
 	const SoMouseButtonEvent * mbe = (SoMouseButtonEvent *)mouseCallback->getEvent();
 	if( mbe->getButton() == SoMouseButtonEvent::BUTTON1 &&
 		mbe->getState() == SoButtonEvent::DOWN ) 
	{
 		SoWinExaminerViewer * viewer = (SoWinExaminerViewer *)userdata;
 		SoRayPickAction rp(viewer->getViewportRegion());
 		rp.setPoint(mbe->getPosition());
 		rp.apply(viewer->getSceneManager()->getSceneGraph());
 
 		SoPickedPoint * point = rp.getPickedPoint();
 		if( point == NULL) 
 		{
 			return;
 		}

		Get_Nearest_Vertex_On_Model( point->getPoint() );
		
 		mouseCallback->setHandled();
	}
}
void Coin_Node::HelpInfo()
{
	std::cout<< "------------- Help Info -------------\n";
	std::cout<< "Mark vertex:               S [num][num]...[num] D\n";
	std::cout<< "Show edge:                 E\n";
	std::cout<< "Save mesh:                 W\n";
	std::cout<< "Hide/Show mesh:            [num](mesh_id)\n";
	std::cout<< "Enable/Disable color       C\n";
	std::cout<< "Change marker length:      UP_ARROW_KEY/DOWN_ARROW_KEY\n";
}
int Coin_Node::TransKey2Num( const SoEvent * ke )
{
	if( SO_KEY_PRESS_EVENT(ke, NUMBER_0) )
	{
		return 0;
	}
	else if( SO_KEY_PRESS_EVENT(ke, NUMBER_1) )
	{
		return 1;
	}
	else if( SO_KEY_PRESS_EVENT(ke, NUMBER_2) )
	{
		return 2;
	}
	else if( SO_KEY_PRESS_EVENT(ke, NUMBER_3) )
	{
		return 3;
	}
	else if( SO_KEY_PRESS_EVENT(ke, NUMBER_4) )
	{
		return 4;
	}
	else if( SO_KEY_PRESS_EVENT(ke, NUMBER_5) )
	{
		return 5;
	}
	else if( SO_KEY_PRESS_EVENT(ke, NUMBER_6) )
	{
		return 6;
	}
	else if( SO_KEY_PRESS_EVENT(ke, NUMBER_7) )
	{
		return 7;
	}
	else if( SO_KEY_PRESS_EVENT(ke, NUMBER_8) )
	{
		return 8;
	}
	else if( SO_KEY_PRESS_EVENT(ke, NUMBER_9) )
	{
		return 9;
	}

	return -1;
}
void Coin_Node::BeginMarkVertex()
{
	std::cout<< "BeginMarkVertex\n";
	bMark_vertex_begin = true;
	Coin_Node::GetInstance()->MarkVerList().clear();
}
void Coin_Node::InsertVertexIdx( const SoEvent * ke )
{
	int ver_idx = TransKey2Num( ke );
	if( ver_idx != -1 )
	{
		Coin_Node::GetInstance()->MarkVerList().push_back( ver_idx );
	}
}
void Coin_Node::EndMarkVertex()
{
	if( !bMark_vertex_begin )
	{
		return;
	}
	bMark_vertex_begin = false;

	std::list<VertexID> ver_id_list = Coin_Node::GetInstance()->MarkVerList();
	std::stringstream sVerId;
	unsigned int count = 0;
	for( std::list<VertexID>::iterator vit = ver_id_list.begin(); vit != ver_id_list.end(); vit ++ )
	{
		sVerId << (*vit);
	}

	int ver_id = atoi( sVerId.str().c_str() );

	std::vector<std::string> mesh_name_ver;
	for( MeshID i = 1; i <= Coin_Node::GetInstance()->GetMeshNum(); i ++ )
	{
		Mesh_Node* mn = Coin_Node::GetInstance()->GetMeshNode( i );
		XMeshLib::Mesh* mesh = mn->GetMesh();
		XMeshLib::Vertex* ver = mesh->idVertex( ver_id );
		if( ver != NULL )
		{
			mesh_name_ver.push_back( mn->GetMeshName() );
			SbVec3f ver_coor( (float)ver->point()[0], (float)ver->point()[1], (float)ver->point()[2] );
			AddPickedVertexMarker_Sphere( ver_coor, 4 );
			PrintText( ver->id(), ver_coor, 4 );
		}
	}

	if( mesh_name_ver.size() > 0 )
	{
		std::cout<< "Vertex " << ver_id << " Marked in mesh:\n";
		for( unsigned int i = 0; i < mesh_name_ver.size(); i ++ )
		{
			std::cout<< mesh_name_ver[i] << "\n";
		}
	}
	else
	{
		std::cout<< "Vertex " << ver_id << " Not found in any mesh\n";
	}
	Coin_Node::GetInstance()->MarkVerList().clear();
}
void Coin_Node::ShowEdge()
{
	static bool show_edge = false;
	show_edge = !show_edge;

	if( show_edge )
	{
		Coin_Node::GetInstance()->GetSoWinViewer()->setDrawStyle( SoWinViewer::STILL, SoWinViewer::VIEW_WIREFRAME_OVERLAY );
	}
	else
	{
		Coin_Node::GetInstance()->GetSoWinViewer()->setDrawStyle( SoWinViewer::STILL, SoWinViewer::VIEW_AS_IS );
	}
}
void Coin_Node::Get_Nearest_Vertex_On_Model( SbVec3f ver_coor )
{
	double min_dist = 1e8;
	XMeshLib::Point point( ver_coor[0], ver_coor[1], ver_coor[2] );
	XMeshLib::Vertex* picked_vertex = NULL;
	MeshID picked_mesh_id = 0;
	std::string picked_mesh_name;

	for( MeshID mesh_id = 1; mesh_id <= Coin_Node::GetInstance()->GetMeshNum(); mesh_id ++ )
	{
		Mesh_Node* mn = Coin_Node::GetInstance()->GetMeshNode( mesh_id );
		XMeshLib::Mesh* mesh = mn->GetMesh();
		for( XMeshLib::MeshVertexIterator vit(mesh); !vit.end(); ++vit )
		{
			XMeshLib::Point pt = (*vit)->point();
			double dist = (pt - point).norm();
			if( dist < min_dist )
			{
				min_dist = dist;
				picked_vertex = *vit;
				picked_mesh_id = mesh_id;
				picked_mesh_name = mn->GetMeshName();
			}
		}
	}
	
	SbVec3f picked_point( (float)picked_vertex->point()[0], (float)picked_vertex->point()[1], (float)picked_vertex->point()[2] );
	AddPickedVertexMarker_Cylinder( picked_mesh_id, picked_vertex->id() );
}
void Coin_Node::AddPickedVertexMarker_Sphere( SbVec3f ver_coor, int rgb_type )
{
	const float radius = (float)Coin_Node::GetInstance()->GetMeshMaxDiagonalLength() / 300.0f;

	SoSeparator * sep = new SoSeparator;
	Coin_Node::GetInstance()->GetMarkSoSeparator()->addChild( sep );

	SoDrawStyle* pick_draw = new SoDrawStyle;
	pick_draw->style = SoDrawStyle::FILLED;
	sep->addChild(pick_draw);

	SoMaterial * pick_mat = new SoMaterial;
	pick_mat->diffuseColor.setValue(COLOR[rgb_type]);
	sep->addChild(pick_mat);

	SoTranslation * trans = new SoTranslation;
	trans->translation.setValue( ver_coor );
	sep->addChild(trans);

	SoSphere * sphere = new SoSphere;
	sphere->radius.setValue( radius );
	sep->addChild( sphere );
}
void Coin_Node::AddPickedVertexMarker_Cylinder( MeshID mesh_id, VertexID vertex_id, int rgb_type )
{
	Coin_Node::GetInstance()->GetCurrentMarkedVerMeshId() = mesh_id;
	Coin_Node::GetInstance()->GetCurrentMarkedVerId() = vertex_id;
	std::cout<< "Picked Vertex on mesh: " << mesh_id << "\nVertex id:" << vertex_id << "\n";
	double vlength = Coin_Node::GetInstance()->GetMarkedVertexCylinderLength( mesh_id, vertex_id );
	if( vlength > -1e-9 )
	{
		std::cout<< "   markder length: " << vlength << "\n";
	}

	if( Coin_Node::GetInstance()->IsVertexMarked( mesh_id, vertex_id) )
	{
		std::cout<< "\n";
		return;
	}
	Coin_Node::GetInstance()->AddMarkedVertex( mesh_id, vertex_id );

	XMeshLib::Vertex* ver = Coin_Node::GetInstance()->GetVertex( mesh_id, vertex_id );
	SbVec3f picked_point( (float)ver->point()[0], (float)ver->point()[1], (float)ver->point()[2] );
	std::string cy_name = GetCylinderSepName( mesh_id, vertex_id );

	XMeshLib::Point ver_normal;
	if( XMeshLib::TraitParser::ReadNormalTrait( ver, ver_normal ) )
	{
		if( vlength < 0.0 )
		{
			vlength = (float)Coin_Node::GetInstance()->GetMeshMaxDiagonalLength() / 50.0f;
		}
		SbVec3f picked_point_normal( (float)ver_normal[0], (float)ver_normal[1], (float)ver_normal[2] );
		vlength = AddPickedVertexMarker_Cylinder( picked_point, picked_point_normal, cy_name, vlength );
		Coin_Node::GetInstance()->SetMarkedVertexCylinderLength( mesh_id, vertex_id, vlength );

		PrintText( vertex_id, picked_point, 3 );
	}
	else
	{
		std::cout<< "normal computation error!\n";
	}
	std::cout<< "\n";
}
double Coin_Node::AddPickedVertexMarker_Cylinder( SbVec3f ver_coor, SbVec3f normal, std::string cy_name, double length, int rgb_type )
{
	const float radius = (float)Coin_Node::GetInstance()->GetMeshMaxDiagonalLength() / 600.0f;
	float height = (float)length;
	float height_half = height * 0.5f;
	
	SoSeparator * sep = new SoSeparator;
	sep->setName( SbName(cy_name.data()) );
	Coin_Node::GetInstance()->GetMarkSoSeparator()->addChild( sep );

	SoDrawStyle* pick_draw = new SoDrawStyle;
	pick_draw->style = SoDrawStyle::FILLED;
	sep->addChild(pick_draw);

	SoMaterial * pick_mat = new SoMaterial;
	pick_mat->diffuseColor.setValue(COLOR[rgb_type]);
	sep->addChild(pick_mat);

	SbVec3f start_pos = ver_coor + normal * height_half;
	SbRotation rot( SbVec3f(0,1,0),SbVec3f(normal[0],normal[1],normal[2]) );
	
	SoTransform  * transf = new SoTransform;
	transf->translation.setValue( start_pos );
	transf->rotation = rot;
	sep->addChild( transf );

	SoCylinder * cylinder = new SoCylinder;
	cylinder->radius.setValue( radius );
	cylinder->height.setValue( height );
	sep->addChild( cylinder );
	std::cout<< "   markder length: " << height << "\n";
	return height;
}
void Coin_Node::ChangePickedVertexMarker_Length( bool elongate )
{
	MeshID   curr_marked_ver_mesh_id = Coin_Node::GetInstance()->GetCurrentMarkedVerMeshId();
	VertexID curr_marked_ver_id = Coin_Node::GetInstance()->GetCurrentMarkedVerId();

	if( curr_marked_ver_mesh_id < 0 || curr_marked_ver_id < 0 )
	{
		std::cout<< "\nPick one vertex first!\n";
		return;
	}
	float delta_length = (float)Coin_Node::GetInstance()->GetMeshMaxDiagonalLength() / 500.0f;
	if( !elongate )
	{
		delta_length *= -1;
	}

	std::string cy_name = GetCylinderSepName( curr_marked_ver_mesh_id, curr_marked_ver_id );

	SbName mark_cy_name(cy_name.data());
	SoSeparator * cylinder_sep = (SoSeparator*)Coin_Node::GetInstance()->GetMarkSoSeparator()->getByName(mark_cy_name);
	
	SoCylinder * cylinder =  (SoCylinder *)cylinder_sep->getChild( 3 );
	float current_height = cylinder->height.getValue();
	float changed_height = current_height + delta_length;
	if( changed_height < 0.0 )
	{
		changed_height = 0.0;
	}
	cylinder->height.setValue( changed_height );

	XMeshLib::Vertex* ver = Coin_Node::GetInstance()->GetVertex(curr_marked_ver_mesh_id, curr_marked_ver_id);
	XMeshLib::Point coor = ver->point();
	XMeshLib::Point ver_normal;
	XMeshLib::TraitParser::ReadNormalTrait( ver, ver_normal );

	SoTransform  * transf = (SoTransform *)cylinder_sep->getChild( 2 );
	XMeshLib::Point start_pos = coor + ver_normal * changed_height * 0.5;
	SbVec3f coor_sbvec( (float)start_pos[0], (float)start_pos[1], (float)start_pos[2] );
	transf->translation.setValue( coor_sbvec );
	
	Coin_Node::GetInstance()->SetMarkedVertexCylinderLength( curr_marked_ver_mesh_id, curr_marked_ver_id, changed_height );
}
std::string Coin_Node::GetCylinderSepName( MeshID mesh_id, VertexID vertex_id )
{
	char cy_name[100];
	sprintf_s( cy_name, 100, "cy_%d_%d", mesh_id, vertex_id );
	return std::string(cy_name);
}
void Coin_Node::PrintText( char* msg, SbVec3f ver_coor, int rgb_type  )
{
	SoSeparator * msg_sep = new SoSeparator;
	Coin_Node::GetInstance()->GetRootSoSeparator()->addChild( msg_sep );
 
  	SoTranslation * msg_trans = new SoTranslation;
  	msg_trans->translation.setValue( (float)ver_coor[0], (float)ver_coor[1], (float)ver_coor[2] * 1.05f );
  	msg_sep->addChild( msg_trans );

	SoTransform* msg_tf = new SoTransform;
	msg_tf->scaleFactor.setValue( 0.5f, 0.5f, 0.5f );
	msg_sep->addChild( msg_tf );

	SoMaterial * msg_mat = new SoMaterial;
	msg_mat->diffuseColor.setValue(SbColor( COLOR[rgb_type] ));
	msg_sep->addChild( msg_mat );

	SoText2 *msg_node = new SoText2;
	msg_node->string.setValue( msg );
	msg_sep->addChild( msg_node );
}
void Coin_Node::PrintText( int num, SbVec3f ver_coor, int rgb_type )
{
	char msg[100];
	_itoa_s( num, msg, 10 );
	PrintText( msg, ver_coor, rgb_type );
}
void Coin_Node::Hide_ShowMesh( MeshID mesh_id )
{
	if( !Coin_Node::GetInstance()->MeshExist( mesh_id ) )
	{
		return;
	}

	Mesh_Node* mn = Coin_Node::GetInstance()->GetMeshNode( mesh_id );
	float transparency = mn->GetTransparency();
	if( transparency < 0.5f )
	{
		mn->SetTransparency( 1.0f );
	}
	else
	{
		mn->SetTransparency( 0.0f );
	}

	std::cout<< "Hide/Show Mesh: " << mesh_id << ":\n";
	std::cout<< mn->GetMeshName() << "\n";
}
void Coin_Node::Enable_DisableMeshColor()
{
	for( MeshID i = 1; i <= Coin_Node::GetInstance()->GetMeshNum(); i ++ )
	{
		Mesh_Node* mn = Coin_Node::GetInstance()->GetMeshNode( i );
		mn->EnableColor();
	}
}
void Coin_Node::SaveMesh()
{
	for( MeshID mesh_id = 1; mesh_id <= Coin_Node::GetInstance()->GetMeshNum(); mesh_id ++ )
	{
		Mesh_Node* mn = Coin_Node::GetInstance()->GetMeshNode( mesh_id );
		XMeshLib::Mesh* mesh = mn->GetMesh();
		std::string mesh_name = mn->GetMeshName();
		std::string mesh_name_save = mesh_name.substr( 0, mesh_name.length() - 2 );
		mesh_name_save.append( "_save.m" );
		mesh->write( mesh_name_save.data() );
		std::cout<< "Save mesh: \n   " << mesh_name << "\n" << "to\n   " << mesh_name_save << "\n";
	}
}