const {embed, display, properCase} = require('../utilities/display');
const {userData, ownedProjects, projectExists, allProjects, projectStatus} = require('../utilities/data');
const {fetchMember} = require('../utilities/convert');
const config = require('../config.json');

exports.name = 'project';
exports.type = 'Utilities';
exports.info = 'Manages your projects.';
exports.usage = 'help';
exports.alias = ['proj'];
exports.root = false;
exports.mod = false;
exports.admin = false;

exports.run = async ({message, args, texts, prefix}) => {
	if (!args.length || args.length === 1 && args[0].toLowerCase() === 'help') {
		// This is the help page for the command.
		await message.channel.send(embed('BLUE', 'Project Commands', undefined, [
			{
				name: 'Management',
				value: [
					display({
						name: 'project create',
						usage: '[name] [text]',
						info: 'Creates a project.'
					}),
					display({
						name: 'project delete',
						usage: '[project]',
						info: 'Deletes a project.'
					}),
					display({
						name: 'project rename',
						usage: '[project] [name]',
						info: "Changes a project's name."
					}),
					display({
						name: 'project description',
						usage: '[project] [text]',
						info: "Changes a project's description."
					}),
					display({
						name: 'project list',
						usage: '[user?]',
						info: "Displays a user's projects."
					}),
					display({
						name: 'project mode',
						usage: '(private|public)?',
						info: 'Configures the project privacy.'
					})
				].join('\n')
			},
			{
				name: 'Members',
				value: [
					display({
						name: 'project invite',
						usage: '[project] [user]',
						info: 'Invites a member to the project.'
					}),
					display({
						name: 'project kick',
						usage: '[project] [user]',
						info: 'Kicks a member from the project.'
					}),
					display({
						name: 'project promote',
						usage: '[project] [user]',
						info: 'Adds permissions to a member.'
					}),
					display({
						name: 'project demote',
						usage: '[project] [user]',
						info: 'Removes permissions from a member.'
					}),
					display({
						name: 'project members',
						usage: '[project]',
						info: 'Displays a list of members in the project.'
					})
				].join('\n')
			},
			{
				name: 'Channels',
				value: [
					display({
						name: 'project add',
						usage: '[project] [name]',
						info: 'Adds a text channel to the project.'
					}),
					display({
						name: 'project vc',
						usage: '[project] [name]',
						info: 'Adds a voice channel to the project.'
					}),
					display({
						name: 'project remove',
						usage: '[project] [channel]',
						info: 'Removes a channel from the project.'
					}),
					display({
						name: 'project edit',
						usage: '[project] [channel] [name]',
						info: 'Edits a channel name for the project.'
					}),
					display({
						name: 'project channels',
						usage: '[project]',
						info: 'Displays a list of channels in the project.'
					}),
					display({
						name: 'project remove',
						usage: '[project] [channel]',
						info: 'Removes a channel from the project.'
					}),
					display({
						name: 'project move',
						usage: '[project] [channel] [amount]',
						info: 'Moves a channel down.'
					})
				].join('\n')
			},
			{
				name: 'Personal',
				value: [
					display({
						name: 'project join',
						usage: '[project]',
						info: 'Joins a public project.'
					}),
					display({
						name: 'project leave',
						usage: '[project]',
						info: 'Leaves a project you are in.'
					}),
					display({
						name: 'project all',
						usage: '',
						info: 'Displays a list of projects.'
					})
				].join('\n')
			}
		]));
	} else if (args.length === 1 && args[0].toLowerCase() === 'all') {
		// Displays a list of projects.
		const projects = allProjects();
		await message.channel.send(embed('BLUE', 'Project List', Object.entries(projects).map(entry => `\`${entry[0]}\` **${entry[1].invite_only ? 'Private' : 'Public'}** • ${entry[1].description}`).join('\n')));
	} else if (args.length === 2 && args[0].toLowerCase() === 'join') {
		// Adds you to a public project.
		const project = args[1];
		const projects = allProjects();
		const content = projects[project];
		if (content === undefined) return await message.channel.send(embed('RED', 'Project Error', 'There is no project with that name.'));
		if (message.author.id === content.owner) return await message.channel.send(embed('RED', 'Join Error', 'You are already the owner of that project.'));
		if (content.developers.includes(message.author.id)) return await message.channel.send(embed('RED', 'Join Error', 'You are already a developer of that project.'));
		if (content.members.includes(message.author.id)) return await message.channel.send(embed('RED', 'Join Error', 'You are already a member of that project.'));
		if (content.invite_only) return await message.channel.send(embed('RED', 'Join Error', 'That project is only accepting new members by invitation.'));
		const category = message.guild.channels.cache.get(content.category);
		await category.updateOverwrite(message.author.id, {
			VIEW_CHANNEL: true
		}, 'Member joined the project.');
		content.members.push(message.author.id);
		await message.channel.send(embed('GREEN', 'Joined Project', `You have successfully joined \`${project}\` and are now a member of the project.`));
	} else if (args.length === 2 && args[0].toLowerCase() === 'leave') {
		// Removes you from a public project.
		const project = args[1];
		const projects = allProjects();
		const content = projects[project];
		if (content === undefined) return await message.channel.send(embed('RED', 'Project Error', 'There is no project with that name.'));
		if (message.author.id === content.owner) return await message.channel.send(embed('RED', 'Leave Error', 'You are are the owner of that project.'));
		if (content.developers.includes(message.author.id)) {
			content.developers.splice(content.developers.indexOf(message.author.id), 1);
		} else if (content.members.includes(message.author.id)) {
			content.members.splice(content.members.indexOf(message.author.id), 1);
		} else return await message.channel.send(embed('RED', 'Leave Error', 'You are not a member or developer of that project.'));
		const category = message.guild.channels.cache.get(content.category);
		await category.updateOverwrite(message.author.id, {
			VIEW_CHANNEL: null,
			USE_VAD: null,
			MANAGE_WEBHOOKS: null,
			MANAGE_MESSAGES: null
		}, 'Member left the project.');
		await message.channel.send(embed('GREEN', 'Left Project', `You have successfully left \`${project}\` and are no longer a member of the project.`));
	} else if (args.length === 3 && args[0].toLowerCase() === 'invite') {
		// Invites a member to a project.
		const project = args[1];
		const member = await fetchMember(args[2], message.guild);
		if (member === null) return await message.channel.send(embed('RED', 'Unknown Member', 'The member you specified could not be found on this server.'));
		if (member.user.id === message.author.id) return await message.channel.send(embed('RED', 'Invite Error', 'You cannot invite yourself to the project.'));
		const projects = allProjects(message.author.id);
		const content = projects[project];
		if (content === undefined) return await message.channel.send(embed('RED', 'Project Error', 'You do not have a project with that name.'));
		if (content.developers.includes(member.user.id)) return await message.channel.send(embed('RED', 'Project Error', `The user **${member.user.tag}** is already a developer of the project \`${project}\`.`));
		if (content.members.includes(member.user.id)) return await message.channel.send(embed('RED', 'Project Error', `The user **${member.user.tag}** is already a member of the project \`${project}\`.`));
		if (projectStatus(message.author.id, content) === 'member') return await message.channel.send(embed('RED', 'Permission Denied', 'You do not have permission to invite members to that project.'));
		const user = userData(member.user.id);
		for (const entry of Object.entries(user.activeInvites)) {
			if (entry.owner === message.author.id && entry.name === project) return await message.channel.send(embed('RED', 'Invite Error', 'That user has already been invited to the project.'));
		}
		let invitation;
		try {
			invitation = await member.user.send(embed('PURPLE', `Invited: ${project}`, `You have been invited to join the project \`${project}\` on **${message.guild.name}**. React below to accept or decline the invitation.`));
			await invitation.react(config.emojis.yes);
			await invitation.react(config.emojis.no);
		} catch {
			return await message.channel.send(embed('RED', 'Invite Error', 'Could not invite that user to the project because their direct messages are disabled or they blocked the bot.'));
		}
		user.activeInvites[invitation.id] = {
			owner: message.author.id,
			name: project
		};
		await message.channel.send(embed('GREEN', 'User Invited', `You have successfully invited **${member.user.tag}** to the project \`${project}\`.`));
	} else if (args.length === 3 && args[0].toLowerCase() === 'promote') {
		// Promotes a member in a project.
		const project = args[1];
		const member = await fetchMember(args[2], message.guild);
		if (member === null) return await message.channel.send(embed('RED', 'Unknown Member', 'The member you specified could not be found on this server.'));
		if (member.user.id === message.author.id) return await message.channel.send(embed('RED', 'Promote Error', 'You cannot promote yourself in the project.'));
		const projects = allProjects(message.author.id);
		const content = projects[project];
		if (content === undefined) return await message.channel.send(embed('RED', 'Project Error', 'You do not have a project with that name.'));
		if (projectStatus(message.author.id, content) !== 'owner') return await message.channel.send(embed('RED', 'Permission Denied', 'You do not have permission to promote members in that project.'));
		if (!content.members.includes(member.user.id)) return await message.channel.send(embed('RED', 'Project Error', `The user **${member.user.tag}** is not a member in the project \`${project}\`.`));
		const category = message.guild.channels.cache.get(content.category);
		await category.updateOverwrite(member.user.id, {
			USE_VAD: true,
			MANAGE_WEBHOOKS: true,
			MANAGE_MESSAGES: true
		}, 'Member promoted in the project.');
		content.members.splice(content.members.indexOf(member.user.id), 1);
		content.developers.push(member.user.id);
		await message.channel.send(embed('GREEN', 'User Promoted', `You have successfully promoted **${member.user.tag}** in the project \`${project}\`.`));
	} else if (args.length === 3 && args[0].toLowerCase() === 'demote') {
		// Demotes a member in a project.
		const project = args[1];
		const member = await fetchMember(args[2], message.guild);
		if (member === null) return await message.channel.send(embed('RED', 'Unknown Member', 'The member you specified could not be found on this server.'));
		if (member.user.id === message.author.id) return await message.channel.send(embed('RED', 'Promote Error', 'You cannot demote yourself in the project.'));
		const projects = allProjects(message.author.id);
		const content = projects[project];
		if (content === undefined) return await message.channel.send(embed('RED', 'Project Error', 'You do not have a project with that name.'));
		if (projectStatus(message.author.id, content) !== 'owner') return await message.channel.send(embed('RED', 'Permission Denied', 'You do not have permission to demote members in that project.'));
		if (!content.developers.includes(member.user.id)) return await message.channel.send(embed('RED', 'Project Error', `The user **${member.user.tag}** is not a developer in the project \`${project}\`.`));
		const category = message.guild.channels.cache.get(content.category);
		await category.updateOverwrite(member.user.id, {
			USE_VAD: null,
			MANAGE_WEBHOOKS: null,
			MANAGE_MESSAGES: null
		}, 'Member promoted in the project.');
		content.developers.splice(content.developers.indexOf(member.user.id), 1);
		content.members.push(member.user.id);
		await message.channel.send(embed('GREEN', 'User Demoted', `You have successfully demoted **${member.user.tag}** in the project \`${project}\`.`));
	} else if (args.length === 3 && args[0].toLowerCase() === 'kick') {
		// Kicks a member from a project.
		const project = args[1];
		const member = await fetchMember(args[2], message.guild);
		if (member === null) return await message.channel.send(embed('RED', 'Unknown Member', 'The member you specified could not be found on this server.'));
		if (member.user.id === message.author.id) return await message.channel.send(embed('RED', 'Invite Error', 'You cannot kick yourself from the project.'));
		const projects = allProjects(message.author.id);
		const names = Object.keys(projects);
		if (!names.includes(project)) return await message.channel.send(embed('RED', 'Project Error', 'You do not have a project with that name.'));
		const content = projects[project];
		const myStatus = projectStatus(message.author.id, content);
		const theirStatus = projectStatus(member.user.id, content);
		if (myStatus === 'member') return await message.channel.send(embed('RED', 'Permission Denied', 'You do not have permission to kick members from that project.'));
		if (theirStatus === 'outsider') return await message.channel.send(embed('RED', 'Project Error', `The user **${member.user.tag}** is not a part of the project \`${project}\`.`));
		if (myStatus !== 'owner' && theirStatus !== 'member') return await message.channel.send(embed('RED', 'Permission Denied', 'You cannot kick other admins or the owner from that project.'));
		if (content.developers.includes(member.user.id)) {
			content.developers.splice(content.developers.indexOf(member.user.id), 1);
		} else if (content.members.includes(member.user.id)) {
			content.members.splice(content.members.indexOf(member.user.id), 1);
		}
		const category = message.guild.channels.cache.get(content.category);
		await category.updateOverwrite(member.user.id, {
			VIEW_CHANNEL: null,
			USE_VAD: null,
			MANAGE_WEBHOOKS: null,
			MANAGE_MESSAGES: null
		}, 'Removed member from project.');
		await message.channel.send(embed('GREEN', 'User Kicked', `You have successfully kicked **${member.user.tag}** from the project \`${project}\`.`));
	} else if (args.length === 4 && args[0].toLowerCase() === 'move') {
		// Moves a channel up or down in a project.
		const project = args[1];
		const channelName = args[2];
		const amount = +args[3];
		if (isNaN(amount) || Math.floor(amount) !== amount || amount === Infinity || amount === -Infinity) return await message.channel.send(embed('RED', 'Invalid Number', 'The number must be in a valid format and be whole and finite.'));
		if (amount === 0) return await message.channel.send(embed('RED', 'Project Error', 'You must provide a non-zero number to move the project by, either positive or negative.'));
		const projects = allProjects(message.author.id);
		const names = Object.keys(projects);
		if (!names.includes(project)) return await message.channel.send(embed('RED', 'Project Error', 'You do not have a project with that name.'));
		const content = projects[project];
		if (projectStatus(message.author.id, content) === 'member') return await message.channel.send(embed('RED', 'Permission Denied', 'You do not have permission to move channels in that project.'));
		let channel;
		content.channels.forEach(id => {
			const temp = message.guild.channels.cache.get(id);
			if (temp.name === channelName) channel = temp;
		});
		if (channel === undefined) return await message.channel.send(embed('RED', 'Project Error', `The project \`${project}\` does not have a channel named \`${name}\`.`));
		await channel.setPosition(amount, {
			relative: true
		});
		await message.channel.send(embed('GREEN', 'Project Updated', `You have successfully moved the channel \`${channelName}\` by a total of ${Math.abs(amount)} positions ${amount > 0 ? 'down' : 'up'} in the project \`${project}\`.`));
	} else if (args.length >= 2 && args[0].toLowerCase() === 'create') {
		// Creates a new project with an optional description.
		const name = args[1];
		if (name.includes('`')) return await message.channel.send(embed('RED', 'Project Error', 'Projects cannot contain the backtick character in their name.'));
		if (name.length > 32 || !name.length) return await message.channel.send(embed('RED', 'Project Error', 'Project names must be between 1 and 32 characters in length.'));
		let description = texts[2] ?? null;
		if (description !== null && description.length > 128) return await message.channel.send(embed('RED', 'Project Error', 'Project descriptions must be at most 128 characters in length.'));
		const user = userData(message.author.id);
		const owned = ownedProjects(message.author.id);
		if (Object.keys(owned).length >= user.maxProjects) return await message.channel.send(embed('RED', 'Project Error', `You have exceeded your maximum number of projects, which is ${user.maxProjects}. To have more, you must purchase a slot from \`u!shop\` and open a ticket.`));
		if (projectExists(name)) return await message.channel.send(embed('RED', 'Project Error', 'There is already a project with that name.'));
		const category = await message.guild.channels.create(name, {
			type: 'category',
			reason: `${message.author.tag} (${message.author.id}) made a new project.`,
			permissionOverwrites: [
				{
					id: message.author.id,
					allow: [
						'VIEW_CHANNEL',
						'USE_VAD',
						'MANAGE_WEBHOOKS',
						'MANAGE_MESSAGES'
					]
				},
				{
					id: message.guild.roles.everyone.id,
					deny: [
						'VIEW_CHANNEL'
					]
				}
			]
		});
		const channel = await message.guild.channels.create('main', {
			type: 'text',
			reason: `Main project channel.`,
			parent: category.id
		});
		owned[name] = {
			owner: message.author.id,
			description,
			category: category.id,
			developers: [],
			members: [],
			invite_only: true,
			channels: [
				channel.id
			]
		};
		await message.channel.send(embed('GREEN', 'Project Created', `You have successfully created a project named \`${name}\`.`));
	} else if (args.length === 3 && args[0].toLowerCase() === 'rename') {
		// Changes a project to a new name.
		const project = args[1];
		const name = args[2];
		if (name.includes('`')) return await message.channel.send(embed('RED', 'Project Error', 'Projects cannot contain the backtick character in their name.'));
		if (name.length > 32 || !name.length) return await message.channel.send(embed('RED', 'Project Error', 'Project names must be between 1 and 32 characters in length.'));
		const owned = ownedProjects(message.author.id);
		const projects = allProjects(message.author.id);
		const content = projects[project];
		if (content === undefined) return await message.channel.send(embed('RED', 'Project Error', 'You do not have a project with that name.'));
		if (projectStatus(message.author.id, content) !== 'owner') return await message.channel.send(embed('RED', 'Permission Denied', 'You do not have permission to change the name of that project.'));
		if (projectExists(name)) return await message.channel.send(embed('RED', 'Project Error', 'There is already a project with that name.'));
		delete owned[project];
		owned[name] = content;
		await message.guild.channels.cache.get(content.category).setName(name, 'Project was renamed.');
		await message.channel.send(embed('GREEN', 'Project Renamed', `You have successfully renamed that project to \`${name}\`.`));
	} else if (args.length === 3 && args[0].toLowerCase() === 'mode') {
		// Changes the privacy mode for a project.
		const project = args[1];
		const mode = args[2].toLowerCase();
		if (!['public', 'private'].includes(mode)) return await message.channel.send(embed('RED', 'Unknown Mode', 'The mode must be either `public` or `private`.'));
		const projects = allProjects(message.author.id);
		const content = projects[project];
		if (content === undefined) return await message.channel.send(embed('RED', 'Project Error', 'You do not have a project with that name.'));
		if (projectStatus(message.author.id, content) === 'member') return await message.channel.send(embed('RED', 'Permission Denied', 'You do not have permission to change the mode of that project.'));
		content.invite_only = mode === 'private';
		await message.channel.send(embed('GREEN', 'Project Updated', `You have successfully made the project \`${project}\` ${mode}.`));
	} else if (args.length === 2 && args[0].toLowerCase() === 'mode') {
		// Displays the privacy mode for a project.
		const project = args[1];
		const projects = allProjects(message.author.id);
		const content = projects[project];
		if (content === undefined) return await message.channel.send(embed('RED', 'Project Error', 'You do not have a project with that name.'));
		await message.channel.send(embed('BLUE', 'Project Mode', `The project \`${project}\` is currently ${content.invite_only ? 'private' : 'public'}.`));
	} else if (args.length === 3 && args[0].toLowerCase() === 'add') {
		// Creates a text channel in a project category.
		const project = args[1];
		const name = args[2];
		if (name.includes('`')) return await message.channel.send(embed('RED', 'Project Error', 'Channels cannot contain the backtick character in their name.'));
		if (name.length > 32 || !name.length) return await message.channel.send(embed('RED', 'Project Error', 'Channel names must be between 1 and 32 characters in length.'));
		const projects = allProjects(message.author.id);
		const content = projects[project];
		if (content === undefined) return await message.channel.send(embed('RED', 'Project Error', 'You do not have a project with that name.'));
		if (projectStatus(message.author.id, content) === 'member') return await message.channel.send(embed('RED', 'Permission Denied', 'You do not have permission to add channels to that project.'));
		let exists = false;
		content.channels.forEach(id => {
			if (message.guild.channels.cache.get(id).name === name) exists = true;
		});
		if (exists) return await message.channel.send(embed('RED', 'Project Error', `The project \`${project}\` already has a channel named \`${name}\`.`));
		if (content.channels.length >= 5) return await message.channel.send(embed('RED', 'Project Error', 'You can have a maximum of 5 channels per project.'));
		const channel = await message.guild.channels.create(name, {
			type: 'text',
			reason: `New project channel.`,
			parent: content.category
		});
		content.channels.push(channel.id);
		await message.channel.send(embed('GREEN', 'Project Updated', `You have successfully added a text channel named \`${name}\` to the project \`${project}\`.`));
	} else if (args.length === 4 && args[0].toLowerCase() === 'edit') {
		// Changes the name of a project channel.
		const project = args[1];
		const channelName = args[2];
		const name = args[3];
		if (name.includes('`')) return await message.channel.send(embed('RED', 'Project Error', 'Channels cannot contain the backtick character in their name.'));
		if (name.length > 32 || !name.length) return await message.channel.send(embed('RED', 'Project Error', 'Channel names must be between 1 and 32 characters in length.'));
		const projects = allProjects(message.author.id);
		const names = Object.keys(projects);
		if (!names.includes(project)) return await message.channel.send(embed('RED', 'Project Error', 'You do not have a project with that name.'));
		const content = projects[project];
		if (projectStatus(message.author.id, content) === 'member') return await message.channel.send(embed('RED', 'Permission Denied', 'You do not have permission to rename channels in that project.'));
		let channel;
		content.channels.forEach(id => {
			const temp = message.guild.channels.cache.get(id);
			if (temp.name === channelName) channel = temp;
		});
		if (channel === undefined) return await message.channel.send(embed('RED', 'Project Error', `The project \`${project}\` does not have a channel named \`${name}\`.`));
		await channel.setName(name);
		await message.channel.send(embed('GREEN', 'Project Updated', `You have successfully renamed the channel \`${channelName}\` to the name \`${name}\` in the project \`${project}\`.`));
	} else if (args.length === 3 && args[0].toLowerCase() === 'vc') {
		// Creates a voice channel in a project category.
		const project = args[1];
		const name = args[2];
		if (name.includes('`')) return await message.channel.send(embed('RED', 'Project Error', 'Channels cannot contain the backtick character in their name.'));
		if (name.length > 32 || !name.length) return await message.channel.send(embed('RED', 'Project Error', 'Channel names must be between 1 and 32 characters in length.'));
		const projects = allProjects(message.author.id);
		const names = Object.keys(projects);
		if (!names.includes(project)) return await message.channel.send(embed('RED', 'Project Error', 'You do not have a project with that name.'));
		const content = projects[project];
		if (projectStatus(message.author.id, content) === 'member') return await message.channel.send(embed('RED', 'Permission Denied', 'You do not have permission to add channels to that project.'));
		let exists = false;
		content.channels.forEach(id => {
			if (message.guild.channels.cache.get(id).name === name) exists = true;
		});
		if (exists) return await message.channel.send(embed('RED', 'Project Error', `The project \`${project}\` already has a channel named \`${name}\`.`));
		if (content.channels.length >= 5) return await message.channel.send(embed('RED', 'Project Error', 'You can have a maximum of 5 channels per project.'));
		const channel = await message.guild.channels.create(name, {
			type: 'voice',
			reason: `New project channel.`,
			parent: content.category
		});
		content.channels.push(channel.id);
		await message.channel.send(embed('GREEN', 'Project Updated', `You have successfully added a voice channel named \`${name}\` to the project \`${project}\`.`));
	} else if (args.length === 2 && args[0].toLowerCase() === 'channels') {
		// Displays a list of the channels in a project.
		const project = args[1];
		const projects = allProjects(message.author.id);
		const names = Object.keys(projects);
		if (!names.includes(project)) return await message.channel.send(embed('RED', 'Project Error', 'You do not have a project with that name.'));
		const content = projects[project];
		const channels = content.channels.map(id => message.guild.channels.cache.get(id));
		if (!channels.length) return await message.channel.send(embed('RED', 'Project Channels', `The project \`${project}\` does not have any channels to display.`));
		await message.channel.send(embed('BLUE', 'Project Channels', channels.map(channel => `\`${channel.name}\` • ${properCase(channel.type)} Channel`).join('\n')));
	} else if (args.length === 2 && args[0].toLowerCase() === 'members') {
		// Displays a list of the users in a project.
		const project = args[1];
		const projects = allProjects(message.author.id);
		const content = projects[project];
		if (content === undefined) return await message.channel.send(embed('RED', 'Project Error', 'You do not have a project with that name.'));
		const users = [content.owner, ...content.developers, ...content.members];
		const members = [];
		for (const user of users) {
			try {
				members.push(await message.guild.members.fetch(user));
			} catch {}
		}
		if (!members.length) return await message.channel.send(embed('RED', 'Project Members', `The project \`${project}\` does not have any members to display.`));
		await message.channel.send(embed('BLUE', 'Project Members', members.map(member => `**${member.user.tag}** • ${properCase(projectStatus(member.user.id, content))}`).join('\n')));
	} else if (args.length === 3 && args[0].toLowerCase() === 'remove') {
		// Deletes a project channel by name.
		const project = args[1];
		const name = args[2];
		const projects = allProjects(message.author.id);
		const names = Object.keys(projects);
		if (!names.includes(project)) return await message.channel.send(embed('RED', 'Project Error', 'You do not have a project with that name.'));
		const content = projects[project];
		if (projectStatus(message.author.id, content) === 'member') return await message.channel.send(embed('RED', 'Permission Denied', 'You do not have permission to remove channels from that project.'));
		let channel;
		content.channels.forEach(id => {
			const temp = message.guild.channels.cache.get(id);
			if (temp.name === name) channel = temp;
		});
		if (channel === undefined) return await message.channel.send(embed('RED', 'Project Error', `The project \`${project}\` does not have a channel named \`${name}\`.`));
		content.channels.splice(content.channels.indexOf(channel.id), 1);
		await channel.delete();
		await message.channel.send(embed('GREEN', 'Project Updated', `You have successfully removed the \`${name}\` channel from the project \`${project}\`.`));
	} else if (args.length >= 2 && args[0].toLowerCase() === 'description') {
		// Changes or displays the description of a project.
		const name = args[1];
		let description = texts[2] ?? null;
		if (description !== null && description.length > 128) return await message.channel.send(embed('RED', 'Project Error', 'Project descriptions must be at most 128 characters in length.'));
		const projects = allProjects(message.author.id);
		const names = Object.keys(projects);
		if (!names.includes(name)) return await message.channel.send(embed('RED', 'Project Error', 'You do not have a project with that name.'));
		const project = projects[name];
		if (description === null) return await message.channel.send(embed('BLUE', 'Project Description', project.description ?? 'No description provided.'));
		if (projectStatus(message.author.id, project) === 'member') return await message.channel.send(embed('RED', 'Permission Denied', 'You do not have permission to change the description of that project.'));
		project.description = description;
		await message.channel.send(embed('GREEN', 'Project Updated', `You have successfully changed the description of \`${name}\`.`));
	} else if (args.length === 2 && args[0].toLowerCase() === 'delete') {
		// Deletes one of your projects and all of its channels.
		const name = args[1];
		const owned = ownedProjects(message.author.id);
		const projects = allProjects(message.author.id);
		const names = Object.keys(projects);
		if (!names.includes(name)) return await message.channel.send(embed('RED', 'Project Error', 'You do not have a project with that name.'));
		const project = projects[name];
		if (projectStatus(message.author.id, project) !== 'owner') return await message.channel.send(embed('RED', 'Permission Denied', 'You do not have permission to delete that project.'));
		const promises = [];
		project.channels.forEach(id => promises.push(message.guild.channels.cache.get(id).delete()));
		await Promise.all(promises);
		await message.guild.channels.cache.get(project.category).delete();
		delete owned[name];
		await message.channel.send(embed('GREEN', 'Project Deleted', `You have successfully deleted ${name}.`));
	} else if (args.length === 1 && args[0].toLowerCase() === 'list') {
		// Shows a list of the projects that you own.
		const projects = allProjects(message.author.id);
		const entries = Object.entries(projects);
		if (!entries.length) return await message.channel.send(embed('RED', 'Project List', 'You do not have any projects to display.'));
		await message.channel.send(embed('BLUE', 'Project List', entries.map(entry => `\`${entry[0]}\` **${projectStatus(message.author.id, entry[1])}** • ${entry[1].description ?? 'No description provided.'}`).join('\n')));
	} else if (args.length === 2 && args[0].toLowerCase() === 'list') {
		// Shows a list of the projects that a user owns.
		const member = await fetchMember(args[1], message.guild);
		if (member === null) return await message.channel.send(embed('RED', 'Unknown Member', 'The member you specified could not be found on this server.'));
		const id = member.user.id;
		const projects = allProjects(id);
		const entries = Object.entries(projects);
		if (!entries.length) return await message.channel.send(embed('RED', 'Project List', 'That user does not have any projects to display.'));
		await message.channel.send(embed('BLUE', 'Project List', entries.map(entry => `\`${entry[0]}\` **${projectStatus(id, entry[1])}** • ${entry[1].description ?? 'No description provided.'}`).join('\n')));
	} else {
		// This is sent whenever invalid arguments are given.
		await message.channel.send(embed('RED', 'Project Error', `Invalid arguments. You can check the subcommands with \`${prefix}project help\`.`));
	}
};