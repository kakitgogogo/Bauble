from fabric.api import *
from fabric.colors import *
from fabric.context_managers import *
from fabric.contrib.console import confirm
import time

env.user = 'root'
env.hosts = ['kakit.top']
env.password = 'Fat724!!'

env.deploy_version = 'pytrade-v'+time.strftime("%Y%m%d")
env.deploy_root = '/home/kakit/'
env.deploy_dir = env.deploy_root+env.deploy_version+'/'
env.deploy_link = env.deploy_root+'pytrade'

env.source_dir = '/home/kakit/code/python_code/pytrade/'
env.source_name = 'www'
env.pack_name = env.source_name+'.tar.gz'

env.supervisor_dir = '/etc/supervisor'

@runs_once
def input_version():
	vid = prompt("please input project rollback version ID:")
	if len(vid) != 8:
		abort("Project vesion ID error!")
	return vid

@runs_once
def tar_pack():
	print yellow("Create source package...")
	with lcd(env.source_dir):
		local("tar -czf %s %s" % (env.pack_name, env.source_name))
	print green('Creating source package success!')

@task
def put_pack():
	print yellow("Start put package...")
	with settings(warn_only=True):
		run('mkdir %s' % env.deploy_dir)
	with cd(env.deploy_dir):
		with settings(warn_only=True):
			result = put(env.source_dir+env.pack_name, env.deploy_dir+env.pack_name)
		if result.failed and not confirm("put file failed, Continue[Y/N]?"):
			abort("Aborting file put task!")

	with settings(warn_only=True):
		lmd5 = local("md5sum %s" % env.source_dir+env.pack_name, capture=True).split(' ')[0]
		rmd5 = run("md5sum %s" % env.deploy_dir+env.pack_name).split(' ')[0]
	if lmd5 == rmd5:
		with cd(env.deploy_dir):
			run("tar -zxvf %s" % env.pack_name)
			run('rm -f %s' % env.pack_name)
			run("rm -f %s" % env.deploy_link)
			run('ln -s %s %s' % (env.deploy_dir, env.deploy_link))
			print green('Put & untar package success!')
	else:
		print "ERROR"

	with settings(warn_only=True):
		with cd(env.supervisor_dir):
			run('supervisorctl stop pytrade')
			run('supervisorctl start pytrade')

@task
def rollback():
	print yellow("rolll back project version")
	with cd(env.deploy_root):
		run('ls')
		version = 'pytrade-v'+input_version()
		run("rm -f %s" % env.deploy_link)
		run("ln -s %s %s", env.deploy_root+version, env.deploy_link)
		print green("roll back success!")

@task
def deploy():
	tar_pack()
	put_pack()
