# frames

### motivation
C++ representation on well-known concept of a *frame of reference* borrowed from physics and *coordinate system* borrowed from mathematics:
- https://en.wikipedia.org/wiki/Frame_of_reference
- https://en.wikipedia.org/wiki/Coordinate_system

The goal was to represent a coordinate on interfaces not only using spatial information but pair it with its *frame of reference* or a *coordinate system*. In a complex system, dealing with geometry, it is inevitable to have many coordinate systems which could be the same, relative to each other (you can convert coordinates of one system to another) or completely independent (you can't convert coordinates between the systems). Unfortunatelly, this is often communicated only by a documentation contract and could lead to many issues:
- programming errors (expecting different coordinate system than received)
- documentation errors (changing coordinate system in code but not updating documentation)
- readability (hard to keep track of current coordinate system of coordinates in play)

Similarly to quantity units libraries (Au, mp-units, ...) the motivation is to represent a context of coordinates with strong type.

### core
`template<typename Space, typename Scalar>`<br>
`struct xy` 
- strongly typed coordinate bound to a `Space` with quantity represented by `Scalar`

`template<typename Space, typename Scalar>`<br>
`struct frame`<br>
- runtime representation of frame of reference or coordinate system

`struct space_default`
- default `Space` used for `frame` and `xy` instances

### convention
`xy<space_default>` 
- coordinate bound to `space_default`

`xy<>` or `xy<void>`
- coordinate bound to anonymous space

### usage
...


### todo
- rethink the approach implemented so far:
    - the problem is that `frames` is more `concept`ual library than implementation
    - maybe fallback to building `concept`s using C++20 and then building sample implementation library
    - current impl. is said sample implementation but is missing the conceptual approach behind it:
        - mathematical scalars
        - mathematical vectors
        - mathematical space with metrics and everything
        - coordinate systems built on space, support (not implement) non-linear and all the physics craziness
- supplied `frame` implementation hadling rotation and offset between frames of reference is just one possible implementation and user needs a way to provide his own for situations where the relation is more complex:
    - different `Scalar` types
    - more complicated transformation, affine for example with scale and skew
    - non-linear transformation
    - am I trying to define Hilbert space?

