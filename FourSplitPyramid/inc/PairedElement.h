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
#ifndef PAIREDELEMENT_HXX
#define PAIREDELEMENT_HXX

#pragma once

#include <vector>
#include <utility>

#include "pt3D.h"
#include "Quad.h"
#include "Hexa.h"

class PairedElement
{
public:
	PairedElement(void);
	~PairedElement(void);

	void PrintDebug();

	static void AddPairOfHexahedra(	const Quad& left, 
						const Quad& right,
						std::vector<pt3D>& pta,
						std::vector<Hexa>& hex,
						std::vector<Quad>& transitionQuad,
						pt3D midPoint
						);

	static void AddHexahedra(
					const std::vector< std::pair<Quad, Quad> >& pairedPartition,
					std::vector<pt3D>& pta,
					std::vector<Hexa>& hex,
					std::vector<Quad>& transitionQuad,
					pt3D midPoint
					);

	std::vector<Hexa> hex;

	std::vector<pt3D> m_pta;

	pt3D midPoint;
};

#endif
