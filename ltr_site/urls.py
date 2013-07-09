# Copyright 2013 Yandex
"""
This file sets accordance between available URLs and views called for them.
"""

from django.conf.urls.defaults import patterns, include, url
from django.views import static
from django.conf import settings
from django.contrib import admin
admin.autodiscover()

from config_maker import views


urlpatterns = patterns(
    '',
    url(r'^$', views.view_home),
    url(r'^view/(\w+)$', views.view_parameters),
    url(r'^launch/(\w+)$', views.view_launch),   # deprecated
    url(r'^create$', views.view_create),
    url(r'^delete/(\w+)$', views.view_delete),
    url(r'^get_object_types$', views.view_get_object_types),
    url(r'^get_object_parameters$', views.view_get_object_parameters),
    url(r'^login$', views.view_login),
    url(r'^logout$', views.view_logout),
    url(r'^users/([^/]+)$', views.view_user),
    url(r'^users/([^/]+)/(.+)$', views.view_file),
    url(r'^profile/delete_all_tasks$', views.view_delete_all_tasks),
    url(r'^profile$', views.view_profile),
    url(r'^export$', views.view_export),
    url(r'^import$', views.view_import),
    url(r'^delete_all$', views.view_delete_all),
    url(r'^log/(\d+)$', views.view_logfile),

    # Uncomment the admin/doc line below to enable admin documentation:
    # url(r'^admin/doc/', include('django.contrib.admindocs.urls')),

    # Uncomment the next line to enable the admin:
    url(r'^admin/', include(admin.site.urls)),
)


if settings.DEBUG:
    urlpatterns += patterns(
        '',
        url(r'^static/(?P<path>.*)$', static.serve,
            {'document_root': settings.STATIC_ROOT}),
    )
