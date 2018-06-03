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
// pt3D.h: interface for the pt3D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PT3D_H__6B715091_5B08_4C7A_86D9_E355E7F88BFD__INCLUDED_)
#define AFX_PT3D_H__6B715091_5B08_4C7A_86D9_E355E7F88BFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct pt3D;

inline pt3D operator*(double v, const pt3D& a);
inline pt3D operator*(const pt3D& a, const pt3D& b);
inline pt3D operator+(const pt3D& a, const pt3D& b);
inline pt3D operator-(const pt3D& a, const pt3D& b);

struct pt3D
{
	enum Type { 
			apex=0, 
			face=1,
			floor=2,
			internal=3,
			top=4
	};
	enum Orientation { 
			north=0, 
			south=1,
			east=2,
			west=3,
			northEast=4,
			northWest=5,
			southEast=6,
			southWest=7,
			neutral=8
	};

	static Orientation MirrorOrientation(Orientation type, int xm, int ym);

	pt3D(): type(internal), id(0), orient(neutral), move(true), bdry(false)
	{
	}

	pt3D(double xi, double yi, double zi): type(internal), id(0), orient(neutral),
		x(xi), y(yi), z(zi), move(true), bdry(false)
	{
	}

	~pt3D()	{}

	double distSqr(const pt3D& other) const
	{
		double dx(other.x-x);
		double dy(other.y-y);
		double dz(other.z-z);
		return dx*dx + dy*dy + dz*dz;
	}

	pt3D& operator=(const pt3D& other)
	{
		x = other.x; y = other.y; z=other.z; type = other.type;
		id = other.id; move = other.move; bdry = other.bdry;
		return *this;
	}

	pt3D& operator+=(const pt3D& other)
	{
		x += other.x; y += other.y; z+=other.z;
		return *this;
	}

	pt3D& operator*=(const double v)
	{
		x *= v; y *= v; z *= v;
		return *this;
	}
	double dot(const pt3D& other) const
	{
		return other.x*x + other.y*y +other.z*z;
	}

	double det(const pt3D& p1, const pt3D& p2) const
	{
		return dot( p1*p2);
	}

	void SetBoundary()
	{
		move = false;
		bdry = true;
	}

	void SetInternalFix()
	{
		move = false;
		bdry = false;
	}

	bool InternalFixed() const
	{
		return !(move || bdry) ;
	}

	bool Boundary() const
	{
		return bdry;
	}

	bool FixedEnd() const
	{
		return !move || bdry ;
	}

	double x;
	double y;
	double z;
	Type type;
	size_t id;

	Orientation orient;

	bool move;
	bool bdry;

	void PrintDebug();
};

inline pt3D operator*(double v, const pt3D& a)
{
	return pt3D(v*a.x,v*a.y,v*a.z);
}

inline pt3D operator*(const pt3D& a, const pt3D& b)
{
	return pt3D(a.y*b.z - b.y*a.z,
				b.x*a.z - a.x*b.z,
				a.x*b.y - b.x*a.y);
}

inline pt3D operator+(const pt3D& a, const pt3D& b)
{
	return pt3D(a.x+b.x, a.y+b.y, a.z+b.z);
}

inline pt3D operator-(const pt3D& a, const pt3D& b)
{
	return pt3D(a.x-b.x,a.y-b.y,a.z-b.z);
}

size_t AddPoint(pt3D& pt, std::vector<pt3D>& pta);
double MergeMinDist();

#endif // !defined(AFX_PT3D_H__6B715091_5B08_4C7A_86D9_E355E7F88BFD__INCLUDED_)
