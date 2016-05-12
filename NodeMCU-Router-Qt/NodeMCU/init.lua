print('Hello NodeMCU !')

lighton=0
	led=4
	
	gpio.write(led, gpio.HIGH)
	tmr.alarm(0,1000,1,function()
		if lighton==0 then
			lighton=1
			gpio.mode(led, gpio.OUTPUT)
			gpio.write(led, gpio.LOW)
		else
			lighton=0
			gpio.write(led, gpio.HIGH)
		end
	end)
	gpio.mode(led, gpio.INPUT)

dofile("telnetd.lua")
wifi.setmode(wifi.STATION)
print('wifi init...')

tmr.alarm(2,5000,1,function()
		local ip = wifi.sta.getip()

		if ip == nil then
	    	print("ip == nil")
		else
			print(ip)
			dofile("send.lua")
			tmr.stop(2)
		end

	end)





