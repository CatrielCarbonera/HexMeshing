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
// Pyramid.h: interface for the Pyramid class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFx_PyRAMID_H__050F05DC_1297_4BE9_A3E1_903AC2D05AA9__INCLUDED_)
#define AFx_PyRAMID_H__050F05DC_1297_4BE9_A3E1_903AC2D05AA9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <vector>
#include "pt3D.h"
#include "Hexa.h"

/*
 */

class Pyramid  
{
public:

	Pyramid();

	virtual ~Pyramid();

	size_t npta;

	std::vector<pt3D> pta;

	std::vector<Hexa> hex;

	void PrintDebug();

	static void	FillFourSplits(
		const pt3D& midPoint,
		std::vector<Hexa>& hexa,
		std::vector<pt3D>& pta,
		std::vector<Quad>& fourSplit
		);

	static void	FillCellOfFourSplit(
		size_t midPointIdx,
		std::vector<Hexa>& hexa,
		std::vector<pt3D>& pta,
		Quad fourSplitCell[4]
		);

private:

	void InitFirstQuadrant( size_t& nhx,
							std::vector<Hexa>& hex,
							std::vector<pt3D>& pta);

	void Mirror(int xM, int yM, size_t& nhx);

};

#endif // !defined(AFx_PyRAMID_H__050F05DC_1297_4BE9_A3E1_903AC2D05AA9__INCLUDED_)
