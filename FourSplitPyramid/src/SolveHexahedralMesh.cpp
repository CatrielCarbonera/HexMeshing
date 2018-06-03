//
//   Copyright 2005-2006  @  C. Carbonera All rights reserved
//   11300 Rupp Drive
//   Burnsville, MN 55337
//   catriel.carbonera@comcast.net
//   This file can be freely redistributed for research and academic purposes only
//   under the condition that the copyright notices, including this header, "
//   are not removed.
//   This file was generated using new techniques in mesh Generation. "
//   described in 
//   \"A Constructive Approach to Hexahedral Mesh Generation\" 
//           by C.D. Carboneara.
//
//

#include "stdafx.h"
#include <iostream>
using namespace std;

#include <map>
#include <algorithm>
#include "inc/MeshIo.h"
#include "inc/solvehexahedralmesh.h"
#include "inc/PairedElement.h"
#include "inc/SplitQuad.h"
#include "inc/Pyramid.h"
#include "inc/GeodeTemplate.h"
#include "inc/BoundaryLayer.h"

namespace
{
	typedef std::pair< size_t, size_t > pairk;
	size_t p1[] = {1, 2, 3, 0, 1, 2, 3, 0};
	//size_t p2[] = {2, 3, 0, 1, 2, 3, 0, 1};
	size_t m1[] = {3, 0, 1, 2, 3, 0, 1, 2};
	SolveHexahedralMesh::Neighbors::edge_t p2[]= {
										SolveHexahedralMesh::Neighbors::third, 
										SolveHexahedralMesh::Neighbors::fourth,
										SolveHexahedralMesh::Neighbors::first,
										SolveHexahedralMesh::Neighbors::second
										};
	SolveHexahedralMesh::Neighbors::edge_t p3[]= {
										SolveHexahedralMesh::Neighbors::fourth,
										SolveHexahedralMesh::Neighbors::first,
										SolveHexahedralMesh::Neighbors::second, 
										SolveHexahedralMesh::Neighbors::third
												};

	void CreateTable( std::vector< Quad >& transitionQuad,
		std::vector<SolveHexahedralMesh::Neighbors>& dual,
						std::vector< std::pair<pairk, size_t> >& edges)
	{
		size_t i, edg;
		dual.reserve(transitionQuad.size());
		for (i = 0, edg=0; i < transitionQuad.size();++i)
		{
			Quad& q = transitionQuad[i];
			SolveHexahedralMesh::Neighbors n;

			for ( size_t j = 0; j < 4; ++j, ++edg )
			{
				n.elem[j] = edg;
				pairk p(q.idx[j], q.idx[p1[j]]);
				if ( p.first > p.second )std::swap(p.first, p.second);
				edges.push_back(std::pair< pairk, size_t > (p,edg) );
			}
			dual.push_back(n);
		}
		std::sort(edges.begin(), edges.end());
	}

	void BuildDual(const std::vector< std::pair<pairk, size_t> >& edges,
		std::vector<SolveHexahedralMesh::Neighbors>& dual )
	{
		//-- Construct dual
		size_t i( 0 );
		size_t j(0);

		while ( i < edges.size() )
		{
			j = i+1;
			while( j < edges.size() && edges[i].first == edges[j].first )
			{
				j++;
			}
			if ( j - i != 2 )
			{
				cout << "ERROR - Topology problem. Quadrilateral " << 
					edges[i].second/4 << 
					" has an edge witout a neighbor\n";
				throw 1;
			}
			size_t ej = edges[j-1].second/4;
			size_t edj = edges[j-1].second%4;
			size_t ei = edges[i].second/4;
			size_t edi = edges[i].second%4;
			dual[ei].elem[edi] = edges[j-1].second;
			dual[ej].elem[edj] = edges[i].second;
			i = j;
		}
	}

	bool FirstDirection(SolveHexahedralMesh::Neighbors::edge_t ed)
	{
		return ed == SolveHexahedralMesh::Neighbors::first ||
			ed == SolveHexahedralMesh::Neighbors::third  ? true : false;
	}

