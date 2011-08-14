#ifndef INPUTMETHODWATCHER_H
#define INPUTMETHODWATCHER_H

#include <e32base.h>

class CKeypadBuddyServer;

class CInputMethodWatcher : public CActive
    {
public:
    CInputMethodWatcher(CKeypadBuddyServer& aServer);
    ~CInputMethodWatcher();
    void WatchKeyL(TUint32 aKey);
    TBool IsHealthy() const;
private:
    void RunL();
    void DoCancel();
    TInt DoWatchKey();
private:
    CKeypadBuddyServer& iServer;
    TUint32 iKey;
    };

#endif //INPUTMETHODWATCHER_H
