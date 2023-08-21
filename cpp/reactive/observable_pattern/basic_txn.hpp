#pragma once

template<typename T>
class basic_txn
{
public:    
    using scoped_observable_type = typename shared_obe<T>;
    using observable_type = typename scoped_observable_type::observable_type;
    using value_type = typename scoped_observable_type::value_type;
    using observed_type = typename scoped_observable_type::observed_type;
};