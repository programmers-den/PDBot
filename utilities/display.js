const {client} = require('..');
const config = require('../config.json');

exports.embed = (color, name, text, fields = []) => {
	return {
		embed: {
			author: {
				icon_url: client.user.displayAvatarURL(),
				name
			},
			description: text,
			color, fields,
			footer: {
				text: 'This was sent on'
			},
			timestamp: Date.now()
		}
	}
};

exports.format = (text, lang, size) => {
	size -= 8 + lang.length;
	if (text.length > size || !text.length) text = text.slice(0, Math.max(0, size - 3)) + '...';
	return `\`\`\`${lang}\n${text}\n\`\`\``;
};

exports.display = ({name, usage, info}) => `\`${config.prefix + name + (usage.length ? ` ${usage}` : '')}\` • ${info}`;

exports.properCase = text => text.replace(/\w\S*/g, text => text.charAt(0).toUpperCase() + text.slice(1).toLowerCase());