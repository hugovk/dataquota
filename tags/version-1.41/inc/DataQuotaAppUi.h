/*
Data Quota for Symbian phones.
http://code.google.com/p/dataquota/
Copyright (C) 2008, 2011  Hugo van Kemenade

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

#ifndef __DATAQUOTAAPPUI_H__
#define __DATAQUOTAAPPUI_H__

// INCLUDES
#include <aknviewappui.h>

// FORWARD DECLARATIONS
class CDataQuotaView;

// CLASS DECLARATION
class CDataQuotaAppUi : public CAknViewAppUi
	{
	public: // Constructors and destructor
		CDataQuotaAppUi();
		void ConstructL();
		virtual ~CDataQuotaAppUi();

	private: // From CEikAppUi
		void HandleCommandL(TInt aCommand);
		void HandleForegroundEventL(TBool aForeground);
		void HandleResourceChangeL(TInt aType);

	private: // From CAknView
		void DynInitMenuPaneL(TInt aResourceId, CEikMenuPane* aMenuPane);

	private: // Data
		CDataQuotaView* iView;
		TBool iHideSugarSync;
	};

#endif // __DATAQUOTAAPPUI_H__

// End of file