	void GenerateCycle(
						std::vector<SolveHexahedralMesh::Neighbors>& dual,
						std::vector< std::vector< size_t > >& outerCycles, 
						size_t edge,
						size_t elem,
						std::vector< std::pair<bool,bool> >& processed,
						bool innerCycle,
						const std::vector<Quad>& quadList
						)
	{
		size_t i;
		std::vector<size_t> cycle;
		size_t currEl, prevEl;
		SolveHexahedralMesh::Neighbors::edge_t prevEd, currEd;

		currEl = elem;
		currEd = (SolveHexahedralMesh::Neighbors::edge_t)edge;
		
		if ( FirstDirection( currEd ) )
		{
			if ( processed[currEl].first )
				return;
		}
		else
		{
			if ( processed[currEl].second )
				return;
		}

		do
		{
			if ( currEd == SolveHexahedralMesh::Neighbors::first ||
				currEd == SolveHexahedralMesh::Neighbors::third )
			{
				processed[currEl].first = true;
			}
			else
			{
				processed[currEl].second = true;
			}
			cycle.push_back(currEl*4+(size_t)currEd);
			prevEd = currEd;
			prevEl = currEl;
			currEd = p2[dual[prevEl].NeighborEdge(prevEd)];
			currEl = dual[prevEl].NeighborElement(prevEd);
		} while (currEl != elem);

		bool outer = false;
		for ( i = 1; i < cycle.size() ; ++i )
		{
			size_t q1 = quadList[ (size_t) (cycle[i]/4) ].group;
			size_t q2 = quadList[ (size_t) (cycle[i-1]/4) ].group;

			//if ( q1/6 != q2/6 ) // the elements are not members of the same cell
			if ( q1 != q2 ) // the elements are not members of the same cell
			{
				outer = true;
			}
		}

		if ( ( outer && !innerCycle ) ||
				( !outer && innerCycle ) )
				outerCycles.push_back(cycle);

		for ( i = 0; i < cycle.size(); ++ i )
		{
			size_t travElem;
			size_t travEdj;
			size_t j ( cycle[i] );
			//--- Orient left side
			currEl = j/4;
			currEd = (SolveHexahedralMesh::Neighbors::edge_t) (j%4);

			// Create cycle for neighbor at left edge of cycle
			size_t leftEd = p1[currEd];
			travEdj = p3[dual[currEl].NeighborEdge(
				(SolveHexahedralMesh::Neighbors::edge_t)leftEd)];
			travElem = dual[currEl].NeighborElement(
				(SolveHexahedralMesh::Neighbors::edge_t)leftEd);
			GenerateCycle( dual, outerCycles, travEdj, travElem, processed,
							innerCycle,
							quadList);

			// Create cycle for neighbor at right edge of cycle
			SolveHexahedralMesh::Neighbors::edge_t rightEd = p3[currEd];
			travEdj = p1[dual[currEl].NeighborEdge(rightEd)];
			travElem = dual[currEl].NeighborElement(rightEd);
			GenerateCycle( dual, outerCycles, travEdj, travElem, processed,
							innerCycle,
							quadList );
		}
	}

	void CreateOuterCycles(std::vector<SolveHexahedralMesh::Neighbors>& dual, 
		std::vector< std::vector<size_t> >& outerCycles,
		const std::vector<Quad>& transitionQuad)
	{
		bool innerCycle(false);
		std::pair<bool, bool> falsePair(false,false);
		std::vector< std::pair<bool,bool> > processed( dual.size(), falsePair );
		for ( size_t i = 0; i < dual.size(); ++i )
		{
			if ( !processed[i].first  )
			{
				GenerateCycle(dual,	outerCycles,
					SolveHexahedralMesh::Neighbors::first, i,
					processed,
					innerCycle, transitionQuad);
			}
			if ( !processed[i].second  )
			{
				GenerateCycle(dual,	outerCycles,
					SolveHexahedralMesh::Neighbors::second, i,
					processed,
					innerCycle, transitionQuad);
			}
		}
	}

	void CreateInnerCycles(std::vector<SolveHexahedralMesh::Neighbors>& dual, 
		std::vector< std::vector<size_t> >& outerCycles,
		const std::vector<Quad>& transitionQuad)
	{
		bool innerCycle(true);
		std::pair<bool, bool> falsePair(false,false);
		std::vector< std::pair<bool,bool> > processed( dual.size(), falsePair );
		for ( size_t i = 0; i < dual.size(); ++i )
		{
			if ( !processed[i].first  )
			{
				GenerateCycle(dual,	outerCycles,
					SolveHexahedralMesh::Neighbors::first, i,
					processed,
					innerCycle, transitionQuad);
			}
			if ( !processed[i].second  )
			{
				GenerateCycle(dual,	outerCycles,
					SolveHexahedralMesh::Neighbors::second, i,
					processed,
					innerCycle, transitionQuad);
			}
		}
	}

