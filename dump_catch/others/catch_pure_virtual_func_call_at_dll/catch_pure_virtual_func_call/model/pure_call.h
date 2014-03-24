#pragma once


class IPureCall
{
public:
    virtual ~IPureCall(){};
    IPureCall()
    {
        call_by_constructor();
    };
    virtual void pure_call() = 0;
    void call_by_constructor()
    {
        pure_call();
    }
};

class CPureCall : public IPureCall
{
public:
    CPureCall()
    {
    }
    void pure_call()
    {
    }
};