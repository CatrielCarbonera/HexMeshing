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
#ifndef SOLVEHEXAHEDRALMESH_HXX
#define SOLVEHEXAHEDRALMESH_HXX

#pragma once

#include <vector>
#include <utility>

#include "pt3D.h"
#include "Quad.h"
#include "Hexa.h"

class SolveHexahedralMesh
{
public:
	struct Neighbors
	{
		enum  edge_t{first=0, second=1, third=2, fourth=3};

		size_t NeighborElement(edge_t i ){return elem[i]/4;}

		edge_t NeighborEdge(edge_t i )
		{
			size_t ii = elem[i]%4;
			return (edge_t) ii;
		}
		size_t elem[4];

	};

	SolveHexahedralMesh(
					const std::vector< std::pair<Quad, Quad> >& pairedPartition,
					std::vector<pt3D>& pta,
					bool useGeodeTemplate
					);

	~SolveHexahedralMesh( void );

	void CreateDual(std::vector<Quad>& transitionQuad,
						std::vector<Neighbors>& dual);

	std::vector< std::vector<size_t> > m_outerCycles;
	std::vector< std::vector<size_t> > m_innerCycles;


	pt3D m_midPoint;

	std::vector<pt3D> m_pta;

	std::vector<Hexa> m_hexa;

};

#endif
