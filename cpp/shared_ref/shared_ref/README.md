# shared_ref
 - never `nullptr`, never reseated shared reference
 - `shared_ref` is to raw reference (`&`) what `shared_ptr` is to raw pointer (`*`)

 # core interface
 `shared_ref() = deleted`
 - no default ctor

 `shared_ref(T)`
 - initialize by value

 `shared_ref(T *)`
 - initialize by owning raw pointer

  `shared_ref(shared_ptr<T>)`
  - initialize by `shared_ptr`
  - throws if `nullptr`

  `operator  *()`, `operator ->()`, `get()`
  - access stored reference

# utilize in dependency-injection
Instead of using `shared_ptr` for passing dependencies into a `class` where you have to handle `nullptr` states:
```cpp
class Service
    Service(shared_ptr<Dependency> sp) {
        if (sp == nullptr) {
            // what you gonna do now, throw in the ctor?
        }
    }
```

you can explicitly say **this class requires certain dependency or don't even bother instancing**:
```cpp
class Service
    Service(shared_ref<Dependency> sr) :
        m_sr(sr)
    {}
```
