<?php

/**
 * @file
 * Helper file to handle login/registration/linking functionality
 */
module_load_include('inc', 'socialloginandsocialshare', 'socialloginandsocialshare.functions');

/**
 * @param $account
 * @param $lrdata
 *
 * Update picture and insert mapping data into authmap and loginradius mapping table.
 */
function socialloginandsocialshare_update_mapping_data($account, $lrdata) {
  $result = db_query("SELECT authname FROM {authmap} WHERE authname = '%s'", check_plain($lrdata['id']));
  $authname_exist = db_fetch_array($result);

  if (empty($authname_exist) && !$authname_exist) {
    db_query("INSERT INTO {authmap} (uid, authname, module) VALUES ('%d' ,'%s' ,'%s')", $account->uid, check_plain($lrdata['id']), 'socialloginandsocialshare');
  }

  if (variable_get('user_pictures', '') == 1 && !empty($lrdata['thumbnail'])) {
    socialloginandsocialshare_insert_picture($lrdata, $account);
  }

  $aid_result = db_query("SELECT aid FROM {authmap} WHERE authname = '%s'", check_plain($lrdata['id']));
  $aid = db_fetch_object($aid_result);
  $check_aids = db_query("SELECT aid FROM {socialloginandsocialshare_mapusers} WHERE aid = '%s'", $aid->aid);
  $check_aid = db_fetch_object($check_aids);

  if (!empty($check_aid->aid)) {
    db_query("DELETE FROM {socialloginandsocialshare_mapusers} WHERE  aid='%d'", $check_aid->aid);
  }

  db_query("INSERT INTO {socialloginandsocialshare_mapusers} VALUES ('%d' ,'%s' ,'%s')", $aid->aid, $lrdata['provider'], check_plain($lrdata['id']));
}

/**
 * @param $lrdata
 * @param $form_state
 *
 * Save new user and provde login to user.
 */
function socialloginandsocialshare_save_new_user($lrdata, &$form_state) {
  $data = socialloginandsocialshare_check_exist_username($lrdata);
  $form_state['values']['name'] = $data['username'];
  $form_state['values']['mail'] = $lrdata['email'];
  $form_state['values']['pass'] = user_password();

  if (variable_get('user_register', '') == 1 || (variable_get('socialloginandsocialshare_enable_user_registration', '') == 1 && variable_get('user_register', '') == 0)) {
    $form_state['values']['status'] = 1;
  }

  $form_state['values']['init'] = $lrdata['email'];
  $form_state['values']['auth_socialloginandsocialshare'] = $lrdata['id'];

  if (variable_get('user_register', 1) || variable_get('user_register', 2) || (variable_get('socialloginandsocialshare_enable_user_registration', '') == 1 && variable_get('user_register', '') == 0)) {
    // Profile module support.
    if (module_exists('profile')) {
      $lrdata['gender'] = !empty($lrdata['gender']) ? socialloginandsocialshare_get_gender($lrdata['gender']) : '';
      $lrdata['sociallogin_register_form'] = 'no';
      module_load_include('inc', 'socialloginandsocialshare', 'socialloginandsocialshare.field');
      socialloginandsocialshare_profile_create_user($form_state['values'], $lrdata);
    }

    $account = user_save('', $form_state['values']);

    if (variable_get('user_pictures', 1) && !empty($lrdata['thumbnail'])) {
      socialloginandsocialshare_insert_picture($lrdata, $account);
    }

    $aid_result = db_query("SELECT aid FROM {authmap} WHERE authname = '%s'", check_plain($lrdata['id']));
    $aid = db_fetch_object($aid_result);
    $check_aids = db_query("SELECT aid FROM {socialloginandsocialshare_mapusers} WHERE aid = '%s'", $aid->aid);
    $check_aid = db_fetch_object($check_aids);

    if (!empty($check_aid->aid)) {
      db_query("DELETE FROM {socialloginandsocialshare_mapusers} WHERE  aid='%d'", $check_aid->aid);
    }

    db_query("INSERT INTO {socialloginandsocialshare_mapusers} VALUES ('%d' ,'%s' ,'%s')", $aid->aid, $lrdata['provider'], check_plain($lrdata['id']));

    if (!$account) {
      drupal_set_message(t("Error saving user account."), 'error');
      $form_state['redirect'] = '';
      return;
    }

    if ($account->status && !variable_get('user_verify', 1)) {
      db_query("UPDATE {users} SET login = %d WHERE uid = %d", (int) $_SERVER['REQUEST_TIME'], $account->uid);

      //user_external_login_register($username, $module);
      if (variable_get('socialloginandsocialshare_send_user_credentials', '') == 1) {
        drupal_mail('socialloginandsocialshare', 'newuser', $account->mail, user_preferred_language($account), array(
          'account' => $account,
          'pass' => $form_state['values']['pass']
        ));
      }

      if (user_authenticate($form_state['values'])) {
        drupal_set_message(t('Registration successful. You are now logged in.'));
      }

      unset($_SESSION['lrdata']);
      $_SESSION['current_social_provider'] = $lrdata['id'];
      socialloginandsocialshare_user_redirect($account, 'socialloginandsocialshare_userregister_redirect');
    }
    elseif ($account->status || variable_get('user_verify', 1)) {
      // Require email confirmation
      _user_mail_notify('status_activated', $account);
      drupal_set_message(t('Once you have verified your e-mail address, you may log in via Social Login.'));
      variable_set('user_verify', 0);
      drupal_goto();
    }
    else {
      _user_mail_notify('register_pending_approval', $account);
      drupal_set_message(t('Thank you for applying for an account. Your account is currently pending approval by the site administrator.<br />In the meantime, a welcome message with further instructions has been sent to your e-mail address.'));
      drupal_goto();
    }
  }
  else {
    drupal_set_message(t('Only site administrators can create new user accounts.'), 'error');
    drupal_goto();
  }
}

