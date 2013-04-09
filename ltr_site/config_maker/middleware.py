# Copyright 2013 Yandex
"""
This file contains all the middleware used in LTR Site.
"""

from django.db.models import signals
from django.utils.functional import curry

from models import get_current_solution


class SolutionMiddleware(object):
    """ Adds 'solution' foreign key ref automatically to any model that has
    'solution' field. Foreign key is added when saving model.
    Almost entirely taken from: http://stackoverflow.com/questions/862522/\
django-populate-user-id-when-saving-a-model/12977709#12977709
    """

    def process_request(self, request):
        if request.method not in ('GET', 'HEAD', 'OPTIONS', 'TRACE'):
            solution = get_current_solution(request)
            mark_solution = curry(self.mark_solution, solution)
            signals.pre_save.connect(mark_solution,
                                     dispatch_uid=(self.__class__, request,),
                                     weak=False)

    def process_response(self, request, response):
        signals.pre_save.disconnect(dispatch_uid=(self.__class__, request,))
        return response

    def mark_solution(self, solution, sender, instance, **kwargs):
        if 'solution' in instance._meta.get_all_field_names():
            instance.solution = solution
