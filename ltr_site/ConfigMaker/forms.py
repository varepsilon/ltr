# Copyright 2011 Yandex

from django import forms
from django.core.validators import RegexValidator
from re import compile
import models

def choices(list):
    return zip(list, list)

# Form for selecting object's category and type
class FormType(forms.Form):
    category_ = forms.ChoiceField(choices=choices(models.object_controller.getCategories()),
                                  label='Category',
                                  widget=forms.Select(attrs={'onchange': 'get_object_types()'}))
    type_ = forms.ChoiceField(choices=choices(models.object_controller.getAllObjectTypes()),
                              label='Type',
                              widget=forms.Select(attrs={'onchange': 'get_object_settings()'}))

    def __init__(self, *args, **kwargs):
        isDisabled = kwargs.pop('disabled', False)
        super(FormType, self).__init__(*args, **kwargs)
        if isDisabled:
            self.fields['category_'].widget.attrs['disabled'] = 'disabled'
            self.fields['type_'].widget.attrs['disabled'] = 'disabled'