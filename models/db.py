# -*- coding: utf-8 -*-
#SILT - Simple Image Labling Tool
#Valentin Heinitz, http://heinitz-it.de, 2013
#Powered by web2py, derived from welcome-app


db = DAL('sqlite://silt.db',pool_size=1,check_reserved=['all'])

response.generic_patterns = ['*'] if request.is_local else []


from gluon.tools import Auth, Crud, Service, PluginManager, prettydate
auth = Auth(db)
crud, service, plugins = Crud(db), Service(), PluginManager()

## create all tables needed by auth if not custom tables
auth.define_tables(username=False, signature=False)

## configure email
#mail = auth.settings.mailer
#mail.settings.server = 'smtp.gmail.com:587'
#mail.settings.sender = ''
#mail.settings.login = ''

## configure auth policy
auth.settings.registration_requires_verification = False
auth.settings.registration_requires_approval = False
auth.settings.reset_password_requires_verification = True


# DB-schema
db.define_table('lableType',
	Field('name','string'),
	Field('readUserGroup','integer'),
	Field('setUserGroup','integer'),
    Field('valueType','string')
)
db.define_table('imageLable',
	Field('imageId','string'),
	Field('lableId','integer'),
	Field('userId','integer'), 
	Field('lableValue','text'),
	Field('replacedById','integer'),
	Field('lableComment','text')
)
db.define_table('image',
	Field('imageId','string')
)
