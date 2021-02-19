const {embed, display} = require('../utilities/display');
const { isMod } = require("../utilities/auth")
const config = require('../config.json');
const { client } = require('..');
exports.name = 'announce';
exports.type = 'Staff';
exports.info = 'Creates a server announcement.';
exports.usage = '[title] [content]';
exports.alias = ["ca"];
exports.root = false;
exports.mod = false;
exports.admin = false;

exports.run = async ({message, args}) => {
    if (!args.length) {
        await message.channel.send(embed('RED', "Announce Example", "To use this command, enter the subject followed by the main point. Each argument can be wrapped in quotes if it doesn't fit in a single word.\n\nBelow are some examples on how to make a announcement.", [
            {
				name: 'Example of Announcement',
				value: display({
					name: 'ca',
					usage: '"Test" "Test"',
					info: 'Send announcement to specific channel, With Title of Test, body of Test.'
				})
			},
        ]))
    } else {
        const ch = await client.channels.cache.get(config.channels.announcements);
        const mod = await isMod(message.author.id);
        if (!mod) {return message.channel.send(embed("RED", "Announce Error", "You don't have the required role!"))};
        ch.send(`||<@${config.roles.announcements}>||`,embed("BLUE", `${args[0]}`, `${args[1]}`))
    }
};