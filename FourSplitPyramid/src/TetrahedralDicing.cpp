#include "stdafx.h"
#include <assert.h>
#include <map>
#include <math.h>
#include "inc\tetrahedraldicing.h"

namespace
{
	size_t p1[] = {1, 2, 3, 0, 1, 2, 3, 0};

	size_t basic[7]={ 
					 1-1, 9-1, 3-1,
					 5-1, 2-1, 7-1,
					11-1 };
	/*size_t basic[7]={ 
					 2-1, 5-1, 3-1,
					 9-1, 1-1, 7-1,
					11-1 };*/

	const double third(1./3.);

	/**NODE*/
	double pt[15][3] = {
		/* 1*/{             1.0,             0.0,             0.0},
		/* 2*/{             0.5,        0.288675,      0.81649658},
		/* 3*/{             0.0,             0.0,             0.0},
		/* 4*/{             0.5,        0.866025,             0.0},
		/* 5*/{            0.25,       0.1443375,      0.40824829},
		/* 6*/{             0.5,         0.57735,      0.40824829},
		/* 7*/{            0.75,       0.1443375,      0.40824829},
		/* 8*/{            0.25,       0.4330125,             0.0},
		/* 9*/{             0.5,             0.0,             0.0},
		/*10*/{             0.5,        0.288675,             0.0},
		/*11*/{             0.5,        0.096225, 0.2721655266667},
		/*12*/{ 0.3333333333333,          0.3849, 0.2721655266667},
		/*13*/{            0.75,       0.4330125,             0.0},
		/*14*/{ 0.6666666666667,          0.3849, 0.2721655266667},
		/*15*/{             0.5,        0.288675,     0.204124145}
	};
//*ELEMENT_SOLID
	int idx[4][8] = 
	{
	/*1*/{ 4-1, 8-1,10-1,13-1, 6-1,12-1,15-1,14-1},
	/*2*/{ 3-1, 9-1,10-1, 8-1, 5-1,11-1,15-1,12-1},
	/*3*/{ 1-1,13-1,10-1, 9-1, 7-1,14-1,15-1,11-1},
	/*4*/{ 2-1, 6-1,14-1, 7-1, 5-1,12-1,15-1,11-1}
	};

	// mid-faces
	int midFaces[3][4] =
	{
		{ 14-1, 2-1, 1-1, 4-1},
		{ 10-1, 4-1, 1-1, 3-1},
		{ 12-1, 2-1, 4-1, 3-1},
	};

	// mid-lines
	int midLines[3][3] =
	{
		{ 6-1, 4-1, 2-1},
		{ 8-1, 4-1, 3-1},
		{13-1, 4-1, 1-1},
	};
};

TetrahedralDicing::TetrahedralDicing(void)
{
	size_t i;
	Hexa h;

	for ( i = 0; i < 15; ++i )
		pta.push_back( pt3D(pt[i][0], pt[i][1], pt[i][2]) );

	for ( i = 0; i <  4; ++i )
	{
		for ( size_t j =0; j < 8; ++j )
			h.idx[j] = idx[i][j];
		hex.push_back(h);
	}
}

TetrahedralDicing::~TetrahedralDicing(void)
{
}

void TetrahedralDicing::FillCellOfThree(
		size_t midPointIdx,
		std::vector<Hexa>& hexa,
		std::vector<pt3D>& pta,
		Quad threeSplitCell[3]
)
{
		static TetrahedralDicing dicedTet;
		size_t nd[7];
		size_t i, j;
		std::map<size_t,size_t> connectivity;
		//--- Count connectivity
		for ( j = 0; j < 3; ++ j)
		{
			for ( size_t k=0 ; k < 4; ++k )
			{
				size_t qnd = threeSplitCell[j].idx[k];
				connectivity[qnd]=0;
			}
		}

		for ( j = 0; j < 3; ++ j)
		{
			for ( size_t k=0 ; k < 4; ++k )
			{
				size_t qnd = threeSplitCell[j].idx[k];
				connectivity[qnd]++;
			}
		}

		//--- Find the center-point
		bool found = false;
		for( j = 0; j < 4; ++j )
		{
			size_t qnd = threeSplitCell[0].idx[j];
			if ( connectivity[qnd] == 3 )
				nd[6] = qnd;
			if ( connectivity[qnd] == 1 )
			{
				nd[0] =qnd;
				nd[1] = threeSplitCell[0].idx[p1[j]];
			}
		}

		//--- find nodes 2 and 3
		size_t qidx = 0;
		for ( size_t ii =1; ii < 6; ii+=2)
		{
			found = false;
			for ( size_t j = 1; j < 3 && !found; ++j )
				if ( j != qidx )
				{
					for ( size_t k = 0; k < 4 && !found; ++k )
					{
						size_t qnd = threeSplitCell[j].idx[k];
						if ( qnd == nd[ii] )
						{
							found = true;
							qidx = j;
							size_t kk = p1[k];
							nd[ii+1] = threeSplitCell[j].idx[ kk ];
							kk = p1[kk];
							nd[ii+2] = threeSplitCell[j].idx[ kk ];
						}
					}
				}
		}

		// Fill the apex
		size_t ptIdx[15];
		ptIdx[4-1] = midPointIdx;

		//--- Fill the coordinates of the tetrahedra
		for ( i = 0; i < 7; ++i )
			ptIdx[basic[i]] = nd[i];

		//--- Fill the mid-lines; bottom edges come from input face
		for ( i = 0; i < 3; ++i )
		{
			pt3D pt = 0.5*( pta[ ptIdx[ midLines[i][1] ] ] + 
							pta[ ptIdx[ midLines[i][2] ] ] );
			ptIdx[ midLines[i][0] ] = AddPoint(pt,pta);
		}

		//--- Fill the mid-faces; bottom face is input face
		for ( i = 0; i < 3; ++i )
		{
			pt3D pt = third*( pta[ ptIdx[ midFaces[i][1] ] ] +
						pta[ ptIdx[ midFaces[i][2] ] ] +
						pta[ ptIdx[ midFaces[i][3] ] ] );
			ptIdx[ midFaces[i][0] ]= AddPoint(pt,pta);
		}
		//--- Fill the mid-point
		pt3D pt = 0.25*(pta[ptIdx[0]]+pta[ptIdx[1]]+pta[ptIdx[2]]+pta[ptIdx[3]]);
		ptIdx[ 15-1 ] = AddPoint(pt,pta);

		//--- The subroutine AddPoint prevents duplicate points.

		for ( i = 0; i < dicedTet.hex.size(); ++i )
		{
			Hexa hx;
			for ( j=0; j < 8; ++j )
			{
				hx.idx[j] = ptIdx[ dicedTet.hex[i].idx[j] ];
			}
			hx.group = 5;
			hexa.push_back(hx);
		}
	
}