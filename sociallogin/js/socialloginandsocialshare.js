var selected_sharing_theme = $('[name="socialloginandsocialshare_selected_share_interface"]');
document.write("<script type='text/javascript'>var islrsharing = true; var islrsocialcounter = true;</script>");
document.write("<script src='//share.loginradius.com/Content/js/LoginRadius.js' type='text/javascript'></script>");
window.onload = function () {
    $("#edit-socialloginandsocialshare-share-horizontal-images-0,#edit-socialloginandsocialshare-share-horizontal-images-1").click(function () {
        sharing_horizontal_show();
    });
    $("#edit-socialloginandsocialshare-share-horizontal-images-2,#edit-socialloginandsocialshare-share-horizontal-images-3").click(function () {
        sharing_horizontal_show();
        sharing_simplehorizontal_show();
    });
    $("#edit-socialloginandsocialshare-share-horizontal-images-8,#edit-socialloginandsocialshare-share-horizontal-images-9").click(function () {
        counter_horizontal_show();
    });
    $("#edit-socialloginandsocialshare-share-vertical-images-4,#edit-socialloginandsocialshare-share-vertical-images-5").click(function () {
        sharing_vertical_show();
    });
    $("#edit-socialloginandsocialshare-share-vertical-images-6,#edit-socialloginandsocialshare-share-vertical-images-7").click(function () {
        counter_vertical_show();
    });
    sharingproviderlist();
    counterproviderlist();
    $("#socialloginandsocialshare_rearrange_providers").sortable({
        revert: true
    });
    $("#socialshare_vertical_rearrange_providers").sortable({
        revert: true
    });
    if (!$("#edit-socialshare-position-top").checked && !$("#edit-socialshare-position-bottom").checked) {
        $("#edit-socialshare-position-top").attr('checked', 'checked');
    }
    if (selected_sharing_theme)
        loginRadiusToggleShareTheme(selected_sharing_theme.val());
    $("#socialloginandsocialshare_veritical").click(function () {
        loginRadiusToggleShareTheme("vertical");
    });
    $("#socialloginandsocialshare_horizontal").click(function () {
        loginRadiusToggleShareTheme("horizontal");
    });
}
// Sharing Theme selected.
function loginRadiusToggleShareTheme(theme) {
    var verticalDisplay = 'none';
    var horizontalDisplay = 'block';
    var socialloginandsocialshare_horizontal =  $("#socialloginandsocialshare_horizontal");
    var socialloginandsocialshare_veritical = $("#socialloginandsocialshare_veritical");
    var vertical_position = $(".form-item.form-type-radios.form-item-socialshare-vertical-position");
    if (theme == "vertical") {
        verticalDisplay = 'block';
        horizontalDisplay = 'none';
        socialloginandsocialshare_horizontal.removeClass("active");
        socialloginandsocialshare_veritical.addClass("active");
        vertical_position.removeClass("element-invisible");
    } else {
        socialloginandsocialshare_horizontal.addClass("active");
        socialloginandsocialshare_veritical.removeClass("active");
        vertical_position.addClass("element-invisible");
    }
    $(".form-radios.socialloginandsocialshare_share_horizontal_images").css("display", horizontalDisplay);
    $(".form-radios.socialloginandsocialshare_share_vertical_images").css("display", verticalDisplay);
}
function sharingproviderlist() {
    var sharing = $SS.Providers.More;
    var div = $('#socialshare_providers_list-wrapper');
    var div_vertical = $('#socialshare_vetical_show_providers_list-wrapper');
    if (div && div_vertical) {
        for (var i = 0; i < sharing.length; i++) {
            var listItem = $("<div class= 'form-item form-type-checkbox form-item-socialshare-show-providers-list-" + sharing[i].toLowerCase() + "'><input type='checkbox' id='edit-socialshare-show-providers-list-" + sharing[i].toLowerCase() + "' onChange='loginRadiusSharingLimit(this),loginRadiusRearrangeProviderList(this)' name='socialshare_show_providers_list[" + sharing[i].toLowerCase() + "]' value='" + sharing[i].toLowerCase() + "' class='form-checkbox' /><label for='edit-socialshare-show-providers-list-" + sharing[i].toLowerCase() + "' class='option'>" + sharing[i] + "</label></div>");
            div.append(listItem);
            var listItem = $("<div class= 'form-item form-type-checkbox form-item-socialshare-vertical-show-providers-list-" + sharing[i].toLowerCase() + "'><input type='checkbox' id='edit-socialshare-vertical-show-providers-list-" + sharing[i].toLowerCase() + "' onChange='loginRadiusverticalSharingLimit(this),loginRadiusverticalRearrangeProviderList(this)' name='socialshare_vertical_show_providers_list[" + sharing[i].toLowerCase() + "]' value='" + sharing[i].toLowerCase() + "' class='form-checkbox' /><label for='edit-socialshare-vertical-show-providers-list-" + sharing[i].toLowerCase() + "' class='option'>" + sharing[i] + "</label></div>");
            div_vertical.append(listItem);
        }
        $('input[name^="socialloginandsocialshare_rearrange_providers_list"]').each(function () {
            var elem = $(this);
            if (!elem.checked) {
                $('#edit-socialshare-show-providers-list-' + elem.val()).attr('checked', 'checked');
            }
        });
        $('input[name^="socialshare_vertical_rearrange_providers_list"]').each(function () {
            var elem = $(this);
            if (!elem.checked) {
                $('#edit-socialshare-vertical-show-providers-list-' + elem.val()).attr('checked', 'checked');
            }
        });
    }
}
/*
 * Show sharing Rearrange Providers.
 */
