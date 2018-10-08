#include <iostream>
#include <string>

using namespace std;

int main()
{
	const char * c_string;

	string std_string;

	std_string = "abcd";
	c_string = std_string.c_str();
	
	cout << c_string << endl;
	std_string[3] = 'x';
	cout << c_string << endl;
	std_string = "asdfljha sdkfjnas;lk fernjgt eogm sdlkfg djfnge;ljkrnygeo gkdfmgosd mgomhsopidmg [skdgmsdjlkfbniortg";
	cout << c_string << endl;
	
	return 0;
}