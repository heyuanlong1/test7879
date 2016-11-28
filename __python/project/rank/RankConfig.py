#!/usr/bin/python
# -*- coding:UTF-8 -*-

"""


"""

class RankConfig :
	def __init__(self):
		self.dict = {}

	def __del__(self):
		self.dict.clear()

	def set(self,key,value):
		self.dict[key] = value

	def delKey(self,key):
		if self.dict.has_key(key) == True:
			del self.dict[key]
		else:
			pass
	def get(self,key):
		if self.dict.has_key(key) == True:
			return self.dict[key]
		else:
			return None
	def getDict(self):
		return self.dict

	def getKeys(self):
		return self.dict.keys()

		
