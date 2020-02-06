#include <iostream>
#include <thread>
#include <chrono>
#include <memory>
#include <vector>
#include <string>
#include <functional>

#include "model.h"

class Observer :
    public IObserver
{
public:
    Observer(const std::string & name = "value") :
        m_name(name)
    {
    }

public:
    // IObserver implementation:
    virtual void OnModification() override
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
    // model_waitable test:
    if (false)
    {
        std::thread t;
        model_waitable w;

        const auto t_procedure  = [&]
        {
            std::cout << "t: work started" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            w.trigger();
            std::cout << "t: work done" << std::endl;
        };

        std::cout << "main: work started" << std::endl;

        t = std::thread(t_procedure);
        w.wait();
        std::cout << "main: work done" << std::endl;
        t.join();

        t = std::thread(t_procedure);
        w.wait();
        std::cout << "main: work done" << std::endl;
        t.join();
    }

    // model_observable test:
    if (false)
    {
        model_observable o;
        
        Observer r;
        Observer s;
        const auto g = o.observe(r);
        const auto h = o.observe(s);

        const auto t_procedure = [&]
        {
            std::cout << "t: work started" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            o.trigger();
            std::cout << "t: work done" << std::endl;
        };

        std::thread t(t_procedure);
        t.join();
    }

    // model_waitable lifetime test:
    if (false)
    {
        std::thread t;
        {
            model_waitable mw;

            const auto t_proc = [&]
            {
                mw.wait();
            };
            t = std::thread(t_proc);
        }

        t.join();
    }

    // Markers test:
    if (true)
    {
        Markers markersModel;

        {
            Marker m;
            m.x.modifier().value() = 7;
            m.y.modifier().value() = 7;
            m.resolved.modifier().value() = false;

            markersModel.markers.modifier().value().push_back(m);
        }

        std::unique_ptr<IObserver> upObserverMarkersX = std::make_unique<Observer>("x");
        std::unique_ptr<IObserver> upObserverMarkersY = std::make_unique<Observer>("y");
        const auto guardX = markersModel.markers.accessor().value().back().x.observe(*upObserverMarkersX);
        const auto guardY = markersModel.markers.accessor().value().back().y.observe(*upObserverMarkersY);

        const auto t_proc = [&]
        {
            std::cout << "t: waiting for modification" << std::endl;
            markersModel.markers.wait();
            std::cout << "t: modified" << std::endl;
        };
        std::thread t(t_proc);

        std::this_thread::sleep_for(std::chrono::seconds(1));

        markersModel.markers.modifier().value().back().x.modifier().value() = 7;
        markersModel.markers.modifier().value().back().y.modifier().value() = 2;
        markersModel.markers.modifier().value().back().x.modifier().value() = 12;

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