from django.template import Library
from django.conf import settings

import copy
import re

register = Library()


@register.filter
def special_properties(properties_list):
    properties = copy.deepcopy(properties_list)
    properties.pop('name', '')
    properties.pop('type', '')
    properties.pop('category', '')
    properties.pop('approach', '')
    properties.pop('format', '')
    return properties.items()


@register.filter
def path_to_url(path):
    return re.sub(settings.MEDIA_ROOT, settings.MEDIA_URL, path)
