<?php
/**
 * @file
 * Default implementation of the visitors locations report template.
 *
 * Available variables:
 * - $piwik_url: piwik server base url.
 * - $data1_url: complete url with params to get selected report.
 */
?>
<div class="content">
  <object width="100%" height="300" type="application/x-shockwave-flash" data="<?php print $piwik_url ?>/libs/open-flash-chart/open-flash-chart.swf?v2i" id="UserCountrygetCountryChart">
    <param name="allowScriptAccess" value="always"/>
    <param name="wmode" value="opaque"/>
    <param name="flashvars" value="data-file=<?php print $data1_url ?>"/>
  </object>
</div>
