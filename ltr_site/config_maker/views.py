# Copyright 2011 Yandex

from django.http import (HttpResponse, Http404, HttpResponseRedirect,
                         HttpResponseForbidden)
from django.shortcuts import render_to_response
from django.template import RequestContext
from django.template.loader import render_to_string
from django.views.decorators.csrf import csrf_exempt
from django.views.decorators.http import (require_POST, require_GET,
                                          require_http_methods)
from django.contrib.auth import authenticate, login, logout
from django.core.exceptions import ObjectDoesNotExist
from django.forms.models import modelform_factory
from django.db import IntegrityError
from django.conf import settings

import models
from models import get_current_solution, BaseObject, Task, object_controller
from forms import FormType
from templatetags.object_handlers import path_to_url
from file_utility import get_unique_name


@require_GET
def view_home(request):
    db_objects = get_current_solution(request).get_objects(BaseObject)
    return render_to_response('home.html', {'objects': db_objects},
                              context_instance=RequestContext(request))


@require_http_methods(["GET", "POST"])
def view_parameters(request, object_id):
    try:
        # TODO: call objects by name
        object_id = int(object_id)
        db_objects = get_current_solution(request).get_objects(BaseObject)
        obj = db_objects[object_id]
    except (ValueError, IndexError, ObjectDoesNotExist):
        raise Http404()
    obj = obj.cast()
    form_object_parameters_class = modelform_factory(type(obj),
                                                     exclude=("solution",))
    launchable = (type(obj).get_category() == 'launch')
    if request.method == 'POST':
        form_object_parameters = form_object_parameters_class(request.POST,
                                                              request.FILES,
                                                              instance=obj)
        if form_object_parameters.is_valid():
            try:
                get_current_solution(request).save_form(form_object_parameters)
                if 'launch' in request.REQUEST:
                    return view_launch(request, object_id)
                return HttpResponseRedirect('/')
            except IntegrityError:
                # TODO: handle IntegrityError
                pass
    else:
        form_object_parameters = form_object_parameters_class(instance=obj)
    form_object_type = FormType(mode='edit_mode',
                                initial={'category_': obj.get_category(),
                                         'type_': obj.get_type()})
    db_objects = get_current_solution(request).get_objects(BaseObject)
    return render_to_response(
        'home.html',
        {'form_object_type': form_object_type,
         'form_object_parameters': form_object_parameters,
         'objects': db_objects,
         'object_name': obj.name,
         'mode': 'edit_mode',
         'launchable': launchable},
        context_instance=RequestContext(request))


@require_http_methods(["GET", "POST"])
def view_create(request):
    solution = get_current_solution(request)
    db_objects = solution.get_objects(BaseObject)
    if request.method == 'POST':
        object_type_string = request.POST['type_']
        object_type = getattr(models, object_type_string)
        launchable = (object_type.get_category() == 'launch')
        form_object_type = modelform_factory(object_type,
                                             exclude=("solution",))
        form_object_parameters = form_object_type(request.POST, request.FILES)
        if form_object_parameters.is_valid():
            try:
                get_current_solution(request).save_form(form_object_parameters)
                if 'launch' in request.REQUEST:
                    db_objects = solution.get_objects(BaseObject)
                    object_id = db_objects.count() - 1
                    return view_launch(request, object_id)
                return HttpResponseRedirect('/')
            except IntegrityError:
                # TODO: show validation errors
                form_object_type = FormType(request.POST)
                reload_type_list = False
        else:
            form_object_type = FormType(request.POST)
            reload_type_list = False
    else:
        default_type_string = object_controller.all_object_types[0]
        default_type = getattr(models, default_type_string)
        launchable = (default_type.get_category() == 'launch')
        form_object_type = FormType(mode='create_mode')
        form_object_parameters = modelform_factory(default_type,
                                                   exclude=("solution",))()
        reload_type_list = True
    return render_to_response(
        'home.html',
        {'form_object_type': form_object_type,
         'form_object_parameters': form_object_parameters,
         'objects': db_objects,
         'reload_type_list': reload_type_list,
         'mode': 'create_mode',
         'launchable': launchable},
        context_instance=RequestContext(request))


