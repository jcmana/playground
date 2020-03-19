#pragma once

#include <thread>
#include <memory>
#include <list>
#include <algorithm>
#include <functional>

class conversation
{
public:
    using table = std::list<std::function<void(int)>>;
    /*
    struct seat
    {
        seat(std::shared_ptr<table> sp_table) :
            m_sp_table(std::move(sp_table))
        {
        }

        void request(int d)
        {
            for (auto & pair : (*m_sp_table))
            {
                if (pair.first != std::this_thread::get_id())
                {
                    pair.second(d);
                }
            }
        }

        void respond(int d)
        {
        }

        std::shared_ptr<table> m_sp_table;
    };
    */

public:
    conversation()
    {
        m_sp_table = std::make_shared<table>();
    }

    ~conversation()
    {
        leave();
    }
    
    void enter(std::function<void(int)> functor)
    {
        m_functor = functor;
        //m_sp_table->insert(m_sp_table->end(), m_functor);
    }

    void leave()
    {
        //m_sp_table->erase(std::find(m_sp_table->begin(), m_sp_table->end(), m_functor));
    }

    void request(int d)
    {
    }

    void respond(int d)
    {
    }

private:
    std::function<void(int)> m_functor;
    std::shared_ptr<table> m_sp_table;
};