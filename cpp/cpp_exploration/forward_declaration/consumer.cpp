#include "user_type.h"
#include "consumer.h"

#ifdef DEFINE_PTR
void consume(user_type * t_ptr)
{
    t_ptr->method();
}
#endif

#ifdef DEFINE_REF
void consume(user_type & t)
{
    t.method();
}
#endif

#ifdef DEFINE_UP
void consume(std::unique_ptr<user_type> up_t)
{
    up_t->method();
}
#endif

#ifdef DEFINE_SP
void consume(std::shared_ptr<user_type> sp_t)
{
    sp_t->method();
}
#endif