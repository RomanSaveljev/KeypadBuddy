#include "KeypadBuddyAppUiLanguageRestore.h"
#include <eikedwin.h>
#include <eiklabel.h>

CKeypadBuddyAppUiLanguageRestore::~CKeypadBuddyAppUiLanguageRestore()
    {
    delete iAsyncCallBack;
    delete iPeriodic;
    delete iEdWin;
    }

void CKeypadBuddyAppUiLanguageRestore::ConstructL()
    {
    BaseConstructL(CAknAppUi::EAknEnableSkin|ENoAppResourceFile|ENoScreenFurniture);

    /*
    iEdWin = new(ELeave) CMyEdWin;
    iEdWin->ConstructL();
    iEdWin->ActivateL();
    iEdWin->SetFocus(ETrue, EDrawNow);
    iEdWin->SetCursorVisible(ETrue);
    iEdWin->SetRect(ClientRect());
    TCoeInputCapabilities caps = iEdWin->InputCapabilities();
    MCoeFepAwareTextEditor* fepAware = caps.FepAwareTextEditor();
    fepAware->StartFepInlineEditL( KNullDesC(), 0, ETrue, NULL, )
    //TCoeInputCapabilities caps;
    //iEdWin->SetInputCapabilitiesL()
    */

    //iAsyncCallBack = new(ELeave) CAsyncCallBack(CActive::EPriorityIdle);
    //iAsyncCallBack->Set(TCallBack(CallBack, this));
    //iAsyncCallBack->CallBack();
    //User::After(1000000);
    //Exit();

    iPeriodic = CPeriodic::New(CActive::EPriorityStandard);
    iPeriodic->Start(600000, 5000000, TCallBack(CallBack, this));
    }

TInt CKeypadBuddyAppUiLanguageRestore::CallBack(TAny* aSelf)
    {
    // FEP will be loaded by active object. So continue here to make sure fep it
    // was loaded
    CCoeEnv* env = CCoeEnv::Static();
    TUid uid = env->FepUid();
    env->InstallFepL(KNullUid);
    User::After(100000);
    env->InstallFepL(uid);
    //CCoeFep* fep = env->Fep();
    env->InputCapabilitiesChanged();
    //RProcess me;
    //me.Rendezvous(KErrNone);
    static_cast<CKeypadBuddyAppUiLanguageRestore*>(aSelf)->Exit();
    return KErrNone;
    }
