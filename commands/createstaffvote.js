const {embed} = require("../utilities/display.js");
const config = require('../config.json');
const {client} = require('..');
const { MessageReaction } = require("discord.js");

exports.name = 'createstaffvote';
exports.type = 'Staff';
exports.info = 'Cast a vote for staff only';
exports.usage = '[desc] [duration]';
exports.alias = ['csv'];
exports.root = false;
exports.mod = true;
exports.admin = false;

exports.run = async({message, args}) => {
    var staff_vote_amount
    const timer = args[0].split(['s', 'm', 'h', 'd'])
    const vote_channel = client.channels.cache.get(config.channels.votes);

    if (staff_vote_amount in global) {
    } else {
        staff_vote_amount = 0
    }

    current_vote_count = staff_vote_amount + 1

    
    switch(timer[1]) {
        case `${timer[0]}`:
        case 's':
            var ftimer = +timer[0] * 1000;
            var embed_time = `${timer[0]} seconds`;
        case 'm':
            var ftimer = +timer[0] * 60000;
            var embed_time = `${timer[0]} minutes`;
        case 'h':
            var ftimer = +timer[0] * 36000000;
            var embed_time = `${timer[0]} hours`;
        case 'd':
            var ftimer = +timer[0] * 864000000;
            var embed_time = `${timer[0]} days`
    }
    var Embed = embed(`BLUE`,`Vote #${current_vote_count} [${embed_time}]`, `${args[1]}`)
    const msg = await vote_channel.send(Embed);
    await msg.react(config.emojis.yes);
    await msg.react(config.emojis.no);

    setTimeout(msg => {
        const yes_count = msg.emoji(id=config.emojis.yes).size - 1;
        const no_count = msg.emoji(id=config.emojis.no).size - 1;

        if (yes_count > no_count) {
            Embed = embed('GREEN', `Vote #${current_vote_count}`, `Approved!`);
        } else if (no_count > yes_count) {
            Embed = embed('RED', `Vote #${current_vote_count}`, `Denied!`)
        } else {
            Embed = embed(`YELLOW`, `Vote #${current_vote_count}`, `Vote Tied!`);
        }
        vote_channel.send(Embed)
    }, ftimer)
}