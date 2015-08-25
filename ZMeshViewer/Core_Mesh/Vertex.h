#ifndef _XMESHLIB_CORE_VERTEX_H_
#define _XMESHLIB_CORE_VERTEX_H_

#include "HalfEdge.h"
#include "Point.h"
#include "Trait.h"

namespace XMeshLib
{
	class Vertex;
	class HalfEdge;
	class TraitList;
	
	class Vertex
	{
	public:		
		Vertex()
		{ 
			m_halfedge = NULL; 
			m_boundary = false; 
			m_sharp = false;
			m_traits = NULL; //new TraitList; 
			m_umbilicPoints = false;
			m_isIsolatedVertex = false;
		}
		~Vertex()
		{
			if (m_traits)
			{
				m_traits->clear();
				delete m_traits;
			}
		}
		Point & point() { return  m_point; }
		//Point & normal(){ return  m_normal;}
		HalfEdge * & halfedge(){ return m_halfedge; }
		TraitList * & traits() { return m_traits; }
		void AddTrait(TraitNode * t);
		TraitNode * GetTrait(int tind);
		bool DelTrait(int tind);
		int & id() { return m_id; }		
		const int id() const  { return m_id; }
		bool & boundary() { return m_boundary; }
		bool & sharp() {return m_sharp;}
		bool & fc () {return m_fc;}
		std::string & string() { return m_string;}
		Point &OneRingFaceNormal(){ return  m_oneringfacenormal; };
		bool operator== (const Vertex & v) const
		{
			return m_id == v.id();
		}
		bool operator< (const Vertex & v) const
		{
			return v.id() >= m_id; 
		}
		HalfEdge *  most_ccw_in_halfedge();
		HalfEdge *  most_ccw_out_halfedge();
		HalfEdge *  most_clw_in_halfedge();
		HalfEdge *  most_clw_out_halfedge();

		double DistToVer( Vertex *ver )
		{
			return (this->point() - ver->point()).norm();
		}

		double & MaxPrincipalCurvature() { return m_maxPrincipalCurvatur; }
		double & MinPrincipalCurvature() { return m_minPrincipalCurvatur; }
		double & NormalValue() { return m_normalValue; }
		Point & NormalDirection() { return m_normalDirection; }
		Point & MaxPrincipalDirection() { return m_maxPrincipalDirection; }
		Point & MinPrincipalDirection() { return m_minPrincipalDirection; }
		bool & UmbilicPoints() { return m_umbilicPoints; }

	protected:
		friend class Mesh;

		int	m_id;		
		Point m_point;
		//Point m_normal;
		HalfEdge * m_halfedge;
		bool m_boundary;
		bool m_sharp;
		std::string m_string;
		TraitList * m_traits;
		Point m_oneringfacenormal;
		bool m_fc; //to indicate whether it is in the fixed constraint part

		double m_normalValue;
		double m_maxPrincipalCurvatur;
		double m_minPrincipalCurvatur;
		Point m_normalDirection;
		Point m_maxPrincipalDirection;
		Point m_minPrincipalDirection;
		bool m_umbilicPoints;

	public:
		bool & IsIsolatedVertex(){ return m_isIsolatedVertex; }
	protected:
		bool m_isIsolatedVertex;

	public:
		double & sdf() { return m_sdf; }
		double & SymmetryDistError() { return m_symmetrydisterror; }
	protected:
		double m_sdf;
		double m_symmetrydisterror;
	};

}//name space XMeshLib

#endif //_XMESHLIB_CORE_VERTEX_H_ defined
