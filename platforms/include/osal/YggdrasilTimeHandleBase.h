#pragma once

#include <common/YggdrasilTypes.h>

/**
 * @brief xxxxxx
 *
 * xxxxxx
 */
class YggdrasilTimeHandleBase
{

//define space
public:

    struct UtcTime
    {
        uint64_t utc;
        int8_t   zone;
        int64_t  year;
        int64_t  month;
        int64_t  day;
        int64_t  hour;
        int64_t  minute;
        int64_t  second;
        int64_t  millisecond;
        int64_t  microsecond;
    };

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

    YggdrasilTimeHandleBase();

    ~YggdrasilTimeHandleBase();

    virtual double GetBootTime() = 0;

    virtual int64_t GetBootTimeMs() = 0;

    virtual int64_t GetBootTimeUs() = 0;

    virtual double GetAppStartTime() = 0;

    virtual int64_t GetAppStartTimeMs() = 0;

    virtual int64_t GetAppStartTimeUs() = 0;

    virtual UtcTime GetUtcTime() = 0;



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
