<?php

/**
 * @file
 * Default implementation of the visitors overview report template.
 *
 * Available variables:
 * - $data_url: complete url with params to get selected report.
 */
foreach($data_url as $data) {
  if (!is_null($data['empty_text'])) {
    print $data['empty_text'];
  }
  else {
    dpm('The contents of $data, one element at a time:');
    foreach ($data as $key => $value) {
      dpm($value, $key);
    }
    ?>
    <h2><?php print $data['title']; ?></h2>
    <div class="widgetIframe">
      <iframe style="border: 0px none; width: 100%; height: <?php print $data['iframe_height'] . 'px;" src="' . $data['url'];?>" scrolling="auto" frameborder="0"></iframe>
      <!--iframe src="http://matomo.techccessible.com/index.php?module=Widgetize&action=iframe&moduleToWidgetize=Dashboard&actionToWidgetize=index&idSite=22&period=week&date=yesterday" frameborder="0" marginheight="0" marginwidth="0" width="100%" height="100%"></iframe-->
    </div>
  <?php }
}
