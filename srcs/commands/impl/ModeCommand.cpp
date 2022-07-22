#include "commands/impl/ModeCommand.hpp"

void ModeCommand::execute(const Command& cmd, Client& sender) {
    std::vector<std::string> args = cmd.getArgs();

    if (args.size() == 0) {
        Irc::getInstance().getServer()->send_err_needmoreparams(sender, "MODE");
        return;
    }

    std::string target = args[0];
    if (Irc::getInstance().getServer()->getChannels().find(target) == Irc::getInstance().getServer()->getChannels().end()) {
        Irc::getInstance().getServer()->send_err_nosuchchannel(sender, target);
        return;
    }

    if (args.size() == 1) {
        Irc::getInstance().getServer()->send_rpl_channelmodeis(sender, Irc::getInstance().getServer()->getChannels()[target]);
        return;
    }

    if (args.size() == 2 || args.size() == 3) {
        Channel* channel = &Irc::getInstance().getServer()->getChannels()[target];
        std::string modes = args[1];

        if (channel->getClientsAndMod()[sender.getSocket()] != '@') {
            Irc::getInstance().getServer()->send_err_chanoprivsneeded(sender, target);
            return;
        }

        bool remove = modes.at(0) == '-';
        std::string validModes = "stniklm";

        std::cout << "remove: " << remove << std::endl;
        
        for (int i = 1; i < modes.size(); i++) {
            char mode = modes.at(i);
            if (validModes.find(mode) == std::string::npos) {
                modes.erase(modes.find(mode), 1); // remove invalid mode from modes string so it doesn't appear in the reply
                Irc::getInstance().getServer()->send_err_unknownmode(sender, mode);
                continue;
            }
            
            if ((mode == LIMIT || mode == KEY) && !remove) {
                if (args.size() < 3) {
                    Irc::getInstance().getServer()->send_err_needmoreparams(sender, "MODE");
                    return;
                }
                if (mode == LIMIT) {
                    int limit = atoi(args[2].c_str());
                    if (limit <= 0) // if the limit is not a positive number or is not a number, do nothing
                        return;
                    channel->setLimit(limit);
                } else if (mode == KEY) {
                    channel->setKey(args[2]);
                }
            }

            if (remove) {
                    std::cout << "remove: " << mode << std::endl;
                if (channel->hasMode(mode))
                    channel->removeMode(mode);
                else
                    modes.erase(modes.find(mode), 1); // remove mode from modes string so it doesn't appear in the reply
            } else {
                std::cout << "add: " << mode << std::endl;
                if (!channel->hasMode(mode))
                    channel->addMode(mode);
                else
                    modes.erase(modes.find(mode), 1); // remove mode from modes string so it doesn't appear in the reply
                std::cout << "modes: " << channel->getModes() << std::endl;
            }
        }
        // replace first char of mode string with '+' if it is neither '+' nor '-' for valid reply
        if (modes.at(0) != '+' && modes.at(0) != '-')
            modes = modes.replace(0, 1, "+");

        //TODO afficher host
        std::string message = ":" + sender.getNickname() + "!~" + sender.getUsername() + "@localhost MODE " + target + " " + modes;
        if (args.size() > 2)
            message += " " + args[2];
        message += CRLF;
        if (modes.length() > 1) // don't send the message if no mode was added or removed
            send(sender.getSocket(), message.c_str(), message.size(), 0);
        return;
    }
}