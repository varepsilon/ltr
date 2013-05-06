# Copyright 2013 Yandex
from ltr_object_test_case import LTRObjectsTestCase


class CreatingDeletingTestCase(LTRObjectsTestCase):

    """Checks creating different objects."""

    def create_delete_object(self,
                             object_category,
                             object_type,
                             object_name="object",
                             **object_parameters):
        """Checks operations with type.

        This function creates object of given category and type and deletes it
        through http client. It checks the object to be when it must be and
        not to be when it mustn't be.

        """
        self.create_object(object_category,
                           object_type,
                           object_name,
                           **object_parameters)
        self.delete_object(object_name)

    def test_leave_one_out_splitter(self):
        self.create_delete_object('splitter',
                                  'LeaveOneOutSplitter',
                                  approach='pointwise')

    def test_abs_error(self):
        self.create_delete_object('measure', 'AbsError')

    def test_average_precision(self):
        self.create_delete_object('measure', 'AveragePrecision')

    def test_best_feature_learner(self):
        self.create_object('measure', 'AbsError', 'test_measure')
        self.create_delete_object('learner',
                                  'BestFeatureLearner',
                                  approach='pointwise',
                                  measure=1)

    def test_file(self):
        self.create_delete_object('data',
                                  'File',
                                  approach='pointwise',
                                  format='Yandex',
                                  file=open(self._testfiles + '/data.yandex'))

    def test_train(self):
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
                           file=open(self._testfiles + '/data.yandex'))
        self.create_delete_object('launch',
                                  'Train',
                                  train_data=3,
                                  learner=2,
                                  generate_cpp=1,
                                  predict=3)

    def test_cross_validation(self):
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
                           file=open(self._testfiles + '/data.yandex'))
        self.create_object('splitter',
                           'LeaveOneOutSplitter',
                           'test_splitter',
                           approach='pointwise')
        self.create_delete_object('launch',
                                  'CrossValidation',
                                  data=3,
                                  splitter=4,
                                  learners=2,
                                  measures=1)


class AuthCreatingDeletingTestCase(CreatingDeletingTestCase):

    """Checks the same operations with authentificated user."""

    def setUp(self):
        super(LTRObjectsTestCase, self).setUp()
        self.login('test_user')
