const { embed } = require("../utilities/display");
const { isMod } = require('../utilities/auth');
const { client } = require('..');

exports.name = 'disconnect';
exports.type = 'Music';
exports.info = 'Skips the currently playing song.';
exports.usage = '';
exports.alias = ['dc', 'fuckoff'];
exports.root = false;
exports.admin = false;
exports.mod = false;

exports.run = async ({message}) => {
	const player = client.music.get(message.guild.id);
	if (player === undefined || player.state !== 'CONNECTED') return await message.channel.send(embed('RED', 'Skip Error', 'The bot is not connected to a voice channel.'));
	if (player.textChannel !== message.channel.id || player.voiceChannel !== message.member.voice.channel.id) {
		return await message.channel.send(embed('RED', 'Disconnect Error', `You must be in <#${player.voiceChannel}> and run commands in <#${player.textChannel}> to control the queue.`));
	}
	const mod = await isMod(message.author.id);
	if (message.guild.channels.cache.get(message.member.voice.channelID).members.filter(member => member.user.bot).size > 1 || !mod) return await message.channel.send(embed('RED', 'Disconnect Error', 'You can only disconnect the bot if you are alone in the voice channel.'));
	player.destroy();
	await message.channel.send(embed('GREEN', 'Bot Disconnected', `You have successfully disconnected the bot from the voice channel.`));
}