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
        'name':  d['plant']['sensor_name'],
        'sw_version':  d['plant']['release_version']
    }
    entity_payloads = {


        'Tgrow_HIGrow': {
            'name': f"{d['plant']['sensor_name']} Mac ID",
            'unit_of_meas': "",
            'enabled_by_default': 'false',
        },
        'sensor_name': {
            'name': f"{d['plant']['sensor_name']} Sensor Name",
            'unit_of_meas': "",
            'enabled_by_default': 'false',
        },
        'last_updated': {
            'name': f"{d['plant']['sensor_name']} Last Updated",
            'unit_of_meas': "",
            'device_class': 'timestamp',
            'frc_upd':True,
        },
        'valve_number': {
            'name': f"{d['plant']['sensor_name']} Valve Number",
            'unit_of_meas': "", 
            'enabled_by_default': 'false',
        }, 


        'lux': {
            'name': f"{d['plant']['sensor_name']} Lux",
            'unit_of_meas': "lx", 
            'device_class': 'illuminance',
        }, 
        'temperature': {
            'name': f"{d['plant']['sensor_name']} Temperature",
            'unit_of_meas': "°C",
            'device_class': 'temperature',
        }, 
        'humidity': {
            'name': f"{d['plant']['sensor_name']} Humidity",
            'unit_of_meas': "%", 
            'device_class': 'humidity',
        }, 
        'pressure': {
            'name': f"{d['plant']['sensor_name']} Pressure",
            'unit_of_meas': "hPa",
            'device_class': 'pressure',
        },


        # Skipped as uninteresting: raw_soil_moisture
        'soil_moisture': {
            'name': f"{d['plant']['sensor_name']} Soil Moisture",
            'unit_of_meas': "%", 
            'device_class': 'humidity',
        }, 
        'soil_temperature': {
            'name': f"{d['plant']['sensor_name']} Soil Temperature",
            'unit_of_meas': "°C",
            'device_class': 'temperature',
        },
        # Skipped as uninteresting: raw_soil_conductivity
        'soil_conductivity': {
            'name': f"{d['plant']['sensor_name']} Soil Conductivity",
            'unit_of_meas': "μS/cm",
            'icon':'mdi:bottle-tonic'
        },
        'salt_advice': {
            'name': f"{d['plant']['sensor_name']} Fertilizer State",
            'unit_of_meas': "", 
            'icon':'mdi:emoticon-poop'
        },


        # Skipped as uninteresting: raw_battery_voltage
        'battery_voltage': {
            'name': f"{d['plant']['sensor_name']} Battery Voltage",
            'unit_of_meas': "V",
            'device_class': 'battery',
            'icon':'mdi:flash'
        },
        'battery_percentage': {
            'name': f"{d['plant']['sensor_name']} Battery",
            'unit_of_meas': "%", 
            'device_class': 'battery',
        }, 
        'battery_charge_state': {
            'name': f"{d['plant']['sensor_name']} Charge State",
            'unit_of_meas': "", 
            'icon':'mdi:battery'
        }, 
        'battery_last_charged': {
            'name': f"{d['plant']['sensor_name']} Last Charged",
            'unit_of_meas': "", 
            'device_class': 'timestamp',
        }, 


        'boot_count': {
            'name': f"{d['plant']['sensor_name']} Boot Count",
            'unit_of_meas': "", 
            'icon':'mdi:counter',
            'enabled_by_default': 'false',
        }, 
        'sleep_5_count': {
            'name': f"{d['plant']['sensor_name']} Sleep5 Count",
            'unit_of_meas': "", 
            'icon':'mdi:counter',
            'enabled_by_default': 'false',
        }, 
        'release_version': {
            'name': f"{d['plant']['sensor_name']} Release Version",
            'unit_of_meas': "", 
            'icon':'mdi:counter',
            'enabled_by_default': 'false',
        }, 


        'wifi_ssid': {
            'name': f"{d['plant']['sensor_name']} WiFi SSID",
            'unit_of_meas': "", 
            'icon':'mdi:wifi',
            'enabled_by_default': 'false',
        }, 
        'wifi_rssi': {
            'name': f"{d['plant']['sensor_name']} WiFi Signal Strength",
            'unit_of_meas': "dBm",
            'device_class': 'signal_strength',
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
    try:
      name=d["plant"]["sensor_name"]
    except KeyError:
      print("Device with MAC ID %s using legacy format, skipping" % (mac_id, ))
      return
      
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
