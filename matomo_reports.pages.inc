<?php

/**
 * @file
 * Reports callback for Matomo Reports.
 */

/**
 * Return the $form that handles Matomo reports.
 *
 * @param array $form
 *   See form API.
 * @param array $form_state
 *   See form API.
 * @param string $report
 *   The report name.
 *
 * @return array|null
 *   See Form API.
 */
function matomo_reports_reports(array $form, array &$form_state, $report) {
  $config = config('matomo_reports.settings');
  global $user;

  $matomo_auth = (check_plain($config->get('matomo_reports_token_auth') ? $config->get('matomo_reports_token_auth') : $user->data['matomo_reports_token_auth']));
  $matomo_url = matomo_reports_get_url();
  $sites = matomo_reports_get_sites($matomo_auth);
  if (empty($sites)) {
    $_SESSION['matomo_reports_site'] = '';
    backdrop_set_message(t('You cannot access any data on the selected Matomo server. Please check authentication string and permissions with your Matomo server administrator.'), 'warning');
    return;
  }
  $form['#attributes'] = array(
    'class' => array(
      'search-form',
      'container-inline',
    ),
  );
  $period = array(
    0 => t('Today'),
    1 => t('Yesterday'),
    2 => t('Last week'),
    3 => t('Last month'),
    4 => t('Last year'),
  );
  $form['matomo_filters'] = array(
    '#type' => 'fieldset',
    '#title' => t('Select site and time period'),
  );
  $form['matomo_filters']['period'] = array(
    '#type' => 'select',
    '#title' => '',
    '#default_value' => isset($_SESSION['matomo_reports_period']) ? $_SESSION['matomo_reports_period'] : 0,
    '#options' => $period,
  );
  $options = array();
  $allowed_keys = explode(',', $config->get('matomo_reports_allowed_sites'));
  $session_site_exists = FALSE;
  foreach ($sites as $site) {
    if (empty($allowed_keys[0]) || in_array($site['idsite'], $allowed_keys)) {
      $options[$site['idsite']] = $site['name'];
    }
    if (isset($_SESSION['matomo_reports_site']) && $_SESSION['matomo_reports_site'] == $site['idsite']) {
      $session_site_exists = TRUE;
    }
  }
  if (!isset($_SESSION['matomo_reports_site']) || $_SESSION['matomo_reports_site'] == '' || !$session_site_exists || !in_array($_SESSION['matomo_reports_site'], $allowed_keys)) {
    // When not set, set to first of the allowed sites.
    $_SESSION['matomo_reports_site'] = $allowed_keys[0];
  }
  if (count($options) > 1) {
    $form['matomo_filters']['site'] = array(
      '#type' => 'select',
      '#title' => t('Site:'),
      '#weight' => -5,
      '#default_value' => $_SESSION['matomo_reports_site'],
      '#options' => $options,
    );
  }
  elseif (count($options) == 1) {
    foreach ($options as $siteid => $sitename) {
      break;
    }
    $form['matomo_filters']['site'] = array(
      '#type' => 'hidden',
      '#value' => $siteid,
    );
    $form['matomo_filters']['sitename'] = array(
      '#type' => 'textfield',
      '#title' => t('Site:'),
      '#weight' => -5,
      '#size' => 25,
      '#value' => $sitename,
      '#disabled' => TRUE,
    );
    $form['matomo_filters']['period']['#attributes'] = array('onchange' => 'this.form.submit();');
  }
  $form['matomo_filters']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
    '#weight' => 15,
  );

  $matomo_site_id = $_SESSION['matomo_reports_site'];
  $period = isset($_SESSION['matomo_reports_period']) ? $_SESSION['matomo_reports_period'] : 0;

  if ($period == 1) {
    // Special handling for "yesterday" = 1.
    // The yesterday date value is required.
    $date = _matomo_reports_select_period($period);
  }
  else {
    // Otherwise it returns the today date value.
    $date = _matomo_reports_select_period(0);
  }
  $period_name = _matomo_reports_get_period_name($period);

  // Create an array of URL parameters for easier maintenance.
  $data_params[0] = array();
  $data_params[0]['idSite'] = $matomo_site_id;
  $data_params[0]['date'] = $date;
  $data_params[0]['period'] = $period_name;
  $data_params[0]['disableLink'] = 1;
  $data_params[0]['module'] = 'Widgetize';
  $data_params[0]['action'] = 'iframe';
  $data_params[0]['disableLink'] = 1;
  $data_params[0]['widget'] = 1;

  if (!empty($matomo_auth)) {
    $data_params[0]['token_auth'] = $matomo_auth;
  }

  switch ($report) {
    case 'visitors_overview':
      $iframe_height[0] = 950;
      $title[0] = t('Visits trend');
      $data_params[0]['columns[]'] = 'nb_visits';
      $data_params[0]['moduleToWidgetize'] = 'VisitsSummary';
      $data_params[0]['actionToWidgetize'] = 'index';
      break;

    case 'visitors_times':
      $title[0] = t('Visits by local time');
      $data_params[0]['moduleToWidgetize'] = 'VisitTime';
      $data_params[0]['actionToWidgetize'] = 'getVisitInformationPerLocalTime';
      break;

    case 'visitors_settings':
      $data_params[0]['filter_limit'] = 6;

      $data_params[1] = $data_params[0];
      $data_params[2] = $data_params[0];
      $data_params[3] = $data_params[0];
      // Browser families.
      $title[0] = t('Browser families');
      $data_params[0]['moduleToWidgetize'] = 'DevicesDetection';
      $data_params[0]['actionToWidgetize'] = 'getBrowserEngines';
      // Screen resolutions.
      $title[1] = t('Screen resolution');
      $data_params[1]['moduleToWidgetize'] = 'Resolution';
      $data_params[1]['actionToWidgetize'] = 'getConfiguration';
      // Operating systems.
      $title[2] = t('Operating system');
      $data_params[2]['moduleToWidgetize'] = 'DevicesDetection';
      $data_params[2]['actionToWidgetize'] = 'getOsVersions';
      // Client configurations.
      $title[3] = t('Client configuration');
      $data_params[3]['moduleToWidgetize'] = 'Resolution';
      $data_params[3]['actionToWidgetize'] = 'getResolution';
      break;

    case 'visitors_locations':
      $title[0] = t('Visitors countries');
      $iframe_height[0] = 750;
      $data_params[0]['moduleToWidgetize'] = 'UserCountry';
      $data_params[0]['actionToWidgetize'] = 'getCountry';
      $data_params[0]['filter_limit'] = 15;
      break;

    case 'custom_variables':
      $title[0] = t('Custom variables');
      $iframe_height[0] = 1000;
      $data_params[0]['moduleToWidgetize'] = 'CustomVariables';
      $data_params[0]['actionToWidgetize'] = 'getCustomVariables';
      $data_params[0]['filter_limit'] = 15;
      break;

    case 'actions_pageurls':
      $title[0] = t('Page visits');
      $iframe_height[0] = 750;
      $data_params[0]['moduleToWidgetize'] = 'Actions';
      $data_params[0]['actionToWidgetize'] = 'getPageUrls';
      $data_params[0]['filter_limit'] = 15;
      break;

    case 'actions_entryurls':
      $title[0] = t('Page visits');
      $iframe_height[0] = 750;
      $data_params[0]['moduleToWidgetize'] = 'Actions';
      $data_params[0]['actionToWidgetize'] = 'getEntryPageUrls';
      $data_params[0]['filter_limit'] = 15;
      break;

    case 'actions_exiturls':
      $title[0] = t('Page visits');
      $iframe_height[0] = 750;
      $data_params[0]['moduleToWidgetize'] = 'Actions';
      $data_params[0]['actionToWidgetize'] = 'getExitPageUrls';
      $data_params[0]['filter_limit'] = 15;
      break;

    case 'actions_sitesearch':
      $data_params[1] = $data_params[0];
      $data_params[2] = $data_params[0];
      $data_params[3] = $data_params[0];

      $title[0] = t('Site Search Keywords');
      $iframe_height[0] = 750;
      $data_params[0]['moduleToWidgetize'] = 'Actions';
      $data_params[0]['actionToWidgetize'] = 'getSiteSearchKeywords';
      $data_params[0]['filter_limit'] = 15;
      // Pages following search.
      $title[1] = t('Pages Following a Site Search');
      $data_params[1]['moduleToWidgetize'] = 'Actions';
      $data_params[1]['actionToWidgetize'] = 'getPageUrlsFollowingSiteSearch';
      // No results.
      $title[2] = t('Site Search No Result Keyword');
      $data_params[2]['moduleToWidgetize'] = 'Actions';
      $data_params[2]['actionToWidgetize'] = 'getSiteSearchNoResultKeywords';
      // Categories.
      $title[3] = t('Site Search Categories');
      $data_params[3]['moduleToWidgetize'] = 'Actions';
      $data_params[3]['actionToWidgetize'] = 'getSiteSearchCategories';
      break;

    case 'actions_outlinks':
      $title[0] = '';
      $iframe_height[0] = 750;
      $data_params[0]['moduleToWidgetize'] = 'Actions';
      $data_params[0]['actionToWidgetize'] = 'getOutlinks';
      $data_params[0]['filter_limit'] = 15;
      break;

    case 'actions_downloads':
      $title[0] = '';
      $iframe_height[0] = 750;
      $data_params[0]['moduleToWidgetize'] = 'Actions';
      $data_params[0]['actionToWidgetize'] = 'getDownloads';
      $data_params[0]['filter_limit'] = 15;
      break;

    case 'referrers_overview':
      $data_params[1] = $data_params[0];
      // Types.
      $title[0] = '';
      $data_params[0]['moduleToWidgetize'] = 'Referrers';
      $data_params[0]['actionToWidgetize'] = 'getReferrerType';
      // Referrers.
      $title[1] = t('Referrers');
      $data_params[1]['moduleToWidgetize'] = 'Referrers';
      $data_params[1]['actionToWidgetize'] = 'getAll';
      break;

    case 'referrers_search':
      $data_params[1] = $data_params[0];

      $title[0] = t('Search engines');
      $data_params[0]['moduleToWidgetize'] = 'Referrers';
      $data_params[0]['actionToWidgetize'] = 'getSearchEngines';

      $title[1] = t('Keywords');
      $data_params[1]['moduleToWidgetize'] = 'Referrers';
      $data_params[1]['actionToWidgetize'] = 'getKeywords';
      break;

    case 'referrers_websites':
      $data_params[1] = $data_params[0];
      $title[0] = '';
      $iframe_height[0] = 1020;
      $data_params[0]['moduleToWidgetize'] = 'Referrers';
      $data_params[0]['actionToWidgetize'] = 'getWebsites';

      $title[1] = t('Social Networks');
      $data_params[1]['moduleToWidgetize'] = 'Referrers';
      $data_params[1]['actionToWidgetize'] = 'getSocials';
      break;

    case 'goals':
      $goals = matomo_reports_get_goals($matomo_auth, $_SESSION['matomo_reports_site']);
      if (count($goals) == 0) {
        $empty_text = t('No goals have been set. Check with your Matomo server administrator if you desire some.');
        $title[0] = NULL;
        break;
      }
      $common_data_params = $data_params[0];
      $i = 0;
      foreach ($goals as $goal) {
        $title[$i] = $goal['name'];
        $data_params[$i] = $common_data_params;
        $data_params[$i]['moduleToWidgetize'] = 'Goals';
        $data_params[$i]['actionToWidgetize'] = 'widgetGoalReport';
        $data_params[$i]['idGoal'] = $goal['idgoal'];
        $i++;
      }
      break;
  }
