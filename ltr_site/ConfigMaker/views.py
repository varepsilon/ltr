# Copyright 2011 Yandex

from django.http import Http404, HttpResponseRedirect
from django.shortcuts import render_to_response
from django.template import RequestContext
from django.views.decorators.csrf import csrf_exempt
from django.contrib.auth import authenticate, login, logout
from django.core.exceptions import ObjectDoesNotExist
from forms import FormType
import models
from django.forms.models import modelform_factory
from django.db import IntegrityError
from models import currentFolder, BaseObject

def viewConfig(request):
    db_objects = currentFolder(request).getObjects(models.LtrObject).all()
    ltr_objects = []
    for obj in db_objects:
        ltr_objects.append(obj.cast())
        
    ltr_datas = currentFolder(request).getObjects(models.Data).all()
    ltr_trains = currentFolder(request).getObjects(models.Train).all()
    ltr_cv = currentFolder(request).getObjects(models.CrossValidation).all()

    response = render_to_response('config.xml',
                                  {'objects': ltr_objects,
                                   'datas': ltr_datas,
                                   'trains': ltr_trains,
                                   'crossvalidations': ltr_cv},
                                  context_instance=RequestContext(request))
    response['Content-Disposition'] = 'attachment; filename=config.xml'
    return response


def viewHome(request):
    db_objects = currentFolder(request).getObjects(BaseObject).all()
    return render_to_response('home.html', {'objects': db_objects},
                              context_instance=RequestContext(request))


def viewSettings(request, object_id):
    try:
        # TODO: call settings by name
        object_id = int(object_id)
        obj = currentFolder(request).getObjects(BaseObject).all()[object_id]
    except (ValueError, IndexError, ObjectDoesNotExist):
        raise Http404()
    obj = obj.cast()
    formSettingsClass =  modelform_factory(type(obj), exclude=("folder",))
    if request.method == 'POST':
        formSettings = formSettingsClass(request.POST,
                                         instance=obj)
        if formSettings.is_valid():
            try:# TODO: catch integrity error
                currentFolder(request).saveForm(formSettings)
                return HttpResponseRedirect('/')
            except IntegrityError:
                pass            
    else:
        formSettings = formSettingsClass(instance=obj)
    formType = FormType(disabled=True, initial={'category_':obj.getCategory(), 'type_':type(obj).__name__})
    db_objects = currentFolder(request).getObjects(BaseObject).all()
    return render_to_response('home.html', {'formType': formType,
                                            'formSettings': formSettings,
                                            'objects': db_objects,
                                            'objectName': obj.name,
                                            'edit': True},
                              context_instance=RequestContext(request))


def viewCreate(request):
    db_objects = currentFolder(request).getObjects(BaseObject).all()
    if request.method == 'POST':
        objectTypeString = request.POST['type_']
        objectType = getattr(models, objectTypeString)
        formSettings = modelform_factory(objectType, exclude=("folder",))(request.POST)
        if formSettings.is_valid():
            try:
                currentFolder(request).saveForm(formSettings)
                return HttpResponseRedirect('/')
            except IntegrityError:
                formType = FormType(request.POST)
                load_types = False
        else:
            formType = FormType(request.POST)
            load_types = False
    else:
        defaultTypeString = models.object_controller.getAllObjectTypes()[0]
        defaultType = getattr(models, defaultTypeString)
        formType = FormType()
        formSettings = modelform_factory(defaultType, exclude=("folder",))()
        load_types = True
    return render_to_response('home.html', {'formType': formType,
                                            'formSettings': formSettings,
                                            'objects': db_objects,
                                            'load_types': load_types},
                              context_instance=RequestContext(request))


def viewDelete(request, object_name):
    try:
        possible_objects = currentFolder(request).getObjects(BaseObject).filter(name=object_name)
        object_to_delete = possible_objects[0].cast()
    except IndexError:
        raise Http404()

    if request.method == 'POST':
        object_to_delete.delete()
    return HttpResponseRedirect('/')

def viewLogin(request):
    if request.method == 'POST':
        redirect_to = request.REQUEST.get('next', '')
        username = request.POST.get('username', '')
        password = request.POST.get('password', '')
        user = authenticate(username=username, password=password)
        if user is not None:
            if user.is_active:
                login(request, user)
                return HttpResponseRedirect(redirect_to)
            else:
                # TODO: return a 'disabled account' error message
                return HttpResponseRedirect('/777')
        else:
            # TODO: return an 'invalid login' error message.
            return HttpResponseRedirect(redirect_to + '?login_failed=true')


def viewLogout(request):
    redirect_to = request.REQUEST.get('next', '')
    logout(request)
    return HttpResponseRedirect(redirect_to)


@csrf_exempt
def viewGetObjectTypes(request):
    if request.is_ajax() and request.method == 'POST':
        category = request.POST.get('category', '')
        types = models.object_controller.getObjectTypes(category)
        if types:
            return render_to_response('_object_types.html', {'types': types},
                                      context_instance=RequestContext(request))


@csrf_exempt
def viewGetObjectSettings(request):
    if request.is_ajax() and request.method == 'POST':
        type_ = request.POST.get('type', '')
        formType = modelform_factory(getattr(models, type_), exclude=("folder",))
        form = formType()
        return render_to_response('_object_settings.html', {'form':  form},
                                  context_instance=RequestContext(request))
