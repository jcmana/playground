#include "dxf_parser.h"
#define _USE_MATH_DEFINES
#include <math.h>

static const int DXF_LINE_MAX = 1024;  // maximum number of characters for line in DXF file

enum tEntity
{
	STATUS_ROOT,
	STATUS_ERROR,
	STATUS_SECTION_START,
	STATUS_HEADER_SECTION,
	STATUS_TABLES_SECTION,
	STATUS_TABLE,
	STATUS_TABLE_LAYERS,
	STATUS_LAYER,
	STATUS_BLOCKS_SECTION,
	STATUS_BLOCK_HEADER,
	STATUS_BLOCK,
	STATUS_ENTITIES_SECTION,
	STATUS_POINT,
	STATUS_LINE,
	STATUS_POLYLINE,
	STATUS_LWPOLYLINE,
	STATUS_LWPOLYLINE_VERTICES,
	STATUS_VERTEX,
	STATUS_ARC,
	STATUS_CIRCLE,
	STATUS_SPLINE,
	STATUS_ELLIPSE,
	STATUS_INSERT,
	STATUS_SOLID,
	STATUS_HATCH,
	STATUS_HATCH_PATH,
	STATUS_HATCH_POLYLINE_PATH,
	STATUS_HATCH_POLYLINE_PATH_VERTICES,
	STATUS_HATCH_EDGE_LINE,
	STATUS_HATCH_EDGE_CIRCLE,
	STATUS_HATCH_EDGE_ELLIPSE,
	STATUS_HATCH_EDGE_SPLINE
};




DxfParser::DxfParser() : 
	m_drawing(NULL), m_codeStr(NULL), m_valueStr(NULL), m_varName(NULL), 
	m_varValue(NULL), m_status(STATUS_ROOT), m_insideBlock(false), m_varType(VAR_NONE), m_varX(0), m_varY(0), m_varZ(0), m_varInt(0),
	m_varXValid(false), m_varYValid(false), m_varZValid(false), m_nExpectedCoords(0), m_nCoords(0), 
	m_ccAngles(true), m_angularUnit(UNIT_DEGREES), m_varDouble(0), m_code(0)
{
}


DxfParser::~DxfParser()
{
	DestroyBuffers();
}


// Allocate aux buffers
void DxfParser::AllocateBuffers()
{
	m_codeStr = new char[DXF_LINE_MAX];
	m_valueStr = new char[DXF_LINE_MAX];
	m_varName = new char[DXF_LINE_MAX];
	m_varValue = new char[DXF_LINE_MAX];
}

// Destroy aux buffers
void DxfParser::DestroyBuffers()
{
	delete[] m_codeStr;
	delete[] m_valueStr;
	delete[] m_varName;
	delete[] m_varValue;
	m_codeStr = NULL;
	m_valueStr = NULL;
	m_varName = NULL;
	m_varValue = NULL;
}


bool DxfParser::ReadLine(FILE* stream, char* str, int n)
{
	register int c;
	register char* cs;
	cs = str;

	// ignore leading white spaces
	while (--n > 0 && (c = getc(stream)) != EOF)
	{
		if (c != ' ') {
			*cs++ = c;
			n++;
			break;
		}
	}

	if (c != '\n')
	{
		while (--n > 0 && (c = getc(stream)) != EOF)
		{
			// put the input char into the current pointer position, then increment it
			// if a newline entered, break
			if (c == '\n')
				break;
			*cs++ = c;
		}
	}

	// cut '\r'
	if (cs != str)
	{
		cs--;
		if (*cs == '\r')
			*cs = '\0';
		cs++;
	}
	*cs = '\0';
	return (c != EOF || cs != str);
}


double DxfParser::ValueToDouble(double defaultValue)
{
	double value = 0.0;
	char* stop;
	char* p;

	p = strchr(m_valueStr, ',');
	if (p) 
		*p = '.';
	value = strtod(m_valueStr, &stop);
	if (stop == m_valueStr)
		value = defaultValue;
	return value;
}


// Transform actual value to angle in counter-clockwise degrees
double DxfParser::ValueToAngle(double defaultValue)
{
	double angle = ValueToDouble(defaultValue);

	// ensure angle is in degrees
	switch (m_angularUnit)
	{
		case UNIT_RADIANS:
			angle = (angle / 180.0) * M_PI;
			break;

		case UNIT_GRADIANS:
			angle = (angle * 9.0) / 10.0;
			break;

		default:
			break;
	}

	// ensure angle direction is counter-clockwise
	if (!m_ccAngles)
		angle = 360.0 - angle;
	return angle;
}


