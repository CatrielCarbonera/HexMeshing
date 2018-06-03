//
//   Copyright 2005-2006  @  C. Carbonera All rights reserved
//   11300 Rupp Drive
//   Burnsville, MN 55337
//   catriel.carbonera@comcast.net
//   This file can be freely redistributed for research and academic purposes only
//   undser the condition that the copyright notices, including this header, "
//   are not removed.
//   This file was generated using new techniques in mesh Generation. "
//   described in 
//   \"A Constructive Approach to Hexahedral Mesh Generation\" 
//           by C.D. Carboneara.
//
// SplitQuad.cpp: implementation of the SplitQuad class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "inc/SplitQuad.h"
#include <iostream>
#include <fstream>
#include "inc/MeshIo.h"

using namespace std;


void fillPta( std::vector<pt3D>& pta )
{
	size_t ndx(0);

	// Information on object # 0
	pta[ndx].x =    2.25000000000;
	pta[ndx].y =    2.25000000000;
	pta[ndx].z =   -4.00000000000;
	pta[ndx].type = pt3D::floor;
	pta[ndx].orient = pt3D::northEast;
	ndx++;

	// Information on object # 1
	pta[ndx].x =   -2.25000000000;
	pta[ndx].y =    2.25000000000;
	pta[ndx].z =   -4.00000000000;
	pta[ndx].type = pt3D::floor;
	pta[ndx].orient = pt3D::northWest;
	ndx++;
 
	// Information on object # 2
	pta[ndx].x =   -2.25000000000;
	pta[ndx].y =   -2.25000000000;
	pta[ndx].z =   -4.00000000000;
	pta[ndx].orient = pt3D::southWest;
	pta[ndx].type = pt3D::floor;
	ndx++;
 
	// Information on object # 3
	pta[ndx].x =    2.25000000000;
	pta[ndx].y =   -2.25000000000;
	pta[ndx].z =   -4.00000000000;
	pta[ndx].orient = pt3D::southEast;
	pta[ndx].type = pt3D::floor;
	ndx++;

	// Information on object # 4
	pta[ndx].x =    2.25000000000;
	pta[ndx].y =    2.25000000000;
	pta[ndx].z =   -3.00000000000;
	pta[ndx].type = pt3D::face;
	pta[ndx].orient = pt3D::northEast;
	ndx++;

	// Information on object # 5
	pta[ndx].x =   -2.25000000000;
	pta[ndx].y =    2.25000000000;
	pta[ndx].z =   -3.00000000000;
	pta[ndx].type = pt3D::face;
	pta[ndx].orient = pt3D::northWest;
	ndx++;

	// Information on object # 6
	pta[ndx].x =   -2.25000000000;
	pta[ndx].y =    0.00000000000;
	pta[ndx].z =   -3.00000000000;
	pta[ndx].type = pt3D::face;
	pta[ndx].orient = pt3D::west;
	ndx++;

	// Information on object # 7
	pta[ndx].x =    2.25000000000;
	pta[ndx].y =    0.00000000000;
	pta[ndx].z =   -3.00000000000;
	pta[ndx].orient = pt3D::east;
	pta[ndx].type = pt3D::face;
	ndx++;

	// Information on object # 8
	pta[ndx].x =    2.25000000000;
	pta[ndx].y =    2.25000000000;
	pta[ndx].z =   -2.00000000000;
	pta[ndx].type = pt3D::top;
	pta[ndx].orient = pt3D::northEast;
	ndx++;

	// Information on object # 9
	pta[ndx].x =   -2.25000000000;
	pta[ndx].y =    2.25000000000;
	pta[ndx].z =   -2.00000000000;
	pta[ndx].orient = pt3D::southEast;
	pta[ndx].type = pt3D::top;
	ndx++;
 
	// Information on object # 10
	pta[ndx].x =   -2.25000000000;
	pta[ndx].y =    0.00000000000;
	pta[ndx].z =   -2.00000000000;
	pta[ndx].type = pt3D::top;
	pta[ndx].orient = pt3D::west;
	ndx++;
 
	// Information on object # 11
	pta[ndx].x =   -2.25000000000;
	pta[ndx].y =   -2.25000000000;
	pta[ndx].z =   -2.00000000000;
	pta[ndx].type = pt3D::top;
	pta[ndx].orient = pt3D::southWest;
	ndx++;

	// Information on object # 12
	pta[ndx].x =    2.25000000000;
	pta[ndx].y =   -2.25000000000;
	pta[ndx].z =   -2.00000000000;
	pta[ndx].orient = pt3D::southEast;
	pta[ndx].type = pt3D::top;
	ndx++;

	// Information on object # 13
	pta[ndx].x =    2.25000000000;
	pta[ndx].y =    0.00000000000;
	pta[ndx].z =   -2.00000000000;
	pta[ndx].orient = pt3D::east;
	pta[ndx].type = pt3D::top;
	ndx++;
}

