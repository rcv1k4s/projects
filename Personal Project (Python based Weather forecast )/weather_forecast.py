# Ramachandra Vikas Chamarthi#vikaschamarthi240@gmail.com#
from pymongo import MongoClient
from pprint import pprint
import requests
import pymongo
import time
import json
import thread
import time
import urllib2
client = MongoClient()
db = client.test
h = 1
t1 = ();
while h==1:
 h = int(input("1:continue|2.Exit::"))           				#Flow control
 if h==2:
  break
 print("Weather Forecast:")
 def func5day(t,d):								# 5 day Forecast
    #print('select options: 1.Charlotte 2.Newyork 3.London 4.Delhi')
    #x = int(input("Please enter selection:"))
    x = d									# city Selection
    if x==1:
     k = 'http://api.openweathermap.org/data/2.5/forecast?q=Charlotte&APPID=f32b0590d7a69e134a82e2cc73047459'
    elif x==2:
     k = 'http://api.openweathermap.org/data/2.5/forecast?q=Newyork&APPID=f32b0590d7a69e134a82e2cc73047459'
    elif x==3:
     k = 'http://api.openweathermap.org/data/2.5/forecast?q=London&APPID=f32b0590d7a69e134a82e2cc73047459'
    elif x==4:
     k = 'http://api.openweathermap.org/data/2.5/forecast?q=Delhi&APPID=f32b0590d7a69e134a82e2cc73047459'
    else:
     print('invalid selection')
    v = requests.get(k)								#Retrieving data in JSON format 
    pprint(v.json())
    db.wd5day.insert_one(v.json());						# Inserting into Database
    data = json.loads(v.text)							# Converting into dict class for easier sorting
    #print(data)
    q=0
    while (q < 5):
     q=q+1
     k = data["list"][q]["weather"][0]["description"]				# Parsing Data for Weather description
     print("Day %d Weather: %s "%((q),str(k)))
    print("Hit 1 to continue")
    return
 

 def func16day(t,d):
    
    x = d									# city selection
    if x==1:
     m = 'http://api.openweathermap.org/data/2.5/forecast/daily?q=Charlotte&APPID=f32b0590d7a69e134a82e2cc73047459&cnt=16'
    elif x==2:
     m = 'http://api.openweathermap.org/data/2.5/forecast/daily?q=Newyork&APPID=f32b0590d7a69e134a82e2cc73047459&cnt=16'
    elif x==3:
     m = 'http://api.openweathermap.org/data/2.5/forecast/daily?q=London&APPID=f32b0590d7a69e134a82e2cc73047459&cnt=16'
    elif x==4:
     m = 'http://api.openweathermap.org/data/2.5/forecast/daily?q=Delhi&APPID=f32b0590d7a69e134a82e2cc73047459&cnt=16'
    else:
     print('invalid selection')
    g = requests.get(m)								#Retrieving data in JSON format
    
    db.wd16day.insert(g.json());						#Inserting into Database
    data = json.loads(g.text)							# Converting into dict class for easier sorting
    
    n=1
    q=0
    while (q < 15):
     q=q+1
     k = data["list"][q]["weather"][0]["description"]				# Parsing Data for Weather description	
     print("Day %d Weather: %s "%((q),str(k)))
    print("Hit 1 to continue")
    return
 
 z = int(input('city no:'))							# City user Input
 t=('0',)
 a = z
 print('Enter 1.5 day data,2.16 Day data')
 y = int(input("Please enter selection:"))
 if y==1:
  thread.start_new_thread (func5day,(t,a))
 elif y==2:
  thread.start_new_thread (func16day,(t,a))
 else:
  print('invalid selection')

 f= int(input("Delete Data :1"))						#Clearing Databases
 if f==1:
  db.wd16day.delete_many({})
  db.wd5day.delete_many({})
 
 
