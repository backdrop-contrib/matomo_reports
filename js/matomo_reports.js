(function ($) {
  Backdrop.behaviors.matomo_reports = {
    attach: function(context, settings) {

      var pk_url = settings.matomo_reports.url;
      var data;
      var header = '<table class="sticky-enabled sticky-table" width="100%"><tbody></tbody></table>';

      // Add the table and show "Loading data..." status message for long running requests.
      $("#matomopageviews").html(header);
      $("#matomopageviews > table > tbody").html("<tr><td>" + Backdrop.t('Loading data...') + "</td></tr>");

      // Get data from remote Matomo server.
      $.getJSON(pk_url, function(data){
        var item = '';
        $.each(data, function(key, val) {
          item = val;
        });
        var pk_content = "";
        if (item != '') {
          if (item.nb_visits) {
            pk_content += "<tr><td>" + Backdrop.t('Visits') + "</td>";
            pk_content += "<td>" + item.nb_visits + "</td></tr>" ;
          }
          if (item.nb_hits) {
            pk_content += "<tr><td>" + Backdrop.t('Page views') + "</td>";
            pk_content += "<td>" + item.nb_hits + "</td></tr>" ;
          }
        }
        // Push data into table and replace "Loading data..." status message.
        if (pk_content) {
          $("#matomopageviews > table > tbody").html(pk_content);
        }
        else {
          $("#matomopageviews > table > tbody > tr > td").html(Backdrop.t('No data available.'));
        }
      });
    }
  };
})(jQuery);
