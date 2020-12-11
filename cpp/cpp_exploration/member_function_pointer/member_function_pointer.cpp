#include <iostream>
#include <functional>

struct simple
{
    void method() const
    {
        std::cout << "A::method()" << std::endl;
    }

    void method(int n) const
    {
        std::cout << "A::method(int)" << std::endl;
    }
};

struct intf
{
    virtual void method() = 0;
    virtual void method_const() const = 0;
    virtual void method_parameteric(int n) = 0;
};

struct impl_a : intf
{
    virtual void method() override
    {
        std::cout << "impl_a::method()" << std::endl;
    }

    virtual void method_const() const override
    {
        std::cout << "impl_a::method_const()" << std::endl;
    }

    virtual void method_parameteric(int n) override
    {
        std::cout << "impl_a::method_parameteric()" << std::endl;
    }
};

struct impl_b : intf
{
    virtual void method() override
    {
        std::cout << "impl_b::method()" << std::endl;
    }

    virtual void method_const() const override
    {
        std::cout << "impl_b::method_const()" << std::endl;
    }

    virtual void method_parameteric(int n) override
    {
        std::cout << "impl_b::method_parameteric()" << std::endl;
    }
};

template<typename F>
constexpr auto pointer_to_member_id(F pointer_to_member)
{
    // Chosen type to represent pointer to member
    using ptr_id_t = std::ptrdiff_t;

    // Pointer-to-member type and ptr_id_t has to match, otherwise we have undefined behaviour
    static_assert(sizeof(ptr_id_t) == sizeof(F), "Type size mismatch.");

    return static_cast<ptr_id_t>(*reinterpret_cast<const ptr_id_t *>(&pointer_to_member));
}

int main()
{
    // pointer to normal class method
    {
        simple a;

        void(simple:: * ptr)() const = &simple::method;
        (a.*ptr)();

        void(simple:: * ptr_overload)(int) const = &simple::method;
        (a.*ptr)();
    }

    // pointer to polymorphic class method
    {
        intf * ia_ptr = new impl_a;
        intf * ib_ptr = new impl_b;

        void(intf:: * ptr_method)() = &intf::method;
        void(intf:: * ptr_method_const)() const = &intf::method_const;
        void(intf:: * ptr_method_parameteric)(int) = &intf::method_parameteric;

        (ia_ptr->*ptr_method_const)();
        (ib_ptr->*ptr_method_const)();

        delete ia_ptr;
        delete ib_ptr;
    }

    const auto intf_method_id = pointer_to_member_id(&intf::method);
    const auto intf_method_const_id = pointer_to_member_id(&intf::method_const);
    const auto intf_method_parametric_id = pointer_to_member_id(&intf::method_parameteric);

    // member function identification:
    {
        std::cout << std::hex << "0x" << intf_method_id << std::endl;
        std::cout << std::hex << "0x" << intf_method_const_id << std::endl;
        std::cout << std::hex << "0x" << intf_method_parametric_id << std::endl;
    }

    // WHAT IN THE ACTUAL FUCK?!

    // cast the id back to member pointer:
    {
        using method_ptr_t = void (intf:: *)();

        method_ptr_t intf_method_ptr;
        *reinterpret_cast<std::ptrdiff_t *>(&intf_method_ptr) = intf_method_id;

        intf * ia_ptr = new impl_a;
        intf * ib_ptr = new impl_b;

        (ia_ptr->*intf_method_ptr)();
        (ib_ptr->*intf_method_ptr)();

        delete ia_ptr;
        delete ib_ptr;
    }

    return 0;
}