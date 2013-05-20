from creating_deleting_test import *
from file_utility_test import *
from login_test import *
from object_access_test import *
from launch_test import *


"""This is a list of doctests in project. Add all your doctests in this list.

"""
from ..file_utility import get_unique_name

__test__ = {
    'file_utility.get_unique_name': get_unique_name
}
