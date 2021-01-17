const fs = require('fs');

fs.readdirSync('./commands').filter(file => file.endsWith('.js')).forEach(file => {
	exports[file.slice(0, -3)] = {
		name: 'test',
		type: 'Uncategorized',
		info: 'No description provided.',
		alias: [],
		root: false,
		mod: false,
		admin: false,
		run: async () => {},
		...require(`../commands/${file}`)
	};
});