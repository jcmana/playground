#include <iostream>
#include <mutex>

#include "basic_txn.hpp"

int main()
{
    // Brainstorming:
    // - is it just unique and shared?
    // - operations: read, modify, rollback?
    // - target type API:
    //      - support for the operations
    //      - main concern from shared_obe+unique_txn: how to handle modification notification?


    // First kick-off
    basic_txn_system s;
    basic_txn tx(s);
}