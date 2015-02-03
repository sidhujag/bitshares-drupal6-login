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
    <?php
  endif;
  ?>
  <div class="interfacecontainerdiv"></div>
  <?php
endif;
?>