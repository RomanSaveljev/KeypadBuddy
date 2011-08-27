/*
 ============================================================================
 Name		: KeypadBuddyApplication.cpp
 Author	  :
 Copyright   : Your copyright notice
 Description : Main application class
 ============================================================================
 */

// INCLUDE FILES
#include "KeypadBuddy.hrh"
#include "KeypadBuddyDocument.h"
#include "KeypadBuddyApplication.h"
#include "keypadbuddyuids.hrh"
#include "MonitorDefinitions.h"

// ============================ MEMBER FUNCTIONS ===============================

CKeypadBuddyApplication::CKeypadBuddyApplication()
    {
    TBuf8<32> argBuf;
    TInt err = User::GetDesParameter(KArgumentSlot, argBuf);
    iLanguageRestore = err == KErrNone && argBuf == KLanguageRestore;
    }
// -----------------------------------------------------------------------------
// CKeypadBuddyApplication::CreateDocumentL()
// Creates CApaDocument object
// -----------------------------------------------------------------------------
//
CApaDocument* CKeypadBuddyApplication::CreateDocumentL()
    {
    // Create an KeypadBuddy document, and return a pointer to it
    return CKeypadBuddyDocument::NewL(*this);
    }

// -----------------------------------------------------------------------------
// CKeypadBuddyApplication::AppDllUid()
// Returns application UID
// -----------------------------------------------------------------------------
//
TUid CKeypadBuddyApplication::AppDllUid() const
    {
    // Return the UID for the KeypadBuddy application
    TUid uid = {KKeypadBuddyUidValue};
    return uid;
    }

TFileName CKeypadBuddyApplication::ResourceFileName() const
    {
    if (EFalse && iLanguageRestore)
        {
        return TFileName();
        }
    else
        {
        return CAknApplication::ResourceFileName();
        }
    }

// End of File
