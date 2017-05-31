import socket
import psutil
from datetime import datetime

def socket_prefix(prefix):
	return dict((getattr(socket, attr), attr) for attr in dir(socket) if attr.startswith(prefix))

socket_families = socket_prefix('AF_')
socket_types = socket_prefix('SOCK_')

def get_rlimits(process):
	return {
		'RLIMIT_AS':		process.rlimit(psutil.RLIMIT_AS),
		'RLIMIT_CORE':		process.rlimit(psutil.RLIMIT_CORE),
		'RLIMIT_CPU':		process.rlimit(psutil.RLIMIT_CPU),
		'RLIMIT_DATA':		process.rlimit(psutil.RLIMIT_DATA),
		'RLIMIT_FSIZE':		process.rlimit(psutil.RLIMIT_FSIZE),
		'RLIMIT_LOCKS':		process.rlimit(psutil.RLIMIT_LOCKS),
		'RLIMIT_MEMLOCK':	process.rlimit(psutil.RLIMIT_MEMLOCK),
        'RLIMIT_MSGQUEUE':	process.rlimit(psutil.RLIMIT_MSGQUEUE),
        'RLIMIT_NICE':		process.rlimit(psutil.RLIMIT_NICE),
        'RLIMIT_NOFILE':	process.rlimit(psutil.RLIMIT_NOFILE),
        'RLIMIT_NPROC':		process.rlimit(psutil.RLIMIT_NPROC),
        'RLIMIT_RSS':		process.rlimit(psutil.RLIMIT_RSS),
        'RLIMIT_RTPRIO':	process.rlimit(psutil.RLIMIT_RTPRIO),
        'RLIMIT_RTTIME':	process.rlimit(psutil.RLIMIT_RTTIME),
        'RLIMIT_SIGPENDING':process.rlimit(psutil.RLIMIT_SIGPENDING),
        'RLIMIT_STACK':		process.rlimit(psutil.RLIMIT_STACK)
	}


def format_time(value, data_format='%Y-%m-%d %H:%M:%S'):
	dt = datetime.fromtimestamp(int(value))
	return dt.strftime(data_format)

B_TO_G = 1073741824.0
B_TO_M = 1048576.0
B_TO_K = 1024.0

def format_size(value):
	tmp = value / B_TO_G
	if tmp < 1.0:
		tmp = value / B_TO_M
		if tmp < 1.0:
			tmp = value / B_TO_K
			return "%s K" % round(tmp, 2)
		return "%s M" % round(tmp, 2)
	return "%s G" % round(tmp, 2)

def socket_family(value):
	return socket_families[value]

def socket_type(value):
	return socket_types[value]

def format_addr_port(value):
	if value:
		result = value[0] + ':' + str(value[1])
	else:
		result = ''

	return result