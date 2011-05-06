<?php
/**
 * @file
 * Default implementation of the visitors overview report template.
 *
 * Available variables:
 * - $piwik_url: piwik server base url.
 * - $data1_url: complete url with params to get selected report.
 */
?>
<h2><?php
  switch ($period) {
    case 'week':
      print t('Visitors in time period by week');
      break;

    default:
      print t('Visitors in time period by day');
  }
?></h2>
<div class="content">
  <object width="100%" height="300" type="application/x-shockwave-flash" data="<?php print $piwik_url ?>/libs/open-flash-chart/open-flash-chart.swf?piwik=1.4" id="VisitsSummarygetLastVisitsGraphChart">
    <param name="allowScriptAccess" value="always"/>
    <param name="wmode" value="opaque"/>
    <param name="flashvars" value="data-file=<?php print $data1_url ?>"/>
  </object>
</div>