function loginRadiusRearrangeProviderList(elem) {
    var ul = $('#socialloginandsocialshare_rearrange_providers');
    var input = $('#socialloginandsocialshare_chnage_name');
    if (elem.checked) {
        var provider = $("<li id='edit-lrshare-iconsprite32" + elem.value + "' title='" + elem.value + "' class='lrshare_iconsprite32 lrshare_" + elem.value + "'><input type='hidden' value='" + elem.value + "' name='socialloginandsocialshare_rearrange_providers_list[]' id='input-lrshare-" + elem.value + "'></li>");
        ul.append(provider);
    } else {
        if ($('#edit-lrshare-iconsprite32' + elem.value)) {
            $('#edit-lrshare-iconsprite32' + elem.value).remove();
        }
    }
}
// vertical Sharing Rearrange counter
function loginRadiusverticalRearrangeProviderList(elem) {
    var ul = $('#socialshare_vertical_rearrange_providers');
    var input = $('#socialshare_chnage_name');
    if (elem.checked) {
        var provider = $("<li id='edit-lrshare-iconsprite32_vertical" + elem.value + "' title='" + elem.value + "' class='lrshare_iconsprite32 lrshare_" + elem.value + "'><input type='hidden' value='" + elem.value + "' name='socialshare_vertical_rearrange_providers_list[]' id='input-lrshare-vertical-" + elem.value + "'></li>");
        ul.append(provider);
    } else {
        if ($('#edit-lrshare-iconsprite32_vertical' + elem.value)) {
            $('#edit-lrshare-iconsprite32_vertical' + elem.value).remove();
        }
    }
}
// check limit for horizontal Social sharing.
function loginRadiusSharingLimit(elem) {
    var checkCount = $('input[name^="socialloginandsocialshare_rearrange_providers_list"]').length;
    if (elem.checked) {
        // count checked providers
        checkCount++;
        if (checkCount >= 10) {
            elem.checked = false;
            $("#loginRadiusSharingLimit").show('slow');
            setTimeout(function () {
                $("#loginRadiusSharingLimit").hide('slow');
            }, 2000);
            return;
        }
    }
}
// check limit for vertical Social sharing.
function loginRadiusverticalSharingLimit(elem) {
    var checkCount = $('input[name^="socialshare_vertical_rearrange_providers_list"]').length;
    if (elem.checked) {
        // count checked providers
        checkCount++;
        if (checkCount >= 10) {
            elem.checked = false;
            $("#loginRadiusSharingLimit_vertical").show('slow');
            setTimeout(function () {
                $("#loginRadiusSharingLimit_vertical").hide('slow');
            }, 2000);
            return;
        }
    }
}
// show Provider List for horizontal Social counter.
function counterproviderlist() {
    var counter = $SC.Providers.All;
    var div = $('#socialcounter_show_providers_list-wrapper');
    var div_vertical = $('#socialcounter_vertical_show_providers_list-wrapper');
    if (div && div_vertical) {
        for (var i = 0; i < counter.length; i++) {
            var value = counter[i].split(' ').join('');
            value = value.replace("++", "plusplus");
            value = value.replace("+", "plus");
            var listItem = $("<div class= 'form-item form-type-checkbox form-item-socialshare_counter_show_providers_list-" + counter[i] + "'><input type='checkbox' id='edit-socialshare-counter-show-providers-list-" + value + "' onChange='loginRadiuscounterProviderList(this)' name='socialcounter_providers_list[]' value='" + counter[i] + "' class='form-checkbox' /><label for='edit-socialshare-counter-show-providers-list-" + value + "' class='option'>" + counter[i] + "</label></div>");
            div.append(listItem);
            var listItem = $("<div class= 'form-item form-type-checkbox form-item-socialshare_counter_vertical_show_providers_list-" + counter[i] + "'><input type='checkbox' id='edit-socialshare-counter-vertical-show-providers-list-" + value + "' onChange='loginRadiuscounterverticalProviderList(this)' name='socialcounter_new_vertical_providers_list[]' value='" + counter[i] + "' class='form-checkbox' /><label for='edit-socialshare-counter-vertical-show-providers-list-" + value + "' class='option'>" + counter[i] + "</label></div>");
            div_vertical.append(listItem);
        }
        $('input[name^="socialcounter_rearrange_providers_list"]').each(function () {
            var elem = $(this);
            if (!elem.checked) {
                var value = elem.val().split(' ').join('');
                value = value.replace("++", "plusplus");
                value = value.replace("+", "plus");
                $('#edit-socialshare-counter-show-providers-list-' + value).attr('checked', 'checked');
            }
        });
        $('input[name^="socialcounter_vertical_rearrange_providers_list"]').each(function () {
            var elem = $(this);
            if (!elem.checked) {
                var value = elem.val().split(' ').join('');
                value = value.replace("++", "plusplus");
                value = value.replace("+", "plus");
                $('#edit-socialshare-counter-vertical-show-providers-list-' + value).attr('checked', 'checked');
            }
        });
    }
}
/*
 * Show Counter Providers selected.
 */
