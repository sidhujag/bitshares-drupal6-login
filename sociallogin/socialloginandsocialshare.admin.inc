<?php

/**
 * @file
 * Admin page callback file for the socialloginandsocialshare module.
 */
module_load_include('inc', 'socialloginandsocialshare', 'socialloginandsocialshare.admin_functions');
/**
 * Implements Admin setting for the socialloginandsocialshare module.
 */

/**
 * @param $form_state
 * @return mixed
 *
 * Show Admin UI for Social Login tab.
 */
function socialloginandsocialshare_admin_settings(&$form_state) {
  if (!variable_get('socialloginandsocialshare_apikey', FALSE) || !variable_get('socialloginandsocialshare_apisecret', FALSE)) {
    drupal_set_message(t('To activate the Social Login, insert LoginRadius API Key and Secret in the Social Login section below. Social Sharing do not require API Key and Secret.'), 'warning');
  }

  drupal_add_css(drupal_get_path('module', 'socialloginandsocialshare') . '/css/socialloginandsocialshare.admin.css');
  $get_socialloginandsocialshare = '';

  if (!variable_get('socialloginandsocialshare_apikey', FALSE)) {
    $get_socialloginandsocialshare = t('For creating a social login account, please visit <a href="@get_socialloginandsocialshare" target="_blank">LoginRadius</a>', array('@get_socialloginandsocialshare' => 'http://ish.re/4'));
  }

  $thanks_text = t('To activate the module, you will need to first configure it (manage your desired social networks, etc.) from your LoginRadius account. If you do not have an account,
   click <a href="@loginradius" target="_blank">here</a> and create one for FREE!<br/>We also offer social addons for
   <a href="@joomla" target="_blank">Joomla</a>, <a href="@wordpress" target="_blank">Wordpress</a>,
   <a href="@vbulletin" target="_blank">vBulletin</a>, <a href="@vanillaforum" target="_blank">VanillaForum</a>,
   <a href="@magento" target="_blank">Magento</a>, <a href="@oscommerce" target="_blank">OSCommerce</a>,
   <a href="@prestashop" target="_blank">Prestashop</a>, <a href="@xcart" target="_blank">X-Cart</a>,
   <a href="@zencart" target="_blank">Zen-Cart</a>, <a href="@phpbb" target="_blank">phpBB</a>,
   <a href="@smf" target="_blank">SMF</a> and <a href="@dotnetnuke" target="_blank">DotNetNuke</a>.<br/>
   <a href="@loginradius" target="_blank"><br/><input class="form-submit" type="button" value="Set up my FREE account!"></a>
   (<a href="@get_sociallogin" target="_blank">How to set up an account?</a>)', array(
    '@loginradius' => 'http://ish.re/4',
    '@joomla' => 'http://ish.re/ALT8',
    '@wordpress' => 'http://ish.re/ADDT',
    '@vbulletin' => 'http://ish.re/ALTA',
    '@vanillaforum' => 'http://ish.re/ALUN',
    '@magento' => 'http://ish.re/ALT6',
    '@oscommerce' => 'http://ish.re/ALTC',
    '@prestashop' => 'http://ish.re/ALTD',
    '@xcart' => 'http://ish.re/ALUL',
    '@zencart' => 'http://ish.re/ALUM',
    '@phpbb' => 'http://ish.re/A7MH',
    '@smf' => 'http://ish.re/A7MF',
    '@dotnetnuke' => 'http://ish.re/ALUP',
    '@get_sociallogin' => 'http://ish.re/ATM4'
  ));
  $form['thanks_block'] = array(
    '#type' => 'fieldset',
    '#description' => $thanks_text,
  );
  $setup_group = 'setup';
  $form[$setup_group] = array(
    '#type' => 'fieldset',
    '#title' => t('LoginRadius API settings'),
    '#id' => 'setup',
    '#collapsible' => TRUE,
    '#description' => $get_socialloginandsocialshare,
  );
  $form[$setup_group]['socialloginandsocialshare_apikey'] = array(
    '#type' => 'textfield',
    '#title' => t('API Key'),
    '#default_value' => variable_get('socialloginandsocialshare_apikey', ''),
    '#description' => t('To activate the module, insert LoginRadius API Key (<a href="@get_socialloginandsocialshare" target="_blank">How to get it?</a>)', array('@get_socialloginandsocialshare' => 'http://ish.re/9VBI')),
  );
  $form[$setup_group]['socialloginandsocialshare_apisecret'] = array(
    '#type' => 'textfield',
    '#title' => t('API Secret'),
    '#default_value' => variable_get('socialloginandsocialshare_apisecret', ''),
    '#description' => t('To activate the module, insert LoginRadius API Secret (<a href="@get_socialloginandsocialshare" target="_blank">How to get it?</a>)', array('@get_socialloginandsocialshare' => 'http://ish.re/9VBI')),
  );
  $basic_group = 'basic_setting';
  $form[$basic_group] = array(
    '#type' => 'fieldset',
    '#title' => t('Social Login basic settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form[$basic_group]['interface']['socialloginandsocialshare_disable_traditional_login'] = array(
    '#type' => 'radios',
    '#title' => t('Do you want users to log in using social login only?'),
    '#default_value' => variable_get('socialloginandsocialshare_disable_traditional_login', 0),
    '#options' => array(
      1 => t('Yes, I want to disable the  Drupal native login form.(In this case, admin can login with his/her linked social account or via /user [example(clean url): site-url/user or example(not clean url): site-url/?q=user])'),
      0 => t('No, I want my users to have both options at their disposal'),
    ),
  );
  $form[$basic_group]['socialloginandsocialshare_display_username'] = array(
    '#type' => 'radios',
    '#title' => t('How would you like username to be created? Select your desired composition rule for username.'),
    '#default_value' => variable_get('socialloginandsocialshare_display_username', 0),
    '#options' => array(
      0 => t('Firstname Lastname [Ex: John Doe]'),
      1 => t('Firstname-Lastname [Ex: John-Doe]'),
      2 => t('Email Address [Ex: johndoe@abc.com]'),
    ),
  );
  $form[$basic_group]['socialloginandsocialshare_userlogin_redirect'] = array(
    '#type' => 'radios',
    '#title' => t('Redirection settings after login'),
    '#default_value' => variable_get('socialloginandsocialshare_userlogin_redirect', 0),
    '#options' => array(
      0 => t('Redirect to same page of site'),
      1 => t('Redirect to profile page of site'),
      2 => t('Redirect to custom page of site (If you want user to be redirected to specific URL after login)'),
    ),
  );
  $form[$basic_group]['socialloginandsocialshare_userlogin_redirect']['socialloginandsocialshare_custom_redirection'] = array(
    '#type' => 'textfield',
    '#weight' => 50,
    '#default_value' => variable_get('socialloginandsocialshare_custom_redirection', ''),
  );
  $form[$basic_group]['socialloginandsocialshare_userregister_redirect'] = array(
    '#type' => 'radios',
    '#title' => t('Redirection settings after registration'),
    '#default_value' => variable_get('socialloginandsocialshare_userregister_redirect', 0),
    '#options' => array(
      0 => t('Redirect to same page of site'),
      1 => t('Redirect to profile page of site'),
      2 => t('Redirect to custom page of site (If you want user to be redirected to specific URL after registration)'),
    ),
  );
  $form[$basic_group]['socialloginandsocialshare_userregister_redirect']['socialloginandsocialshare_custom_register_redirection'] = array(
    '#type' => 'textfield',
    '#weight' => 50,
    '#default_value' => variable_get('socialloginandsocialshare_custom_register_redirection', ''),
  );
  $linking_group = 'account_linking';
  $form[$linking_group] = array(
    '#type' => 'fieldset',
    '#title' => t('Social account linking settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form[$linking_group]['socialloginandsocialshare_account_linking'] = array(
    '#type' => 'radios',
    '#title' => t('Do you want to enable social account linking at user profile page?'),
    '#default_value' => variable_get('socialloginandsocialshare_account_linking', 1),
    '#options' => array(
      1 => t('Yes'),
      0 => t('No'),
    )
  );
  $form[$linking_group]['linking']['socialloginandsocialshare_identity_label_string'] = array(
    '#type' => 'textfield',
    '#title' => t('What header text do you want to display to users for the social account linking functionality?'),
    '#default_value' => variable_get('socialloginandsocialshare_identity_label_string', 'Link to an existing social account'),
    '#description' => t('This text will be displayed just above social login add more identities interface.'),
  );
  // Submit handler.
  $form['#submit'][] = 'socialloginandsocialshare_admin_settings_submit';
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );
  return $form;
}