/**
 * @param $lrdata
 * loginradius data that go from social network
 * @param $form_state
 *
 * Function for adding social user.
 */
function socialloginandsocialshare_adduser($lrdata, &$form_state) {
  if (isset($lrdata['id']) && !empty($lrdata['id']) && !empty($lrdata['email'])) {
    $account = user_external_load($lrdata['id']);

    if (!$account) {
      $account = user_load(array('mail' => $lrdata['email']));

      if ($account) {
        socialloginandsocialshare_update_mapping_data($account, $lrdata);
      }
    }

    if (isset($account->uid)) {
      socialloginandsocialshare_provide_login_to_unblock_user($account, $lrdata, $form_state);
    }
    else {
      socialloginandsocialshare_save_new_user($lrdata, $form_state);
    }
  }
}

/**
 * @param $account
 * user account information
 * @param string $token
 * loginradius token
 *
 * Submit sociallogin identities for the specified user.
 */
function socialloginandsocialshare_user_identities_submit($account, $token = '') {
  unset($_SESSION['lr_token']);

  if (!class_exists(LoginRadius)) {
    include 'socialloginandsocialshare.loginradius.inc';
  }

  $obj = new LoginRadius();
  drupal_set_title(check_plain($account->name));

  if (user_is_logged_in() && $obj->loginradius_is_valid_guid($token)) {
    if (!class_exists(LoginRadius)) {
      include 'socialloginandsocialshare.loginradius.inc';
    }
    $userprofile = $obj->loginradius_get_user_profiledata($token);

    if (isset($userprofile->ID) && $userprofile->ID != '' && user_is_logged_in()) {
      $lrdata = socialloginandsocialshare_getuser_data($userprofile);
      $result = db_query("SELECT provider FROM {authmap} am INNER JOIN {socialloginandsocialshare_mapusers} sm ON am.aid = sm.aid WHERE module = 'socialloginandsocialshare'  AND uid = %d", $account->uid);
      $mapped = '';

      while ($identity = db_fetch_object($result)) {
        if ($identity->provider == $lrdata['provider']) {
          $mapped = 'yes';
        }
      }

      $authname_exist_result = db_query("SELECT authname FROM {authmap} am INNER JOIN {socialloginandsocialshare_mapusers} sm ON am.aid = sm.aid WHERE am.authname = '%s'", check_plain($lrdata['id']));
      $authname_exist = db_fetch_array($authname_exist_result);

      if ($mapped != 'yes') {
        if (empty($authname_exist) && !$authname_exist) {
          db_query("DELETE FROM {authmap} WHERE authname='%s'", check_plain($lrdata['id']));
          db_query("INSERT INTO {authmap} (uid, authname, module) VALUES ('%d' ,'%s' ,'%s')", $account->uid, check_plain($lrdata['id']), 'socialloginandsocialshare');

          if (variable_get('socialloginandsocialshare_update_profile', '') == 1) {
            if (variable_get('user_pictures', 1) && !empty($lrdata['thumbnail'])) {
              socialloginandsocialshare_insert_picture($lrdata, $account);
            }
          }

          $aid_result = db_query("SELECT aid FROM {authmap} WHERE authname = '%s'", $lrdata['id']);
          $aid = db_fetch_object($aid_result);
          $check_aids = db_query("SELECT aid FROM {socialloginandsocialshare_mapusers} WHERE aid = '%s'", $aid->aid);
          $check_aid = db_fetch_object($check_aids);

          if (!empty($check_aid->aid)) {
            db_query("DELETE FROM {socialloginandsocialshare_mapusers} WHERE  aid='%d'", $check_aid->aid);
          }

          db_query("INSERT INTO {socialloginandsocialshare_mapusers} VALUES ('%d' ,'%s' ,'%s')", $aid->aid, $lrdata['provider'], check_plain($lrdata['id']));
          drupal_set_message(t("Your account successfully mapped with this account."));
        }
        else {
          drupal_set_message(t("This social network is already linked with an account. Kindly unmap the current social network before linking new social network."), 'error');
        }
      }
      else {
        drupal_set_message(t("This account is already linked with an account. try to choose another account."), 'error');
      }

      $destination = urldecode(substr(drupal_get_destination(), 12));
      drupal_goto($destination);
    }
  }
}

