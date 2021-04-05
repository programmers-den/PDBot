const {client} = require('..');
const {embed} = require('../utilities/display');
const fs = require('fs');
const { compareText } = require('mathjs');

exports.name = "cat";
exports.type = "Restricted";
exports.info = `Display content of a file`;
exports.usage = "[file]"
exports.alias = [''];
exports.root = true;
exports.admin = false;
exports.mod = false;

exports.run = async ({message, args}) => {
    if (args.length == 0) { args = './'}
    else if (args[0].startsWith('./')) {}
    else {args = `./${args}`};

    try {
        split_dir = args.split('/');
        for(const i in ['..', 'config.json', `data`]) {
            if (i in split_dir) {
                await message.channel.send(embed(`RED`, `CAT Error`, `You may not cat this file!`))
                return;
            };
        }

        var lang = ``;
        switch(split_dir[-1].split('.')[-1]) {
            case 'py':
                lang = `py`
                break;
            case 'js':
                lang = `js`
                break;
            case 'json':
                lang = `json`
                break;
            case 'sh':
                lang = `sh`
                break;
        }

        const xg = fs.readFileSync(`${args}`);
        if (xg.length < 2000) {
            await message.channel.send(embed(`BLUE`, `Content in ${args}`, `\`\`\`${lang}\n${xg}\`\`\``))
        } else {
            const xg1 = fs.writeFileSync(`./output.txt`, `${xg}`);
            await message.channel.send(embed(`BLUE`, `Content was too long!`, `File content is over 2000, sending as output.txt`), {files: [`./output.txt`]})
            fs.unlinkSync(`./output.txt`)
        }
    } catch (err) {
        await message.channel.send(embed(`RED`, `CAT Error`, `File Doesn't Exist!`))
    }
    
}