dpm($data_params, '$data_params');
  // Build the data URL with all params and urlencode it.
  $theme_args = array();
  foreach ($data_params as $key => $data) {
dpm($data, '$data');
//      <iframe src="https://matomo.techccessible.com/index.php?module=Widgetize&action=iframe&moduleToWidgetize=Dashboard&actionToWidgetize=index&idSite=22&period=week&date=yesterday" frameborder="0" marginheight="0" marginwidth="0" width="100%" height="100%"></iframe>
    $data['moduleToWidgetize'] = 'Dashboard';
    $data['actionToWidgetize'] = 'index';
    unset($data['disableLink']);
    unset($data['widget']);
    unset($data['filter_limit']);
    $theme_args['data_url'][] = array(
//      'url' => $matomo_url . 'index.php?' . backdrop_http_build_query($data),
      'url' => 'https://matomo.techccessible.com/index.php?module=Widgetize&action=iframe&moduleToWidgetize=Dashboard&actionToWidgetize=index&idSite=22&period=week&date=yesterday&token_auth=6f2eac9d3f375ec74886fa9d1289742c',
      'title' => $title[$key],
      'iframe_height' => (isset($iframe_height[$key]) && $iframe_height[$key] > 0 ? $iframe_height[$key] : 400),
      'empty_text' => (isset($empty_text) ? $empty_text : NULL),
    );
  }
  // Render HTML code.
dpm($theme_args, '$theme_args');
  $output = theme('matomo_reports', $theme_args);
  $form['content'] = array(
    '#type' => 'item',
    '#markup' => $output,
  );
  return $form;
}

/**
 * Reports form submit function.
 */
function matomo_reports_reports_submit($form, &$form_state) {
  $_SESSION['matomo_reports_period'] = $form_state['values']['period'];
  $_SESSION['matomo_reports_site'] = $form_state['values']['site'];
}

/**
 * Returns the starting and ending dates according to the selected period.
 *
 * @param int $period
 *   Selected period.
 *
 * @return string
 *   Formatted date.
 */
function _matomo_reports_select_period($period) {
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
 * Helper function to return the name of the selected period.
 *
 * @param int $period
 *   Selected period.
 *
 * @return string
 *   Name of period.
 */
function _matomo_reports_get_period_name($period) {
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
