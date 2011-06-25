#
# Data Quota for Symbian phones.
# http://code.google.com/p/dataquota/
# Copyright (C) 2008, 2010  Hugo van Kemenade
# 
# This file is part of Data Quota.
# 
# Data Quota is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
# 
# Data Quota is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with Data Quota.  If not, see <http://www.gnu.org/licenses/>.
#
ifeq (WINS,$(findstring WINS, $(PLATFORM)))
ZDIR=$(EPOCROOT)epoc32\release\$(PLATFORM)\$(CFG)\Z
else
ZDIR=$(EPOCROOT)epoc32\data\z
endif

TARGETDIR=$(ZDIR)\resource\apps
ICONTARGETFILENAME=$(TARGETDIR)\dataquota_aif.mif

ICONDIR=..\gfx

do_nothing :
	@rem do_nothing

MAKMAKE : do_nothing

BLD : do_nothing

CLEAN : do_nothing

LIB : do_nothing

CLEANLIB : do_nothing

RESOURCE :
	mifconv $(ICONTARGETFILENAME) \
		/X /c32 $(ICONDIR)\dataquota.svg

FREEZE : do_nothing

SAVESPACE : do_nothing

RELEASABLES :
	@echo $(ICONTARGETFILENAME)

FINAL : do_nothing

# End of file
