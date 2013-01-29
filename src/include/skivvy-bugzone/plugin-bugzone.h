#pragma once
#ifndef _SKIVVY_PLUGIN_BUGZONE_H_
#define _SKIVVY_PLUGIN_BUGZONE_H_
/*
 * plugin-example.h
 *
 *  Created on: 30 Jan 2012
 *      Author: oaskivvy@gmail.com
 */

/*-----------------------------------------------------------------.
| Copyright (C) 2012 SooKee oaskivvy@gmail.com               |
'------------------------------------------------------------------'

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
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301, USA.

http://www.gnu.org/licenses/gpl-2.0.html

'-----------------------------------------------------------------*/

#include <skivvy/ircbot.h>
#include <skivvy/types.h>
#include <skivvy/store.h>
#include <skivvy/logrep.h>

#include <skivvy-chanops/plugin-chanops.h>

#include <mutex>

namespace skivvy { namespace bugzone {

using namespace skivvy;
using namespace skivvy::ircbot;
using namespace skivvy::utils;

/**
 *
 */
class BugzoneIrcBotPlugin
: public BasicIrcBotPlugin
{
private:
	std::mutex mtx;
	BackupStore store;

	IrcBotPluginHandle<ChanopsIrcBotPlugin> chanops;

	/**
	 * One of the plugin's functions
	 */
	bool do_bug(const message& msg);
	bool do_buglist(const message& msg);
	bool do_feature(const message& msg);

public:
	BugzoneIrcBotPlugin(IrcBot& bot);
	virtual ~BugzoneIrcBotPlugin();

	// INTERFACE: BasicIrcBotPlugin

	virtual bool initialize();

	// INTERFACE: IrcBotPlugin

	virtual str get_id() const;
	virtual str get_name() const;
	virtual str get_version() const;
	virtual void exit();
};

}} // skivvy::bugzone

#endif // _SKIVVY_PLUGIN_BUGZONE_H_
