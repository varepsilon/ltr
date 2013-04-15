# Copyright 2013 Yandex
from django.test import TestCase
from ..file_utility import file_not_empty
import tempfile


class FileUtilityTest(TestCase):

    """Tests functions in `file_utility.py`."""

    def test_file_not_empty(self):
        """Check `file_not_empty` on empty and nonexisting files."""
        temp_filename = tempfile.mktemp()
        self.assertFalse(file_not_empty(temp_filename))
        open(temp_filename, 'w').close()
        self.assertFalse(file_not_empty(temp_filename))
        temp_file = open(temp_filename, 'w')
        temp_file.write('Now i\'m not empty!')
        temp_file.close()
        self.assertTrue(file_not_empty(temp_filename))
