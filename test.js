console.log('Starting v0.0.1');

const EventEmitter = require('eventemitter3');
const readline = require('readline');
const DiscordRPC = require('./build/Release/discordrpc.node');
const AppID = '380924869245796352';
const ee = new EventEmitter();
ee.on('meow', () => console.log('meow'));

DiscordRPC.setHandler(function (key, value) {
    console.log(key, value);
});

console.log('Handler set');

DiscordRPC.initialize(AppID);

console.log('Initialized');

const rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout
});

function exit() {
    DiscordRPC.shutdown();
    process.exit();
}

function prompt() {
    rl.question('> ', (answer) => {
        if (answer === 'q')
            return exit();
        DiscordRPC.updateDiscordPresence(answer);
        prompt();
    });
}
prompt();