#include "user_type.h"
#include "forwarding_only.h"

int main()
{
#ifdef DEFINE_PTR
    user_type t;
    forward(&t);
#endif

#ifdef DEFINE_REF
    user_type t;
    forward(t);
#endif

#ifdef DEFINE_UP
    auto up_t = std::make_unique<user_type>();
    forward(std::move(up_t));
#endif

#ifdef DEFINE_SP
    auto sp_t = std::make_shared<user_type>();
    forward(std::move(sp_t));
#endif
}