//
//
//   Copyright 2006 (C) Sandia National Laboratories All rights reserved 
//
//
#include "stdafx.h"
#include <assert.h>
#include <map>
#include <math.h>
#include "inc\geodetemplate.h"
#include "inc\tetrahedraldicing.h"

namespace
{
	const double tol(1.e-6);
	const double tolSqr(tol*tol);
	const size_t p1[] = {1, 2, 3, 0, 1, 2, 3, 0};
	const double half(0.5);
	const double third(1./3.), twoThirds(2./3.);

	size_t basic[9]={ 
					21-1, 17-1, 16-1,
					27-1, 15-1, 11-1,
					10-1, 22-1, 31-1 };

	size_t basicHex[8] = { 10-1, 15-1, 16-1, 21-1, 8-1, 1-1, 4-1, 2-1 };

	size_t dicedTetFace[2][3][4] =
	{
		{ {5-1,4-1,6-1,7-1}, {1-1,5-1,7-1,3-1}, {3-1,7-1,6-1,2-1} },
		{ {1-1,3-1,32-1,9-1}, {32-1,3-1,2-1,26-1}, {8-1,9-1,32-1,26-1} }
	};

	bool ptSet[48]= {
		false, // 1
		false, // 2
		false, // 3
		false, // 4
		false, // 5
		false, // 6
		false, // 7
		false, // 8
		false, // 9
		false, // 10
		false, // 11
		false, // 12
		false, // 13
		false, // 14
		false, // 15
		false, // 16
		false, // 17
		false, // 18
		false, // 19
		false, // 20
		false, // 21
		false, // 22
		false, // 23
		false, // 24
		false, // 25
		false, // 26
		false, // 27
		false, // 28
		false, // 29
		false, // 30
		false, // 31
		false, // 32
		false, // 33
		false, // 34
		false, // 35
		false, // 36
		false, // 37
		false, // 38
		false, // 39
		false, // 40
		false, // 41 
		false, // 42
		false, // 43
		false, // 44
		false, // 45
		false, // 46
		false, // 47
		false  // 48
	};

	double pt[48][3] = {
		{-12.0,  7.0, 12.0}, // 1
		{ 12.0,  7.0,-12.0}, // 2
		{  0.0,  7.0,  0.0}, // 3
		{ 12.0,  7.0, 12.0}, // 4
		{  0.0,  7.0, 12.0}, // 5
		{ 12.0,  7.0,  0.0}, // 6
		{  4.6,  7.0,  4.6}, // 7
		{-12.0,  7.0,-12.0}, // 8
		{-12.0,  7.0,  0.0}, // 9
		{-12.0, -7.0,-12.0}, // 10
		{-12.0, -7.0,  0.0}, // 11
		{-12.0,  1.7, -5.0}, // 12
		{-12.0,  4.5,  0.0}, // 13
		{-12.0,  1.7,  5.0}, // 14
		{-12.0, -7.0, 12.0}, // 15
		{ 12.0, -7.0, 12.0}, // 16
		{ 12.0, -7.0,  0.0}, // 17
		{ 12.0,  1.7,  5.0}, // 18
		{ 12.0,  4.5,  0.0}, // 19
		{ 12.0,  1.7, -5.0}, // 20
		{ 12.0, -7.0,-12.0}, // 21
		{  0.0, -7.0,-12.0}, // 22
		{  5.0,  1.7,-12.0}, // 23
		{  0.0,  4.5,-12.0}, // 24
		{ -5.0,  1.7,-12.0}, // 25
		{  0.0,  7.0,-12.0}, // 26
		{  0.0, -7.0, 12.0}, // 27
		{ -5.0,  1.7, 12.0}, // 28
		{  0.0,  4.5, 12.0}, // 29
		{  5.0,  1.7, 12.0}, // 30
		{  0.0, -7.0,  0.0}, // 31
		{ -4.6,  7.0, -4.6}, // 32
		{  3.9, -1.8, -3.6}, // 33
		{ -3.9, -1.8,  3.7}, // 34
		{ -4.1, -1.5, -4.1}, // 35
		{ -0.2,  2.5, -5.3}, // 36
		{  0.0,  3.9, -6.5}, // 37 : { -8.5,  3.9, -6.5}
		{ -4.3,  4.2, -2.3}, // 38
		{  0.0,  3.5,  0.0}, // 39
		{  4.4,  4.3,  2.3}, // 40
		{  0.0,  3.9,  6.6}, // 41 : {  8.5,  3.9,  6.6}
		{  0.2,  2.5,  5.4}, // 42
		{  4.1, -1.5,  4.2}, // 43
		{  5.7,  0.7,  0.7}, // 44
		{  2.6,  1.4,  2.1}, // 45
		{  0.0,  0.0,  0.0}, // 46
		{ -2.6,  1.4, -2.0}, // 47
		{ -5.6,  0.7, -0.6}  // 48
	};

