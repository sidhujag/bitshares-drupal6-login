<?php

/**
 * @file
 * Helper file that contain functions that required by social login module file
 */

/**
 * Get callback url where page landed after login/registration.
 */
function get_callback_url() {
  $destination = urldecode(substr(drupal_get_destination(), 12));
  $query[] = array('destination' => $destination);
  return url('socialloginandsocialshare/token_handler', array('query' => $query, 'absolute' => TRUE));
}

/**
 * @param $gender
 * @return string
 *
 * Get gender.
 */
function socialloginandsocialshare_get_gender($gender) {
  if ($gender == 'M') {
    return 'Male';
  }
  elseif ($gender == 'F') {
    return 'Female';
  }
}

/**
 * @return string
 *
 * Get email body content.
 */
function socialloginandsocialshare_email_content_text() {
  $output = variable_get('socialloginandsocialshare_email_content', FALSE);

  if (!$output && $output !== '') {
    $output = '';
    $output .= '!username,

Your account at !site has been created.

Please use following credentials to log into your account:

Website : !site
User name : !username
Password :  !pass

--  !site';
  }

  return $output;
}

/**
 * @param $key
 * @param null $language
 * @param array $variables
 * @return mixed
 *
 * GEt email required content
 */
function socialloginandsocialshare_mail_text($key, $language = NULL, $variables = array()) {
  $langcode = isset($language) ? $language->language : NULL;

  // No override, return default string.
  switch ($key) {
    case 'newuser_subject':
      $text = t('Thank you for registering at !site', $variables, $langcode);
      break;
    case 'newuser_body':
      $email_content = socialloginandsocialshare_email_content_text();
      // $email_content = t($email_content, array('@password' => t($variables['pass'])));
      $text = t($email_content, $variables, $langcode);
      break;
  }

  if (module_exists('token')) {
    $text = token_replace_multiple($text, array('global' => NULL));
  }

  return $text;
}

/**
 * @param $form_state
 * @return mixed
 *
 * Contain form element that will show in email popup.
 */
function socialloginandsocialshare_popup_form($form_state) {
  $form['email'] = array(
    '#type' => 'textfield',
    '#name' => 'email',
    '#size' => 40,
    '#attributes' => array(
      'id' => 'email',
      'class' => 'inputtxt',
    ),
  );
  $form['socialloginandsocialshare_emailclick'] = array(
    '#type' => 'submit',
    '#value' => t('Send'),
    '#name' => 'socialloginandsocialshare_emailclick',
    '#attributes' => array(
      'id' => 'socialloginandsocialshare_emailclick',
      'class' => 'inputbutton',
    ),
  );
  $form['socialloginandsocialshare_emailclick_cancel'] = array(
    '#type' => 'button',
    '#value' => t('Cancel'),
    '#name' => 'socialloginandsocialshare_emailclick_cancel',
    '#attributes' => array(
      'id' => 'socialloginandsocialshare_emailclick_cancel',
      'class' => 'inputbutton',
    ),
  );
  return $form;
}

/**
 * @param $vars
 *
 * Set all variables that required to show social login/linking interface.
 */
function socialloginandsocialshare_options(&$vars) {
  $vars['api_key'] = trim(variable_get('socialloginandsocialshare_apikey', ''));
  $vars['loc'] = get_callback_url();
  $vars['interfaceiconsize'] = variable_get('socialloginandsocialshare_interface_size', '') == 0 ? "small" : "";
  $interfacebackgroundcolor = variable_get('socialloginandsocialshare_interface_bgcolor', '');
  $vars['interfacebackgroundcolor'] = !empty($interfacebackgroundcolor) ? trim($interfacebackgroundcolor) : "";
  $interfacerow = variable_get('socialloginandsocialshare_interface_columns', '');
  $vars['interfacerow'] = (!empty($interfacerow) && is_numeric($interfacerow)) ? trim($interfacerow) : 0;
}

/**
 * @param $lrdata
 * @param $account
 *
 * Function that insert social user picture.
 */
