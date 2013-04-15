# Copyright 2013 Yandex
from django.test import TestCase
from django.test.client import Client
from django.contrib.auth.models import User
from ..models import BaseObject
import os
import re


class LTRObjectsTestCase(TestCase):

    """Some fuctions to make testing LTR objects easier."""

    _testfiles = os.path.abspath(os.path.dirname(__file__)) + '/testfiles'

    def setUp(self):
        """Create django client for surfing."""
        self.client = Client()

    def tearDown(self):
        self.delete_all_objects()
        if hasattr(self, 'user'):
            self.user.delete()

    def login(self, client=None):
        """Creates new user and login."""
        if client is None:
            client = self.client
        self.user = User.objects.create_user('test_name',
                                             'test_name@test_server.com',
                                             'test_password')
        self.user.save()
        client.login(username='test_name', password='test_password')

    def check_object_existence(self,
                               object_name,
                               must_exist,
                               client=None):
        """Checks existence of object in database and through client."""
        if client is None:
            client = self.client
        self.assertEqual(BaseObject.objects.filter(name=object_name).exists(),
                         must_exist)
        response = client.get('/')
        self.assertEqual(response.status_code, 200)
        link_regexp = '<p><a href="(/view/\w+)">' + object_name + '</a></p>'
        link_search = re.search(link_regexp, response.content)
        self.assertEqual(link_search is not None, must_exist)
        if must_exist:
            response = self.client.get(link_search.group(1))
            self.assertEqual(response.status_code, 200)

    def create_object(self,
                      object_category,
                      object_type,
                      object_name='test_object',
                      client=None,
                      **object_parameters):
        """Creates object through client and checks every step."""
        if client is None:
            client = self.client
        self.check_object_existence(object_name, False)
        response = self.client.post('/create',
                                    dict(object_parameters,
                                         category_=object_category,
                                         type_=object_type,
                                         name=object_name),
                                    follow=True)
        self.assertEqual(response.status_code, 200)
        self.assertNotIn("This field is required.", response.content)
        self.assertEqual(response.redirect_chain,
                         [('http://testserver/', 302)])
        self.assertIn(object_name, response.content)
        self.check_object_existence(object_name, True)

    def delete_object(self, object_name, client=None):
        """Deletes object through client and checks every step."""
        if client is None:
            client = self.client
        self.check_object_existence(object_name, True)
        response = self.client.post('/delete/' + object_name, follow=True)
        self.assertEqual(response.status_code, 200)
        self.assertEqual(response.redirect_chain,
                         [('http://testserver/', 302)])
        self.check_object_existence(object_name, False)

    def delete_all_objects(self, client=None):
        """Deletes all objects through special button."""
        if client is None:
            client = self.client
        response = self.client.post('/delete_all', follow=True)
        self.assertEqual(response.status_code, 200)
        self.assertEqual(response.redirect_chain,
                         [('http://testserver/', 302)])
        self.assertIn("No objects were created", response.content)
        self.assertEqual(BaseObject.objects.count(), 0)
