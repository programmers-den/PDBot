// Setup Client
const {Client, Intents} = require('discord.js');
const client = exports.client = new Client({
	partials: ['CHANNEL', 'MESSAGE', 'REACTION'],
	ws: {
		intents: Intents.ALL
	}
});

// Dependencies
const config = require('./config.json');
const {isRoot, isAdmin, isMod} = require('./utilities/auth');
const {embed, format} = require('./utilities/display');
const {userData, ownedProjects} = require('./utilities/data');
const commands = require('./utilities/commands');

// Execute Scripts
require('./utilities/cleanup');

// Active Executors
const active = [];

// Project Invitation Handler
client.on('messageReactionAdd', async (reaction, user) => {
	if (reaction.partial) try {
		await reaction.fetch();
	} catch {return}
	if (reaction.message.partial) try {
		await reaction.message.fetch();
	} catch {return}
	const data = userData(user.id);
	for (const [messageId, invite] of Object.entries(data.activeInvites)) {
		if (messageId === reaction.message.id) {
			if (reaction.emoji.id === config.emojis.yes) {
				const projects = ownedProjects(invite.owner);
				const project = projects[invite.name];
				const channel = client.channels.cache.get(project.category);
				try {
					await channel.updateOverwrite(user.id, {
						VIEW_CHANNEL: true
					}, 'Added member to project.');
				} catch {return}
				project.members.push(user.id);
				delete data.activeInvites[messageId];
				await reaction.message.edit(embed('PURPLE', 'Accepted Invite', `You accepted the invitation to the \`${invite.name}\` project on **${channel.guild.name}**.`));
			} else if (reaction.emoji.id === config.emojis.no) {
				delete data.activeInvites[messageId];
				await reaction.message.edit(embed('PURPLE', 'Accepted Invite', `You rejected the invitation to the \`${invite.name}\` project on **${channel.guild.name}**.`));
			}
			break;
		}
	}
});

// Command Handler
client.on('message', async message => {
	if (message.author.bot || !message.guild || active.includes(message.author.id)) return;
	let text = message.content;
	const ping = `<@${client.user.id}>`;
	const nick = `<@!${client.user.id}>`;
	if (text.startsWith(config.prefix)) text = text.slice(config.prefix.length).trimLeft();
	else if (text.startsWith(ping)) text = text.slice(ping.length).trimLeft();
	else if (text.startsWith(nick)) text = text.slice(nick.length).trimLeft();
	else return;
	if (!text.length) return;
	const args = text.match(/[^\s\\"]+|"(?:[^"\\]|\\["\\])*"/g) ?? [];
	const name = args.shift().toLowerCase();
	text = text.slice(name.length).trimLeft();
	const texts = [text];
	let textCurrent = text;
	for (const arg of args) {
		textCurrent = textCurrent.slice(arg.length).trimLeft();
		if (textCurrent.length) texts.push(textCurrent);
	}
	let command;
	for (const [fileName, commandData] of Object.entries(commands)) {
		if (fileName === name || commandData.alias.includes(name)) {
			command = commandData;
			break;
		}
	}
	if (command === undefined) return;
	const root = await isRoot(message.author.id);
	const mod = await isMod(message.author.id);
	const admin = await isAdmin(message.author.id);
	if (command.root && !root) return await message.channel.send(embed('RED', 'Permission Denied', 'You must be an authorized root user to access this command.'));
	if (command.mod && !mod) return await message.channel.send(embed('RED', 'Permission Denied', 'You must be an authorized moderator to access this command.'));
	if (command.admin && !admin) return await message.channel.send(embed('RED', 'Permission Denied', 'You must be an authorized administrator to access this command.'));
	active.push(message.author.id);
	try {
		await command.run({name, args: args.map(arg => {
			if (arg.startsWith('"') && arg.endsWith('"')) return arg.slice(1, -1).replace(/\\([\\"])/g, '\1');
			else return arg;
		}), text, texts, command, root, mod, message, prefix: config.prefix});
		active.splice(active.indexOf(message.author.id), 1);
	} catch(error) {
		active.splice(active.indexOf(message.author.id), 1);
		console.log(error);
		await message.channel.send(embed('RED', 'Command Error', format(error.message, '', 2000)));
	}
});

// Ready Event
client.once('ready', async () => {
	await client.user.setPresence({
		activity: {
			name: 'you type ./help',
			type: 'LISTENING'
		},
		status: 'online'
	});
	console.log(`${client.user.tag} is now online and ready.`);
});

// Client Login
client.login(config.token);