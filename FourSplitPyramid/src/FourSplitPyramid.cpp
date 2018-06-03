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
// FourSplitPyramid.cpp : Defines the entry point for the console application.
//
// Instructions:
//    To run this executable, make sure that it is run from the level where 
//    the directories input\ and output\ lie.
//    type FourSplitPyramid.exe to run.
//    the output files will be stored in output\
//
#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#include "inc/MeshIo.h"
#include "inc/SolveHexahedralMesh.h"
#include "inc/Pyramid.h"
#include "inc/PairedElement.h"
#include "inc/SplitQuad.h"
#include "inc/RemoveInternalSingularity.h"
#include "inc/FixDegeneracies.h"
#include "inc/GeodeTemplate.h"
#include "inc/TetrahedralDicing.h"

namespace
{
	void
	RemoveBoundarySingularity()
	{
		double pt[8][3] = {
			{ 0, 0,  0},
			{ 1, 0,  0},
			{ 1, 1,  0},
			{ 0, 1,  0},
			{ 0, 0,  1},
			{ 1, 0,  1},
			{ 1, 1,  1},
			{ 0, 1,  1}
		};
		std::vector< pt3D > pta(8);
		Hexa hex;
		for ( size_t  i = 0 ; i < 8; ++i )
			hex.idx[i] = i;

		for ( size_t j = 0; j < pta.size(); ++j )
		{
			pta[j].x = pt[j][0];
			pta[j].y = pt[j][1];
			pta[j].z = pt[j][2];
			if( pta[j].z < .5 )
				pta[j].SetBoundary();
		}

		std::vector<Hexa> h(1,hex);

		// cout << "Min volume of a cube = " << Hexa::MinVolume(pta,h) << endl;

		RemoveInternalSingularity rmSingularity;
		rmSingularity(h,pta);
		std::string solutionRmSing;
		solutionRmSing += "output\\RemoveBoundarySingularity.dyn";
		FixDegeneracies rmDegeneracies;
		rmDegeneracies(h,pta);
		WriteFile(solutionRmSing.c_str(), pta, h);
	}

	void addPta( double x, double y, double z, std::vector<pt3D>& pta)
	{
		pta.push_back(pt3D(x, y, z));
	}

	void TestPairedTransition()
	{
		size_t q[4]= { 2, 1, 4, 5};
		double pt[6][3] = {{1,1,0},{ 0,1,0},{ 0,0,0},{1,0,0},{-1,1,0},{-1,0,0}};
		std::vector< std::pair<Quad, Quad> > pairedPartition;
		std::vector<pt3D> pta;
		std::vector<Hexa> hex;
		std::vector<Quad> transitionQuad;
		pt3D midPoint;

		std::pair<Quad, Quad> tq;

		for ( size_t i = 0; i < 4; ++i )
		{
			tq.first.idx[i] = i;
			tq.second.idx[i] = q[i];
		}

		pairedPartition.push_back(tq);
		for ( i = 0; i < 6; ++ i )
			addPta(pt[i][0], pt[i][1], pt[i][2], pta);

		PairedElement::AddHexahedra(pairedPartition,pta, hex,transitionQuad,
			midPoint);

		for(i = 0; i < hex.size() ; ++i )
			hex[i].PrintDebug(true);

		for ( i = 0; i < transitionQuad.size(); ++i )
			transitionQuad[i].PrintDebug();

		WriteFile("C:\\Meshing\\FourSplitPyramid\\output\\pairedPartition.dn",
					pta, 
					hex);

		WriteFile("C:\\Meshing\\FourSplitPyramid\\output\\pairedTransition.dn",
				pta, 
				transitionQuad);

	}

	void ProcessFile(char* dir, char* fn, bool useGeodeTemplate, bool removeSingularities)
	{
		std::vector<pt3D> pta;
		std::vector<Hexa> hexa;
		std::vector< std::pair<Quad, Quad> > pairedPartition;
		std::vector<Quad> transitionQuad;
		pt3D midPoint;

		std::string  filename;

		filename += dir ;
		filename += fn;
		filename += ".dyn";

		ReadFile(filename.c_str(),
				pta, hexa, pairedPartition);

		std::string solution;

		solution = "output\\";
		solution += fn;
		if ( useGeodeTemplate )
			solution += "GeodeTemplate";
		solution += "Solution.dyn";

		//--------------------------------------------------------------
		//----------- Solve the constrained problem with singularities
		//--------------------------------------------------------------
		SolveHexahedralMesh obj(
			pairedPartition,
			pta,
			useGeodeTemplate);

		//--- Output the solution with singularities
		WriteFile(solution.c_str(), obj.m_pta, obj.m_hexa);

		if ( !removeSingularities )
			return;

		//--- remove internal singularities; 
		//---  fix elements that share more than one face with the same neighbor
		RemoveInternalSingularity rmSingularity;
		rmSingularity(obj.m_hexa,obj.m_pta);

		//--- remove edge singularities; 
		//---  fix faces that share more than one edge with the same face
		FixDegeneracies rmDegeneracies;
		rmDegeneracies(obj.m_hexa,obj.m_pta);

		//--- Output the solution with singularities
		std::string solutionRmSing;
		solutionRmSing = "output\\";
		solutionRmSing += fn;
		if ( useGeodeTemplate )
			solutionRmSing += "GeodeTemplate";
		solutionRmSing += "RmSingularitySolution.dyn";
		WriteFile(solutionRmSing.c_str(), obj.m_pta, obj.m_hexa);

#ifdef DUMP_NEGATIVE
		cout << "Mininum determinat of " << filename <<" = "  << 
			Hexa::MinDet(obj.m_pta, obj.m_hexa) <<endl;

		cout << "Mininum Volume of " << filename <<" = "  << 
			Hexa::MinVolume(obj.m_pta, obj.m_hexa) <<endl;
		std::string negative;
		negative =  fn;
		negative+= "Negative";
		negative+= ".dyn";
		WriteNegativeHexa(negative.c_str(),obj.m_pta, obj.m_hexa);
#endif
	}};

int main(int argc, char* argv[])
{

	try
	{
		TetrahedralDicing obj;
		WriteFile("output\\TetDice.dyn", obj.pta, obj.hex);

		/*
		GeodeTemplate obj;
		WriteFile("output\\GeodeTemplate.dyn", obj.pta, obj.hex);
		cout << "Mininum determinat of Geode-Templates = "  << 
			Hexa::MinDet(obj.pta, obj.hex) <<endl;

		cout << "Mininum Volume of Geode-Templates = "  << 
			Hexa::MinVolume(obj.pta, obj.hex) <<endl;
		

		RemoveBoundarySingularity();
		*/

		cout << " Solving input\\SchneidersPyramid.dyn" << endl;
		ProcessFile("input\\","SchneidersPyramid", true, false);

		exit(1);

		cout << " Solving input\\Fig10.dyn" << endl;
		ProcessFile("input\\","Fig10", true, false);

		cout << " Solving input\\QuadrilateralOctahedron.dyn" << endl;
		ProcessFile("input\\","QuadrilateralOctahedron", true, false);

		cout << " Solving input\\EppsteinTwo.dyn" << endl;
		ProcessFile("input\\","EppsteinTwo", true, false);

	}
	catch (...)
	{
		cout << "Error\n";
	}

	return 0;
}

