local ip 

file.open("ip.lua","r")
ip = file.readline()
print(ip)

cu=net.createConnection(net.UDP)
cu:on("receive",function(cu,c) print(c) end)
cu:connect(8888,ip)

-- uart
uart.setup(0,38400,8,0,1,0)
uart.on("data",
function(data)
	cu:send(data)
end, 0)
