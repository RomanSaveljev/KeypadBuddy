#include "KeypadBuddyEngine.h"
#include "MonitorDefinitions.h"
#include <e32uid.h>

class RMonitorApi : public RSessionBase
    {
public:
    TInt Connect()
        {
        TInt ret = CreateSession(KMonitorServerName, TVersion(1, 0, 0));
        return ret;
        }
    TInt Deactivate()
        {
        return SendReceive(EMonitorDeactivate);
        }
    TInt ResetCache()
        {
        return SendReceive(EMonitorResetCache);
        }
    };

CKeypadBuddyEngine* CKeypadBuddyEngine::NewL(MMonitorStateObserver& aObserver)
    {
    CKeypadBuddyEngine* self = new(ELeave) CKeypadBuddyEngine(aObserver);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

CKeypadBuddyEngine::CKeypadBuddyEngine(MMonitorStateObserver& aObserver) : CActive(CActive::EPriorityStandard),
    iObserver(aObserver),
    iStage(EStageRendezvous)
    {
    CActiveScheduler::Add(this);
    }

void CKeypadBuddyEngine::ConstructL()
    {
    TFindProcess findProcess(KMonitorServerNameSearch);
    TFullName name;
    if (findProcess.Next(name) == KErrNone)
        {
        TInt err = iMonitor.Open(name);
        User::LeaveIfError(err);
        AttachToMonitor();
        }
    }

CKeypadBuddyEngine::~CKeypadBuddyEngine()
    {
    Cancel();
    iMonitor.Close();
    }

TBool CKeypadBuddyEngine::MonitorActive() const
    {
    RProcess me;
    return iMonitor.Handle() != me.Handle() && iStage == EStageLogon;
    }

void CKeypadBuddyEngine::SetMonitorActiveL(TBool aActive)
    {
    if (aActive)
        {
        if (MonitorActive())
            {
            // can not activate twice
            User::Leave(KErrAlreadyExists);
            }
        else
            {
            if (!IsActive())
                {

                TInt err = iMonitor.Create(KMonitorServerFileName, KNullDesC, TUidType(KExecutableImageUid, TUid::Null(), TUid::Null()));
                //TInt err = iMonitor.Create(_L("about.exe"), KNullDesC);
                User::LeaveIfError(err);
                iStatus = KRequestPending;
                iMonitor.Rendezvous(iStatus);
                iMonitor.Resume();
                SetActive();
                }
            // no else, server startup/shutdown is in progress
            }
        }
    else
        {
        if (!MonitorActive())
            {
            User::Leave(KErrNotReady);
            }
        else
            {
            RMonitorApi api;
            User::LeaveIfError(api.Connect());
            CleanupClosePushL(api);
            User::LeaveIfError(api.Deactivate());
            CleanupStack::PopAndDestroy(&api);
            }
        }
    }

void CKeypadBuddyEngine::ResetCacheL()
    {
    if (MonitorActive())
        {
        RMonitorApi api;
        User::LeaveIfError(api.Connect());
        CleanupClosePushL(api);
        User::LeaveIfError(api.ResetCache());
        CleanupStack::PopAndDestroy(&api);
        }
    else
        {
        User::Leave(KErrNotReady);
        }
    }

void CKeypadBuddyEngine::AttachToMonitor()
    {
    ASSERT(!IsActive());
    iStage = EStageLogon;
    iStatus = KRequestPending;
    iMonitor.Logon(iStatus);
    SetActive();
    }

void CKeypadBuddyEngine::RunL()
    {
    if (iStage == EStageRendezvous && iStatus.Int() == KErrNone)
        {
        AttachToMonitor();
        iObserver.MonitorActivated();
        }
    else
        {
        iStage = EStageRendezvous;
        iMonitor.Close();
        iMonitor = RProcess();
        iObserver.MonitorDeactivated();
        }
    }

void CKeypadBuddyEngine::DoCancel()
    {
    if (iStage == EStageRendezvous)
        {
        iMonitor.RendezvousCancel(iStatus);
        }
    else
        {
        iMonitor.LogonCancel(iStatus);
        }
    }
