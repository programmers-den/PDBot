const {embed} = require('../utilities/display');
const ms = require('ms');

exports.name = 'guild';
exports.type = 'Information';
exports.info = 'Displays guild information.';
exports.usage = '[guild?]';
exports.alias = ['guildinfo', 'gi', 'server', 'serverinfo', 'si'];
exports.root = false;
exports.mod = false;
exports.admin = false;

exports.run = async ({message, args}) => {
	const date = Date.now();
	const members = await message.guild.members.fetch();
	await message.channel.send(embed('BLUE', `Guild: ${message.guild.name}`, '', [
		{
			name: 'Owner',
			value: `<@${message.guild.ownerID}> made the server.`
		}, {
			name: 'Identifier',
			value: `The ID is **${message.guild.id}**.`
		}, {
			name: 'Created',
			value: `The server was made **${ms(date - message.guild.createdTimestamp, {long: true})}** ago.`
		}, {
			name: 'Members',
			value: `There are **${members.size}** members and **${members.filter(member => member.presence.status !== 'offline').size}** of them are online.`
		}, {
			name: 'Users',
			value: `There are **${members.filter(member => !member.user.bot).size}** humans and **${members.filter(member => !member.user.bot && member.presence.status !== 'offline').size}** of them are online.`
		}, {
			name: 'Bots',
			value: `There are **${members.filter(member => member.user.bot).size}** bots and **${members.filter(member => member.user.bot && member.presence.status !== 'offline').size}** of them are online.`
		}
	], message.guild.iconURL({
		dynamic: true,
		size: 256
	})));
};