// transform actual value string (m_valueStr) to integer
int DxfParser::ValueToInt(int defaultValue)
{
	int value;
	char* stop;
	value = strtol(m_valueStr, &stop, 10);
	if (stop == m_valueStr)
		value = defaultValue;
	return value;
}


void DxfParser::ParseEntityEnd()
{
	m_status = m_insideBlock ? STATUS_BLOCK : STATUS_ENTITIES_SECTION;
	Parse();
}


void DxfParser::ParsePoint()
{
	switch (m_code)
	{
		case 0:  // End of POINT entity, add it to drawing containter and start to parse next entity
			m_drawing->AddPoint(m_dxfPoint);
			ParseEntityEnd();
			break;
		case 8:  // layer name
			m_dxfPoint.layerName = m_valueStr;
			break;
		case 10: // X
			m_dxfPoint.x = ValueToDouble();
			break;
		case 20: // Y
			m_dxfPoint.y = ValueToDouble();
			break;
		case 30: // Z
			m_dxfPoint.z = ValueToDouble();
			break;
	}
}


void DxfParser::ParseLine()
{
	switch (m_code)
	{
		case 0:  // end of LINE entity, add it to drawing container and start to parse next entity
			m_drawing->AddLine(m_dxfLine);
			ParseEntityEnd();
			break;
		case 8:  // layer name
			m_dxfLine.layerName = m_valueStr;
			break;
		case 10: // x1
			m_dxfLine.x1 = ValueToDouble();
			break;
		case 11: // x2
			m_dxfLine.x2 = ValueToDouble();
			break;
		case 20: // y1
			m_dxfLine.y1 = ValueToDouble();
			break;
		case 21: // y2
			m_dxfLine.y2 = ValueToDouble();
			break;
		case 30: // z1
			m_dxfLine.z1 = ValueToDouble();
			break;
		case 31: // z2
			m_dxfLine.z2 = ValueToDouble();
			break;
	}
}


void DxfParser::ParseArc()
{
	switch (m_code)
	{
		case 0:  // End of ARC entity, add it to drawing and start to parse next entity
			m_drawing->AddArc(m_dxfArc);
			ParseEntityEnd();
			break;
		case 8:  // layer name
			m_dxfArc.layerName = m_valueStr;
			break;
		case 10:  // center x
			m_dxfArc.cx = ValueToDouble();
			break;
		case 20:  // center y
			m_dxfArc.cy = ValueToDouble();
			break;
		case 30:  // center z
			m_dxfArc.cz = ValueToDouble();
			break;
		case 40:  // radius
			m_dxfArc.radius = ValueToDouble();
			break;
		case 50:  // Start angle
			m_dxfArc.startAngle = ValueToAngle();
			break;
		case 51:  // End angle
			m_dxfArc.endAngle = ValueToAngle();
			break;
	}
}


void DxfParser::ParseEllipse()
{
	switch (m_code)
	{
		case 0:  // End of ELLIPSE entity, add it to drawing and start to parse next entity
			m_drawing->AddEllipse(m_dxfEllipse);
			ParseEntityEnd();
			break;
		case 8:  // layer name
			m_dxfEllipse.layerName = m_valueStr;
			break;
		case 10:  // center x
			m_dxfEllipse.cx = ValueToDouble();
			break;
		case 11:  // end point of major axis (x)
			m_dxfEllipse.epX = ValueToDouble();
			break;
		case 20:  // center y
			m_dxfEllipse.cy = ValueToDouble();
			break;
		case 21:  // end point of major axis (y)
			m_dxfEllipse.epY = ValueToDouble();
			break;
		case 30:  // center z
			m_dxfEllipse.cz = ValueToDouble();
			break;
		case 31:  // end point of major axis (z)
			m_dxfEllipse.epZ = ValueToDouble();
			break;
		case 40:  // ratio
			m_dxfEllipse.ratio = ValueToDouble();
			break;
		case 41:  // start angle
			m_dxfEllipse.startAngle = (ValueToDouble() / M_PI) * 180.0;
			break;
		case 42:  // end angle
			m_dxfEllipse.endAngle = (ValueToDouble() / M_PI) * 180.0;
			break;
	}
}


void DxfParser::ParseCircle()
{
	switch (m_code)
	{
		case 0: // End of CIRCLE entity, add it to drawing container and parse next entity
			m_drawing->AddCircle(m_dxfCircle);
			ParseEntityEnd();
			break;
		case 8:  // layer name
			m_dxfCircle.layerName = m_valueStr;
			break;
		case 10: // center x
			m_dxfCircle.cx = ValueToDouble();
			break;
		case 20: // center y
			m_dxfCircle.cy = ValueToDouble();
			break;
		case 30: // center z
			m_dxfCircle.cz = ValueToDouble();
			break;
		case 40: // radius
			m_dxfCircle.radius = ValueToDouble();
			break;
	}
}


