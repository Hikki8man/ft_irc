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
    
        if (channel->getClientsAndMode().at(sender.getSocket()) != CHANNEL_OP) {
            Irc::getInstance().getServer()->send_err_chanoprivsneeded(sender, target);
            return;
        }

        bool remove = modes.at(0) == '-';
        std::string validModes = "stniklmov";

        std::cout << "remove: " << remove << std::endl;
        
        for (size_t i = 1; i < modes.size(); i++) {
            char mode = modes.at(i);
            if (validModes.find(mode) == std::string::npos) {
                modes.erase(modes.find(mode), 1); // remove invalid mode from modes string so it doesn't appear in the reply
                Irc::getInstance().getServer()->send_err_unknownmode(sender, mode);
                continue;
            }
            
            // +i and +k specific cases
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
            // +o and +v specific cases
            } else if (mode == OPERATOR || mode == VOICE) {
                if (args.size() < 3) {
                    Irc::getInstance().getServer()->send_err_needmoreparams(sender, "MODE");
                    return;
                }
                // Check if target nick exists
                if (Irc::getInstance().getServer()->nickIsUsed(args[2]) == false) {
                    Irc::getInstance().getServer()->send_err_nosuchnick(sender, args[2]);
                    return;
                }

                Client& targetClient = Irc::getInstance().getServer()->findClientByName(args[2]);
                if (mode == OPERATOR) {
                    if (remove)
                        channel->setClientMode(targetClient, NONE);
                    else
                        channel->setClientMode(targetClient, CHANNEL_OP);
                } else if (mode == VOICE) {
                    if (remove)
                        channel->setClientMode(targetClient, NONE);
                    else if (channel->getClientsAndMode().at(targetClient.getSocket()) != CHANNEL_OP)
                        channel->setClientMode(targetClient, CHANNEL_VOICE);
                }

                // replace first char of mode string with '+' if it is neither '+' nor '-' for valid reply
                if (modes.at(0) != '+' && modes.at(0) != '-')
                    modes = modes.replace(0, 1, "+");

                // send message to sender and target client
                for (std::map<int, char>::const_iterator clientIt = channel->getClientsAndMode().begin(); clientIt !=               channel->getClientsAndMode().end(); ++clientIt) {
                    sender.sendMessage(clientIt->first, "MODE " + target + " " + modes + " " + args[2]);
                }
                return;
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

        std::string message = "MODE " + target + " " + modes;
        if (args.size() > 2)
            message += " " + args[2];
        if (modes.length() > 1) { // don't send the message if no mode was added or removed
            for (std::map<int, char>::const_iterator clientIt = channel->getClientsAndMode().begin(); clientIt != channel->getClientsAndMode().end(); ++clientIt) {
                sender.sendMessage(clientIt->first, message);
            }
        }
    }
}