#ifndef KEYPADBUDDY_ENGINE_H
#define KEYPADBUDDY_ENGINE_H


#include <e32base.h>

class MMonitorStateObserver
    {
public:
    virtual void MonitorDeactivated() = 0;
    virtual void MonitorActivated() = 0;
    };

class CKeypadBuddyEngine : public CActive
    {
public:
    static CKeypadBuddyEngine* NewL(MMonitorStateObserver& aObserver);
    ~CKeypadBuddyEngine();
    TBool MonitorActive() const;
    void SetMonitorActiveL(TBool aActive);
    void ResetCacheL();
private:
    enum TStage
        {
        EStageRendezvous,
        EStageLogon
        };
    CKeypadBuddyEngine(MMonitorStateObserver& aObserver);
    void ConstructL();
    void AttachToMonitor();
    void RunL();
    void DoCancel();
private:
    MMonitorStateObserver& iObserver;
    RProcess iMonitor;
    TStage iStage;
    };

#endif //KEYPADBUDDY_ENGINE_H
