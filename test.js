console.log('Starting v0.0.1');

const EventEmitter = require('eventemitter3');
const readline = require('readline');
const DiscordRPC = require('./build/Release/discordrpc.node');
const AppID = '380924869245796352';
const games = require('./games.json');
const ee = new EventEmitter();
ee.on('meow', () => console.log('meow'));

DiscordRPC.setHandler(function (key, value) {
    console.log(key, value);
});

console.log('Handler set');

console.log('Initialized');

const rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout
});

function exit() {
    DiscordRPC.shutdown();
    process.exit();
}

let cont = true;

let discordPresence = {};

function startGame(game) {
    DiscordRPC.initialize(game.id);
    discordPresence = {};
    if (game.defaults) {
        for (const key in game.defaults) {
            discordPresence[key] = game.defaults[key];
        }
        if (game.defaults.state)
            discordPresence.startTimestamp = Date.now() / 1000;
        DiscordRPC.updateDiscordPresence(discordPresence);
    }
}

function prompt(question) {
    return new Promise((res) => {
        rl.question(question, answer => {
            res(answer);
        })
    });
}

async function gameMenu() {
    let q = 'Please select the game you are playing:\n';
    let list = [];
    for (let i = 0; i < games.length; i++) {
        let game = games[i];
        let g = `${i + 1}. ${game.name}`;
        list.push(g);
    }
    let res = await prompt(q + list.join('\n') + '\n> ');
    let index = parseInt(res);
    if (isNaN(index)) {
        console.log('Invalid selection. Try again.');
        return await gameMenu();
    }
    index -= 1;
    if (index < 0 || index >= games.length) {
        console.log('Invalid selection. Try again.');
        return await gameMenu();
    }
    let game = games[index];

    startGame(game);
}

async function status() {
    let answer = await prompt('> ');
    if (answer === 'q')
        return exit();
    if (answer.startsWith('/')) {
        answer = answer.substring(1).split(' ');
        let key = answer[0];
        answer = answer.slice(1).join(' ');
        switch (key) {
            case 'game':
                DiscordRPC.shutdown();
                return await gameMenu();
            default:
                console.log('Setting', key, 'to', answer);
                discordPresence[key] = answer
                break;
        }
    } else
        discordPresence.details = answer;
    try {
        DiscordRPC.updateDiscordPresence(discordPresence);
    } catch (err) {
        console.error(err);
    }
}

gameMenu().then(async () => {
    while (cont) await status();
})