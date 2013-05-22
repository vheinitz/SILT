# -*- coding: utf-8 -*-
#SILT - Simple Image Labling Tool
#Valentin Heinitz, http://heinitz-it.de, 2013
#Powered by web2py, derived from welcome-app
#TPDP: check if working at all, provide curl examples in comments

def add_image_labels():
    import json
    vars = request.vars
    json_data = vars["data"]
    j = json.loads(json_data)

    #inserting data in images table
    row = db.images.insert(hash_value=j["imageId"])

    #inserting data in labels table
    labels = j["labels"]
    for label_name, label_value in labels.items():
        db.labels.insert(label_name=label_name, label_value=label_value, image=row)  

def get_image_labels():
    import json
    vars = request.vars
    json_data = vars["data"]
    j = json.loads(json_data)
    row = db((db.images.id == db.labels.image) & (db.images.hash_value==j["imageId"])).select()
    return dict(row=row)

