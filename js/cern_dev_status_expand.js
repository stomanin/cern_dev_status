(function ($) { 

window.onload = function()
{
	$('body').addClass('dev-status-ui');
	$('#dev-status-message').addClass('open');
	$('#dev-status-message').removeClass('js-disabled');
	$('#dev-status-action').click(function () { 	
		$('#dev-status-message-content').slideToggle(50);
		$('#dev-status-message').toggleClass('open closed');		      
    });  
}
})(jQuery)

