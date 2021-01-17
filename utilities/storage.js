const path = require('path');
const fs = require('fs');

exports.files = {};

exports.resolve = name => path.resolve(`./data/${name}.json`);

exports.load = name => {
	const location = exports.resolve(name);
	if (!fs.existsSync(location)) fs.writeFileSync(location, '{}', 'utf8');
	exports.files[name] = JSON.parse(fs.readFileSync(location, 'utf8'));
}

exports.save = name => {
	const location = exports.resolve(name);
	if (exports.files[name] === undefined) exports.files[name] = {};
	fs.writeFileSync(location, JSON.stringify(exports.files[name], null, '\t'), 'utf8');
}

exports.list = [
	'users', 'polls',
	'votes', 'projects',
	'starboard'
];

exports.list.forEach(exports.load);

exports.cleanup = () => exports.list.forEach(exports.save);