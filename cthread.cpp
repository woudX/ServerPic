#include "cthread.h"

CThread::~CThread()
{
    if (!IsOver())
        Kill();

    safe_del(_t);
}

bool CThread::IsOver()
{
    return state == CThreadState::Abort || state == CThreadState::Finish;
}

void CThread::Kill()
{
    pthread_t tid = _t->native_handle();
    pthread_kill(tid, SIGTERM);
}

void CThread::Join()
{
    _t->join();
}

void CThread::Detach()
{
    _t->detach();
}
