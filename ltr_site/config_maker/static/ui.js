// Copyright 2013 Yandex
// Uses Prototype library (MIT licence)

function update_object_types() {
    new Ajax.Request('/get_object_types', { 
    method: 'post',
    parameters: $H({'category': $('id_category_').getValue()}),
    onSuccess: function(transport) {
        if(transport.responseText) {
            var object_types = $('id_type_');
            object_types.update(transport.responseText);
            update_object_parameters()
        }
    }
    }); // end new Ajax.Request
}

function update_object_parameters(mode) {
    new Ajax.Request('/get_object_parameters', {
    method: 'post',
    parameters: $H({'type': $('id_type_').getValue(), 'mode': mode}),
    onSuccess: function(transport) {
        if(transport.responseText) {
            var object_parameters = $('id_parameters_');
            object_parameters.update(transport.responseText)
        }
    }
    }); // end new Ajax.Request
}

function submit_with_confirm(form, message) {
    if(typeof(message)==='undefined') {
        message="Are you sure you want to submit the form?";
    }
    if (confirm(message)) {
        form.submit();
    }
}
