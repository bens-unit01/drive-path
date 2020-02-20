#! python
'''
Created on Apr 7, 2016

@author: user
'''

import json
from pprint import pprint

#j = json.loads('{"one" : "1", "two" : "2", "three" : "3"}')
#print j['two']

def display():
    with open('dp_logs.json') as data_file:    
        data = json.load(data_file)
    i = 1
    j = 1
    for d in data["logDataList"]:
#         print "Label:\t\t", d["mLabel"] 
#         print "Pos. angle:\t", d["mPosAngle"] 
#         print "Heading:\t", d["mHeading"] 
#         print "Range:\t\t", d["mRange"]
#         print "Reading matrix: "
        print  "A{", i, ",", j, ":} = [";
        for matrix in d["mMatrix"]:
            print matrix[0],"\t", matrix[1],"\t", matrix[2], "\t", matrix[3] 
        print "]" 
        print "----------------------------" 
        j = j + 1
        if j == 11:
            i = i + 1
            j = 1
            
#pprint(data)
if __name__ == '__main__':
   display() 

'''
{u'mPosAngle': u' 79', u'mHeading': u' 80', u'mRange': u' 6582', u'mLabel': u'aa', u'mMatrix': [[u'1436', u'1289', u'0', u'0'], [u'1548', u'1548', u'1257', u'1231'], [u'1555', u'1562', u'1423', u'1342'], [u'0', u'0', u'0', u'0'], [u'1574', u'1343', u'1283', u'1475'], [u'1527', u'886', u'748', u'1244'], [u'1215', u'417', u'0', u'1131'], [u'1463', u'0', u'0', u'1025'], [u'893', u'0', u'0', u'634']]}
{u'mPosAngle': u' 79', u'mHeading': u' 80', u'mRange': u' 6582', u'mLabel': u'ab', u'mMatrix': [[u'1436', u'1289', u'0', u'0'], [u'1548', u'1548', u'1257', u'1231'], [u'1555', u'1562', u'1423', u'1342'], [u'0', u'0', u'0', u'0'], [u'1574', u'1343', u'1283', u'1475'], [u'1527', u'886', u'748', u'1244'], [u'1215', u'417', u'0', u'1131'], [u'1463', u'0', u'0', u'1025'], [u'893', u'0', u'0', u'634']]}
{u'mPosAngle': u' 79', u'mHeading': u' 80', u'mRange': u' 6582', u'mLabel': u'ac', u'mMatrix': [[u'1436', u'1289', u'0', u'0'], [u'1548', u'1548', u'1257', u'1231'], [u'1555', u'1562', u'1423', u'1342'], [u'0', u'0', u'0', u'0'], [u'1574', u'1343', u'1283', u'1475'], [u'1527', u'886', u'748', u'1244'], [u'1215', u'417', u'0', u'1131'], [u'1463', u'0', u'0', u'1025'], [u'893', u'0', u'0', u'634']]}
'''
