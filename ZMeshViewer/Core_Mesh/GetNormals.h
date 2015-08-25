#ifndef XMESHLIB_GETNORMALS_H
#define XMESHLIB_GETNORMALS_H

#include "../Core_Mesh/Mesh.h"
#include "../Core_Mesh/Iterators.h"
#include "../Core_Mesh/Parser.h"
#include "../Core_Mesh/TraitParser.h"

#pragma warning (disable: 4018)

namespace XMeshLib
{
	class GetNormals
	{
	public:

		//Parse string to get normals information of the surface
		//Parameters:
		//mesh:		the given surface we try to parse its normals
		//			if normals information can be parsed, they will be stored in the mesh->Normals() attributes.
		//Return:
		//		true if normals are parsed successfully
		//		false if normal information are not correctly parsed
		static bool ParseNormals(Mesh * mesh)
		{
			bool flag = true;
			bool cflag;
			mesh->Normals().clear();
			for (MeshVertexIterator viter(mesh); !viter.end(); ++viter)
			{
				Vertex * ver = *viter;
				Parser p(ver->string());
				cflag = false;
				for (int i=0; i<p.tokens.size(); ++i)
				{
					std::string key = p.tokens[i].key;
					std::string value = p.tokens[i].value;
					if (key == "normal")
					{
						cflag = true;
						double w[3];
						sscanf_s(value.c_str(), "%lf %lf %lf", &w[0], &w[1], &w[2]);						
						Point pnorm(w[0], w[1], w[2]);
						mesh->Normals().add(ver, pnorm);		
					}
				}
				flag = flag & cflag;
				if (!flag)
					return false;
			}
			return true;
		}

		//Compute normals of the given surface
		//Parameters:
		//mesh:		the given surface we try to parse its normals
		//			the results are stored in the mesh->Normals() attributes.
		static void ComputeNormals(Mesh * mesh)
		{
			std::string keystr = "normal";
			mesh->Normals().clear();
			for (MeshVertexIterator viter(mesh); !viter.end(); ++viter)
			{
				Vertex * ver = *viter;
				double sumAngles = 0;
				std::vector<Point> faceNormals;
				std::vector<double> cornerAngles;
				for (VertexFaceIterator vfiter(ver); !vfiter.end(); ++vfiter)
				{
					Face * face = *vfiter;
					HalfEdge * he = face->halfedge();
					for (int i=0;i<3;++i)
					{
						if (he->source() == ver)
						{
							break;
						}
						else
							he = he->he_next();
					}
					HalfEdge * phe = he->he_prev();
					Point * cv[3];
					cv[0] = &phe->source()->point();
					cv[1] = &ver->point();
					cv[2] = &he->target()->point();
					Point fnorm = (*cv[1] - *cv[0])^(*cv[2] - *cv[1]);
					fnorm/= fnorm.norm();
					faceNormals.push_back(fnorm);
					double elen[3];
					elen[0] = (*cv[2] - *cv[1]).norm();
					elen[1] = (*cv[2] - *cv[0]).norm();
					elen[2] = (*cv[0] - *cv[1]).norm();
					double cangle = acos((elen[0] * elen[0] + elen[2] * elen[2] - elen[1] * elen[1])/(2*elen[0]*elen[2]));
					cornerAngles.push_back(cangle);
				}
				Point vnorm;
				for (int i=0;i<cornerAngles.size();++i)
				{
					vnorm += faceNormals[i] * cornerAngles[i];
					sumAngles += cornerAngles[i];
				}
				vnorm /= sumAngles;
				vnorm /= vnorm.norm();
				mesh->Normals().add(ver, vnorm);

				TraitParser::UpdateTrait(ver->string(), keystr, 3, vnorm.v);
			}
		}


		//Parse or compute normals for the given surface
		//Parameters:
		//1.	mesh:	given surface we try to assign its normals on vertices. 
		//				the results will be stored in its normals attributes
		//2.	tryParse	--> false:	compute the normals directly
		//					--> true:	try to parse normals from vertex string first, if there is no relative information, then compute
		//Return:
		//		true if the results are parsed from vertex strings
		//		false if the results are computed
		static bool GetNorm(Mesh * mesh, bool tryParse)
		{
			bool flag = false;
			if (tryParse)
			{
				flag = ParseNormals(mesh);
				ComputeFaceNormal(mesh);
				//if we get the normals from strings, flag will be true
			}
			if (!tryParse || !flag)
			{
				ComputeNormals(mesh);
			}
			return flag;
		}
		static void ComputeFaceNormal(Mesh* mesh)
		{
			for(MeshFaceIterator fiter(mesh); !fiter.end(); ++fiter)
			{
				Face * f = *fiter;
				Point p[3];
				int i = 0;
				for( FaceVertexIterator fviter(f); !fviter.end(); ++ fviter )
				{
					Vertex * v = *fviter;
					p[i++] = v->point();
				}
				Point n = (p[1]-p[0])^(p[2]-p[0]);
				n /= n.norm();	

				f->Normal() = n;
			}
		}

		static void ComputeVertexOneRingFaceNormal( Mesh *mesh )
		{
			ComputeFaceNormal(mesh);
			for( MeshVertexIterator mviter(mesh); !mviter.end(); ++mviter )
			{
				Point averNormal(0.0, 0.0, 0.0);
				if( (*mviter)->halfedge() == NULL )
				{
					continue;
				}

				VertexFaceIterator vfiter(*mviter);

				while( !vfiter.end() )
				{
					averNormal += (*vfiter)->Normal();
					++vfiter;
				}

				averNormal /= averNormal.norm();

				(*mviter)->OneRingFaceNormal() = averNormal;
// 				if( (*mviter)->id() < 10 )
// 				{
// 					mesh->AddNormaltetdra( *mviter );
// 				}
			}
		}

		//Inverse all the normal directions of the given surface
		static void InverseNormal(Mesh * mesh)
		{
			int ind=0;
			for (MeshVertexIterator viter(mesh); !viter.end(); ++viter)
			{
				Vertex * ver = *viter;
				Point & cvnorm = mesh->verNormal(ver); //>Normals()[ind++];
				cvnorm *= -1.0;
			}
		}

		//Update the vertex string with the normal stored as attributes
		//Parameters:
		//mesh: given surface whose vertex strings we want to update		
		static void Normal2String(Mesh * mesh)
		{
			std::string keystr = "normal";
			int ind=0;
			for (MeshVertexIterator viter(mesh); !viter.end(); ++viter)
			{
				Vertex * ver = *viter;
				Point & cvnorm = mesh->verNormal(ver); 
				TraitParser::UpdateTrait(ver->string(), keystr, 3, cvnorm.v);
			}
		}


	};
}

#endif