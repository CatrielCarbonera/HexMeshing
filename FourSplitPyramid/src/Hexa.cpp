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
// Hexa.cpp: implementation of the Hexa class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <iostream>
#include <fstream>
using namespace std;
#include "inc/Hexa.h"
#include "inc/Quad.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

namespace
{
/*
 =
	{     
	  {1-1,       8-1,       6-1,       5-1},
      {8-1,       3-1,       6-1,       7-1},
      {1-1,       3-1,       8-1,       4-1},
      {1-1,       6-1,       3-1,       2-1},
      {6-1,       8-1,       1-1,       3-1}
	};

*/
	size_t volTet[5][4] =
	{
		{0,7,5,4},
		{7,2,5,6},
		{0,2,7,3},
		{0,5,2,1},
		{5,7,0,2}
	};
	size_t shift[] = {0,1,2,3,0,1,2,3};
	size_t face[6][4] = {
							{0,1,2,3},	// TOP
							{4,7,6,5},	// BOTTOM
							{0,3,7,4},	// RIGHT
							{5,6,2,1},	// LEFT
							{4,5,1,0},	// FRONT
							{2,6,7,3}	// BACK
						};
	size_t faceOpposite[6][4] = {
							{4,5,6,7},	// BOTTOM
							{0,3,2,1},	// TOP
							{1,2,6,5},	// LEFT
							{4,7,3,0},	// RIGHT
							{7,6,2,3},	// BACK
							{1,5,4,0}	// FRONT
						};

	size_t p4[8] = {4,5,6,7, 0,1,2,3};
	size_t di[8][3][2] = { 
							{ {0, 1}, {0, 3}, {4, 0} },
							{ {0, 1}, {1, 2}, {5, 1} },
							{ {3, 2}, {1, 2}, {6, 2} },
							{ {3, 2}, {0, 3}, {7, 3} },

							{ {4, 5}, {4, 7}, {4, 0} },
							{ {4, 5}, {5, 6}, {5, 1} },
							{ {7, 6}, {5, 6}, {6, 2} },
							{ {7, 6}, {4, 7}, {7, 3} }
	};

#if 0
	double detOrig(const pt3D& p0, const pt3D& p1, const pt3D& p2)
	{
		double nx(  p1.y*p2.z - p2.y*p1.z );
		double ny(-(p1.x*p2.z - p2.x*p1.z));
		double nz(  p1.x*p2.y - p2.x*p1.y );

		return p0.x*nx+p0.y*ny+p0.z*nz;
	}
#endif
};

double Hexa::volume(int i, const std::vector<pt3D>& points) const
{
	pt3D pt[3];
	pt3D p0[3];
	pt3D p1[3];
	for ( size_t j = 0; j < 3; ++j )
	{
		size_t idx0(di[i][j][0]), idx1(di[i][j][1]);
		p0[j] = points[ idx[idx0] ];
		p1[j] = points[ idx[idx1] ];
		pt[j].x = p0[j].x - p1[j].x;
		pt[j].y = p0[j].y - p1[j].y;
		pt[j].z = p0[j].z - p1[j].z;
	}

	double vol = pt[2].det(pt[0], pt[1])/6;
	return vol;
}

double Hexa::volume(pt3D inPt[4] )
{
	pt3D pt[3];
	for ( size_t j = 0; j < 3; ++j )
	{
		pt[j].x = inPt[j].x - inPt[3].x;
		pt[j].y = inPt[j].y - inPt[3].y;
		pt[j].z = inPt[j].z - inPt[3].z;
	}

	double vol = pt[2].det(pt[0], pt[1])/6;
	return vol;
}
void Hexa::PrintDebug(bool OK)
{
	size_t j;
	cout << "	{ { ";
	for (j = 0; j < 3; ++j)
	{
		cout << idx[j] << ",";
	}
	cout << idx[j] << " }, { ";
	for (j = 4; j < 7; ++j)
	{
		cout << idx[j] << ",";
	}
	cout << idx[j] << " } }\n";
}

void Hexa::PrintDebug(std::vector<pt3D>& pta)
{
	cout << "volumes =  " << volume(0, pta);
	for (size_t j = 1; j < 8 ; ++j )
	{
		cout << ", " << volume(j, pta);
	}
	cout << "\n";
}

double Hexa::MinDet(const std::vector<pt3D>& pta)
{
	double vol = volume(0, pta);
	for (size_t j = 1; j < 8 ; ++j )
	{
		double v( volume(j, pta) );
		if ( v < vol )
			vol = v;
	}
	return vol;
}

double Hexa::MinDet(
					const std::vector<pt3D>& pta, 
					std::vector< Hexa >& hex)
{
	double vol = hex[0].MinDet(pta);
	for (size_t j = 1; j < hex.size() ; ++j )
	{
		double v( hex[j].MinDet(pta) );
		if ( v < vol )
			vol = v;
	}
	return vol;
}

double Hexa::MinVolume(
					const std::vector<pt3D>& pta, 
					std::vector< Hexa >& hex)
{
	pt3D pt[4];
	double minvol = 1.e+36;
	double maxvol = -1.e+36;
	size_t jmin=-1;
	size_t ngV(0);
	for (size_t j = 0; j < hex.size() ; ++j )
	{
		double v( 0 );
		for ( size_t i = 0; i < 5; ++i )
		{
			for ( size_t k =0; k < 4; ++k)
				pt[k] = pta[ hex[j].idx[ volTet[i][k] ] ];

				v += Hexa::volume(pt);
		}
		ngV += (v < 0 ? 1 : 0);
		if ( v < minvol )
		{
			jmin = j;
			minvol = v;
		}
		if ( v > maxvol )
			maxvol = v;
	}
	//cout << "Maximum volume = " << maxvol << " ";
	cout << "number of elements with negative volume = " << ngV << "\n";
	cout << "Element with minimum volume = " << jmin+1 << "\n";
	return minvol;
}
void Hexa::GetQuads(std::vector<Quad>& quads)
{
	for ( size_t i = 0; i < 6; ++i )
	{
		size_t j;
		size_t start, end;
		Quad q;
		start = 0;
		end = 4;
		for( j = 1; j < 4; ++j )
		{
			if ( idx[face[i][j]] < idx[face[i][start]])
			{
				start = j;
				end = start+4;
			}
		}

		for( j = start; j < end; ++j )
			q.idx[j-start] = idx[ face[i][shift[j]] ];
		q.referenceQuad=-1;
		quads.push_back(q);
	}
}

void Hexa::SetFirstFace(size_t i)
{
	size_t j;
	int idxTmp[8];;

	for ( j = 0; j < 4; ++j )
	{
		idxTmp[j] = idx[face[i][j]];
		idxTmp[j+4] = idx[ faceOpposite[i][j] ];
	}

	for ( j = 0; j < 8; ++j ) 
		idx[j] = idxTmp[j];
}

pt3D Hexa::Interpolate(const std::vector< pt3D > &pt,
					double u, double v, double w )
{
	pt3D p(0,0,0);
	p = (1.0-w)*((1.0-v)*((1.0-u)*pt[0]+u*pt[1])+
					    v*((1.0-u)*pt[3]+u*pt[2])) +
		       w*((1.0-v)*((1.0-u)*pt[4]+u*pt[5])+
					    v*((1.0-u)*pt[7]+u*pt[6]));

	return p;
}

