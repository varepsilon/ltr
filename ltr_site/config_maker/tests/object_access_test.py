# Copyright 2013 Yandex
from django.test.client import Client

from ltr_object_test_case import LTRObjectsTestCase
from ..models import get_current_solution

from os.path import join


class UserPermissionTestCase(LTRObjectsTestCase):

    """Tests access to objects.

    Creates objects and checks that they are visible only to one user.

    """

    def check_permissions(self, creator, accessor):
        """Creates data through creator and checks that accessor has no access
        to it.

        """
        measure_id = self.create_object('measure',
                                        'AbsError',
                                        'test_measure',
                                        creator)
        for client, must_have_access in ((creator, True),
                                         (accessor, False)):
            self.assertEqual(must_have_access,
                             self.is_in_object_list(client, 'test_measure'))
            for test_type in ('CrossValidation', 'BestFeatureLearner'):
                self.assertEqual(must_have_access,
                                 self.is_in_parameters(client,
                                                       'test_measure',
                                                       test_type))

            client.post('/create', {'category_': 'learner',
                                    'type_': 'BestFeatureLearner',
                                    'name': 'test_learner',
                                    'approach': 'pointwise',
                                    'measure': measure_id}, follow=True)
            self.check_object_existence('test_learner',
                                        must_have_access,
                                        client)
            if must_have_access:
                self.delete_object('test_learner', client)

            learner_id = self.create_object('learner',
                                            'LinearLearner',
                                            'test_learner',
                                            client,
                                            approach='pointwise')
            data_id = self.create_object('data',
                                         'File',
                                         'test_file',
                                         client,
                                         approach='pointwise',
                                         format='Yandex',
                                         file=open(join(self._testfiles,
                                                        'data.yandex')))
            splitter_id = self.create_object('splitter',
                                             'LeaveOneOutSplitter',
                                             'test_splitter',
                                             client,
                                             approach='pointwise')
            client.post('/create', {'category_': 'launch',
                                    'type_': 'CrossValidation',
                                    'name': 'test_launch',
                                    'measures': measure_id,
                                    'data': data_id,
                                    'splitter': splitter_id,
                                    'learners': learner_id}, follow=True)
            self.check_object_existence('test_launch',
                                        must_have_access,
                                        client)
            if must_have_access:
                self.delete_object('test_launch', client)
            self.delete_object('test_learner', client)
            self.delete_object('test_file', client)
            self.delete_object('test_splitter', client)
        self.delete_all_objects(creator)

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
