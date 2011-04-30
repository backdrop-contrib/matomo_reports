
(function ($) {
  Drupal.behaviors.piwik_reports = {
    attach: function(context, settings) {

      var pk_url = settings.piwik_reports.url
      var pk_page = settings.piwik_reports.page
      var data
      var columns = 2;

      var header = "<table class='sticky-enabled sticky-table'><thead class='tableHeader-processed'>";
      header += "<tr><th>" + Drupal.t('Label') + "</th>";
      switch (pk_page) {
        case "actions":
          header += "<th>" + Drupal.t('Visitors') + "</th><th>" + Drupal.t('Hits') + "</th>";
          columns = 3;
          break;
        case "websites":
          header += "<th>" + Drupal.t('Visitors') + "</th>";
          break;
        case "search":
          header += "<th>" + Drupal.t('Visits') + "</th>";
          break;
      }
      header += "</tr></thead><tbody></tbody></table>";

      // Add the table and show "Loading data..." status message for long running requests.
      $("#pagestable").html(header);
      $("#pagestable > table > tbody").html("<tr class='odd'><td colspan='" + columns + "'>" + Drupal.t('Loading data...') + "</td></tr>");

      // Get data from remote Piwik server.
      $.getJSON(pk_url, function(data){
        var pk_content = "";
        var tr_class = "even";

        $.each(data, function(i,item){
          if (tr_class == "odd") { item_class = "even"; } else { item_class = "odd"; }
          tr_class = item_class;

          pk_content += "<tr class='" + item_class + "'><td>" + item["label"] + "</td>";
          switch (pk_page) {
            case "actions":
              pk_content += "<td>" + item["nb_visits"] + "</td><td>" + item["nb_hits"] + "</td>";
              break;
            case "websites":
              pk_content += "<td>" + item["nb_visits"] + "</td>";
              break;
            case "search":
              pk_content += "<td>" + item["nb_visits"] + "</td>";
              break;
          }
          pk_content += "</tr>";
        });
        // Push data into table and replace "Loading data..." status message.
        if (pk_content) {
          $("#pagestable > table > tbody").html(pk_content);
        }
        else {
          $("#pagestable > table > tbody > tr.odd > td").html(Drupal.t('No data available.'));
        }
      });
   
    }

  };

})(jQuery);
