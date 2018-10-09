#pragma once

#include <stdio.h>

#include <string>

#include "dxf_types.h"
#include "dxf_drawing.h"

//
// DXF parser
//
class DxfParser
{
public:
	// DXF parser result (error codes)
	enum tResult {
		DXF_OK,           // ok, no error
		DXF_ERR_FILEOPEN, // can't open input file for parsing
		DXF_ERR_SYNTAX,   // GDS2 syntax error
		DXF_ERR_MEMALLOC  // memmory allocation error
	};

private:
	enum tVariable {
		VAR_NONE,
		VAR_STRING,
		VAR_INT,
		VAR_DOUBLE,
		VAR_VECTOR_DOUBLE
	};

	enum tAngularUnit {
		UNIT_DEGREES,
		UNIT_RADIANS,
		UNIT_GRADIANS,
		UNIT_SURVEYORS
	};

private:
	tAngularUnit      m_angularUnit;    // unit format for angles
	bool              m_ccAngles;       // counter-clockwise (true) or clockwise angles (false)
	DxfDrawing *      m_drawing;        // pointer to drawing object (container of DXF entities)
	
	DxfLine           m_dxfLine;        // current LINE data
	DxfPoint          m_dxfPoint;       // current POINT data
	DxfArc            m_dxfArc;         // current ARC data  
	DxfCircle         m_dxfCircle;      // current CIRCLE data
	DxfEllipse        m_dxfEllipse;     // current ELLIPSE data
	DxfInsert         m_dxfInsert;      // current INSERT data
	DxfSolid          m_dxfSolid;       // current SOLID data
	DxfBlockHeader    m_dxfBlockHeader; // current BLOCK data
	DxfLayer          m_dxfLayer;       // current LAYER data 
	DxfPolyline       m_dxfPolyline;    // current LWPOLYLINE/POLYLINE data
	DxfVertex         m_dxfVertex;      // current LWPOLYLINE or POLYLINE vertex data
	
	DxfHatch                  m_dxfHatch;                // current HATCH entity
	DxfHatchPath              m_dxfHatchPath;            // current HATCH boundary path entity
	DxfHatchPolylinePath      m_dxfHatchPolylinePath;    // current HATCH polyline boundary
	DxfHatchEdge_Line         m_dxfHatchEdgeLine;        // current HATCH line edge
	DxfHatchEdge_CircularArc  m_dxfHatchEdgeCircArc;     // current HATCH circular edge
	DxfHatchEdge_EllipticArc  m_dxfHatchEdgeEllipticArc; // current HATCH elliptical edge
	DxfHatchEdge_Spline       m_dxfHatchEdgeSpline;      // current HATCH spline edge

	int               m_code;           // actual DXF code
	int               m_status;         // current parser status 
	int               m_nExpectedCoords;// number of expected coordinates 
	int               m_nCoords;        // number of received coordinates
	bool              m_insideBlock;    // is parser inside BLOCK element?
	
	char*             m_codeStr;        // string representation of actual DXF code
	char*             m_valueStr;       // string representation of actual DXF value
	char*             m_varName;        // variable key name
	char*             m_varValue;
	
	double            m_varDouble;      // variable double value
	double            m_varX;
	double            m_varY;
	double            m_varZ;
	int               m_varInt;         // variable int value
	tVariable         m_varType;        // current type of variable 
	bool              m_varXValid;
	bool              m_varYValid;
	bool              m_varZValid;

private:

	// Allocate aux buffers
	void AllocateBuffers();

	// Destroy aux buffers
	void DestroyBuffers();

	// transform actual value string (m_valueStr) to double value
	double ValueToDouble(double defaultValue = 0.0);

	// transform actual value string to the angle in degrees in counter-clockwise orientation
	double ValueToAngle(double defaultValue = 0.0);

	// transform actual value string (m_valueStr) to integer
	int ValueToInt(int defaultValue = 0);

	// read line of size n characters from input file to allocated buffer
	bool ReadLine(FILE* stream, char* str, int n);
	
	// parse actual code and value pair
	void Parse();
		
	void ParseLine();
	void ParsePoint();
	void ParseArc();
	void ParseEllipse();
	void ParseCircle();
	void ParseEntityEnd();
	void ParseInsert();
	void ParseBlockHeader();
	void ParseBlock();
	void ParseHeader();
	void ParseSolid();
	void ParseLWPolyline();
	void ParseLWPolylineVertices();
	void ParsePolyline();
	void ParseVertex();
	void ParseSpline();
	void ParseHatch();
	void ParseHatchPath();
	void ParseHatchPolylinePath();
	void ParseHatchPolylinePathVertices();
	void ParseHatchEdgeType();
	void ParseHatchEdge_Line();
	void ParseHatchEdge_CircularArc();
	void ParseHatchEdge_EllipticalArc();
	void ParseHatchEdge_Spline();

public:
	DxfParser();
	~DxfParser();

	tResult Parse(const wchar_t * filename, DxfDrawing * drawing);
};
