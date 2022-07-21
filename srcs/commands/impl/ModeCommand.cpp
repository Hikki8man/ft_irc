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

    if (args.size() == 2) {
        Channel* channel = &Irc::getInstance().getServer()->getChannels()[target];
        std::string modes = args[1];

        if (channel->getClientAndMod(sender.getNickname()).mod != '@') {
            Irc::getInstance().getServer()->send_err_chanoprivsneeded(sender, target);
            return;
        }

        bool remove = modes.at(0) == '-';
        std::string validModes = "stniklm";

        std::cout << "remove: " << remove << std::endl;
        
        for (int i = 1; i < modes.size(); i++) {
            char mode = modes.at(i);
            if (validModes.find(mode) == std::string::npos) {
                Irc::getInstance().getServer()->send_err_unknownmode(sender, mode);
                continue;
            }
            
            if (remove) {
                    std::cout << "remove: " << mode << std::endl;
                if (channel->getModes().find(mode) != std::string::npos)
                    channel->removeMode(mode);
            } else {
                std::cout << "add: " << mode << std::endl;
                if (channel->getModes().find(mode) == std::string::npos)
                    channel->addMode(mode);
                std::cout << "modes: " << channel->getModes() << std::endl;
            }
        }
        //Irc::getInstance().getServer()->send_rpl_channelmodeis(sender, Irc::getInstance().getServer()->getChannels()[target]);
        return;
    }
}