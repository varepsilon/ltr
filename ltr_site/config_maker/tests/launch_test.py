# Copyright 2013 Yandex

from django.conf import settings
from ltr_object_test_case import LTRObjectsTestCase
from time import sleep
from shutil import rmtree
from os.path import join


class launchTestCase(LTRObjectsTestCase):
    def setUp(self):
        """Creates objects for trainings."""
        super(LTRObjectsTestCase, self).setUp()
        rmtree(join(settings.MEDIA_ROOT, 'test_user', 'task'),
               ignore_errors=True)
        self.login('test_user')
        self.create_object('measure', 'AbsError', 'test_measure')
        self.create_object('learner',
                           'BestFeatureLearner',
                           'test_learner',
                           approach='pointwise',
                           measure=1)
        self.create_object('data',
                           'File',
                           'test_file',
                           approach='pointwise',
                           format='Yandex',
                           file=open(join(self._testfiles, 'data.yandex')))
        self.create_object('launch',
                           'Train',
                           'test_train',
                           train_data=3,
                           learner=2,
                           generate_cpp=1,
                           predict=3)

    def check_file(self, filename):
        """Checks that `parent_url` page contains `url` and `url` and
        `filename` file contents are equal.

        """
        url = '/users/test_user/task/' + filename
        parent_url = '/users/test_user/task/report.html'
        response = self.client.get(parent_url)
        self.assertEqual(response.status_code, 200)
        self.assertIn(url, response.content)
        response = self.client.get(url)
        self.assertEqual(response.status_code, 200)
        expected_response_file = open(join(self._testfiles, filename + '.exp'))
        expected_response = expected_response_file.read()
        expected_response_file.close()
        self.assertEqual(response.content, expected_response)

    def test_launch(self):
        response = self.client.post('/launch/test_train', follow=True)
        self.assertEqual(response.status_code, 200)
        self.assertEqual(response.redirect_chain,
                         [('http://testserver/profile', 302)])
        tries_left = 20
        while tries_left > 0 and 'In progress...' in response.content:
            tries_left -= 1
            sleep(1)
            response = self.client.get('/profile')
            self.assertEqual(response.status_code, 200)

        self.check_file('test_learner.test_file.predicts')
        self.check_file('test_learner.cpp')
