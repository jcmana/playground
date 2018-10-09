#include "dxf_types.h"

DxfLayer::DxfLayer() : flags(0)
{}


// POINT entity
DxfPoint::DxfPoint() : x(0), y(0), z(0)
{}


// LINE entity
DxfLine::DxfLine() : x1(0), y1(0), z1(0), x2(0), y2(0), z2(0)
{}


// ARC entity
DxfArc::DxfArc() : cx(0), cy(0), cz(0), radius(0), startAngle(0), endAngle(0)
{}


// CIRCLE entity
DxfCircle::DxfCircle() : cx(0), cy(0), cz(0), radius(0)
{}


// ELLIPSE entity
DxfEllipse::DxfEllipse() : cx(0), cy(0), cz(0), epX(0), epY(0), epZ(0), ratio(0), startAngle(0), endAngle(0)
{}


// SOLID entity
DxfSolid::DxfSolid() : x1(0), y1(0), z1(0), x2(0), y2(0), z2(0), x3(0), y3(0), z3(0), x4(0), y4(0), z4(0)
{}


DxfInsert::DxfInsert() : x(0), y(0), z(0), rotation(0), sx(1), sy(1), sz(1), columns(1), rows(1), columnSpacing(0), rowSpacing(0)
{}


void DxfInsert::Init()
{
	rotation = 0.0;
	sx = sy = sz = 1.0;
	columns = rows = 1;
	columnSpacing = rowSpacing = 0.0;
}


DxfBlockHeader::DxfBlockHeader()
{
	bpx = bpy = bpz = 0.0;
	flags = 0;
}


DxfVertex::DxfVertex()
{
	x = 0.0;
	y = 0.0;
	z = 0.0;
	bulge = 0.0;
}


DxfPolyline::DxfPolyline() : nVertices(0), isNVerticesValid(false), startWidth(0), endWidth(0), flags(0)
{
}


// HATCH entity
DxfHatch::DxfHatch() : nPaths(0)
{}


DxfHatchPolylinePath::DxfHatchPolylinePath() : nVertices(0), closed(false), hasBulge(false)
{}


DxfHatchPath::DxfHatchPath() : nEdges(0)
{}


DxfHatchEdge_Line::DxfHatchEdge_Line() : x1(0), y1(0), x2(0), y2(0)
{}


DxfHatchEdge_CircularArc::DxfHatchEdge_CircularArc() : cx(0), cy(0), radius(0), startAngle(0), endAngle(0), isCounterClockwise(false)
{}


DxfHatchEdge_EllipticArc::DxfHatchEdge_EllipticArc() : cx(0), cy(0), epx(0), epy(0), minorAxisLength(0), startAngle(0), endAngle(0), isCounterClockwise(false)
{}
