const {embed, display} = require('../utilities/display');
const config = require('../config.json');
const fs = require('fs')

exports.name = 'listdir';
exports.type = 'Restricted';
exports.info = 'List Directory';
exports.usage = '[directory]';
exports.alias = ["ls"];
exports.root = false;
exports.mod = false;
exports.admin = false;

exports.run = async ({message, args}) => {
    try {
        if (!args.length) {args = '.'}
        else {args = args};
        let x = ''
        const split_dir = args.toString().split('/');
        for (const i of [`..`]) {
            if (i in split_dir) {
                await message.channel.send(embed("RED", `Warning ❗`, `You may not ls this directory!`))
                return;
            }
        }
        for (const i of fs.readdirSync(...args)) {
            if (!(i.toLowerCase() in ['config.json'])) {
                x = `${x}${i}\n`
            }
        }
        await message.channel.send(embed("BLUE", `List of files and folder in \`${args}\``, `\`\`\`\css\n${x}\`\`\``))
    } catch (err) {
        await message.channel.send(embed("RED", "Error! ⚠️", `Directory doesn't exist! \`\`${err.message}\`\``))
    };
};