const {client} = require("..");
const {sendMenu, pages} = require('../utilities/menus');
const {embed} = require("../utilities/display");
const figlet = require('figlet');

exports.name = "artfonts";
exports.type = 'Utilities';
exports.info = "Show what fonts are avaliable for art comamnd";
exports.usage = '';
exports.alias = [];
exports.root = false;
exports.mod = false;
exports.admin = false;

exports.run = async ({message}) => {
    const fonts = figlet.fontsSync();

    const xg = fonts.map((font, index) => `${index + 1} • **${font}**`);
    const menu = [];
    const size = 10;

    for (let i = 0; i < xg.length; i += size) {
        menu.push(embed(`BLUE`, `Font List ${i / size + 1}/${Math.ceil(xg.length / size)}`, xg.slice(i, i + size).join('\n')));
    }
    if (menu.length === 1) return await message.channel.send(menu[0]);
    await sendMenu(message.channel, pages(...menu), message.author);
}
