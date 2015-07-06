/*****************************************************************************/
/*                                                                           */
/*           系统名        ：        内蒙气象局后台服务                      */
/*           客户名        ：        内蒙气象局                              */
/*           机能名        ：        共通机能                                */
/*           程序名        ：        Thread.h                                */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*                VERSION    DATE      BY      CHANGE/COMMENT                */
/*---------------------------------------------------------------------------*/
/*                V1.00      09-11-11          Create                        */
/*                                                                           */
/*****************************************************************************/

#if !defined(_THREAD_H)
#define _THREAD_H

/*****************************************************************************
    系统include
******************************************************************************/
#include <list>

/*****************************************************************************
    用户include
******************************************************************************/
#include "VodUserType.h"
#include "Process.h"

class CThread {
public:
    void InitWork();
    // Init and then start the thread
    void Create();
    // Interrupt the thread and clean all the ressouce it used 
    void Cancel();
    // Ask the thread to die and clean all the ressouce it used
    // Should be preferred to Cancel unless you know what you do
    void Stop();
    void Join();
    // Interrupt the normal processing of a thread unless it has been created
    // ininterruptible (but this is not yet implemented :)
    void Interrupt();
    // Wait during a fixed delay before interrupting the thread if needed
    void DelayedInterruption();
    // Thread status (only the 1st one is implemented yet)
    bool IsCanceled() const;
    bool IsRunning()                  { return m_bIsRunning;   }
    void SetRunningState(bool bState) { m_bIsRunning = bState; }
    void CleanWork();
    void SetName(p_char_t pchName);
    p_char_t GetName()                { return m_chThreadName; }
    CThread();
    ~CThread();
private:
    // Internal method used to allow pthread "extern C" functions to call 
    // internal C++ methods
    static void* StartRoutine(void* pThread);
    IProcess*    m_pProcess;
    pthread_t    m_tId;
    bool         m_bIsRunning;
    char_t       m_chThreadName[20]; // Thread's name
};

#endif  //_THREAD_H

/*****************************************************************************
    End
*****************************************************************************/
