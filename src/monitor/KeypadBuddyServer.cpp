#include "KeypadBuddyServer.h"
#include "KeypadBuddySession.h"
#include "ForegroundApplicationWatcher.h"
#include "InputMethodWatcher.h"
#include "MonitorDefinitions.h"
#include <s32file.h>
#include <e32debug.h>
#include <akndef.h>
#include <APGTASK.H>
#include <aknglobalnote.h>
#include "KeypadBuddyUids.hrh"

const TUid KCRUidAknFep = { 0x101F876D };
const TUint32 KAknFepChineseInputMode = 0x00000004;
const TUint32 KAknFepInputTxtLang = 0x00000005;
const TUint32 KAknFepJapanesePredTxtFlag = 0x00000006;
const TUint32 KAknFepPredTxtFlag = 0x00000007;
const TUint32 KAknFepCangJieMode = 0x00000009;

_LIT(KCacheFileName, "cache.dat");

struct TInputMethodSettings
    {
    TInt iChineseInputMode;
    TInt iInputTxtLang;
    TInt iJapanesePredTxtFlag;
    TInt iPredTxtFlag;
    TInt iCangJieMode;
    TInputMethodSettings() :
        iChineseInputMode(0),
        iInputTxtLang(0),
        iJapanesePredTxtFlag(0),
        iPredTxtFlag(0),
        iCangJieMode(0)
        {}
    void LoadL(RDictionaryReadStream& aReadStream)
        {
        iChineseInputMode = aReadStream.ReadInt32L();
        iInputTxtLang = aReadStream.ReadInt32L();
        iJapanesePredTxtFlag = aReadStream.ReadInt32L();
        iPredTxtFlag = aReadStream.ReadInt32L();
        iCangJieMode = aReadStream.ReadInt32L();
        }
    void SaveL(RDictionaryWriteStream& aWriteStream)
        {
        aWriteStream.WriteInt32L(iChineseInputMode);
        aWriteStream.WriteInt32L(iInputTxtLang);
        aWriteStream.WriteInt32L(iJapanesePredTxtFlag);
        aWriteStream.WriteInt32L(iPredTxtFlag);
        aWriteStream.WriteInt32L(iCangJieMode);
        aWriteStream.CommitL();
        }
    };

