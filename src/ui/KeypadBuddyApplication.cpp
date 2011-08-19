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

// ============================ MEMBER FUNCTIONS ===============================

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

// End of File
