# Copyright 2011 Yandex

import forms
import json


REGISTERED_TYPES = {'learner': {'BestFeatureLearner': 'BestFeatureLearnerForm'},
                    'measure': {'AbsError': 'AbsErrorForm',
                                'Accuracy': 'AccuracyForm'},
}


def getObjects(request):
    if 'local_objects' not in request.session:
        if request.user.is_authenticated():
            jsonDec = json.decoder.JSONDecoder()
            return jsonDec.decode(request.user.get_profile().db_objects)
    return request.session.get('local_objects', [])


def setObjects(request, objects):
    if request.user.is_authenticated():
        profile = request.user.get_profile()
        profile.db_objects = json.dumps(objects)
        profile.save()
    request.session['local_objects'] = objects


def getCategories():
    return REGISTERED_TYPES.keys()


def getObjectTypes(category):
    if category in getCategories():
        return REGISTERED_TYPES[category].keys()
    return ()


def getAllObjectTypes():
    result = []
    for category in getCategories():
        result += REGISTERED_TYPES[category].keys()
    return result


def getForm(object_type):
    for category in getCategories():
        if object_type in getObjectTypes(category):
            formName = REGISTERED_TYPES[category][object_type]
            formClass = getattr(forms, formName)
            return formClass
    return None


def getCreatedObjectNames(objects, category=None):
    if category:
        return ('-- none --',) +  \
            tuple(obj['name_'] for obj in objects if obj['category_'] == category)
    else:
        return tuple(obj['name_'] for obj in objects)


def renameObjectUsages(objects, old_name, new_name):
    for obj in objects:
        for field_name in obj.keys():
            if obj[field_name] == old_name:
                obj[field_name] = new_name


def deleteObjectUsages(objects, object_name):
    for obj in objects:
        for field_name in obj.keys():
            if obj[field_name] == object_name:
                del obj[field_name]