CKeypadBuddyServer* CKeypadBuddyServer::NewL()
    {
    CKeypadBuddyServer* self = new(ELeave) CKeypadBuddyServer;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

CKeypadBuddyServer::CKeypadBuddyServer() : CServer2(CActive::EPriorityStandard)
    {}

void CKeypadBuddyServer::ConstructL()
    {
    User::LeaveIfError(iFs.Connect());
    StartL(KMonitorServerName);
    iFepRepository = CRepository::NewL(KCRUidAknFep);
    iForegroundAppWatcher = CForegroundApplicationWatcher::NewL(*this);
    iChineseInputModeWatcher = new(ELeave) CInputMethodWatcher(*this);
    iInputTxtLangWatcher = new(ELeave) CInputMethodWatcher(*this);
    iJapanesePredTxtFlagWatcher = new(ELeave) CInputMethodWatcher(*this);
    iPredTxtFlagWatcher = new(ELeave) CInputMethodWatcher(*this);
    iCangJieModeWatcher = new(ELeave) CInputMethodWatcher(*this);
    StartWatchingFepKeysL();
    }

CKeypadBuddyServer::~CKeypadBuddyServer()
    {
    delete iForegroundAppWatcher;
    delete iChineseInputModeWatcher;
    delete iInputTxtLangWatcher;
    delete iJapanesePredTxtFlagWatcher;
    delete iPredTxtFlagWatcher;
    delete iCangJieModeWatcher;
    delete iFepRepository;
    iFs.Close();
    }

CSession2* CKeypadBuddyServer::NewSessionL( const TVersion& /*aVersion*/, const RMessage2& /*aMessage*/ ) const
    {
    return new(ELeave) CKeypadBuddySession(const_cast<CKeypadBuddyServer&>(*this));
    }

void CKeypadBuddyServer::ForegroundApplicationChanged()
    {
    TRAP(iLastError, ForegroundApplicationChangedL());
    }

void CKeypadBuddyServer::ForegroundApplicationChangedL()
    {
    TUid uid = iForegroundAppWatcher->ForegroundAppL();
    if (uid != TUid::Null())
        {
        TFileName privatePath;
        User::LeaveIfError(GetCacheFilePath(privatePath));
        RProcess me;
        CDictionaryFileStore* fileStore = CDictionaryFileStore::OpenLC(iFs, privatePath, me.SecureId());
        if (fileStore->IsPresentL(uid))
            {
            RDictionaryReadStream in;
            in.OpenLC(*fileStore, uid);
            TInputMethodSettings settings;
            settings.LoadL(in);
            CancelWatchingFepKeys();
            TInt err = KErrNone;
            TUint32 keyInfo = 0;
            do
                {
                User::LeaveIfError(iFepRepository->StartTransaction(CRepository::EConcurrentReadWriteTransaction));
                if (settings.iChineseInputMode != KErrNotFound)
                    {
                    User::LeaveIfError(iFepRepository->Set(KAknFepChineseInputMode, settings.iChineseInputMode));
                    }
                if (settings.iInputTxtLang != KErrNotFound)
                    {
                    User::LeaveIfError(iFepRepository->Set(KAknFepInputTxtLang, settings.iInputTxtLang));
                    }
                if (settings.iJapanesePredTxtFlag != KErrNotFound)
                    {
                    User::LeaveIfError(iFepRepository->Set(KAknFepJapanesePredTxtFlag, settings.iJapanesePredTxtFlag));
                    }
                if (settings.iPredTxtFlag != KErrNotFound)
                    {
                    User::LeaveIfError(iFepRepository->Set(KAknFepPredTxtFlag, settings.iPredTxtFlag));
                    }
                if (settings.iCangJieMode != KErrNotFound)
                    {
                    User::LeaveIfError(iFepRepository->Set(KAknFepCangJieMode, settings.iCangJieMode));
                    }
                err = iFepRepository->CommitTransaction(keyInfo);
                __ASSERT_ALWAYS(err == KErrNone || err == KErrLocked, User::Leave(err));
                }
            while(err == KErrLocked);
            if (keyInfo > 0)
                {
                TInt focusedWg = iForegroundAppWatcher->WsSession().GetFocusWindowGroup();
                TWsEvent event;
                event.SetType(KEikInputLanguageChange);
                User::LeaveIfError(iForegroundAppWatcher->WsSession().SendEventToWindowGroup(focusedWg, event));
                RNotifier notif;
                User::LeaveIfError(notif.Connect());
                CleanupClosePushL(notif);
                TUid uid = {KKeypadBuddyNotifierUidValue};
                User::LeaveIfError(notif.StartNotifier(uid, KNullDesC8));
                CleanupStack::PopAndDestroy(&notif);
                }
            StartWatchingFepKeysL();
            CleanupStack::PopAndDestroy(&in);
            }
        // no else, no settings saved
        CleanupStack::PopAndDestroy(fileStore);
        }
    // no else, no tracking of applications without UID3
    }

void CKeypadBuddyServer::InputMethodSettingsChanged()
    {
    TRAP(iLastError, InputMethodSettingsChangedL());
    }

void CKeypadBuddyServer::InputMethodSettingsChangedL()
    {
    TUid uid = iForegroundAppWatcher->ForegroundAppL();
    if (uid != TUid::Null())
        {
        TInputMethodSettings settings;
        if (KErrNone != iFepRepository->Get(KAknFepChineseInputMode, settings.iChineseInputMode))
            {
            settings.iChineseInputMode = KErrNotFound;
            }
        if (KErrNone != iFepRepository->Get(KAknFepInputTxtLang, settings.iInputTxtLang))
            {
            settings.iInputTxtLang = KErrNotFound;
            }
        if (KErrNone != iFepRepository->Get(KAknFepJapanesePredTxtFlag, settings.iJapanesePredTxtFlag))
            {
            settings.iJapanesePredTxtFlag = KErrNotFound;
            }
        if (KErrNone != iFepRepository->Get(KAknFepPredTxtFlag, settings.iPredTxtFlag))
            {
            settings.iPredTxtFlag = KErrNotFound;
            }
        if (KErrNone != iFepRepository->Get(KAknFepCangJieMode, settings.iCangJieMode))
            {
            settings.iCangJieMode = KErrNotFound;
            }
        TFileName privatePath;
        User::LeaveIfError(GetCacheFilePath(privatePath));
        RProcess me;
        CDictionaryFileStore* fileStore = CDictionaryFileStore::OpenLC(iFs, privatePath, me.SecureId());
        RDictionaryWriteStream out;
        out.AssignLC(*fileStore, uid);
        settings.SaveL(out);
        fileStore->CommitL();
        CleanupStack::PopAndDestroy(2, fileStore);
        }
    // no else, no tracking for applications without UID3
    }

TInt CKeypadBuddyServer::GetCacheFilePath(TFileName& aFileName)
    {
    RProcess me;
    TBuf<2> drive = me.FileName().Left(2);
    if (drive.CompareF(_L("z:")) == 0)
        {
        drive = _L("c:");
        }
    TInt ret = iFs.PrivatePath(aFileName);
    if (KErrNone == ret)
        {
        aFileName.Insert(0, drive);
        ret = iFs.MkDirAll(aFileName);
        if (KErrAlreadyExists == ret)
            {
            ret = KErrNone;
            }
        if (KErrNone == ret)
            {
            aFileName.Append(KCacheFileName);
            }
        }
    return ret;
    }

CRepository& CKeypadBuddyServer::FepRepository()
    {
    return *iFepRepository;
    }

void CKeypadBuddyServer::ResetCacheL()
    {
    User::LeaveIfError(iFs.Delete(KCacheFileName));
    }

void CKeypadBuddyServer::StartWatchingFepKeysL()
    {
    iChineseInputModeWatcher->WatchKeyL(KAknFepChineseInputMode);
    iInputTxtLangWatcher->WatchKeyL(KAknFepInputTxtLang);
    iJapanesePredTxtFlagWatcher->WatchKeyL(KAknFepJapanesePredTxtFlag);
    iPredTxtFlagWatcher->WatchKeyL(KAknFepPredTxtFlag);
    iCangJieModeWatcher->WatchKeyL(KAknFepCangJieMode);
    }

void CKeypadBuddyServer::CancelWatchingFepKeys()
    {
    iChineseInputModeWatcher->Cancel();
    iInputTxtLangWatcher->Cancel();
    iJapanesePredTxtFlagWatcher->Cancel();
    iPredTxtFlagWatcher->Cancel();
    iCangJieModeWatcher->Cancel();
    }


void RunServerL()
    {
    RDebug::Print(_L("Test rdebug print"));
    CActiveScheduler* scheduler = new ( ELeave ) CActiveScheduler();
    CleanupStack::PushL( scheduler );
    CActiveScheduler::Install( scheduler );
    CKeypadBuddyServer* server = CKeypadBuddyServer::NewL();
    RProcess me;
    me.Rendezvous(KErrNone);
    CActiveScheduler::Start();
    delete server;
    CleanupStack::PopAndDestroy( scheduler );
    }

TInt E32Main()
    {
    TInt result;
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if ( cleanup )
        {
        TRAP(result, RunServerL());
        }
    else
        {
        result = KErrNoMemory;
        }
    delete cleanup;
    return result;
    }
