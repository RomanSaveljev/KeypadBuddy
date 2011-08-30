/*
 ============================================================================
 Name		: KeypadBuddyDocument.cpp
 Author	  :
 Copyright   : Your copyright notice
 Description : CKeypadBuddyDocument implementation
 ============================================================================
 */

// INCLUDE FILES
#include "KeypadBuddyAppUi.h"
#include "KeypadBuddyDocument.h"
#include <eikproc.h>
#include "MonitorDefinitions.h"
#include "KeypadBuddyApplication.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CKeypadBuddyDocument::NewL()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CKeypadBuddyDocument* CKeypadBuddyDocument::NewL(CEikApplication& aApp)
    {
    CKeypadBuddyDocument* self = NewLC(aApp);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CKeypadBuddyDocument::NewLC()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CKeypadBuddyDocument* CKeypadBuddyDocument::NewLC(CEikApplication& aApp)
    {
    CKeypadBuddyDocument* self = new (ELeave) CKeypadBuddyDocument(aApp);

    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// CKeypadBuddyDocument::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CKeypadBuddyDocument::ConstructL()
    {
    // No implementation required
    }

// -----------------------------------------------------------------------------
// CKeypadBuddyDocument::CKeypadBuddyDocument()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CKeypadBuddyDocument::CKeypadBuddyDocument(CEikApplication& aApp) :
    CAknDocument(aApp)
    {
    // No implementation required
    }

// ---------------------------------------------------------------------------
// CKeypadBuddyDocument::~CKeypadBuddyDocument()
// Destructor.
// ---------------------------------------------------------------------------
//
CKeypadBuddyDocument::~CKeypadBuddyDocument()
    {
    // No implementation required
    }

// ---------------------------------------------------------------------------
// CKeypadBuddyDocument::CreateAppUiL()
// Constructs CreateAppUi.
// ---------------------------------------------------------------------------
//
CEikAppUi* CKeypadBuddyDocument::CreateAppUiL()
    {
    return new (ELeave) CKeypadBuddyAppUi();
    }

// End of File
