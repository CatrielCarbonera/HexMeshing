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
#ifndef BOUNDARY_LAYER_HXX
#define BOUNDARY_LAYER_HXX
#include "pt3D.h"
#include "Quad.h"
#include "Hexa.h"
class BoundaryLayer
{
public:
	BoundaryLayer(void);
	~BoundaryLayer(void);

	static void AddLayer(
		const pt3D& midPoint,
		const std::vector< Quad >& refQuad,
		std::vector< Hexa >& hexList,
		std::vector< pt3D >& pointList,
		std::vector< Quad >& quadBry
		);

	static void AddSingleCell(
		double r,
		const Quad& q,
		const pt3D& midPoint,
		std::vector< Hexa >& hexList,
		std::vector< pt3D >& pointList,
		std::vector< Quad >& quadOut
		);

};
#endif
