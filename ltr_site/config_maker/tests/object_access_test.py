# Copyright 2013 Yandex
from ltr_object_test_case import LTRObjectsTestCase
from django.test.client import Client
from django.contrib.auth.models import User
from ..file_utility import get_unique_name


class UserPermissionTestCase(LTRObjectsTestCase):

    """Tests access to objects.

    Creates objects and checks that they are visible only to one user.

    """

    _object_category = 'measure'
    _object_type = 'AbsError'
    _object_name = 'test_measure'

    def check_permissions(self, creator, accessor):
        """Creates data through creator and checks that accessor has no access
        to it.

        """
        self.create_object(self._object_category,
                           self._object_type,
                           self._object_name,
                           creator)
        for client, must_have_access in ((creator, True),
                                         (accessor, False)):
            self.assertEqual(must_have_access,
                             self.is_in_object_list(client, self._object_name))
            for test_type in ('CrossValidation', 'BestFeatureLearner'):
                self.assertEqual(must_have_access,
                                 self.is_in_parameters(client,
                                                       self._object_name,
                                                       test_type))

    def test_authentificated_to_authentificated(self):
        """Checks that authentificated user has no access to authentificated
        user data.

        """
        creator, accessor = Client(), Client()
        self.login('test_user_1', creator)
        self.login('test_user_2', accessor)
        self.check_permissions(creator, accessor)

    def test_authentificated_to_anonymous(self):
        """Checks that authentificated user has no access to anonymous user
        data.

        """
        creator, accessor = Client(), Client()
        self.login('test_user', accessor)
        self.check_permissions(creator, accessor)

    def test_anonymous_to_authentificated(self):
        """Checks that anonymous user has no access to authentificated user
        data.

        """
        creator, accessor = Client(), Client()
        self.login('test_user', creator)
        self.check_permissions(creator, accessor)

    def test_anonymous_to_anonymous(self):
        """Checks that anonymous user has no access to anonymous user data."""
        creator, accessor = Client(), Client()
        self.check_permissions(creator, accessor)
