//	 
//   Copyright 2005  (C) Sandia National Laboratories All rights reserved 
//	 
#pragma once

#include <vector>
#include <utility>

#include "pt3D.h"
#include "Quad.h"
#include "Hexa.h"

class GeodeTemplate
{
public:
	GeodeTemplate(void);
	~GeodeTemplate(void);

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
		
	std::vector<Hexa> hex;
	std::vector<pt3D> pta;
};
