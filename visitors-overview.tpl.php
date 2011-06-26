<?php
/**
 * @file
 * Default implementation of the visitors overview report template.
 *
 * Available variables:
 * - $piwik_url: piwik server base url.
 * - $data1_url: complete url with params to get selected report.
 */dsm($data1_url); dsm($piwik_url);
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
<div id="widgetIframe"><iframe width="100%" height="350" <?php print 'src="' . $data1_url; ?>" scrolling="no" frameborder="0" marginheight="0" marginwidth="0"></iframe></div>
