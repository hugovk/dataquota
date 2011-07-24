/*
Data Quota for Symbian phones.
http://code.google.com/p/dataquota/
Copyright (C) 2008, 2009, 2010, 2011  Hugo van Kemenade

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

#ifndef __DATAQUOTAVIEW_H__
#define __DATAQUOTAVIEW_H__

// INCLUDES
#include <AknView.h>

// FORWARD DECLARATIONS
#ifndef __OVI_SIGNED__
class CBrowserLauncher;
#endif
class CDataQuotaContainer;

// CONSTANTS
const TUid KViewId = {1}; // UID of view

// CLASS DECLARATION
class CDataQuotaView : public CAknView
	{
	public: // Constructors and destructor
		CDataQuotaView();
		static CDataQuotaView* NewL();
		virtual ~CDataQuotaView();
		void ConstructL();

	public: // New functions
		TBool IsDailyQuotaType();

	public: // From CAknView
		TUid Id() const;
		void HandleCommandL(TInt aCommand);
		void HandleViewRectChange();

	private: // New functions
		void OpenWebBrowserL(const TDesC& aUrl);
#ifdef __OVI_SIGNED__
		void UninstallSelfSignedVersionL();
#else
		void LaunchOviSignedVersionL();
#endif
	
	private: // From CAknView
		void DoActivateL(const TVwsViewId& aPrevViewId,
						 TUid aCustomMessageId,
						 const TDesC8& aCustomMessage);
		void DoDeactivate();

	private: // Data
		CDataQuotaContainer* iContainer;
#ifdef __OVI_SIGNED__
		TBool iUninstallAttempted;
#else
		CBrowserLauncher* iBrowserLauncher;
#endif

	};

#endif  //__DATAQUOTAVIEW_H__

// End of file
