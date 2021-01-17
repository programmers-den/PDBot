const storage = require('./storage');

let handled = false;

const handler = () => {
	
	if (handled) return;
	handled = true;

	storage.cleanup();
	
}

process.on('exit', handler);
process.on('SIGINT', () => {
	handler();
	process.exit();
});
process.on('SIGUSR1', () => {
	handler();
	process.exit();
});
process.on('SIGUSR2', () => {
	handler();
	process.exit();
});
process.on('uncaughtException', () => {
	handler();
	process.exit();
});