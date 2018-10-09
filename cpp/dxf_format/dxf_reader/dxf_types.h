#pragma once

#include <string>

// DXF LAYER
struct DxfLayer
{
	std::string name;
	int         flags;

	DxfLayer();
};


// POINT entity
struct DxfPoint
{
	std::string layerName; // layer name
	double x, y, z;        // point coordinate

	DxfPoint();
};


// LINE entity
struct DxfLine
{
	std::string layerName; // layer name
	double x1, y1, z1;     // start point coordinate
	double x2, y2, z2;     // end point coordinate

	DxfLine();
};


// ARC entity
struct DxfArc
{
	std::string layerName; // layer name
	double cx, cy, cz;     // center
	double radius;         // radius
	double startAngle;     // start angle in counterclockwise degrees
	double endAngle;       // end angle in counterclockwise degrees

	DxfArc();
};


// CIRCLE entity
struct DxfCircle
{
	std::string layerName; // layer name
	double cx, cy, cz;     // center
	double radius;         // radius

	DxfCircle();
};


// ELLIPSE entity
struct DxfEllipse
{
	std::string layerName; // layer name
	double cx, cy, cz;     // center
	double epX, epY, epZ;  // end point of major axis relative to the center 
	double ratio;          // ratio of minor axis to major axis
	double startAngle;     // Start angle in counterclockwise degrees (0.0 for full ellipse)
	double endAngle;       // End angle in counterclockwise degrees (360.0 for full ellipse)

	DxfEllipse();
};


// SOLID entity
struct DxfSolid
{
	std::string layerName; // layer name
	double x1, y1, z1;     // first point
	double x2, y2, z2;     // second point
	double x3, y3, z3;     // third point
	double x4, y4, z4;     // fourth point

	DxfSolid();
};


// INSERT entity (block insertion)
struct DxfInsert
{
	std::string layerName;     // name of layer
	std::string blockName;     // name of inserted block
	double      x, y, z;       // insertion point   
	double      sx, sy, sz;    // scale factor in x, y and z direction
	double      rotation;      // rotation angle in counterclockwise degrees (optional, default == 0.0)
	double      columnSpacing; // distance between columns (optional, default == 0.0)
	double      rowSpacing;    // distance between rows (optional, default == 0.0
	int         columns;       // number of columns (optional, default == 1)
	int         rows;          // number of rows (optional, default == 1)

	DxfInsert();

	void Init();
};


// LWPOLYLINE, POLYLINE entity
struct DxfPolyline
{
	std::string layerName;         // name of layer
	unsigned    flags;             // polyline flags 
	unsigned    nVertices;         // number of vertices (default = 0, valid for LWPOLYLINE entity only)
	unsigned    isNVerticesValid;  // is number of vertices valid? (default = false)
	double      startWidth;        // starting width of each segment (default = 0)
	double      endWidth;          // end width of each segment (default = 0)

	DxfPolyline();
};


// POLYLINE vertex
struct DxfVertex
{
	double x, y, z;      // coordinate
	double bulge;        // bulge factor  

	DxfVertex();
};


// HATCH entity
struct DxfHatch
{
	std::string layerName;  // layer name
	unsigned    nPaths;     // number of boundary paths (loops) 

	DxfHatch();
};


struct DxfHatchPolylinePath
{
	unsigned  nVertices;    // number of vertices
	bool      closed;       // is closed polyline?
	bool      hasBulge;     // is bulge valid in vertices array?

	DxfHatchPolylinePath();
};


struct DxfHatchPath
{
	unsigned nEdges;        // number of edges

	DxfHatchPath();
};


struct DxfHatchEdge_Line
{
	double x1, y1;         // start point
	double x2, y2;         // end point

	DxfHatchEdge_Line();
};


struct DxfHatchEdge_CircularArc
{
	double cx, cy;               // center
	double radius;               // radius 
	double startAngle;           // start angle in counterclockwise degrees
	double endAngle;             // end angle in counterclockwise degrees 
	bool   isCounterClockwise;   // is it clockwise or counter-clockwise? 

	DxfHatchEdge_CircularArc();
};


struct DxfHatchEdge_EllipticArc
{
	double cx, cy;             // center
	double epx, epy;           // end point of major axis relative to the center point 
	double minorAxisLength;    // length of minor axis (percentage of major axis length)
	double startAngle;         // start angle in counterclockwise degrees
	double endAngle;           // end angle in counterclockwise degrees
	bool   isCounterClockwise; // is counter-clockwise

	DxfHatchEdge_EllipticArc();
};


struct DxfHatchEdge_Spline
{

};

// BLOCK header
struct DxfBlockHeader
{
	std::string name;           // block name 
	double      bpx, bpy, bpz;  // base point coordinates
	unsigned    flags;          // block flags bit coded

	DxfBlockHeader();
};
