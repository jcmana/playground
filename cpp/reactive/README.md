# shared_obe
Reactive, concurrent, scope-safe, templated approach to write interactive systems.

# core interface
`shared_obe(T value)`
- initializes observable with value

`shared_obe(const shared_obe & other)`
- creates a copy of observable with shared value but omits the observers

`shared_obe(shared_obe && other)`
- move observable with shared value and the observers

`observe(…)`
- subscribes a functor to receive change notifications
- subscription expires when local copy of observable is destroyed


# observers
Observing changes with `std::function`

```cpp
{
    shared_obe so = std::string(“initial string value“);

    auto observer = [](const std::string & value)
    {
    	std::cout << value << std::endl;
    };
    so.observe(observer);
}
```

# transactions
Transactional approach to reading, writing and notifying `shared_obe`s.

`unique_txn`
- unique access to the value
- read and write access
- notifies observers on expiration

`shared_txn`
- shared access to the value
- read access

```cpp
{
    shared_obe so = std::string("initial string value");

    auto observer = [](const std::string & value)
    {
        std::cout << value << std::endl;
    };
    so.observe(observer);

    unique_txn tx(so);
    std::string & value_ref = tx.get();
    value_ref = "changed string value";
}
```

# raii
Each copy of `shared_obe` holds its observers and deleting the copy will
reset local observers.

```cpp
auto observer = [](const std::string & value)
{
    std::cout << value << std::endl;
};

shared_obe so = std::string("initial string value");
so.observe(observer);

{
    shared_obe local_so = so;
    local_so.observe(observer);

    unique_txn(local_so) = "changed string value";
}

unique_txn(so) = "another changed string value";
```

# algorithms

`await_if`
- blocks until predicate is satisfied
- overloads `await_any`, `await`

`join`
- joins two `shared_obe`s into a single `std::tuple`
- invoked when either value changed

`transform`
- trasnforms value and forwards to another `share_obe` when changed

`forward`
- forwards value to another `shared_obe` when changed

`synchronize`
- synchronizes values between `shared_obe`s without causing an infinite loop

`compose`
- assigns value of `shared_obe` into a member of `struct` when changed

`decompose`
- assigns value of `struct` member into `shared_obe`

# todo
- <s>conversion to `const T`</s>
- different assignment strategies
    - move without observers reset
    - assign different observable without observers reset (might be immpossible)
    - maybe more = figure out the requirements
- `mutex` customization point for `link`, `callback_store`, `basic_obe`
- reduce the weight and optimize
- observe overload for previous and new value
- extend algorithms library