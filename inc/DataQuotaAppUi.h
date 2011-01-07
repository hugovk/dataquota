/*
Data Quota for S60 phones.
http://code.google.com/p/dataquota/
Copyright (C) 2008, 2010  Hugo van Kemenade

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
#include <aknappui.h>

// FORWARD DECLARATIONS
class CBrowserLauncher;
class CDataQuotaAppView;

// CLASS DECLARATION
class CDataQuotaAppUi : public CAknAppUi
	{
	public: // Constructors and destructor
		void ConstructL();
		CDataQuotaAppUi();
		virtual ~CDataQuotaAppUi();

	private:  // Functions from base classes
		void HandleCommandL(TInt aCommand);
		void HandleForegroundEventL(TBool aForeground);
		void HandleResourceChangeL(TInt aType);

	private: // Data
		CBrowserLauncher* iBrowserLauncher;
		CDataQuotaAppView* iAppView;
	};

#endif // __DATAQUOTAAPPUI_H__

// End of file
