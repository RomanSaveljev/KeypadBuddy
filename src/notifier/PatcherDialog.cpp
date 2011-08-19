#include "PatcherDialog.h"
#include "KeypadBuddyUids.hrh"
#include <AknNoteDialog.h>
#include <KeypadBuddy.rsg>
#include <FEPBASE.H>
#include <eiklabel.h>
#include <COEAUI.H>
#include <eikdialg.h>

const TUid KScreenOutputChannel = { 0x10009D48 };

void CPatcherDialog::Release()
    {
    delete this;
    }

MEikSrvNotifierBase2::TNotifierInfo CPatcherDialog::RegisterL()
    {
    return Info();
    }

MEikSrvNotifierBase2::TNotifierInfo CPatcherDialog::Info() const
    {
    MEikSrvNotifierBase2::TNotifierInfo info;
    info.iChannel = KScreenOutputChannel;
    info.iPriority = ENotifierPriorityVLow;
    info.iUid.iUid = KKeypadBuddyNotifierUidValue;
    return info;
    }

TPtrC8 CPatcherDialog::StartL(const TDesC8& /*aBuffer*/)
    {
    CCoeEnv* env = CCoeEnv::Static();
    _LIT( KDrive, "c:" );
    TBuf<2> drive;
    drive = KDrive;
    TFileName fn;
    Dll::FileName( fn );
    TParse parse;
    parse.Set(_L("\\resource\\apps\\KeypadBuddy.rsc"), &fn, &drive);
    fn = parse.FullName();
    TInt offset = env->AddResourceFileL(fn);
    CEikDialog* dialog = new(ELeave) CEikDialog();
    dialog->PrepareLC(R_PATCHER_DIALOG);
    dialog->RunLD();
    User::After(400000);
    delete dialog;
    return TPtrC8();
    }

void CPatcherDialog::StartL(const TDesC8& /*aBuffer*/, TInt /*aReplySlot*/, const RMessagePtr2& /*aMessage*/)
    {
    User::Leave(KErrNotSupported);
    }

void CPatcherDialog::Cancel()
    {
    }

TPtrC8 CPatcherDialog::UpdateL(const TDesC8& /*aBuffer*/)
    {
    User::Leave(KErrNotSupported);
    return TPtrC8();
    }

