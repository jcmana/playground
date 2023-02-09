#pragma once

#include "config.h"

class user_type;

#ifdef DEFINE_PTR
void forward(user_type * t_ptr);
#endif

#ifdef DEFINE_REF
void forward(user_type & t);
#endif

#ifdef DEFINE_UP
void forward(std::unique_ptr<user_type> up_t);
#endif

#ifdef DEFINE_SP
void forward(std::shared_ptr<user_type> sp_t);
#endif