/**
 * @param $lrdata
 * @return mixed
 *
 * Function that get username.
 */
function socialloginandsocialshare_get_username($lrdata) {
  if (!empty($lrdata['fullname'])) {
    $username = $lrdata['fullname'];
  }
  elseif (!empty($lrdata['profilename'])) {
    $username = $lrdata['profilename'];
  }
  elseif (!empty($lrdata['nickname'])) {
    $username = $lrdata['nickname'];
  }
  elseif (!empty($lrdata['email'])) {
    $user_name = explode('@', $lrdata['email']);
    $username = $user_name[0];
  }
  else {
    $username = $lrdata['id'];
  }

  return $username;
}

/**
 * @param $lrdata
 * @return mixed|string
 *
 * Get username according to option that saved at admin settings.
 */
function socialloginandsocialshare_username_option($lrdata) {
  if (!empty($lrdata['fname']) && !empty($lrdata['lname']) && variable_get('socialloginandsocialshare_display_username', '') != 2) {
    if (variable_get('socialloginandsocialshare_display_username', '') == 1) {
      $user_name = $lrdata['fname'] . '-' . $lrdata['lname'];
    }
    else {
      $user_name = $lrdata['fname'] . ' ' . $lrdata['lname'];
    }
  }
  elseif (variable_get('socialloginandsocialshare_display_username', '') == 2 && !empty($lrdata['email'])) {
    $user_name = $lrdata['email'];
  }
  else {
    $user_name = socialloginandsocialshare_get_username($lrdata);
  }

  return $user_name;
}

/**
 * @param $lrdata
 * @return mixed
 *
 * Check username exist in database or not.
 */
function socialloginandsocialshare_check_exist_username($lrdata) {
  $user_name = socialloginandsocialshare_username_option($lrdata);
  // Look for user with username match.
  $index = 0;

  while (TRUE) {
    if (user_load(array('name' => $user_name))) {
      $index++;
      $user_name = $user_name . $index;
    }
    else {
      break;
    }
  }

  $data['username'] = socialloginandsocialshare_remove_unescaped_char($user_name);
  $data['name'] = $lrdata['fname'];

  if (empty($data['name'])) {
    $data['name'] = socialloginandsocialshare_get_username($lrdata);
  }

  return $data;
}

/**
 * @param $userprofile
 *  An array containing all userprofile data keys:
 * @return mixed
 *
 * Function getting social user profile data.
 */
