
var sys = require('sys'), express = require('express');
var querystring = require('querystring');

var notmuch = require('../build/default/notmuch');

var app = express.createServer();
module.exports = app;

app.configure(function() {
    app.use(express.logger());
    app.use(express.bodyDecoder());
    app.use(app.router);
    app.use(express.staticProvider(__dirname + '/public'));
});

var db = new notmuch.open('/Users/tomc/.mail');


/**************************************************************************
* Routes                                                                 */

app.get('/search/*', function(req, res) {
    res.headers.contentType = 'application/json';
    var query = querystring.unescape(req.params[0]);
    db.query(query).threads(function(result) {
        sys.puts(result.length);
        res.send(JSON.stringify(result));
    });
});

app.get('/message/*', function(req, res) {
    var id = querystring.unescape(req.params[0]);
    sys.puts(id);
    db.find(id, function(message) {
        if (message == null) {
            res.send("Message not found", 404);
            return;
        }
        sys.puts(JSON.stringify(message));
        res.send(res.partial('message.jade', { collection: [ message ] }));
    });
});

app.get('/thread/*', function(req, res) {
    var id = querystring.unescape(req.params[0]);
    sys.puts(id);
    db.query('thread:'+id).messages(function(result) {
        sys.puts(result.length);
        res.send(JSON.stringify(result));
    });
});
