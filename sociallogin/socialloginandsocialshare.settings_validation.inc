<?php

/**
 * @file
 * Helper function file to build and validate settings.
 */

/**
 * 
 * @return string
 * 
 * Bulid social login and sharing settings string.
 */
function socialloginandsocialshare_settings_bulid() {
  module_load_include('inc', 'socialloginandsocialshare', 'socialloginandsocialshare.admin_functions');

  $string = '~1#' . variable_get('socialloginandsocialshare_disable_traditional_login', 0) . '|' . variable_get('socialloginandsocialshare_display_username', 0);
  $string .= '|' . variable_get('socialloginandsocialshare_userlogin_redirect', 0) . '|' . variable_get('socialloginandsocialshare_userregister_redirect', 0);
  $string .= '|' . variable_get('socialloginandsocialshare_account_linking', 1) . '|"' . variable_get('socialloginandsocialshare_identity_label_string', 'Link to an existing social account') . '"';
  $string .= '~2#' . variable_get('socialloginandsocialshare_enable_horizontal_share', 0) . '|' . variable_get('socialloginandsocialshare_share_horizontal_images', 0) . '|["' . implode('","', variable_get('socialloginandsocialshare_rearrange_providers_list', socialloginandsocialshare_get_default_sharing_networks('socialloginandsocialshare_rearrange_providers_list'))) . '"]';
  $string .= '|["' . implode('","', variable_get('socialcounter_rearrange_providers_list', socialloginandsocialshare_get_default_sharing_networks('socialcounter_rearrange_providers_list'))) . '"]' . '|' . variable_get('socialshare_position_top', 1) . '|' . variable_get('socialshare_position_bottom', 1) .
      '|' . variable_get('socialshare_show_pages', 0) . '|"' . variable_get('socialshare_show_exceptpages', '') . '"';
  $string .= '|' . variable_get('socialloginandsocialshare_enable_vertical_share', 0) . '|' . variable_get('socialloginandsocialshare_share_vertical_images', 4) . '|' . variable_get('socialsharing_vertical_position', 0) . '|["' . implode('","', variable_get('socialshare_vertical_rearrange_providers_list', socialloginandsocialshare_get_default_sharing_networks('socialshare_vertical_rearrange_providers_list'))) . '"]';
  $string .= '|["' . implode('","', variable_get('socialcounter_vertical_rearrange_providers_list', socialloginandsocialshare_get_default_sharing_networks('socialcounter_vertical_rearrange_providers_list'))) . '"]' . '|' . variable_get('socialshare_vertical_show_pages', 0) . '|"' . variable_get('socialshare_vertical_show_exceptpages', '') . '"';
  $string .= '~3#"' . variable_get('socialloginandsocialshare_label_string', 'Login with Social ID') . '"|' . variable_get('socialloginandsocialshare_interface_size', 0);
  $string .= '|' . variable_get('socialloginandsocialshare_interface_columns', '') . '|"' . variable_get('socialloginandsocialshare_interface_bgcolor', '') . '"';
  $string .= '|' . variable_get('socialloginandsocialshare_attach_login_form', 1) . '|' . variable_get('socialloginandsocialshare_loginpage_position', 1);
  $string .= '|' . variable_get('socialloginandsocialshare_registerpage_position', 1) . '|' . variable_get('socialloginandsocialshare_commenting_enable', 1);
  $string .= '|' . variable_get('socialloginandsocialshare_showicon_location', 0) . '|' . variable_get('socialloginandsocialshare_enable_user_registration', 1);
  $string .= '|' . variable_get('socialloginandsocialshare_force_registration', 1) . '|' . variable_get('socialloginandsocialshare_email_required', 1);
  $string .= '|"' . variable_get('socialloginandsocialshare_emailrequired_popup_top', 'You are trying to connect with @provider') . '"';
  $string .= '|"' . variable_get('socialloginandsocialshare_emailrequired_popup_text', 'Unfortunately we could not retrieve your email from your @provider account Please enter your email in the form below in order to continue.') . '"';
  $string .= '|"' . variable_get('socialloginandsocialshare_emailrequired_popup_wrong', 'This email is already registered. Please enter another email or link this account via account linking on your profile page') . '"';
  $string .= '|' . variable_get('socialloginandsocialshare_send_user_credentials', 1) . '|"' . variable_get('socialloginandsocialshare_email_content', '') . '"';
  $string .= '|' . variable_get('socialloginandsocialshare_update_profile', 0);
  //user profile fields
  $user_field = variable_get('socialloginandsocialshare_user_profile', '');
  $user_field_value = str_replace(array('{', '}'), array('[', ']'), json_encode($user_field));
  $string .= '|' . $user_field_value;
  $string .= '|' . variable_get('socialloginandsocialshare_debug_mode', 0);
  return $string;
}

/**
 * 
 * @param type $string
 * @return type
 * 
 * Submit all social login and sharing settings to LoginRadius and validate API credentials.
 */
function socialloginandsocialshare_settings_validation($string) {
  module_load_include('inc', 'socialloginandsocialshare', 'socialloginandsocialshare.loginradius');
  $obj = new LoginRadius();
  $api_key = trim(variable_get('socialloginandsocialshare_apikey', ''));
  $api_secret = trim(variable_get('socialloginandsocialshare_apisecret', ''));
  $validateurl = 'https://' . LR_DOMAIN . '/api/v2/app/validate?apikey=' . rawurlencode($api_key) . '&apisecret=' . rawurlencode($api_secret);
  $data = array(
    'addon' => 'Drupal',
    'version' => '4.4',
    'agentstring' => $_SERVER["HTTP_USER_AGENT"],
    'clientip' => $_SERVER["REMOTE_ADDR"],
    'configuration' => $string,
  );

  try {
    $json_result = $obj->loginradius_api_client($validateurl, $data);
    $result = json_decode($json_result);

    if (empty($result)) {
      form_set_error('', check_plain('please check your php.ini settings to enable CURL or FSOCKOPEN'));
      return;
    }

    if (isset($result->Status) && !$result->Status) {
      $error = array(
        "API_KEY_NOT_VALID" => "LoginRadius API key is invalid. Get your LoginRadius API key from <a href='http://www.loginradius.com' target='_blank'>LoginRadius</a>",
        "API_SECRET_NOT_VALID" => "LoginRadius API Secret is invalid. Get your LoginRadius API Secret from <a href='http://www.loginradius.com' target='_blank'>LoginRadius</a>",
        "API_KEY_NOT_FORMATED" => "LoginRadius API Key is not formatted correctly",
        "API_SECRET_NOT_FORMATED" => "LoginRadius API Secret is not formatted correctly"
      );

      foreach ($result->Messages as $value) {
        form_set_error('', $error["$value"]);
        return;
      }
    }
  }
  catch (Exception $e) {
    watchdog_exception('type', $e);
  }
}
