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
// pt3D.cpp: implementation of the pt3D class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <vector>
#include <math.h>
#include "inc/pt3D.h"
#include <iostream>
#include <fstream>
using namespace std;

char* TYPE[] = { 
			"apex", 
			"face",
			"floor",
			"internal",
			"top"
		};

char* ORIENTATION[] = { 
		"north", 
		"south",
		"east",
		"west",
		"northEast",
		"northWest",
		"southEast",
		"southWest",
		"neutral"
};
namespace
{
	const double tol(1.e-6);
	const double tolSqr(tol*tol);

	double minDist(0);

};

size_t AddPoint(pt3D& pt, std::vector<pt3D>& pta)
{
	size_t npta = pta.size();
	for ( size_t i = 0; i < npta; ++i )
	{
		double distSqr = pt.distSqr(pta[i]);
		if ( distSqr < tolSqr )
		{
			if ( minDist < distSqr )
				minDist = distSqr;
			return i;
		}
	}

	// Add point
	pta.push_back( pt );

	return npta;
}

double MergeMinDist()
{
	return sqrt(minDist);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

pt3D::Orientation pt3D::MirrorOrientation(pt3D::Orientation ort, int xm, int ym)
{
	switch ( ort )
	{
	case north:
		return ( ym == -1 ? south: north);
	case south:
		return ( ym == -1 ? north: south);
	case east:
		return ( xm == -1 ? west: east);
	case west:
		return ( xm == -1 ? east: west);
	case northEast:
		if ( xm == 1 )
			return ( ym == -1 ? southEast: northEast);
		else
			return ( ym == -1 ? southWest: northWest);
		break;
	case southEast:
		if ( xm == 1 )
			return ( ym == -1 ? northEast: southEast);
		else
			return ( ym == -1 ? northWest: southWest);
		break;
	}
	return ort;
}

void pt3D::PrintDebug()
{
	cout << "(x, y, z) = (" << x << ", " << y << "," << z << ") ";
	cout << "type = " << TYPE[(size_t)type] << " ";
	cout << "orientation = " << ORIENTATION[(size_t)orient] << " \n";
}
