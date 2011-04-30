<?php

/**
 * @file
 * Drupal Module: Piwik Reports
 */

/**
 * Report menu links
 */
function piwik_reports_menu_block_page() {
  $item = menu_get_item();
  if ($content = system_admin_menu_block($item)) {
    $output = theme('admin_block_content', array('content' => $content));
  }
  else {
    $output = '<p>' . t('You do not have any reports.') . '</p>';
  }

  return $output;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function piwik_reports_dateselect_form() {

  $period = array(
    0 => t('Today'),
    1 => t('Yesterday'),
    2 => t('Last week'),
    3 => t('Last month'),
    4 => t('Last year'),
  );

  $form['#attributes'] = array('class' => 'search-form');
  $form['basic'] = array(
    '#type' => 'fieldset',
    '#title' => t('Filter time period'),
  );
  $form['basic']['inline'] = array(
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
  );
  $form['basic']['inline']['period'] = array(
    '#type' => 'select',
    '#title' => '',
    '#default_value' => isset($_SESSION['piwik_reports_period']) ? $_SESSION['piwik_reports_period'] : 0,
    '#options' => $period,
  );
  $form['basic']['inline']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
  );

  return $form;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function piwik_reports_overview($form, &$form_state) {
  global $user;

  // Get basic configuration variables.
  $piwik_url = variable_get('piwik_url_http', '');
  $piwik_site_id = variable_get('piwik_site_id', '');
  $piwik_auth = $user->data['piwik']['piwik_token_auth'];
  $period = $_SESSION['piwik_reports_period'];

  $now = _piwik_reports_select_period(0);
  if ($period == 1) {
    // Special handling for "yesterday" = 1.
    // Set yesterday date value for "from" and "to" date.
    $date = _piwik_reports_select_period($period);
    $now = $date;
  }
  else {
    // Otherwise it returns the start date value.
    $date = _piwik_reports_select_period($period);
  }

  // Granuarity of measuring points in graph.
  switch ($period) {
    case 4:
      $period = "week";
      break;

    default:
      $period = "day";
  }

  // Create an array of URL parameters for easier maintenance.
  $data1_params = array();
  $data1_params['module'] = 'VisitsSummary';
  $data1_params['action'] = 'getEvolutionGraph';
  $data1_params['columns[]'] = 'nb_visits';
  $data1_params['moduleToWidgetize'] = 'VisitsSummary';
  $data1_params['actionToWidgetize'] = 'getEvolutionGraph';
  $data1_params['idSite'] = $piwik_site_id;
  $data1_params['period'] = $period;
  $data1_params['date'] = $date . ',' . $now;
  $data1_params['disableLink'] = 1;
  $data1_params['viewDataTable'] = 'generateDataChartEvolution';
  $data1_params['loading'] = t('Loading data...');
  if (!empty($piwik_auth)) {
    $data1_params['token_auth'] = $piwik_auth;
  }

  // Build the data URL with all params and urlencode it.
  // Note: drupal_urlencode() doesn't work here!
  $data1_url = rawurlencode($piwik_url . '/index.php?' . drupal_http_build_query($data1_params));

  // Render HTML code.
  $output = theme('visitors_overview', array('piwik_url' => $piwik_url, 'data1_url' => $data1_url, 'period' => $period));
  $form = piwik_reports_dateselect_form();
  $form['content'] = array(
    '#type' => 'item',
    '#markup' => $output,
  );

  return $form;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function piwik_reports_overview_submit($form, &$form_state) {
dsm($form_state);
  $_SESSION['piwik_reports_period'] = $form_state['values']['period'];
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function piwik_reports_settings($form, &$form_state) {
  global $user;

  // Get basic configuration variables.
  $piwik_url = variable_get('piwik_url_http', '');
  $piwik_site_id = variable_get('piwik_site_id', '');
  $piwik_auth = $user->data['piwik']['piwik_token_auth'];
  $period = $_SESSION['piwik_reports_period'];

  if ($period == 1) {
    // Special handling for "yesterday" = 1.
    // The yesterday date value is required.
    $date = _piwik_reports_select_period($period);
  }
  else {
    // Otherwise it returns the today date value.
    $date = _piwik_reports_select_period(0);
  }
  $period_name = _piwik_reports_get_period_name($period);

  // Create an array of data URL parameters for easier maintenance.

  // Browser families
  $data1_params = array();
  $data1_params['module'] = 'UserSettings';
  $data1_params['action'] = 'getBrowserType';
  $data1_params['idSite'] = $piwik_site_id;
  $data1_params['period'] = $period_name;
  $data1_params['date'] = $date;
  $data1_params['filter_limit'] = 5;
  $data1_params['filter_sort_column'] = 'nb_uniq_visitors';
  $data1_params['filter_sort_order'] = 'desc';
  $data1_params['viewDataTable'] = 'generateDataChartPie';
  $data1_params['loading'] = t('Loading data...');
  if (!empty($piwik_auth)) {
    $data1_params['token_auth'] = $piwik_auth;
  }

  // Client configurations
  $data2_params = array();
  $data2_params['module'] = 'UserSettings';
  $data2_params['action'] = 'getConfiguration';
  $data2_params['idSite'] = $piwik_site_id;
  $data2_params['period'] = $period_name;
  $data2_params['date'] = $date;
  $data2_params['enable_sort'] = 1;
  $data2_params['filter_limit'] = 5;
  $data2_params['filter_sort_column'] = 'nb_uniq_visitors';
  $data2_params['filter_sort_order'] = 'desc';
  $data2_params['viewDataTable'] = 'generateDataChartPie';
  $data2_params['loading'] = t('Loading data...');
  if (!empty($piwik_auth)) {
    $data2_params['token_auth'] = $piwik_auth;
  }

  // Operating systems
  $data3_params = array();
  $data3_params['module'] = 'UserSettings';
  $data3_params['action'] = 'getOS';
  $data3_params['idSite'] = $piwik_site_id;
  $data3_params['period'] = $period_name;
  $data3_params['date'] = $date;
  $data3_params['filter_limit'] = 10;
  $data3_params['filter_sort_column'] = 'nb_uniq_visitors';
  $data3_params['filter_sort_order'] = 'desc';
  $data3_params['viewDataTable'] = 'generateDataChartPie';
  $data3_params['loading'] = t('Loading data...');
  if (!empty($piwik_auth)) {
    $data3_params['token_auth'] = $piwik_auth;
  }

  // Screen resolutions
  $data4_params = array();
  $data4_params['module'] = 'UserSettings';
  $data4_params['action'] = 'getResolution';
  $data4_params['idSite'] = $piwik_site_id;
  $data4_params['period'] = $period_name;
  $data4_params['date'] = $date;
  $data4_params['filter_limit'] = 10;
  $data4_params['filter_sort_column'] = 'nb_uniq_visitors';
  $data4_params['filter_sort_order'] = 'desc';
  $data4_params['viewDataTable'] = 'generateDataChartPie';
  $data4_params['loading'] = t('Loading data...');
  if (!empty($piwik_auth)) {
    $data4_params['token_auth'] = $piwik_auth;
  }

  // Build the data URL with all params and urlencode it.
  // Note: drupal_urlencode() doesn't work with Flash variables!
  $data1_url = rawurlencode($piwik_url . '/index.php?' . drupal_http_build_query($data1_params));
  $data2_url = rawurlencode($piwik_url . '/index.php?' . drupal_http_build_query($data2_params));
  $data3_url = rawurlencode($piwik_url . '/index.php?' . drupal_http_build_query($data3_params));
  $data4_url = rawurlencode($piwik_url . '/index.php?' . drupal_http_build_query($data4_params));

  // Render HTML code.
  // TODO Please change this theme call to use an associative array for the $variables parameter.
  $output = theme('visitors_settings', array('piwik_url' => $piwik_url,
                                            'data1_url' => $data1_url,
                                            'data2_url' => $data2_url,
                                            'data3_url' => $data3_url,
                                            'data4_url' => $data4_url,
                                            ));

  $form = piwik_reports_dateselect_form();
  $form['content'] = array(
    '#type' => 'item',
    '#markup' => $output,
  );

  return $form;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function piwik_reports_settings_submit($form, &$form_state) {
  $_SESSION['piwik_reports_period'] = $form_state['values']['period'];
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function piwik_reports_times($form, &$form_state) {
  global $user;

  // Get basic configuration variables.
  $piwik_url = variable_get('piwik_url_http', '');
  $piwik_site_id = variable_get('piwik_site_id', '');
  $piwik_auth = $user->data['piwik']['piwik_token_auth'];
  $period = $_SESSION['piwik_reports_period'];

  if ($period == 1) {
    // Special handling for "yesterday" = 1.
    // The yesterday date value is required.
    $date = _piwik_reports_select_period($period);
  }
  else {
    // Otherwise it returns the today date value.
    $date = _piwik_reports_select_period(0);
  }
  $period_name = _piwik_reports_get_period_name($period);

  // Create an array of data URL parameters for easier maintenance.
  $data1_params = array();
  $data1_params['module'] = 'VisitTime';
  $data1_params['action'] = 'getVisitInformationPerLocalTime';
  $data1_params['idSite'] = $piwik_site_id;
  $data1_params['period'] = $period_name;
  $data1_params['date'] = $date;
  $data1_params['viewDataTable'] = 'generateDataChartVerticalBar';
  $data1_params['loading'] = t('Loading data...');
  if (!empty($piwik_auth)) {
    $data1_params['token_auth'] = $piwik_auth;
  }

  // Build the data URL with all params and urlencode it.
  // Note: drupal_urlencode() doesn't work with Flash variables!
  $data1_url = rawurlencode($piwik_url . '/index.php?' . drupal_http_build_query($data1_params));

  $form = piwik_reports_dateselect_form();
  $output = theme('visitors_times', array('piwik_url' => $piwik_url, 'data1_url' => $data1_url));
  $form['content'] = array(
    '#type' => 'item',
    '#markup' => $output,
  );

  return $form;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function piwik_reports_times_submit($form, &$form_state) {
  $_SESSION['piwik_reports_period'] = $form_state['values']['period'];
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function piwik_reports_locations($form, &$form_state) {
  global $user;

  // Get basic configuration variables.
  $piwik_url = variable_get('piwik_url_http', '');
  $piwik_site_id = variable_get('piwik_site_id', '');
  $piwik_auth = $user->data['piwik']['piwik_token_auth'];
  $period = $_SESSION['piwik_reports_period'];

  if ($period == 1) {
    // Special handling for "yesterday" = 1.
    // The yesterday date value is required.
    $date = _piwik_reports_select_period($period);
  }
  else {
    // Otherwise it returns the today date value.
    $date = _piwik_reports_select_period(0);
  }
  $period_name = _piwik_reports_get_period_name($period);

  // Create an array of data URL parameters for easier maintenance.
  $data1_params = array();
  $data1_params['module'] = 'UserCountry';
  $data1_params['action'] = 'getCountry';
  $data1_params['idSite'] = $piwik_site_id;
  $data1_params['period'] = $period_name;
  $data1_params['date'] = $date;
  // FIXME: filter_limit seems broken.
  $data1_params['filter_limit'] = 10;
  $data1_params['viewDataTable'] = 'generateDataChartPie';
  $data1_params['loading'] = t('Loading data...');
  if (!empty($piwik_auth)) {
    $data1_params['token_auth'] = $piwik_auth;
  }

  // Build the data URL with all params and urlencode it.
  // Note: drupal_urlencode() doesn't work with Flash variables!
  $data1_url = rawurlencode($piwik_url . '/index.php?' . drupal_http_build_query($data1_params));

  $form = piwik_reports_dateselect_form();
  $output = theme('visitors_locations', array('piwik_url' => $piwik_url, 'data1_url' => $data1_url));
  $form['content'] = array(
    '#type' => 'item',
    '#markup' => $output,
  );

  return $form;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function piwik_reports_locations_submit($form, &$form_state) {
  $_SESSION['piwik_reports_period'] = $form_state['values']['period'];
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function piwik_reports_pages($form, &$form_state) {
  global $user;
  drupal_add_js(drupal_get_path('module', 'piwik_reports') . '/piwik_reports.js');

  // Get basic configuration variables.
  $piwik_url = variable_get('piwik_url_http', '');
  $piwik_site_id = variable_get('piwik_site_id', '');
  $piwik_auth = $user->data['piwik']['piwik_token_auth'];
  $period = $_SESSION['piwik_reports_period'];

  if ($period == 1) {
    // Special handling for "yesterday" = 1.
    // The yesterday date value is required.
    $date = _piwik_reports_select_period($period);
  }
  else {
    // Otherwise it returns the today date value.
    $date = _piwik_reports_select_period(0);
  }
  $period_name = _piwik_reports_get_period_name($period);

  // Create an array of data URL parameters for easier maintenance.
  $data1_params = array();
  $data1_params['module'] = 'API';
  $data1_params['method'] = 'Actions.getPageUrls';
  $data1_params['idSite'] = $piwik_site_id;
  $data1_params['period'] = $period_name;
  $data1_params['date'] = 'today';//$date;
  $data1_params['format'] = 'JSON';
  if (!empty($piwik_auth)) {
    $data1_params['token_auth'] = $piwik_auth;
  }

  // Build the data URL with all params.
  $data1_url = $piwik_url . 'index.php?' . drupal_http_build_query($data1_params) . '&jsoncallback=?';
  drupal_add_js(array('piwik_reports' => array('url' => $data1_url, 'page' => 'actions')), 'setting');
  $form = piwik_reports_dateselect_form();

  $form['content'] = array(
    '#type' => 'item',
    '#markup' => '<div id="pagestable"></div>',
  );

  return $form;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function piwik_reports_pages_submit($form, &$form_state) {
  $_SESSION['piwik_reports_period'] = $form_state['values']['period'];
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function piwik_reports_outlinks($form, &$form_state) {
  global $user;
  drupal_add_js(drupal_get_path('module', 'piwik_reports') . '/piwik_reports.js');

  // Get basic configuration variables.
  $piwik_url = variable_get('piwik_url_http', '');
  $piwik_site_id = variable_get('piwik_site_id', '');
  $piwik_auth = $user->data['piwik']['piwik_token_auth'];
  $period = $_SESSION['piwik_reports_period'];

  if ($period == 1) {
    // Special handling for "yesterday" = 1.
    // The yesterday date value is required.
    $date = _piwik_reports_select_period($period);
  }
  else {
    // Otherwise it returns the today date value.
    $date = _piwik_reports_select_period(0);
  }
  $period_name = _piwik_reports_get_period_name($period);

  // Create an array of data URL parameters for easier maintenance.
  $data1_params = array();
  $data1_params['module'] = 'API';
  $data1_params['method'] = 'Actions.getOutlinks';
  $data1_params['idSite'] = $piwik_site_id;
  $data1_params['period'] = $period_name;
  $data1_params['date'] = $date;
  $data1_params['format'] = 'JSON';
  if (!empty($piwik_auth)) {
    $data1_params['token_auth'] = $piwik_auth;
  }

  // Build the data URL with all params.
  $data1_url = $piwik_url . '/index.php?' . drupal_http_build_query($data1_params) . '&jsoncallback=?';
  drupal_add_js(array('piwik_reports' => array('url' => $data1_url, 'page' => 'actions')), 'setting');

  $form = piwik_reports_dateselect_form();
  $form['content'] = array(
    '#type' => 'item',
    '#markup' => '<div id="pagestable"></div>',
  );

  return $form;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function piwik_reports_outlinks_submit($form, &$form_state) {
  $_SESSION['piwik_reports_period'] = $form_state['values']['period'];
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function piwik_reports_downloads($form, &$form_state) {
  global $user;
  drupal_add_js(drupal_get_path('module', 'piwik_reports') . '/piwik_reports.js');

  // Get basic configuration variables.
  $piwik_url = variable_get('piwik_url_http', '');
  $piwik_site_id = variable_get('piwik_site_id', '');
  $piwik_auth = $user->data['piwik']['piwik_token_auth'];
  $period = $_SESSION['piwik_reports_period'];

  if ($period == 1) {
    // Special handling for "yesterday" = 1.
    // The yesterday date value is required.
    $date = _piwik_reports_select_period($period);
  }
  else {
    // Otherwise it returns the today date value.
    $date = _piwik_reports_select_period(0);
  }
  $period_name = _piwik_reports_get_period_name($period);

  // Create an array of data URL parameters for easier maintenance.
  $data1_params = array();
  $data1_params['module'] = 'API';
  $data1_params['method'] = 'Actions.getDownloads';
  $data1_params['idSite'] = $piwik_site_id;
  $data1_params['period'] = $period_name;
  $data1_params['date'] = $date;
  $data1_params['format'] = 'JSON';
  if (!empty($piwik_auth)) {
    $data1_params['token_auth'] = $piwik_auth;
  }

  // Build the data URL with all params.
  $data1_url = $piwik_url . '/index.php?' . drupal_http_build_query($data1_params) . '&jsoncallback=?';

  drupal_add_js(array('piwik_reports' => array('url' => $data1_url, 'page' => 'actions')), 'setting');
  $form = piwik_reports_dateselect_form();

  $form['content'] = array(
    '#type' => 'item',
    '#markup' => '<div id="pagestable"></div>',
  );

  return $form;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function piwik_reports_downloads_submit($form, &$form_state) {
  $_SESSION['piwik_reports_period'] = $form_state['values']['period'];
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function piwik_reports_evolution($form, &$form_state) {
  global $user;

  // Get basic configuration variables.
  $piwik_url = variable_get('piwik_url_http', '');
  $piwik_site_id = variable_get('piwik_site_id', '');
  $piwik_auth = $user->data['piwik']['piwik_token_auth'];
  $period = $_SESSION['piwik_reports_period'];

  $now = _piwik_reports_select_period(0);
  if ($period == 1) {
    // Special handling for "yesterday" = 1.
    // Set yesterday date value for "from" and "to" date.
    $date = _piwik_reports_select_period($period);
    $now = $date;
  }
  else {
    // Otherwise it returns the start date value.
    $date = _piwik_reports_select_period($period);
  }
  $period_name = _piwik_reports_get_period_name($period);

  // Granuarity of measuring points in graph.
  switch ($period_name) {
    case 4:
      $period_name = "week";
      break;

    default:
      $period_name = "day";
  }

  // Create an array of data URL parameters for easier maintenance.
  $data1_params = array();
  $data1_params['module'] = 'Referers';
  $data1_params['action'] = 'getRefererType';
  $data1_params['columns[]'] = 'nb_visits';
  $data1_params['moduleToWidgetize'] = 'Referers';
  $data1_params['actionToWidgetize'] = 'getRefererType';
  $data1_params['idSite'] = $piwik_site_id;
  $data1_params['period'] = $period_name;
  $data1_params['date'] = $date . ',' . $now;
  $data1_params['viewDataTable'] = 'generateDataChartEvolution';
  $data1_params['loading'] = t('Loading data...');
  if (!empty($piwik_auth)) {
    $data1_params['token_auth'] = $piwik_auth;
  }

  // Build the data URL with all params and urlencode it.
  // Note: drupal_urlencode() doesn't work with Flash variables!
  $data1_url = rawurlencode($piwik_url . '/index.php?' . drupal_http_build_query($data1_params));

  $form = piwik_reports_dateselect_form();
  $output = theme('referers_evolution', array('piwik_url' => $piwik_url, 'data1_url' => $data1_url));
  $form['content'] = array(
    '#type' => 'item',
    '#markup' => $output,
  );

  return $form;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function piwik_reports_evolution_submit($form, &$form_state) {
  $_SESSION['piwik_reports_period'] = $form_state['values']['period'];
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function piwik_reports_search($form, &$form_state) {
  global $user;
  drupal_add_js(drupal_get_path('module', 'piwik_reports') . '/piwik_reports.js');

  // Get basic configuration variables.
  $piwik_url = variable_get('piwik_url_http', '');
  $piwik_site_id = variable_get('piwik_site_id', '');
  $piwik_auth = $user->data['piwik']['piwik_token_auth'];
  $period = $_SESSION['piwik_reports_period'];

  if ($period == 1) {
    // Special handling for "yesterday" = 1.
    // The yesterday date value is required.
    $date = _piwik_reports_select_period($period);
  }
  else {
    // Otherwise it returns the today date value.
    $date = _piwik_reports_select_period(0);
  }
  $period_name = _piwik_reports_get_period_name($period);

  // Create an array of data URL parameters for easier maintenance.
  $data1_params = array();
  $data1_params['module'] = 'Referers';
  $data1_params['action'] = 'getSearchEngines';
  $data1_params['idSite'] = $piwik_site_id;
  $data1_params['period'] = $period_name;
  $data1_params['date'] = $date;
  $data1_params['viewDataTable'] = 'generateDataChartPie';
  $data1_params['loading'] = t('Loading data...');
  if (!empty($piwik_auth)) {
    $data1_params['token_auth'] = $piwik_auth;
  }

  // Build the data URL with all params and urlencode it.
  // Note: drupal_urlencode() doesn't work with Flash variables!
  $data1_url = rawurlencode($piwik_url . '/index.php?' . drupal_http_build_query($data1_params));

  // Create an array of data URL parameters for easier maintenance.
  $data2_params = array();
  $data2_params['module'] = 'API';
  $data2_params['method'] = 'Referers.getKeywords';
  $data2_params['idSite'] = $piwik_site_id;
  $data2_params['period'] = $period_name;
  $data2_params['date'] = $date;
  $data2_params['format'] = 'JSON';
  if (!empty($piwik_auth)) {
    $data2_params['token_auth'] = $piwik_auth;
  }

  // Build the data URL with all params.
  $data2_url = $piwik_url . '/index.php?' . drupal_http_build_query($data2_params) . '&jsoncallback=?';

  drupal_add_js(array('piwik_reports' => array('url' => $data2_url, 'page' => 'actions')), 'setting');
  $form = piwik_reports_dateselect_form();
  $output = theme('referers_search', array('piwik_url' => $piwik_url, 'data1_url' => $data1_url));
  $form['content'] = array(
    '#type' => 'item',
    '#markup' => $output,
  );
  $form['tablecontent'] = array(
    '#type' => 'item',
    '#markup' => '<div id="pagestable"></div>',
  );

  return $form;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function piwik_reports_search_submit($form, &$form_state) {
  $_SESSION['piwik_reports_period'] = $form_state['values']['period'];
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function piwik_reports_websites($form, &$form_state) {
  global $user;
  drupal_add_js(drupal_get_path('module', 'piwik_reports') . '/piwik_reports.js');

  // Get basic configuration variables.
  $piwik_url = variable_get('piwik_url_http', '');
  $piwik_site_id = variable_get('piwik_site_id', '');
  $piwik_auth = $user->data['piwik']['piwik_token_auth'];
  $period = $_SESSION['piwik_reports_period'];

  if ($period == 1) {
    // Special handling for "yesterday" = 1.
    // The yesterday date value is required.
    $date = _piwik_reports_select_period($period);
  }
  else {
    // Otherwise it returns the today date value.
    $date = _piwik_reports_select_period(0);
  }
  $period_name = _piwik_reports_get_period_name($period);

  // Create an array of data URL parameters for easier maintenance.
  $data1_params = array();
  $data1_params['module'] = 'API';
  $data1_params['method'] = 'Referers.getWebsites';
  $data1_params['idSite'] = $piwik_site_id;
  $data1_params['period'] = $period_name;
  $data1_params['date'] = $date;
  $data1_params['format'] = 'JSON';
  if (!empty($piwik_auth)) {
    $data1_params['token_auth'] = $piwik_auth;
  }

  // Build the data URL with all params.
  $data1_url = $piwik_url . '/index.php?' . drupal_http_build_query($data1_params) . '&jsoncallback=?';

  drupal_add_js(array('piwik_reports' => array('url' => $data1_url, 'page' => 'actions')), 'setting');
  $form = piwik_reports_dateselect_form();
  $form['content'] = array(
    '#type' => 'item',
    '#markup' => '<div id="pagestable"></div>',
  );

  return $form;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function piwik_reports_websites_submit($form, &$form_state) {
  $_SESSION['piwik_reports_period'] = $form_state['values']['period'];
}

function _piwik_reports_select_period($period) {
  switch ($period) {
    case 0:
      $date = date("Y-m-d");
      break;

    case 1:
      $d = mktime(0, 0, 0, date("m"), date("d") - 1, date("Y"));
      $date = date("Y-m-d", $d);
      break;

    case 2:
      $d = mktime(0, 0, 0, date("m"), date("d") - 7, date("Y"));
      $date = date("Y-m-d", $d);
      break;

    case 3:
      $d = mktime(0, 0, 0, date("m") - 1, date("d"), date("Y"));
      $date = date("Y-m-d", $d);
      break;

    case 4:
      $d = mktime(0, 0, 0, date("m"), date("d"), date("Y") - 1);
      $date = date("Y-m-d", $d);
      break;
  }
  return $date;
}

function _piwik_reports_get_period_name($period) {
  // Possible periods are day, week, month, year.
  switch ($period) {
    case 0:
      $p = "day";
      break;

    case 1:
      $p = "day";
      break;

    case 2:
      $p = "week";
      break;

    case 3:
      $p = "month";
      break;

    case 4:
      $p = "year";
      break;
  }
  return $p;
}
