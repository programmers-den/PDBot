const {embed, format} = require('../utilities/display');
const util = require('util');

exports.name = 'eval';
exports.type = 'Restricted';
exports.info = 'Executes a snippet of code.';
exports.usage = '[text]';
exports.alias = [];
exports.root = true;
exports.mod = false;
exports.admin = false;

exports.run = async ({message, text}) => {
	try {
		result = eval(text);
		if (result instanceof Promise) result = await result;
		const type = format(typeof result, '', 1024);
		const output = format(util.inspect(result), 'js', 1024);
		await message.channel.send(embed('GREEN', 'Script Result', undefined, [
			{
				name: 'Output',
				value: output
			},
			{
				name: 'Type',
				value: type
			}
		]));
	} catch(error) {
		const content = format(util.inspect(error), '', 1024);
		await message.channel.send(embed('RED', 'Script Result', undefined, [
			{
				name: 'Error',
				value: content
			}
		]));
	}
};