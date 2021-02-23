const {embed} = require("../utilities/display.js");
const config = require('../config.json');
const {client} = require('..');

exports.name = 'createstaffvote';
exports.type = 'Staff';
exports.info = 'Cast a vote for staff only';
exports.usage = '[duration (number)] [desc]';
exports.alias = ['csv'];
exports.root = false;
exports.mod = true;
exports.admin = false;

exports.run = async({message, args}) => {
    const msg_channel = client.channels.cache.get(config.channels.votes);
    global.staff_vote_amount = '';
    if (staff_vote_amount in global) {null} else {staff_vote_amount = 0}

    var current_vote_count = staff_vote_amount + 1
    var timer = args[0].split(/(?=[smhd])/);
    const timer_substring = timer[1]
    var desc = args.slice(1).join(' ');
    var timeout;

    console.log(timer)
    switch(timer_substring) {
        default:
            return message.channel.send("Invalid time given")
        case 's':
            timeout = +timer[0] * 1000
            timer = `${timer[0]} seconds`
            break;
        case 'm':
            timeout = +timer[0] * 60000
            timer = `${timer[0]} minutes`
            break;
        case 'h':
            timeout = +timer[0] * 360000
            timer = `${timer[0]} hours`
            break;
        case 'd':
            timeout = timer[0] * 86400000
            timer = `${timer[0]} days`;
            break;
    }

    const Embed = embed('BLUE', `Vote #${current_vote_count} [${timer}]`, `${desc}`);

    staff_vote_amount += 1;

    const v_message = await msg_channel.send(`|| ||`,Embed);
    await v_message.react(config.emojis.yes);
    await v_message.react(config.emojis.no);
    console.log(timeout)
    setTimeout( () => {
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