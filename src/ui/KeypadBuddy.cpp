/*
 ============================================================================
 Name		: KeypadBuddy.cpp
 Author	  : 
 Copyright   : Your copyright notice
 Description : Main application class
 ============================================================================
 */

// INCLUDE FILES
#include <eikstart.h>
#include "KeypadBuddyApplication.h"

LOCAL_C CApaApplication* NewApplication()
    {
    return new CKeypadBuddyApplication;
    }

GLDEF_C TInt E32Main()
    {
    return EikStart::RunApplication(NewApplication);
    }

