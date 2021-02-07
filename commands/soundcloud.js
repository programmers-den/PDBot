const { embed, display, duration } = require("../utilities/display.js");
const { client } = require('..');

exports.name = 'play';
exports.type = 'Music';
exports.info = 'Plays music either with a search query or a link.';
exports.usage = '[query]';
exports.alias = ['sc'];
exports.root = false;
exports.admin = false;
exports.mod = false;

exports.run = async ({message, args, text}) => {
	if (!args.length) {
		return await message.channel.send(embed('BLACK', 'Soundcloud Example', 'Refer to the following fields for an example of how to use this command.', [
			{
				name: 'From Link',
				value: display({
					name: 'soundcloud',
					usage: 'https://soundcloud.com/iamkylejohnson/rick-roll',
					info: 'Rick roll with a link.'
				})
			},
			{
				name: 'From Query',
				value: display({
					name: 'soundcloud',
					usage: 'RICK ROLL$',
					info: 'Rick roll using a query.'
				})
			}
		]))
	}

	if (message.member.voice.channel === null) return await message.channel.send(embed('RED', 'Play Error', 'You must be connected to a voice channel to play music.'));

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
		return await message.channel.send(embed('RED', 'Play Error', `You must be in <#${player.voiceChannel}> and run commands in <#${player.textChannel}> to control the queue.`));
	}

	let valid = false;
	try {
		new URL(args[0]);
		valid = true;
	} catch {}

	let result;
	if (valid) {
		result = await player.search(args[0], message.author.id)
	} else {
		result = await player.search({
			query: text,
			source: 'soundcloud'
		}, message.author.id);
	}

	if (['SEARCH_RESULT', 'TRACK_LOADED'].includes(result.loadType)) {
		const shouldPlayNow = !player.playing && !player.paused && !player.queue.size;
		player.queue.add(result.tracks[0]);
		if (!shouldPlayNow) await message.channel.send(embed('BLACK', 'Added to Queue', `[${result.tracks[0].title}](${result.tracks[0].uri})\nArtist • ${result.tracks[0].author}\nRequester • ${message.author.tag}\nDuration • ${duration(result.tracks[0].duration)}`, [], result.tracks[0].displayThumbnail('maxresdefault')));
		else await player.play();
	} else if (result.loadType === 'PLAYLIST_LOADED') {
		if (!result.playlist) return;
		const shouldPlayNow = !player.playing && !player.paused && !player.queue.totalSize === result.tracks.length;
		if (!shouldPlayNow) await message.channel.send(embed('BLACK', 'Added to Queue', `[${result.playlist.title}](${result.playlist.uri})\Tracks • ${result.tracks.length}\nRequester • ${message.author.tag}\nDuration • ${duration(result.tracks[0].duration)}`, [], result.playlist.selectedTrack.displayThumbnail('maxresdefault')));
		else await player.play();
	} else if (result.loadType === 'NO_MATCHES') {
		await message.channel.send(embed('RED', 'Play Error', `No matches found for your query.`));
	} else if (result.loadType === 'LOAD_FAILED') {
		await message.channel.send(embed('RED', 'Play Error', `Failed to load the tracks for your query.`));
	}
}