function socialloginandsocialshare_getuser_data($userprofile) {
  $lrdata['fullname'] = (!empty($userprofile->FullName) ? trim($userprofile->FullName) : '');
  $lrdata['profilename'] = (!empty($userprofile->ProfileName) ? trim($userprofile->ProfileName) : '');
  $lrdata['fname'] = (!empty($userprofile->FirstName) ? trim($userprofile->FirstName) : '');
  $lrdata['lname'] = (!empty($userprofile->LastName) ? trim($userprofile->LastName) : '');
  $lrdata['id'] = (!empty($userprofile->ID) ? $userprofile->ID : '');
  $lrdata['provider'] = (!empty($userprofile->Provider) ? $userprofile->Provider : '');
  $lrdata['nickname'] = (!empty($userprofile->NickName) ? trim($userprofile->NickName) : '');
  $lrdata['email'] = (sizeof($userprofile->Email) > 0 ? $userprofile->Email[0]->Value : '');
  $lrdata['thumbnail'] = (!empty($userprofile->ImageUrl) ? trim($userprofile->ImageUrl) : '');

  if (empty($lrdata['thumbnail']) && $lrdata['provider'] == 'facebook') {
    $lrdata['thumbnail'] = socialloginandsocialshare_get_facebook_social_picture($lrdata['id']);
  }

  $lrdata['dob'] = (!empty($userprofile->BirthDate) ? $userprofile->BirthDate : '');
  $lrdata['gender'] = (!empty($userprofile->Gender) ? $userprofile->Gender : '');
  $lrdata['company'] = (!empty($userprofile->Positions[1]->Company->Name) ? $userprofile->Positions[1]->Company->Name : '');

  if (empty($lrdata['company'])) {
    $lrdata['company'] = (!empty($userprofile->Industry) ? $userprofile->Industry : '');
  }

  $lrdata['address'] = (!empty($userprofile->Addresses) ? $userprofile->MainAddress : '');

  if (empty($lrdata['address'])) {
    $lrdata['address'] = (!empty($userprofile->HomeTown) ? $userprofile->HomeTown : '');
  }

  $lrdata['aboutme'] = (!empty($userprofile->About) ? $userprofile->About : '');
  $lrdata['website'] = (!empty($userprofile->ProfileUrl) ? $userprofile->ProfileUrl : '');
  $lrdata['state'] = (!empty($userprofile->State) ? $userprofile->State : '');
  $lrdata['city'] = (!empty($userprofile->City) ? $userprofile->City : '');

  if (empty($lrdata['city']) || $lrdata['city'] == 'unknown') {
    $lrdata['city'] = (!empty($userprofile->LocalCity) && $userprofile->LocalCity != 'unknown' ? $userprofile->LocalCity : '');
  }

  $lrdata['country'] = (!empty($userprofile->Country) ? $userprofile->Country : '');

  if (empty($lrdata['country'])) {
    $lrdata['country'] = (!empty($userprofile->LocalCountry) ? $userprofile->LocalCountry : '');
  }

  $lrdata['positions'] = "";

  if (!empty($userprofile->Positions)) {
    for ($i = 0; $i < sizeof($userprofile->Positions); $i++) {
      $lrdata['positions'] .= $userprofile->Positions[$i]->Position . ",";
    }
  }

  $lrdata['educations'] = "";

  if (!empty($userprofile->Educations)) {
    for ($i = 0; $i < sizeof($userprofile->Educations); $i++) {
      $lrdata['educations'] .= ($i + 1) . ". " . $userprofile->Educations[$i]->School . " ";
    }
  }

  $lrdata['languages'] = "";

  if (!empty($userprofile->Languages)) {
    for ($i = 0; $i < sizeof($userprofile->Languages); $i++) {
      $lrdata['languages'] .= $userprofile->Languages[$i]->Name . ",";
    }
  }

  $lrdata['favoritethings'] = "";

  if (!empty($userprofile->FavoriteThings)) {
    for ($i = 0; $i < sizeof($userprofile->FavoriteThings); $i++) {
      $lrdata['favoritethings'] .= $userprofile->FavoriteThings[$i]->Name . ",";
    }
  }

  return $lrdata;
}

/**
 * @param $account
 * @param int $providerid
 *  Social network id
 *
 * Delete social login indentities from user account.
 */
function socialloginandsocialshare_user_delete($account, $providerid = 0) {
  db_query("DELETE FROM {authmap} WHERE uid=%d AND authname='%s' AND module='socialloginandsocialshare'", $account->uid, $providerid);
  db_query("DELETE FROM {socialloginandsocialshare_mapusers} WHERE  provider_id='%s'", $providerid);

  if (db_affected_rows()) {
    drupal_set_message(t('Social Login ID deleted.'));
  }

  drupal_goto('user/' . $account->uid . '/edit');
}

/**
 * @param $account
 * @param string $variable_path
 *  Get variable is of login/registration
 *
 * Redirect user after login/registration
 */
function socialloginandsocialshare_user_redirect($account, $variable_path = '') {
  $variable_path = (!empty($variable_path) ? $variable_path : 'socialloginandsocialshare_userlogin_redirect');
  $variable_custom_path = (($variable_path == 'socialloginandsocialshare_userlogin_redirect') ? 'socialloginandsocialshare_custom_redirection' : 'socialloginandsocialshare_custom_register_redirection');
  $custom_url = variable_get($variable_custom_path, '');
  $request_uri = urldecode(request_uri());

  if (strpos($request_uri, 'user/register') && $variable_path != 'socialloginandsocialshare_userregister_redirect') {
    $variable_path = 'socialloginandsocialshare_userregister_redirect';
    socialloginandsocialshare_user_redirect($account, $variable_path);
  }
  else {
    if (variable_get($variable_path, '') == 1) {
      // Redirect to profile.
      unset($_REQUEST['destination']);
      drupal_goto('user/' . $account->uid . '/edit');
    }
    elseif (variable_get($variable_path, '') == 2) {
      // Redirect to custom page.
      $custom_url = variable_get($variable_custom_path, '');

      if (!empty($custom_url)) {
        unset($_REQUEST['destination']);
        drupal_goto($custom_url);
      }
      else {
        drupal_redirect_form($form, $redirect = NULL);
      }
    }
    else {
      //redirect to same page
      if (strpos($request_uri, 'register')) {
        unset($_REQUEST['destination']);
        drupal_goto('user');
      }
      elseif (strpos($request_uri, 'user/login')) {
        unset($_REQUEST['destination']);
        drupal_goto('user/' . $account->uid);
      }
      else {
        drupal_redirect_form($form, $redirect = NULL);
      }
    }
  }
}

