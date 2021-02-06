const { embed, duration } = require("../utilities/display.js");
const { client } = require('..');

exports.name = 'nowplaying';
exports.type = 'Music';
exports.info = 'Displays the song that is currently playing.';
exports.usage = '';
exports.alias = ['np'];
exports.root = false;
exports.admin = false;
exports.mod = false;

exports.run = async ({message}) => {
	const player = client.music.get(message.guild.id);
	if (player === undefined || player.state !== 'CONNECTED' || player.queue.current === null) return await message.channel.send(embed('RED', 'Skip Error', 'There is no music playing on this server.'));
	const user = await client.users.fetch(player.queue.current.requester);
	await message.channel.send(embed('BLACK', 'Now Playing', `[${player.queue.current.title}](${player.queue.current.uri})\nArtist • ${player.queue.current.author}\nRequester • ${user.tag}\nDuration • ${duration(player.queue.current.duration)}`, [], player.queue.current.displayThumbnail('maxresdefault')));
}