const {embed, display} = require(`../utilities/display`);
const {client} = require('..');

exports.name = 'aplay';
exports.type = 'Music';
exports.info = 'Play music with discord file attachments'
exports.usage = '[attachment link prob]';
exports.alias = [`apl`];
exports.root = false;
exports.admin = false
exports.mod = false;

exports.run = async({message, args}) => {
    if (!args.length) {
        return await message.channel.send(embed(`BLACK`, `Attachment Play Example`, `Refer to the following example to play the attachment file.` [
            {
                name: `Example`,
                value: display({
                    name: `apl`,
                    usage: `https://cdn.discordapp.com/attachments/x/d/Never_gonna_give_you_up.mp3`,
                    info: `Copy the attachment link (Right click on attachment and press copy link) and play the attachment.`
                })
            }
        ]))
    }

    if (message.member.voice.channel === null) return await message.channel.send(embed('RED', "Attachment Play Error", `You must be connected to a voice channel to play music.`));

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

    if (player.state !== `CONNECTED`) player.connect();
    if (player.state === `CONNECTED` && (player.textChannel !== message.channel.id || player.voiceChannel !== message.member.voice.channel.id)) {
		return await message.channel.send(embed('RED', 'Attachment Play Error', `You must be in <#${player.voiceChannel}> and run commands in <#${player.textChannel}> to control the queue.`));
	}

    let valid = false;
    try {
        new URL(args[0]);
        valid = true;
    } catch {}

    let result;
    if (valid) {
        result = await player.search({query: args[0], source: `http`}, message.author.id)
    } else {
        return await message.channel.send(`No. It. Must. Be. A. Discord. Link.`);
    }

    if (['SEARCH_RESULT', 'TRACK_LOADED'].includes(result.loadType)) {
		const shouldPlayNow = !player.playing && !player.paused && !player.queue.size;
		player.queue.add(result.tracks[0]);
		if (!shouldPlayNow) await message.channel.send(embed('BLACK', 'Added to Queue', `[${result.tracks[0].title}](${result.tracks[0].uri})\nArtist • ${result.tracks[0].author}\nRequester • ${message.author.tag}\nDuration • ${duration(result.tracks[0].duration)}`, [], result.tracks[0].displayThumbnail('maxresdefault')));
		else await player.play();
	} else if (result.loadType === 'LOAD_FAILED') {
		await message.channel.send(embed('RED', 'Attachment Play Error', `Failed to load the tracks.`));
	}
}

