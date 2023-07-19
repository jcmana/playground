#include <iostream>
#include <vector>
#include <string>

#include "unique_ref.hpp"
#include "shared_ref.hpp"

struct intf
{
    virtual void method() = 0;
};

struct impl : intf
{
    virtual void method() override
    {
        std::cout << "impl::method()" << std::endl;
    };
};

shared_ref<int> return_shared_ref()
{
    return shared_ref<int>(0);
}

shared_ref<intf> cast_and_return_shared_ref()
{
    return shared_ref(impl());
}

int main()
{
    {
        shared_ref<int> sr(7);
        shared_ref<int> sr_copy = sr;

        (*sr)++;
        (*sr_copy)++;

        std::cout << (*sr) << std::endl;
        std::cout << (*sr_copy) << std::endl;

        //sr = 4;        // compile error, deleted function

        std::shared_ptr<int> sp = sr;
        (*sp)++;

        std::cout << (*sr) << std::endl;
        std::cout << (*sr_copy) << std::endl;
        std::cout << (*sp) << std::endl;

        auto sp_ptr = sp.get();
    }

    {
        const shared_ref<int> sr(7);
        (*sr)++;
    }

    {
        shared_ref<const int> sr(7);
        //(*sr)++;      // compile error, const expression
    }

    {
        shared_ref<std::string> sr("asdfasdf");
        std::cout << (*sr) << std::endl;
    }

    // Missing default ctor
    {
        //shared_ref<int> sr;
    }

    // Construct from literal conversion
    {
        shared_ref<std::string> sr("asdffgfd");
    }

    // Construct from a copy
    {
        std::string s = "tuhnhdfgh";
        shared_ref<std::string> sr(s);
    }

    // Construct from a rref
    {
        std::string s("tuhnhdfgh");
        shared_ref<std::string> sr(std::move(s));
    }

    // Construct from a shared_ptr
    {
        auto sp = std::make_shared<std::string>("xvcbjteyueetu");
        auto sr = shared_ref(sp);
        if (false) auto sq = shared_ref<int>(std::shared_ptr<int>());        // runtime exception
    }

    {
        unique_ref ur(7);
        //unique_ref<int> ur_copy = ur;             // compile error, deleted function
        //unique_ref<int> ur_move = std::move(ur);  // compile error, deleted function
    }

    // Return shared from a function
    {
        auto sr = return_shared_ref();
    }

    // From nullptr:
    {
        //shared_ref<int> sr(nullptr);
    }

    // constness and reseat-ability:
    {
        shared_ref sr(7);
        sr = static_cast<const shared_ref<int> &>(shared_ref(2));

        const shared_ref const_sr(4);
        //const_sr = sr;      // cannot assign to const
    }

    // implicit conversion to interface in ctor:
    {
        shared_ref<intf> sr(std::make_shared<impl>());
    }

    // implicit conversion to interface in cast:
    {
        auto sr_impl = shared_ref(impl());
        auto sr_intf_1 = static_cast<shared_ref<intf>>(sr_impl);
        shared_ref<intf> sr_intf_2{sr_impl};

        auto sr_from_fn = cast_and_return_shared_ref();
    }
}