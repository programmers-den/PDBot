const {embed, display} = require('../utilities/display');
const { isMod } = require("../utilities/auth")
const config = require('../config.json');
const { client } = require('..');
exports.name = 'fixannounce';
exports.type = 'Staff';
exports.info = 'Modify a server announcement.';
exports.usage = '[id] [title] [content]';
exports.alias = ["fca"];
exports.root = false;
exports.mod = false;
exports.admin = false;

exports.run = async ({message, args}) => {
    if (!args.length) {
        await message.channel.send(embed('RED', "Announce Example", "To use this command, enter the message id followed by the subject and the body. Each argument can be wrapped in quotes if it doesn't fit in a single word.\n\nBelow are some examples on how to fix a announcement.", [
            {
				name: 'Example to fix a Announcement',
				value: display({
					name: 'fca',
					usage: '"413632156061925380" "Test" "Test"',
					info: 'Send announcement to specific channel, With Title of Test, body of Test.'
				})
			},
        ]))
    } else {
        if(!args[1] || !args[2]) {return message.channel.send(embed('RED', "Announce Example", "To use this command, enter the message id followed by the subject and the body. Each argument can be wrapped in quotes if it doesn't fit in a single word.\n\nBelow are some examples on how to fix a announcement.", [
            {
				name: 'Example to fix a Announcement',
				value: display({
					name: 'fca',
					usage: '"413632156061925380" "Test" "Test"',
					info: 'Send announcement to specific channel, With Title of Test, body of Test.'
				})
			},
        ]))}
        const ch = await client.channels.cache.get(config.channels.announcements);
        const mod = await isMod(message.author.id);
        if (!mod) {return message.channel.send(embed("RED", "Announce Error", "You don't have the required role!"))};
        ch.messages.fetch(args[0])
            .then(message => message.edit(`||<@${config.roles.announcements}>||`,embed("BLUE", `${args[1]}`, `${args[2]}`)));
    }
};