<?php

/**
 * @file
 * Helper file for admin UI.
 */

/**
 * @param $api_key
 * @return int
 *
 * Check API Credentials is valid ot not.
 */
function socialloginandsocialshare_isvalid_apikey($api_key) {
  return preg_match('/^\{?[A-Z0-9]{8}-[A-Z0-9]{4}-[A-Z0-9]{4}-[A-Z0-9]{4}-[A-Z0-9]{12}\}?$/i', $api_key);
}

/**
 * @param $form_state
 * A keyed array containing the current state of the form.
 *
 * Save all form element values into database.
 */
function socialloginandsocialshare_save_setings(&$form_state) {
  foreach ($form_state['values'] as $key => $value) {
    if (is_array($value) && isset($form_state['values']['array_filter'])) {
      $value = array_keys(array_filter($value));
    }
    variable_set($key, $value);
  }

  socialloginandsocialshare_call_settings_validation();
  drupal_set_message(t('The configuration options have been saved.'));
  // Clear the cached pages and blocks.
  cache_clear_all();
  menu_rebuild();
}

/**
 * @param $form
 * @param $form_state
 * A keyed array containing the current state of the form.
 *
 * Validate social login tab settings.
 */
function socialloginandsocialshare_admin_settings_validate($form, &$form_state) {
  if (!class_exists(LoginRadius)) {
    include 'socialloginandsocialshare.loginradius.inc';
  }

  unset($_SESSION['messages']['warning']);
  unset($_SESSION['messages']['error']);
  $apikey = trim($form_state['values']['socialloginandsocialshare_apikey']);
  $apisecret = trim($form_state['values']['socialloginandsocialshare_apisecret']);

  if (trim($apikey) == trim($apisecret)) {
    drupal_set_message(t('API Key and Secret cannot be same. Please paste correct API Key and Secret from your LoginRadius account in the corresponding fields'), 'error');
  }
  elseif (!socialloginandsocialshare_isvalid_apikey($apikey)) {
    drupal_set_message(t('Your LoginRadius API key is not valid, please correct it or contact LoginRadius support at <a href="@get_socialloginandsocialshare" target="_blank">www.Loginradius.com</a>', array('@get_socialloginandsocialshare' => 'http://www.Loginradius.com/')), 'error');
  }
  elseif (!socialloginandsocialshare_isvalid_apikey($apisecret)) {
    drupal_set_message(t('Your LoginRadius API Secret is not valid, please correct it or contact LoginRadius support at <a href="@get_socialloginandsocialshare" target="_blank">www.Loginradius.com</a>', array('@get_socialloginandsocialshare' => 'http://www.Loginradius.com/')), 'error');
  }
}

/**
 * @param $form
 * @param $form_state
 *
 * Submit Social Login settings.
 */
function socialloginandsocialshare_admin_settings_submit($form, &$form_state) {
  $settings = array(
    'socialloginandsocialshare_label_string' => 'Login with Social ID',
    'socialloginandsocialshare_interface_size' => '0',
    'socialloginandsocialshare_attach_login_form' => '1',
    'socialloginandsocialshare_loginpage_position' => '1',
    'socialloginandsocialshare_registerpage_position' => '1',
    'socialloginandsocialshare_showicon_location' => '0',
    'socialloginandsocialshare_force_registration' => '1',
    'socialloginandsocialshare_email_required' => '1',
    'socialloginandsocialshare_emailrequired_popup_top' => 'You are trying to connect with @provider',
    'socialloginandsocialshare_emailrequired_popup_text' => 'Unfortunately we could not retrieve your email from your @provider account Please enter your email in the form below in order to continue.',
    'socialloginandsocialshare_emailrequired_popup_wrong' => 'This email is already registered. Please enter another email or link this account via account linking on your profile page',
    'socialloginandsocialshare_send_user_credentials' => '1',
  );

  foreach ($settings as $key => $values) {
    if (!variable_get($key, FALSE) && variable_get($key, '') != '0') {
      variable_set($key, $values);
    }
  }

  if (module_exists('profile')) {
    module_load_include('inc', 'socialloginandsocialshare', 'socialloginandsocialshare.field');

    if (isset($form_state['values']['socialloginandsocialshare_user_fields'])) {
      variable_set('socialloginandsocialshare_user_profile', array_filter($form_state['values']['socialloginandsocialshare_user_profile']));
    }
  }

  socialloginandsocialshare_save_setings($form_state);
}

/**
 * @param $form
 * @param $form_state
 *
 * Submit Advance  settings.
 */
function socialloginandsocialshare_advance_settings_form_submit($form, &$form_state) {
  socialloginandsocialshare_save_setings($form_state);
}

/**
 * @param $form
 * @param $form_state
 *
 * Submit the Social share settings.
 */
function socialloginandsocialshare_sharebar_settings_submit($form, &$form_state) {
  socialloginandsocialshare_save_network_providers($form_state);
  socialloginandsocialshare_save_setings($form_state);
}

/**
 * @param $form_state
 * A keyed array containing the current state of the form.
 *
 * Get sharing/Counter providers into database.
 */
function socialloginandsocialshare_save_network_providers(&$form_state) {
  // Set default value of sharing networks.
  $settings = array(
    'socialloginandsocialshare_rearrange_providers_list',
    'socialcounter_rearrange_providers_list',
    'socialshare_vertical_rearrange_providers_list',
    'socialcounter_vertical_rearrange_providers_list'
  );

  foreach ($settings as $value) {
    socialloginandsocialshare_save_network_providers_submit($form_state, $value);
  }
}

/**
 * @param $form_state
 * A keyed array containing the current state of the form.
 * @param $network
 * database key for sharing/counter.
 *
 * Save the sharing/counter networks.
 */
function socialloginandsocialshare_save_network_providers_submit(&$form_state, $network) {
  if (isset($form_state['clicked_button']['#post'][$network])) {
    $form_state['clicked_button']['#post'][$network] = array_filter($form_state['clicked_button']['#post'][$network]);
  }
  else {
    $form_state['clicked_button']['#post'][$network] = socialloginandsocialshare_get_default_sharing_networks($network);
  }
}

/**
 * @param $key
 * Database key for sharing/counter.
 * @return array
 * default sharing/counter networks
 *
 * Get default values of sharing/counter networks.
 */
function socialloginandsocialshare_get_default_sharing_networks($key) {
  if ($key == 'socialloginandsocialshare_rearrange_providers_list' || $key == 'socialshare_vertical_rearrange_providers_list') {
    $data = array(
      "0" => "facebook",
      "1" => "googleplus",
      "2" => "google",
      "3" => "pinterest",
      "4" => "twitter",
    );
  }
  else {
    $data = array(
      "0" => "Facebook Like",
      "1" => "Google+ +1",
      "2" => "Pinterest Pin it",
      "3" => "LinkedIn Share",
    );
  }

  return $data;
}

/**
 * Validate module settings.
 */
function socialloginandsocialshare_call_settings_validation() {
  module_load_include('inc', 'socialloginandsocialshare', 'socialloginandsocialshare.settings_validation');
  $string = socialloginandsocialshare_settings_bulid();
  socialloginandsocialshare_settings_validation($string);
}
