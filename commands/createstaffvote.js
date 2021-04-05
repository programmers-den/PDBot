const {embed, defaultPollOptions} = require("../utilities/display.js");
const config = require('../config.json');
const {client} = require('..');

exports.name = 'createstaffvote';
exports.type = 'Moderation';
exports.info = 'Cast a vote for staff only';
exports.usage = '[x minutes] [desc]';
exports.alias = ['csv'];
exports.root = false;
exports.mod = true;
exports.admin = false;

exports.run = async({message, args}) => {
    let timeout;
    const options = [`Yes`, `No`];
    const msg_channel = client.channels.cache.get(config.channels.votes);
    var main_text = '';
    
    const defEmojiID = [
        config.emojis.yes,
        config.emojis.no,
    ];

    const emojiList = [];

    for (const emName of defEmojiID) {
        const emoji = client.emojis.cache.get(emName)
        emojiList.push(emoji)
    }

    const emojiInfo = {};

    for (const option of options) {
        const emoji = emojiList.splice(0, 1);
        emojiInfo[emoji] = {option: option, votes: 0};
        main_text += `${emoji} : \`${option}\`\n\n`;
    };
    
    
    const afterEmojiList = Object.keys(emojiInfo);

    const poll = await msg_channel.send(embed(`BLUE`, `${args[1]}`, `${main_text}`));
    for(const emojis of afterEmojiList) await poll.react(emojis);

    const reactionCollector = await poll.createReactionCollector(
        (reaction, user) => afterEmojiList.includes(reaction.emoji.name) && !user.bot,
        timeout === 0 ? {} : {time: args[0] * 60000}
    );

    const voterInfo = new Map();

    reactionCollector.on('collect', (reaction, user) => {
        if (afterEmojiList.includes(reaction.emoji.name)) {
            if (!voterInfo.has(user.id)) voterInfo.set(user.id, {emoji: reaction.emoji.name});
            const votedEmoji = voterInfo.get(user.id).emoji;
            if (votedEmoji !== reaction.emoji.name) {
                const lastVote = poll.reactions.get(votedEmoji);
                lastVote.count -= 1;
                lastVote.users.remove(user.id);
                emojiInfo[votedEmoji].votes -= 1;
                voterInfo.set(user.id, {emoji: reaction.emoji.name});
            }
            emojiInfo[reaction.emoji.name].votes += 1;
        }
    });

    reactionCollector.on('dispose', (reaction, user) => {
		if (afterEmojiList.includes(reaction.emoji.name)) {
			voterInfo.delete(user.id);
			emojiInfo[reaction.emoji.name].votes -= 1;
		}
	});

    reactionCollector.on('end', async () => {
		var end_text = `Time's up for [this vote](${poll.url})!\n Results are in,\n\n`
		for (const emoji in emojiInfo) end_text += `\`${emojiInfo[emoji].option}\` - \`${emojiInfo[emoji].votes}\`\n\n`;
        await poll.reply(embed(`YELLOW`, `Vote Ended`, `${end_text}`));
	});
}