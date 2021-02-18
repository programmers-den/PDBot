const {embed, display} = require('../utilities/display');
const config = require('../config.json');
const { client } = require('..');
exports.name = 'announce';
exports.type = 'Staff';
exports.info = 'Creates a server announcement.';
exports.usage = '[title] [content]';
exports.alias = [];
exports.root = false;
exports.mod = true;
exports.admin = false;

exports.run = async ({message, args}) => {
    const ch = client.channels.cache.get(+config.channels.announcements);
	switch(args[0]) {
        case 'general':
            break;
        case 'add':
            break;
        case 'remove':
            break;
        case 'modify':
            break;
    }
};