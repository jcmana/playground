#include <iostream>
#include <string>

#include <stdlib.h>
#include <wtypes.h>


BYTE ConvertCharToByte(const char c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	if (c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	if (c >= 'A' && c <= 'F')
		return c - 'A' + 10;
	return -1;
}

void ConvertCharArrayToByteArray(BYTE * dst, const char * src, size_t size)
{
	for (int i = 0; i < size; i = i + 2)
	{
		dst[i / 2] = ConvertCharToByte(src[i]) << 4 | ConvertCharToByte(src[i + 1]);
	}
}

int main()
{
	std::string sHWaddress = "0022bd36d000";
	const char * sHWaddressRaw = sHWaddress.c_str();
	
/*
	unsigned long long int pHWaddress = 0;
	int nReadInputCount = 0;
	sscanf(sHWaddress.c_str(), "%12llx%n", &pHWaddress, &nReadInputCount);

	BYTE * pHWaddressBytes = (BYTE *)(&pHWaddress);

	int i = 0;
	std::cout << std::hex;
	for (; i < 5; ++i)
	{
		std::cout << (unsigned int)(pHWaddressBytes[i]) << ":";
	}
	std::cout << pHWaddressBytes[i] << std::endl;
*/

	BYTE pHWaddressBytes[6] = {};

	ConvertCharArrayToByteArray(pHWaddressBytes, sHWaddress.c_str(), sHWaddress.size());

	return 0;
}