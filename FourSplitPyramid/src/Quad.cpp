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
// Quad.cpp: implementation of the Quad class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <iostream>
#include <fstream>
using namespace std;
#include "inc/Quad.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
bool Quad::CompReferenceQuad(const Quad& a, const Quad& b)
{
	return a.referenceQuad < b.referenceQuad;
}

void Quad::PrintDebug()
{
	size_t j;
	cout << "	{ ";
	for (j = 0; j < 3; ++j)
	{
		cout << idx[j] << ",";
	}
	cout << idx[j] << " }\n";
}
