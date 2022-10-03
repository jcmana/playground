# usage:
```cpp
shared_obe<int> so;

auto observer = [](int value)
{
    std::cout << "value changed = " << value << std::endl;
};
so.observe(observer);

// Change value using unique transaction on observable 'so'. Invokes all observers.
unique_txn{so}.get() = 7;

// This outputs:
// value changed = 7

// Access value using shared transaction. Doesn't invoke observers.
const auto copy = shared_txn{so}.get();

// Creates copy of shared observable, both copies have the same shared value just like std::shared_ptr
auto so_copy = so;

// Changing value on copy invokes observers from all copies
unique_txn{so_copy}.get() = 3;

// This outputs:
// value changed = 3
```

# observable todo list:
- <s>conversion to const T</s>
- different assignment strategies
    - move without observers reset
    - assign different observable without observers reset (might be immossible)
    - maybe more = figure out the requirements
- mutex customization point for link, callback_store, basic_obe
- reduce the weight and optimize
- observe overload for previous and new value