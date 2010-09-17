
var sys = require('sys'), notmuch = require('./build/default/notmuch');

var db = new notmuch.open('/Users/tomc/.mail');

var query = db.query('*');
query.messages(function(result) {
    result.forEach(function(message) {
        //sys.puts(message.id);
    });
});
sys.puts("second run");
var query = db.query('*');
query.threads(function(result) {
    sys.puts(result[0]);
    sys.puts(result[0]);
});


db.find('0000000000001405', function(message) {
    sys.puts(message);
});

db.find('478156105.99975294683923@gasworks.com', function(message) {
    sys.puts(message);
});


var query = db.query('*');
query.messages(function(result) {
	for (i = 0; i < result.length; ++i) {
    	sys.puts(result[i].header('date'));
	}
});

var query = db.query('*');
query.messages(function(result) {
    sys.puts(result[0].id);
    sys.puts(result[0].filename);
});