void DxfParser::ParseSolid()
{
	switch (m_code)
	{
		case 0: // End of SOLID entity, add it to drawing container and parse next entity 
			m_drawing->AddSolid(m_dxfSolid);
			ParseEntityEnd();
			break;
		case 8:  // layer name
			m_dxfSolid.layerName = m_valueStr;
			break;
		case 10:  // x1
			m_dxfSolid.x1 = ValueToDouble();
			break;
		case 11:  // x2
			m_dxfSolid.x2 = ValueToDouble();
			break;
		case 12:  // x3
			m_dxfSolid.x3 = ValueToDouble();
			break;
		case 13:  // x4
			m_dxfSolid.x4 = ValueToDouble();
			break;
		case 20:  // y1
			m_dxfSolid.y1 = ValueToDouble();
			break;
		case 21:  // y2
			m_dxfSolid.y2 = ValueToDouble();
			break;
		case 22:  // y3 
			m_dxfSolid.y3 = ValueToDouble();
			break;
		case 23:  // y4
			m_dxfSolid.y4 = ValueToDouble();
			break;
		case 30:  // z1
			m_dxfSolid.z1 = ValueToDouble();
			break;
		case 31:  // z2
			m_dxfSolid.z2 = ValueToDouble();
			break;
		case 32:  // z3
			m_dxfSolid.z3 = ValueToDouble();
			break;
		case 33:  // z4
			m_dxfSolid.z4 = ValueToDouble();
			break;
	} // switch
}


void DxfParser::ParseInsert()
{
	switch (m_code)
	{
		case 0:  // end of INSERT entity, add it to drawing and start to parse next entity
			m_drawing->AddInsert(m_dxfInsert);
			m_dxfInsert.Init();
			ParseEntityEnd();
			break;
		case 2:   // block name
			m_dxfInsert.blockName = m_valueStr;
			break;
		case 8:  // layer name
			m_dxfInsert.layerName = m_valueStr;
			break;
		case 10:  // x
			m_dxfInsert.x = ValueToDouble();
			break;
		case 20:  // y
			m_dxfInsert.y = ValueToDouble();
			break;
		case 30:  // z
			m_dxfInsert.z = ValueToDouble();
			break;
		case 41:   // scale in x direction (optional, default == 1.0)
			m_dxfInsert.sx = ValueToDouble(1.0);
			break;
		case 42:   // scale in y direction (optional, default == 1.0)
			m_dxfInsert.sy = ValueToDouble(1.0);
			break;
		case 43:   // scale in z direction (optional, default == 1.0)
			m_dxfInsert.sz = ValueToDouble(1.0);
			break;
		case 44:   // column spacing (optional, default == 0.0)
			m_dxfInsert.columnSpacing = ValueToDouble();
			break;
		case 45:   // row spacing (optional, default == 0.0)
			m_dxfInsert.rowSpacing = ValueToDouble();
			break;
		case 50:   // rotation angle (optional, default == 0.0)
			m_dxfInsert.rotation = ValueToAngle();
			break;
		case 70:   // column count (optional, default == 1)
			m_dxfInsert.columns = ValueToInt(1);
			break;
		case 71:   // row count (optional, default == 1)
			m_dxfInsert.rows = ValueToInt(1);
			break;
	}
}


void DxfParser::ParseBlockHeader()
{
	switch (m_code)
	{
		case 0:  // End of block header, start to parse next entity
			m_drawing->StartBlock(m_dxfBlockHeader);
			m_insideBlock = true;
			m_status = STATUS_BLOCK;
			Parse();
			break;
		case 2:   // block name
			m_dxfBlockHeader.name = m_valueStr;
			break;
		case 10:  // base point x
			m_dxfBlockHeader.bpx = ValueToDouble();
			break;
		case 20:  // base point y
			m_dxfBlockHeader.bpy = ValueToDouble();
			break;
		case 30:  // base point z
			m_dxfBlockHeader.bpz = ValueToDouble();
			break;
	} // switch
}


