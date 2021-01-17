const emojiRegex = require('emoji-regex');

exports.regex = {
	user: /^<@!?([0-9]+)>$/,
	channel: /^<#([0-9]+)>$/,
	role: /^<@&([0-9]+)>$/,
	emoji: /^<a?:[^:]+:([0-9]+)>$/,
	id: /^([0-9]+)$/
};

exports.fetchUnicodeEmoji = text => {
	const regex = emojiRegex();
	let match;
	const matches = [];
	while (match = regex.exec(text)) {
		matches.push(match[0]);
	}
	return matches.length == 1 && matches[0] == text ? matches[0] : null;
};

exports.fetchCustomEmoji = (text, guild) => {
	const matches = text.match(exports.regex.emoji) || text.match(exports.regex.id);
	return matches ? guild.emojis.cache.get(matches[1]) : null;
};

exports.fetchEmoji = (text, guild) => {
	const unicodeEmoji = exports.unicodeEmoji(text);
	return unicodeEmoji == null ? exports.customEmoji(text, guild) : ue;
};

exports.fetchChannel = (text, guild) => {
	const matches = text.match(exports.regex.channel) || text.match(exports.regex.id);
	return matches ? guild.channels.cache.get(matches[1]) : guild.channels.cache.get(text);
};

exports.fetchMember = async (text, guild) => {
	const matches = text.match(exports.regex.user) || text.match(exports.regex.id);
	if (matches) {
		try {
			return await guild.members.fetch(matches[1]);
		} catch {}
	}
	return guild.members.cache.find(member => member.user.tag === text || member.user.username === text) ?? null;
};

exports.fetchRole = async (text, guild) => {
	const matches = text.match(exports.regex.role) || text.match(exports.regex.id);
	if (matches) {
		try {
			return await guild.roles.fetch(matches[1]);
		} catch {}
	}
	return guild.roles.cache.find(role => role.name === text) ?? null;
};

exports.fetchUser = async (text, bot) => {
	const matches = text.match(exports.regex.user) || text.match(exports.regex.id);
	if (matches) {
		try {
			return await bot.users.fetch(matches[1]);
		} catch {}
	}
	return bot.users.cache.find(user => user.tag === name || user.username === name) ?? null;
};