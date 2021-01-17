const {embed, display} = require('../utilities/display');
const commands = require('../utilities/commands');

exports.name = 'help';
exports.type = 'Information';
exports.info = 'Displays the list of commands.';
exports.usage = '';
exports.alias = [];
exports.root = false;
exports.mod = false;
exports.admin = false;

exports.run = async ({message}) => {
	const array = Object.values(commands).slice().sort((a, b) => a.name < b.name ? -1 : (a.name > b.name ? 1 : 0));
	const fields = {};
	for (let i = 0; i < array.length; i++) {
		const command = array[i];
		if (command.perms === null) continue;
		if (fields[command.type] === undefined) {
			fields[command.type] = [];
		}
		fields[command.type].push(command);
	}
	if (!Object.keys(fields).length) return await message.channel.send(embed('RED', 'Help Error', 'There are no commands to be displayed.'));
	await message.channel.send(embed('BLUE', 'PD Commands', undefined, Object.entries(fields).map(field => {
		return {
			name: field[0],
			value: field[1].map(display)
		};
	})));
};