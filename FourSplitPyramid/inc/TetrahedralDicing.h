#pragma once

#include <vector>
#include <utility>

#include "pt3D.h"
#include "Quad.h"
#include "Hexa.h"

class TetrahedralDicing
{
public:
	TetrahedralDicing(void);
	~TetrahedralDicing(void);

	static void	FillCellOfThree(
		size_t midPointIdx,
		std::vector<Hexa>& hexa,
		std::vector<pt3D>& pta,
		Quad threeSplitCell[3]
		);

	std::vector<Hexa> hex;
	std::vector<pt3D> pta;

};
