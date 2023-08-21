#pragma once

#include <iostream>
#include <string>

#include "observable.hpp"

class UserAccountLogic
{
public:
    UserAccountLogic()
    {
        //join(std::bind(&UserAccountLogic::BuildUsername, this,  std::placeholders::_1, std::placeholders::_2), idModel, nameModel);
    }

public:
    shared_obe<std::string> nameModel;
    shared_obe<int> idModel;
    shared_obe<std::string> usernameModel;

private:
    void BuildUsername(int id, const std::string & name)
    {
        unique_txn{usernameModel} = name + " " + std::to_string(id);
    }
};