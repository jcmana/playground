#include <iostream>
#include <thread>
#include <chrono>
#include <memory>
#include <vector>
#include <string>
#include <functional>

#include "model_waitable.hpp"
#include "model_observable.hpp"
#include "model.hpp"

class observer :
    public model_observer_intf<int>
{
public:
    observer(const std::string & name = "value") :
        m_name(name)
    {
    }

public:
    // IObserver implementation:
    virtual void on_modification(model_accessor<int>) override
    {
        std::cout << m_name << " modified" << std::endl;
    }

private:
    std::string m_name;
};

enum class Resolution
{
    IDLE,
    PENDING,
    RESOLVED
};

struct Marker
{
    model<int> x;
    model<int> y;
    model<bool> resolved;
};

struct Markers
{
    model<std::vector<Marker>> markers;
};

void main()
{
    // modifier/accessor synchronization test:
    if (false)
    {
        model<int> m;

        const auto t_proc = [&]
        {
            auto mm = m.modifier();

            std::cout << "t: modifying" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "t: modifying done" << std::endl;
        };
        std::thread t;

        {
            auto ma = m.accessor();

            t = std::thread(t_proc);

            std::cout << "m: reading" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "m: reading done" << std::endl;
        }

        {
            auto ma = m.accessor();

            std::cout << "m: reading" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "m: reading done" << std::endl;
        }

        t.join();
    }

    // wait test:
    if (true)
    {
        model<int> m;

        std::thread ta;
        std::thread tb;

        const auto t_proc = [&]
        {
            std::cout << "t: accessing" << std::endl;
            auto mm = m.modifier();

            std::cout << "t: writing" << std::endl;
            mm.value() = 7;
            std::cout << "t: writing done" << std::endl;
        };

        ta = std::thread(t_proc);

        {
            std::cout << "m: accessing" << std::endl;
            auto ma = m.wait();

            tb = std::thread(t_proc);

            std:: cout << "m: reading" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "m: value = " << ma.value() << std::endl;
            std::cout << "m: reading done" << std::endl;
        }

        ta.join();
        tb.join();
    }

    // Markers test:
    if (false)
    {
        model<std::vector<Marker>> markers;

        {
            Marker m;
            m.x.modifier().value() = 7;
            m.y.modifier().value() = 7;
            m.resolved.modifier().value() = false;

            markers.modifier().value().push_back(m);
        }

        std::unique_ptr<model_observer_intf<int>> upObserverMarkersX = std::make_unique<observer>("x");
        std::unique_ptr<model_observer_intf<int>> upObserverMarkersY = std::make_unique<observer>("y");
        const auto guard_x = markers.accessor().value().back().x.observe(*upObserverMarkersX);
        const auto guard_y = markers.accessor().value().back().y.observe(*upObserverMarkersY);

        const auto t_proc = [&]
        {
            std::cout << "t: waiting for modification" << std::endl;
            markers.wait();
            std::cout << "t: modified" << std::endl;
        };
        std::thread t(t_proc);

        std::this_thread::sleep_for(std::chrono::seconds(1));

        markers.modifier().value().back().x.modifier().value() = 7;
        markers.modifier().value().back().y.modifier().value() = 2;
        markers.modifier().value().back().x.modifier().value() = 12;

        t.join();
    }

    // move, copy test:
    if (false)
    {
        model_waitable mw;
        //auto mw_copy = mw;                // not copyable
        //auto mw_move = std::move(mw);     // not movable

        model_observable mo;
        //auto mo_copy = mo;                // not copyable
        auto mo_move = std::move(mo);       // movable

        model<int> m;
        auto m_copy = m;
        auto m_move = std::move(m);
    }
}
