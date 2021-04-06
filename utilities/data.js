const {files} = require('./storage');

exports.registerPoll = (id, poll) => files.polls[id] = poll;

exports.fetchPoll = id => files.polls[id];

exports.userData = id => {
	if (files.users[id] === undefined) files.users[id] = {};
	const user = files.users[id];
	if (user.maxProjects === undefined) user.maxProjects = 1;
	if (user.activeInvites === undefined) user.activeInvites = {};
	return user;
};

exports.ownedProjects = id => {
	if (files.projects[id] === undefined) files.projects[id] = {};
	const projects = files.projects[id];
	return projects;
};

exports.projectExists = name => {
	for (const projects of Object.values(files.projects)) if (name in projects) return true;
	return false;
};

exports.allProjects = id => {
	const projects = {};
	for (const entries of Object.values(files.projects)) {
		for (const [name, project] of Object.entries(entries)) {
			if (id === undefined || project.owner === id || project.members.includes(id) || project.developers.includes(id)) {
				projects[name] = project;
			}
		}
	}
	return projects;
};

exports.fetchProject = name => exports.allProjects()[name];

exports.inProject = (id, name) => name in exports.allProjects(id);

exports.projectStatus = (id, project) => {
	if (project.owner === id) return 'owner';
	else if (project.developers.includes(id)) return 'developer';
	else if (project.members.includes(id)) return 'member';
	else return 'outsider';
}

exports.startConstStar = id => {
	if (files.starboard[id] === undefined) files.starboard[id] = {};
	const starboard = files.starboard[id];
	return starboard
}

exports.allStarMes = (id, ch) => {
	const starboards = {};
	for (const entries of Object.values(files.starboard)) {
		for (const [id, channel] of Object.entries(entries)) {
			if (id === undefined || channel.oriMesID === ch) {
				starboards[id] = channel;
			} 
		}
	}
	return starboards;
}