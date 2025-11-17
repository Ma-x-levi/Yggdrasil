#pragma once

#include <stdio.h>

#include <component/YggdrasilLoggerHandleBase.h>

#include <vector>

class NanoYggdrasil
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

    // YggdrasilLoggerHandleBase::HandlePtr loggerHandle;

    std::vector<std::reference_wrapper<YggdrasilLoggerHandleBase>> loggerHandleList;   //Todo: use no new realize


//method space
public:
    // NanoYggdrasil() = default;

    // ~NanoYggdrasil() = default;

    NanoYggdrasil();

    ~NanoYggdrasil();

    // void test(){ printf("hhhhh");}
    void test();

    YggdrasilTypes::ReturnCode RegisterLoggerHandle(std::reference_wrapper<YggdrasilLoggerHandleBase> handle);

    YggdrasilTypes::ReturnCode init();
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
