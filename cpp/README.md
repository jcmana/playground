Most of the stuff here is just one big mess from various attempts, ideas, proofs of concepts. There are few though which are kinda mature and maybe even finished or usable:
- `callback_store` - scope-safe and thread-safe callbacks
- `concurrency/switch_mutex` - unique or shared mutex which allows upgrading or downgrading (implemented using `std::mutex`)
- `link` - binds two scopes with one `link_element` in each allowing to check if other scope has been destroyed
- `reactive` - observable type implementation mainly for event-driven GUI programming
- `shared_ref` - `shared_ptr` which can't ever be `nullptr` or reseated