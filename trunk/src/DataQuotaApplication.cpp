/*
DataQuotaApplication.cpp

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
#include "DataQuotaApplication.h"
#include "DataQuotaDocument.h"


// UID for the application should match the UID defined in the mmp file
const TUid KUidDataQuotaApp = {0xA000B6EF};


CApaDocument* CDataQuotaApplication::CreateDocumentL()
	{
	// Create an DataQuota document, and return a pointer to it
	return (static_cast<CApaDocument*>
					(CDataQuotaDocument::NewL(*this)));
	}


TUid CDataQuotaApplication::AppDllUid() const
	{
	// Return the UID for the DataQuota application
	return KUidDataQuotaApp;
	}

// End of file