/**
 * @param $form_state
 * @return mixed
 *
 * Show admin UI for advanced tab.
 */
function socialloginandsocialshare_advance_settings_form(&$form_state) {
  drupal_add_css(drupal_get_path('module', 'socialloginandsocialshare') . '/css/socialloginandsocialshare.admin.css');
  $interface_group = 'interface_settings';
  $form[$interface_group] = array(
    '#type' => 'fieldset',
    '#title' => t('Social Login interface customization'),
    '#id' => 'interface_settings',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form[$interface_group]['socialloginandsocialshare_label_string'] = array(
    '#type' => 'textfield',
    '#title' => t('What text do you want to display above the social login interface?'),
    '#default_value' => variable_get('socialloginandsocialshare_label_string', 'Login with Social ID'),
    '#description' => t('Leave empty for no text'),
  );
  $form[$interface_group]['socialloginandsocialshare_interface_size'] = array(
    '#type' => 'radios',
    '#title' => t('Select the icon size to use in the social login interface'),
    '#default_value' => variable_get('socialloginandsocialshare_interface_size', 1),
    '#options' => array(
      0 => t('Small'),
      1 => t('Medium'),
    ),
  );
  $form[$interface_group]['socialloginandsocialshare_interface_columns'] = array(
    '#type' => 'textfield',
    '#title' => t('How many social icons would you like to be displayed per row?'),
    '#size' => 7,
    '#default_value' => variable_get('socialloginandsocialshare_interface_columns', ''),
  );
  $form[$interface_group]['socialloginandsocialshare_interface_bgcolor'] = array(
    '#type' => 'textfield',
    '#title' => t('What background color would you like to use for the social login interface?'),
    '#default_value' => variable_get('socialloginandsocialshare_interface_bgcolor', ''),
    '#description' => t('Leave empty for transparent. You can enter hexa-decimal code as well as name of the color.'),
  );
  $display_group = 'interface_display_settings';
  $form[$display_group] = array(
    '#type' => 'fieldset',
    '#title' => t('Social Login interface display settings'),
    '#id' => 'interface_display_settings',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form[$display_group]['interface']['socialloginandsocialshare_attach_login_form'] = array(
    '#type' => 'radios',
    '#title' => t('Do you want to show the social login interface on the user login block?'),
    '#default_value' => variable_get('socialloginandsocialshare_attach_login_form', 1),
    '#options' => array(
      1 => t('Yes'),
      0 => t('No'),
    ),
  );
  $form[$display_group]['interface']['socialloginandsocialshare_loginpage_position'] = array(
    '#type' => 'radios',
    '#title' => t('Do you want to show the social login interface with Drupal`s native login form?'),
    '#default_value' => variable_get('socialloginandsocialshare_loginpage_position', 1),
    '#options' => array(
      1 => t('Yes'),
      0 => t('No'),
    ),
  );
  $form[$display_group]['interface']['socialloginandsocialshare_registerpage_position'] = array(
    '#type' => 'radios',
    '#title' => t('Do you want to show the social login interface with Drupal`s native user registration form?'),
    '#default_value' => variable_get('socialloginandsocialshare_registerpage_position', 1),
    '#options' => array(
      1 => t('Yes'),
      0 => t('No'),
    ),
  );
  $form[$display_group]['interface']['socialloginandsocialshare_showicon_location'] = array(
    '#type' => 'radios',
    '#title' => t('How do you want the social login interface to be displayed on your Drupal traditional registration/login form?'),
    '#default_value' => variable_get('socialloginandsocialshare_showicon_location', 0),
    '#options' => array(
      0 => t('Above the native registration/login form'),
      1 => t('Below the native registration/login form'),
    ),
  );
  $user_settings_group = 'user_settings';
  $form[$user_settings_group] = array(
    '#type' => 'fieldset',
    '#title' => t('Social Login user settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form[$user_settings_group]['socialloginandsocialshare_enable_user_registration'] = array(
    '#type' => 'radios',
    '#title' => t('Do you want to enable social login functionality when the Administrators only option is checked?'),
    '#default_value' => variable_get('socialloginandsocialshare_enable_user_registration', 0),
    '#options' => array(
      1 => t('Yes'),
      0 => t('No'),
    ),
  );
  $form[$user_settings_group]['socialloginandsocialshare_force_registration'] = array(
    '#type' => 'radios',
    '#title' => t('Do you want users to get registered automatically or have them manually complete the registration process?'),
    '#default_value' => variable_get('socialloginandsocialshare_force_registration', 1),
    '#options' => array(
      1 => t('Automatically register a user and create a new user account'),
      0 => t('Let the user submit the user registration form after social login process.(Users will be redirected to registration page with user fields auto filled and users finally submit the form to create an account with your website.)'),
    ),
  );
  $form[$user_settings_group]['socialloginandsocialshare_email_required'] = array(
    '#type' => 'radios',
    '#title' => t('A few network providers do not supply user email address as part of user profile data. Do you want users to provide their email address before completing the registration process?'),
    '#default_value' => variable_get('socialloginandsocialshare_email_required', 1),
    '#options' => array(
      1 => t('Yes, get real email address from the users (Ask users to enter their email address in a pop-up)'),
      0 => t('No, just auto-generate random email address for the users'),
    )
  );
  $form[$user_settings_group]['socialloginandsocialshare_emailrequired_popup_top'] = array(
    '#type' => 'textarea',
    '#title' => t('Please enter the title of the pop-up asking users to enter their email address'),
    '#rows' => 2,
    '#default_value' => variable_get('socialloginandsocialshare_emailrequired_popup_top', 'You are trying to connect with @provider'),
    '#description' => t('You may use @provider, it will be replaced by the Provider name.'),
  );
  $form[$user_settings_group]['socialloginandsocialshare_emailrequired_popup_text'] = array(
    '#type' => 'textarea',
    '#title' => t('Please enter the message to be displayed to the user in the pop-up asking for their email address'),
    '#rows' => 2,
    '#default_value' => variable_get('socialloginandsocialshare_emailrequired_popup_text', 'Unfortunately we could not retrieve your email from your @provider account Please enter your email in the form below in order to continue.'),
    '#description' => t('You may use @provider, it will be replaced by the Provider name.'),
  );
  $form[$user_settings_group]['socialloginandsocialshare_emailrequired_popup_wrong'] = array(
    '#type' => 'textarea',
    '#title' => t('Please enter the message to be shown to the user in case of an already registered email'),
    '#rows' => 2,
    '#default_value' => variable_get('socialloginandsocialshare_emailrequired_popup_wrong', 'This email is already registered. Please enter another email or link this account via account linking on your profile page'),
  );
  $form[$user_settings_group]['socialloginandsocialshare_send_user_credentials'] = array(
    '#type' => 'radios',
    '#title' => t('Do you want to send a welcome email to new users after successful registration?'),
    '#default_value' => variable_get('socialloginandsocialshare_send_user_credentials', 1),
    '#options' => array(
      1 => t('Yes'),
      0 => t('No'),
    ),
    '#description' => t('Note: It will work only with networks which give email address like Facebook, linkedin.'),
  );
  $form[$user_settings_group]['socialloginandsocialshare_email_content'] = array(
    '#type' => 'textarea',
    '#title' => t('Edit the welcome e-mail messages sent to new member accounts'),
    '#default_value' => socialloginandsocialshare_email_content_text(),
    '#rows' => 10,
  );
  $form[$user_settings_group]['socialloginandsocialshare_update_profile'] = array(
    '#type' => 'radios',
    '#title' => t('Do you want to update the user profile data in your database everytime a user logs into your website?'),
    '#default_value' => variable_get('socialloginandsocialshare_update_profile', 0),
    '#options' => array(
      1 => t('Yes'),
      0 => t('No'),
    )
  );
  $field_map_group = 'fieldmap';

  if (module_exists('profile')) {
    $description = t('Configure your user fields from <strong> PROFILES MODULE FIELD MAPPING </strong>section. Each of your fields that are attached to users are listed below. Map social provider data to field by choosing from select list.');
  }
  else {
    $description = t('<strong>Please enable PROFILE module form modules section for enable field mapping.</strong>');
  }

  $form[$field_map_group] = array(
    '#type' => 'fieldset',
    '#title' => t('Social Login field mapping'),
    '#description' => $description,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  if (module_exists('profile')) {
    module_load_include('inc', 'socialloginandsocialshare', 'socialloginandsocialshare.field');
    $form[$field_map_group]['socialloginandsocialshare_user_profile'] = array(
      '#type' => 'fieldset',
      '#tree' => TRUE,
      '#weight' => 5,
    );
    $properties = socialloginandsocialshare_user_properties();
    $property_options = array('' => t('- Do not import -'));

    foreach ($properties as $property => $property_info) {
      $property_options[$property] = '[' . $property . '] ' . $property_info['label'];
    }

    $field_defaults = variable_get('socialloginandsocialshare_user_profile', array());
    $field_select = array(
      '#type' => 'select',
      '#options' => $property_options,
    );
    $profile_fields = socialloginandsocialshare_profile_fields();

    foreach ($profile_fields as $profile_name => $profile_field) {
      $form[$field_map_group]['socialloginandsocialshare_user_profile'][$profile_name] = $field_select;
      $form[$field_map_group]['socialloginandsocialshare_user_profile'][$profile_name]['#title'] = t($profile_field['title']);
      $form[$field_map_group]['socialloginandsocialshare_user_profile'][$profile_name]['#default_value'] = isset($field_defaults[$profile_name]) ? $field_defaults[$profile_name] : '';
    }
  }

  $debug_group = 'debug';
  $form[$debug_group] = array(
    '#type' => 'fieldset',
    '#title' => t('Debug'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form[$debug_group]['socialloginandsocialshare_debug_mode'] = array(
    '#type' => 'radios',
    '#title' => t('Do you want to enable debugging mode?'),
    '#default_value' => variable_get('socialloginandsocialshare_debug_mode', 0),
    '#options' => array(
      1 => t('Yes'),
      0 => t('No'),
    )
  );
  // Submit handler.
  $form['#submit'][] = 'socialloginandsocialshare_advance_settings_form_submit';
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );
  return $form;
}

/**
 * @param $form_state
 * @return mixed
 *
 * Show Admin UI for  Social sharing tab
 */
function socialloginandsocialshare_sharebar_settings_form(&$form_state) {
  global $base_url;
  $external_jquery = 'http://code.jquery.com/jquery-1.8.3.js';
  drupal_add_js('document.write(unescape("%3Cscript src=\'' . $external_jquery . '\' type=\'text/javascript\'%3E%3C/script%3E"));', 'inline');
  $external_jquery_ui = 'http://code.jquery.com/ui/1.10.0/jquery-ui.js';
  drupal_add_js('document.write(unescape("%3Cscript src=\'' . $external_jquery_ui . '\' type=\'text/javascript\'%3E%3C/script%3E"));', 'inline');
  drupal_add_js(drupal_get_path('module', 'socialloginandsocialshare') . '/js/socialloginandsocialshare.js');
  drupal_add_css(drupal_get_path('module', 'socialloginandsocialshare') . '/css/socialloginandsocialshare.admin.css');
  drupal_add_js('jQuery(document).ready(function() {
        var horizontal_image="' . variable_get("socialloginandsocialshare_share_horizontal_images", "") . '";
		var vertical_image="' . variable_get("socialloginandsocialshare_share_vertical_images", "") . '";
		if(horizontal_image == 8 || horizontal_image == 9) {
		  counter_horizontal_show();
		}
		else {
		  sharing_horizontal_show();
		 if(horizontal_image == 2 || horizontal_image == 3) {
        sharing_simplehorizontal_show();
      } 
		}
		if(vertical_image == 6 || vertical_image == 7) {
		  counter_vertical_show();
		 
		}
		else  {
		   sharing_vertical_show();
		   
		}
	var selected_theme = "' . variable_get("socialloginandsocialshare_selected_share_interface", "") . '";
	if(selected_theme == "vertical"){
	  hidden_horizontal_widget();
	}
	else {
	  display_horizontal_widget();
	}
  });', 'inline', 'header');
  $my_path = drupal_get_path('module', 'socialloginandsocialshare');
  $share_group = 'share';
  $form[$share_group] = array(
    '#type' => 'fieldset',
    '#title' => t('Social Sharing Settings'),
  );
  $form[$share_group]['socialloginandsocialshare_share_horizontal'] = array(
    '#type' => 'item',
    '#prefix' => '<div><b>' . t('
    <div class="description">Please select the social sharing widget, horizontal and vertical widgets can be enabled simultaneously.</div>') . '</b></div>',
    '#value' => '<div id="lr_tabs">
    <a href=javascript:void(0) id=socialloginandsocialshare_horizontal onclick = display_horizontal_widget();>' . check_plain(t('Horizontal')) . '</a>' . '<a href=javascript:void(0) onclick = hidden_horizontal_widget(); id=socialloginandsocialshare_veritical>' . check_plain(t('Vertical')) . '</a>',
  );
  $form[$share_group]['socialloginandsocialshare_selected_share_interface'] = array(
    '#type' => 'hidden',
    '#title' => t('selected share interface'),
    '#suffix' => '</div><div id=lrsharing_divwhite></div><div id=lrsharing_divgrey></div><div id="show_horizontal_block">',
    '#default_value' => variable_get('socialloginandsocialshare_selected_share_interface', ''),
  );
  $form[$share_group]['socialloginandsocialshare_enable_horizontal_share'] = array(
    '#type' => 'radios',
    '#attributes' => array('class' => 'socialloginandsocialshare_enable_horizontal_share'),
    '#title' => t('Do you want to enable horizontal social sharing for your website?'),
    '#default_value' => variable_get('socialloginandsocialshare_enable_horizontal_share', 0),
    '#options' => array(
      1 => t('Yes'),
      0 => t('No'),
    )
  );
  $form[$share_group]['socialloginandsocialshare_enable_vertical_share'] = array(
    '#type' => 'radios',
    '#title' => t('Do you want to enable vertical social sharing for your website?'),
    '#attributes' => array('class' => 'socialloginandsocialshare_enable_vertical_share'),
    '#default_value' => variable_get('socialloginandsocialshare_enable_vertical_share', 0),
    //'#prefix' => '<div id=lrsharing_divwhite></div><div id=lrsharing_divgrey></div><div id="show_horizontal_block">',
    '#options' => array(
      1 => t('Yes'),
      0 => t('No'),
    )
  );
  $form[$share_group]['socialloginandsocialshare_share_horizontal_images'] = array(
    '#type' => 'radios',
    '#default_value' => variable_get('socialloginandsocialshare_share_horizontal_images', 0),
    '#attributes' => array('class' => 'socialloginandsocialshare_share_horizontal_images'),
    '#options' => array(
      0 => '<img src="' . $base_url . '/' . $my_path . '/images/horizonSharing32.png"></img>',
      1 => '<img src="' . $base_url . '/' . $my_path . '/images/horizonSharing16.png" style="margin-top: 12px;"></img>',
      2 => '<img src="' . $base_url . '/' . $my_path . '/images/single-image-theme-large.png" style="margin-top: 7px;"></img>',
      3 => '<img src="' . $base_url . '/' . $my_path . '/images/single-image-theme-small.png" style="margin-top: 12px;"></img>',
      8 => '<img src="' . $base_url . '/' . $my_path . '/images/horizontalvertical.png" style="margin-top: -8px;margin-left: -1px;"></img>',
      9 => '<img src="' . $base_url . '/' . $my_path . '/images/horizontal.png" style="margin-top: 2px;margin-left: -4px;"></img>',
    ),
  );
  $form[$share_group]['socialloginandsocialshare_share_vertical_images'] = array(
    '#type' => 'radios',
    '#attributes' => array('class' => 'socialloginandsocialshare_share_vertical_images'),
    '#default_value' => variable_get('socialloginandsocialshare_share_vertical_images', 4),
    '#options' => array(
      4 => '<img src="' . $base_url . '/' . $my_path . '/images/32VerticlewithBox.png"></img>',
      5 => '<img id="VerticlewithBox" src="' . $base_url . '/' . $my_path . '/images/16VerticlewithBox.png" style="margin-left: 8px;"></img>',
      6 => '<img src="' . $base_url . '/' . $my_path . '/images/hybrid-verticle-vertical.png" style="margin-left: -6px;"></img>',
      7 => '<img id="hybrid-verticle-horizontal"  src="' . $base_url . '/' . $my_path . '/images/hybrid-verticle-horizontal.png" style="margin-left: -1px;"></img>',
    ),
  );
  $form[$share_group]['socialloginandsocialshare_show_horizotal'] = array(
    '#type' => 'hidden',
    '#suffix' => '<div id="socialloginandsocialshare_show_horizotal_widget">',
  );

  $counter_providers = variable_get('socialcounter_rearrange_providers_list', '');

  if (empty($counter_providers)) {
    variable_set('socialcounter_rearrange_providers_list', socialloginandsocialshare_get_default_sharing_networks('socialcounter_rearrange_providers_list'));
  }

  $form[$share_group]['socialshare_counter_show_providers_list'] = array(
    '#type' => 'item',
    '#id' => 'socialcounter_show_providers_list',
    '#class' => 'form-item form-type-checkboxes form-item-socialshare-counter-show-providers-list',
    '#title' => t('What sharing networks do you want to show in the sharing widget? (All other sharing networks will be shown as part of LoginRadius sharing icon)'),
    '#default_value' => variable_get('socialcounter_rearrange_providers_list', ''),
    '#suffix' => '<div id="socialcounter_hidden_field" style="display:none;">',
  );

  foreach (variable_get("socialcounter_rearrange_providers_list", '') as $provider) {
    if (!empty($provider)) {
      $raw = $provider;
      $provider = str_replace(array(' ', '++', '+'), array('', 'plusplus', 'plus'), $provider);
      $form[$share_group][$provider] = array(
        '#type' => 'markup',
        '#attributes' => array('class' => array('lrshare_' . $provider)),
        '#value' => '<input type = "hidden" id = "input-lrcounter-' . $provider . '" name = "socialcounter_rearrange_providers_list[]" value = "' . $raw . '" />',
      );
    }
  }

  $share_providers = variable_get('socialloginandsocialshare_rearrange_providers_list', '');

  if (empty($share_providers)) {
    variable_set('socialloginandsocialshare_rearrange_providers_list', socialloginandsocialshare_get_default_sharing_networks('socialloginandsocialshare_rearrange_providers_list'));
  }

  $form[$share_group]['socialshare_show_providers_list'] = array(
    '#type' => 'item',
    '#name' => 'socialshare_show_providers_list',
    '#id' => 'socialshare_providers_list',
    '#title' => t('What sharing networks do you want to show in the sharing widget? (All other sharing networks will be shown as part of LoginRadius sharing icon)'),
    '#default_value' => variable_get('socialloginandsocialshare_rearrange_providers_list', ''),
    '#prefix' => '</div><div id="loginRadiusSharingLimit" style="color:red; display:none; margin-bottom: 5px;">' . t('You can select only 9 providers.') . '</div>',
    '#suffix' => '<div id="rearrange_sharing_text"><b>' . t('What sharing network order do you prefer for your sharing widget? Drag the icons around to set the order') . '</b></div><ul id="socialloginandsocialshare_rearrange_providers" class="socialloginandsocialshare_rearrange_providers">'
  );

  foreach (variable_get('socialloginandsocialshare_rearrange_providers_list', '') as $provider) {
    if (!empty($provider)) {
      $form[$share_group]['lrshare_iconsprite32' . $provider] = array(
        '#type' => 'markup',
        '#attributes' => array('class' => array('lrshare_' . $provider)),
        '#value' => '<li id = "edit-lrshare-iconsprite32' . $provider . '" class = "lrshare_iconsprite32 lrshare_' . $provider . '" title = "' . $provider . '" >
	    <input type = "hidden" id = "input-lrshare-' . $provider . '" name = "socialloginandsocialshare_rearrange_providers_list[]" value = "' . $provider . '" /></li>',
      );
    }
  }

  $form[$share_group]['socialloginandsocialshare_rearran_providers_list'] = array(
    '#type' => 'hidden',
    '#prefix' => '</ul><div id="socialloginandsocialshare_question"></div>',
    '#suffix' => '</div>',
  );
  $form[$share_group]['socialloginandsocialshare_show_veritical'] = array(
    '#type' => 'hidden',
    '#suffix' => '<div id="socialloginandsocialshare_show_veritcal_widget">',
  );
  $vertical_counter_providers = variable_get('socialcounter_vertical_rearrange_providers_list', '');

  if (empty($vertical_counter_providers)) {
    variable_set('socialcounter_vertical_rearrange_providers_list', socialloginandsocialshare_get_default_sharing_networks('socialcounter_vertical_rearrange_providers_list'));
  }

  $form[$share_group]['socialshare_counter_vertical_show_providers_list'] = array(
    '#type' => 'item',
    '#id' => 'socialcounter_vertical_show_providers_list',
    '#title' => t('What sharing networks do you want to show in the sharing widget? (All other sharing networks will be shown as part of LoginRadius sharing icon)'),
    '#default_value' => variable_get('socialcounter_vertical_rearrange_providers_list', ''),
    '#suffix' => '<div id="socialcounter_vertical_hidden_field" style="display:none;">',
  );

  foreach (variable_get("socialcounter_vertical_rearrange_providers_list", "") as $provider) {
    if (!empty($provider)) {
      $raw = $provider;
      $provider = str_replace(array(' ', '++', '+'), array('', 'plusplus', 'plus'), $provider);
      $form[$share_group]['lrshare_vertical_' . $provider] = array(
        '#type' => 'markup',
        '#attributes' => array('class' => array('lrshare_vertical_' . $provider)),
        '#value' => '<input type = "hidden" id = "input-lrcounter-vertical-' . $provider . '" name = "socialcounter_vertical_rearrange_providers_list[]" value = "' . $raw . '" />',
      );
    }
  }

  $vertical_share_providers = variable_get('socialshare_vertical_rearrange_providers_list', '');

  if (empty($vertical_share_providers)) {
    variable_set('socialshare_vertical_rearrange_providers_list', socialloginandsocialshare_get_default_sharing_networks('socialshare_vertical_rearrange_providers_list'));
  }

  $form[$share_group]['socialshare_vetical_show_providers_list'] = array(
    '#type' => 'item',
    '#id' => 'socialshare_vetical_show_providers_list',
    '#title' => t('What sharing networks do you want to show in the sharing widget? (All other sharing networks will be shown as part of LoginRadius sharing icon)'),
    '#default_value' => variable_get('socialshare_vertical_rearrange_providers_list', ''),
    '#prefix' => '</div><div id="loginRadiusSharingLimit_vertical" style="color:red; display:none; margin-bottom: 5px;">' . t('You can select only 9 providers.') . '</div>',
    '#suffix' => '<div id="rearrange_sharing_text_vertical"><b>' . t('What sharing network order do you prefer for your sharing widget?(Drag around to set the order)') . '</b></div><ul id="socialshare_vertical_rearrange_providers" class="socialshare_vertical_rearrange_providers">',
  );

  foreach (variable_get("socialshare_vertical_rearrange_providers_list", "") as $provider) {
    if (!empty($provider)) {
      $form[$share_group]['lrshare_iconsprite32_vertical' . $provider] = array(
        '#type' => 'markup',
        '#attributes' => array('class' => array('lrshare_' . $provider)),
        '#value' => '<li id = "edit-lrshare-iconsprite32_vertical' . $provider . '" class = "lrshare_iconsprite32 lrshare_' . $provider . '" title = "' . $provider . '" >
	    <input type = "hidden" id = "input-lrshare-vertical-' . $provider . '" name = "socialshare_vertical_rearrange_providers_list[]" value = "' . $provider . '" /></li>',
      );
    }
  }

  $form[$share_group]['socialshare_vertical_rearra_providers_list'] = array(
    '#type' => 'hidden',
    '#prefix' => '</ul><div id="socialloginandsocialshare_question"></div>',
    '#suffix' => '</div>',
  );
  $form[$share_group]['socialloginandsocialshare_share_vertical_images']['socialshare_vertical_position'] = array(
    '#type' => 'radios',
    '#title' => t('Select the position of social sharing widget'),
    '#weight' => 50,
    '#attributes' => array(
      'class' => 'socialshare_vertical_position',
    ),
    '#id' => 'socialshare_vertical_position',
    '#default_value' => variable_get('socialshare_vertical_position', 0),
    '#options' => array(
      0 => t('Top Left'),
      1 => t('Top Right'),
      2 => t('Bottom Left'),
      3 => t('Bottom Right'),
    ),
  );
  $form[$share_group]['socialshare_position_top'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show at the top of content.'),
    '#default_value' => variable_get('socialshare_position_top', 1) ? 1 : 0,
    '#prefix' => '<div style="clear:both"> <b>Select the position of the social sharing interface</b>',
    '#suffix' => '</div>',
  );
  $form[$share_group]['socialshare_position_bottom'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show at the bottom of content.'),
    '#default_value' => variable_get('socialshare_position_bottom', 1) ? 1 : 0,
  );
  $form[$share_group]['socialshare_show_pages'] = array(
    '#type' => 'radios',
    '#title' => t('Show social share on specific pages'),
    '#default_value' => variable_get('socialshare_show_pages', 0),
    '#options' => array(
      0 => t('All pages except those listed'),
      1 => t('Only the listed pages'),
    ),
  );
  $form[$share_group]['socialshare_show_exceptpages'] = array(
    '#type' => 'textarea',
    '#default_value' => variable_get('socialshare_show_exceptpages', ''),
    '#description' => t('Enter the page title(s) (in the same format provided during page creation) or node ID (if url is http://example.com/node/1 then enter 1) separated by commas'),
    '#rows' => 5,
  );
  $form[$share_group]['socialshare_vertical_show_pages'] = array(
    '#type' => 'radios',
    '#title' => t('Show social share on specific pages'),
    '#prefix' => '<div style="clear:both"></div>',
    '#default_value' => variable_get('socialshare_vertical_show_pages', 0),
    '#options' => array(
      0 => t('All pages except those listed'),
      1 => t('Only the listed pages'),
    ),
  );
  $form[$share_group]['socialshare_vertical_show_exceptpages'] = array(
    '#type' => 'textarea',
    '#default_value' => variable_get('socialshare_vertical_show_exceptpages', ''),
    '#description' => t('Enter the page title(s) (in the same format provided during page creation) or node ID (if url is http://example.com/node/1 then enter 1) separated by commas'),
    '#rows' => 5,
    '#suffix' => '</div>',
  );
  //submit the sharing settings.
  $form['#submit'][] = 'socialloginandsocialshare_sharebar_settings_submit';
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );
  return $form;
}

/**
 * Show Admin UI for help tab.
 */
function socialloginandsocialshare_user_help() {
  global $base_url;
  $my_path = drupal_get_path('module', 'socialloginandsocialshare');
  drupal_add_css(drupal_get_path('module', 'socialloginandsocialshare') . '/css/socialloginandsocialshare.admin.css');
  $sociallogin_get_updates = t('To receive updates on new features, releases, etc. Please connect to one of our social media pages<br/><br/>
   <div>
   <a  href="https://www.facebook.com/loginradius" target="_blank"><img src="' . $base_url . '/' . $my_path . '/images/footer-media-links/facebook.png" /></a>
   <a href="https://twitter.com/LoginRadius" target="_blank"><img src="' . $base_url . '/' . $my_path . '/images/footer-media-links/twitter.png" /></a>
  <a href="https://plus.google.com/+Loginradius" target="_blank"> <img src="' . $base_url . '/' . $my_path . '/images/footer-media-links/google.png" /></a>
  <a href="http://www.linkedin.com/company/loginradius" target="_blank"> <img src="' . $base_url . '/' . $my_path . '/images/footer-media-links/linkedin.png" /></a>
  <a href="https://www.youtube.com/user/LoginRadius" target="_blank"> <img src="' . $base_url . '/' . $my_path . '/images/footer-media-links/youtube.png" /></a>
   </div>
');
  $support_text = t('If you liked our FREE open-source addon, please send your feedback/testimonial to <a href="@feedback">feedback@loginradius.com</a>!', array('@feedback' => 'mailto:feedback@loginradius.com'));
  $output = '';
  $output .= '<h3>' . t('Help & Documentations') . '</h3>';
  $important_title = t('Important links');
  $items_list = array(
    l(t('Module Installation and Configuration'), 'http://ish.re/AEFU', $options = array('attributes' => array('target' => '_blank'))),
    l(t('How to get LoginRadius API Key & Secret'), 'http://ish.re/9VBI', $options = array('attributes' => array('target' => '_blank'))),
    l(t('Discussion Forum'), 'http://ish.re/8PG2', $options = array('attributes' => array('target' => '_blank'))),
    l(t('About LoginRadius'), 'http://ish.re/96M7', $options = array('attributes' => array('target' => '_blank'))),
    l(t('LoginRadius Products'), 'http://ish.re/96M9', $options = array('attributes' => array('target' => '_blank'))),
    l(t('Social Plugins'), 'http://ish.re/96M8', $options = array('attributes' => array('target' => '_blank'))),
    l(t('Social SDKs'), 'http://ish.re/96MA', $options = array('attributes' => array('target' => '_blank'))),
  );
  $output .= theme('item_list', $items_list, $important_title);
  $title = t('Get Updates');
  $items = array(
    $sociallogin_get_updates,
  );
  $output .= theme('item_list', $items, $title);
  $supporttitle = t('<div style="clear:both;"></div>Support Us');
  $item = array(
    $support_text,
  );
  $output .= theme('item_list', $item, $supporttitle);
  return $output;
}
