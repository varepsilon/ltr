# Copyright 2011 Yandex

import os
import itertools


def get_unique_name(name, existing_names=(),
                    existence_predicate=lambda x: False):
    """ Appends a counter to the end of the name if
    such name already exists."""
    # TODO: add doctests
    if name not in existing_names and not existence_predicate(name):
        return name

    root, extension = os.path.splitext(name)
    for i in itertools.count(1):
        new_name = "%s_%d%s" % (root, i, extension)
        if new_name not in existing_names and not existence_predicate(name):
            return new_name


def get_unique_filename(filename):
    """ Appends a counter to the end of file name if
    such file already exists."""
    return get_unique_name(os.path.basename(filename), (), os.path.exists)


def ensure_path_exists(path):
    if not os.path.exists(path):
        os.makedirs(path)


def file_not_empty(filename):
    if not os.path.isfile(filename):
        return False
    try:
        open(filename).close()
        return os.path.getsize(filename) > 0
    except IOError:
        return False
