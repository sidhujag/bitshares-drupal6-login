<?php

/**
 * @file
 * socialloginandsocialshare Field mapping.
 */

/**
 * @return array
 *
 * Get profile fields.
 */
function socialloginandsocialshare_profile_fields() {
  if (module_exists('profile')) {
    static $profile_fields;

    if (!isset($profile_fields)) {
      $profile_fields = array();
      $profile_categories = profile_categories();

      foreach ($profile_categories as $profile_category) {
        $result = _profile_get_fields($profile_category['name']);

        while ($field = db_fetch_array($result)) {
          $profile_fields[$field['name']] = $field;
        }
      }
    }

    return $profile_fields;
  }
}

/**
 * Add data to the profile module.
 */
function socialloginandsocialshare_profile_create_user(&$form_state, $lrdata) {
  $profile_map = variable_get('socialloginandsocialshare_user_profile', array());
  $profile_fields = socialloginandsocialshare_profile_fields();

  foreach ($profile_map as $profile_field_name => $socialloginandsocialshare_property_name) {
    if (isset($profile_fields[$profile_field_name])) {
      $profile_field = $profile_fields[$profile_field_name];

      switch ($profile_field['type']) {
        case 'date':
          $date = array();
          $sldate = explode('/', $lrdata[$socialloginandsocialshare_property_name]);

          if (count($sldate) == 3) {
            foreach (array('month', 'day', 'year') as $count => $part) {
              $date[$part] = (int) ltrim($sldate[$count], '0');
            }

            if ($lrdata['sociallogin_register_form'] == 'yes') {
              $form_state[$profile_field['category']][$profile_field_name]['#default_value'] = $date;
            }
            else {
              $form_state[$profile_field_name] = $date;
            }
          }
          break;
        case 'selection':
          // We can't import anything other than strings into selects.
          if (!is_string($lrdata[$socialloginandsocialshare_property_name])) {
            break;
          }

          $options = explode("\n", $profile_field['options']);
          $best_match = 0.0;
          $best_option = NULL;
          $sl_option = is_string($lrdata[$socialloginandsocialshare_property_name]) ? $lrdata[$socialloginandsocialshare_property_name] : '';
          $match_sl = strtolower($lrdata[$socialloginandsocialshare_property_name]);

          foreach ($options as $option) {
            $option = trim($option);
            $match_option = strtolower($option);
            $this_match = 0;
            similar_text($match_option, $match_sl, $this_match);

            if ($this_match > $best_match) {
              $best_match = $this_match;
              $best_option = $option;
            }
          }

          if (isset($best_option)) {
            if ($lrdata['sociallogin_register_form'] == 'yes') {
              $form_state[$profile_field['category']][$profile_field_name]['#default_value'] = $best_option;
            }
            else {
              $form_state[$profile_field_name] = $best_option;
            }
          }
          break;
        case 'textfield':
        case 'textarea':
        case 'list':
        case 'url':
        default:
          if (!is_string($lrdata[$socialloginandsocialshare_property_name])) {
            break;
          }

          if ($lrdata['sociallogin_register_form'] == 'yes') {
            $form_state[$profile_field['category']][$profile_field_name]['#default_value'] = $lrdata[$socialloginandsocialshare_property_name];
          }
          else {
            $form_state[$profile_field_name] = $lrdata[$socialloginandsocialshare_property_name];
          }

          break;
      }
    }
  }
}

/**
 * Return a all list of social user properties.
 */
function socialloginandsocialshare_user_properties() {
  $common = array(
    'id' => array(
      'label' => t('Provider ID'),
    ),
    'provider' => array(
      'label' => t('Social Provider'),
    ),
    'fullname' => array(
      'label' => t('Full name'),
    ),
    'fname' => array(
      'label' => t('First name'),
    ),
    'lname' => array(
      'label' => t('Last name'),
    ),
    'email' => array(
      'label' => t('E-mail'),
    ),
    'gender' => array(
      'label' => t('Gender'),
    ),
    'dob' => array(
      'label' => t('Birthday'),
    ),
    'aboutme' => array(
      'label' => t('About me (a short bio)'),
    ),
    'address' => array(
      'label' => t('Location'),
    ),
    'company' => array(
      'label' => t('Work history'),
    ),
    'website' => array(
      'label' => t('Profile url'),
    ),
    'nickname' => array(
      'label' => t('Nick name'),
    ),
    'state' => array(
      'label' => t('State'),
    ),
    'city' => array(
      'label' => t('City'),
    ),
    'country' => array(
      'label' => t('Country'),
    ),
    'id' => array(
      'label' => t('Social ID'),
    ),
    'positions' => array(
      'label' => t('Positions'),
    ),
    'educations' => array(
      'label' => t('Educations'),
    ),
    'favoritethings' => array(
      'label' => t('Favorite Things'),
    ),
  );
  drupal_alter('socialloginandsocialshare_user_properties', $common);
  ksort($common);
  return $common;
}
