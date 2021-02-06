const {client} = require('..');

let active = {};

client.on("messageDelete", async message => {
	delete active[message.id];
});
client.on("messageReactionAdd", async (reaction, user) => {
	if (user.bot) return;
	if (reaction.partial) await reaction.fetch();
	if (reaction.message.partial) await reaction.message.fetch;
	const {message} = reaction;
	if (message.id in active) {
		const menu = active[message.id];
		if (reaction.emoji.name in menu) {
			await menu[reaction.emoji.name](user);
		}
		if (reaction.message.guild && reaction.message.guild.me.hasPermission("MANAGE_MESSAGES")) {
			await reaction.users.remove(user).catch(() => {});
		}
	}
});
client.on("messageReactionRemove", async (reaction, user) => {
	if (user.bot) return;
	if (reaction.partial) await reaction.fetch();
	if (reaction.message.partial) await reaction.message.fetch;
	const {message} = reaction;
	if (reaction.message.guild && reaction.message.guild.me.hasPermission("MANAGE_MESSAGES")) return;
	if (message.id in active) {
		const menu = active[message.id];
		if (reaction.emoji.name in menu) {
			await menu[reaction.emoji.name](user);
		}
	}
});

async function replyMenu(message, menu) {
	return await sendMenu(message.channel, menu, message.author);
}

async function sendMenu(channel, menu, user) {
	const allowed = user;
	if (user === undefined) user = () => true;
	else if (typeof user !== 'function') user = current => current.id === allowed.id;
	const message = await menu[''](channel, user);
	active[message.id] = menu;
	for (const emoji of Object.keys(menu)) {
		if (emoji !== '') await message.react(emoji);
	}
	return message;
}

function pagesFull(...pages) {
	const data = {};
	return {
		'': async (channel, user) => {
			data.page = 1;
			data.pages = pages;
			data.channel = channel;
			data.user = user;
			data.message = await channel.send(data.pages[data.page - 1]);
			data.repage = () => data.message.edit(data.pages[data.page - 1]);
			return data.message;
		},
		'\u23EA': async user => {
			if (!data.user(user)) return;
			data.page = 1;
			await data.repage();
		},
		'\u25C0': async user => {
			if (!data.user(user)) return;
			data.page--;
			if (data.page < 1) {
				data.page = data.pages.length;
			}
			await data.repage();
		},
		'\u25B6': async user => {
			if (!data.user(user)) return;
			data.page++;
			if (data.page > data.pages.length) {
				data.page = 1;
			}
			await data.repage();
		},
		'\u23E9': async user => {
			if (!data.user(user)) return;
			data.page = data.pages.length;
			await data.repage();
		}
	}
}

function pages(...pages) {
	const data = {};
	return {
		'': async (channel, user) => {
			data.page = 1;
			data.pages = pages;
			data.channel = channel;
			data.user = user;
			data.message = await channel.send(data.pages[data.page - 1]);
			data.repage = () => data.message.edit(data.pages[data.page - 1]);
			return data.message;
		},
		'\u25C0': async user => {
			if (!data.user(user)) return;
			data.page--;
			if (data.page < 1) {
				data.page = data.pages.length;
			}
			await data.repage();
		},
		'\u25B6': async user => {
			if (!data.user(user)) return;
			data.page++;
			if (data.page > data.pages.length) {
				data.page = 1;
			}
			await data.repage();
		}
	}
}

exports.sendMenu = sendMenu;
exports.replyMenu = replyMenu;
exports.pagesFull = pagesFull;
exports.pages = pages;