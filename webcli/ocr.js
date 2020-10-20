var express = require('express');

var SERVER_PORT = 9999;
var serverapp = express(); 

serverapp.use(express.static(__dirname)); 

serverapp.listen(SERVER_PORT, function() { 
    console.log('Started server at http://0.0.0.0:' + SERVER_PORT); 
});
