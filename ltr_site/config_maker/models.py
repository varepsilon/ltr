# Copyright 2013 Yandex
"""
This file contains all the models used in LTR Site.
"""

from django.db import models
from django.contrib.auth.models import User
from django.contrib.contenttypes.models import ContentType
from django.core.validators import RegexValidator
from django.core import serializers
from django.contrib.sessions.models import Session
from django.conf import settings

import subprocess
import itertools
import os

from file_utility import (get_unique_filename, ensure_path_exists,
                          file_not_empty)


class cached_property(object):
    """ Decorator that converts a method with a single
    self argument into a property cached on the instance.

    Taken from http://habrahabr.ru/post/159099/
    In Django >= 1.4 can be found in django.utils.functional
    """
    def __init__(self, func):
        self.func = func

    def __get__(self, instance, type=None):
        result = instance.__dict__[self.func.__name__] = self.func(instance)
        return result


class ObjectTypeController:
    """ Manages object hierarchy and registered types
    """
    _registered_types = {}

    @cached_property
    def categories(self):
        return self._registered_types.keys()

    @cached_property
    def all_object_types(self):
        return tuple(itertools.chain.from_iterable(
            self._registered_types.itervalues()))

    def get_object_types(self, category):
        return self._registered_types.get(category, [])

    def register(self, category, type_):
        if not (category in self._registered_types):
            self._registered_types[category] = []
        self._registered_types[category].append(type_)

object_controller = ObjectTypeController()


def get_current_solution(request):
    """  Returns solution belonging to current user (or anonymous) """
    if request.user.is_authenticated():
        if not hasattr(request.user, 'solution'):
            solution = Solution(user=request.user, session=None)
            solution.save()
        return request.user.solution
    else:
        if not request.session.exists(request.session.session_key):
            request.session.create()
        session = Session.objects.get(pk=request.session.session_key)
        if not hasattr(session, 'solution'):
            solution = Solution(user=None, session=session)
            solution.save()
        return session.solution


class Solution(models.Model):
    """ User's solution containing group of objects
    """
    user = models.OneToOneField(User, blank=True, null=True)
    session = models.OneToOneField(Session, blank=True, null=True)

    def get_objects(self, object_type):
        """ Returns objects belonging to this solution """
        return object_type.objects.filter(solution=self)

    def save_form(self, form):
        # deprecated
        obj = form.save(commit=False)
        obj.save()

    def get_content_filename(self, filename):
        """ Creates filename for file being saved to disk """
        if self.user is not None:
            username = self.user.username
        else:
            username = 'Anonymous'
        path = os.path.join(username, 'data')
        ensure_path_exists(settings.MEDIA_ROOT + path)
        return get_unique_filename(os.path.join(path, filename))

    def export_objects(self):
        """ Exports all the objects to JSON """
        raise NotImplementedError()

    def import_objects(self, data):
        """ Imports all the objects from JSON """
        raise NotImplementedError()


APPROACH_CHOICES = (
    ('pointwise', 'pointwise'),
    ('pairwise', 'pairwise'),
    ('listwise', 'listwise'))

FORMAT_CHOICES = (
    ('Yandex', 'Yandex'),
    ('ARFF', 'ARFF'),
    ('SVMLIGHT', 'SVMLight'))

MAX_FILE_PATH_LENGTH = 260
MAX_STRING_LENGTH = 30


class Task(models.Model):
    """ Stores information about launched ltr_client, its config and results
    """
    solution = models.ForeignKey(Solution)
    config_filename = models.CharField(max_length=MAX_FILE_PATH_LENGTH,
                                       unique=False)
    report_filename = models.CharField(max_length=MAX_FILE_PATH_LENGTH,
                                       unique=True)
    working_dir = ""

    @staticmethod
    def create(solution, launchable_id):
        """ Creates a Task instance """
        task = Task()
        task.solution = solution
        task.working_dir = get_unique_filename(
            settings.MEDIA_ROOT +
            '/'.join([task.solution.user.username, 'task']))
        ensure_path_exists(task.working_dir)
        task.config_filename = get_unique_filename(
            task.working_dir + '/config.xml')
        file_config = open(task.config_filename, "w")
        file_config.write(task.make_config(launchable_id))

        task.report_filename = get_unique_filename(
            task.working_dir + '/report.html')
        open(task.report_filename, 'w').close()
        task.save()
        return task

    def make_config(self, launchableId):
        """ Creates XML config for ltr_client """
        from django.shortcuts import render_to_response

        db_objects = self.solution.get_objects(LtrObject)
        ltr_objects = tuple(obj.cast() for obj in db_objects)

        db_datas = self.solution.get_objects(Data)
        ltr_datas = tuple(data.cast() for data in db_datas)

        all_objects = self.solution.get_objects(BaseObject)
        launchable = all_objects[launchableId].cast()

        ltr_trains, ltr_crossvalidations = (), ()
        if launchable in self.solution.get_objects(Train):
            ltr_trains = (launchable,)
        elif launchable in self.solution.get_objects(CrossValidation):
            ltr_crossvalidations = (launchable,)

        response = render_to_response(
            'config.xml',
            {'objects': ltr_objects,
             'datas': ltr_datas,
             'trains': ltr_trains,
             'crossvalidations': ltr_crossvalidations,
             'root_directory': self.working_dir})
        response['Content-Disposition'] = 'attachment; filename=config.xml'
        return response.content

    def run(self):
        """ Runs ltr_client """
        subprocess.Popen([settings.LTR_CLIENT_PATH, '-f',
                          self.config_filename])

    def is_complete(self):
        """ Checks if ltr_client's launch has successfully finished """
        return file_not_empty(self.report_filename)


