#pragma once

#include "shared_observable.hpp"

#include "State.h"

void ComputationTask(int question, shared_observable<State> state, shared_observable<int> answer, shared_observable<bool> terminate)
{
    // Reset the output states
    unique_txn{state} = State::RUNNING;
    unique_txn{answer} = 0;

    // Compute the answer (q * 3 + 14) with the possibility to terminate early:
    auto q = question;
    auto a = 0;

    for (auto n = 0; n < 3; ++n)
    {
        a += q;

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        if (shared_txn{terminate}.get<0>())
        {
            unique_txn{state} = State::TERMINATED;
            return;
        }
    }

    for (auto n = 0; n < 14; ++n)
    {
        a += 1;

        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        if (shared_txn{terminate}.get<0>())
        {
            unique_txn{state} = State::TERMINATED;
            return;
        }
    }

    // Set the output states
    unique_txn{answer} = a;
    unique_txn{state} = State::FINISHED;
}
