const { embed } = require("../utilities/display.js");
const { isMod } = require('../utilities/auth');
const { client } = require('..');

exports.name = 'forceskip';
exports.type = 'Music';
exports.info = 'Skips the currently playing song.';
exports.usage = '';
exports.alias = ['fs', 'fskip'];
exports.root = false;
exports.admin = false;
exports.mod = false;

exports.run = async ({message}) => {
	const player = client.music.get(message.guild.id);
	if (player === undefined || player.state !== 'CONNECTED' || player.current === null) return await message.channel.send(embed('RED', 'Skip Error', 'There is no music playing on this server.'));
	if (player.textChannel !== message.channel.id || player.voiceChannel !== message.member.voice.channel.id) {
		return await message.channel.send(embed('RED', 'Skip Error', `You must be in <#${player.voiceChannel}> and run commands in <#${player.textChannel}> to control the queue.`));
	}
	const mod = await isMod(message.author.id);
	if (message.guild.channels.cache.get(message.member.voice.channelID).members.filter(member => member.user.bot).size > 1 || !mod) return await message.channel.send(embed('RED', 'Skip Error', 'You can only force skip songs if you are alone in the voice channel.'));
	const song = player.queue.current;
	player.stop();
	if (player.queue.size === 0) await message.channel.send(embed('GREEN', 'Music Skipped', `You have successfully skipped [${song.title}](${song.uri}).`));
}