	// Hex n1 n2 n3 n4 n5 n6 n7 n8
	int idx[26][8] = {
		{17-1,20-1,2-1,21-1,31-1,33-1,23-1,22-1},
		{43-1,18-1,17-1,31-1,44-1,19-1,20-1,33-1},
		{43-1,30-1,27-1,31-1,18-1,4-1,16-1,17-1},
		{36-1,24-1,26-1,37-1,33-1,23-1,2-1,20-1},
		{37-1,26-1,32-1,38-1,20-1,2-1,3-1,39-1},
		{19-1,6-1,2-1,20-1,40-1,7-1,3-1,39-1},
		{45-1,40-1,19-1,44-1,46-1,39-1,20-1,33-1},
		{46-1,39-1,20-1,33-1,47-1,38-1,37-1,36-1},
		{33-1,23-1,22-1,31-1,36-1,24-1,25-1,35-1},
		{48-1,47-1,46-1,34-1,35-1,36-1,33-1,31-1},
		{34-1,46-1,45-1,42-1,31-1,33-1,44-1,43-1},
		{31-1,22-1,10-1,11-1,35-1,25-1,8-1,12-1},
		{42-1,29-1,28-1,34-1,43-1,30-1,27-1,31-1},
		{48-1,13-1,12-1,35-1,34-1,14-1,11-1,31-1},
		{28-1,27-1,15-1,1-1,34-1,31-1,11-1,14-1},
		{41-1,5-1,1-1,14-1,42-1,29-1,28-1,34-1},
		{38-1,32-1,9-1,13-1,39-1,3-1,1-1,14-1},
		{40-1,7-1,3-1,39-1,41-1,5-1,1-1,14-1},
		{46-1,39-1,14-1,34-1,45-1,40-1,41-1,42-1},
		{47-1,38-1,13-1,48-1,46-1,39-1,14-1,34-1},
		{36-1,24-1,25-1,35-1,37-1,26-1,8-1,12-1},
		{47-1,38-1,37-1,36-1,48-1,13-1,12-1,35-1},
		{37-1,26-1,8-1,12-1,38-1,32-1,9-1,13-1},
		{41-1,5-1,4-1,18-1,40-1,7-1,6-1,19-1},
		{42-1,41-1,18-1,43-1,45-1,40-1,19-1,44-1},
		{42-1,29-1,30-1,43-1,41-1,5-1,4-1,18-1}
	};

	void ClassifyPt(pt3D& pt)
	{
		//--- set x-coordinates
		if ( fabs(pt.x) < tol )
		{
			pt.x = 0.0;
		} 
		else if ( fabs(pt.x-0.5) < tol)
		{
			pt.x = 0.5;
		}
		else if ( fabs(pt.x-1) < tol )
		{
			pt.x = 1;
		}

		//--- set y-coordinates
		if ( fabs(pt.y) < tol )
		{
			pt.y = 0.0;
		} 
		else if ( fabs(pt.y-0.5) < tol )
		{
			pt.y = 0.5;
		}
		else if ( fabs(pt.y-1) < tol )
		{
			pt.y = 1;
		}

		//--- set z-coordinates
		if ( fabs(pt.z) < tol )
		{
			pt.z = 0;
			pt.type = pt3D::floor;
		}
		else if ( fabs(pt.z-1) < tol )
		{
			pt.z = 1;
			pt.type = pt3D::top;
		}
	}
};

