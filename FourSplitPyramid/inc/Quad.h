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
// Quad.h: interface for the Quad class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUAD_H__C105F4B8_1897_43D9_B5F1_51AD21B5E197__INCLUDED_)
#define AFX_QUAD_H__C105F4B8_1897_43D9_B5F1_51AD21B5E197__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct Quad
{
	Quad():group(1)
	{
	}

	~Quad(){}

	int idx[4];
	static bool CompReferenceQuad(const Quad& a, const Quad& b);

	void PrintDebug();
	size_t referenceQuad;
	size_t group;
};

#endif // !defined(AFX_QUAD_H__C105F4B8_1897_43D9_B5F1_51AD21B5E197__INCLUDED_)
