# Copyright 2011 Yandex

from django.conf.urls.defaults import patterns, include, url
from django.views.generic.simple import direct_to_template
from django.views import static
from ConfigMaker import views

# Uncomment the next two lines to enable the admin:
from django.contrib import admin
admin.autodiscover()

urlpatterns = patterns('',
    url(r'^about$', direct_to_template, {'template': 'about.html'}),
    url(r'^$', views.viewHome),
    url(r'^view/(\d+)$', views.viewSettings),
    url(r'^create$', views.viewCreate),
    url(r'^delete/(\w+)$', views.viewDelete),
    url(r'^get_object_types$', views.viewGetObjectTypes),
    url(r'^get_object_settings$', views.viewGetObjectSettings),
    url(r'^login$', views.viewLogin),
    url(r'^logout$', views.viewLogout),
    url(r'^config$', views.viewConfig),

    # Uncomment the admin/doc line below to enable admin documentation:
    # url(r'^admin/doc/', include('django.contrib.admindocs.urls')),

    # Uncomment the next line to enable the admin:
    url(r'^admin/', include(admin.site.urls)),
)


from settings import DEBUG, MEDIA_ROOT
if DEBUG:
    urlpatterns += patterns('',
        url(r'^media/(?P<path>.*)$', static.serve, {'document_root': '.' + MEDIA_ROOT}),
    )
