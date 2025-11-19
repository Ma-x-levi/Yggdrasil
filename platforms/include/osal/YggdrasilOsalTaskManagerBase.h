#pragma once

#include <common/YggdrasilTypes.h>

class YggdrasilOsalTaskManagerBase
{

//define space
public:

//define space
protected:

//define space
private:


//variable space
public:

//variable space
protected:

//variable space
private:


//method space
public:
    YggdrasilOsalTaskManagerBase();

    ~YggdrasilOsalTaskManagerBase();


    virtual YggdrasilTypes::ReturnCode TaskCreate(std::string_view name, uint32_t stackSize) = 0;

//method space
protected:

//method space
private:


//interface space
public:

//interface space
protected:

//interface space
private:

};
