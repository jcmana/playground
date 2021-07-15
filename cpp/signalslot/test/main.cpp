#include <iostream>

#include "../signalslot/signal.hpp"
#include "../signalslot/slot.hpp"
#include "../signalslot/connect.h"

void main()
{
    if (false)
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

        // note:
        // signal+slot models a bit different situation than observable, which i inteded
        // to replace with this
        //
        // observable is more strict implementation:
        //  - supports only modification "signal"
        //  - handles and synchronizes the value storage
        //  - its composition doesn't allow to mix presentation logic with storage, which
        //    is easily achievable with signal+slot
    }

    // implementing observables using signal+slot
    if (true)
    {
        struct observable : signal<int>
        {
            observable()
            {
            }

            observable(int value) :
                m_value(value)
            {
            }

            void set(int value)
            {
                m_value = value;
                (*this)(m_value);
            }

            int get() const
            {
                return m_value;
            }

        private:
            int m_value;
        };

        struct observer : slot<int>
        {
            using slot<int>::slot;
        };

        auto f_a = [](int value)
        {
            std::cout << "observer a: " << value << std::endl;
        };
        auto f_b = [](int value)
        {
            std::cout << "observer b: " << value << std::endl;
        };

        observable o(17);
        observer a(f_a);
        observer b(f_b);

        connect(o, a);
        connect(o, b);

        o.set(4);
        o.set(7);
    }
}
