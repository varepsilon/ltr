from django.template import Library
from copy import deepcopy

register = Library()


@register.filter
def special_properties(properties_list):
    properties = deepcopy(properties_list)
    properties.pop('name_', '')
    properties.pop('type_', '')
    properties.pop('category_', '')
    properties.pop('approach_', '')
    properties.pop('format_', '')
    return properties.items()
