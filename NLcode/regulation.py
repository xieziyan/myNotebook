#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Dec 26 11:26:13 2019

@author: ziyan
"""
import numpy as np


def check(a,b):
    re = abs(a-b)%17 
    if re<=1:
        return 1
    return 0

# rule 1
def oneDirection(data,th = 0.87):
    """
    0:neg
    1:pos
    
    """
    threshold = th
    data = data[:,:18]
    max_percent = np.max(data)
#    print("max percent:",max_percent)
    if max_percent>threshold:
        return 0
    else:
        return 1

# rule 2
def directionNum(data,th=4):
    """
    0:neg
    1:pos
    """
    threshold = th
    subsrcipt = np.argsort(data[:,:18])[:,-3:]
    max_num = []
    for item in subsrcipt:
        max_num.append(data[0][item])
        
    max_num = np.stack(max_num)
    
    mean = np.mean(max_num)
    evaluation_index = (abs(max_num-mean)/mean).sum()
    E = evaluation_index/len(max_num)
    if E<threshold:
        return 1
    else:
        return 0 
    
# rule 3
def linesLength(data):
    """
    0:neg
    1:pos
    """
    min_threshold = 28
    max_threshold = 1035
    length = data[0,18]
    if length>min_threshold and length<max_threshold:
        return 1
    else:
        return 0
    
# rule 4
def straightAngle(data,th=0.72):
    """
    0:neg
    1:pos
    """
#    th = 0.41
    subsrcipt = np.argsort(data[:,:18])[:,-3:]
    sub = subsrcipt.reshape(-1).tolist()
    a,b,c = sub[0],sub[1],sub[2]
    angleA,angleB = 0,0
    if check(a,b):
        angleA,angleB = b,c
    if check(a,c):
        angleA,angleB = b,c
    if check(b,c):
        angleA,angleB = a,c  
    else:
        angleA,angleB = b,c    
#    print(angleA,angleB)
    valueCOS = np.cos(np.abs(angleA*10-angleB*10)/180*np.pi)
    va = np.abs(valueCOS)
    if va>=th:
        return 0
    else :
        return 1

def classfication(data):
    A = oneDirection(data)
    B = directionNum(data)
    C = linesLength(data)
    D = straightAngle(data)
    result = A+B+C+D
#    print("{}-{}-{}-{}".format(A,B,C,D))
    if result>=3:
        return 1
    else:
        return 0
    
    
    
    