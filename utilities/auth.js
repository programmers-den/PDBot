const {client} = require('..');
const config = require('../config.json');

exports.isRoot = async id => {
	const guild = client.guilds.cache.get(config.guild);
	const member = await guild.members.fetch(id);
	return member.roles.cache.has(config.roles.root);
};

exports.isAdmin = async id => {
	const guild = client.guilds.cache.get(config.guild);
	const member = await guild.members.fetch(id);
	return member.roles.cache.has(config.roles.admin) || (await exports.isRoot(id));
};

exports.isMod = async id => {
	const guild = client.guilds.cache.get(config.guild);
	const member = await guild.members.fetch(id);
	return member.roles.cache.has(config.roles.mod) || (await exports.isAdmin(id));
};