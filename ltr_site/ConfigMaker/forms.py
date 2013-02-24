# Copyright 2011 Yandex

from django import forms
from django.core.validators import RegexValidator
from re import compile
from object_hierarchy import getCategories, getAllObjectTypes,  \
                             getCreatedObjectNames


def choices(list):
    return zip(list, list)


choicesApproach = choices(('pointwise', 'pairwise', 'listwise'))


# Form for selecting object's category and type
class FormType(forms.Form):
    category_ = forms.ChoiceField(choices=choices(getCategories()),
                                  label='Category',
                                  widget=forms.Select(attrs={'onchange': 'get_object_types()'}))
    type_ = forms.ChoiceField(choices=choices(getAllObjectTypes()),
                              label='Type',
                              widget=forms.Select(attrs={'onchange': 'get_object_settings()'}))

    def __init__(self, *args, **kwargs):
        isDisabled = kwargs.pop('disabled', False)
        super(FormType, self).__init__(*args, **kwargs)
        if isDisabled:
            self.fields['category_'].widget.attrs['disabled'] = 'disabled'
            self.fields['type_'].widget.attrs['disabled'] = 'disabled'


# Base class for all the forms with objects' settings
class FormBase(forms.Form):
    nameHelp = 'Name must be alphanumeric, max 30 symbols,letter first'
    nameRegex = '^[A-Za-z]\w{0,29}$'
    name_ = forms.CharField(validators=[RegexValidator(regex=compile(nameRegex),
                                                       code='invalid')],
                            error_messages={'invalid': nameHelp},
                            widget=forms.TextInput(attrs={'pattern': nameRegex,
                                                          'title': nameHelp,
                                                          'placeholder': 'Name',
                                                          'maxlength': 30,
                                                          'required': True}))

    def __init__(self, *args, **kwargs):
        self.objects = kwargs.pop('objects', ('',))  # Magic: doesn't work if
                                                     # second argument is ()
        self.objectName = kwargs.pop('object_name', '')
        super(FormBase, self).__init__(*args, **kwargs)

    def clean(self):
        cleanedData = super(FormBase, self).clean()
        self.validateNameFree(cleanedData)
        return cleanedData

    def validateObjectExistance(self, cleaned_data, field_name):
        if field_name in cleaned_data.keys():
            name = cleaned_data.get(field_name)
            allNames = (item['name_'] for item in self.objects)
            if name not in allNames:
                message = 'You must select an already existing object'
                self._errors[field_name] = self.error_class([message])
                del cleaned_data[field_name]

    def validateNameFree(self, cleaned_data):
        if 'name_' in cleaned_data.keys():
            name = cleaned_data.get('name_')
            allNames = (item['name_'] for item in self.objects)
            if name in allNames and name != self.objectName:
                message = 'Name already in use'
                self._errors['name_'] = self.error_class([message])
                del cleaned_data['name_']


# Below - forms for every registered object -----------------------------------


class AbsErrorForm(FormBase):
    pass


class AccuracyForm(FormBase):
    pass


class BestFeatureLearnerForm(FormBase):
    approach_ = forms.ChoiceField(choices=choicesApproach,
                                  label='Approach')
    measure_ = forms.ChoiceField(label='Measure')

    def __init__(self, *args, **kwargs):
        super(BestFeatureLearnerForm, self).__init__(*args, **kwargs)
        self.fields['measure_'].choices =  \
                choices(getCreatedObjectNames(self.objects, 'measure'))

    def clean(self):
        cleanedData = super(BestFeatureLearnerForm, self).clean()
        self.validateObjectExistance(cleanedData, 'measure_')
        return cleanedData
