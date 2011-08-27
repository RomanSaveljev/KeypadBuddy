#include "ForegroundApplicationWatcher.h"
#include <APGWGNAM.H>
#include "KeypadBuddyServer.h"
#include <akndef.h>

CForegroundApplicationWatcher* CForegroundApplicationWatcher::NewL(CKeypadBuddyServer& aServer)
    {
    CForegroundApplicationWatcher* self = new(ELeave) CForegroundApplicationWatcher(aServer);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

CForegroundApplicationWatcher::CForegroundApplicationWatcher(CKeypadBuddyServer& aServer) : CActive(CActive::EPriorityStandard),
    iServer(aServer)
    {
    CActiveScheduler::Add(this);
    }

void CForegroundApplicationWatcher::ConstructL()
    {
    User::LeaveIfError(iWsSession.Connect());
    iWg = RWindowGroup(iWsSession);
    User::LeaveIfError(iWg.Construct((TUint32)&iWg, EFalse));
    iWg.SetOrdinalPosition(-1);
    iWg.EnableReceiptOfFocus(EFalse);
    CApaWindowGroupName* wn = CApaWindowGroupName::NewLC(iWsSession);
    wn->SetHidden(ETrue);
    wn->SetWindowGroupName(iWg);
    CleanupStack::PopAndDestroy();
    User::LeaveIfError(iWg.EnableGroupChangeEvents());
    User::LeaveIfError(iWg.EnableFocusChangeEvents());
    User::LeaveIfError(iWg.EnableGroupListChangeEvents());
    Watch();
    }

CForegroundApplicationWatcher::~CForegroundApplicationWatcher()
    {
    Cancel();
    iWg.Close();
    iWsSession.Close();
    }

void CForegroundApplicationWatcher::Watch()
    {
    iStatus = KRequestPending;
    iWsSession.EventReady(&iStatus);
    SetActive();
    }

void CForegroundApplicationWatcher::RunL()
    {
    if (iStatus == KErrNone)
        {
        TWsEvent e;
        iWsSession.GetEvent(e);
        }
    if (iStatus != KErrCancel)
        {
        Watch();
        }
    iServer.ForegroundApplicationChanged();
    }

TUid CForegroundApplicationWatcher::ForegroundAppL()
    {
    TInt wgid = iWsSession.GetFocusWindowGroup();
    CApaWindowGroupName* gn;
    gn = CApaWindowGroupName::NewLC(iWsSession, wgid);
    TUid ret = gn->AppUid();
    CleanupStack::PopAndDestroy(gn);
    return ret;
    }

void CForegroundApplicationWatcher::DoCancel()
    {
    iWsSession.EventReadyCancel();
    }

