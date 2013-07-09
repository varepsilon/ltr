# Copyright 2013 Yandex
"""Set of function for parsing and formatting log file of Ltr Client."""
from django.conf import settings

import datetime
import re


class LogEntry(object):
    """Describes one log entry"""
    level = 'info'
    time = None
    file_path = ''
    message = ''


def __split_log(log):
    log_prefix_regexp = (r'^(\d{2}:\d{2}:\d{2}) ' +
                         r'\[(info|warning|error|debug)\] ' +
                         r'\((.*:\d*)\) ')
    entries = []
    last_text_start = 0
    for match in re.finditer(log_prefix_regexp, log, flags=re.MULTILINE):
        if len(entries) > 0:
            entries[-1].message = log[last_text_start:match.start()]
        log_entry = LogEntry()
        log_entry.level = match.group(2)
        log_entry.time = datetime.datetime.strptime(match.group(1), '%H:%M:%S')
        log_entry.file_path = match.group(3)
        entries.append(log_entry)
        last_text_start = match.end()
    if len(entries) > 0:
        entries[-1].message = log[last_text_start:]
    return entries


def __replace_files_with_urls(text):
    filename_regexp = settings.MEDIA_ROOT + r'(\S+\.\w+)'

    def replace_function(text):
        from django.template.loader import render_to_string
        return render_to_string('link_to_file.html',
                                {'link': settings.MEDIA_URL + text.group(1),
                                 'STATIC_URL': settings.STATIC_URL}).strip()

    return re.sub(filename_regexp, replace_function, text)


def __add_br_tags(text):
    return re.sub('\n', '</br>\n', text)


def parse_log(log):
    """Parses log file into list of LogEntry objects."""
    entries = __split_log(log)

    filter_list = (lambda text: text.strip(),
                   __replace_files_with_urls,
                   __add_br_tags)

    for entry in entries:
        for filter_function in filter_list:
            entry.message = filter_function(entry.message)

    return entries
