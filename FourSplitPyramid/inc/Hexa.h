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
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HEXA_H__C03F334B_61CE_4AEF_8CF9_9C71C1E13E7F__INCLUDED_)
#define AFX_HEXA_H__C03F334B_61CE_4AEF_8CF9_9C71C1E13E7F__INCLUDED_

#include <vector>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "pt3D.h"

struct Quad;

struct Hexa
{
	Hexa():group(0)	{}

	Hexa(int i0, int i1, int i2, int i3,
		 int i4, int i5, int i6, int i7 )
	{
		idx[0] = i0;
		idx[1] = i1;
		idx[2] = i2;
		idx[3] = i3;
		idx[4] = i4;
		idx[5] = i5;
		idx[6] = i6;
		idx[0] = i7;
	}

	~Hexa(){}

	void GetQuads(std::vector<Quad>& quads);

	void SetFirstFace(size_t i);

	double volume(int i, const std::vector<pt3D>& points) const;

	static double MinDet(
		const std::vector<pt3D>& pta,
		std::vector< Hexa >& hex);

	static pt3D Interpolate(const std::vector< pt3D > &eightNodes,
					double u, double v, double w );

	double MinDet(const std::vector<pt3D>& pta);

	static double volume(pt3D inPt[4] );

	static double MinVolume(
		const std::vector<pt3D>& pta,
		std::vector< Hexa >& hex);

	void PrintDebug(bool OK);
	void PrintDebug(std::vector<pt3D>& pta);

	int idx[8];
	size_t group;
};


#endif // !defined(AFX_HEXA_H__C03F334B_61CE_4AEF_8CF9_9C71C1E13E7F__INCLUDED_)
