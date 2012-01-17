/*
Data Quota for Symbian phones.
http://code.google.com/p/dataquota/
Copyright (C) 2008, 2009, 2010, 2011, 2012  Hugo van Kemenade

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

// OWN INCLUDE
#include "DataQuotaView.h"

// SYSTEM INCLUDES
#include <AknMessageQueryDialog.h>
#include <AknViewAppUi.h>
#include <apgcli.h>
#ifdef __OVI_SIGNED__
#include <SWInstApi.h>
#include <SWInstDefs.h>
#else
#include <BrowserLauncher.h>
#endif

// USER INCLUDES
#include "DataQuotaContainer.h"
#include "DataQuota.hrh"
#include "DataQuota.rsg.h"
#include "DataQuotaUids.h"


CDataQuotaView::CDataQuotaView()
	: iContainer(NULL)
#ifdef __OVI_SIGNED__
	, iUninstallAttempted(EFalse)
#endif
	{
	// No implementation required
	}


CDataQuotaView::~CDataQuotaView()
	{
#ifndef __OVI_SIGNED__
	delete iBrowserLauncher;
#endif
	delete iContainer;
	}


CDataQuotaView* CDataQuotaView::NewL()
	{
	CDataQuotaView* self(new(ELeave) CDataQuotaView);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}


void CDataQuotaView::ConstructL()
	{
	BaseConstructL(R_DATAQUOTA_VIEW);
#ifndef __OVI_SIGNED__
	LaunchOviSignedVersionL();
#endif
	}


TUid CDataQuotaView::Id() const
	{
	return KViewId;
	}


void CDataQuotaView::HandleCommandL(TInt aCommand)
	{
	switch (aCommand)
		{
		// case EAknSoftkeyBack:
			// AppUi()->ProcessCommandL(EAknCmdExit);
			// break;

		case EAknCmdExit:
			{
			TApaTask task(iEikonEnv->WsSession());
			task.SetWgId(CEikonEnv::Static()->RootWin().Identifier());
			task.SendToBackground();
			AppUi()->HandleCommandL(EAknCmdExit);
			}
			break;

		case EDataQuotaRefresh:
			{
			iContainer->UpdateValuesL();
			iContainer->DrawNow();
#ifdef __OVI_SIGNED__
			// UninstallSelfSignedVersionL();
#endif
			}
			break;

		case EDataQuotaEditQuota:
			{
			const TInt KMaxQuota(999999);
			TInt number(iContainer->DataQuota());
			CAknNumberQueryDialog* dlg(CAknNumberQueryDialog::NewL(number));
			dlg->PrepareLC(R_AVKON_DIALOG_QUERY_VALUE_NUMBER);
			dlg->SetMinimumAndMaximum(1, KMaxQuota);
			if (dlg->RunLD())
				{
				iContainer->SetDataQuotaL(number);
				HandleCommandL(EDataQuotaRefresh);
				}
			}
			break;

		case EDataQuotaEditBillingDay:
			{
			const TInt KMinBillingDay(1);
			const TInt KMaxBillingDay(31);
			TInt number(iContainer->BillingDay() + 1);
			CAknNumberQueryDialog* dlg(CAknNumberQueryDialog::NewL(number));
			dlg->PrepareLC(R_AVKON_DIALOG_QUERY_VALUE_NUMBER);
			dlg->SetMinimumAndMaximum(KMinBillingDay, KMaxBillingDay);
			if (dlg->RunLD())
				{
				iContainer->SetBillingDayL(number - 1);
				HandleCommandL(EDataQuotaRefresh);
				}
			}
			break;

		case EDataQuotaEditDailyQuota:
			iContainer->SetQuotaTypeL(CDataQuotaContainer::EDaily);
			HandleCommandL(EDataQuotaRefresh);
			break;

		case EDataQuotaEditMonthlyQuota:
			iContainer->SetQuotaTypeL(CDataQuotaContainer::EMonthly);
			HandleCommandL(EDataQuotaRefresh);
			break;

/*		case EDataQuotaEditBillingPeriod:
			{
			TInt number(iContainer->BillingDay() + 1);
			TBool numberValid(EFalse);
			while (!numberValid)
				{
				CAknNumberQueryDialog* dlg(CAknNumberQueryDialog::NewL(number));
				dlg->PrepareLC(R_AVKON_DIALOG_QUERY_VALUE_NUMBER);
				if (dlg->RunLD())
					{
					if (number > 0 && number < 32)
						{
						iContainer->SetBillingDay(number - 1);
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
		case EDataQuotaResetQuota:
			{
			CAknQueryDialog* dlg(CAknQueryDialog::NewL());
			HBufC* text(iEikonEnv->AllocReadResourceLC(
				R_DATAQUOTA_RESET_QUOTA_CONFIRMATION));
			if (dlg->ExecuteLD(R_DATAQUOTA_YES_NO_QUERY_DIALOG, *text))
				{
				iContainer->ResetQuota();
				HandleCommandL(EDataQuotaRefresh);
				}
			CleanupStack::PopAndDestroy(text);
			}
			break;

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
			_LIT(KMessage, "(C) 2008-2012 Hugo van Kemenade\ncode.google.com/p/dataquota\ntwitter.com/DataQuota");
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
			TBool forceNativeBrowser(EFalse);
			switch (aCommand)
				{
				case EDataQuotaMoreAppsPodOClock:
					{
#ifdef __OVI_SIGNED__
					_LIT(KUrl, "http://store.ovi.mobi/content/180798/");
					forceNativeBrowser = ETrue;
#else
					_LIT(KUrl, "http://code.google.com/p/podoclock/");
#endif
					url.Copy(KUrl);
					}
					break;
				case EDataQuotaMoreAppsMobbler:
					{
#ifdef __OVI_SIGNED__
					_LIT(KUrl, "http://store.ovi.mobi/content/75692");
					forceNativeBrowser = ETrue;
#else
					_LIT(KUrl, "http://code.google.com/p/mobbler/");
#endif
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
			
			OpenWebBrowserL(url, forceNativeBrowser);
			}
			break;
		
		default:
			break;
		}
	}


void CDataQuotaView::HandleViewRectChange()
	{
	if (iContainer)
		{
		iContainer->SetRect(ClientRect());
		}
	}


void CDataQuotaView::OpenWebBrowserL(const TDesC& aUrl,
									 const TBool aForceNative)
	{
	// To suppress compiler warnings
	(void)aUrl;
	(void)aForceNative;

#ifdef __OVI_SIGNED__
	RApaLsSession lsSession;
	User::LeaveIfError(lsSession.Connect());
	CleanupClosePushL(lsSession); 

	const TUid KBrowserUid = {0x10008D39};
	TUid handlerUid = KBrowserUid;

	if (!aForceNative)
		{
		// Find the default browser, on S^1/S^3 it may be a 3rd party browser
		_LIT8(KMimeDataType, "application/x-web-browse");
		TDataType mimeDataType(KMimeDataType);
		// Get the default application UID for "application/x-web-browse"
		lsSession.AppForDataType(mimeDataType, handlerUid);
	
		if (handlerUid.iUid == 0)
			{
			// For S60 3.x
			handlerUid = KBrowserUid;
			}
		}
	
	TApaTaskList taskList(CEikonEnv::Static()->WsSession());
	TApaTask task(taskList.FindApp(handlerUid));
	if(task.Exists())
		{
		task.BringToForeground();
		HBufC8* param8(HBufC8::NewLC(aUrl.Length()));
		param8->Des().Append(aUrl);
		task.SendMessage(TUid::Uid(0), *param8); // UID not used
		CleanupStack::PopAndDestroy(param8);
		}
	else
		{
		TThreadId thread;
		User::LeaveIfError(lsSession.StartDocument(aUrl, handlerUid, thread));
		}
	CleanupStack::PopAndDestroy(&lsSession);

#else // !__OVI_SIGNED__

	if (!iBrowserLauncher)
		{
		iBrowserLauncher = CBrowserLauncher::NewL();
		}
	iBrowserLauncher->LaunchBrowserEmbeddedL(aUrl);
#endif // __OVI_SIGNED__
	}


	void CDataQuotaView::DoActivateL(const TVwsViewId& /*aPrevViewId*/,
								 TUid /*aCustomMessageId*/,
								 const TDesC8& /*aCustomMessage*/)
	{
	// Create container object
	if (!iContainer)
		{
		iContainer = new (ELeave) CDataQuotaContainer(this);
		iContainer->ConstructL(ClientRect());
		iContainer->SetMopParent(this);
		
		HBufC* refreshText(
			CCoeEnv::Static()->AllocReadResourceLC(R_DATAQUOTA_REFRESH));
		TInt pos(Cba()->PositionById(EAknSoftkeyExit));
		Cba()->RemoveCommandFromStack(pos, EAknSoftkeyExit);
		Cba()->SetCommandL(pos, EDataQuotaRefresh, *refreshText);
		CleanupStack::PopAndDestroy(refreshText);
		}

	// Add container to view control stack
	AppUi()->AddToStackL(*this, iContainer);
	iContainer->MakeVisible(ETrue);
	}


	void CDataQuotaView::DoDeactivate()
	{
	// Delete the container class object
	if (iContainer)
		{
		iContainer->MakeVisible(EFalse);
		// Remove container from view control stack
		AppUi()->RemoveFromStack(iContainer);
		}
	}


