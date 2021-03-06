-- define SSID credentials
SSID = "ScreamBananaForPassword"
SSID_PASSWORD = "dingdong"

-- define mongoDB API key
mongoAPI_key = "265NHXlP3tObb374O6Sn9D4TkcqJEy_0"

-- mongoDB REST URLs
mongo_base_path = "https://api.mlab.com/api/1"

-- Relay IP
relay_ip = "http://52.38.18.190"
relay_test = "/test"
relay_insert = "/api/db/insert"
relay_retrieve = "/api/db/get"
relay_call = "/api/call"
relay_achieve = "/api/db/insert"
relay_get = "/api/db/get"


-- configure ESP as a station
tmr.delay(3000000)
wifi.setmode(wifi.STATION)
wifi.sta.config(SSID,SSID_PASSWORD)
wifi.sta.autoconnect(1)
gpio.mode(3, gpio.OUTPUT)
gpio.write(3, gpio.HIGH)

-- pause for connection to take place - adjust time delay if necessary or repeat until connection made
print("Waiting to establish wifi...")
tmr.delay(5000000) -- wait 1,000,000 us = 1 second
status = wifi.sta.status()
print(status)

count = 0
while (status ~= 5 and count <= 5)
do
  tmr.delay(3000000)
  print("Attempting to reconnect... "..count)
  status = wifi.sta.status()
  count = count + 1
end

if (count <= 5) then
  print("Connected Successfully")
  gpio.write(3, gpio.LOW)
else
  print("Could not find connection")
  gpio.write(3, gpio.HIGH)
end

-- This function registers a function to echo back any response from the server, to our DE1/NIOS system
-- or hyper-terminal (depending on what the dongle is connected to)
function display(sck,response)
     print(response)
end

function testGet()
  print(relay_ip..relay_test)
  http.get(relay_ip..relay_test, nil, function(code,data)
    if (code < 0) then
      print("FAILED")
    else
      print(code, data)
    end
  end)
end

function testPost()
  print("Attempting to POST to: "..relay_ip..relay_insert)
  sampleTable = genTable("0:00","30:00","slat","slong","elat","elong","150","5")
  encodedTable = cjson.encode(sampleTable)
  --insert_info(encodedTable)
  http.post(relay_ip..relay_insert, 'Content-Type: application/json\r\n', encodedTable, function(code,data)
    if (code < 0) then
      print("FAILED")
    else
      print(code, data)
    end
  end)
end

function insertDB(start_time, time_elapsed, state_latitude, start_longitude, end_latitude, end_longitude, total_distance, speed)
  print("Attempting to POST to: "..relay_ip..relay_insert)
  sampleTable = genTable(start_time, time_elapsed, state_latitude, start_longitude, end_latitude, end_longitude, total_distance, speed)
  encodedTable = cjson.encode(sampleTable)
  http.post(relay_ip..relay_insert, 'Content-Type: application/json\r\n', encodedTable, function(code,data)
    if (code < 0) then
      print("FAILED")
    else
      print(code, data)
    end
  end)
end
-- functions

function genTable(start_time, time_elapsed, start_latitude, start_longitude, end_latitude, end_longitude, total_distance, speed)
  table = {}
  table["start_time"] = start_time
  table["end_time"] = time_elapsed
  table["start_lat"] = start_latitude
  table["start_long"] = start_longitude
  table["end_lat"] = end_latitude
  table["end_long"] = end_longitude
  table["total_distance"] = total_distance
  table["speed"] = speed
  print(table)
  return table
end

function twilioCall(numberToCall)
  print("Attempting to POST to: "..relay_ip..relay_call)
  table = {}
  table["to"] = numberToCall
  encodedTable = cjson.encode(table);
  http.post(relay_ip..relay_call, 'Content-Type: application/json\r\n', encodedTable, function(code,data)
    if (code < 0) then
      print(code)
    else
      print(code, data)
    end
  end)
end

function InsertAchieve(id, dist1, dist2, ses1, ses2, speed1, speed2, numSessions, achieve_radius)
  print("Attempting to POST to: "..relay_ip..relay_achieve)
  table = {}
  table["_id"] = id
  table["distance1"] = dist1
  table["distance2"] = dist2
  table["session1"] = ses1
  table["session2"] = ses2
  table["speed1"] = speed1
  table["speed2"] = speed2
  table["numSessions"] = numSessions
  table["achievementRadius"] = achieve_radius
  encodedTable = cjson.encode(table);
  http.post(relay_ip..relay_achieve, 'Content-Type: application/json\r\n', encodedTable, function(code,data)
    if (code < 0) then
      print(code)
    else
      print(code, data)
    end
  end)
end

function getID(id)
  print("Attempting to POST to: "..relay_ip..relay_get)
  table = {}
  table["id"] = id
  encodedTable = cjson.encode(table)
  print(encodedTable)
  http.post(relay_ip..relay_get, 'Content-Type: application/json\r\n', encodedTable, function(code,data)
    if (code < 0) then
      print("FAILED")
    else
      print(code, data)
    end
  end)
end
