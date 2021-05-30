#include <iostream>

#include "../signalslot/signal.hpp"
#include "../signalslot/slot.hpp"
#include "../signalslot/connect.h"

void main()
{
    signal<int> signal_a;
    signal<int, int> signal_b;

    {
        auto slot_a_func = [](int value)
        {
            std::cout << "slot() = " << value << std::endl;
        };
        slot<int> slot_a(slot_a_func);

        auto slot_b_func = [](int value_x, int value_y)
        {
            std::cout << "slot() = " << value_x << ", " << value_y << std::endl;
        };
        slot<int, int> slot_b(slot_b_func);

        connect(signal_a, slot_a);
        connect(signal_b, slot_b);

        signal_b(3, 12);
        signal_a(7);
    }

    signal_b(3, 12);
    signal_a(7);
}