function socialloginandsocialshare_insert_picture($lrdata, $account) {
  if ($account->uid) {
    $path = file_create_path('pictures');
    file_check_directory($path, FILE_CREATE_DIRECTORY);
    $picture_result = drupal_http_request($lrdata['thumbnail']);
    $picture_path = $path . '/picture-' . $account->uid . '.jpg';
    $file = fopen($picture_path, 'w');
    fwrite($file, $picture_result->data);
    fclose($file);
    $picture_info = image_get_info($picture_path);
    list($max_dimensions['width'], $max_dimensions['height']) = explode('x', variable_get('user_picture_dimensions', '85x85'));

    if (image_get_toolkit() && $picture_info['width'] > $max_dimensions['width'] || $picture_info['height'] > $max_dimensions['height']) {
      image_scale($picture_path, $picture_path, $max_dimensions['width'], $max_dimensions['height']);
    }

    db_query("UPDATE {users} SET picture = '%s' WHERE uid = %d", $picture_path, $account->uid);
  }
}

/**
 * @param $str
 * @return mixed|string
 *
 * Function that remove unescaped char from string.
 */
function socialloginandsocialshare_remove_unescaped_char($str) {
  $in_str = str_replace(array('<', '>', '&', '{', '}', '*', '/', '(', '[', ']', '!', ')', '&', '*', '#', '$', '%', '^', '|', '?', '+', '=', '"', ','), array(''), $str);
  $cur_encoding = mb_detect_encoding($in_str);

  if ($cur_encoding == "UTF-8" && mb_check_encoding($in_str, "UTF-8")) {
    return $in_str;
  }
  else {
    return utf8_encode($in_str);
  }
}

function socialloginandsocialshare_check_jsexist($val) {
  $current_js = drupal_add_js(NULL, NULL, 'header');
  $current_js_settings = $current_js['setting'];
  $my_setting_isset = FALSE;

  if (is_array($current_js_settings)) {
    foreach ($current_js_settings as $current_js_setting) {
      if (array_key_exists($val, $current_js_setting)) {
        // We found it so set the flag to true.
        $my_setting_isset = TRUE;
      }
    }
  }

  return $my_setting_isset;
}

/**
 * @param $email
 * @param $provider_id
 * @param $provider
 * @return string
 *
 * Get email address by provider id when email is empty.
 */
function socialloginandsocialshare_get_email_by_provider_id($email, $provider_id, $provider) {
  switch ($provider) {
    case 'twitter':
      $email = $provider_id . '@' . $provider . '.com';
      break;
    default:
      $email_id = drupal_substr($provider_id, 7);
      $email_id2 = str_replace("/", "_", $email_id);
      $email = str_replace(".", "_", $email_id2) . '@' . $provider . '.com';
      break;
  }

  return $email;
}

/**
 * @param $lrdata
 *
 * Forcefully submit data through registration form
 */
function socialloginandsocialshare_enable_force_registration($lrdata) {
  if (variable_get('socialloginandsocialshare_force_registration', '') == 0) {
    $_SESSION['social_lrdata'] = $lrdata;
    // We'll want to redirect back to the same place.
    unset($_REQUEST['destination']);
    drupal_goto('user/register');
  }
}

/**
 * @param $userprofile
 *
 * Show error message when user profile is not retrieved or error message is showing.
 */
function socialloginandsocialshare_show_error_message($userprofile) {
  if (variable_get('socialloginandsocialshare_debug_mode', '') == 1) {
    if (isset($userprofile->description) && !empty($userprofile->description)) {
      drupal_set_message(t($userprofile->description), 'error');
    }
    else {
      drupal_set_message(t('UserProfile is not retrieved'), 'error');
    }
  }
  else {
    drupal_set_message(t('An error occurred during the processing of your request. Please try again in a few minutes or contact the site admin.'), 'error');
  }

  drupal_goto();
}

/**
 * @param $id social network id
 * @return string
 *
 * Get facebook social picture.
 */
function socialloginandsocialshare_get_facebook_social_picture($id) {
  return "http://graph.facebook.com/" . $id . "/picture?type=large";
}