	pt3D MidPoint(const std::vector< pt3D >& pta )
	{
		pt3D midPoint(0,0,0);
		for ( size_t i = 0 ; i < pta.size(); ++ i)
			midPoint += pta[i];
		double factor(1./pta.size());
		midPoint *= factor;
		return midPoint;
	}

	//--- Debug information
	void PrintDebugCycle(
		std::vector< std::vector< size_t > >& outerCycles,
		std::vector< pt3D >& pta,
		std::vector< Quad >& transitionQuad)
	{
		std::vector< Quad > outQuad;
		for( size_t c=0; c< outerCycles.size(); ++c )
		{
			for ( size_t q=0; q < outerCycles[c].size(); ++q )
			{
				size_t qel = outerCycles[c][q]/4;
				Quad Q ( transitionQuad[qel] );
				Q.group = c+1;
				outQuad.push_back(Q);
			}
		}

		WriteFile("C:\\Meshing\\FourSplitPyramid\\pairedTransition.dyn",
					pta, outQuad);
	}

	size_t currGroup(1);
	void FillGroup(Hexa& h)
	{
		if ( h.group == 0 )
			h.group = currGroup;
	}
};

SolveHexahedralMesh::SolveHexahedralMesh(
					const std::vector< std::pair<Quad, Quad> >& pairedPartition,
					std::vector<pt3D>& pta,
					bool useGeodeTemplate
					):m_pta(pta), m_midPoint(0,0,0)
{
	size_t prevHex(0);
	size_t c;
	std::vector<Quad> transitionQuad;
	std::vector<Neighbors> dual;
	m_midPoint = MidPoint(m_pta);

	PairedElement::AddHexahedra(pairedPartition,
								m_pta, 
								m_hexa,
								transitionQuad,
								m_midPoint);
	CreateDual(transitionQuad, dual);
	std::for_each( m_hexa.begin(),m_hexa.end(), FillGroup);
	currGroup++;
	//-- Create Cycles
	CreateOuterCycles(dual, m_outerCycles, transitionQuad);

	//--- Two Split Quad
	std::vector<Quad> splitQuad;

	size_t firstSplitIdx( m_hexa.size() );

	for ( size_t iquad =0 ; iquad < transitionQuad.size(); ++iquad )
		transitionQuad[iquad].referenceQuad = iquad+1;

	for( c=0; c< m_outerCycles.size(); ++c )
	{
		SplitQuad::AddSplitQuad(
			m_midPoint,
			m_outerCycles[c],
			transitionQuad,	
			m_hexa,	
			m_pta,
			splitQuad);
	}

	std::for_each( m_hexa.begin(),m_hexa.end(), FillGroup);
	currGroup++;

	//-- Create Inner Cycles
	std::vector< Quad > fourSplit;
	std::vector<Neighbors> dualSplit;
	CreateDual(splitQuad, dualSplit);
	CreateInnerCycles(dualSplit, m_innerCycles, splitQuad);
	std::vector< Quad > fourSplitOut;
	for( c=0; c< m_innerCycles.size(); ++c )
	{
		SplitQuad::AddSplitQuad(
			m_midPoint,
			m_innerCycles[c],
			splitQuad,	
			m_hexa,	
			m_pta,
			fourSplit);
	}
	std::for_each( m_hexa.begin(),m_hexa.end(), FillGroup);
	currGroup++;

	//--- Sort elements by their source quadrilateral. This is done to 
	//--- identify the four split cells.
	std::sort(fourSplit.begin(), fourSplit.end(), Quad::CompReferenceQuad);

	//--- Fill the quadrilaterals
	if ( useGeodeTemplate )
		GeodeTemplate::FillFourSplits(
			m_midPoint,
			m_hexa,
			m_pta,
			fourSplit
		);
	else
		Pyramid::FillFourSplits(
			m_midPoint,
			m_hexa,
			m_pta,
			fourSplit
		);

	std::for_each( m_hexa.begin(),m_hexa.end(), FillGroup);
	currGroup++;
	//
	//Pyramid pyr;
	//pyr.PrintDebug();
}

SolveHexahedralMesh::~SolveHexahedralMesh(void)
{
}

void SolveHexahedralMesh::CreateDual(std::vector<Quad>& transitionQuad,
									 std::vector<Neighbors>& dual)
{
	std::vector< std::pair<pairk, size_t> > edges;

	//-- Create table
	CreateTable(transitionQuad, dual, edges);

	//-- Build Dual
	BuildDual(edges, dual );

}
