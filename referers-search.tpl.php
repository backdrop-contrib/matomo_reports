<?php
// $Id: referers-search.tpl.php,v 1.1.2.6 2010/10/09 15:20:08 hass Exp $
?>
<h2><?php print t('Search engines') ?></h2>
<div class="content">
  <object width="100%" height="300" type="application/x-shockwave-flash" data="<?php print $piwik_url ?>/libs/open-flash-chart/open-flash-chart.swf?v2i" id="ReferersgetSearchEnginesChart">
    <param name="allowScriptAccess" value="always"/>
    <param name="wmode" value="opaque"/>
    <param name="flashvars" value="data-file=<?php print $data1_url ?>"/>
  </object>
</div>

<h2><?php print t('Keywords') ?></h2>
