# Copyright 2011 Yandex

from django.http import Http404, HttpResponseRedirect
from django.shortcuts import render_to_response
from django.template import RequestContext
from django.views.decorators.csrf import csrf_exempt
from django.contrib.auth import authenticate, login, logout
from forms import FormType
from object_hierarchy import getObjectTypes, getAllObjectTypes, getForm, \
                             getCreatedObjectNames, renameObjectUsages,  \
                             deleteObjectUsages, setObjects, getObjects
from datetime import datetime


def toDict(queryDict):
    result = dict(queryDict)
    for key in result.keys():
        result[key] = result[key][0]
    result.pop('csrfmiddlewaretoken', '')
    return result


def viewConfig(request):
    response = render_to_response('config.xml', {'objects': getObjects(request)},
                                  context_instance=RequestContext(request))
    response['Content-Disposition'] = 'attachment; filename=config.xml'
    return response


def viewHome(request):
    return render_to_response('home.html', {'objects': getObjects(request)},
                              context_instance=RequestContext(request))


def viewSettings(request, object_id):
    try:
        object_id = int(object_id)
        obj = getObjects(request)[object_id]
    except (ValueError, IndexError):
        raise Http404()

    if request.method == 'POST':
        formSettings = getForm(obj['type_'])(request.POST,
                                             objects=getObjects(request),
                                             object_name=obj['name_'])
        if formSettings.is_valid():
            obj.update(toDict(formSettings.data))
            renameObjectUsages(getObjects(request), formSettings.objectName, obj['name_'])
            return HttpResponseRedirect('/')
    else:
        formSettings = getForm(obj['type_'])(obj,
                                             objects=getObjects(request),
                                             object_name=obj['name_'])
    formType = FormType(obj, disabled=True)
    return render_to_response('home.html', {'formType': formType,
                                            'formSettings': formSettings,
                                            'objects': getObjects(request),
                                            'objectName': obj['name_'],
                                            'edit': True},
                              context_instance=RequestContext(request))


def viewCreate(request):
    if request.method == 'POST':
        objectType = request.POST['type_']
        formSettings = getForm(objectType)(request.POST,
                                           objects=getObjects(request))
        if formSettings.is_valid():
            objects = getObjects(request)
            objects.append(toDict(formSettings.data))
            setObjects(request, objects)
            return HttpResponseRedirect('/')
        else:
            formType = FormType(request.POST)
            load_types = False
    else:
        defaultType = getAllObjectTypes()[0]
        formType = FormType()
        formSettings = getForm(defaultType)(objects=getObjects(request))
        load_types = True
    return render_to_response('home.html', {'formType': formType,
                                            'formSettings': formSettings,
                                            'objects': getObjects(request),
                                            'load_types': load_types},
                              context_instance=RequestContext(request))


def viewDelete(request, object_name):
    try:
        objectNames = getCreatedObjectNames(getObjects(request))
        id = objectNames.index(object_name)
    except ValueError:
        raise Http404()

    if request.method == 'POST':
        deleteObjectUsages(getObjects(request), object_name)
        objects = getObjects(request)
        del objects[id]
        setObjects(request, objects)
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
                profile = request.user.get_profile()
                profile.birthday = datetime.today()
                profile.save()
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
        types = getObjectTypes(category)
        if types:
            return render_to_response('_object_types.html', {'types': types},
                                      context_instance=RequestContext(request))


@csrf_exempt
def viewGetObjectSettings(request):
    if request.is_ajax() and request.method == 'POST':
        type_ = request.POST.get('type', '')
        form = getForm(type_)(objects=getObjects(request))
        return render_to_response('_object_settings.html', {'form':  form},
                                  context_instance=RequestContext(request))
