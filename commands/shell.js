const {embed, format} = require('../utilities/display');
const {execSync} = require('child_process');

exports.name = 'shell';
exports.type = 'Restricted';
exports.info = 'Executes a shell script.';
exports.usage = '[text]';
exports.alias = ['sh'];
exports.root = true;
exports.mod = false;
exports.admin = false;

exports.run = async ({message, text}) => {
	try {
		result = execSync(text).toString();
		const output = format(result, '', 1024);
		await message.channel.send(embed('GREEN', 'Shell Result', undefined, [
			{
				name: 'Output',
				value: output
			}
		]));
	} catch(error) {
		const content = format(error.message, '', 1024);
		await message.channel.send(embed('RED', 'Shell Result', undefined, [
			{
				name: 'Error',
				value: content
			}
		]));
	}
};