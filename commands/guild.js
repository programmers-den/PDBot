const {embed} = require('../utilities/display');

exports.name = 'guild';
exports.type = 'Information';
exports.info = 'Displays guild information.';
exports.usage = '[guild?]';
exports.alias = ['guildinfo', 'gi', 'server', 'serverinfo', 'si'];
exports.root = false;
exports.mod = false;
exports.admin = false;

exports.run = async ({message, args}) => {
	await message.channel.send(embed('BLUE', `Guild: ${message.guild.name}`, 'This will be filled in later.'));
};