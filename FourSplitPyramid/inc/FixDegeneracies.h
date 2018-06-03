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
#ifndef FIXDEGENERACIES_HXX
#define FIXDEGENERACIES_HXX

#include <vector>
#include <utility>

#include "pt3D.h"
#include "Quad.h"
#include "Hexa.h"

class FixDegeneracies
{
public:

	FixDegeneracies(void);
	~FixDegeneracies(void);

	void operator()(
		std::vector< Hexa >& hexList,
		std::vector< pt3D >& pointList);

	void ReplaceCubeWithCell(
		const Hexa &hex,
		std::vector< Hexa >& hexList,
		std::vector< pt3D >& pointList);

	std::vector<Hexa> hex;

	std::vector<pt3D> pta;
};
#endif
