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
// Pyramid.cpp: implementation of the Pyramid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <assert.h>
#include <map>
#include <math.h>
#include <iostream>
#include <fstream>
#include "inc/MeshIo.h"
#include "inc/Pyramid.h"
#include "inc/Quad.h"

using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
namespace
{

	size_t p1[] = {1, 2, 3, 0, 1, 2, 3, 0};

	size_t basic[9]={ 
					27-1, 18-1, 19-1,
					 8-1,  7-1,  6-1,
					24-1, 25-1,  5-1 };

	size_t averageIdx[4][3] =
	{
		{26-1, 14-1, 25-1},
		{13-1, 14-1,  6-1},
		{15-1, 14-1,  8-1},
		{21-1, 14-1, 18-1}
	};
	
	const double half(0.5);

	const double third(1./3.), twoThirds(2./3.);
	const double fourth(0.25), threeFourths(0.75);
	// double mlF(4.5);
	double mlF[5][2] = { 
		{1, 0}, 
		{3.25/4.5, 1.25/4.5}, 
		//{half, half}, 
		{2.25/4.5, 2.25/4.5}, 
		{1.75/4.5, 2.75/4.5}, 
		{1.25/4.5, 3.25/4.5} 
	};
	size_t midLine[6] = { 14-1, 11-1, 12-1, 9-1, 1-1, 5-1 };

	size_t IntFloorCorner[4][3] =
	{
		{17-1, 12-1, 19-1},
		{28-1, 12-1, 27-1},
		{22-1, 12-1, 24-1},
		{ 3-1, 12-1,  7-1}
	};

	size_t midFlrEdgs[4][3] =
	{
		{23-1, 28-1, 22-1},
		{ 2-1, 22-1,  3-1},
		{ 4-1,  3-1, 17-1},
		{16-1, 17-1, 28-1}
	};

	

};

Pyramid::Pyramid():npta(0),hex(16)
{
	size_t nhx(0);
	InitFirstQuadrant( nhx, hex, pta);

	Mirror(1,-1,nhx);

	Mirror(-1,1,nhx);

}

Pyramid::~Pyramid()
{

}

