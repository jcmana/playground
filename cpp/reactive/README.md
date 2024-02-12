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

`forward`
- forwards value to another `shared_obe` when changed

`synchronize`
- synchronizes values between `shared_obe`s without causing an infinite loop

`compose`
- assigns value of `shared_obe` into a member of `struct` when changed

`decompose`
- assigns value of `struct` member into `shared_obe`

# abstractions
Reactive library is built upon following abstractions, ordered from the most primitive to the most abstract:

`atomic_link_element`
- binds thread execution stacks together

`atomic_callback` 
- 1:1 notification

`atomic_callback` + `atomic_callback_guard`
- 1:1 notification with scope safety

`atomic_callback_store`
- 1:N notification with scope safety
 
 `shared_evt`
 - 1:N notification subscription with scope-local copy-move semantics

 `shared_obe`
 - 1:N notification subscription with value and transactions

# todo
- <s>conversion to `const T`</s>
- different assignment strategies
    - move without observers reset
    - assign different observable without observers reset (might be immpossible)
    - maybe more = figure out the requirements
- `mutex` customization point for `link`, `callback_store`, `basic_obe`
- reduce the weight and optimize
- observe overload for previous and new value (not a part of API, just how-to)
- <s>weak observable reference - `shared_obe_weak` - to break circular dependency leaks</s>
- extend algorithms library

# ideas
- `atomic_callback_store` might be similar redundancy as deprecated implicit `std::tuple` observable type
    - build `shared_obe` upon simple `atomic_callback` and provide 1:N notification as a different
      class used as a `atomic_callback` template (something like `atomic_callback<std::vector<observers>>`) 
- use `shared_evt` as a building block for `shared_obe` - needs refactoring first