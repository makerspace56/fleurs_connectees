import smbus
import time

bus = smbus.SMBus(1)
adresse = 0x13

print("envoi 1 a l adresse")
bus.write_byte(adresse,1)

time.sleep(1)
reponsse = bus.read_byte(adresse)
print("la reponse2 :", reponsse)