void Pyramid::InitFirstQuadrant( 
	size_t& nhx,
	std::vector<Hexa>& hex,
	std::vector<pt3D>& pta)
{
	pt3D point;
	size_t i;

  // Element 1:
	i = 0;

	point.x =    0.00000000000;
	point.y =    0.00000000000;
	point.z =    1.25000000000;
	hex[nhx].idx[i++] = AddPoint(point, pta);

	point.x =    0.00000000000;    
	point.y =   -1.00000000000;
	point.z =    1.25000000000;
	hex[nhx].idx[i++] = AddPoint(point, pta);

	point.x =    1.00000000000;
	point.y =   -1.00000000000;
	point.z =    1.25000000000;
	hex[nhx].idx[i++] = AddPoint(point, pta);

	point.x =    1.00000000000;
	point.y =    0.00000000000;
	point.z =    1.25000000000;
	hex[nhx].idx[i++] = AddPoint(point, pta);

	point.x =    0.00000000000;
	point.y =    0.00000000000;
	point.z =    0.00000000000;
	point.type = pt3D::floor;
	hex[nhx].idx[i++] = AddPoint(point, pta);

	point.x =    0.00000000000;
	point.y =   -2.25000000000;
	point.z =    0.00000000000;
	point.type = pt3D::floor;
	point.orient = pt3D::south;
	hex[nhx].idx[i++] = AddPoint(point, pta);

	point.x =    2.25000000000;
	point.y =   -2.25000000000;
	point.z =    0.00000000000;
	point.type = pt3D::floor;
	point.orient = pt3D::southEast;
	hex[nhx].idx[i++] = AddPoint(point, pta);

	point.x =    2.25000000000;
	point.y =    0.00000000000;
	point.z =    0.00000000000;
	point.type = pt3D::floor;
	point.orient = pt3D::east;
	hex[nhx].idx[i++] = AddPoint(point, pta);
	nhx++;

  // Element 2
	i = 0;

	point.x =    0.00000000000;
	point.y =    0.00000000000;
	point.z =    1.75000000000;
	hex[nhx].idx[i++] = AddPoint(point, pta);

	point.x =    0.00000000000;
	point.y =   -0.50000000000;
	point.z =    2.25000000000;
	hex[nhx].idx[i++] = AddPoint(point, pta);

	point.x =    0.00000000000;
	point.y =    0.00000000000;
	point.z =    3.25000000000;
	hex[nhx].idx[i++] = AddPoint(point, pta);

	point.x =    0.00000000000;
	point.y =    0.00000000000;
	point.z =    2.25000000000;
	hex[nhx].idx[i++] = AddPoint(point, pta);

	point.x =    0.00000000000;
	point.y =    0.00000000000;
	point.z =    1.25000000000;
	hex[nhx].idx[i++] = AddPoint(point, pta);

	point.x =    0.00000000000;
	point.y =   -1.00000000000;
	point.z =    1.25000000000;
	hex[nhx].idx[i++] = AddPoint(point, pta);

	point.x =    1.00000000000;
	point.y =   -1.00000000000;
	point.z =    1.25000000000;
	hex[nhx].idx[i++] = AddPoint(point, pta);

	point.x =    1.00000000000;
	point.y =    0.00000000000;
	point.z =    1.25000000000;
	hex[nhx].idx[i++] = AddPoint(point, pta);
	nhx++;

  // Element 3:
	i = 0;

	point.x =    0.00000000000;
	point.y =   -1.00000000000;
	point.z =    1.25000000000;
	hex[nhx].idx[i++] = AddPoint(point, pta);

	point.x =    0.00000000000;
	point.y =   -0.50000000000;
	point.z =    2.25000000000;
	hex[nhx].idx[i++] = AddPoint(point, pta);

	point.x =    0.00000000000;
	point.y =    0.00000000000;
	point.z =    3.25000000000;
	hex[nhx].idx[i++] = AddPoint(point, pta);

	point.x =    1.00000000000;
	point.y =   -1.00000000000;
	point.z =    1.25000000000;
	hex[nhx].idx[i++] = AddPoint(point, pta);

	point.x =    0.00000000000;
	point.y =   -2.25000000000;
	point.z =    0.00000000000;
	point.type = pt3D::floor;
	point.orient = pt3D::south;
	hex[nhx].idx[i++] = AddPoint(point, pta);

	point.x =    0.00000000000;
	point.y =   -1.12500000000;
	point.z =    2.25000000000;
	point.type = pt3D::face;
	point.orient = pt3D::south;
	hex[nhx].idx[i++] = AddPoint(point, pta);

	point.x =    0.00000000000;
	point.y =    0.00000000000;
	point.z =    4.50000000000;
	hex[nhx].idx[i++] = AddPoint(point, pta);

	point.x =    2.25000000000;
	point.y =   -2.25000000000;
	point.z =    0.00000000000;
	point.type = pt3D::floor;
	point.orient = pt3D::southEast;
	hex[nhx].idx[i++] = AddPoint(point, pta);
	nhx++;

  // Element 4
	i = 0;

	point.x =    0.00000000000;
	point.y =    0.00000000000;
	point.z =    2.25000000000;
	hex[nhx].idx[i++] = AddPoint(point, pta);

	point.x =    0.00000000000;
	point.y =    0.00000000000;
	point.z =    3.25000000000;
	hex[nhx].idx[i++] = AddPoint(point, pta);

	point.x =    0.00000000000;
	point.y =    0.00000000000;
	point.z =    4.50000000000; // Apex
	point.type = pt3D::apex;
	hex[nhx].idx[i++] = AddPoint(point, pta);

	point.x =    1.12500000000;
	point.y =    0.00000000000;
	point.z =    2.25000000000;
	point.type = pt3D::face;
	point.orient = pt3D::east;
	hex[nhx].idx[i++] = AddPoint(point, pta);

	point.x =    1.00000000000;
	point.y =    0.00000000000;
	point.z =    1.25000000000;
	hex[nhx].idx[i++] = AddPoint(point, pta);

	point.x =    1.00000000000;
	point.y =   -1.00000000000;
	point.z =    1.25000000000;
	hex[nhx].idx[i++] = AddPoint(point, pta);

	point.x =    2.25000000000;
	point.y =   -2.25000000000;
	point.z =    0.00000000000;
	point.type = pt3D::floor;
	point.orient = pt3D::southEast;
	hex[nhx].idx[i++] = AddPoint(point, pta);

	point.x =    2.25000000000;
	point.y =    0.00000000000;
	point.z =    0.00000000000;
	point.type = pt3D::floor;
	point.orient = pt3D::east;
	hex[nhx].idx[i++] = AddPoint(point, pta);
	nhx++;

	for ( size_t elem =0; elem < nhx; ++elem )
	{
		for ( size_t j = 0; j < 4; ++j )
			std::swap(hex[elem].idx[j], hex[elem].idx[j+4]);
	}
}

