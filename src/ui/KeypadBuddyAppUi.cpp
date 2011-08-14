/*
 ============================================================================
 Name		: KeypadBuddyAppUi.cpp
 Author	  :
 Copyright   : Your copyright notice
 Description : CKeypadBuddyAppUi implementation
 ============================================================================
 */

// INCLUDE FILES
#include <avkon.hrh>
#include <stringloader.h>
#include <f32file.h>
#include <s32file.h>
#include <hlplch.h>

#include <keypadbuddy.rsg>

#include "KeypadBuddy.hrh"
#include "KeypadBuddy.pan"
#include "KeypadBuddyApplication.h"
#include "KeypadBuddyAppUi.h"
#include "KeypadBuddyAppView.h"

// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CKeypadBuddyAppUi::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CKeypadBuddyAppUi::ConstructL()
    {
    // Initialise app UI with standard value.
    BaseConstructL(CAknAppUi::EAknEnableSkin|CAknAppUi::EAppOrientationPortrait);

    iEngine = CKeypadBuddyEngine::NewL(*this);
    // Create view object
    iAppView = CKeypadBuddyAppView::NewL(ClientRect());

    }
// -----------------------------------------------------------------------------
// CKeypadBuddyAppUi::CKeypadBuddyAppUi()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CKeypadBuddyAppUi::CKeypadBuddyAppUi()
    {
    // No implementation required
    }

// -----------------------------------------------------------------------------
// CKeypadBuddyAppUi::~CKeypadBuddyAppUi()
// Destructor.
// -----------------------------------------------------------------------------
//
CKeypadBuddyAppUi::~CKeypadBuddyAppUi()
    {
    delete iAppView;
    delete iEngine;
    }

// -----------------------------------------------------------------------------
// CKeypadBuddyAppUi::HandleCommandL()
// Takes care of command handling.
// -----------------------------------------------------------------------------
//
void CKeypadBuddyAppUi::HandleCommandL(TInt aCommand)
    {
    switch (aCommand)
        {
        case EEikCmdExit:
        case EAknSoftkeyExit:
        case EAknSoftkeyBack:
            {
            Exit();
            break;
            }
        case ECommandActivateMonitor:
            {
            iEngine->SetMonitorActiveL(ETrue);
            break;
            }
        case ECommandDeactivateMonitor:
            {
            iEngine->SetMonitorActiveL(EFalse);
            // icon is updated from the callback
            break;
            }
        case ECommandResetCache:
            {
            iEngine->ResetCacheL();
            break;
            }
        default:
            Panic(EKeypadBuddyUi);
            break;
        }
    }

void CKeypadBuddyAppUi::MonitorDeactivated()
    {
    TRAPD(err, iAppView->SetMonitorActiveL(EFalse));
    if (KErrNone != err)
        {
        Exit();
        }
    }

void CKeypadBuddyAppUi::MonitorActivated()
    {
    TRAPD(err, iAppView->SetMonitorActiveL(ETrue));
    if (KErrNone != err)
        {
        Exit();
        }
    }

// -----------------------------------------------------------------------------
//  Called by the framework when the application status pane
//  size is changed.  Passes the new client rectangle to the
//  AppView
// -----------------------------------------------------------------------------
//
void CKeypadBuddyAppUi::HandleStatusPaneSizeChange()
    {
    iAppView->SetRect(ClientRect());
    }

void CKeypadBuddyAppUi::DynInitMenuPaneL(TInt aResourceId, CEikMenuPane* aMenuPane)
    {
    if (R_MENU == aResourceId)
        {
        TBool monitorActive = iEngine->MonitorActive();
        aMenuPane->SetItemDimmed(ECommandActivateMonitor, monitorActive);
        aMenuPane->SetItemDimmed(ECommandDeactivateMonitor, !monitorActive);
        aMenuPane->SetItemDimmed(ECommandResetCache, !monitorActive);
        }
    }

// End of File
