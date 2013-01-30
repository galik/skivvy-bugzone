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
#include <skivvy/logrep.h>

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
, chanops(bot, "chanops")
{
}

BugzoneIrcBotPlugin::~BugzoneIrcBotPlugin() {}

// Workflow

// new - new bugs
// fix - fixed
// die - won't fix
// dup: #id - duplicate of id

const str BUG_ENTRY_PREFIX = "bug.entry.";

str BugzoneIrcBotPlugin::get_user(const message& msg)
{
	bug_func();
	bug_var(chanops);
	// chanops user | msg.userhost
	if(chanops && chanops->is_userhost_logged_in(msg.get_userhost()))
		return chanops->get_userhost_username(msg.get_userhost());
	return msg.get_userhost();
}

bool BugzoneIrcBotPlugin::do_bug(const message& msg)
{
	BUG_COMMAND(msg);
	if(msg.get_user_params().empty())
		return false;

	str user = get_user(msg);
	bug_var(user);

	lock_guard lock(mtx);
	siz id = store.get("bug.id", 0);
	store.set("bug.id", ++id);
	store.add(BUG_ENTRY_PREFIX + std::to_string(id) + ".new", user + " \"" + msg.get_user_params() + "\"");
	bot.fc_reply(msg, "Your bug has been filed with tracking number " + std::to_string(id));
	return true;
}

bool BugzoneIrcBotPlugin::do_buglist(const message& msg)
{
	BUG_COMMAND(msg);
	// !buglist *(new|fixed|dead|dups) - list bugs if associated with caller

	str params = msg.get_user_params();
	bug_var(params);
	replace(params, ",", " ");
	bug_var(params);

	siss iss(params);

	str state;
	str_set stati;
	while(iss >> state)
		stati.insert(state);

	bug_var(stati.size());

	str_set keys;

	lock_guard lock(mtx);

	if(stati.empty())
		keys = store.get_keys_if_wild(BUG_ENTRY_PREFIX + "*");
	else
		for(const str& state: stati)
			for(const str& key: store.get_keys_if_wild(BUG_ENTRY_PREFIX + "*." + state))
				keys.insert(key);

	bug_var(keys.size());

	str user = get_user(msg);
	bug_var(user);

	// key = bug.entry.1.new
	for(const str& key: keys)
	{
		bug_var(key);
		if(key.size() <= BUG_ENTRY_PREFIX.size())
			continue;

		siss iss(key.substr(BUG_ENTRY_PREFIX.size()));

		str skip;
		siz id = 0;
		if(!(iss >> id))
		{
			log("ERROR: unexpected key in bug store: " << key);
			continue;
		}

		str status = "unknown";
		if(!sgl(sgl(iss, skip, '.'), status))
		{
			log("ERROR: unexpected key in bug store: " << key);
			continue;
		}
		bug_var(id);
		iss.clear();
		iss.str(store.get(key));

		str quser, text;
		if(!sgl(sgl(iss >> quser, skip, '"'), text, '"'))
		{
			log("ERROR: bad value in bug store: " << key);
			continue;
		}
		bug_var(text);

		if(quser == user)
			bot.fc_reply(msg, status + ": " + std::to_string(id) + " " + text);
	}
	return true;
}

bool BugzoneIrcBotPlugin::do_feature(const message& msg)
{
	BUG_COMMAND(msg);
//	if(msg.get_user_params().empty())
//		return false;
//
//	lock_guard lock(mtx);
//	siz id = store.get("feature.id", 0);
//	store.set("feature.id", ++id);
//	store.add("feature." + std::to_string(id) + ".new", " \"" + msg.get_user_params() + "\"");
//	bot.fc_reply(msg, "Your feature request has been filed with tracking number " + std::to_string(id));
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
		"!buglist"
		, "!buglist *(new|dead|fixed|dups) list your bugs, optionally by category."
		, [&](const message& msg){ do_buglist(msg); }
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
