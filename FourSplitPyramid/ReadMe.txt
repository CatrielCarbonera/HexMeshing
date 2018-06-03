========================================================================
       CONSOLE APPLICATION : FourSplitPyramid
========================================================================


AppWizard has created this FourSplitPyramid application for you.  

This file contains a summary of what you will find in each of the files that
make up your FourSplitPyramid application.

FourSplitPyramid.dsp
    This file (the project file) contains information at the project level and
    is used to build a single project or subproject. Other users can share the
    project (.dsp) file, but they should export the makefiles locally.

src\FourSplitPyramid.cpp
    This is the main application source file.

src\BoundaryLayer.cpp
    This is the implementation of the code that adds boundary layers

src\MeshIO.cpp
    Implementation of the Input and Output LS-Dyna files

src\pt3D.cpp
    Implementation of the point class

src\Quad.cpp
    Implementation of the class that contains quadrilaterals

src\SolveHexahedralMesh.cpp
    Implementation of the class that Solves the Constrained hexahedra

src\FixDegeneracies.cp
    Implementation of the class the resolves the degeneracies related to quadrilaterals sharing
    more than one edge

src\PairedElement.cpp
    Implementation of the class that adds the transition element to the Paired Elelements

src\Pyramid.cpp
    Implementation of the class that adds the Four Split Pyramid to the Four Split Cells

src\RemoveInternalSingularity.cpp
    Implementation of the class that resolves degeneracies related to 

src\SplitQuad.cpp
    Implementation of the class that, after applied twice, transitions from paired elements
    to four split quads

/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named FourSplitPyramid.pch and a precompiled types file named StdAfx.obj.


/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" to indicate parts of the source code you
should add to or customize.

___________________________________________________________________________
About the LS-Dyna Format used by MeshIo:

		The keywords *NODE, *ELEMENT_SHELL, and *ELEMENT_SOLID follow the 
		LS-Dyna specification The keyword *PAIRED_PARTITION has been added to
		indicate the paired-partition of a file definining the Constraining 
		Quadrilateral Mesh; the paired quadrilateral elements are specified 
		at the end of the file before the keyword *END.
		The Paired partition follows the syntax
			<pair number>   <quad>   <quadrilateral that shares an edge>
		The union of all the pairs listed under this section must be equal
		to the total set of quadrilaterals, any two pairs must be mutually
		disjoint, and the quadrilaterals listed must share an edge.                                                       



/////////////////////////////////////////////////////////////////////////////
