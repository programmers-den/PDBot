const {client} = require('..');
const config = require('../config.json');

exports.embed = (color, name, text, fields = [], thumbnail) => {
	return {
		embed: {
			author: {
				icon_url: client.user.displayAvatarURL(),
				name
			},
			description: text,
			color, fields,
			footer: {
				text: 'This was sent'
			},
			timestamp: Date.now(),
			thumbnail: {
				url: thumbnail
			}
		}
	}
};

exports.defaultPollOptions = [
	{
		emoji: `<:yes:${config.emojis.yes}>`,
		name: 'Yes',
		count: 0
	},
	{
		emoji: `<:no:${config.emojis.no}>`,
		name: 'No',
		count: 0
	}
];

exports.pollEmbed = (title, options = exports.defaultPollOptions, date = Date.now()) => {
	const result = exports.embed('PURPLE', title, options.map(option => `${option.emoji} • **${option.count} votes** • ${option.name}`).join('\n'));
	result.content = `<@&${config.roles.polls}>`;
	result.embed.timestamp = date;
	return result;
}

exports.format = (text, lang, size) => {
	size -= 8 + lang.length;
	if (text.length > size || !text.length) text = text.slice(0, Math.max(0, size - 3)) + '...';
	return `\`\`\`${lang}\n${text}\n\`\`\``;
};

exports.display = ({name, usage, info}) => `\`${config.prefix + name + (usage.length ? ` ${usage}` : '')}\` • ${info}`;

exports.properCase = text => text.replace(/\w\S*/g, text => text.charAt(0).toUpperCase() + text.slice(1).toLowerCase());