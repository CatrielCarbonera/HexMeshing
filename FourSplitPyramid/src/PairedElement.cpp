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
#include "stdafx.h"
#include <iostream>
#include <fstream>

using namespace std;
#include "inc/PairedElement.h"

namespace
{

	const double fith(0.2);
	const double twoFiths(0.4);
	const double threeFiths(0.6);
	const double fourFiths(0.8);
	size_t plus1[] = {1, 2, 3, 0, 1, 2, 3, 0};

	size_t pairedTransition[2][3][4] = {
		{ {0, 1, 5, 4}, {1, 2, 6, 5}, {2, 3, 7, 6} },	// left quads
		{ {0, 1, 5, 4}, {0, 4, 7, 3}, {3, 7, 6, 2} }	// right quads
	};

	size_t bottomFaces[2][4]=
	{
		{1,2,3,0},	//RIGHT
		{2,1,8,9}	//LEFT
	};

	int hexP[2][8] = {
		{ 0, 1, 2, 3, 4, 5, 6, 7 },
		{ 1, 8, 9, 2, 5, 4, 7, 6 }
	};

	double ptP[10][3] = {
		{ 1,   1, 0}, // 0
		{ 0,   1, 0}, // 1
		{ 0,  -1, 0}, // 2
		{ 1,  -1, 0}, // 3
		{ 0,  1./3, 1.0}, // 4
		{ 0,  2./3, 0.5}, // 5
		{ 0, -2./3, 0.5}, // 6
		{ 0, -1./3, 1.0}, // 7

		{-1,   1, 0}, // 8
		{-1,  -1, 0}, // 9
	};
};

PairedElement::PairedElement(void):m_pta(10),hex(2)
{
	size_t i;
	for ( i = 0; i < 2; ++i )
	{
		for ( int j = 0; j < 8; ++j )
			hex[i].idx[j] = hexP[i][j];
	}

	for ( i = 0; i < 10; ++ i )
	{
		m_pta[i].x = ptP[i][0];
		m_pta[i].y = ptP[i][1];
		m_pta[i].z = ptP[i][2];
	}
}

PairedElement::~PairedElement(void)
{
}

void PairedElement::AddHexahedra(
					const std::vector< std::pair<Quad, Quad> >& pairedPartition,
					std::vector<pt3D>& pta,
					std::vector<Hexa>& hex,
					std::vector<Quad>& tansitionQuad,
					pt3D midPoint
					)
{
	for (size_t i = 0; i < pairedPartition.size(); ++i )
	{

		try
		{
		AddPairOfHexahedra(pairedPartition[i].first, pairedPartition[i].second,
					pta, hex, tansitionQuad,
					midPoint);
		}
		catch (int i)
		{
			cout << "Error - pair number " << i << "; quadrilaterals do not share an edge.\n";
			throw (1);
		}
	}
}

void PairedElement::AddPairOfHexahedra(	const Quad& left, 
					const Quad& right,
					std::vector<pt3D>& pta,
					std::vector<Hexa>& hex,
					std::vector<Quad>& tansitionQuad,
					pt3D midPoint
					)
{
	const double point9(0.9), point1(0.1);
	pt3D prevPt;
	pt3D pt;
	size_t i(4), j(4); 
	size_t is, js;
	bool found(false);

	// Determine the shared edge
	for ( is = 0; is < 4 && !found; ++ is )
	{
		for ( js = 0 ; js < 4 && !found; ++js )
			if ( left.idx[is] == right.idx[ plus1[js] ] &&
				 left.idx[ plus1[is] ] == right.idx[js] )
			{
				i = is;
				j = js;
				found = true;
			}
	}
	if ( !found )
	{
		throw 1;
	}

	//--- Add the paired elements

	// add right hexahedra
	Hexa hr;
	hr.idx[0] = right.idx[ plus1[j + 2] ];
	hr.idx[1] = right.idx[       j      ];
	hr.idx[2] = right.idx[ plus1[j    ] ];
	hr.idx[3] = right.idx[ plus1[j + 1] ];

	pt3D pt0 = pta[ hr.idx[ 1 ] ];
	pt3D pt1 = pta[ hr.idx[ 2 ] ];

	pt3D ptAvg(twoFiths*pt1 + threeFiths*pt0);
	pt = point9*ptAvg + point1* midPoint; 
	prevPt = pt;

	hr.idx[4] = pta.size(); // four
	pta.push_back(pt);

	pt = 0.5*prevPt + 0.5*pt0;
	hr.idx[5] = pta.size(); // five
	pta.push_back(pt);

	ptAvg = threeFiths*pt1 + twoFiths*pt0;
	prevPt = point9*ptAvg + point1* midPoint;

	pt = 0.5*pt1 + 0.5*prevPt;
	hr.idx[6] = pta.size(); // six
	pta.push_back(pt);

	//pt = 0.8*(threeFiths*pt1 + twoFiths*pt0) + 0.2* midPoint;
	hr.idx[7] = pta.size(); // seven
	pta.push_back(prevPt);

	hex.push_back(hr);

	// add left hexahedra
	Hexa hl;
	hl.idx[0] = left.idx[ plus1[i    ] ];
	hl.idx[1] = left.idx[ plus1[i + 1] ];
	hl.idx[2] = left.idx[ plus1[i + 2] ];
	hl.idx[3] = left.idx[       i      ];

	hl.idx[4] = hr.idx[5];	// six
	hl.idx[5] = hr.idx[4];	// five
	hl.idx[6] = hr.idx[7];	// four
	hl.idx[7] = hr.idx[6];	// seven

	hex.push_back(hl);

	Quad q;
	size_t group( tansitionQuad.size()/6 );
	q.group = group;
	for ( i = 0; i< 3; ++i )
	{
		for ( j = 0; j < 4; ++j )
		{
			size_t k = pairedTransition[0][i][j];
			q.idx[j] = hl.idx[ k ];
		}
		q.referenceQuad = 2*i;
		tansitionQuad.push_back( q );

		for ( j = 0; j < 4; ++j )
		{
			size_t k = pairedTransition[1][i][j];
			q.idx[j] = hr.idx[ k ];
		}
		q.referenceQuad = 2*i+1;
		tansitionQuad.push_back( q );
	}
}

void PairedElement::PrintDebug()
{
	size_t i;
	cout << "Paired Element Mesh = { \n";

	for ( i = 0; i < 2; ++i )
	{
		hex[i].PrintDebug(true);
	}

	for ( i = 0; i < 2; ++i )
	{
		hex[i].PrintDebug(m_pta);
	}

	for (i = 0; i < 10; ++i)
	{
		m_pta[i].PrintDebug();
	}
}
