# Copyright 2013 Yandex
"""This file demonstrates writing tests using the unittest module. These will
pass when you run "manage.py test" or "manage.py test config_maker".

Replace this with more appropriate tests for your application.

"""

from django.test import TestCase


class SimpleTest(TestCase):
    def test_basic_addition(self):
        """Tests that 1 + 1 always equals 2."""
        self.assertEqual(1 + 1, 2)

    def test_basic_multiplication(self):
        """Tests that 2 * 2 always equals 4."""
        self.assertEqual(2 * 2, 4)
