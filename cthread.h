#ifndef CTHREAD
#define CTHREAD

#include "stdafx.h"

enum CThreadState
{
    Ready, Running, Finish, Abort
};

class CThread
{
public:
    template<typename _Callable, typename..._Args> explicit
    CThread(_Callable&& __f, _Args&&... __args)
    {
        auto func = [](CThread *_this, _Callable&& _func, _Args&&... _args) {
            (__bind_simple(forward<_Callable>(_func), forward<_Args>(_args)...))();

            _this->state = CThreadState::Finish;
        };

        _t = new thread(func, this, __f, __args...);
    }

    bool IsOver();
    void Kill();
    void Join();
    void Detach();

    ~CThread();

    CThreadState state;

private:
    thread *_t;
};

#endif // CTHREAD