void DxfParser::ParseBlock()
{
	switch (m_code)
	{
	case 0:
		if (strcmp(m_valueStr, "POINT") == 0)
			m_status = STATUS_POINT;
		else if (strcmp(m_valueStr, "LINE") == 0)
			m_status = STATUS_LINE;
		else if (strcmp(m_valueStr, "POLYLINE") == 0)
			m_status = STATUS_POLYLINE;
		else if (strcmp(m_valueStr, "LWPOLYLINE") == 0)
			m_status = STATUS_LWPOLYLINE;
		else if (strcmp(m_valueStr, "VERTEX") == 0)
			m_status = STATUS_VERTEX;
		else if (strcmp(m_valueStr, "ARC") == 0)
			m_status = STATUS_ARC;
		else if (strcmp(m_valueStr, "CIRCLE") == 0)
			m_status = STATUS_CIRCLE;
		else if (strcmp(m_valueStr, "ELLIPSE") == 0)
			m_status = STATUS_ELLIPSE;
		else if (strcmp(m_valueStr, "SPLINE") == 0)
			m_status = STATUS_SPLINE;
		else if (strcmp(m_valueStr, "INSERT") == 0)
			m_status = STATUS_INSERT;
		else if (strcmp(m_valueStr, "HATCH") == 0)
			m_status = STATUS_HATCH;
		else if (strcmp(m_valueStr, "ENDBLK") == 0) {
			m_insideBlock = false;
			m_drawing->EndBlock();
			m_status = STATUS_BLOCKS_SECTION;
		}
		break;
	} // switch
}


void DxfParser::ParseHeader()
{
	if (m_code == 0)
	{
		if (strcmp(m_valueStr, "ENDSEC") == 0)
		{
			m_status = STATUS_ROOT;

			if (m_varType != VAR_NONE)
			{
				if (m_varType == VAR_STRING) {
					m_drawing->SetVariableString(m_varName, m_varValue);
				}
				else if (m_varType == VAR_INT) {
					m_drawing->SetVariableInt(m_varName, m_varInt);
				}
				else if (m_varType == VAR_DOUBLE) {
					m_drawing->SetVariableDouble(m_varName, m_varDouble);
				}
				else if (m_varType == VAR_VECTOR_DOUBLE) {
					m_drawing->SetVariableDoubleVector(m_varName, m_varX, m_varY, m_varZ, true, true, true);
				}
			}
		}
	}
	else if (m_code == 9)
	{
		if (m_varType != VAR_NONE)
		{
			if (m_varType == VAR_STRING) {
				m_drawing->SetVariableString(m_varName, m_varValue);
			}
			else if (m_varType == VAR_INT) 
			{
				if (strcmp("$AUNITS", m_varName) == 0)
				{
					if (m_varInt == 0 || m_varInt == 1)
						m_angularUnit = UNIT_DEGREES;
					else if (m_varInt == 2)
						m_angularUnit = UNIT_GRADIANS;
					else if (m_varInt == 3)
						m_angularUnit = UNIT_RADIANS;
				}
				else if (strcmp("$ANGDIR", m_varName) == 0)
				{
					m_ccAngles = (m_varInt == 0);
				}
				m_drawing->SetVariableInt(m_varName, m_varInt);
			}
			else if (m_varType == VAR_DOUBLE) {
				m_drawing->SetVariableDouble(m_varName, m_varDouble);
			}
			else if (m_varType == VAR_VECTOR_DOUBLE) {
				m_drawing->SetVariableDoubleVector(m_varName, m_varX, m_varY, m_varZ, true, true, true);
			}
		}
		strcpy_s(m_varName, DXF_LINE_MAX, m_valueStr);
	}
	else if (m_code > 0 && m_code < 9)
	{
		strcpy_s(m_varValue, DXF_LINE_MAX, m_valueStr);
		m_varType = VAR_STRING;
	}
	else if (m_code >= 10 && m_code <= 39)
	{
		m_varType = VAR_VECTOR_DOUBLE;
		if (m_code == 10)
			m_varX = ValueToDouble();
		else if (m_code == 20)
			m_varY = ValueToDouble();
		else if (m_code == 30)
			m_varZ = ValueToDouble();
	}
	else if (m_code >= 40 && m_code <= 59)
	{
		m_varType = VAR_DOUBLE;
		m_varDouble = ValueToDouble();
	}
	else if (m_code >= 60 && m_code <= 99)
	{
		m_varType = VAR_INT;
		m_varInt = ValueToInt();
	}
	else if (m_code >= 0)
	{
		m_varType = VAR_STRING;
		strcpy_s(m_varValue, DXF_LINE_MAX, m_valueStr);
	}
}


void DxfParser::ParseLWPolyline()
{
	switch (m_code)
	{
		case 0:
			ParseEntityEnd();
			break;

		case 8:  // layer name
			m_dxfPolyline.layerName = m_valueStr;
			break;

		case 10:  // start of first vertex
			m_drawing->AddPolyline(m_dxfPolyline);
			m_dxfVertex.bulge = 0;
			m_dxfVertex.x = ValueToDouble();
			m_status = STATUS_LWPOLYLINE_VERTICES;
			break;

		case 43:  // constant width (optional, default = 0)
			m_dxfPolyline.startWidth = ValueToDouble();
			m_dxfPolyline.endWidth = ValueToDouble();
			break;

		case 70:  // polyline flags (0 - default, normal polyline, 1 - closed polyline)
			m_dxfPolyline.flags = (unsigned)ValueToInt();
			break;

		case 90:  // number of vertices
			m_dxfPolyline.nVertices = (unsigned)ValueToInt();
			m_dxfPolyline.isNVerticesValid = true;
			break;
	}
}


