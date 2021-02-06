const { embed } = require("../utilities/display.js");
const { sendMenu, pages } = require('../utilities/menus');
const { client } = require('..');

exports.name = 'queue';
exports.type = 'Music';
exports.info = 'Displays a list of songs in the queue.';
exports.usage = '';
exports.alias = ['q'];
exports.root = false;
exports.admin = false;
exports.mod = false;

exports.run = async ({message}) => {
	const player = client.music.get(message.guild.id);
	if (player === undefined || player.state !== 'CONNECTED' || player.queue.current === null) return await message.channel.send(embed('RED', 'Skip Error', 'There is no music playing on this server.'));
	const songs = player.queue.map((song, index) => `${index + 1} • [${song.title}](${song.uri})`);
	songs.unshift(`Now Playing • [${player.queue.current.title}](${player.queue.current.uri})\n`);
	const menus = [];
	const size = 8;
	for (let i = 0; i < songs.length; i += size) {
		menus.push(embed('BLACK', `Song Queue: ${i / size + 1}/${Math.ceil(songs.length / size)}`, songs.slice(i, i + size).join('\n')));
	}
	if (menus.length === 1) return await message.channel.send(menus[0]);
	await sendMenu(message.channel, pages(...menus), message.author);
}