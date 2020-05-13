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

#include "SNMPTracer.h"

#include <sstream>
#include <iostream>

#define __VER__ "1.0"

#pragma warning(disable : 4996)


SNMPTracer SNMPTracer::_snmpTracer;

SNMPTracer::SNMPTracer() {
#ifdef _DEBUG
	_active = false;
	_hConsole = INVALID_HANDLE_VALUE;
	_hConsoleInput = INVALID_HANDLE_VALUE;
//	::AllocConsole();
	_hConsole = ::GetStdHandle(STD_OUTPUT_HANDLE);
	_hConsoleInput = ::GetStdHandle(STD_INPUT_HANDLE);
#endif
}

SNMPTracer::~SNMPTracer() {
#ifdef _DEBUG
	::CloseHandle(_hConsole);
	::CloseHandle(_hConsoleInput);
//	::FreeConsole();
#endif
}

void SNMPTracer::trace(const std::string & message)	{	
#ifdef _DEBUG
	if(!isEnabled() || _hConsole == INVALID_HANDLE_VALUE) {
		return;
	}
	std::stringstream ss;
	SYSTEMTIME	time;
	::GetLocalTime(&time);
	char timeStr[33];
	sprintf(timeStr, "%.2d:%.2d:%.2d,%.3d", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
	ss << timeStr << ": " << message << "\n";
	DWORD chars;
	::WriteConsole(_hConsole, ss.str().c_str(), (DWORD)ss.str().size(), &chars, NULL);
#endif
}