function loginRadiuscounterProviderList(elem) {
    var ul = $('#socialcounter_show_providers_list-wrapper');
    var raw = elem.value;
    var value = elem.value.split(' ').join('');
    value = value.replace("++", "plusplus");
    value = value.replace("+", "plus");
    if (elem.checked) {
        var provider = $("<input type='hidden' value='" + raw + "' name='socialcounter_rearrange_providers_list[]' id='input-lrcounter-" + elem.value + "'>");
        ul.append(provider);
    } else {
        $('#input-lrcounter-' + value).remove();
        $('#edit-' + value).remove();
    }
}
// Provider list selcted in vertical counter.
function loginRadiuscounterverticalProviderList(elem) {
    var ul = $('#socialcounter_vertical_show_providers_list');
    var raw = elem.value;
    var value = elem.value.split(' ').join('');
    value = value.replace("++", "plusplus");
    value = value.replace("+", "plus");
    if (elem.checked) {
        var provider = $("<input type='hidden' value='" + raw + "' name='socialcounter_vertical_rearrange_providers_list[]' id='input-lrcounter-vertical-" + value + "'>");
        ul.append(provider);
    } else {
        $('#input-lrcounter-vertical-' + value).remove();
        $('#edit-lrshare-vertical-' + value).remove();
    }
}
// show Sharing Horizontal
function sharing_horizontal_show() {
    toggle_sharing_counter(true);
}
// show Counter Horizontal .
function counter_horizontal_show() {
    toggle_sharing_counter(false);
}
function sharing_simplehorizontal_show() {
    toggle_sharing_counter(true, true);
}
function toggle_sharing_counter(is_social_share, is_social_counter) {
    var simple_sharing = is_social_share ? "addClass" : "removeClass";
    var simple_counter = is_social_share ? "removeClass" : "addClass";
    if (is_social_counter) {
        simple_counter = "addClass";
    }
    $("#socialcounter_show_providers_list-wrapper")[simple_sharing]("element-invisible");
    $("#socialshare_providers_list-wrapper")[simple_counter]("element-invisible");
    $("#edit-socialshare-horizontal-location-content-wrapper").parent().parent()[simple_sharing]("element-invisible");
    $("#rearrange_sharing_text")[simple_counter]("element-invisible");
    $("#socialloginandsocialshare_rearrange_providers")[simple_counter]("element-invisible");
    $("#edit-socialshare-horizontal-location-content-wrapper").parent().parent()[simple_counter]("addtopmargin");
}
// Show sharing vertical.
function sharing_vertical_show() {
    toggle_sharing_vertical_show(true);
}
// show Counter Vertical.
function counter_vertical_show() {
    toggle_sharing_vertical_show(false);
}

