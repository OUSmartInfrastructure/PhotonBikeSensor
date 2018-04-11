#!/usr/bin/python

import socket, time, os, sys, copy

ip = "132.235.160.114"
port = 51234

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((ip, port))
s.listen(2)

print "Listening on {}:{}".format(ip,port)
go = 1

while go:
        client, address = s.accept()
        file = str(time.time())+".jpg"

        print "Connection made from {}:{}".format(address[0],address[1])
        data = client.recv(1024)

        while go:
                oldData = data
                data = data + client.recv(1024)
                if oldData == data:
                        break
        #Get into long hex string
        checkData = copy.copy(data)
        checkData = str(data).encode('hex')

#       print ("recieved data is: {}".format(data))
        if ('ffd8' and 'ffd9') in checkData:
                with open(file, "w+") as f:
                        f.write(data)

                os.rename(file,str("testImages/"+str(file)))
                print("Image written")
        else:
                print("Valid image not detected")
