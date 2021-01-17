const {embed} = require('../utilities/display');
const axios = require('axios');
const cheerio = require('cheerio');

exports.name = 'wiki';
exports.type = 'Information';
exports.info = 'Searches Wikipedia for a topic.';
exports.usage = '[topic]';
exports.alias = ['wikipedia'];
exports.root = false;
exports.mod = false;
exports.admin = false;

exports.run = async ({message, text}) => {
	if (!text.length) return await message.channel.send(embed('RED', 'Wikipedia Error', 'You did not provide a topic to search Wikipedia for.'));
	try {
		const url = `https://en.wikipedia.org/wiki/${encodeURI(text)}`;
		const {data} = await axios.get(url);
		const $ = cheerio.load(data);
		let result = $('#mw-content-text .mw-parser-output > p').text().trim();
		result = result.replace(/\[\d+\]/g, '');
		result = result.replace(/\n{3,}/g, '\n\n');
		if (result.includes(' may refer to:')) {
			const items = [];
			$('#mw-content-text .mw-parser-output ul li').each(function() {
				items.push($(this).text().trim());
			});
			let content = 'That query was too ambiguous. The following specific items can be used as a query instead.\n\n' + items.join('\n');
			if (content.length > 2000) content = content.slice(0, 1997) + '...';
			await message.channel.send(embed('RED', 'Wikipedia Result', content));
			return;
		}
		let content = `[Click here to view the Wikipedia page.](${url})\n\n${result}`;
		if (content.length > 2000) content = content.slice(0, 1997) + '...';
		await message.channel.send(embed('BLUE', 'Wikipedia Result', content));
	} catch(error) {
		await message.channel.send(embed('RED', 'Wikipedia Error', 'An error occurred while trying to search Wikipedia for this topic. Try again later or use a different query.\n\n' + error.message));
	}
};