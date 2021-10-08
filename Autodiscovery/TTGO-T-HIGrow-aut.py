'''
TTGO-HIGrow to MQTT Gateway, release 4.3.1
'''
import json
import os
import yaml
import paho.mqtt.client as mqtt


# Configuration File Locations

SENSORS_FILE = "sensors/sensors.yaml"


# Write data to YAML file
def write_yaml_file(mac_id,  name):
    print("Writing newly-discovered sensor " + mac_id + " to yaml file")
    sensors = dict()
    sensors['mac_id'] = mac_id
    sensors['name'] = name

    with open(SENSORS_FILE, 'a') as yaml_file:
        yaml_file.write(yaml.safe_dump(sensors))

# Read data from YAML file
def read_yaml_file(mac_id,  name):
    print('Checking YAML file for previously-discovered sensor')
    with open(SENSORS_FILE, 'r',  encoding='utf-8') as yaml_file:
        for line in yaml_file:
            line_mac_id_input = line.strip()
            line_mac_id_arr = line_mac_id_input.split(": ")
            line_mac_id = line_mac_id_arr[1]
            if line_mac_id == mac_id:
                mac_id_found = True
                break
        else:
            mac_id_found = False 
    
    yaml_file.close()        
    if mac_id_found == False:
         write_yaml_file(mac_id,  name)
         
    return mac_id_found

# Send discovery topics
def send_discovery_topics(msg):

    print("Writing sensor discovery topics for new sensor")
    d = json.loads(msg.payload)
    device_payload = {
        'identifiers': [f"{d['plant']['Tgrow_HIGrow']}"],
        'manufacturer': "LILYGO, programmed by Per Rose",
        'model':'TTGO T-Higrow',   
        'name':  d['plant']['sensorname'],
        'sw_version':  d['plant']['rel']
    }
    entity_payloads = {
        'Tgrow_HIGrow': {
            'name': f"{d['plant']['sensorname']} Mac-ID",
            'unit_of_meas': "",
            'enabled_by_default': 'false',
        },
        'sensorname': {
            'name': f"{d['plant']['sensorname']}",
            'unit_of_meas': "",
            'enabled_by_default': 'false',
        },
        'date': {
            'name': f"{d['plant']['sensorname']} Date",
            'unit_of_meas': "",             
            'icon': 'mdi:calendar'
        },
        'time': {
            'name': f"{d['plant']['sensorname']} Time",
            'unit_of_meas': "", 
            'icon': 'mdi:clock-outline', 
            'frc_upd':True
        },
        'sleep5Count': {
            'name': f"{d['plant']['sensorname']} Sleep5count",
            'unit_of_meas': "", 
            'icon':'mdi:counter',
            'enabled_by_default': 'false',
        }, 
        'bootCount': {
            'name': f"{d['plant']['sensorname']} Bootcount",
            'unit_of_meas': "", 
            'icon':'mdi:counter',
            'enabled_by_default': 'false',
        }, 
        'lux': {
            'name': f"{d['plant']['sensorname']} Lux",
            'unit_of_meas': "lx", 
            'device_class': 'illuminance',
        }, 
        'temp': {
            'name': f"{d['plant']['sensorname']} Temperature",
            'unit_of_meas': "°C",
            'device_class': 'temperature',
        }, 
        'humid': {
            'name': f"{d['plant']['sensorname']} Humidity",
            'unit_of_meas': "%", 
            'device_class': 'humidity',
        }, 
        'soil': {
            'name': f"{d['plant']['sensorname']} Soil",
            'unit_of_meas': "%", 
            'device_class': 'humidity',
        }, 
        'soilTemp': {
            'name': f"{d['plant']['sensorname']} SoilTemp",
            'unit_of_meas': "°C",
            'device_class': 'temperature',
        },
        'salt': {
            'name': f"{d['plant']['sensorname']} Fertilizer",
            'unit_of_meas': "%", 
            'icon':'mdi:bottle-tonic'
        },
        'saltadvice': {
            'name': f"{d['plant']['sensorname']} Fertilize state",
            'unit_of_meas': "", 
            'icon':'mdi:alpha-i-circle-outline'
        },
        'bat': {
            'name': f"{d['plant']['sensorname']} Battery",
            'unit_of_meas': "%", 
            'device_class': 'battery',
        }, 
        'batcharge': {
            'name': f"{d['plant']['sensorname']} Charging",
            'unit_of_meas': "", 
            'icon':'mdi:battery'
        }, 
        'batchargeDate': {
            'name': f"{d['plant']['sensorname']} batchargeDate", 
            'unit_of_meas': "", 
            'icon':'mdi:calendar'
        }, 
        'daysOnBattery': {
            'name': f"{d['plant']['sensorname']} daysOnBattery", 
            'unit_of_meas': "days", 
            'icon':'mdi:calendar'
        }, 
        'wifissid': {
            'name': f"{d['plant']['sensorname']} WIFI",
            'unit_of_meas': "", 
            'icon':'mdi:wifi',
            'enabled_by_default': 'false',
        }, 
        'pressure': {
            'name': f"{d['plant']['sensorname']} Pressure",
            'unit_of_meas': "Hpa", 
            'device_class': 'pressure',
        }, 
        'plantValveNo': {
            'name': f"{d['plant']['sensorname']} plantValveNo",
            'unit_of_meas': "", 
            'enabled_by_default': 'false',
        }, 
        'rel': {
            'name': f"{d['plant']['sensorname']} Release",
            'unit_of_meas': "", 
            'icon':'mdi:counter',
            'enabled_by_default': 'false',
        }, 
    }
    
    for entity, entity_payload in entity_payloads.items():
        entity_payload['val_tpl'] = f"{{{{ value_json.plant.{entity} }}}}"
        entity_payload['uniq_id'] = f"TTGO_{d['plant']['Tgrow_HIGrow']}_{entity}"
        entity_payload['stat_t'] =  f"{'Tgrow_HIGrow'}/{d['plant']['Tgrow_HIGrow']}"
        entity_payload['dev'] = device_payload
        sensor_type = ("sensor")
        entity_topic = f"{'homeassistant'}/{sensor_type}/{'Tgrow_HIGrow_'}{d['plant']['Tgrow_HIGrow']}/{entity}/config"
 
        print(json.dumps(entity_payload))
 
        client.publish(
            entity_topic,
            payload=json.dumps(entity_payload),
            qos=1, 
            retain=True
        )

def on_connect(client, userdata, flags, rc):
    print("Connected to MQTT server with result code "+str(rc))
    client.subscribe("Tgrow_HIGrow/#")

def on_message(client, userdata, msg):
    print("Received message on topic " + msg.topic)
    print("Body: " +str(msg.payload))
    d = json.loads(msg.payload)
    mac_id=d["plant"]["Tgrow_HIGrow"]
    name=d["plant"]["sensorname"]
    yaml_data = read_yaml_file(mac_id,  name)

    if yaml_data == False:
        print("New device discovered; sending discovery topics")
        send_discovery_topics(msg)
    else:
        print("Device already sent to discovery; skipping")
        
    print("Completed parse of message on " + msg.topic)

# Initialize MQTT client connection
client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

host = os.environ.get("MQTT_HOST")
if host is None:
  print("No MQTT_HOST environment variable specified; using default")
  host = "192.168.1.64"

username = os.environ.get("MQTT_USER")
password = os.environ.get("MQTT_PASS")
if username is None or password is None:
  print("No MQTT_USER/MQTT_PASS environment variables specified; connecting without authentication")
else:
  client.username_pw_set(username, password)

client.connect(host, 1883, 60)

client.loop_forever()