int hx[3][8] = { 
				{ 0,  1,  2,  3,  4,  5,  6,  7 },
				{ 7,  6,  2,  3, 13, 10, 11, 12 },
				{ 4,  5,  6,  7,  8,  9, 10, 13 }
				};

size_t qq[2][4] = {
	{8, 9, 10, 13},
	{13, 10, 11, 12}
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SplitQuad::SplitQuad():pta(14), hex(3)
{
	fillPta(pta);
	for ( size_t i = 0; i < 3; ++i )
	{
		for ( size_t j = 0; j < 8; ++j )
			hex[i].idx[j] = hx[i][j];
	}
}

SplitQuad::~SplitQuad()
{

}

void SplitQuad::AddSplitQuad(
							const pt3D& midPoint,
							const std::vector< size_t >& cycle,
							const std::vector< Quad >& refQuad,
							std::vector< Hexa >& hexList,
							std::vector< pt3D >& pointList,
							std::vector< Quad >& quadOut
		)
{
	for( size_t i = 0; i < cycle.size(); ++i )
	{
		size_t qEl = cycle[i]/4;
		size_t qEd = cycle[i]%4;
		Quad q( refQuad[ qEl ] );
		SplitSingleQuad( qEd, q,midPoint, hexList, pointList, quadOut);
	}
}

void SplitQuad::SplitSingleQuad(
								size_t ed,
								const Quad& q,
								const pt3D& midPoint,
								std::vector< Hexa >& hexList,
								std::vector< pt3D >& pointList,
								std::vector< Quad >& quadOut
		)
{
	size_t m4[] = {0,1,2,3,0,1,2,3};
	const double alpha(.5);
	const double beta(.5);
	size_t ip[14];
	pt3D pt[14];

	//--- Add base points
	ip[0] = q.idx[m4[ed+3]];
	ip[1] = q.idx[ed];
	ip[2] = q.idx[m4[ed+1]];
	ip[3] = q.idx[m4[ed+2]];

	pt[0] = pointList[ ip[0] ];
	pt[1] = pointList[ ip[1] ];
	pt[2] = pointList[ ip[2] ];
	pt[3] = pointList[ ip[3] ];

	//--- Add top points
	pt[8] = alpha*pt[0] + beta* midPoint;
	pt[9] = alpha*pt[1] + beta* midPoint;
	pt[11] = alpha*pt[2] + beta* midPoint;
	pt[12] = alpha*pt[3] + beta* midPoint;

	//--- Add mid-edge points
	pt[4] = 0.5*(pt[0]+pt[8]);
	pt[5] = 0.5*(pt[1]+pt[9]);
	pt[10] = 0.5*(pt[9]+pt[11]);
	pt[13] = 0.5*(pt[8]+pt[12]);

	//--- Add mid-face points
	pt[7] = 0.25*(pt[0]+pt[3]+pt[8]+pt[12]);
	pt[6] = 0.25*(pt[9]+pt[1]+pt[2]+pt[11]);

	for ( size_t idx = 0; idx < 14; ++idx )
		ip[idx] = AddPoint(pt[idx], pointList);

	//--- Add hexahedra
	// Add base

	for ( size_t i =0; i < 3; ++i )
	{
		Hexa h;
		for ( size_t j = 0; j < 8; ++j )
			h.idx[j] = ip[ hx[i][j] ];
		hexList.push_back(h);
	}

	Quad qtmp;
	qtmp.group = q.group;
	for (size_t iq = 0; iq < 2; ++iq)
	{
		for ( size_t i = 0; i < 4; ++i )
			qtmp.idx[i] = ip[qq[iq][i]];
		qtmp.referenceQuad = q.referenceQuad;
		quadOut.push_back(qtmp);
	}
}

void SplitQuad::PrintDebug()
{
	size_t i;
	cout << "SplitQuad Mesh = { \n";

	for ( i = 0; i < 3; ++i )
	{
		hex[i].PrintDebug(false);
	}

	for ( i = 0; i < 2; ++i )
	{
		hex[i].PrintDebug(pta);
	}

	for (i = 0; i < 14; ++i)
	{
		pta[i].PrintDebug();
	}
	
	WriteFile("C:\\Meshing\\FourSplitPyramid\\SplitQuad.dyn", pta, hex);
}