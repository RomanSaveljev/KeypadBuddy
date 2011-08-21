#include "KeypadBuddyEmptyView.h"
#include <KeypadBuddy.rsg>
#include <barsread.h>

CKeypadBuddyEmptyView* CKeypadBuddyEmptyView::NewL()
    {
    CKeypadBuddyEmptyView* self = new(ELeave) CKeypadBuddyEmptyView;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

CKeypadBuddyEmptyView::CKeypadBuddyEmptyView()
    {

    }

void CKeypadBuddyEmptyView::ConstructL()
    {
    CreateWindowL();
    TResourceReader reader;
    CCoeEnv::Static()->CreateResourceReaderLC(reader, R_PATCHER_DIALOG);
    ConstructFromResourceL(reader);
    CleanupStack::PopAndDestroy();
    ActivateL();
    }
