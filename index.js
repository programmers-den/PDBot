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
const {embed, format, display} = require('./utilities/display');
const {userData, ownedProjects, allProjects} = require('./utilities/data');
const commands = require('./utilities/commands');

// Execute Scripts
require('./utilities/cleanup');

// Active Executors
const active = [];

// Music Node Init
const { Manager } = require("erela.js");
client.Music = new Manager({
	nodes: [{
		host: config.lavalink.host,
		port: parseInt(config.lavalink.port),
		password: config.lavalink.pass,
		identifier: 'Cheeki',
	}],
	send(id, payload) {
		const guild = client.guilds.cache.get(id)
		if (guild) guild.shard.send(payload)
	},
})
	.on('nodeConnect', node => console.log(`Connected to NODE: ${node.options.identifier}`))
	.on('nodeError', (node, err) => console.log(`Falied to connect to ${node.options.identifier} with error:\n${err.message}`))
	.on('trackStart', (player, track) => {
		let embed = embed('BLACK', 'Now Playing...', undefined, [
			{
				name: undefined,
				value: [
				display({
					name: `Title`,
					usage: `[${track.title}](${track.uri})`
				}),
				display({
					name: `Requester`,
					usage: `${track.requester}`
				}),
				]
			}
		], track.displayThumbnail('maxresdefault'))
		client.channels.cache
			.get(player.textChannel)
			.send(embed);
	})
	.on(`queueEnd`, (player) => {
		client.LavaQueueTimeout = setTimeout(() => {
			if (player.queue.length != 0 || player.queue.length == 0 && player.queue.current) {return;}
			else {
				embed = embed('BLACK', undefined, "Looks like 10 minutes has passed and I'm not playing any music. Disconnecting to save bandwidth.")
				client.channels.cache
					.get(player.textChannel)
					.send(embed);
			}
		}, 600000)
	});
// Music event
client.on('raw', (d) => {client.Music.updateVoiceState(d)})

// Suggestion Channel Message
client.on('message', async message => {
	if (message.channel?.id !== config.channels.suggestions || message.author.bot) return;
	await message.delete();
	const content = embed('PURPLE', `Suggestion: ${message.author.tag}`, message.content).embed;
	content.author.icon_url = message.author.displayAvatarURL({
		dynamic: true
	});
	const replacement = await message.channel.send({embed: content});
	await replacement.react(config.emojis.yes);
	await replacement.react(config.emojis.no);
});

// Counter Channel Message
client.on('message', async message => {
	if (message.channel?.id !== config.channels.counting) return;
	const messages = await message.channel.messages.fetch({
		limit: 1,
		before: message.id
	});
	const previous = +messages.array().map(message => message.content)[0];
	if (isNaN(previous) || previous <= 0 || Math.floor(previous) !== previous || previous === Infinity || previous === -Infinity) return;
	const next = (previous + 1) + '';
	if (message.content != next) await message.delete().catch(() => {});
});

// Counter Channel Edit
client.on('messageUpdate', async (_, message) => {
	if (message.channel?.id !== config.channels.counting) return;
	const messages = await message.channel.messages.fetch({
		limit: 1,
		before: message.id
	});
	const previous = +messages.array().map(message => message.content)[0];
	if (isNaN(previous) || previous <= 0 || Math.floor(previous) !== previous || previous === Infinity || previous === -Infinity) return;
	const next = (previous + 1) + '';
	if (message.content !== next) await message.delete().catch(() => {});
});

// Project Member Remover
client.on('guildMemberRemove', async member => {
	const projects = allProjects();
	for (const [name, project] of Object.entries(projects)) {
		if (project.owner === member.user.id) {
			const owned = ownedProjects(project.owner);
			delete owned[name];
		} else if (project.members.includes(member.user.id)) {
			project.members.splice(project.members.indexOf(member.user.id), 1);
		} else if (project.developers.includes(member.user.id)) {
			project.developers.splice(project.developers.indexOf(member.user.id), 1);
		}
	}
});

// Project Channel Adder
client.on('channelCreate', async channel => {
	if (channel.parentID === null || !['text', 'voice'].includes(channel.type)) return;
	const projects = allProjects();
	for (const project of Object.values(projects)) {
		if (channel.parentID === project.category) {
			project.channels.push(channel.id);
		}
	}
});

// Project Channel Remover
client.on('channelDelete', async channel => {
	if (channel.parentID === null || !['text', 'voice', 'category'].includes(channel.type)) return;
	const projects = allProjects();
	for (const [name, project] of Object.entries(projects)) {
		console.log(project, channel.id, channel.type);
		if (project.category === channel.id) {
			const owned = ownedProjects(project.owner);
			delete owned[name];
		} else if (project.channels.includes(channel.id)) {
			project.channels.splice(project.channels.indexOf(channel.id), 1);
		}
	}
});

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

// Ready Event + Manager Init
client.once('ready', async () => {
	await client.user.setPresence({
		activity: {
			name: 'you type ./help',
			type: 'LISTENING'
		},
		status: 'online'
	});
	console.log(`${client.user.tag} is now online and ready.`);
	client.Music.init(client.user.id)
});

// Client Login
client.login(config.token);