GeodeTemplate::GeodeTemplate(void)
{
	size_t i;
	Hexa h;
	for ( i = 0; i <  48; ++i )
	{
		pt3D pt((pt[i][2]+12)/24, (pt[i][0]+12)/24, (pt[i][1]+7)/14);
		ClassifyPt(pt);
		pta.push_back( pt );
	}

	for ( i = 0; i <  26; ++i )
	{
		for ( size_t j =0; j < 8; ++j )
			h.idx[j] = idx[i][j];
		hex.push_back(h);
	}

	for( i =0; i < 9; ++i )
		ptSet[ basic[i] ] = true;
	for( i = 0; i < 8; ++i )
		ptSet[ basicHex[i] ] = true;
}

GeodeTemplate::~GeodeTemplate(void)
{
}

void GeodeTemplate::FillFourSplits(
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

void GeodeTemplate::FillCellOfFourSplit(
	size_t midPointIdx,
	std::vector<Hexa>& hexa,
	std::vector<pt3D>& ptaList,
	Quad fourSplitCell[4]
	)
	{
		static GeodeTemplate geode;
		static GeodeTemplate geodeCannonical;
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
			{
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
		}

		//--- Fill the coordinates of the geodeamid
		std::vector< size_t > ptIdx(geode.pta.size());
		for ( i = 0; i < 9; ++i )
			ptIdx[basic[i]] = nd[i];

		//--- fill the coordinates of the mid-points on the surface
		for ( i = 0; i < 9; ++i )
			geode.pta[basic[i]] = ptaList[ nd[i] ];

		pt3D& pt0 = third*ptaList[midPointIdx];
		std::vector< pt3D > eightNodes(8);

		for ( i = 0; i < 4; ++i )
		{
			pt3D& pt1 = ptaList[ ptIdx[ basicHex[i] ] ];
			eightNodes[i] = pt1;
			geode.pta[ basicHex[i+4]] = pt0 + twoThirds*pt1;
			eightNodes[i+4] = geode.pta[ basicHex[i+4]];

		}

		//--- Fill Internal the mid line coordinates
		for ( i = 0; i < geode.pta.size(); ++i )
		{
			if ( !ptSet[i] )
			{
				geode.pta[i] = Hexa::Interpolate(eightNodes, 
								geodeCannonical.pta[i].x,
								geodeCannonical.pta[i].y,
								geodeCannonical.pta[i].z);
			}
		}

		//--- Fill Internal Points

		//--- Numerical corrections (??? need to clean the code above)

		//--- The subroutine AddPoint prevents duplicate points.
		for ( i = 0; i < geode.pta.size(); ++i )
		{
			size_t nd = AddPoint(geode.pta[i], ptaList);
			ptIdx[i] = nd;
		}

		for ( i = 0; i < 26; ++i )
		{
			Hexa hx;
			for ( j=0; j < 8; ++j )
			{
				hx.idx[j] = ptIdx[ geode.hex[i].idx[j] ];
			}
			hexa.push_back(hx);
		}

		//--- Add tet split

		for ( i =0; i < 2; ++i )
		{
			Quad threeSplitCell[3];
			for ( j = 0 ; j < 3; ++j )
			{
				for ( size_t k = 0; k < 4; ++k )
					threeSplitCell[j].idx[k] = ptIdx[ dicedTetFace[i][j][k] ];
			}
 			TetrahedralDicing::FillCellOfThree(midPointIdx, hexa,ptaList,threeSplitCell);
		}
		//geode.PrintDebug();
	}