@require_POST
def view_delete(request, object_name):
    try:
        db_objects = get_current_solution(request).get_objects(BaseObject)
        possible_objects = db_objects.filter(name=object_name)
        object_to_delete = possible_objects[0].cast()
    except IndexError:
        raise Http404()

    object_to_delete.delete()
    return HttpResponseRedirect('/')


@require_POST
def view_login(request):
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


def view_logout(request):
    # TODO: make via POST
    redirect_to = request.REQUEST.get('next', '')
    logout(request)
    return HttpResponseRedirect(redirect_to)


@require_GET
def view_file(request, solution, filename):
    if request.user.is_authenticated():
        username = request.user.username
    else:
        username = settings.ANONYMOUS_USER

    if username == solution:
        from django.core.servers.basehttp import FileWrapper
        import mimetypes
        import os

        file_path = settings.MEDIA_ROOT + '/'.join((solution, filename))
        wrapper = FileWrapper(open(file_path))
        content_type = mimetypes.guess_type(file_path)[0]
        response = HttpResponse(wrapper, content_type=content_type)
        if not file_path.endswith('.html'):
            response['Content-Length'] = os.path.getsize(file_path)
            response['Content-Disposition'] = "attachment;"
        else:
            response = HttpResponse(path_to_url(response.content))
        return response
    else:
        page403 = render_to_string('403.html', {},
                                   context_instance=RequestContext(request))
        return HttpResponseForbidden(page403)


@require_POST
def view_launch(request, object_id):
    try:
        # TODO: call settings by name
        object_id = int(object_id)
        obj = get_current_solution(request).get_objects(BaseObject)[object_id]
    except (ValueError, IndexError, ObjectDoesNotExist):
        raise Http404()
    obj = obj.cast()
    if obj.get_category() != 'launch':
        return HttpResponse('Unable to launch "%s"' % obj.get_category())

    task = Task.create(get_current_solution(request), object_id)
    task.run()
    return HttpResponseRedirect('/profile')


@require_GET
def view_user(request, username):
    # TODO: do we need other users' profiles?
    if request.user.is_authenticated() and username == request.user.username:
        return view_profile(request)
    else:
        page403 = render_to_string('403.html', {},
                                   context_instance=RequestContext(request))
        return HttpResponseForbidden(page403)


@require_GET
def view_profile(request):
    tasks = get_current_solution(request).get_objects(Task)
    return render_to_response('profile.html',
                              {'tasks': tasks},
                              context_instance=RequestContext(request))


@require_http_methods(["GET", "POST"])
def view_delete_all_tasks(request):
    # To be removed
    get_current_solution(request).get_objects(Task).delete()
    return HttpResponseRedirect('/profile')


@require_GET
def view_export(request):
    response = HttpResponse(get_current_solution(request).export_objects())
    response['Content-Disposition'] = 'attachment; filename=export.json'
    return response


@require_http_methods(["GET", "POST"])
def view_import(request):
    if request.method == 'POST':
        data = request.FILES['file'].read()
        get_current_solution(request).import_objects(data)
    else:
        return render_to_response('load_file.html', {},
                                  context_instance=RequestContext(request))
    return HttpResponseRedirect('/')


@require_POST
def view_delete_all(request):
    get_current_solution(request).get_objects(BaseObject).delete()
    return HttpResponseRedirect('/')


@csrf_exempt
@require_POST
def view_get_object_types(request):
    if request.is_ajax():
        category = request.POST.get('category', '')
        types = object_controller.get_object_types(category)
        return render_to_response('_object_types.html', {'types': types},
                                  context_instance=RequestContext(request))


@csrf_exempt
@require_POST
def view_get_object_parameters(request):
    if request.is_ajax():
        type_ = request.POST.get('type', '')
        mode = request.POST.get('mode', 'create_mode')
        object_type = getattr(models, type_)
        launchable = (object_type.get_category() == 'launch')
        form_type = modelform_factory(object_type, exclude=("solution",))
        solution = get_current_solution(request)
        objects = solution.get_objects(object_type)
        object_names = list(obj.name for obj in objects)
        name = get_unique_name(type_, object_names)
        form = form_type(initial={'name': name})
        return render_to_response('_object_properties.html',
                                  {'form_object_parameters': form,
                                   'mode': mode,
                                   'launchable': launchable},
                                  context_instance=RequestContext(request))
