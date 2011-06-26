<?php

/**
 * @file
 * Reports callback for Piwik Reports
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
 * Return the form where to select the report period.
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
 * Return the $form that handles piwik reports
 *
 * @param $form
 * @param $form_state
 *   see form api.
 * @param $report
 *   The report name.
 *
 * @return
 */
function piwik_reports_reports($form, &$form_state, $report) {
  global $user;

  // Get basic configuration variables.
  $piwik_auth = (variable_get('piwik_reports_token_auth', '') ? variable_get('piwik_reports_token_auth', '') : $user->data['piwik_reports_token_auth']);
  $piwik_url = piwik_reports_get_url();
  $piwik_site_id = 3;//variable_get('piwik_site_id', '');
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

/*  if (in_array($report, array('visitors_overview', 'referers_evolution'))) {
    $now = _piwik_reports_select_period(0);
    if ($period == 1) {
      $date = _piwik_reports_select_period($period);
      $now = $date;
    }
    else {
      // Otherwise it returns the start date value.
      $date = _piwik_reports_select_period($period);
    }

    $date = $date . ',' . $now;

    // Granuarity of measuring points in graph.
    switch ($period) {
      case 4:
        $period_name = "week";
        break;

      default:
        $period_name = "day";
    }
  }*/

  // Create an array of URL parameters for easier maintenance.
  $data1_params = array();
  $data1_params['idSite'] = $piwik_site_id;
  $data1_params['date'] = $date;
  $data1_params['period'] = $period_name;
  $data1_params['disableLink'] = 1;
  $data1_params['module'] = 'Widgetize';
  $data1_params['action'] = 'iframe';
  $data1_params['disableLink'] = 1;
  $data1_params['widget'] = 1;
 // $data1_params['loading'] = t('Loading data...');
  if (!empty($piwik_auth)) {
    $data1_params['token_auth'] = $piwik_auth;
  }

  switch ($report) {
    case 'visitors_overview':
      $title1 = t('Visits trend');
      $data1_params['columns[]'] = 'nb_visits';
      $data1_params['moduleToWidgetize'] = 'VisitsSummary';
      $data1_params['actionToWidgetize'] = 'getEvolutionGraph';
    break;
    case 'visitors_times':
      $title1 = t('Visits by local time');
      $data1_params['moduleToWidgetize'] = 'VisitTime';
      $data1_params['actionToWidgetize'] = 'getVisitInformationPerLocalTime';
    break;
    case 'visitors_settings':
      $data1_params['filter_limit'] = 6;

      $data2_params = $data1_params;
      $data3_params = $data1_params;
      $data4_params = $data1_params;
      // Browser families
      $title1 = t('Browser families');
      $data1_params['moduleToWidgetize'] = 'UserSettings';
      $data1_params['actionToWidgetize'] = 'getBrowserType';
      // Screen resolutions
      $title2 = t('Screen resolution');
      $data2_params['moduleToWidgetize'] = 'UserSettings';
      $data2_params['actionToWidgetize'] = 'getConfiguration';
      // Operating systems
      $title3 = t('Operating system');
      $data3_params['moduleToWidgetize'] = 'UserSettings';
      $data3_params['actionToWidgetize'] = 'getOS';
      // Client configurations
      $title4 = t('Client configuration');
      $data4_params['moduleToWidgetize'] = 'UserSettings';
      $data4_params['actionToWidgetize'] = 'getResolution';
    break;
    case 'visitors_locations':
      $title1 = t('Visitors countries');
      $data1_params['moduleToWidgetize'] = 'UserCountry';
      $data1_params['actionToWidgetize'] = 'getCountry';
      $data1_params['filter_limit'] = 15;
    break;
    case 'actions_pageurls':
      $title1 = t('Page visits');
      $data1_params['moduleToWidgetize'] = 'Actions';
      $data1_params['actionToWidgetize'] = 'getPageUrls';
      $data1_params['filter_limit'] = 15;
    break;
    case 'actions_outlinks':
      $title1 = t('');
      $data1_params['moduleToWidgetize'] = 'Actions';
      $data1_params['actionToWidgetize'] = 'getOutlinks';
      $data1_params['filter_limit'] = 15;
    break;
    case 'actions_downloads':
      $title1 = t('');
      $data1_params['moduleToWidgetize'] = 'Actions';
      $data1_params['actionToWidgetize'] = 'getDownloads';
      $data1_params['filter_limit'] = 15;
    break;
    case 'referers_evolution':
      $data1_params['module'] = 'Referers';
      $data1_params['action'] = 'getRefererType';
      $data1_params['columns[]'] = 'nb_visits';
      $data1_params['moduleToWidgetize'] = 'Referers';
      $data1_params['actionToWidgetize'] = 'getRefererType';
      $data1_params['viewDataTable'] = 'generateDataChartEvolution';
    break;
    case 'referers_search':
      $data2_params = $data1_params;

      $data1_params['module'] = 'Referers';
      $data1_params['action'] = 'getSearchEngines';
      $data1_params['viewDataTable'] = 'generateDataChartPie';

      $data2_params['module'] = 'API';
      $data2_params['method'] = 'Referers.getKeywords';
      $data2_params['format'] = 'JSON';
      unset($data2_params['loading']);
    break;
    case 'referers_websites':
      $data1_params['module'] = 'API';
      $data1_params['method'] = 'Referers.getWebsites';
      $data1_params['format'] = 'JSON';
    break;
  }

  // Build the data URL with all params and urlencode it.
  $theme_args['data_url'][] = array('url' => $piwik_url . 'index.php?' . drupal_http_build_query($data1_params), 'title' => $title1,);
   if ($report == 'visitors_settings') {
    $theme_args['data_url'][] = array('url' => $piwik_url . 'index.php?' . drupal_http_build_query($data2_params), 'title' => $title2,);
    $theme_args['data_url'][] = array('url' => $piwik_url . 'index.php?' . drupal_http_build_query($data3_params), 'title' => $title3,);
    $theme_args['data_url'][] = array('url' => $piwik_url . 'index.php?' . drupal_http_build_query($data4_params), 'title' => $title4,);
  }

  // Render HTML code.
  $output = theme('piwik_reports', $theme_args);

  $form = piwik_reports_dateselect_form();

  $form['content'] = array(
    '#type' => 'item',
    '#markup' => $output,
  );

  if ($report == 'referers_search') {
    drupal_add_js(drupal_get_path('module', 'piwik_reports') . '/piwik_reports.js');
    // Build the data URL with all params.
    $data2_url = $piwik_url . 'index.php?' . drupal_http_build_query($data2_params) . '&jsoncallback=?';
    drupal_add_js(array('piwik_reports' => array('url' => $data2_url, 'page' => 'actions')), 'setting');
    $form['tablecontent'] = array(
      '#type' => 'item',
      '#markup' => '<div id="pagestable"></div>',
    );
  }

  return $form;
}

/**
 * Reports form submit function
 */
function piwik_reports_reports_submit($form, &$form_state) {
  $_SESSION['piwik_reports_period'] = $form_state['values']['period'];
}

/**
 * Helper function that return the starting and ending dates according to the
 * selected period.
 *
 * @param $period
 *    selected period 
 * @return
 *    array with starting and ending date
 */
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

/**
 * Helper function that return the name of the selected period.
 *
 * @param $period
 *    selected period 
 * @return
 *    name
 */
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
