from orm import Model, StringField, IntegerField, create_pool, destroy_pool
import asyncio
import logging
import aiomysql
import sys

class User(Model):
	__table__ = 'users'

	id = IntegerField(primary_key=True)
	name = StringField()


# 创建实例:
#user = User(id=123, name='Michael')

#print(user['id'])

loop = asyncio.get_event_loop()

@asyncio.coroutine  
def test():  
	yield from create_pool(loop, user='root', password='131413', db='test')
	user = User(id=4, name='Tom', email='slysly759@gmail.com', password='12345')  
	r = yield from User.findAll()  
	print(r)  
	yield from user.save()  
	yield from user.update()
	r = yield from User.find(8)  
	print(r)  
	r = yield from User.findAll()  
	print(1, r)  
	r = yield from User.findAll(name='Tom')  
	print(2, r)    
	yield from user.remove()  
	yield from destroy_pool() 
   
loop.run_until_complete(test())  
loop.close()  
if loop.is_closed():  
	sys.exit(0)  