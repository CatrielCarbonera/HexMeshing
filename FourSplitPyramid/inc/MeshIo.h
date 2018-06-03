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
//
#ifndef MESHIO_HXX
#define MESHIO_HXX
#include <vector>
struct Hexa;
struct Quad;
struct pt3D;
void WriteFile(const char* fileName,
				std::vector<pt3D>& pta, 
				std::vector<Hexa>& hexa);
void ReadFile(const char* fileName,
				std::vector<pt3D>& pta, 
				std::vector<Hexa>& hexa,
				std::vector< std::pair<Quad, Quad> >& pairedPartition);
void WriteFile(const char* fileName,
				std::vector<pt3D>& pta, 
				std::vector<Quad>& quad);
void WriteNegativeHexa(const char* fileName,
				std::vector<pt3D>& pta, 
				std::vector<Hexa>& hexa
				  );

#endif
