#pragma once

#include <memory>
#include "config.h"

class user_type;

#ifdef DEFINE_PTR
void consume(user_type * t_ptr);
#endif

#ifdef DEFINE_REF
void consume(user_type & t);
#endif

#ifdef DEFINE_UP
void consume(std::unique_ptr<user_type> up_t);
#endif

#ifdef DEFINE_SP
void consume(std::shared_ptr<user_type> sp_t);
#endif