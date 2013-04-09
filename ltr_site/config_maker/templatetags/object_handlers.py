# Copyright 2013 Yandex
"""
This file contains filters used in templates.
"""

from django.template import Library
from django.conf import settings

import copy
import re

register = Library()


@register.filter
def special_properties(properties_list):
    """ Returns list of properties that should be children of <object> tag in
    XML config
    :param properties_list: list of all object's properties
    :return: list of desired properties
    """
    properties = copy.deepcopy(properties_list)
    properties.pop('name', '')
    properties.pop('type', '')
    properties.pop('category', '')
    properties.pop('approach', '')
    properties.pop('format', '')
    return properties.items()


@register.filter
def path_to_url(path):
    """ Returns URL corresponding to local path (on server)
    :param path: local path
    :return: corresponding URL
    """
    return re.sub(settings.MEDIA_ROOT, settings.MEDIA_URL, path)
