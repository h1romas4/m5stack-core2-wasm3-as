/**
 * Define
 */
require('dotenv').config();

const WEBSOCKET_PORT = process.env.WEBSOCKET_PORT;
const SERIAL_PORT = process.env.SERIAL_PORT;
const SERIAL_BAUDRATE = parseInt(process.env.SERIAL_BAUDRATE);

const webSocketServer = require('websocket').server;
const http = require('http');
const SerialPort = require("serialport").SerialPort

/**
 * WebSocket
 */
const server = http.createServer(function (request, response) {});
let clients = [];

server.listen(WEBSOCKET_PORT, function () {
    console.log('Server is listening on port ' + WEBSOCKET_PORT);
});

const wsServer = new webSocketServer({ httpServer: server });

wsServer.on('request', function (request) {
    console.log('Connection from origin ' + request.origin + '.');
    let connection = request.accept(null, request.origin);
    console.log('Connection accepted.');

    let index = clients.push(connection) - 1;

    connection.on('close', function (connection) {
        console.log("Peer " + connection.remoteAddress + " disconnected.");
        clients.splice(index, 1);
    });
});

/**
 * Serial
 */
let buffer = "";

const serialPort = new SerialPort({
    path: SERIAL_PORT,
    baudRate: SERIAL_BAUDRATE,
});

function onSerial(msg) {
    console.log("uart msg:" + msg);
    for (let i = 0; i < clients.length; i++) {
        clients[i].sendUTF(msg);
    }
}

serialPort.on('open', function () {
    console.log('open serial communication');
    serialPort.on('data', function (data) {
        buffer += new String(data);
        let lines = buffer.split("\n");
        while (lines.length > 1) {
            onSerial(lines.shift());
        }
        buffer = lines.join("\n");
    });
});
