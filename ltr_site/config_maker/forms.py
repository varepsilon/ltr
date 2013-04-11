# Copyright 2013 Yandex
"""This file contains all the forms used in LTR Site."""

from django import forms

from models import object_controller


def choices(list_):
    """Zips a list for using as ChoiceField.choices."""
    return zip(list_, list_)


class FormType(forms.Form):

    """Form for selecting object's category and type.

    Form can be in edit_mode or create_mode.

    """

    category_ = forms.ChoiceField(
        choices=choices(object_controller.categories),
        label='Category',
        widget=forms.Select(attrs={'onchange': 'update_object_types()'}))
    type_ = forms.ChoiceField(
        choices=choices(object_controller.all_object_types),
        label='Type',
        widget=forms.Select())

    def __init__(self, *args, **kwargs):
        self.mode = kwargs.pop('mode', 'create_mode')
        super(FormType, self).__init__(*args, **kwargs)
        self.fields['type_'].widget.attrs['onchange'] = \
            'update_object_parameters("%s")' % self.mode
        if self.mode == 'edit_mode':
            self.fields['category_'].widget.attrs['disabled'] = 'disabled'
            self.fields['type_'].widget.attrs['disabled'] = 'disabled'
