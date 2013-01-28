/*
 * plugin-bugzone.cpp
 *
 *  Created on: 28 Jan 2013
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

#include <skivvy-bugzone/plugin-bugzone.h>

#include <skivvy/types.h>

#include <string>

namespace skivvy { namespace bugzone {

// You MUST have this macro and it MUST name your plugin class
IRC_BOT_PLUGIN(BugzoneIrcBotPlugin);
PLUGIN_INFO("bugzone", "Bug Tracker", "0.0");

const str STORE_FILE_KEY = "bugzone.store.file";
const str DEFAULT_STORE_FILE = "bugzone-store.txt";

BugzoneIrcBotPlugin::BugzoneIrcBotPlugin(IrcBot& bot)
: BasicIrcBotPlugin(bot)
, store(bot.getf(STORE_FILE_KEY, DEFAULT_STORE_FILE))
{
}

BugzoneIrcBotPlugin::~BugzoneIrcBotPlugin() {}

// Workflow

// new - new bugs
// fix - fixed
// die - won't fix
// dup: #id - duplicate of id

bool BugzoneIrcBotPlugin::do_bug(const message& msg)
{
	if(msg.get_user_params().empty())
		return false;

	lock_guard lock(mtx);
	siz id = store.get("bug.id", 0);
	store.set("bug.id", ++id);
	store.add("bug." + std::to_string(id) + ".new", " \"" + msg.get_user_params() + "\"");
	bot.fc_reply(msg, "Your bug has been filed with tracking number " + std::to_string(id));
	return true;
}

bool BugzoneIrcBotPlugin::do_feature(const message& msg)
{
	if(msg.get_user_params().empty())
		return false;

	lock_guard lock(mtx);
	siz id = store.get("feature.id", 0);
	store.set("feature.id", ++id);
	store.add("feature." + std::to_string(id) + ".new", " \"" + msg.get_user_params() + "\"");
	bot.fc_reply(msg, "Your feature request has been filed with tracking number " + std::to_string(id));
	return true;
}

// INTERFACE: BasicIrcBotPlugin

bool BugzoneIrcBotPlugin::initialize()
{
	// Commands MUST start with ! (unless they are sent by PM)
	add
	({
		"!bug"
		, "!bug <description> Record a bug"
		, [&](const message& msg){ do_bug(msg); }
	});
	add
	({
		"!feature"
		, "!feature <description> Request a feature|modification."
		, [&](const message& msg){ do_feature(msg); }
	});

	// add more commands as appropriate
	//add
	//({
	//	"!do_other_stuff"
	//	, "!do_other_stuff Do other stuff!"
	//	, [&](const message& msg){ do_other_stuff(msg); }
	//});

	//bot.add_monitor(*this);

	return true;
}

// INTERFACE: IrcBotPlugin

str BugzoneIrcBotPlugin::get_id() const { return ID; }
str BugzoneIrcBotPlugin::get_name() const { return NAME; }
str BugzoneIrcBotPlugin::get_version() const { return VERSION; }

void BugzoneIrcBotPlugin::exit()
{
	// clean up (stop threads etc...)
}

}} // skivvy::bugzon
