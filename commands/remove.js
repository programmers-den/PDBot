const { embed } = require("../utilities/display.js");
const { isMod } = require('../utilities/auth');
const { client } = require('..');

exports.name = 'remove';
exports.type = 'Music';
exports.info = 'Remove song from queue.';
exports.usage = '';
exports.alias = ['rm'];
exports.root = false;
exports.admin = false;
exports.mod = false;

exports.run = async ({message, args}) => {
    const num = +args[0]
	const player = client.music.get(message.guild.id);
	if (!player || player.queue === null || isNaN(num) || Math.floor(num) !== num || num < 1 || num > player.queue.size) return message.channel.send(embed('RED', 'Remove Error', 'There is no songs in the queue to remove or index out of range.'));
	if (player.textChannel !== message.channel.id || player.voiceChannel !== message.member.voice.channel.id) {
		return await message.channel.send(embed('RED', 'Remove Error', `You must be in <#${player.voiceChannel}> and run commands in <#${player.textChannel}> to control the queue.`));
	}

    if (player && player.queue !== null) {
        const song = player.queue[num-1]
        await player.queue.remove(num -1)
        await message.channel.send(embed("GREEN", "Song Removed From Queue", `Successfully removed [${song.title}](${song.uri}) from the queue.`).then(msg => msg.delete({timeout: 10000})))
    }
}