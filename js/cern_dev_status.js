// $Id$
(function ($) { 
  $(document).ready(function()
  {
	  var contactMail = $("#cern_dev_status_contact_mail").text().replace("[at]", "@");
  	$("#cern_dev_status_contact_mail").html("<a href=\"mailto:" + contactMail + "\">" + contactMail + "</a>");
  });
})(jQuery)