void DxfParser::ParseLWPolylineVertices()
{
	switch (m_code)
	{
		case 0:  // 0, entity end, start to parse a new entity
			m_drawing->AddPolylineVertex(m_dxfVertex);
			m_dxfVertex.bulge = 0;
			ParseEntityEnd();
			break;

		case 10:  // x, add current vertex to the drawing and start of new vertex
			m_drawing->AddPolylineVertex(m_dxfVertex);
			m_dxfVertex.bulge = 0;
			m_dxfVertex.x = ValueToDouble();
			break;

		case 20:  // y, continue current vertex
			m_dxfVertex.y = ValueToDouble();
			break;

		case 42:  // bulge
			m_dxfVertex.bulge = ValueToDouble();
			break;
	}
}


void DxfParser::ParsePolyline()
{
	switch (m_code)
	{
		case 0:  // start of VERTEX array or start of new entity
			if (strcmp(m_valueStr, "VERTEX") == 0) {
				m_drawing->AddPolyline(m_dxfPolyline);
				m_status = STATUS_VERTEX;
			}
			else {
				ParseEntityEnd();
			}
			break;

		case 8:  // layer name
			m_dxfPolyline.layerName = m_valueStr;
			break;

		case 40: // start width
			m_dxfPolyline.startWidth = ValueToDouble();
			break;

		case 41: // end width
			m_dxfPolyline.endWidth = ValueToDouble();
			break;

		case 70: // polyline flags
			m_dxfPolyline.flags = (unsigned)ValueToInt();
			break;
	}
}


void DxfParser::ParseVertex()
{
	switch (m_code)
	{
		case 0: // end of VERTEX seqence or start of new VERTEX entity
			m_drawing->AddPolylineVertex(m_dxfVertex);
			m_dxfVertex.bulge = 0;
			if (strcmp(m_valueStr, "SEQEND") == 0) {
				m_status = STATUS_POLYLINE;
			}
			else if (strcmp(m_valueStr, "VERTEX") == 0) {
				m_status = STATUS_VERTEX;
			}
			break;

		case 10:  // x
			m_dxfVertex.x = ValueToDouble();
			break;
		case 20:  // y
			m_dxfVertex.y = ValueToDouble();
			break;
		case 30:  // z
			m_dxfVertex.z = ValueToDouble();
			break;
		case 42:  // bulge factor
			m_dxfVertex.bulge = ValueToDouble();
			break;
	}
}


void DxfParser::ParseSpline()
{
	switch (m_code)
	{
		case 0:
			ParseEntityEnd();
			break;

		case 8:   // layer name
			break;
	}
}


void DxfParser::ParseHatch()
{
	switch (m_code)
	{
		case 0:  // End of HATCH entity, start of new entity
			ParseEntityEnd();
			break;

		case 8:  // layer name
			m_dxfHatch.layerName = m_valueStr;
			break;

		case 91: // number of boundary paths (loops)
			m_dxfHatch.nPaths = (unsigned)ValueToInt();
			m_drawing->AddHatch(m_dxfHatch);
			break;
				
		case 92: // start of hatch boundary path
			unsigned pathType = (unsigned)ValueToInt();
			m_status = STATUS_HATCH_PATH;
			if (pathType & 2)
				m_status = STATUS_HATCH_POLYLINE_PATH;
			break;
	}
}


void DxfParser::ParseHatchEdgeType()
{
	if (m_code == 72)
	{
		int edgeType = ValueToInt();
		if (edgeType == 1) {
			m_status = STATUS_HATCH_EDGE_LINE;
			m_nExpectedCoords = 4;
			m_nCoords = 0;
		}
		else if (edgeType == 2) {
			m_status = STATUS_HATCH_EDGE_CIRCLE;
			m_nExpectedCoords = 6;
			m_nCoords = 0;
		}
		else if (edgeType == 3) {
			m_status = STATUS_HATCH_EDGE_ELLIPSE;
			m_nExpectedCoords = 8;
			m_nCoords = 0;
		}
		else if (edgeType == 4) {
			m_status = STATUS_HATCH_EDGE_SPLINE;
			m_nCoords = 0;
		}
	}
}


