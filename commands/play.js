const { embed, display } = require("../utilities/display.js");
const { client } = require('..')

exports.name = 'play';
exports.type = 'Music';
exports.info = 'Plays Music [Either with search term or link]';
exports.usage = '<yt/youtube/sc/soundcloud/link> <search term/undefined>';
exports.alias = ['p'];
exports.root = false;
exports.admin = false;
exports.mod = false;

exports.run = async({message, args}) => {
    if (!args.length) {
        return message.channel.send(embed("BLACK", "Play Example", "To use this command, pls look at the following fields.", [
            {
                name: "Examples",
                value: [
                    display({
                        name: "play",
                        usage: "play <link>",
                        info: "Play music using links (yt, soundcloud)"
                    }),
                    display({
                        name: 'play',
                        usage: 'play <source -> yt/sc> <terms>',
                        info: "Search for music and play the first search results"
                    })
                ]
            }
        ]))
    }

    let player;  
    if (!client.music.player) {
        player = await client.music.create({
            guild: message.guild.id,
            voiceChannel: message.member.voice.channel.id,
            textChannel: message.channel.id,
            selfMute: false,
            selfDeafen: true,
        })
    }

    if (player.state != 'CONNECTED') {player.connect(); message.channel.send(`Joined \`${message.member.voice.channel.name}\` and binded to \`${message.channel.name}\``)}
    if (player.state == 'CONNECTED' && player.textChannel != message.channel.id && player.voiceChannel != message.member.voice.channel.id) {
        return message.reply(`You're not in the correct text channel/voice channel or both.`)
    }

    let res;
    function isValidURL(string) {
        var result = string.match(/(http(s)?:\/\/.)?(www\.)?[-a-zA-Z0-9@:%._\+~#=]{2,256}\.[a-z]{2,6}\b([-a-zA-Z0-9@:%_\+.~#?&//=]*)/g);
        return (result !== null)
    }

    if (isValidURL(args[0])) {
        res = await player.search(args[0], message.author.id)
    } else if (args[0] == 'yt' || args[0] == 'youtube') {
        res = await player.search({query: `${args.slice(1).join(' ')}`, source: 'youtube'}, message.author.id)
    } else if (args[0]  == 'sc' || args[0] == 'soundcloud') {
        res = await player.search({query: `${args.slice(1).join(' ')}`, source: 'soundcloud'}, message.author.id)
    }

    switch(res.loadType) {
        case 'SEARCH_RESULT':
        case 'TRACK_LOADED':
            player.queue.add(res.tracks[0])
            const e = embed('BLUE', `Added To Queue`, `\`\`Title:\`\` [${res.tracks[0].title}](${res.tracks[0].uri}) \n \`\`Artist:\`\` ${res.tracks[0].author} \n \`\`Requester:\`\` ${message.author.username}`, [], res.tracks[0].displayThumbnail('maxresdefault'))
            await message.channel.send(e)
            // Prevents current playing track gets skipped by queued track
            if (!player.playing && !player.paused && !player.queue.size) {await player.play();}
            break;
        case 'PLAYLIST_LOADED':
            if (!res.playlist) return;
            const em = embed("BLUE", `Added To Queue`, `\`\`Playlist Name:\`\` [${res.playlist.title}](${res.playlist.uri}) \n \`\`Total Tracks:\`\` ${res.tracks.length} \n \`\`Requester:\`\` ${message.author.username}`, [], res.playlist.selectedTrack.displayThumbnail('maxresdefault'))
            message.channel.send(em)
            if (!player.playing && !player.paused && !player.queue.totalSize === res.tracks.length) {await player.play();}
            break;
        case 'NO_MATCHES':
            message.reply(`No matches found for ${res.query}`)
            break;
        case 'LOAD_FAILED':
            message.reply("Failed to load the tracks requested.")
            break;        
    }
}