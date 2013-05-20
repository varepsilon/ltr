# Copyright 2013 Yandex
from django.test import TestCase
from django.test.client import Client
from django.contrib.auth.models import User
from ..models import BaseObject
from ..file_utility import get_unique_name
import os


class LTRObjectsTestCase(TestCase):

    """Some fuctions to make testing LTR objects easier."""

    _testfiles = os.path.join(os.path.abspath(os.path.dirname(__file__)),
                              'testfiles')

    def setUp(self):
        """Create django client for surfing."""
        self.client = Client()

    def tearDown(self):
        self.delete_all_objects()
        if hasattr(self, 'user'):
            self.user.delete()

    def login(self, username=None, client=None):
        """Creates new user and logs client in."""
        if client is None:
            client = self.client
        if username is None:
            user_exist = lambda x: User.objects.filter(username=x).count() == 1
            username = get_unique_name('user', existence_predicate=user_exist)
        if User.objects.filter(username=username).count() == 0:
            User.objects.create_user(username,
                                     'test_name@test_server.com',
                                     'test_password').save()
        client.login(username=username, password='test_password')

    def is_in_object_list(self, client, object_name):
        """Checks that there is object in object list."""
        response = client.get('/')
        self.assertEqual(response.status_code, 200)
        link_string = '<a href="/view/{0}">{0}</a>'.format(object_name)
        return link_string in response.content

    def is_in_parameters(self, client, object_name, parametrized_object_type):
        """Checks that there is object in parametrized_object_name parameters.

        """
        response = client.post('/get_object_parameters',
                               {'type': parametrized_object_type,
                                'mode': 'create_mode'},
                               HTTP_X_REQUESTED_WITH='XMLHttpRequest')
        self.assertEqual(response.status_code, 200)
        return object_name in response.content

    def check_object_existence(self,
                               object_name,
                               must_exist,
                               client=None):
        """Checks existence of object in database and through client."""
        if client is None:
            client = self.client
        self.assertEqual(BaseObject.objects.filter(name=object_name).exists(),
                         must_exist)
        self.assertEqual(self.is_in_object_list(client, object_name),
                         must_exist)
        response = client.get('/view/' + object_name)
        if must_exist:
            self.assertEqual(response.status_code, 200)
        else:
            self.assertEqual(response.status_code, 404)

    def create_object(self,
                      object_category,
                      object_type,
                      object_name='test_object',
                      client=None,
                      **object_parameters):
        """Creates object through client and checks every step."""
        if client is None:
            client = self.client
        self.check_object_existence(object_name, False, client)
        response = client.post('/create',
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
        self.check_object_existence(object_name, True, client)

    def delete_object(self, object_name, client=None):
        """Deletes object through client and checks every step."""
        if client is None:
            client = self.client
        self.check_object_existence(object_name, True, client)
        response = client.post('/delete/' + object_name, follow=True)
        self.assertEqual(response.status_code, 200)
        self.assertEqual(response.redirect_chain,
                         [('http://testserver/', 302)])
        self.check_object_existence(object_name, False, client)

    def delete_all_objects(self, client=None):
        """Deletes all objects through special button."""
        if client is None:
            client = self.client
        response = client.post('/delete_all', follow=True)
        self.assertEqual(response.status_code, 200)
        self.assertEqual(response.redirect_chain,
                         [('http://testserver/', 302)])
        self.assertIn("No objects were created", response.content)
