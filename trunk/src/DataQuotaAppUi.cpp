/*
Data Quota for S60 phones.
http://code.google.com/p/dataquota/
Copyright (C) 2008, 2009, 2011  Hugo van Kemenade

This file is part of Data Quota.

Data Quota is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

Data Quota is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Data Quota.  If not, see <http://www.gnu.org/licenses/>.
*/

// INCLUDE FILES
#include <AknMessageQueryDialog.h>
#include <BrowserLauncher.h>
#include <DataQuota.rsg>

#include "DataQuota.hrh"
#include "DataQuotaAppUi.h"
#include "DataQuotaAppView.h"

void CDataQuotaAppUi::ConstructL()
	{
	// Initialise app UI with standard value
	BaseConstructL(CAknAppUi::EAknEnableSkin);
	
	// Create view object
	iAppView = CDataQuotaAppView::NewL(ClientRect());
	AddToStackL(iAppView);
	
	// Change the Exit softkey to Refresh
	HBufC* refreshText(CCoeEnv::Static()->AllocReadResourceLC(R_DATAQUOTA_REFRESH));
	TInt pos(Cba()->PositionById(EAknSoftkeyExit));
	Cba()->RemoveCommandFromStack(pos, EAknSoftkeyExit);
	Cba()->SetCommandL(pos, EDataQuotaRefresh, *refreshText);
	CleanupStack::PopAndDestroy(refreshText);
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
	delete iBrowserLauncher;
	}


void CDataQuotaAppUi::HandleCommandL(TInt aCommand)
	{
	switch (aCommand)
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
			const TInt KMaxQuota(999999);
			TInt number(iAppView->DataQuota());
			CAknNumberQueryDialog* dlg(CAknNumberQueryDialog::NewL(number));
			dlg->PrepareLC(R_AVKON_DIALOG_QUERY_VALUE_NUMBER);
			dlg->SetMinimumAndMaximum(1, KMaxQuota);
			if (dlg->RunLD())
				{
				iAppView->SetDataQuotaL(number);
				HandleCommandL(EDataQuotaRefresh);
				}
			}
			break;

		case EDataQuotaEditBillingDay:
			{
			const TInt KMinBillingDay(1);
			const TInt KMaxBillingDay(31);
			TInt number(iAppView->BillingDay() + 1);
			CAknNumberQueryDialog* dlg(CAknNumberQueryDialog::NewL(number));
			dlg->PrepareLC(R_AVKON_DIALOG_QUERY_VALUE_NUMBER);
			dlg->SetMinimumAndMaximum(KMinBillingDay, KMaxBillingDay);
			if (dlg->RunLD())
					{
					iAppView->SetBillingDayL(number - 1);
					HandleCommandL(EDataQuotaRefresh);
					}
			}
			break;

/*		case EDataQuotaEditBillingPeriod:
			{
			TInt number(iAppView->BillingDay() + 1);
			TBool numberValid(EFalse);
			while (!numberValid)
				{
				CAknNumberQueryDialog* dlg(CAknNumberQueryDialog::NewL(number));
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
		case EDataQuotaHelp:
			{
			// Create the header text
			HBufC* title(iEikonEnv->AllocReadResourceLC(R_DATAQUOTA_HELP));
			HBufC* help(iEikonEnv->AllocReadResourceLC(R_DATAQUOTA_HELP_TEXT));
			
			CAknMessageQueryDialog* dlg(new(ELeave) CAknMessageQueryDialog());
			
			// Initialise the dialog
			dlg->PrepareLC(R_DATAQUOTA_ABOUT_BOX);
			dlg->QueryHeading()->SetTextL(*title);
			dlg->SetMessageTextL(*help);
			
			dlg->RunLD();
			
			CleanupStack::PopAndDestroy(2, title); // title, help
			}
			break;

		case EDataQuotaAbout:
			{
			// Create the header text
			HBufC* title1(iEikonEnv->AllocReadResourceLC(R_DATAQUOTA_ABOUT_TEXT));
			HBufC* title2(KVersion().AllocLC());
			
			HBufC* title(HBufC::NewLC(title1->Length() + title2->Length()));
			title->Des().Append(*title1);
			title->Des().Append(*title2);
			
			CAknMessageQueryDialog* dlg(new(ELeave) CAknMessageQueryDialog());
			
			// Initialise the dialog
			dlg->PrepareLC(R_DATAQUOTA_ABOUT_BOX);
			dlg->QueryHeading()->SetTextL(*title);
			_LIT(KMessage, "(c) 2008-2011 Hugo van Kemenade\ncode.google.com/p/dataquota\ntwitter.com/DataQuota");
			dlg->SetMessageTextL(KMessage);
			
			dlg->RunLD();
			
			CleanupStack::PopAndDestroy(3, title1); // title1, title2, title
			}
			break;

		case EDataQuotaMoreAppsPodOClock:	// intentional fall-through
		case EDataQuotaMoreAppsMobbler:		// intentional fall-through
		case EDataQuotaMoreAppsSugarSync:	// intentional fall-through
			{
			TBuf<256> url;
			switch (aCommand)
				{
				case EDataQuotaMoreAppsPodOClock:
					{
					_LIT(KUrl, "http://code.google.com/p/podoclock/");
					url.Copy(KUrl);
					}
					break;
				case EDataQuotaMoreAppsMobbler:
					{
					_LIT(KUrl, "http://code.google.com/p/mobbler/");
					url.Copy(KUrl);
					}
					break;
				case EDataQuotaMoreAppsSugarSync:
					{
					_LIT(KUrl, "https://www.sugarsync.com/referral?rf=eoovtb627jrd7");
					url.Copy(KUrl);
					}
					break;
				default:
					break;
				}
			
			if (!iBrowserLauncher)
				{
				iBrowserLauncher = CBrowserLauncher::NewL();
				}
			iBrowserLauncher->LaunchBrowserEmbeddedL(url);
			}
			break;

		default:
			break;
		}
	}


void CDataQuotaAppUi::HandleResourceChangeL(TInt aType)
	{
	// Also call the base class
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
		HandleCommandL(EDataQuotaRefresh);
		}
	}

// End of file