void Pyramid::Mirror(int xM, int yM, size_t& nhx)
{
	size_t nhxTmp(nhx);
	pt3D pt;
	for ( size_t i = 0 ; i < nhxTmp; ++i )
	{
		size_t j;
		Hexa& h = hex[i];
		for (j = 4; j < 8; ++j)
		{
			pt3D point( pta[ h.idx[j] ] );
			point.x = xM*point.x;
			point.y = yM*point.y;
			point.orient = pt3D::MirrorOrientation(point.orient,xM, yM);

			hex[nhx].idx[j-4] = AddPoint(point, pta);
		}

		for (j = 0; j < 4; ++j)
		{
			pt3D point( pta[ h.idx[j] ] );
			point.x = xM*point.x;
			point.y = yM*point.y;
			point.orient = pt3D::MirrorOrientation(point.orient,xM, yM);

			hex[nhx].idx[j+4] = AddPoint(point, pta);
		}
		nhx++;
	}
}

void Pyramid::FillFourSplits(
	const pt3D& midPoint,
	std::vector<Hexa>& hexa,
	std::vector<pt3D>& ptaList,
	std::vector<Quad>& fourSplit
	)
{
	size_t midPointIdx = ptaList.size();
	ptaList.push_back(midPoint);
	assert(ptaList.size() > midPointIdx );

	for (size_t j = 0; j < fourSplit.size(); j+=4)
	{
		Quad fourSplitCell[4];
		for ( size_t k =0; k < 4; ++k )
			fourSplitCell[k] = fourSplit[j+k];
		FillCellOfFourSplit( midPointIdx, hexa, ptaList, fourSplitCell);
		//return;
	}
}

