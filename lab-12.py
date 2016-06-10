import threading
import time
import os

file_mutex = threading.Lock()

class ConsumerThread(threading.Thread):
	def __init__(self, csname):
		threading.Thread.__init__(self)
		self.csname = csname

	def run(self):
		global num

		print "consumer:%s started" % self.csname

		while True:
			if file_mutex.acquire(1):
				
				# open file for read
				fh = open('text.txt', 'r')
				print "c%d:%s" % (self.csname, fh.read())
				fh.close()

				file_mutex.release()

class ProducerThread(threading.Thread):
	def __init__(self, pcname):
		threading.Thread.__init__(self)
		self.pcname = pcname

	def run(self):
		global num

		print "producer:%d started" % self.pcname
		while True:
			if file_mutex.acquire(1):
			
				# open file for write
				fh = open('text.txt', 'w')
				fh.write('p%d' % self.pcname)
				fh.close()
			
				file_mutex.release()

class StopThread(threading.Thread):
	def __init__(self, secs):
		threading.Thread.__init__(self)
		self.secs = secs

	def run(self):
		while self.secs:
			self.secs -= 1
			time.sleep(1)

		os._exit(0)

if __name__ == '__main__':
	num = int(raw_input('input # of consumber and producer:'))

	for x in range(num):
		cs = ConsumerThread(x)
		pt = ProducerThread(x)
		cs.start()
		pt.start()

	st = StopThread(1)
	st.start()