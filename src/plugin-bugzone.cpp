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

#include <algorithm>

namespace skivvy { namespace bugzone {

// You MUST have this macro and it MUST name your plugin class
IRC_BOT_PLUGIN(BugzoneIrcBotPlugin);
PLUGIN_INFO("bugzone", "Bug Tracker", "0.0");

BugzoneIrcBotPlugin::BugzoneIrcBotPlugin(IrcBot& bot)
: BasicIrcBotPlugin(bot)
{
}

BugzoneIrcBotPlugin::~ExampleIrcBotPlugin() {}

void BugzoneIrcBotPlugin::do_stuff(const message& msg)
{
	str cmd = msg.get_user_cmd(); // this is !do_stuff
	str params = msg.get_user_params(); // this is any text after the command

	bot.fc_reply_pm(msg, "I sent you a PM!");
	bot.fc_reply(msg, "I did stuff!");
}

// INTERFACE: BasicIrcBotPlugin

bool BugzoneIrcBotPlugin::initialize()
{
	// Commands MUST start with ! (unless they are sent by PM)
	add
	({
		"!do_stuff"
		, "!do_stuff Send you a PM and do stuff!"
		, [&](const message& msg){ do_stuff(msg); }
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

str ExampleIrcBotPlugin::get_id() const { return ID; }
str ExampleIrcBotPlugin::get_name() const { return NAME; }
str ExampleIrcBotPlugin::get_version() const { return VERSION; }

void ExampleIrcBotPlugin::exit()
{
	// clean up (stop threads etc...)
}

}} // skivvy::bugzon
