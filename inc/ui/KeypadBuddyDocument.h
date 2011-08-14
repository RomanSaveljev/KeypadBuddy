/*
 ============================================================================
 Name		: KeypadBuddyDocument.h
 Author	  : 
 Copyright   : Your copyright notice
 Description : Declares document class for application.
 ============================================================================
 */

#ifndef __KEYPADBUDDYDOCUMENT_h__
#define __KEYPADBUDDYDOCUMENT_h__

// INCLUDES
#include <akndoc.h>

// FORWARD DECLARATIONS
class CKeypadBuddyAppUi;
class CEikApplication;

// CLASS DECLARATION

/**
 * CKeypadBuddyDocument application class.
 * An instance of class CKeypadBuddyDocument is the Document part of the
 * AVKON application framework for the KeypadBuddy example application.
 */
class CKeypadBuddyDocument : public CAknDocument
    {
public:
    // Constructors and destructor

    /**
     * NewL.
     * Two-phased constructor.
     * Construct a CKeypadBuddyDocument for the AVKON application aApp
     * using two phase construction, and return a pointer
     * to the created object.
     * @param aApp Application creating this document.
     * @return A pointer to the created instance of CKeypadBuddyDocument.
     */
    static CKeypadBuddyDocument* NewL(CEikApplication& aApp);

    /**
     * NewLC.
     * Two-phased constructor.
     * Construct a CKeypadBuddyDocument for the AVKON application aApp
     * using two phase construction, and return a pointer
     * to the created object.
     * @param aApp Application creating this document.
     * @return A pointer to the created instance of CKeypadBuddyDocument.
     */
    static CKeypadBuddyDocument* NewLC(CEikApplication& aApp);

    /**
     * ~CKeypadBuddyDocument
     * Virtual Destructor.
     */
    virtual ~CKeypadBuddyDocument();

public:
    // Functions from base classes

    /**
     * CreateAppUiL
     * From CEikDocument, CreateAppUiL.
     * Create a CKeypadBuddyAppUi object and return a pointer to it.
     * The object returned is owned by the Uikon framework.
     * @return Pointer to created instance of AppUi.
     */
    CEikAppUi* CreateAppUiL();

private:
    // Constructors

    /**
     * ConstructL
     * 2nd phase constructor.
     */
    void ConstructL();

    /**
     * CKeypadBuddyDocument.
     * C++ default constructor.
     * @param aApp Application creating this document.
     */
    CKeypadBuddyDocument(CEikApplication& aApp);

    };

#endif // __KEYPADBUDDYDOCUMENT_h__
// End of File
