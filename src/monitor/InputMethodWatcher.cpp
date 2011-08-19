#include "InputMethodWatcher.h"
#include "KeypadBuddyServer.h"

CInputMethodWatcher::CInputMethodWatcher(CKeypadBuddyServer& aServer) : CActive(CActive::EPriorityStandard),
    iServer(aServer)
    {
    CActiveScheduler::Add(this);
    }

CInputMethodWatcher::~CInputMethodWatcher()
    {
    Cancel();
    }

void CInputMethodWatcher::WatchKeyL(TUint32 aKey)
    {
    Cancel();
    iKey = aKey;
    User::LeaveIfError(DoWatchKey());
    }

TInt CInputMethodWatcher::DoWatchKey()
    {
    iStatus = KRequestPending;
    TInt ret = iServer.FepRepository().NotifyRequest(iKey, iStatus);
    if (KErrNone == ret)
        {
        SetActive();
        }
    return ret;
    }

void CInputMethodWatcher::RunL()
    {
    if (iStatus.Int() >= 0)
        {
        DoWatchKey();
        iServer.InputMethodSettingsChanged();
        }
    }

void CInputMethodWatcher::DoCancel()
    {
    iServer.FepRepository().NotifyCancel(iKey);
    }

TBool CInputMethodWatcher::IsHealthy() const
    {
    return IsActive();
    }