function toggle_sharing_vertical_show(is_social_share) {
    var simple_vertical_sharing = is_social_share ? "addClass" : "removeClass";
    var simple_vertical_counter = is_social_share ? "removeClass" : "addClass";
    $("#socialcounter_vertical_show_providers_list-wrapper")[simple_vertical_sharing]("element-invisible");
    $("#socialshare_vetical_show_providers_list-wrapper")[simple_vertical_counter]("element-invisible");
    $("#rearrange_sharing_text_vertical")[simple_vertical_counter]("element-invisible");
    $("#socialshare_vertical_rearrange_providers")[simple_vertical_counter]("element-invisible");
    $("#edit-socialshare-show-pages-0-wrapper").parent().parent()[simple_vertical_counter]("addtopmargin");
}
function toggle_horizontal_widget(is_horizontal) {
    var horizontal_sharing = is_horizontal ? "addClass" : "removeClass";
    var vertical_sharing = is_horizontal ? "removeClass" : "addClass";
    $("#socialloginandsocialshare_show_horizotal_widget")[horizontal_sharing]("element-invisible");
    $("#socialloginandsocialshare_show_veritcal_widget")[vertical_sharing]("element-invisible");
    $("#edit-socialshare-show-pages-0-wrapper").parent().parent()[horizontal_sharing]("element-invisible");
    $("#edit-socialshare-show-exceptpages-wrapper")[horizontal_sharing]("element-invisible");
    $("#edit-socialshare-vertical-show-pages-0-wrapper").parent().parent()[vertical_sharing]("element-invisible");
    $("#edit-socialshare-vertical-show-exceptpages-wrapper")[vertical_sharing]("element-invisible");
    $("#edit-socialshare-label-string-wrapper")[horizontal_sharing]("element-invisible");
    $("#horizontal_sharing_show")[horizontal_sharing]("element-invisible");
    $('.form-radios.socialloginandsocialshare_enable_horizontal_share').parent()[horizontal_sharing]("element-invisible");
    $("#edit-socialshare-top-weight-wrapper")[horizontal_sharing]("element-invisible");
    $("#edit-socialshare-bottom-weight-wrapper")[horizontal_sharing]("element-invisible");
    $('.form-radios.socialloginandsocialshare_enable_vertical_share').parent()[vertical_sharing]("element-invisible");
    $('#edit-socialshare-horizontal-location-content-wrapper').parent().parent()[horizontal_sharing]("element-invisible");
}
// Show only vertical widget options.
function hidden_horizontal_widget() {
    $(".form-radios.socialshare_vertical_position").parent().css('clear', 'both');
    toggle_horizontal_widget(true);

}
// Show only Horizontal widget options.
function display_horizontal_widget() {
    toggle_horizontal_widget(false);
}

