/*
 ============================================================================
 Name		: KeypadBuddyAppView.cpp
 Author	  :
 Copyright   : Your copyright notice
 Description : Application view implementation
 ============================================================================
 */

// INCLUDE FILES
#include <coemain.h>
#include "KeypadBuddyAppView.h"
#include <KeypadBuddy.mbg>
#include <AknIconUtils.h>
#include <aknsbasicbackgroundcontrolcontext.h>
#include <aknsskininstance.h>
#include <aknsutils.h>
#include <aknsdrawutils.h>
#include "KeypadBuddy.hrh"
#include "KeypadBuddyAppUi.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CKeypadBuddyAppView::NewL()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CKeypadBuddyAppView* CKeypadBuddyAppView::NewL(const TRect& aRect)
    {
    CKeypadBuddyAppView* self = CKeypadBuddyAppView::NewLC(aRect);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CKeypadBuddyAppView::NewLC()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CKeypadBuddyAppView* CKeypadBuddyAppView::NewLC(const TRect& aRect)
    {
    CKeypadBuddyAppView* self = new (ELeave) CKeypadBuddyAppView();
    CleanupStack::PushL(self);
    self->ConstructL(aRect);
    return self;
    }

// -----------------------------------------------------------------------------
// CKeypadBuddyAppView::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CKeypadBuddyAppView::ConstructL(const TRect& aRect)
    {
    // Create a window for this application view
    CreateWindowL();

    iBgContext = CAknsBasicBackgroundControlContext::NewL(/*KAknsIIDQsnBgScreen*/KAknsIIDQsnBgAreaMain, aRect, ETrue);

    iImage = new( ELeave ) CEikImage();
    iImage->SetPictureOwnedExternally(ETrue);
    iImage->SetContainerWindowL( *this );
    iImage->SetAlignment(EHCenterVCenter);

    // Set the windows size
    SetRect(aRect);

    // Activate the window, which makes it ready to be drawn
    ActivateL();
    }

// -----------------------------------------------------------------------------
// CKeypadBuddyAppView::CKeypadBuddyAppView()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CKeypadBuddyAppView::CKeypadBuddyAppView()
    {
    // No implementation required
    }

// -----------------------------------------------------------------------------
// CKeypadBuddyAppView::~CKeypadBuddyAppView()
// Destructor.
// -----------------------------------------------------------------------------
//
CKeypadBuddyAppView::~CKeypadBuddyAppView()
    {
    delete iImage;
    delete iBitmap;
    delete iMask;
    delete iBgContext;
    }

// -----------------------------------------------------------------------------
// CKeypadBuddyAppView::Draw()
// Draws the display.
// -----------------------------------------------------------------------------
//
void CKeypadBuddyAppView::Draw(const TRect& aRect) const
    {
    // Get the standard graphics context
    CWindowGc& gc = SystemGc();
    MAknsSkinInstance* skin = AknsUtils::SkinInstance();
    MAknsControlContext* cc = AknsDrawUtils::ControlContext( this );
    AknsDrawUtils::Background( skin, cc, this, gc, aRect );
    }

// -----------------------------------------------------------------------------
// CKeypadBuddyAppView::SizeChanged()
// Called by framework when the view size is changed.
// -----------------------------------------------------------------------------
//
void CKeypadBuddyAppView::SizeChanged()
    {
    if (iBgContext)
        {
        iBgContext->SetRect(Rect());
        if (&Window())
            {
            iBgContext->SetParentPos(PositionRelativeToScreen());
            }
        }
    TRAP_IGNORE(SizeChangedL(iMonitorActive));
    }

void CKeypadBuddyAppView::SizeChangedL(TBool aMonitorActive)
    {
    delete iBitmap;
    iBitmap = NULL;
    delete iMask;
    iMask = NULL;
    TMbmKeypadbuddy iconId = aMonitorActive ? EMbmKeypadbuddyButton_active : EMbmKeypadbuddyButton_inactive;
    AknIconUtils::CreateIconL(iBitmap, iMask, _L("\\resource\\apps\\KeypadBuddy.mbm"), iconId, EMbmKeypadbuddyButton_mask);
    User::LeaveIfError(AknIconUtils::SetSize(iBitmap, Rect().Size()));
    iImage->SetPicture(iBitmap, iMask);
    iImage->SetExtent(TPoint(0, 0), iBitmap->SizeInPixels());
    iImage->DrawDeferred();
    }

void CKeypadBuddyAppView::SetMonitorActiveL(TBool aActive)
    {
    SizeChangedL(aActive);
    iMonitorActive = aActive;
    }

// -----------------------------------------------------------------------------
// CKeypadBuddyAppView::HandlePointerEventL()
// Called by framework to handle pointer touch events.
// Note: although this method is compatible with earlier SDKs,
// it will not be called in SDKs without Touch support.
// -----------------------------------------------------------------------------
//
void CKeypadBuddyAppView::HandlePointerEventL(
        const TPointerEvent& aPointerEvent)
    {
    if (aPointerEvent.iType == TPointerEvent::EButton1Up)
        {
        if (iImage)
            {
            TPoint position = aPointerEvent.iPosition;
            TRect imgRect = iImage->Rect();
            imgRect.Move(iImage->Position().iX, iImage->Position().iY * 1.2);
            imgRect.Grow(-imgRect.Width() * 0.2, -imgRect.Height() * 0.2);
            if (imgRect.Contains(aPointerEvent.iPosition))
                {
                CKeypadBuddyAppUi* appUi = static_cast<CKeypadBuddyAppUi*>(CEikonEnv::Static()->AppUi());
                if (iMonitorActive)
                    {
                    appUi->HandleCommandL(ECommandDeactivateMonitor);
                    }
                else
                    {
                    appUi->HandleCommandL(ECommandActivateMonitor);
                    }
                }
            }
        }
    // Call base class HandlePointerEventL()
    CCoeControl::HandlePointerEventL(aPointerEvent);
    }

TInt CKeypadBuddyAppView::CountComponentControls() const
    {
    return 1;
    }

CCoeControl* CKeypadBuddyAppView::ComponentControl(TInt aIndex) const
    {
    switch (aIndex)
        {
        case 0:
            return iImage;
        default:
            return NULL;
        }
    }

TTypeUid::Ptr CKeypadBuddyAppView::MopSupplyObject(TTypeUid aId)
    {
    if (iBgContext)
        {
        return MAknsControlContext::SupplyMopObject(aId, iBgContext);
        }
    return CCoeControl::MopSupplyObject(aId);
    }

// End of File
