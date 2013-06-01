# -*- coding: utf-8 -*-
#SILT - Simple Image Labling Tool
#Valentin Heinitz, http://heinitz-it.de, 2013
#Powered by web2py, derived from welcome-app
#TPDP: check if working at all, provide curl examples in comments

import datetime

#JSON EXAMPLE: ["789456", "124"]
#CURL EXAMPLE: curl -X POST -d data='["789456", "124"]' http://[app]/api/add_images
def add_images():
    import json
    vars = request.vars
    json_data = vars["data"]
    try:
        j = json.loads(json_data)
    except:
        return '{"status":"error", "info":"could not decode json"}'
    else:        
		for i, imageId in enumerate(j):
			db.image.insert(imageId = imageId)
			
	return '{"status":"ok", "info":"Json decoded"}'


#JSON EXAMPLE: {"name":"a", "readUserGroup":"2", "setUserGroup":"1", "valueType":["positive","negative"], "info":"a"}
#CURl EXAMPLE: curl -X POST -d data='{"name":"a", "readUserGroup":"2", "setUserGroup":"1", "valueType":["positive","negative"], "info":"a"}' http://[app]/api/add_label_types
def add_label_types():
    import json
    vars = request.vars
    json_data = vars["data"]
    try:
        j = json.loads(json_data)
    except:
        return '{"status":"error", "info":"could not decode json"}'
    else:        
		name = j["name"]

		#check to see whether a lableType with this name already exists
		row = db.labelType(name=name)
		if not row:
			valueType = json.dumps(j["valueType"])
			db.labelType.insert(name=j["name"], readUserGroup=int(j["readUserGroup"]), setUserGroup=int(j["setUserGroup"]), valueType=valueType, info=j["info"]);
	return '{"status":"ok", "info":"Json decoded"}'

#JSON EXAMPLE: {"imageId":"123456", "labels": [{"labelName":"result","labelValue":"positive","replacedById":"0","labelComment":""}]}
#CURL EXAMPLE: curl -X POST -d data='{"imageId":"123456", "labels": [{"labelName":"result","labelValue":"positive","replacedById":"0","labelComment":""}]}' http://[app]/api/add_image_labels
def add_image_labels():
    import json
    vars = request.vars
    json_data = vars["data"]
    try:
        j = json.loads(json_data)
    except:
        return '{"status":"error", "info":"could not decode json"}'
    else:        
		img=db.image(imageId = j["imageId"]);
		labels = j["labels"]
		for i, labelData in enumerate(labels):
			#if label doesn't exist - create it
			label = db.labelType(name = labelData["labelName"])
			if not label:
				label = db.labelType.insert(name=labelData["labelName"], readUserGroup=0, setUserGroup=0, valueType="[]", info="")

			#inserting data in imageLabel table
			db.imageLabel.insert(imageId = img["id"], labelId = label["id"], userId = 0, labelValue = labelData["labelValue"], 
				replacedById = int(labelData["replacedById"]), labelTimeStamp = datetime.datetime.now(), labelComment = labelData["labelComment"])
	return '{"status":"ok", "info":"Json decoded"}'
