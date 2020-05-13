//
//  SNMP - A C++ Wrapper for the WinSnmp library
//  Copyright (C) 2004  Rog?io Paulo, rpaulo@bigfoot.com
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//

#pragma once 

#include "Winsnmp.h"
#include <string>
#include <vector>
#include <map>
#include "SNMPRequest.h"


class SNMPRequestSet {
protected :
	std::vector<SNMPRequest*> _requests;
public:	
	SNMPRequestSet();	
	virtual ~SNMPRequestSet();

	void addRequest(SNMPRequest * request);
	int getRequestCount()			{ return (int)_requests.size(); }
	SNMPRequest * getRequest(int i)	{ return _requests[i]; }
	void clear();

	void execute(SNMPRemoteAgent * pRemoteAgent = NULL);
	void cancel();
	void wait(DWORD timeout = INFINITE);

	bool isTerminated();

	std::string getDisplayInformation() const;
};
