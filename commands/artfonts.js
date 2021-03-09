const {client} = require("..");
const {embed} = require('../utilities/display');
const axios = require('axios');
const cheerio = require('cheerio');

exports.name = "artfonts";
exports.type = 'Utilities';
exports.info = "Show what fonts are avaliable for art comamnd";
exports.usage = '';
exports.alias = [];
exports.root = false;
exports.mod = false;
exports.admin = false;

exports.run = async ({message, args}) => {
    const { data } = await axios.get('http://www.figlet.org/fontdb.cgi');
    const pageData = await cheerio.load(data);
    const fonts = pageData('td').text();
    var x = '';

    console.log(fonts)
    for(let n = 1, i; n < fonts.length; i = fonts[(n += 3) - 3]) {
        if (i === undefined) break;
        console.log(i)
        text = i.trim();
        if (text != 'Font Name' && text != 'Contact us at info@figlet.org') {
        x = `${x}${text}\n`;
        };
    };

    message.channel.send(embed('BLUE', 'Fonts', `\`\`\`\css\n${x}\`\`\``))
}
