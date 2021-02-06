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

exports.run = async ({message, args}) => {
	const player = client.music.get(message.guild.id);
	if (player && player.queue === null ) return await message.channel.send(embed('RED', 'Remove Error', 'There is no songs in the queue to remove'));
	if (player.textChannel !== message.channel.id || player.voiceChannel !== message.member.voice.channel.id) {
		return await message.channel.send(embed('RED', 'Remove Error', `You must be in <#${player.voiceChannel}> and run commands in <#${player.textChannel}> to control the queue.`));
	}

    const num = +args[0]
    if (player && player.queue !== null && !isNaN(num)) {
        const song = player.queue[num-1]
        await player.queue.remove(num -1)
        await message.channel.send(embed("GREEN", "Song Removed From Queue", `Successfully removed [${song.title}](${song.uri}) from the queue.`))
    }
}