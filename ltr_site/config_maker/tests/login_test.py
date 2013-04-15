# Copyright 2013 Yandex
from django.test import TestCase
from django.test.client import Client
from django.contrib.auth.models import User


class LoginTest(TestCase):

    """Checks login system."""

    def setUp(self):
        """Creates test user."""
        self.test_user = User.objects.create_user('test_name',
                                                  'test_name@test_server.com',
                                                  'test_password')
        self.test_user.save()

    def tearDown(self):
        """Deletes user created in setUp."""
        self.test_user.delete()

    def test_nonexistent_user(self):
        """Tests login form if username is not existed."""
        client = Client()
        response = client.post('/login?next=/',
                               {'username': 'nonexistent_user',
                                'password': 'password'}, follow=True)
        self.assertEqual(response.status_code, 200)
        self.assertEqual(response.redirect_chain,
                         [('http://testserver/?login_failed=true', 302)])
        self.assertIn('Incorrect login/password combination',
                      response.content)

    def test_incorrect_password(self):
        """Tests login form if password is incorrect."""
        client = Client()
        response = client.post('/login?next=/',
                               {'username': 'test_name',
                                'password': 'incorrect_password'}, follow=True)
        self.assertEqual(response.status_code, 200)
        self.assertEqual(response.redirect_chain,
                         [('http://testserver/?login_failed=true', 302)])
        self.assertIn('Incorrect login/password combination',
                      response.content)

    def test_correct_login(self):
        """Tests login form if login and password are both correct."""
        client = Client()
        response = client.post('/login?next=/',
                               {'username': 'test_name',
                                'password': 'test_password'}, follow=True)
        self.assertEqual(response.status_code, 200)
        self.assertEqual(response.redirect_chain,
                         [('http://testserver/', 302)])
        self.assertIn('Hello, <a href="/users/test_name">test_name</a>',
                      response.content)
