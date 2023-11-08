const express = require('express');
const app = express();
const http = require('http');
const server = http.createServer(app);

var x = 0;

app.get('/', (req, res) => {
    res.sendFile(__dirname + '/index.html');
});

app.get('/client.js', (req, res) => {
    console.log("Load wasm module")
	res.sendFile(__dirname + '/client.js');
});

app.get('/client.data', (req, res) => {
    console.log("Load wasm module")
	res.sendFile(__dirname + '/client.data');
});

app.get('/websocket.js', (req, res) => {
    console.log("Load wasm module")
	res.sendFile(__dirname + '/wasm.module.js');
});

app.get('/wasm.module.js', (req, res) => {
    console.log("Load wasm module")
	res.sendFile(__dirname + '/wasm.module.js');
});



server.listen(3000, () => {
    console.log('listening on *:3000');
});
