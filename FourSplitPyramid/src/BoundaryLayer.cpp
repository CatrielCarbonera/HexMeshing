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
#include "stdafx.h"
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#include "inc/MeshIo.h"
#include "inc/BoundaryLayer.h"

BoundaryLayer::BoundaryLayer(void)
{
}

BoundaryLayer::~BoundaryLayer(void)
{
}

void BoundaryLayer::AddLayer(
	const pt3D& midPoint,
	const std::vector< Quad >& refQuad,
	std::vector< Hexa >& hexList,
	std::vector< pt3D >& pointList,
	std::vector< Quad >& quadBry
	)
{
	size_t i;
	double rmin(1.e+38);
	for ( i =0; i < refQuad.size(); ++i)
	{
		const Quad& q(refQuad[i]);
		double r(0);
		for ( size_t j=0; j < 4; ++j )
		{
			pt3D pt( pointList[ q.idx[j] ] );
			double r(pt.distSqr( midPoint ) );
			if ( r < rmin )
				rmin = r;
		}
	}
	rmin = sqrt(rmin);

	rmin *= 0.9;

	for ( i =0; i < refQuad.size(); ++i)
	{
		const Quad& q(refQuad[i]);
		AddSingleCell( rmin, q, midPoint, hexList, pointList, quadBry);
	}
}

void BoundaryLayer::AddSingleCell(
									double radius,
									const Quad& q,
									const pt3D& midPoint,
									std::vector< Hexa >& hexList,
									std::vector< pt3D >& pointList,
									std::vector< Quad >& quadOut
	)
{
	size_t i;
	Hexa hx;
	Quad qTmp(q);

	for ( i = 0; i < 4; ++i )
	{
		hx.idx[i] = q.idx[i];
	}

	
	for ( i = 0; i < 4; ++i )
	{
		const pt3D& ptBase(pointList[q.idx[i]]);
		double d( sqrt(midPoint.distSqr(ptBase)) );
		double alpha(radius/d);

		pt3D pt ( midPoint + alpha*(ptBase-midPoint) );
		qTmp.idx[i] = AddPoint(pt, pointList);
		hx.idx[i+4] = qTmp.idx[i];
	}
	quadOut.push_back(qTmp);
	hexList.push_back(hx);

}

