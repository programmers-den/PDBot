const {embed} = require('../utilities/display');
const {fetchMember} = require('../utilities/convert');

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
	await message.channel.send(embed('BLUE', `Profile: ${member.user.tag}`, 'This will be filled in later.'));
};