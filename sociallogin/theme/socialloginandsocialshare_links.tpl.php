<?php
/**
 * Theme social links.
 * */
if (!user_access('administer site configuration')) :
  $api_key = trim(variable_get('socialloginandsocialshare_apikey', ''));
  if (!empty($api_key)):
    ?>
    <script type="text/javascript">
      var options = {};
      options.login = true;
      LoginRadius_SocialLogin.util.ready(function() {
        $ui = LoginRadius_SocialLogin.lr_login_settings;
        $ui.interfacesize = "<?php print $interfaceiconsize ?>";
        $ui.lrinterfacebackground = "<?php print$interfacebackgroundcolor ?>";
        $ui.noofcolumns = <?php print $interfacerow ?>;
        $ui.apikey = "<?php print $api_key ?>";
        $ui.is_access_token = true;
        $ui.callback = "<?php print url('', array('absolute' => TRUE)); ?>";
        $ui.lrinterfacecontainer = "interfacecontainerdiv";
        LoginRadius_SocialLogin.init(options);
      });
      LoginRadiusSDK.setLoginCallback(function() {
        var token = LoginRadiusSDK.getToken();
        var form = document.createElement('form');
        form.action = "<?php print $loc ?>";
        form.method = 'POST';

        var hiddenToken = document.createElement('input');
        hiddenToken.type = 'hidden';
        hiddenToken.value = token;
        hiddenToken.name = 'token';
        form.appendChild(hiddenToken);

        document.body.appendChild(form);
        form.submit();
      });
    </script>
<?php endif;
if (variable_get('socialloginandsocialshare_bitshareslogin_enabled', 1) == 1) :
drupal_add_css(drupal_get_path('module', 'socialloginandsocialshare') . '/css/crypto.css');
$my_settings = array(
'location' => $loc,
'bitshareslogin_handler' => url('socialloginandsocialshare/bitshareslogin_handler', array('absolute' => TRUE))
);
?>
<script type="text/javascript" src="<?php print $GLOBALS['base_url'] ?>/<?php echo drupal_get_path('module', 'socialloginandsocialshare') ?>/js/sociallogin_bitsharesloginbutton.js">
</script>
<div class="cell text-center">
  <a href="javascript:void(0)" onclick="javascript:getBitsharesLoginURL('<?php echo $my_settings['location']; ?>', '<?php echo $my_settings['bitshareslogin_handler']; ?>')" class="btn btn-block btn-lg btn-social btn-bitshares">
    <img alt="BTS" height="42" src="<?php echo $GLOBALS['base_url']; ?>/<?php echo drupal_get_path('module', 'socialloginandsocialshare'); ?>/images/logo-bitshares.svg" width="42">&nbsp;BitShares Login
  </a>
</div>
<?php endif;
?>
<div class="interfacecontainerdiv"></div>
  <?php
endif;
?>