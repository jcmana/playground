#include <iostream>

#include "../signals/connection.hpp"

struct fucker
{
    fucker() :
        shit([this]{ assfuck = true; })
    {
    }

    slot<> shit;

    bool assfuck = false;
};

void main()
{
    if (false)
    {
        auto f = []
        {
            std::cout << "holy fuck slot" << std::endl;
        };
        slot<> st(f);

        signal<> sl;

        fucker fr;

        sl.connect(st);
        sl.connect(fr.shit);
        sl.send();
    }

    if (true)
    {
        signal<int> sl;

        auto f = [](int n)
        {
            std::cout << "holy fuck numbered slot " << n << std::endl;
        };
        slot<int> st(f);

        //sl.connect(st);
        connect(sl, st);
        sl.send(4);
    }
}
