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
#include <apgcli.h>
#include <apacmdln.h>
#include <e32debug.h>
#include <bautils.h>

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

CKeypadBuddyServer* CKeypadBuddyServer::NewL(RFs& aFs)
    {
    CKeypadBuddyServer* self = new(ELeave) CKeypadBuddyServer(aFs);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

CKeypadBuddyServer::CKeypadBuddyServer(RFs& aFs) : CServer2(CActive::EPriorityStandard),
    iFs(aFs)
    {}

void CKeypadBuddyServer::ConstructL()
    {
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
    if (uid != TUid::Null() && uid.iUid != (TInt32)KKeypadBuddyUidValue)
        {
        CDictionaryFileStore* fileStore = CreateFileStoreLC(iFs, KCachedFepSettingsStream);
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
                User::After(500000);
                event.SetType(EEventFocusLost);
                TInt err = iForegroundAppWatcher->WsSession().SendEventToWindowGroup(focusedWg, event);
                User::LeaveIfError(err);
                User::After(500000);
                event.SetType(EEventFocusGained);
                err = iForegroundAppWatcher->WsSession().SendEventToWindowGroup(focusedWg, event);
                User::LeaveIfError(err);
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
    if (uid != TUid::Null() && uid.iUid != (TInt32)KKeypadBuddyUidValue)
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
        CDictionaryFileStore* fileStore = CreateFileStoreLC(iFs, KCachedFepSettingsStream);
        RDictionaryWriteStream out;
        out.AssignLC(*fileStore, uid);
        settings.SaveL(out);
        fileStore->CommitL();
        CleanupStack::PopAndDestroy(2, fileStore);
        }
    // no else, no tracking for applications without UID3
    }

CDictionaryFileStore* CKeypadBuddyServer::CreateFileStoreLC(RFs& aFs, TUint32 aStreamUid)
    {
    TUid streamUid = {aStreamUid};
    return CDictionaryFileStore::OpenLC(aFs, KCacheFileName, streamUid);
    }

void CKeypadBuddyServer::WriteActivationEnabledSettingL(CDictionaryFileStore& aFileStore, TBool aValue)
    {
    RDictionaryWriteStream out;
    TUid activationEnabledSettingUid = {KActivationEnabledSetting};
    out.AssignLC(aFileStore, activationEnabledSettingUid);
    out.WriteInt32L(aValue);
    out.CommitL();
    aFileStore.CommitL();
    CleanupStack::PopAndDestroy(&out);
    }

CRepository& CKeypadBuddyServer::FepRepository()
    {
    return *iFepRepository;
    }

void CKeypadBuddyServer::ResetCacheL()
    {
    User::LeaveIfError(iFs.Delete(KCacheFileName));
    CDictionaryFileStore* fileStore = CreateFileStoreLC(iFs, KCachedFepSettingsStream);
    // can not reset settings when application is not active, i.e. activation
    // is enabled
    WriteActivationEnabledSettingL(*fileStore, ETrue);
    CleanupStack::PopAndDestroy(); //fileStore
    }

void CKeypadBuddyServer::DeactivateServerL()
    {
    CDictionaryFileStore* fileStore = CreateFileStoreLC(iFs, KMonitorSettingsStream);
    WriteActivationEnabledSettingL(*fileStore, EFalse);
    CleanupStack::PopAndDestroy();
    CActiveScheduler::Stop();
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

void CKeypadBuddyServer::RunServerL()
    {
    TFindServer find(KMonitorServerName);
    TFullName result;
    if (KErrNone != find.Next(result))
        {
        RFs fs;
        User::LeaveIfError(fs.Connect());
        CleanupClosePushL(fs);
        TBool forceStart = EFalse;
        User::GetTIntParameter(KForceActivateArgumentSlot, forceStart);
        if (!BaflUtils::FileExists(fs, KCacheFileName))
            {
            // this is the first launch after installation - force start
            TChar driveChar;
            User::LeaveIfError(fs.DriveToChar(KDefaultDrive, driveChar));
            TInt err = fs.CreatePrivatePath(KDefaultDrive);
            __ASSERT_ALWAYS(err == KErrNone || err == KErrAlreadyExists, User::Leave(err));
            forceStart = ETrue;
            }
        CDictionaryFileStore* fileStore = CreateFileStoreLC(fs, KMonitorSettingsStream);
        TUid activationEnabledSettingUid = {KActivationEnabledSetting};
        if (!fileStore->IsPresentL(activationEnabledSettingUid))
            {
            // file is corrupted in some sense
            forceStart = ETrue;
            }
        if (forceStart)
            {
            WriteActivationEnabledSettingL(*fileStore, ETrue);
            }
        else
            {
            RDictionaryReadStream in;
            in.OpenLC(*fileStore, activationEnabledSettingUid);
            forceStart = in.ReadInt32L();
            CleanupStack::PopAndDestroy(&in);
            }
        CleanupStack::PopAndDestroy(); // fileStore
        if (forceStart)
            {
            CActiveScheduler* scheduler = new (ELeave) CActiveScheduler();
            CleanupStack::PushL(scheduler);
            CActiveScheduler::Install(scheduler);
            CKeypadBuddyServer* server = CKeypadBuddyServer::NewL(fs);
            RProcess me;
            me.Rendezvous(KErrNone);
            CActiveScheduler::Start();
            delete server;
            CleanupStack::PopAndDestroy(scheduler);
            }
        CleanupStack::PopAndDestroy(&fs);
        }
    }

TInt E32Main()
    {
    TInt result;
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if ( cleanup )
        {
        TRAP(result, CKeypadBuddyServer::RunServerL());
        }
    else
        {
        result = KErrNoMemory;
        }
    delete cleanup;
    return result;
    }
