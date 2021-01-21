const {embed, display, pollEmbed, defaultPollOptions} = require('../utilities/display');
const config = require('../config.json');

exports.name = 'poll';
exports.type = 'Utilities';
exports.info = 'Sets up community polls.';
exports.usage = '[title] [options]';
exports.alias = [];
exports.root = false;
exports.mod = false;
exports.admin = false;

exports.run = async ({message, args}) => {
	if (!args.length) {
		await message.channel.send(embed('RED', 'Poll Example', "To use this command, enter the title followed by a list of options. Each argument can be wrapped in quotes if it doesn't fit in a single word.\n\nAfter you run the command, add the reactions to the message and it will be setup. Below are some examples on how to setup a poll.", [
			{
				name: 'Example Question',
				value: display({
					name: 'poll',
					usage: '"Do you like pizza?"',
					info: 'Two reactions, thumbsup and thumbsdown.'
				})
			},
			{
				name: 'Example Poll',
				value: display({
					name: 'poll',
					usage: '"Pizza or spaghetti?" Pizza Spaghetti',
					info: 'Manual custom reactions.'
				})
			}
		]));
	} else if (args.length === 1) {
		const arg = args[0];
		const channel = message.guild.channels.cache.get(config.channels.polls);
		const poll = await channel.send(pollEmbed(arg, defaultPollOptions));
		
		await poll.react(config.emojis.yes);
		await poll.react(config.emojis.no);
	}
};