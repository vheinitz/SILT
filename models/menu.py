# -*- coding: utf-8 -*-
#SILT - Simple Image Labling Tool
#Valentin Heinitz, http://heinitz-it.de, 2013
#Powered by web2py, derived from welcome-app

response.title = T('SLIT')
response.subtitle = T('simple image labeling tool')

response.meta.author = 'Valentin Heinitz <vheinitz@googlemail.com>'
response.meta.description = 'Image Labeling'
response.meta.keywords = 'web2py, python, framework'
response.meta.generator = 'Web2py Web Framework'


#########################################################################
## Menu
#########################################################################
response.menu = [
    (T('Home'), False, URL('default', 'index'), []),
	(T('Lables'), False, URL('default', 'manage_lables'), [
		(T('Add'), False, URL('default', 'add_lables'))
	]),
	(T('Images'), False, URL('default', 'images'), [
		(T('Add'), False, URL('default', 'add_images'))
	])
]
