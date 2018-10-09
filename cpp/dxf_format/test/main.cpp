#include "../dxf_reader/dxf_parser.h"

int main()
{
	DxfDrawing d;
	DxfParser p;

	DxfParser::tResult res = p.Parse(L"C:\\dev\\demo\\projects\\librecad_test.dxf", &d);

	return 0;
}