const {embed, display} = require('../utilities/display');

exports.name = 'purge';
exports.type = 'Moderation';
exports.info = 'Clears messages in bulk.';
exports.usage = 'help';
exports.alias = ['clean', 'clear', 'bulkdelete'];
exports.root = false;
exports.mod = true;
exports.admin = false;

exports.run = async ({message, args}) => {
	if (!args.length || args.length === 1 && args[0] === 'help') {
		await message.channel.send(embed('BLUE', 'Purge Syntax', 'You can provide any of the following as an argument to the purge command and it will be used as a modifier when bulk deleting.', [
			{
				name: 'Amount',
				value: 'Provide a number of messages that will be filtered through and deleted.'
			},
			{
				name: 'User',
				value: 'Provide a user mention or id and messages by that user will be deleted.'
			},
			{
				name: 'Channel',
				value: 'Provide a channel mention and messages in that channel will be deleted.'
			},
			{
				name: 'Role',
				value: 'Provide a role mention and messages by users with that role will be deleted.'
			},
			{
				name: 'Message',
				value: 'Provide a message link (not an id) and messages starting from that position will be deleted.'
			},
			{
				name: 'Example',
				value: display({
					name: 'purge',
					usage: '30 332914508395839490',
					info: 'Purges 30 messages by Rigidity#0001'
				})
			}
		]));
	} else {
		await message.channel.send(embed('RED', 'Project Error', 'That is an unsupported subcommand.'));
	}
};