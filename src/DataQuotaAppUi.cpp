/*
DataQuotaAppUi.cpp

Data Quota for S60 phones.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

// INCLUDE FILES
#include <AknMessageQueryDialog.h>
#include <AknNoteWrappers.h>
#include <avkon.hrh>
#include <DataQuota.rsg>

#include "DataQuota.hrh"
#include "DataQuotaAppUi.h"
#include "DataQuotaAppView.h"

_LIT(KVersion, "0.1.9");
const TInt KMaxQuota(999999);

void CDataQuotaAppUi::ConstructL()
	{
	// Initialise app UI with standard value
	BaseConstructL(CAknAppUi::EAknEnableSkin);
	
	// Create view object
	iAppView = CDataQuotaAppView::NewL(ClientRect());
	AddToStackL(iAppView);
	
	// Change the Exit softkey to Refresh
	HBufC* hideText(CCoeEnv::Static()->AllocReadResourceLC(R_DATAQUOTA_REFRESH));
	TInt pos(Cba()->PositionById(EAknSoftkeyExit));
	Cba()->RemoveCommandFromStack(pos, EAknSoftkeyExit);
	Cba()->SetCommandL(pos, EDataQuotaRefresh, *hideText);
	CleanupStack::PopAndDestroy(hideText);
	}


CDataQuotaAppUi::CDataQuotaAppUi()
	{
	// No implementation required
	}


CDataQuotaAppUi::~CDataQuotaAppUi()
	{
	if (iAppView)
		{
		RemoveFromStack(iAppView);
		delete iAppView;
		iAppView = NULL;
		}
	}


void CDataQuotaAppUi::HandleCommandL(TInt aCommand)
	{
	switch(aCommand)
		{
		case EEikCmdExit:
		case EAknSoftkeyExit:
			Exit();
			break;

		case EDataQuotaRefresh:
			{
			iAppView->UpdateValuesL();
			iAppView->DrawNow();
			}
			break;

		case EDataQuotaEditQuota:
			{
			TInt number(iAppView->DataQuota());
			CAknNumberQueryDialog* dlg(CAknNumberQueryDialog::NewL(number));
			dlg->PrepareLC(R_AVKON_DIALOG_QUERY_VALUE_NUMBER);
			dlg->SetMinimumAndMaximum(1, KMaxQuota);
			if (dlg->RunLD())
				{
				iAppView->SetDataQuota(number);
				}
			}
			break;

		case EDataQuotaEditBillingDay:
			{
			TInt number(iAppView->BillingDay() + 1);
			CAknNumberQueryDialog* dlg(CAknNumberQueryDialog::NewL(number));
			dlg->PrepareLC(R_AVKON_DIALOG_QUERY_VALUE_NUMBER);
			dlg->SetMinimumAndMaximum(1, 31);
			if (dlg->RunLD())
					{
					iAppView->SetBillingDay(number - 1);
					}
			}
			break;

/*		case EDataQuotaEditBillingPeriod:
			{
			TInt number(iAppView->BillingDay() + 1);
			TBool numberValid(EFalse);
			while (!numberValid)
				{
				CAknNumberQueryDialog* dlg = CAknNumberQueryDialog::NewL(number);
				dlg->PrepareLC(R_AVKON_DIALOG_QUERY_VALUE_NUMBER);
				if (dlg->RunLD())
					{
					if (number > 0 && number < 32)
						{
						iAppView->SetBillingDay(number - 1);
						numberValid = ETrue;
						}
					}
				else
					{
					numberValid = ETrue;
					}
				}
			}
			break;
*/
		case EDataQuotaAbout:
			{
			// create the header text
			HBufC* title1(iEikonEnv->AllocReadResourceLC(R_DATAQUOTA_ABOUT_TEXT));
			HBufC* title2(KVersion().AllocLC());
			
			HBufC* title(HBufC::NewLC(title1->Length() + title2->Length()));
			title->Des().Append(*title1);
			title->Des().Append(*title2);
			
			CAknMessageQueryDialog* dlg(new(ELeave) CAknMessageQueryDialog());
			
			// initialise the dialog
			dlg->PrepareLC(R_DATAQUOTA_ABOUT_BOX);
			dlg->QueryHeading()->SetTextL(*title);
			dlg->SetMessageTextL(_L("http://code.google.com/p/dataquota"));
			
			dlg->RunLD();
			
			CleanupStack::PopAndDestroy(3); // title, title1, title2
			}
			break;

		default:
			break;
		}
	}


void CDataQuotaAppUi::HandleResourceChangeL(TInt aType)
	{
	// base-class call also
	CAknAppUi::HandleResourceChangeL(aType);
	if (aType == KEikDynamicLayoutVariantSwitch)
		{
		if (iAppView)
			{
			iAppView->SetRect(ClientRect());
			}
		}
	}


void CDataQuotaAppUi::HandleForegroundEventL(TBool aForeground)
	{
	CAknAppUi::HandleForegroundEventL(aForeground);
	if (aForeground)
		{
		iAppView->UpdateValuesL();
		iAppView->DrawNow();
		}
	}

// End of file
