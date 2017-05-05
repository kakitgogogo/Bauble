from flask import Blueprint
import sys
from imp import reload

reload(sys)

main = Blueprint('main', __name__, static_folder='static', template_folder='templates')

from . import views