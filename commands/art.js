const {client} = require('..');
const figlet = require('figlet');
const {embed, display} = require('../utilities/display')

exports.name = 'art';
exports.type = 'Fun';
exports.info = 'You know, ascill art :)';
exports.usage = '[font] [content]';
exports.alias = [''];
exports.root = false;
exports.admin = false;
exports.mod = false;

exports.run = async ({message, args}) => {
    if (!args.length) {
        return await message.channel.send(embed('RED', 'Art Example', "To use this command, enter the font followed by a list of art fonts. Each argument can be wrapped in quotes if it doesn't fit in a single word. Below are some examples on how to use the art command.", [
			{
				name: 'Example one word font',
				value: display({
					name: 'art',
					usage: 'Ghost "xD my guy"',
					info: 'Uses the art font Ghost'
				})
			},
			{
				name: 'Example x > 1 word font',
				value: display({
					name: 'art',
					usage: '"AMC Slash" "xD my guy"',
					info: 'Uses the art font AMC SLASH'
				})
			}
		]));
    }

    try {
        await message.channel.send(embed('BLUE', ``, `\`\`\`${figlet.textSync(`${args[1]}`, {font: `${args[0]}`, horizontalLayout: 'full'})}\n\`\`\``));
    } catch (error) {
        await message.channel.send(embed('RED', `Art Error`, `Oops, Looks like an error occured.\n\n` + `\`\`\`${error.message}\`\`\``));
    }
}