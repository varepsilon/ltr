# Copyright 2011 Yandex
from django.db import models
from django.contrib.auth.models import User
from django.contrib.contenttypes.models import ContentType
from django.core.validators import RegexValidator
from django.contrib.sessions.models import Session
import cgi

#This class is obsolete and will be removed in Eugene's commit
class ObjectTypeController:
    REGISTERED_TYPES = {}
    
    def getCategories(self):
        return self.REGISTERED_TYPES.keys()


    def getObjectTypes(self, category):
        return self.REGISTERED_TYPES.get(category, [])

    def getAllObjectTypes(self):
        result = []
        for category in self.getCategories():
            result += self.REGISTERED_TYPES[category]
        return result

    def register(self, category, class_name):
        if category not in self.REGISTERED_TYPES:
            self.REGISTERED_TYPES[category] = []
        self.REGISTERED_TYPES[category].append(class_name)


object_controller = ObjectTypeController()
#################

def getSession(key):
    if key == None:
        return Session()
    return Session.objects.get(pk=key)

def currentFolder(request):
    if request.user.is_authenticated():
        if not hasattr(request.user, 'folder'):
            folder = Folder(user=request.user, session=None)
            folder.save()
        return request.user.folder
    else:
        if not request.session.exists(request.session.session_key):
            request.session.create()
        session = getSession(request.session.session_key)
        if not hasattr(session, 'folder'):
            folder = Folder(user=None, session=session)
            folder.save()
        return session.folder



class Folder(models.Model):
    user = models.OneToOneField(User, blank=True, null=True)
    session = models.OneToOneField(Session, blank=True, null=True)

    def getObjects(self, object_type):
        return object_type.objects.filter(folder=self)

    def saveForm(self, form):
        obj = form.save(commit=False)
        obj.folder = self
        obj.save()

APPROACH_CHOICES = (
    ('pointwise', 'pointwise'),
    ('pairwise', 'pairwise'),
    ('listwise', 'listwise')
)

FORMMAT_CHOICES = (
    ('Yandex', 'Yandex'),
    ('ARFF', 'ARFF'),
    ('SVMLIGHT', 'SVMLight')
)

class InheritanceCastModel(models.Model):
    """
    An abstract base class that provides a ``real_type`` FK to ContentType.

    For use in trees of inherited models, to be able to downcast
    parent instances to their child types.

    """
    real_type = models.ForeignKey(ContentType, editable=False)

    def save(self, *args, **kwargs):
        if not self.id:
            self.real_type = self._get_real_type()
        super(InheritanceCastModel, self).save(*args, **kwargs)

    def _get_real_type(self):
        return ContentType.objects.get_for_model(type(self))

    def cast(self):
        return self.real_type.get_object_for_this_type(pk=self.pk)

    class Meta:
        abstract = True


nameRegex = '^[A-Za-z]\w{0,29}$'

class BaseObject(InheritanceCastModel):
    folder = models.ForeignKey(Folder)

    name = models.CharField(max_length=30, unique=True,
                            validators=[RegexValidator(regex=nameRegex, code='invalid', message='only alphanumeric values are allowed')])
    def __unicode__(self):
        return self.name

class LtrObject(BaseObject):
    def xml(self):
        if hasattr(self, 'approach'):
            approach_text = "approach=\"{0}\"".format(self.approach)
        else:
            approach_text = ""

        attributes = self.getAttributes()

        xml = "<object name=\"{0}\" type=\"{1}\" {2}>\n{3}</object>\n"
        return xml.format(self.name, self.__class__.__name__, approach_text, attributes)

    def getAttributes(self):
        return ''

    def wrapAttribute(self, name, attr):
        return "<{0}>{1}</{0}>\n".format(name, cgi.escape(attr))


class Launch(BaseObject):
    def getCategory(self):
        return 'launch'

class Data(BaseObject):
    object_controller.register('data', 'Data')

    approach = models.CharField(max_length=20, choices=APPROACH_CHOICES)
    format = models.CharField(max_length=20, choices=FORMMAT_CHOICES)
    path = models.CharField(max_length=100)

    def getCategory(self):
        return 'data'

class Measure(LtrObject):
    def getCategory(self):
        return 'measure'

class Learner(LtrObject):
    def getCategory(self):
        return 'learner'

class Splitter(LtrObject):
    def getCategory(self):
        return 'splitter'

class AbsError(Measure):
    object_controller.register('measure', 'AbsError')

class FakeSplitter(Splitter):
    object_controller.register('splitter', 'FakeSplitter')

class BestFeatureLearner(Learner):
    object_controller.register('learner', 'BestFeatureLearner')

    approach = models.CharField(max_length=20, choices=APPROACH_CHOICES)
    measure = models.ForeignKey(Measure)

    def getAttributes(self):
        xml = self.wrapAttribute('measure', self.measure.name)
        return xml

class Train(Launch):
    object_controller.register('launch', 'Train')

    trainData = models.ForeignKey(Data, related_name='+')
    learner = models.ForeignKey(Learner)
    predict = models.ManyToManyField(Data)
    generate_cpp = models.BooleanField()

class CrossValidation(Launch):
    object_controller.register('launch', 'CrossValidation')

    data = models.ForeignKey(Data)
    learners = models.ManyToManyField(Learner)
    measures = models.ManyToManyField(Measure)
    splitter = models.ForeignKey(Splitter)

class GPLearner(Learner):
    population_size = models.PositiveIntegerField()
    number_of_generations = models.PositiveIntegerField()
    min_init_depth = models.PositiveIntegerField()
    max_init_depth = models.PositiveIntegerField()
    init_grow_probability = models.DecimalField(max_digits=3, decimal_places=3)
    seed = models.PositiveIntegerField()

    object_controller.register('learner', 'GPLearner')