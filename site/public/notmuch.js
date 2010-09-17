
function error(req, status, err) {
	$('#content').text('An error occurred while processing the request ('+err+', '+req.status+')')
}

$('document').ready(function() {
  var search = $('#search');

  var searchUpdate = function() {
    if (search.val().length < 1) {
      $('#sidebar').empty();
      return;
    };

    $.ajax({
  		type: 'GET', url: '/search/'+search.val(), dataType: 'json',
  		success: function(data) {
  		  $('#sidebar').empty();

  		  $.each(data, function(val, data) {
  				var message = $('<div href="#repo" class="entry"/>')
  				$('<h1 class="message"/>').text(data.subject).appendTo(message)

                message.click(function() {
                    $.get('/thread/'+data.id, function(data) {
                          $('.result').html(data);
                    });
                });
  				message.appendTo('#sidebar')
  			});
		  },
	  });
  }
  search.change(searchUpdate).keyup(searchUpdate);

  var url = window.location.toString().match(/\?(.+)$/)
  $('input').val(RegExp.$1)
});