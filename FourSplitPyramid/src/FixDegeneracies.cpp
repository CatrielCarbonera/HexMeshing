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
#include "inc/FixDegeneracies.h"
#include <assert.h>

#include "inc/Hexa.h"
#include "inc/Quad.h"
namespace
{
	double pt[16][3] = {
		/*  1 */ { 0.0, 0.0, 0.0},
		/*  2 */ { 1.0, 0.0, 0.0},
		/*  3 */ { 1.0, 1.0, 0.0},
		/*  4 */ { 0.0, 1.0, 0.0},
		/*  5 */ { 0.0, 0.0, 1.0},
		/*  6 */ { 1.0, 0.0, 1.0},
		/*  7 */ { 1.0, 1.0, 1.0},
		/*  8 */ { 0.0, 1.0, 1.0},
		/*  9 */ {0.25,0.25, 0.5},
		/* 10 */ {0.75,0.25, 0.5},
		/* 11 */ {0.75,0.75, 0.5},
		/* 12 */ {0.25,0.75, 0.5},
		/* 13 */ {0.25,0.25, 1.0},
		/* 14 */ {0.75,0.25, 1.0},
		/* 15 */ {0.75,0.75, 1.0},
		/* 16 */ {0.25,0.75, 1.0}
		};
	//*ELEMENT_SOLID
	size_t idx[6][8] = {
		/* 1 */{ 1-1, 2-1, 3-1, 4-1, 9-1,10-1,11-1,12-1},
		/* 2 */{ 4-1, 1-1, 9-1,12-1, 8-1, 5-1,13-1,16-1},
		/* 3 */{ 3-1, 4-1,12-1,11-1, 7-1, 8-1,16-1,15-1},
		/* 4 */{ 3-1,11-1,10-1, 2-1, 7-1,15-1,14-1, 6-1},
		/* 5 */{10-1, 9-1, 1-1, 2-1,14-1,13-1, 5-1, 6-1},
		/* 6 */{11-1,12-1, 9-1,10-1,15-1,16-1,13-1,14-1}
		};

	size_t GetFixedCount( 
		const std::vector< pt3D> &ptl,
		const Hexa &hex)
	{
		size_t numFix(0);
		for ( size_t i = 0; i < 8; ++i )
		{
			numFix += ptl[hex.idx[i]].FixedEnd() ? 1 : 0;
		}

		return numFix;
	}

	size_t GetQuadFixedCount( 
		const std::vector< pt3D> &ptl,
		const Quad &q)
	{
		size_t numFix(0);
		for ( size_t i = 0; i < 4; ++i )
		{
			numFix += ptl[q.idx[i]].InternalFixed() ? 1 : 0;
		}

		return numFix;
	}
};



FixDegeneracies::FixDegeneracies(void)
{
	size_t i;
	Hexa h;
	for ( i = 0; i <  16; ++i )
	{
		pta.push_back(pt3D(pt[i][0], pt[i][1], pt[i][2]) );
	}

	for ( i = 0; i <  6; ++i )
	{
		for ( size_t j =0; j < 8; ++j )
			h.idx[j] = idx[i][j];
		hex.push_back(h);
	}
}

FixDegeneracies::~FixDegeneracies(void)
{
}

void FixDegeneracies::operator()(
	std::vector< Hexa >& hexList,
	std::vector< pt3D >& pointList)
{
	std::vector< Hexa > hexListTmp;
	for ( size_t i =0; i < hexList.size(); ++i )
	{
		if ( GetFixedCount( pointList, hexList[i]) < 8 )
		{
			std::vector < Quad > fc;
			hexList[i].GetQuads(fc);

			for ( size_t j =0; j < 6 ; ++j )
			{
				if ( GetQuadFixedCount(pointList,fc[j])==4 )
				{
					hexList[i].SetFirstFace(j);
					ReplaceCubeWithCell(hexList[i],hexListTmp,pointList);
					j = 7;
				}
			}
			assert(j == 8);
		}
		else
		{
			hexListTmp.push_back(hexList[i]);
		}
	}
	hexList = hexListTmp;
}

void FixDegeneracies::ReplaceCubeWithCell(
	const Hexa &hex,
	std::vector< Hexa >& hexList,
	std::vector< pt3D >& pointList)
{
	size_t i;
	int idx[16];
	std::vector<pt3D> eightNodes;
	for ( i = 0; i < 8; ++i )
	{
		eightNodes.push_back( pointList[ hex.idx[i] ] );
		idx[i] = hex.idx[i];
	}

	for ( i = 8; i < 16; ++i )
	{
		pt3D pt = Hexa::Interpolate(eightNodes, pta[i].x, pta[i].y, pta[i].z);
		pt.move = false;
		size_t nd = AddPoint(pt, pointList);
		idx[i] = nd;
	}

	for ( i =0; i < 6; ++i )
	{
		Hexa hx;
		hx.group = hex.group;
		for ( size_t j =0; j < 8; ++j )
		{
			hx.idx[j] = idx[ this->hex[i].idx[j] ];
		}
		hexList.push_back(hx);
	}
}