void DxfParser::ParseHatchPath()
{
	switch (m_code)
	{
		case 0:  // Entity end, parse next entity
			ParseHatch();
			break;

		case 72: // edge type, it marks start of the hatch edge
			ParseHatchEdgeType();
			break;

		case 93: // number of edges in this boundary path
			m_dxfHatchPath.nEdges = (unsigned)ValueToInt();
			m_drawing->AddHatchPath(m_dxfHatchPath);
			break;

		case 92: // start of the next hatch path
			ParseHatch();
			break;
	}
}


void DxfParser::ParseHatchPolylinePath()
{
	switch (m_code)
	{
		case 0:  // end of HATCH entity, add last vertex and continue parsing the next entity
			ParseEntityEnd();
			break;

		case 72:  // is bulge valid for vertices?
			m_dxfHatchPolylinePath.hasBulge = (ValueToInt() != 0);
			break;

		case 73:  // is polyline closed?
			m_dxfHatchPolylinePath.closed = (ValueToInt() != 0);
			break;

		case 92:  // continue parsing the next hatch path
			ParseHatch();
			break;

		case 93:  // number of polyline vertices
			m_dxfHatchPolylinePath.nVertices = (unsigned)ValueToInt();
			m_nExpectedCoords = m_dxfHatchPolylinePath.nVertices * 2;
			if (m_dxfHatchPolylinePath.hasBulge)
				m_nExpectedCoords = m_dxfHatchPolylinePath.nVertices * 3;
			m_nCoords = 0;
			m_drawing->AddHatchPolylinePath(m_dxfHatchPolylinePath);
			m_status = STATUS_HATCH_POLYLINE_PATH_VERTICES;
			break;
	}
}


void DxfParser::ParseHatchPolylinePathVertices()
{
	switch (m_code)
	{
		case 10:  // x
			m_dxfVertex.x = ValueToDouble();
			m_nCoords++;
			break;

		case 20:  // y
			m_dxfVertex.y = ValueToDouble();
			m_nCoords++;
			if (!m_dxfHatchPolylinePath.hasBulge) {
				m_drawing->AddHatchPolylinePathVertex(m_dxfVertex);
			}
			break;

		case 42:  // bulge (optional)
			m_dxfVertex.bulge = ValueToDouble();
			m_nCoords++;
			m_drawing->AddHatchPolylinePathVertex(m_dxfVertex);
			break;

		default:  // unexpected code - syntax error
			m_status = STATUS_ERROR;
			break;
	}

	if (m_nCoords == m_nExpectedCoords)
	{
		m_nCoords = 0;
		m_dxfVertex.bulge = 0;  // reset bulge, because it is optional value
		m_status = STATUS_HATCH_POLYLINE_PATH;
	}
}


void DxfParser::ParseHatchEdge_Line()
{
	switch (m_code)
	{
		case 10:  // x1
			m_dxfHatchEdgeLine.x1 = ValueToDouble();
			m_nCoords++;
			break;
		case 11:  // x2
			m_dxfHatchEdgeLine.x2 = ValueToDouble();
			m_nCoords++;
			break;
		case 20:  // y1
			m_dxfHatchEdgeLine.y1 = ValueToDouble();
			m_nCoords++;
			break;
		case 21:  // y2
			m_dxfHatchEdgeLine.y2 = ValueToDouble();
			m_nCoords++;
			break;

		default:
			if (m_nCoords != m_nExpectedCoords)
				m_status = STATUS_ERROR;
			break;
	}

	if (m_nCoords == m_nExpectedCoords) {
		m_drawing->AddHatchEdge_Line(m_dxfHatchEdgeLine);
		m_status = STATUS_HATCH_PATH;
	}
}


void DxfParser::ParseHatchEdge_CircularArc()
{
	switch (m_code)
	{
		case 10:  // center x
			m_dxfHatchEdgeCircArc.cx = ValueToDouble();
			m_nCoords++;
			break;
		case 20:  // center y
			m_dxfHatchEdgeCircArc.cy = ValueToDouble();
			m_nCoords++;
			break;
		case 40:  // radius
			m_dxfHatchEdgeCircArc.radius = ValueToDouble();
			m_nCoords++;
			break;
		case 50:  // start angle
			m_dxfHatchEdgeCircArc.startAngle = ValueToAngle();
			m_nCoords++;
			break;
		case 51:  // end angle
			m_dxfHatchEdgeCircArc.endAngle = ValueToAngle();
			m_nCoords++;
			break;
		case 73:  // counter-clockwise flag
			m_dxfHatchEdgeCircArc.isCounterClockwise = (ValueToInt() != 0);
			m_nCoords++;
			break;

		default:
			if (m_nCoords != m_nExpectedCoords)
				m_status = STATUS_ERROR;
			break;
	}

	if (m_nCoords == m_nExpectedCoords) {
		m_drawing->AddHatchEdge_Circular(m_dxfHatchEdgeCircArc);
		m_status = STATUS_HATCH_PATH;
	}
}


