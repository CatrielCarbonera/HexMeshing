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
// File RemoveIntenalSigularity.h
//
#include "stdafx.h"
#include "inc/RemoveInternalSingularity.h"

Hexa RemoveInternalSingularity::defHex(0,1,2,3,4,5,6,7);
namespace
{
	/*NODE*/
	double pt[16][3] = {
		{  0.0, 0.0, 0.0}, //  1
		{  1.0, 0.0, 0.0}, //  2
		{  1.0, 1.0, 0.0}, //  3 
		{  0.0, 1.0, 0.0}, //  4
		{  0.0, 0.0, 1.0}, //  5
		{  1.0, 0.0, 1.0}, //  6
		{  1.0, 1.0, 1.0}, //  7
		{  0.0, 1.0, 1.0}, //  8
		{ 0.25,0.25,0.25}, //  9
		{ 0.75,0.25,0.25}, // 10
		{ 0.75,0.75,0.25}, // 11
		{ 0.25,0.75,0.25}, // 12
		{ 0.25,0.25,0.75}, // 13
		{ 0.75,0.25,0.75}, // 14
		{ 0.75,0.75,0.75}, // 15
		{ 0.25,0.75,0.75}  // 16 
		};

	/*ELEMENT_SOLID*/
	int idx[7][8] = {
		/*1*/  {1-1,  9-1, 10-1,  2-1,  4-1, 12-1, 11-1,  3-1},
		/*2*/  {1-1,  2-1, 10-1,  9-1,  5-1,  6-1, 14-1, 13-1},
		/*3*/  {2-1,  3-1, 11-1, 10-1,  6-1,  7-1, 15-1, 14-1},
		/*4*/  {3-1,  4-1, 12-1, 11-1,  7-1,  8-1, 16-1, 15-1},
		/*5*/  {1-1,  5-1, 13-1,  9-1,  4-1,  8-1, 16-1, 12-1},
		/*6*/  {5-1, 13-1, 16-1,  8-1,  6-1, 14-1, 15-1,  7-1},
		/*7*/ {12-1,  9-1, 10-1, 11-1, 16-1, 13-1, 14-1, 15-1}
	};
};

RemoveInternalSingularity::RemoveInternalSingularity(void)
{
	size_t i;
	Hexa h;
	for ( i = 0; i <  16; ++i )
	{
		pta.push_back(pt3D(pt[i][0], pt[i][1], pt[i][2]) );
	}

	for ( i = 0; i <  7; ++i )
	{
		for ( size_t j =0; j < 8; ++j )
			h.idx[j] = idx[i][j];
		hex.push_back(h);
	}
}

RemoveInternalSingularity::~RemoveInternalSingularity(void)
{
}

void RemoveInternalSingularity::ReplaceCubeWithCell(
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
		pt.SetInternalFix();
		idx[i] = pointList.size();
		pointList.push_back(pt);
	}

	for ( i =0; i < 7; ++i )
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
void RemoveInternalSingularity::operator()(
	std::vector< Hexa >& hexList,
	std::vector< pt3D >& pointList)
{
	std::vector<Hexa> tmp;
	for ( size_t i = 0; i < hexList.size(); ++i )
		ReplaceCubeWithCell(hexList[i], tmp, pointList);
	hexList = tmp;
}