/**
 * @param $form
 * @param $lrdata
 *  loginradius data that got from social network.
 *
 * Fill registration form with social data.
 */
function socialloginandsocialshare_fill_register_form_with_socialdata(&$form, $lrdata) {
  if (isset($lrdata['id']) && !empty($lrdata['id'])) {
    $data = socialloginandsocialshare_check_exist_username($lrdata);

    if (isset($form['account'])) {
      $use_form = & $form['account'];
    }
    else {
      $use_form = & $form;
    }

    $use_form['name']['#default_value'] = $data['username'];
    $use_form['mail']['#default_value'] = $lrdata['email'];

    if (variable_get('user_email_verification', TRUE)) {
      $use_form['pass']['#type'] = 'hidden';
      $use_form['pass']['#value'] = user_password();
    }

    // Profile module support and add field mapping data.
    if (module_exists('profile')) {
      $lrdata['gender'] = !empty($lrdata['gender']) ? socialloginandsocialshare_get_gender($lrdata['gender']) : '';
      $lrdata['sociallogin_register_form'] = 'yes';
      module_load_include('inc', 'socialloginandsocialshare', 'socialloginandsocialshare.field');
      socialloginandsocialshare_profile_create_user($form, $lrdata);
    }
  }
}

/**
 * @param $lrdata
 * @param $form_state
 *
 * Check user exist in database and handle login fucntionalty.
 */
function socialloginandsocialshare_check_existing_user($lrdata, &$form_state) {
  if ((empty($lrdata['email']) && variable_get('socialloginandsocialshare_email_required', '') == 0) || !empty($lrdata['email'])) {
    if (empty($lrdata['email']) && variable_get('socialloginandsocialshare_email_required', '') == 0) {
      $lrdata['email'] = socialloginandsocialshare_get_email_by_provider_id($lrdata['email'], $lrdata['id'], $lrdata['provider']);
    }

    $account = user_external_load($lrdata['id']);

    if (!$account) {
      $account = user_load(array('mail' => $lrdata['email']));

      if (!$account) {
        // Fill the user profile data from Login radius to Registeration form.
        if (variable_get('socialloginandsocialshare_force_registration', '') == 0) {
          $_SESSION['social_lrdata'] = $lrdata;
          unset($_REQUEST['destination']);
          drupal_goto('user/register');
        }
      }
    }
    //Handle Login process.
    socialloginandsocialshare_adduser($lrdata, $form_state);
  }
}

/**
 * @param $account
 * @param $lrdata
 * @param $form_state
 *
 * Provide login to user who is not blocked.
 */
function socialloginandsocialshare_provide_login_to_unblock_user($account, $lrdata, &$form_state) {
  if ($account->login) {
    global $user;

    if (variable_get('socialloginandsocialshare_update_profile', '') == 1) {
      $lrdata['sociallogin_register_form'] = 'no';
      module_load_include('inc', 'socialloginandsocialshare', 'socialloginandsocialshare.field');
      $data = array();
      $lrdata['gender'] = !empty($lrdata['gender']) ? socialloginandsocialshare_get_gender($lrdata['gender']) : '';
      socialloginandsocialshare_profile_create_user($data, $lrdata);
      $account = user_save($account, $data);

      if (variable_get('user_pictures', '') == 1 && !empty($lrdata['thumbnail'])) {
        socialloginandsocialshare_insert_picture($lrdata, $account);
      }
    }

    //user_external_login($account->name, $module);
    $user = $account;
    user_authenticate_finalize($form_state['values']);
    $_SESSION['current_social_provider'] = $lrdata['id'];
    socialloginandsocialshare_user_redirect($account);
  }
  else {
    drupal_set_message(t("You are either blocked, or have not activated your account. Please check your email."), 'error');
    drupal_goto();
  }
}
