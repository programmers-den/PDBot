const {embed} = require('../utilities/display');
const {fetchMember} = require('../utilities/convert');
const ms = require('ms');

exports.name = 'user';
exports.type = 'Information';
exports.info = 'Displays user information.';
exports.usage = '[user?]';
exports.alias = ['userinfo', 'ui'];
exports.root = false;
exports.mod = false;
exports.admin = false;

exports.run = async ({message, args}) => {
	const member = args.length ? (await fetchMember(args[0], message.guild)) : message.member;
	if (member === null) return await message.channel.send(embed('RED', 'Unknown Member', 'The member you specified could not be found on this server.'));
	const date = Date.now();
	await message.channel.send(embed('BLUE', `Profile: ${member.user.tag}`, '', [
		{
			name: 'Nickname',
			value: `Their name is **${member.nickname ?? member.user.username}** on the server`
		}, {
			name: 'Identifier',
			value: `Their ID is **${member.user.id}**.`
		}, {
			name: 'Joined',
			value: `They joined the server **${ms(date - member.joinedTimestamp, {long: true})}** ago.`
		}
	], member.user.displayAvatarURL({
		dynamic: true,
		size: 256
	})));
};