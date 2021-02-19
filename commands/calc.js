const {embed, display} = require('../utilities/display');
const { isMod } = require("../utilities/auth")
const config = require('../config.json');
const { client } = require('..');
const math = require('mathjs');

exports.name = 'announce';
exports.type = 'cacl';
exports.info = 'Math calculation';
exports.usage = '0 + 1 + 3';
exports.alias = [];
exports.root = false;
exports.mod = false;
exports.admin = false;

exports.run = async ({message, args}) => {
    if (!args.length) {
        return message.channel.send(embed("RED", "Math Example", "To use this command, you write the operands and values.\n\nBelow are some examples on how to use the math command.", [
            {
                name: 'Plus Example',
                value: display({
                    name: 'cacl',
                    usage: '10 + 5',
                    info: 'Plus 10 with 5 (+), result, 15',
                }),
            },
            {
                name: `Minus Example`,
                value: display({
                    name: `cacl`,
                    usage: `10 - 5`,
                    info: `Minus 10 with 5 (-), result 5`,
                }),
            },
            {
                name: `Multiply Example`,
                value: display({
                    name: 'cacl',
                    usage: `4 * 1`,
                    info: `Multiply Number by 1 (*), result 4`,
                }),
            },
            {
                name: `Divide Example`,
                value: display({
                    name: `cacl`,
                    usage: `4 / 2`,
                    info: `Divide 4 by 2 (/), result 2`,
                }),
            },
            {
                name: `Prioritize Example`,
                value: display({
                    name: `cacl`,
                    usage: `4 / 2 (10 - 5)`,
                    info: `Do minus then divide 4 by 2 and multiply the value inside bracket () > [] > {} , result 4`,
                }),
            },
            {
                name: `Power Example`,
                value: display({
                    name: `cacl`,
                    usage: `2^4`,
                    info: `2 are multiplied by 4 times as 4 is a power (^), result 16`,
                }),
            },
        ]))
    }
    try {
        message.channel.send(embed("BLUE", "Math Calculation:", `Input:\n\`\`\`${args.join(' ')}\`\`\`\nOutput: \`\`\`${math.evaluate(args.join(' '))}\`\`\``))
            .then(msg => msg.delete({timeout: 15000}));
    } catch (err) {
        message.channel.send(embed("RED", `Math Error`, `\`\`\`${err.message}\`\`\``))
            .then(msg => msg.delete({timeout: 10000}));
    }
};