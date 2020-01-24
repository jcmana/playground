#pragma once

#include <thread>
#include <memory>
#include <map>
#include <functional>

class conversation
{
public:
    using table = std::map<std::thread::id, std::function<void(int)>>;

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
        auto pair = std::make_pair(std::this_thread::get_id(), functor);
        m_sp_table->insert(pair);
    }

    void leave()
    {
        m_sp_table->erase(std::this_thread::get_id());
    }

private:
    std::shared_ptr<table> m_sp_table;
};