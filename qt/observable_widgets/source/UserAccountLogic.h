#pragma once

#include <iostream>
#include <string>

#include "shared_observable.hpp"

class UserAccountLogic
{
public:
    UserAccountLogic()
    {
        join(std::bind(&UserAccountLogic::BuildUsername, this,  std::placeholders::_1, std::placeholders::_2), idModel, nameModel);
    }

public:
    shared_observable<std::string> nameModel;
    shared_observable<int> idModel;
    shared_observable<std::string> usernameModel;

private:
    void BuildUsername(int id, const std::string & name)
    {
        unique_txn{usernameModel} = name + " " + std::to_string(id);
    }
};