TBool CDataQuotaView::IsDailyQuotaType()
	{
	return iContainer->IsDailyQuotaType();
	}


#ifdef __OVI_SIGNED__
void CDataQuotaView::UninstallSelfSignedVersionL()
	{
	if (iUninstallAttempted)
		{
		return;
		}
	else
		{
		iUninstallAttempted = ETrue;
		}
	
	SwiUI::RSWInstLauncher launcher; 
	TInt error(launcher.Connect());
	 if (KErrNone == error)
		{
		SwiUI::TInstallOptions options;
		SwiUI::TInstallOptionsPckg optionsPckg;  
		options.iKillApp = SwiUI::EPolicyAllowed;
		optionsPckg = options;  
		error = launcher.SilentUninstall(
			TUid::Uid(KUidSelfSigned), 
			optionsPckg,
			SwiUI::KSisxMimeType);
		}
	launcher.Close();
	}
#endif // __OVI_SIGNED__


#ifndef __OVI_SIGNED__
void CDataQuotaView::LaunchOviSignedVersionL()
	{
	RApaLsSession lsSession;
	User::LeaveIfError(lsSession.Connect());
	CleanupClosePushL(lsSession);

	TApaAppInfo appInfo;
	TInt error(lsSession.GetAppInfo(
		appInfo, 
		TUid::Uid(KUidOviSigned)));

	if (KErrNone == error)
		{
		CApaCommandLine* cmdLine(CApaCommandLine::NewLC());
		cmdLine->SetExecutableNameL(appInfo.iFullName);
		cmdLine->SetCommandL(EApaCommandRun);
		User::LeaveIfError(lsSession.StartApp(*cmdLine));
		CleanupStack::PopAndDestroy(cmdLine);
		}
	
	CleanupStack::PopAndDestroy(&lsSession);
	
	if (KErrNone == error)
		{
		HandleCommandL(EEikCmdExit);
		}
	}
#endif // !__OVI_SIGNED__


// End of file
