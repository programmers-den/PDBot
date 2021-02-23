const {embed} = require("../utilities/display.js");
const config = require('../config.json');
const {client} = require('..');

exports.name = 'createstaffvote';
exports.type = 'Staff';
exports.info = 'Cast a vote for staff only';
exports.usage = '[duration (number)] [-tags(s, m , h, d)| = Sets what duration, default min] [duration]';
exports.alias = ['csv'];
exports.root = false;
exports.mod = true;
exports.admin = false;

exports.run = async({message, args}) => {
    const msg_channel = client.channels.cache.get(config.channels.votes);
    global.staff_vote_amount = null;
    if (global.staff_vote_amount == isNaN(global.staff_vote_amount)) {} else staff_vote_amount = 0

    var current_vote_count = staff_vote_amount + 1
    if (!(args[2] in ["-s", '-h', '-m', '-d'])) {
        var desc = args.slice(1).join(' ');
    } else {
        var desc = args.slice(2).join(' ');
    }
        var timer = args[0]
    let timeout;

    if (args[2].split("-")) {
        switch(args[2]) {
            case `${args[2].split("-")}`:
            case 's':
                timer = `${timer[0]} seconds`;
                timeout = +timer[0] * 1000;
                break;
            case 'm':
                timer = `${timer[0]} minutes`;
                timeout = +timer[0] * 60000;
                break;
            case 'h':
                timer = `${timer[0]} hours`;
                timeout = +timer[0] * 360000;
                break;
            case 'd':
                timer = `${timer[0]} days`;
                timeout = +timer[0] * 86400000;
                break;
        };
    } else {
        timer = `${timer} minutes`
        timeout = +timer * 60 * 1000
    }

    const Embed = embed('BLUE', `Vote #${current_vote_count} [${timer}]`, `${desc}`);

    staff_vote_amount += 1;
    const v_message = await msg_channel.send(`|| ||`,Embed);
    await v_message.react(config.emojis.yes);
    await v_message.react(config.emojis.no);
    setTimeout(() => {
        const yes_count = v_message.reactions.cache.get(config.emojis.yes).size - 1
        const no_count = v_message.reactions.cache.get(config.emojis.no).size - 1

        if (yes_count > no_count) {
            const fEmbed = embed("GREEN", `Vote #${current_vote_count}`, `Approved!`);
            msg_channel.send(fEmbed);
        } else if (no_count > yes_count) {
            const fEmbed = embed("RED", `Vote #${current_vote_count}`, `Denied!`);
            msg_channel.send(fEmbed);
        } else if (yes_count == no_count) {
            const fEmbed = embed("YELLOW", `Vote #${current_vote_count}`, `Vote Tied!`);
            msg_channel.send(fEmbed);
        }
        staff_vote_amount -= 1
    }, timeout);
}