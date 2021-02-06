const { embed, display, duration } = require("../utilities/display");
const { sendMenu, pages } = require('../utilities/menus');
const { client } = require('..');

exports.name = 'search';
exports.type = 'Music';
exports.info = 'Searches for songs by a query and lets you select one.';
exports.usage = '[query]';
exports.alias = [''];
exports.root = false;
exports.admin = false;
exports.mod = false;

exports.run = async ({message, args, text}) => {
	if (!args.length) {
		return await message.channel.send(embed('BLACK', 'Search Example', 'Refer to the following fields for an example of how to use this command.', [
			{
				name: 'Example Usage',
				value: display({
					name: 'search',
					usage: 'rick roll',
					info: 'Pick from a list of rick rolls.'
				})
			}
		]))
	}

	if (message.member.voice.channel === null) return await message.channel.send(embed('RED', 'Search Error', 'You must be connected to a voice channel to play music.'));

	let player = client.music.get(message.guild.id);
	if (player === undefined) {
		player = await client.music.create({
			guild: message.guild.id,
			voiceChannel: message.member.voice.channelID,
			textChannel: message.channel.id,
			selfMute: false,
			selfDeafen: true,
		});
	}

	if (player.state !== 'CONNECTED') player.connect();
	if (player.state === 'CONNECTED' && (player.textChannel !== message.channel.id || player.voiceChannel !== message.member.voice.channel.id)) {
		return await message.channel.send(embed('RED', 'Search Error', `You must be in <#${player.voiceChannel}> and run commands in <#${player.textChannel}> to control the queue.`));
	}

	const result = await player.search({
		query: text,
		source: 'youtube'
	}, message.author.id);

	if (['SEARCH_RESULT', 'TRACK_LOADED'].includes(result.loadType)) {
		const menus = [];
		const size = 5;
		for (let i = 0; i < result.tracks.length; i += size) {
			menus.push(embed('BLACK', `Song Search: ${i / size + 1}/${Math.ceil(result.tracks.length / size)}`, result.tracks.slice(i, i + size).map((track, index) => {
				const number = i + index + 1;
				return `${number} • [${track.title}](${track.uri})\nArtist • ${track.author}\nDuration • ${duration(track.duration)}`;
			}).join('\n\n')));
		}
		if (menus.length === 1) await message.channel.send(menus[0]);
		else await sendMenu(message.channel, pages(...menus), message.author);
		let collected;
		try {
			collected = await message.channel.awaitMessages(msg => msg.author.id === message.author.id, {
				max: 1,
				time: 60000
			});
		} catch {
			return await message.channel.send(embed('RED', 'Search Timeout', 'Your search has timed out. If you want to select a song still, search again.'));
		}
		const text = collected.first().content.toLowerCase();
		if (text === 'cancel') return await message.channel.send(embed('RED', 'Search Cancelled', 'You have cancelled your music search.'));
		let content = +text.replace(/[^0-9]/g, '');
		if (isNaN(content) || Math.floor(content) !== content && [Infinity, -Infinity].includes(content) || content < 1 || content > result.tracks.length) return await message.channel.send(embed('RED', 'Search Cancelled', 'The number provided was in an invalid format. Perform the search again and enter a song index.'));
		content--;
		const shouldPlayNow = !player.playing && !player.paused && !player.queue.size;
		player.queue.add(result.tracks[content]);
		if (!shouldPlayNow) await message.channel.send(embed('BLACK', 'Added to Queue', `[${result.tracks[content].title}](${result.tracks[content].uri})\nArtist • ${result.tracks[content].author}\nRequester • ${message.author.tag}\nDuration • ${duration(result.tracks[content].duration)}`, [], result.tracks[content].displayThumbnail('maxresdefault')));
		else await player.play();
	} else if (result.loadType === 'NO_MATCHES') {
		await message.channel.send(embed('RED', 'Play Error', `No matches found for \`${result.query}\`.`));
	} else if (result.loadType === 'LOAD_FAILED') {
		await message.channel.send(embed('RED', 'Play Error', `Failed to load the tracks for \`${result.query}\`.`));
	}
}