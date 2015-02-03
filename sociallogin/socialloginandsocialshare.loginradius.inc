<?php

/**
 * @file
 * Sdk page callback file for the advance_social_login_and_share module.
 * This class should be used only for Drupal site.
 */
// Define LoginRadius domain
define('LR_DOMAIN', 'api.loginradius.com');
/**
 * Class for Social Authentication.
 *
 * This is the main class to communicate with LogiRadius Unified Social API. It contains functions for Social Authentication with User Profile Data (Basic and Extended)
 *
 * Copyright 2013 LoginRadius Inc. - www.LoginRadius.com
 *
 * This file is part of the LoginRadius SDK package.
 *
 */
class LoginRadius {

  /**
   * LoginRadius function - It validates against GUID format of keys.
   *
   * @param string $value data to validate.
   *
   * @return boolean If valid - true, else - false
   */
  public function loginradius_is_valid_guid($value) {
    return preg_match('/^\{?[A-Z0-9]{8}-[A-Z0-9]{4}-[A-Z0-9]{4}-[A-Z0-9]{4}-[A-Z0-9]{12}\}?$/i', $value);
  }

  /**
   * LoginRadius function - Check, if it is a valid callback i.e. LoginRadius authentication token is set
   *
   * @return boolean True, if a valid callback.
   */
  public function loginradius_is_callback() {
    if (isset($_POST['token'])) {
      return TRUE;
    }
    else {
      return FALSE;
    }
  }

  /**
   * LoginRadius function - Fetch LoginRadius access token after authentication. It will be valid for the specific duration of time specified in the response.
   *
   * @param string LoginRadius API Secret
   *
   * @return string LoginRadius access token.
   */
  public function loginradius_fetch_access_token($secret) {
    if (!$this->loginradius_is_valid_guid($secret)) {
      return FALSE;
    }
    $validate_url = "https://" . LR_DOMAIN . "/api/v2/access_token?token=" . $_REQUEST['token'] . "&secret=" . $secret;
    $response = json_decode($this->loginradius_api_client($validate_url));
    if (isset($response->access_token) && $response->access_token != '') {
      return $response->access_token;
    }
    else {
      if (isset($response->description) && !empty($response->description)) {
        return $response->description;
      }
      return FALSE;
    }
  }

  /**
   * LoginRadius function - To fetch social profile data from the user's social account after authentication. The social profile will be retrieved via oAuth and OpenID protocols. The data is normalized into LoginRadius' standard data format.
   *
   * @param string $access_token LoginRadius access token
   * @param boolean $raw If true, raw data is fetched
   *
   * @return object User profile data.
   */
  public function loginradius_get_user_profiledata($access_token, $raw = FALSE) {
    $validate_url = "https://" . LR_DOMAIN . "/api/v2/userprofile?access_token=" . $access_token;
    if ($raw) {
      $validate_url = "https://" . LR_DOMAIN . "/api/v2/userprofile/raw?access_token=" . $access_token;
      return $this->loginradius_api_client($validate_url);
    }
    return json_decode($this->loginradius_api_client($validate_url));
  }

  /**
   * LoginRadius function - To fetch data from the LoginRadius API URL.
   *
   * @param string $validate_url Target URL to fetch data from.
   *
   * @return string Data fetched from the LoginRadius API.
   */
  public function loginradius_api_client($validate_url, $post = '') {
    global $base_url;
    $jsonresponse = '';
    if (function_exists('drupal_http_request') && !in_array('drupal_http_request', $this->loginradius_get_disabled_functions())) {
      $headers = array(
        'Referer' => $base_url,
        'User-Agent' => 'SocialLoginandSocialShare_Module',
      );
      $options = array(
        'headers' => $headers,
        'max_redirects' => 5,
      );
      if ($post) {
        $options = array(
          'http' =>
            array(
              'method' => 'POST',
              'timeout' => 10,
              'header' => 'Content-type: application/x-www-form-urlencoded',
              'content' => http_build_query($post)
            )
        );
      }
      $result = drupal_http_request($validate_url, $options);
      $jsonresponse = isset($result->data) ? $result->data : '';
    }
    if (empty($jsonresponse) && in_array('curl', get_loaded_extensions())) {
      $curl_handle = curl_init();
      curl_setopt($curl_handle, CURLOPT_URL, $validate_url);
      curl_setopt($curl_handle, CURLOPT_CONNECTTIMEOUT, 15);
      curl_setopt($curl_handle, CURLOPT_TIMEOUT, 15);
      curl_setopt($curl_handle, CURLOPT_ENCODING, 'json');
      curl_setopt($curl_handle, CURLOPT_SSL_VERIFYPEER, FALSE);
      if ($post) {
        curl_setopt($curl_handle, CURLOPT_POST, 1);
        curl_setopt($curl_handle, CURLOPT_POSTFIELDS, http_build_query($post));
      }
      if (ini_get('open_basedir') == '' && (ini_get('safe_mode') == 'Off' or !ini_get('safe_mode'))) {
        curl_setopt($curl_handle, CURLOPT_FOLLOWLOCATION, 1);
        curl_setopt($curl_handle, CURLOPT_RETURNTRANSFER, TRUE);
        $jsonresponse = curl_exec($curl_handle);
      }
      else {
        curl_setopt($curl_handle, CURLOPT_HEADER, 1);
        $url = curl_getinfo($curl_handle, CURLINFO_EFFECTIVE_URL);
        curl_close($curl_handle);
        $ch = curl_init();
        $url = str_replace('?', '/?', $url);
        curl_setopt($ch, CURLOPT_URL, $url);
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
        $jsonresponse = curl_exec($ch);
        curl_close($ch);
      }
    }
    return $jsonresponse;
  }

  /**
   * Return a list of disabled functions.
   */
  function loginradius_get_disabled_functions() {
    $disabled_functions = trim(ini_get('disable_functions'));
    if (drupal_strlen($disabled_functions) == 0) {
      $disabled_functions = array();
    }
    else {
      $disabled_functions = explode(',', $disabled_functions);
      $disabled_functions = array_map('trim', $disabled_functions);
    }
    return $disabled_functions;
  }
}

?>