#include <iostream>
#include <string>
#include <filesystem>

void main()
{
    /*
    std::string stringpath = "asdfasdf";
    std::filesystem::path path = stringpath;
    std::cout << path.string() << std::endl;
    std::wcout << path.wstring() << std::endl;
    std::cout << std::endl;
    */

    {
        std::filesystem::path a = L"šě\u03B11";
        std::filesystem::path b = u8"šě\u03B11";
        std::cout << (a == b) << std::endl;
    }

    {
        std::wstring a = L"šě\u03B11";
        //std::string b = u"šě\u03B11";
    }

    {
        std::filesystem::path a = "x/y/z.pdf";
        std::filesystem::path b = "C:/x/y/z.pdf";

        std::cout << "a.has_root_directory() = " << a.has_root_directory() << std::endl;
        std::cout << "b.has_root_directory() = " << b.has_root_directory() << std::endl;
    }
}