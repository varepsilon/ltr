// Copyright 2011 Yandex
// Uses Prototype library (MIT licence)

function get_object_types(){
    new Ajax.Request('/get_object_types', { 
    method: 'post',
    parameters: $H({'category': $('id_category_').getValue()}),
    onSuccess: function(transport) {
        var e = $('id_type_')
        if(transport.responseText){
            e.update(transport.responseText)
            get_object_settings()
        }
    }
    }); // end new Ajax.Request
}

function get_object_settings(){
    new Ajax.Request('/get_object_settings', { 
    method: 'post',
    parameters: $H({'type': $('id_type_').getValue()}),
    onSuccess: function(transport) {
        var e = $('id_settings_')
        if(transport.responseText)
            e.update(transport.responseText)
    }
    }); // end new Ajax.Request
}

function submit_with_confirm(form, message="Are you sure you want to submit the form?") {
    if (confirm(message)) {
        form.submit();
    }
}