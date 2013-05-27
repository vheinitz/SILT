# -*- coding: utf-8 -*-
#SILT - Simple Image Labeling Tool
#Valentin Heinitz, http://heinitz-it.de, 2013
#Powered by web2py, derived from welcome-app

import datetime

def index():
	per_page = 10
	total_images = db(db.image.id > 0).count()
	total_pages = total_images / per_page	
	if total_images > per_page and total_pages == 1 and total_pages * per_page != total_images:
		total_pages = 2
	page = int(request.args(0)) if request.args(0) else 1
	limit = int(page-1) * per_page
	#images = db(db.image).select(orderby=~db.image.id, limitby=(limit, per_page*page))
	query = request.post_vars['query']
	if not query:
		images = db(db.image).select(orderby=~db.image.id, limitby=(limit, per_page*page))
	else:
		images = db((db.imageLabel.labelValue.like('%'+query+'%')) & (db.imageLabel.imageId == db.image.id)).select(orderby=~db.image.id, limitby=(limit, per_page*page))
	print images
	return locals()

@auth.requires_login()
def label():
	import json
	id = request.args(0)
	img = db.image(id)
	
	labels = db((db.imageLabel.labelId == db.labelType.id) ).select( 
		db.imageLabel.labelValue, db.labelType.name, db.auth_user.first_name, db.auth_user.last_name, db.imageLabel.labelTimeStamp, 
		left=db.auth_user.on(db.imageLabel.userId==db.auth_user.id)
		)

	labelTypes = db(db.labelType).select(db.labelType.id, db.labelType.name)
	return locals()		

def getLabelFormContet():
	labelId = request.post_vars['labelId']
	label = db(db.labelType.id == labelId).select(db.labelType.valueType)
	return label[0].valueType

@auth.requires_login()	
def addLabel():
	labelTypeId = request.post_vars['labelId']
	imageId = request.post_vars['imageId']
	labelValue = request.post_vars['value']

	#inserting label value in imageLabel
	db.imageLabel.insert(imageId=imageId, labelId=labelTypeId, labelValue=labelValue, userId=auth.user.id, labelTimeStamp=datetime.datetime.now() );
	redirect(URL('default', 'label/'+imageId))
	
@auth.requires_login()	
def manage_labels():
	#return dict(message=T('Labels'))
    query = ((db.labelType.id>0))
    form = SQLFORM.grid( query=query, deletable=True, editable=True, maxtextlength=64, paginate=10, user_signature=False)
    return dict(form=form)

def add_labels():
   form = SQLFORM(db.labelType)
   if form.process().accepted:
	   response.flash = 'form accepted'
   elif form.errors:
	   response.flash = 'form has errors'
   else:
	   response.flash = 'please fill out the form'
   return dict(form=form)	
	
def add_images():
   form = SQLFORM(db.image)
   if form.process().accepted:
	   response.flash = 'form accepted'
   elif form.errors:
	   response.flash = 'form has errors'
   else:
	   response.flash = 'please fill out the form'
   return dict(form=form)	
   
def user():
	"""
	exposes:
	http://..../[app]/default/user/login
	http://..../[app]/default/user/logout
	http://..../[app]/default/user/register
	http://..../[app]/default/user/profile
	http://..../[app]/default/user/retrieve_password
	http://..../[app]/default/user/change_password
	use @auth.requires_login()
		@auth.requires_membership('group name')
		@auth.requires_permission('read','table name',record_id)
	to decorate functions that need access control
	"""
	return dict(form=auth())

#users management
def users():
    query = ((db.auth_user.id>0))
    form = SQLFORM.grid( query=query, deletable=True, editable=True, maxtextlength=64, paginate=10, user_signature=False)
    return dict(form=form)

def download():
	"""
	allows downloading of uploaded files
	http://..../[app]/default/download/[filename]
	"""
	return response.download(request, db)


def call():
	"""
	exposes services. for example:
	http://..../[app]/default/call/jsonrpc
	decorate with @services.jsonrpc the functions to expose
	supports xml, json, xmlrpc, jsonrpc, amfrpc, rss, csv
	"""
	return service()


@auth.requires_signature()
def data():
	"""
	http://..../[app]/default/data/tables
	http://..../[app]/default/data/create/[table]
	http://..../[app]/default/data/read/[table]/[id]
	http://..../[app]/default/data/update/[table]/[id]
	http://..../[app]/default/data/delete/[table]/[id]
	http://..../[app]/default/data/select/[table]
	http://..../[app]/default/data/search/[table]
	but URLs must be signed, i.e. linked with
	  A('table',_href=URL('data/tables',user_signature=True))
	or with the signed load operator
	  LOAD('default','data.load',args='tables',ajax=True,user_signature=True)
	"""
	return dict(form=crud())
