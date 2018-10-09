#pragma once

//
// DXF drawing, container for DXF entities.
// They are added to this object by DxfParser
//
class DxfDrawing
{
public:
	DxfDrawing() {}
	virtual ~DxfDrawing() {}

	virtual void AddLayer(DxfLayer& layer) {}

	virtual void AddLine(DxfLine& line) {}
	virtual void AddPoint(DxfPoint& point) {}
	virtual void AddArc(DxfArc& arc) {}
	virtual void AddCircle(DxfCircle& circle) {}
	virtual void AddEllipse(DxfEllipse& ellipse) {}
	virtual void AddSolid(DxfSolid& solid) {}
	virtual void AddInsert(DxfInsert& insert) {}
	virtual void AddPolyline(DxfPolyline& polyline) {}
	virtual void AddPolylineVertex(DxfVertex& vertex) {}

	virtual void AddHatch(DxfHatch& hatch) {}
	virtual void AddHatchPath(DxfHatchPath& hatchPath) {}
	virtual void AddHatchPolylinePath(DxfHatchPolylinePath& hatchPolylinePath) {}
	virtual void AddHatchPolylinePathVertex(DxfVertex& vertex) {}
	virtual void AddHatchEdge_Line(DxfHatchEdge_Line& line) {}
	virtual void AddHatchEdge_Circular(DxfHatchEdge_CircularArc& circular) {}
	virtual void AddHatchEdge_Elliptical(DxfHatchEdge_EllipticArc& elliptical) {}
	virtual void AddHatchEdge_Spline(DxfHatchEdge_Spline& spline) {}

	virtual void StartBlock(DxfBlockHeader& blockHeader) {}
	virtual void EndBlock() {}

	virtual void SetVariableString(char* varName, char* value) {}
	virtual void SetVariableInt(char* varName, int value) {}
	virtual void SetVariableDouble(char* varName, double value) {}
	virtual void SetVariableDoubleVector(char* varName, double x, double y, double z, bool validX, bool validY, bool validZ) {}
};