void DxfParser::ParseHatchEdge_EllipticalArc()
{
	switch (m_code)
	{
		case 10:  // center x
			m_dxfHatchEdgeEllipticArc.cx = ValueToDouble();
			m_nCoords++;
			break;
		case 11:  // end point x of the major axis (relative to the center)
			m_dxfHatchEdgeEllipticArc.epx = ValueToDouble();
			m_nCoords++;
			break;
		case 20:  // center y
			m_dxfHatchEdgeEllipticArc.cy = ValueToDouble();
			m_nCoords++;
			break;
		case 21:  // end point y of the major axis (relative to the center)
			m_dxfHatchEdgeEllipticArc.epy = ValueToDouble();
			m_nCoords++;
			break;
		case 40:  // length of minor axis (percentage of major axis length)
			m_dxfHatchEdgeEllipticArc.minorAxisLength = ValueToDouble();
			m_nCoords++;
			break;
		case 50:  // start angle
			m_dxfHatchEdgeEllipticArc.startAngle = ValueToAngle();
			m_nCoords++;
			break;
		case 51:  // end angle
			m_dxfHatchEdgeEllipticArc.endAngle = ValueToAngle();
			m_nCoords++;
			break;
		case 73:  // counter-clockwise flag
			m_dxfHatchEdgeEllipticArc.isCounterClockwise = (ValueToInt() != 0);
			m_nCoords++;
			break;
		
		default:
			if (m_nCoords != m_nExpectedCoords)
				m_status = STATUS_ERROR;
			break;
	}

	if (m_nCoords == m_nExpectedCoords) {
		m_drawing->AddHatchEdge_Elliptical(m_dxfHatchEdgeEllipticArc);
		m_status = STATUS_HATCH_PATH;
	}
}


void DxfParser::ParseHatchEdge_Spline()
{
	switch (m_code)
	{
		case 0:   // HATCH entity end, add current linear hatch edge to the drawing and continue with parsing
			ParseEntityEnd();
			break;

		case 72:  // type of next edge, add current linear hatch edge to the drawing and
				  // continue with parsing of the next hatch edge
			ParseHatchEdgeType();
			break;

		case 92:  // path type, start to parsing next hatch path
			ParseHatch();
	}
}


