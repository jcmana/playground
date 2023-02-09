#include "consumer.h"

#ifdef DEFINE_PTR
void forward(user_type * t_ptr)
{
    consume(t_ptr);
}
#endif

#ifdef DEFINE_REF
void forward(user_type & t)
{
    consume(t);
}
#endif

#ifdef DEFINE_UP
void forward(std::unique_ptr<user_type> up_t)
{
    consume(std::move(up_t));
}
#endif DEFINE_UP

#ifdef DEFINE_SP
void forward(std::shared_ptr<user_type> sp_t)
{
    consume(sp_t);
}
#endif