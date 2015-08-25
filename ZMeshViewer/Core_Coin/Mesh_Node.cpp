#include "Mesh_Node.h"

USE_NAMESPACE_ZCoinLib

Mesh_Node::Mesh_Node( const char *mesh_name )
{
	m_mesh = new XMeshLib::Mesh;
	m_mesh->read( mesh_name );

    m_separator = new SoSeparator;
	m_separator->ref();

	m_separator->setName( mesh_name );

	m_ver_face_mat = new SoMaterial;
	m_transparency = 0.0;

	m_verid2linkid_map.clear();
	m_faceid2linked_map.clear();

	m_coin_vert = NULL;
	m_coin_face = NULL;
	m_coin_edge = NULL;
	m_coin_boudary_edge = NULL;
	m_coin_sharp_edge = NULL;

	m_enalbe_color = false;

	std::vector<XMeshLib::Curve *> bounds;
	m_boundary_num = XMeshLib::MeshUtility::TraceAllBoundaries( m_mesh, bounds );
	XMeshLib::MeshUtility::MarkSharpEdges( m_mesh );
	//ShowCone();
	ConstruceMesh();
}
Mesh_Node::~Mesh_Node()
{
	delete []m_ver_rgb;
	delete []m_default_ver_rgb;
	delete m_mesh;
	m_mesh = NULL;
	m_separator->unref();
}
SoSeparator* Mesh_Node::GetSeperator()
{
	return m_separator;
}
void Mesh_Node::ShowCone()
{
	SoSeparator * instructsep = new SoSeparator;
	m_separator->addChild(instructsep);

	SoTranslation * instructtrans = new SoTranslation;
	instructsep->addChild( instructtrans );

	float x = (float)(rand() % 10);
	instructtrans->translation.setValue( x, 1, 1);

	SoCone * cone = new SoCone;
	instructsep->addChild( cone );
}
XMeshLib::Mesh*  Mesh_Node::GetMesh()
{
	return m_mesh;
}
std::string Mesh_Node::GetMeshName()
{
	return std::string( m_separator->getName() );
}
int Mesh_Node::NumVertex()
{
	return m_mesh->numVertices();
}
int Mesh_Node::NumFace()
{
	return m_mesh->numFaces();
}
int Mesh_Node::NumEdge()
{
	return m_mesh->numEdges();
}
int Mesh_Node::EulerCharacteristic()
{
	return NumVertex() - NumEdge() + NumFace();
}
int Mesh_Node::NumBoundary()
{
	return m_boundary_num;
}
int Mesh_Node::NumComponent()
{
	return NumBoundary();
}
float Mesh_Node::Genus()
{
	return (2.0f * NumComponent() - EulerCharacteristic() - NumBoundary()) / (2.0f * NumComponent());
}
void Mesh_Node::SetTransparency( float trans_value )
{
	m_transparency = trans_value;
	m_ver_face_mat->transparency = trans_value;
}
float Mesh_Node::GetTransparency()
{
	return m_transparency;
}
void Mesh_Node::EnableColor()
{
	unsigned int ver_num = NumVertex();
	if( m_enalbe_color )
	{
		m_ver_face_mat->diffuseColor.setValues(0,ver_num,(SbColor *)m_ver_rgb);
	}
	else
	{
		m_ver_face_mat->diffuseColor.setValues(0,ver_num,(SbColor *)m_default_ver_rgb);
	}
	m_enalbe_color = !m_enalbe_color;
}
void Mesh_Node::ConstruceMesh()
{
	InitializeMesh();

	ConstructVertex();
	ConstructFace();
	ConstructSharpEdge();
	ConstructBoudaryEdge();
	ConstructEdge();

	ShowMeshInfo();
}
void Mesh_Node::InitializeMesh()
{
	unsigned int ver_link_id = 0;
	for( XMeshLib::MeshVertexIterator vit(m_mesh); !vit.end(); ++vit )
	{
		m_verid2linkid_map[ (*vit)->id() ] = ver_link_id++;
	}

	unsigned int face_link_id = 0;
	for( XMeshLib::MeshFaceIterator fit(m_mesh); !fit.end(); ++fit )
	{
		m_faceid2linked_map[ (*fit)->id() ] = face_link_id++;
	}
}
void Mesh_Node::ConstructVertex()
{
	std::string rgb_key = "rgb";
	unsigned int ver_num = NumVertex();

	m_coin_vert = new SbVec3f[ver_num];
	//float *ver_rgb = new float[ver_num * 3];
	m_ver_rgb = new float[ver_num * 3];
	m_default_ver_rgb = new float[ver_num * 3];

	int ver_count = 0;
	int color_count = 0;
	for( XMeshLib::MeshVertexIterator vit(m_mesh); !vit.end(); ++vit )
	{
		XMeshLib::Vertex* ver = *vit;
		XMeshLib::Point pt = ver->point();
		m_coin_vert[ver_count].setValue( (float)pt[0], (float)pt[1], (float)pt[2] );

		double color[3];
		if( XMeshLib::TraitParser::ReadTrait((*vit)->string(),rgb_key,3,color) )
		{
			m_ver_rgb[color_count  ] = (float)color[0];
			m_ver_rgb[color_count+1] = (float)color[1];
			m_ver_rgb[color_count+2] = (float)color[2];
		}
		else
		{
			m_ver_rgb[color_count  ] = COLOR[2][0];
			m_ver_rgb[color_count+1] = COLOR[2][1];
			m_ver_rgb[color_count+2] = COLOR[2][2];
		}

		m_default_ver_rgb[color_count  ] = COLOR[2][0];
		m_default_ver_rgb[color_count+1] = COLOR[2][1];
		m_default_ver_rgb[color_count+2] = COLOR[2][2]; 

		ver_count ++;
		color_count += 3;
	}

	SoMaterialBinding *ver_mat_binding = new SoMaterialBinding;
	ver_mat_binding->value = SoMaterialBinding::PER_VERTEX_INDEXED;
	m_separator->addChild(ver_mat_binding);  

	//SoMaterial * ver_mat = new SoMaterial;
	SetTransparency( 0.0 );
	m_ver_face_mat->diffuseColor.setValues(0,ver_num,(SbColor *)m_ver_rgb);
	m_separator->addChild(m_ver_face_mat);

	SoCoordinate3 * coords = new SoCoordinate3;
	coords->point.setValues(0, ver_num, m_coin_vert);
	m_separator->addChild(coords);

	//delete []ver_rgb;
}
void Mesh_Node::ConstructFace()
{
	unsigned int face_num = NumFace();
	unsigned int face_idx_num = face_num * 4;
	m_coin_face = new int32_t[face_idx_num];
	int face_count = 0;
	for( XMeshLib::MeshFaceIterator fit(m_mesh); !fit.end(); ++fit )
	{
		XMeshLib::Vertex * v[3];
		XMeshLib::HalfEdge * he = (*fit)->halfedge();

		v[0] = he->source();
		v[1] = he->target();
		v[2] = he->he_next()->target();

		m_coin_face[face_count  ] = VertexIdx(v[0]);
		m_coin_face[face_count+1] = VertexIdx(v[1]);
		m_coin_face[face_count+2] = VertexIdx(v[2]);
		m_coin_face[face_count+3] = SO_END_FACE_INDEX;

		face_count += 4;
	}


	SoIndexedFaceSet * faceset = new SoIndexedFaceSet;
	faceset->coordIndex.setValues(0, face_idx_num, m_coin_face);
	m_separator->addChild(faceset);	
}
void Mesh_Node::ConstructEdge()
{
	unsigned int edge_num = NumEdge() * 3;
	m_coin_edge = new int32_t[edge_num];

	int edge_count = 0;
 	for( XMeshLib::MeshEdgeIterator eit(m_mesh); !eit.end(); ++eit )
 	{
 		m_coin_edge[edge_count  ] = VertexIdx( (*eit)->vid(0) );
 		m_coin_edge[edge_count+1] = VertexIdx( (*eit)->vid(1) );
 		m_coin_edge[edge_count+2] = SO_END_LINE_INDEX;
 		edge_count += 3;
 	}

	SoDrawStyle* edge_draw = new SoDrawStyle;
	edge_draw->style = SoDrawStyle::FILLED;
	m_separator->addChild(edge_draw);

	SoMaterialBinding *edge_mat_binding = new SoMaterialBinding;
	edge_mat_binding->value = SoMaterialBinding::OVERALL;
	m_separator->addChild(edge_mat_binding);

	SoMaterial * edge_mat = new SoMaterial;
	edge_mat->diffuseColor.setValue( COLOR[1] );
	m_separator->addChild(edge_mat);

	SoIndexedLineSet * sharp_lineset = new SoIndexedLineSet;
	sharp_lineset->coordIndex.setValues(0,edge_num,m_coin_edge);
	m_separator->addChild(sharp_lineset);
}
void Mesh_Node::ConstructBoudaryEdge()
{
	int boundary_edge_num = NumBoundaryEdge() * 3;
	m_coin_boudary_edge = new int32_t[boundary_edge_num];

	int bedge_count = 0;
	for( XMeshLib::MeshEdgeIterator meit(m_mesh); !meit.end(); ++meit )
	{
		XMeshLib::Edge* edge = *meit;
		if( edge->boundary() )
		{
			m_coin_boudary_edge[bedge_count  ] = VertexIdx( edge->vid(0) );
			m_coin_boudary_edge[bedge_count+1] = VertexIdx( edge->vid(1) );
			m_coin_boudary_edge[bedge_count+2] = SO_END_LINE_INDEX;

			bedge_count += 3;
		}
	}

	SoMaterialBinding *boundary_edge_mat_binding = new SoMaterialBinding;
	boundary_edge_mat_binding->value = SoMaterialBinding::OVERALL;
	m_separator->addChild(boundary_edge_mat_binding);

	SoMaterial * boundary_edge_mat = new SoMaterial;
	boundary_edge_mat->diffuseColor.setValue(COLOR[5]);
	m_separator->addChild(boundary_edge_mat);

	SoIndexedLineSet * boundary_lineset = new SoIndexedLineSet;
	boundary_lineset->coordIndex.setValues(0,boundary_edge_num,m_coin_boudary_edge);
	m_separator->addChild(boundary_lineset);	
}
void Mesh_Node::ConstructSharpEdge()
{
	int sharp_edge_num = NumSharpEdge() * 3;
	m_coin_sharp_edge = new int32_t[sharp_edge_num];

	int sharp_edge_count = 0;
	for( XMeshLib::MeshEdgeIterator meit(m_mesh); !meit.end(); ++meit )
	{
		XMeshLib::Edge* edge = *meit;
		if( edge->sharp() )
		{
			m_coin_sharp_edge[sharp_edge_count  ] = VertexIdx( edge->vid(0) );
			m_coin_sharp_edge[sharp_edge_count+1] = VertexIdx( edge->vid(1) );
			m_coin_sharp_edge[sharp_edge_count+2] = SO_END_LINE_INDEX;

			sharp_edge_count += 3;
		}
	}

	SoMaterialBinding *sharp_edge_mat_binding = new SoMaterialBinding;
	sharp_edge_mat_binding->value = SoMaterialBinding::OVERALL;
	m_separator->addChild(sharp_edge_mat_binding);

	SoMaterial * sharp_edge_mat = new SoMaterial;
	sharp_edge_mat->diffuseColor.setValue(COLOR[6]);
	m_separator->addChild(sharp_edge_mat);

	SoIndexedLineSet * sharp_lineset = new SoIndexedLineSet;
	sharp_lineset->coordIndex.setValues(0,sharp_edge_num,m_coin_sharp_edge);
	m_separator->addChild(sharp_lineset);	
}
void Mesh_Node::ShowMeshInfo()
{
	//std::cout<< " c=" << NumComponent();
	std::cout<< " b=" << NumBoundary();
	std::cout<< " v=" << NumVertex();
	std::cout<< " f=" << NumFace(); 
	std::cout<< " e=" << NumEdge();
	//std::cout<< " g=" << Genus();
	
	std::cout<< "\n";
}
int Mesh_Node::VertexIdx( XMeshLib::Vertex* vertex )
{
	return m_verid2linkid_map[ vertex->id() ];
	//return vertex->id() - 1;
}
int Mesh_Node::VertexIdx( VertexID verid )
{
	return m_verid2linkid_map[ verid ];
}
int Mesh_Node::FaceIdx( XMeshLib::Face* face )
{
	return m_faceid2linked_map[ face->id() ];
	//return (face->id() - 1) * 4;
}
int Mesh_Node::NumBoundaryEdge()
{
	int boundary_edge_num = 0;
	for( XMeshLib::MeshEdgeIterator meit(m_mesh); !meit.end(); ++meit )
	{
		XMeshLib::Edge* e = *meit;
		if( e->boundary() )
		{
			boundary_edge_num ++;
		}
	}

	return boundary_edge_num;
}
int Mesh_Node::NumSharpEdge()
{
	int sharp_edge_num = 0;
	for( XMeshLib::MeshEdgeIterator meit(m_mesh); !meit.end(); ++meit )
	{
		XMeshLib::Edge* e = *meit;
		if( e->sharp() )
		{
			sharp_edge_num ++;
		}
	}

	return sharp_edge_num;
}