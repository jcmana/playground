#include <iostream>

#include <cassert>

#include <thread>
#include <chrono>
#include <memory>
#include <vector>
#include <string>
#include <functional>
#include <random>

#include "model.hpp"

struct Marker
{
    int image;
    int x;
    int y;
    int state;
    // 0 = nothing
    // 1 = pending
    // 2 = resolved
};

class Markers
{
public:
    void Clear()
    {
        m_markers.clear();
    }

    void Add(const Marker & marker)
    {
        m_markers.emplace_back(model<Marker>(marker));
    }

    bool IsAnyPending() const
    {
        for (const auto & marker : m_markers)
        {
            if (marker.accessor().value().state == 1)
            {
                return true;
            }
        }

        return false;
    }

    bool IsEachPending() const
    {
        for (const auto & marker : m_markers)
        {
            if (marker.accessor().value().state != 1)
            {
                return false;
            }
        }

        return true;
    }

    bool IsAnyResolved() const
    {
        for (const auto & marker : m_markers)
        {
            if (marker.accessor().value().state == 2)
            {
                return true;
            }
        }

        return false;
    }


    bool IsEachResolved() const
    {
        for (const auto & marker : m_markers)
        {
            if (marker.accessor().value().state != 2)
            {
                return false;
            }
        }

        return true;
    }

    auto Count() const
    {
        return m_markers.size();
    }

    auto begin()
    {
        return m_markers.begin();
    }

    auto end()
    {
        return m_markers.end();
    }

    auto begin() const
    {
        return m_markers.begin();
    }

    auto end() const
    {
        return m_markers.end();
    }

private:
    std::vector<model<Marker>> m_markers;
};

/*
class MarkerObserver :
    public model_observer_intf<Marker>
{
public:
    // IObserver implementation:
    virtual void on_modification(const Marker & marker) override
    {
        std::cout << "emit markerModelChanged(const Marker & marker)" << std::endl;
    }
};
*/

class MarkersObserver :
    public model_observer_intf<Markers>
{
public:
    // IObserver implementation:
    virtual void on_modification(const Markers & markers) override
    {
        
    }
};

void main()
{
    // modifier/accessor synchronization test:
    if (false)
    {
        model<int> m;

        const auto t_proc = [&]
        {
            std::cout << "t: accessing" << std::endl;
            auto mm = m.modifier();

            std::cout << "t: modifying" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "t: modifying done" << std::endl;
        };
        std::thread t;

        {
            std::cout << "m: accessing" << std::endl;
            auto ma = m.accessor();

            t = std::thread(t_proc);

            std::cout << "m: reading" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "m: reading done" << std::endl;
        }

        std::this_thread::yield();

        {
            std::cout << "m: accessing" << std::endl;
            auto ma = m.accessor();

            std::cout << "m: reading" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "m: reading done" << std::endl;
        }

        t.join();
    }

    // model_guard test:
    if (false)
    {
        int n = 6;
        model<int> m(n);

        {
            auto mm = m.modifier_guard();
            mm.value() = 7;
            std::cout << mm.value() << std::endl; 
        }

        {
            auto ma = m.accessor_guard();
            std::cout << ma.value() << std::endl;
        }
    }

    // wait test:
    if (false)
    {
        model<int> m;

        std::thread ta;
        std::thread tb;

        const auto t_proc = [&]
        {
            {
                std::cout << "t: accessing" << std::endl;
                auto mm = m.modifier();

                std::cout << "t: writing" << std::endl;
                mm.value() = 7;
                std::cout << "t: writing done" << std::endl;
            }
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

    // marker test:
    if (false)
    {
        model<Markers> markers;

        MarkersObserver mc;
        const auto guard = markers.observe(mc);

        const auto t_proc = [&markers]
        {
            std::cout << "t: acquiring markers" << std::endl;
            {
                auto mm = markers.modifier();
                mm.value().Add(Marker{17, 0, 0, 1});
                mm.value().Add(Marker{18, 0, 0, 1});
                mm.value().Add(Marker{19, 0, 0, 1});

                //std::this_thread::sleep_for(std::chrono::seconds(1));
            }
            std::cout << "t: acquiring markers done" << std::endl;

            std::cout << "t: requesting markers resolution" << std::endl;
            while (markers.accessor().value().IsEachResolved() == false)
            {
                markers.wait();
            }
            std::cout << "t: markers resolution done" << std::endl;
        };
        std::thread t(t_proc);

        {
            while (markers.accessor().value().IsEachPending() == false)
            {
                markers.wait();
            }

            std::cout << "m: markers resolution start" << std::endl;

            auto mm = markers.modifier();

            for (auto & marker : mm.value())
            {
                auto mm = marker.modifier();
                mm.value().x = std::uniform_int_distribution<>(-100, +100)(std::default_random_engine());
                mm.value().y = std::uniform_int_distribution<>(-100, +100)(std::default_random_engine());
                mm.value().state = 2;
            }

            std::this_thread::sleep_for(std::chrono::seconds(1));

            std::cout << "m: markers resolution done" << std::endl;
        }

        t.join();
    }

    // reference model:
    if (true)
    {
        //Marker marker;
        //model<Marker &> m(marker);
    }
}
