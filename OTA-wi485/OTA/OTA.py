from http.server import HTTPServer, BaseHTTPRequestHandler
from urllib.parse import unquote
import os
import threading
import paho.mqtt.client as mqtt
import json
import time
import socket

PORT = 1577
BASE_DIR = os.path.dirname(os.path.abspath(__file__))
FIRMWARE_DIR = os.path.join(BASE_DIR, "FILE")

MQTT_BROKER = "27.105.113.156"
MQTT_PORT = 1883
MQTT_TOPIC = "wi485update"
MQTT_USERNAME = "test"
MQTT_PASSWORD = "test123"


class OTARequestHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        # 解碼 URL
        decoded_path = unquote(self.path)
        
        # 如果是根目錄，列出所有檔案
        if decoded_path == '/' or decoded_path == '':
            self.list_directory()
            return
        
        # 取得檔案名稱
        filename = decoded_path.lstrip('/')
        filepath = os.path.join(FIRMWARE_DIR, filename)
        
        print(f" Request: {decoded_path}")
        print(f" Looking for: {filepath}")
        
        # 檢查檔案是否存在
        if not os.path.exists(filepath):
            self.send_error(404, f"File not found: {filename}")
            return
        
        # 讀取並傳送檔案
        try:
            with open(filepath, 'rb') as f:
                self.send_response(200)
                self.send_header('Content-Type', 'application/octet-stream')
                self.send_header('Content-Length', str(os.path.getsize(filepath)))
                self.end_headers()
                self.wfile.write(f.read())
            print(f" Sent: {filename}")
        except Exception as e:
            print(f" Error: {e}")
            self.send_error(500, str(e))
    
    def list_directory(self):
        """列出 FIRMWARE_DIR 中的所有檔案"""
        try:
            files = os.listdir(FIRMWARE_DIR)
            self.send_response(200)
            self.send_header('Content-Type', 'text/html; charset=utf-8')
            self.end_headers()
            
            html = f'<html><head><title>Directory listing for /</title></head><body><h1>Directory listing for /</h1><hr><ul>'
            for f in files:
                html += f'<li><a href="/{f}">{f}</a></li>'
            html += '</ul><hr></body></html>'
            
            self.wfile.write(html.encode('utf-8'))
        except Exception as e:
            self.send_error(500, str(e))


def get_local_ip():
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        s.connect(('8.8.8.8', 80))
        ip = s.getsockname()[0]
        s.close()
    except:
        ip = '127.0.0.1'
    return ip


def on_message(client, userdata, msg):
    print(f"\n RECEIVE MQTT topic={msg.topic} payload={msg.payload.decode()}")
    try:
        data = json.loads(msg.payload.decode())
        meter_id = data.get("meterID")
        cmd_mode = data.get("cmd_Mode", "")
        stm32_ver = data.get("STM32_Ver", "")

        files = [f for f in os.listdir(FIRMWARE_DIR) if f.endswith(".bin")]
        if not files:
            print(" NO OTA FILE")
            return

        file_name = files[0]
        ota_url = f"http://27.105.113.156:1577/{file_name}"
        print(f" OTA_URL：{ota_url}")

        ver_index = file_name.find("Stv") + 3
        update_ver = int(file_name[ver_index:ver_index+6].replace(".", ""), 10) if ver_index >= 3 else 999999
        try:
            current_ver = int(stm32_ver, 16) if stm32_ver else 0
        except:
            current_ver = 0

        print(f" STM32 {current_ver}，更新版本 {update_ver}")
        if cmd_mode == "update" and update_ver != current_ver:
            size_start = file_name.find("size") + 4
            crc_start = file_name.find("crc") + 3
            size = file_name[size_start:size_start+4]
            crc = file_name[crc_start:crc_start+4]

            config_msg = {
                "meterID": meter_id,
                "ssid": "",
                "wifi_pwd": "",
                "host": "",
                "user": "",
                "mqtt_pwd": "",
                "mode": "4",
                "ota_url": "",
                "http_url": ota_url,
                "checksum": crc,
                "ota_size": size
            }

            target_topic = f"eMeterConfigSet/{meter_id}"
            client.publish(target_topic, json.dumps(config_msg))
            print(f" 已發送到 {target_topic}")
        else:
            print("⏭ NO UPDATE")

    except Exception as e:
        print(" ERROR：", e)


def start_mqtt():
    client = mqtt.Client()
    client.username_pw_set(MQTT_USERNAME, MQTT_PASSWORD)
    client.on_message = on_message
    client.connect(MQTT_BROKER, MQTT_PORT, 60)
    client.subscribe(MQTT_TOPIC)
    print(f" MQTT 監聽 {MQTT_TOPIC}")
    client.loop_forever()
    


def start_http():
    httpd = HTTPServer(('0.0.0.0', PORT), OTARequestHandler)
    local_ip = get_local_ip()
    print(f" HTTP Server running at http://{local_ip}:{PORT}/")
    print(f" Serving from: {FIRMWARE_DIR}")
    print(f" Files: {os.listdir(FIRMWARE_DIR)}")
    httpd.serve_forever()


def publish_ota():
    client = mqtt.Client()
    client.username_pw_set(MQTT_USERNAME, MQTT_PASSWORD)
    client.connect(MQTT_BROKER, MQTT_PORT, 60)
    client.loop_start()
    

    files = [f for f in os.listdir(FIRMWARE_DIR) if f.endswith(".bin")]
    if not files:
        print(" No .bin file in FILE/")
        return

    latest = sorted(files)[-1]
    ota_url = f"http://{get_local_ip()}:{PORT}/{latest}"
    payload = {"url": ota_url}

    print(f" Publishing OTA URL: {ota_url}")
    client.publish("wi485update", json.dumps(payload))
    print(" OTA command sent")
    time.sleep(1)
    client.loop_stop()


if __name__ == "__main__":
    # 先印出除錯資訊
    print(f" FIRMWARE_DIR = {FIRMWARE_DIR}")
    print(f" 存在: {os.path.exists(FIRMWARE_DIR)}")
    if os.path.exists(FIRMWARE_DIR):
        print(f" 檔案: {os.listdir(FIRMWARE_DIR)}")
    
    threading.Thread(target=start_http, daemon=True).start()
    time.sleep(2)
    publish_ota()
    
    print(" Server is running. Press Ctrl+C to stop.")
    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        print("\n Server stopped")