const { embed } = require('../utilities/display');
const { exec } = require('child_process');
const axios = require('axios');
const fs = require('fs/promises');

exports.name = 'compress';
exports.type = 'Utilities';
exports.info = 'Compresses an image or video.';
exports.usage = 'audio|video min|max [attachment|link]';
exports.alias = [];
exports.root = false;
exports.admin = false;
exports.mod = false;

let running = false;

exports.run = async ({message, args}) => {
	if (running) return await message.channel.send(embed('RED', 'Compression Error', 'Please wait until nobody is compressing anything to use this command.'));
	running = true;
	const url = message.attachments.size > 0 ? message.attachments.array()[0].url : args.filter(arg => {
		try {
			new URL(arg);
			return true;
		} catch {
			return false;
		}
	})[0];
	if (url === undefined) {
		running = false;
		return await message.channel.send(embed('RED', 'Compression Error', 'No input url or attachment has been specified.'));
	}
	const {data} = await axios.get(url);
	// write file and continue here.
	const cp = exec('ffmpeg -i - -c:v libvpx-vp9 -r:v 24 -crf 10 -c:a libopus -b:a 14k -ac 1 output.mp4');
	cp.stdin.write(data);
	cp.stderr.pipe(process.stderr);
	cp.stdout.pipe(process.stdout);
}