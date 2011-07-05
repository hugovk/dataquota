/*
Data Quota for Symbian phones.
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

// OWN INCLUDE
#include "DataQuotaAppUi.h"

// SYSTEM INCLUDES
#include <EikMenuP.h>

// USER INCLUDES
#include "DataQuota.hrh"
#include "DataQuota.rsg.h"
#include "DataQuotaView.h"


CDataQuotaAppUi::CDataQuotaAppUi()
	{
	// No implementation required
	}


void CDataQuotaAppUi::ConstructL()
	{
	// Initialise app UI with standard value
	BaseConstructL(EAknEnableSkin);
	
	// Creates CDataQuotaView class object
	iView = CDataQuotaView::NewL();
	
	// Transfer ownership to CAknViewAppUi
	AddViewL(iView); 
	
	ActivateLocalViewL(iView->Id());
	}


CDataQuotaAppUi::~CDataQuotaAppUi()
	{
	// No implementation required
	}


void CDataQuotaAppUi::HandleCommandL(TInt aCommand)
	{
	switch (aCommand)
		{
		case EEikCmdExit:
		case EAknSoftkeyExit:
			Exit();
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
		// CAknView* view(View(KViewId));
		iView->HandleViewRectChange();
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


void CDataQuotaAppUi::DynInitMenuPaneL(
	TInt aResourceId, CEikMenuPane* aMenuPane)
	{
	if (R_DATAQUOTA_EDIT_MENU_PANE == aResourceId)
		{
		TBool dailyQuota(iView->IsDailyQuotaType());
		
		// Hide if daily:
		aMenuPane->SetItemDimmed(EDataQuotaEditDailyQuota, dailyQuota);
		aMenuPane->SetItemDimmed(EDataQuotaEditBillingDay, dailyQuota);
		
		// Hide if monthly:
		aMenuPane->SetItemDimmed(EDataQuotaEditMonthlyQuota, !dailyQuota);
		}
	
	// iView->HandleCommandL(EDataQuotaRefresh);
	}


// End of file
