# transaction
Transaction is a concept how to approach accessing certain datatype:
- entrire transaction is atomic
- operations on datatype are available exclusively through transactions
- thread and scope safety
- examples:
    - read transaction (multiple readers, no writers, immutable operations only)
    - write transaction (single reader, singler writer, mutable and immutable operations)

# todo
- form the initial idea
- stabilize interface
- test on several cases
- full implementation and documentation
- backport into `shared_evt` and `shared_obe` implementations