class InheritanceCastModel(models.Model):
    """ An abstract base class that provides a "real_type" FK to ContentType.

    For use in trees of inherited models, to be able to downcast
    parent instances to their child types.

    Taken from http://stackoverflow.com/questions/929029/how-do-i-access\
-the-child-classes-of-an-object-in-django-without-knowing-the-nam
    """
    real_type = models.ForeignKey(ContentType, editable=False)

    def save(self, *args, **kwargs):
        if not self.id:
            self.real_type = self._get_real_type()
        super(InheritanceCastModel, self).save(*args, **kwargs)

    def _get_real_type(self):
        return ContentType.objects.get_for_model(type(self))

    def cast(self):
        """  Downcasts object to its real type """
        return self.real_type.get_object_for_this_type(pk=self.pk)

    class Meta:
        abstract = True


OBJECT_NAME_REGEX = '^[A-Za-z]\w{0,29}$'


class BaseObject(InheritanceCastModel):
    """ Base class in object hierarchy
    """
    solution = models.ForeignKey(Solution)

    name = models.CharField(
        max_length=MAX_STRING_LENGTH,
        validators=[RegexValidator(
            regex=OBJECT_NAME_REGEX,
            code='invalid',
            message='Name must be alphanumeric, max 30 symbols,letter first')])

    def __unicode__(self):
        return self.name

    def get_properties(self):
        """ Returns object properties that should be stored in XML config """
        def is_auxiliary_field(field):
            return field.name in ('id', 'solution', 'real_type')

        def is_pointer_to_base_class(field):
            return field.name.endswith('_ptr')

        # TODO: make sure that ManyToMany fields are processed correctly
        fields = {}
        for field in self._meta.fields:
            if not (is_auxiliary_field(field) or
                    is_pointer_to_base_class(field)):
                value = getattr(self, field.name)
                if isinstance(value, BaseObject):
                    value = value.name
                fields[field.name] = value
        return fields

    class Meta:
        unique_together = (("name", "solution"),)


class LtrObject(BaseObject):
    """ Base class for objects that should be turned into <object> tag in
    XML config
    """
    pass


def object_(cls):
    """ Class decorator used to register categories in object hierarchy """
    cls.get_type = staticmethod(lambda: cls.__name__)
    object_controller.register(cls.get_category(), cls.__name__)
    return cls


def category(cls):
    """ Class decorator used to register objects in object hierarchy """
    cls.get_category = staticmethod(lambda: cls.__name__.lower())
    return cls


@category
class Launch(BaseObject):
    pass


@category
class Data(BaseObject):
    pass


@category
class Measure(LtrObject):
    pass


@category
class Learner(LtrObject):
    pass


@category
class Splitter(LtrObject):
    pass


@object_
class File(Data):
    approach = models.CharField(max_length=MAX_STRING_LENGTH,
                                choices=APPROACH_CHOICES)
    format = models.CharField(max_length=MAX_STRING_LENGTH,
                              choices=FORMAT_CHOICES)
    file = models.FileField(
        upload_to=lambda x, y: x.solution.get_content_filename(y))


@object_
class AbsError(Measure):
    pass


@object_
class AveragePrecision(Measure):
    pass


@object_
class FakeSplitter(Splitter):
    pass


@object_
class BestFeatureLearner(Learner):
    approach = models.CharField(max_length=MAX_STRING_LENGTH,
                                choices=APPROACH_CHOICES)
    measure = models.ForeignKey(Measure)


@object_
class Train(Launch):
    train_data = models.ForeignKey(Data, related_name='+')
    learner = models.ForeignKey(Learner)
    predict = models.ManyToManyField(Data)
    generate_cpp = models.BooleanField()


@object_
class CrossValidation(Launch):
    data = models.ForeignKey(Data)
    learners = models.ManyToManyField(Learner)
    measures = models.ManyToManyField(Measure)
    splitter = models.ForeignKey(Splitter)


class GPLearner(Learner):
    population_size = models.PositiveIntegerField()
    number_of_generations = models.PositiveIntegerField()
    min_init_depth = models.PositiveIntegerField()
    max_init_depth = models.PositiveIntegerField()
    init_grow_probability = models.DecimalField(max_digits=4,
                                                decimal_places=3)
    seed = models.PositiveIntegerField()