void DxfParser::Parse()
{
	switch (m_status)
	{
		case STATUS_ROOT:
			if (m_code == 0) 
			{
				if (strcmp(m_valueStr, "SECTION") == 0)
					m_status = STATUS_SECTION_START;
			}
			break;

		case STATUS_SECTION_START:
			if (m_code == 2)
			{
				if (strcmp(m_valueStr, "HEADER") == 0) {
					m_status = STATUS_HEADER_SECTION;
				}
				else if (strcmp(m_valueStr, "TABLES") == 0) {
					m_status = STATUS_TABLES_SECTION;
				}
				else if (strcmp(m_valueStr, "BLOCKS") == 0) {
					m_status = STATUS_BLOCKS_SECTION;
				}
				else if (strcmp(m_valueStr, "ENTITIES") == 0) {
					m_status = STATUS_ENTITIES_SECTION;
				}
			}
			break;

		case STATUS_HEADER_SECTION:
			ParseHeader();
			break;

		case STATUS_TABLES_SECTION:
			if (m_code == 0)
			{
				if (strcmp(m_valueStr, "TABLE") == 0) {
					m_status = STATUS_TABLE;
				}
				else if (strcmp(m_valueStr, "ENDSEC") == 0) {
					m_status = STATUS_ROOT;
				}
			}
			break;

		case STATUS_TABLE:
			switch (m_code)
			{
				case 0:
					if (strcmp(m_valueStr, "ENDTAB") == 0) 
						m_status = STATUS_TABLES_SECTION;
					break;
				case 2:
					if (strcmp(m_valueStr, "LAYER") == 0) 
						m_status = STATUS_TABLE_LAYERS;
			}
			break;

		case STATUS_TABLE_LAYERS:
			if (m_code == 0)
			{
				if (strcmp(m_valueStr, "ENDTAB") == 0)
					m_status = STATUS_TABLES_SECTION;
				else if (strcmp(m_valueStr, "LAYER") == 0)
					m_status = STATUS_LAYER;
			}
			break;
		
		case STATUS_LAYER:
			switch (m_code)
			{
				case 0:
					m_drawing->AddLayer(m_dxfLayer);
					m_status = STATUS_TABLE_LAYERS;
					Parse();
					break;

				case 2:  // layer name
					m_dxfLayer.name = m_valueStr;
					break;

				case 70: // flags
					m_dxfLayer.flags = ValueToInt();
					break;
			}
			break;

		case STATUS_BLOCKS_SECTION:
			if (m_code == 0)
			{
				if (strcmp(m_valueStr, "BLOCK") == 0) {
					m_status = STATUS_BLOCK_HEADER;
					m_insideBlock = true;
				}
				else if (strcmp(m_valueStr, "ENDSEC") == 0) {
					m_status = STATUS_ROOT;
				}
			}
			break;
		
		case STATUS_BLOCK_HEADER:
			ParseBlockHeader();
			break;

		case STATUS_BLOCK:
			ParseBlock();
			break;

		case STATUS_ENTITIES_SECTION:
			if (m_code == 0)
			{
				if (strcmp(m_valueStr, "POINT") == 0)
					m_status = STATUS_POINT;
				else if (strcmp(m_valueStr, "LINE") == 0)
					m_status = STATUS_LINE;
				else if (strcmp(m_valueStr, "POLYLINE") == 0)
					m_status = STATUS_POLYLINE;
				else if (strcmp(m_valueStr, "LWPOLYLINE") == 0)
					m_status = STATUS_LWPOLYLINE;
				else if (strcmp(m_valueStr, "VERTEX") == 0)
					m_status = STATUS_VERTEX;
				else if (strcmp(m_valueStr, "ARC") == 0)
					m_status = STATUS_ARC;
				else if (strcmp(m_valueStr, "CIRCLE") == 0)
					m_status = STATUS_CIRCLE;
				else if (strcmp(m_valueStr, "ELLIPSE") == 0)
					m_status = STATUS_ELLIPSE;
				else if (strcmp(m_valueStr, "SPLINE") == 0)
					m_status = STATUS_SPLINE;
				else if (strcmp(m_valueStr, "INSERT") == 0)
					m_status = STATUS_INSERT;
				else if (strcmp(m_valueStr, "HATCH") == 0)
					m_status = STATUS_HATCH;
				else if (strcmp(m_valueStr, "ENDSEC") == 0)
					m_status = STATUS_ROOT;
			}
			break;
		
		case STATUS_POINT:
			ParsePoint();
			break;

		case STATUS_LINE:
			ParseLine();
			break;

		case STATUS_POLYLINE:
			ParsePolyline();
			break;

		case STATUS_LWPOLYLINE:
			ParseLWPolyline();
			break;

		case STATUS_LWPOLYLINE_VERTICES:
			ParseLWPolylineVertices();
			break;

		case STATUS_VERTEX:
			ParseVertex();
			break;

		case STATUS_ARC:
			ParseArc();
			break;

		case STATUS_CIRCLE:
			ParseCircle();
			break;

		case STATUS_SPLINE:
			ParseSpline();
			break;

		case STATUS_ELLIPSE:
			ParseEllipse();
			break;

		case STATUS_INSERT:
			ParseInsert();
			break;

		case STATUS_HATCH:
			ParseHatch();
			break;

		case STATUS_HATCH_PATH:
			ParseHatchPath();
			break;

		case STATUS_HATCH_POLYLINE_PATH:
			ParseHatchPolylinePath();
			break;
		
		case STATUS_HATCH_POLYLINE_PATH_VERTICES:
			ParseHatchPolylinePathVertices();
			break;

		case STATUS_HATCH_EDGE_LINE:
			ParseHatchEdge_Line();
			break;

		case STATUS_HATCH_EDGE_CIRCLE:
			ParseHatchEdge_CircularArc();
			break;

		case STATUS_HATCH_EDGE_ELLIPSE:
			ParseHatchEdge_EllipticalArc();
			break;

		case STATUS_HATCH_EDGE_SPLINE:
			ParseHatchEdge_Spline();
			break;
	} // switch
}


DxfParser::tResult DxfParser::Parse(const wchar_t* filename, DxfDrawing* drawing)
{
	FILE * dxfFile = NULL;
	tResult res = DXF_OK;
			
	if (_wfopen_s(&dxfFile, filename, L"rb") == 0)
	{
		m_varType = VAR_NONE;
		m_drawing = drawing;
		AllocateBuffers();

		while (ReadLine(dxfFile, m_codeStr, DXF_LINE_MAX) && ReadLine(dxfFile, m_valueStr, DXF_LINE_MAX))
		{
			m_code = atoi(m_codeStr);
			Parse();
			if (m_status == STATUS_ERROR) {
				res = DXF_ERR_SYNTAX;
				break;
			}
		}

		DestroyBuffers();
	}
	else
	{
		res = DXF_ERR_FILEOPEN;
	}

	return res;
}
