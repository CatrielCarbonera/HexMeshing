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
// SplitQuad.h: interface for the SplitQuad class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPLITQUAD_H__C537BBDE_57CC_4284_BEB7_393BBE761185__INCLUDED_)
#define AFX_SPLITQUAD_H__C537BBDE_57CC_4284_BEB7_393BBE761185__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <vector>
#include "pt3D.h"
#include "Quad.h"
#include "Hexa.h"

class SplitQuad
{
public:

	SplitQuad();

	virtual ~SplitQuad();

	static void AddSplitQuad(
		const pt3D& midPoint,
		const std::vector< size_t >& cycle,
		const std::vector< Quad >& refQuad,
		std::vector< Hexa >& hexList,
		std::vector< pt3D >& pointList,
		std::vector< Quad >& quadBry
		);

	static void SplitSingleQuad(
								size_t ed,
								const Quad& q,
								const pt3D& midPoint,
								std::vector< Hexa >& hexList,
								std::vector< pt3D >& pointList,
								std::vector< Quad >& quadOut
		);

	void PrintDebug();

	std::vector<Hexa> hex;

	std::vector<pt3D> pta;
};

#endif // !defined(AFX_SPLITQUAD_H__C537BBDE_57CC_4284_BEB7_393BBE761185__INCLUDED_)
