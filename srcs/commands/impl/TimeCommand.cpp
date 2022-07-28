#include "commands/impl/TimeCommand.hpp"
#include "Irc.hpp"

#include <time.h>

void TimeCommand::execute(const Command& cmd, Client& sender) {

	static_cast<void>(cmd);
	time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

	Irc::getInstance().getServer()->send_rpl_time(sender, std::string(buf));
}