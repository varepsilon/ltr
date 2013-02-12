# Copyright 2011 Yandex

from django.db import models
from django.contrib.auth.models import User


class UserProfile(models.Model):
    user = models.ForeignKey(User, unique=True)
    db_objects = models.TextField(default="[]")  # JSON-serialized version of list


def create_profile(sender, **kwargs):
    if kwargs['created']:
        UserProfile.objects.create(user=kwargs['instance'])

models.signals.post_save.connect(create_profile, sender=User)
