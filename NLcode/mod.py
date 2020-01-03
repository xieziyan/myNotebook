#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Dec 25 14:57:08 2019

@author: ziyan
"""
import cv2
import os
import copy
import numpy as np
import math
import matplotlib.pyplot as plt
import pandas as pd
from tqdm import tqdm

root_path = './dm/'
subfile = [] 
for item in os.listdir(root_path):
    path = os.path.join(root_path,item)
    subfile.append(path)

pos1_path = []
for item in os.listdir(subfile[0]):
    path = os.path.join(subfile[0],item)
    pos1_path.append(path)
    
testpos1_path = []
for item in os.listdir(subfile[1]):
    path = os.path.join(subfile[1],item)
    testpos1_path.append(path)
    
neg1_path = []
for item in os.listdir(subfile[2]):
    path = os.path.join(subfile[2],item)
    neg1_path.append(path)
    
testneg1_path = []
for item in os.listdir(subfile[3]):
    path = os.path.join(subfile[3],item)
    testneg1_path.append(path)
    
def binary(img):
    if len(img.shape)!=2:
        img = cv2.cvtColor(img,cv2.COLOR_RGB2GRAY)
    img = cv2.GaussianBlur(img, (5, 5), 0)
#    img, th2 = cv2.threshol,(img, 127, 255, cv2.THRESH_BINARY + cv2.THRESH_OTSU)
    img = cv2.adaptiveThreshold(img, 255,cv2.ADAPTIVE_THRESH_MEAN_C, cv2.THRESH_BINARY, 11, 2)
    img = cv2.fastNlMeansDenoising(img,None,10,7,11)
    return img
    
def stat(angle,weight): 
    a = list(range(-10,11))
    dic = dict(zip(a,np.zeros(21)))
    for i in range(len(angle)):
#        print(angle[i])
        kk = angle[i]//10
        if kk>0:
            kk+=1
        if kk<0:
            kk-=1
        if weight[i]-2>0:
            w = weight[i]-2
        else:
            w = 0
        dic[kk]+=w
    return dic

def houghtran(src):
    angle = []
    weight = []
    draw = copy.deepcopy(src)
    bina = binary(src)
    img = cv2.Canny(bina,20,300,3)
    lines = cv2.HoughLinesP(img,1.0,np.pi/180,33,20,2)
    if isinstance(lines,np.ndarray):
        for x1, y1, x2, y2 in lines.reshape(-1,4):
            cv2.line(draw, (x1, y1), (x2, y2), (125, 0, 255), 1)
            agg = (math.atan2(y2-y1,x2-x1))/math.pi*180
            w =math.sqrt((x1-x2)**2+(y1-y2)**2)
            weight.append(w)
            angle.append(int(agg))
#    
#    cv2.imshow('src',draw)
#    cv2.waitKey(0)
        
    return angle,weight

def get(img):
    angle,w = houghtran(img)

    d = stat(angle,w)
    sumation = sum(list(d.values()))
    dic = {} 
    for item in d:
        dic[item] = d[item]/sumation
    dic['length'] = sumation
    return dic

def write_dic(path,names):
    item = path[0]
    img = cv2.imread(item)
    dic = get(img)
    dic['name'] = item.split('/')[-1]
    data = pd.DataFrame(dic,index=[0])
    for i in tqdm(range(1,len(path))):
        item = path[i]
        img = cv2.imread(item)
        dic = get(img)
        dic['name'] = item.split('/')[-1]
        item = pd.DataFrame(dic,index=[0])
        data = pd.concat([data,item],ignore_index=True)	
    name = data['name']
    length = data['length']
    data.drop(labels=['name','length'], axis=1,inplace = True)
    data.insert(0, 'name',name)
    data.insert(0, 'length',length)
    data.to_csv('./{}.csv'.format(names),float_format='%.2f')	
    
    return data	

if __name__=='__main__':
    write_dic(pos1_path,'pos1')
    write_dic(testpos1_path,'testpos1')
    write_dic(neg1_path,'neg1')
    write_dic(testneg1_path,'testneg1')
    
    
    
    
    
    
    
    
    
    
    
    
    