void Pyramid::FillCellOfFourSplit(
	size_t midPointIdx,
	std::vector<Hexa>& hexa,
	std::vector<pt3D>& ptaList,
	Quad fourSplitCell[4]
	)
	{
		static Pyramid pyr;
		size_t nd[9];
		size_t i, j;
		std::map<size_t,size_t> connectivity;
		//--- Count connectivity
		for ( j = 0; j < 4; ++ j)
		{
			for ( size_t k=0 ; k < 4; ++k )
			{
				size_t qnd = fourSplitCell[j].idx[k];
				connectivity[qnd]=0;
			}
		}

		for ( j = 0; j < 4; ++ j)
		{
			for ( size_t k=0 ; k < 4; ++k )
			{
				size_t qnd = fourSplitCell[j].idx[k];
				connectivity[qnd]++;
			}
		}

		//--- Find the center-point
		bool found = false;
		for( j = 0; j < 4; ++j )
		{
			size_t qnd = fourSplitCell[0].idx[j];
			if ( connectivity[qnd] == 4 )
				nd[8] = qnd;
			if ( connectivity[qnd] == 1 )
			{
				nd[0] =qnd;
				nd[1] = fourSplitCell[0].idx[p1[j]];
			}
		}

		//--- find nodes 2 and 3
		size_t qidx = 0;
		for ( size_t ii =1; ii < 8; ii+=2)
		{
			found = false;
			for ( size_t j = 1; j < 4 && !found; ++j )
				if ( j != qidx )
				{
					for ( size_t k = 0; k < 4 && !found; ++k )
					{
						size_t qnd = fourSplitCell[j].idx[k];
						if ( qnd == nd[ii] )
						{
							found = true;
							qidx = j;
							size_t kk = p1[k];
							nd[ii+1] = fourSplitCell[j].idx[ kk ];
							kk = p1[kk];
							nd[ii+2] = fourSplitCell[j].idx[ kk ];
						}
					}
				}
		}

		//--- Fill the coordinates of the pyramid
		std::vector< size_t > ptIdx(pyr.pta.size());
		for ( i = 0; i < 9; ++i )
			ptIdx[basic[i]] = nd[i];
		ptIdx[14-1] = midPointIdx;

		//--- fill the coordinates of the mid-points on the surface
		for ( i = 0; i < 9; ++i )
			pyr.pta[basic[i]] = ptaList[ nd[i] ];

		pt3D& pt0 = twoThirds*ptaList[midPointIdx];
		for ( i = 0; i < 4; ++i )
		{
			size_t i0(averageIdx[i][0]);
			size_t i2(averageIdx[i][2]);
			pt3D& pt1 = ptaList[ ptIdx[ i2 ] ];
			pyr.pta[ i0 ] = pt0 + third*pt1;
		}

		//--- Fill Apex
		pyr.pta[14-1] = ptaList[midPointIdx];

		//--- Fill Internal the mid line coordinates
		double step(0.2);
		for ( i = 1; i < 5; ++i )
		{
#ifdef OLD
#define OLD
			double factor(i*step);
			double oneMinusFactor(1.-factor);
			pyr.pta[ midLine[i] ] = oneMinusFactor*pyr.pta[midLine[0]]+
									factor*pyr.pta[midLine[5]];
#else
			pyr.pta[ midLine[i] ] = mlF[i][0]*pyr.pta[midLine[0]]+
									mlF[i][1]*pyr.pta[midLine[5]];
#endif
		}

		//--- Fill Internal Points
		for ( i = 0; i < 4; ++i )
		{
			pyr.pta[ IntFloorCorner[i][0] ] = half*(pyr.pta[IntFloorCorner[i][1]]+
									pyr.pta[IntFloorCorner[i][2]]);
		}

		//--- Fill Internal Points
		for ( i = 0; i < 4; ++i )
		{
			pyr.pta[ midFlrEdgs[i][0] ] = half*(pyr.pta[midFlrEdgs[i][1]]+
									pyr.pta[midFlrEdgs[i][2]]);
		}

		pyr.pta[20-1] = half*(pyr.pta[16-1]+pyr.pta[11-1]);
		pyr.pta[10-1] = half*(pyr.pta[ 2-1]+pyr.pta[11-1]);

		//--- Numerical corrections (??? need to clean the code above)

		//--- The subroutine AddPoint prevents duplicate points.
		for ( i = 0; i < pyr.pta.size(); ++i )
		{
			size_t nd = AddPoint(pyr.pta[i], ptaList);
			ptIdx[i] = nd;
		}

		for ( i = 0; i < 16; ++i )
		{
			Hexa hx;
			for ( j=0; j < 8; ++j )
			{
				hx.idx[j] = ptIdx[ pyr.hex[i].idx[j] ];
			}
			hexa.push_back(hx);
		}
		//pyr.PrintDebug();
	}

void Pyramid::PrintDebug()
{
	size_t i;
	std::vector<Hexa> hexa;
	cout << "Pyramid Mesh = { \n";

	for ( i = 0; i < 16; ++i )
	{
		hexa.push_back(hex[i]);
		hex[i].PrintDebug(i < 15 ? true: false);
	}

	for ( i = 0; i < 16; ++i )
	{
		hex[i].PrintDebug(pta);
	}
	for (i = 0; i < pta.size(); ++i)
	{
		pta[i].PrintDebug();
	}

	WriteFile("C:\\Meshing\\FourSplitPyramid\\Pyramid.dyn", pta, hexa);
}